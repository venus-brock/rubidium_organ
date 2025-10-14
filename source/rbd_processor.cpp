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
		}
		note_held[i] = false;
		in_release[i] = false;
	}
	for(int i = 0; i < 10; i++){
		attack[i] = 0.1;
		release[i] = 0.1;
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
	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				paramQueue->getPoint(numPoints - 1, sampleOffset, value);
				switch (paramQueue->getParameterId ())
				{
				case kOsc1:
					osc_volume[0] = (float)value;
					break;
				case kOsc2:
					osc_volume[1] = (float)value;
					break;
				case kOsc3:
					osc_volume[2] = (float)value;
					break;
				case kOsc4:
					osc_volume[3] = (float)value;
					break;
				case kOsc5:
					osc_volume[4] = (float)value;
					break;
				case kOsc6:
					osc_volume[5] = (float)value;
					break;
				case kOsc7:
					osc_volume[6] = (float)value;
					break;
				case kOsc8:
					osc_volume[7] = (float)value;
					break;
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
						if(!note_held[i] && !in_release[i]){
							note_held[i] = true;
							fund_freq[i] = 440.0f * powf(2.0f, (float)(event.noteOn.pitch - 69) / 12.f);
							delta_angle[i] = PI2 * fund_freq[i] / data.processContext->sampleRate;
							fVolume = 0.3f;
							for(int j = 0; j < NUM_OSC; j++)
								phase[j][i] = 0.f;
							break;
						}
					}
					break;
				case Vst::Event::kNoteOffEvent:
					for(int i = 0; i < MAX_POLYPHONY; i++){
						if(note_held[i] && fund_freq[i] == 440.f * powf(2.0f, (float)(event.noteOff.pitch - 69) / 12.f)){
							note_held[i] = false;
							in_release[i] = true;
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
			if(note_held[i] || in_release[i]){
				for(int j = 0; j < NUM_OSC; j++){
					temp += envelope_volume[j][i] * sin(phase[j][i]);
					phase[j][i] += delta_angle[i] * interval_ratios[j];
					while(phase[j][i] > PI2){
						phase[j][i] -= PI2;
					}
					if(in_release[i]){
						envelope_volume[j][i] -= osc_volume[j] / (release[j] * data.processContext->sampleRate);
						if(envelope_volume[j][i] <= 0){
							envelope_volume[j][i] = 0;
							if(j == NUM_OSC - 1){
								in_release[i] = false;
								for(int k = 0; k < NUM_OSC; k++){
									if(envelope_volume[k][i] > 0)
										in_release[i] = true;
								}
							}
						}
					}
					else if(envelope_volume[j][i] < osc_volume[j]){
						envelope_volume[j][i] += osc_volume[j] / (attack[j] * data.processContext->sampleRate);
						if(envelope_volume[j][i] > osc_volume[j])
							envelope_volume[j][i] = osc_volume[j];
					}
				}
			}
		}
		temp *= fVolume;
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

	return kResultOk;
}

tresult PLUGIN_API CRubidiumProcessor::getState (IBStream* state)
{
	return kResultOk;
}

}
