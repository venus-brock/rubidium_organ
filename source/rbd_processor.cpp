/*   Rubidium Organ - A simple additive synthesizer using VST3
*   Copyright (C) 2025 Venus Brock
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
*    Contact me at venus_brock@venusmusic.ca
*/

#include "rbd_processor.h"
#include "rbd_cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

#include <cmath>

using namespace Steinberg;

namespace brock {
CRubidiumProcessor::CRubidiumProcessor ()
{
    setControllerClass (kCRubidiumControllerUID);


    // Initialize arrays
    for(int i = 0; i < MAX_POLYPHONY; i++){
        for(int j = 0; j < NUM_OSC; j++){
            phase[j][i] = 0.f;
            envelope_volume[j][i] = 0.f;
            adsr_stage[j][i] = -1;
        }
        note_on[i] = false;
    }
    for(int i = 0; i < NUM_OSC; i++){
        attack[i] = pow(default_a, 2);
        decay[i] = pow(default_d, 2);
        sustain[i] = default_s;
        release[i] = pow(default_r, 2);
    }
}

CRubidiumProcessor::~CRubidiumProcessor ()
{}

tresult PLUGIN_API CRubidiumProcessor::initialize (FUnknown* context)
{
    tresult result = AudioEffect::initialize (context);
    if (result != kResultOk)
    {
        return result;
    }

    //--- create Audio IO ------
    // addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
    addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

    addEventInput (STR16 ("Event In"), 1);

    return kResultOk;
}

tresult PLUGIN_API CRubidiumProcessor::terminate ()
{
    return AudioEffect::terminate ();
}

tresult PLUGIN_API CRubidiumProcessor::setActive (TBool state)
{
    return AudioEffect::setActive (state);
}

tresult PLUGIN_API CRubidiumProcessor::process (Vst::ProcessData& data)
{
    if(data.inputParameterChanges){
        int32 num_params = data.inputParameterChanges->getParameterCount();
        for(int i = 0; i < num_params; i++){
            if(auto* param_queue = data.inputParameterChanges->getParameterData(i)){
                Vst::ParamValue value;
                int32 sample_offset;
                int32 num_points = param_queue->getPointCount();
                param_queue->getPoint(num_points - 1, sample_offset, value);
                int param_id = param_queue->getParameterId();
                if(param_id < NUM_OSC){
                    osc_volume[param_id] = (float)value;
                } else if(param_id < 2 * NUM_OSC){
                    attack[param_id - NUM_OSC] = pow((float)value, 2);
                } else if(param_id < 3 * NUM_OSC){
                    decay[param_id - 2 * NUM_OSC] = pow((float)value, 2);
                } else if(param_id < 4 * NUM_OSC){
                    sustain[param_id - 3 * NUM_OSC] = (float)value;
                } else{
                    release[param_id - 4 * NUM_OSC] = pow((float)value, 2);
                }
            }
        }
    }
    Vst::IEventList* events = data.inputEvents;
    if(events != NULL){
        int32 numEvent = events->getEventCount();
        for(int32 i = 0; i < numEvent; i++){
            Vst::Event event;
            if(events->getEvent(i, event) == kResultOk){
                switch(event.type){
                case Vst::Event::kNoteOnEvent:
                    for(int i = 0; i < MAX_POLYPHONY; i++){
                        if(!note_on[i]){
                            note_on[i] = true;
                            fund_freq[i] = 440.0f * powf(2.0f, (float)(event.noteOn.pitch - 69) / 12.f);
                            delta_angle[i] = PI2 * fund_freq[i] / data.processContext->sampleRate;
                            for(int j = 0; j < NUM_OSC; j++){
                                phase[j][i] = 0.f;
                                adsr_stage[j][i] = 0;
                            }
                            break;
                        }
                    }
                    break;
                case Vst::Event::kNoteOffEvent:
                    for(int i = 0; i < MAX_POLYPHONY; i++){
                        if(adsr_stage[0][i] != -1 && adsr_stage[0][i] != 3 && fund_freq[i] == 440.f * powf(2.0f, (float)(event.noteOff.pitch - 69) / 12.f)){
                            for(int j = 0; j < NUM_OSC; j++)
                                adsr_stage[j][i] = 3;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    if(data.numSamples == 0)
        return kResultOk;

    int num_channels = data.outputs[0].numChannels;
    
    void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);

    Vst::Sample32* ptr_out_l = (Vst::Sample32*)out[0];
    Vst::Sample32* ptr_out_r = NULL;
    if(num_channels > 1)
        ptr_out_r = (Vst::Sample32*)out[1];
    int32 samples = data.numSamples;
    
    while(--samples >= 0){        
        Vst::Sample32 temp = 0.f;
        for(int i = 0; i < MAX_POLYPHONY; i++){
            if(note_on[i]){
                for(int j = 0; j < NUM_OSC; j++){
                    temp += envelope_volume[j][i] * sin(phase[j][i]);
                    phase[j][i] += delta_angle[i] * interval_ratios[j];
                    while(phase[j][i] > PI2){
                        phase[j][i] -= PI2;
                    }
                    switch(adsr_stage[j][i]){
                    case 0: // attack
                        if(attack[j] == 0 || envelope_volume[j][i] >= osc_volume[j]){
                            envelope_volume[j][i] = osc_volume[j];
                            adsr_stage[j][i]++;
                            break;
                        }
                        envelope_volume[j][i] += osc_volume[j] / (attack[j] * data.processContext->sampleRate);
                        break;
                    case 1: // decay
                        if(decay[j] == 0 || envelope_volume[j][i] <= osc_volume[j] * sustain[j]){
                            envelope_volume[j][i] = osc_volume[j] * sustain[j];
                            adsr_stage[j][i]++;
                            break;
                        }
                        envelope_volume[j][i] -= (osc_volume[j] - osc_volume[j] * sustain[j]) / (decay[j] * data.processContext->sampleRate);
                        break;
                    case 2: // sustain
                        envelope_volume[j][i] = osc_volume[j] * sustain[j];
                        break;
                    case 3: // release
                        if(release[j] == 0 || envelope_volume[j][i] <= 0){
                            envelope_volume[j][i] = 0;
                            adsr_stage[j][i] = -1;
                            note_on[i] = false;
                            for(int k = 0; k < NUM_OSC; k++){
                                if(adsr_stage[k][i] != -1)
                                    note_on[i] = true;
                            }
                            break;
                        }
                        envelope_volume[j][i] -= (osc_volume[j] * sustain[j]) / (release[j] * data.processContext->sampleRate);
                        break;
                    }
                    if(envelope_volume[j][i] > osc_volume[j]){
                        // the envelope volume for any index should NEVER be greater than the relevant oscillator volume
                        // this fixes issues that may occur when oscillator volumes are changed during the run time of a note
                        envelope_volume[j][i] = osc_volume[j];
                    }
                }
            }
        }
        temp *= global_volume;
        (*ptr_out_l++) = temp;
        if(num_channels > 1)
            (*ptr_out_r++) = temp;
    }
    return kResultOk;
}

tresult PLUGIN_API CRubidiumProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
    return AudioEffect::setupProcessing (newSetup);
}

tresult PLUGIN_API CRubidiumProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
    if (symbolicSampleSize == Vst::kSample32)
        return kResultTrue;

    return kResultFalse;
}

tresult PLUGIN_API CRubidiumProcessor::setState (IBStream* state)
{
    if(!state){
        return kResultFalse;
    }

    IBStreamer streamer(state, kLittleEndian);
    for(int i = 0; i < NUM_OSC; i++){
        float param = 0;
        if(streamer.readFloat(param) == false){
            return kResultFalse;
        }
        osc_volume[i] = param;
        if(streamer.readFloat(param) == false){
            return kResultFalse;
        }
        attack[i] = pow(param, 2);
        if(streamer.readFloat(param) == false){
            return kResultFalse;
        }
        decay[i] = pow(param, 2);
        if(streamer.readFloat(param) == false){
            return kResultFalse;
        }
        sustain[i] = param;
        if(streamer.readFloat(param) == false){
            return kResultFalse;
        }
        release[i] = pow(param, 2);
    }
    return kResultOk;
}

tresult PLUGIN_API CRubidiumProcessor::getState (IBStream* state)
{
    IBStreamer streamer(state, kLittleEndian);
    for(int i = 0; i < NUM_OSC; i++){
        streamer.writeFloat(osc_volume[i]);
        streamer.writeFloat(sqrt(attack[i]));
        streamer.writeFloat(sqrt(decay[i]));
        streamer.writeFloat(sustain[i]);
        streamer.writeFloat(sqrt(release[i]));
    }
    return kResultOk;
}

}
