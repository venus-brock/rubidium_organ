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

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "defaults.h"

namespace brock {

class CRubidiumProcessor : public Steinberg::Vst::AudioEffect
{
public:
    CRubidiumProcessor ();
    ~CRubidiumProcessor () SMTG_OVERRIDE;

    static Steinberg::FUnknown* createInstance (void* /*context*/) 
    { 
        return (Steinberg::Vst::IAudioProcessor*)new CRubidiumProcessor; 
    }

    Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
    
    Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
    
    Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

    Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
    
    Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

    Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
        
    Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

protected:
    float osc_volume[NUM_OSC];
    float phase[NUM_OSC][MAX_POLYPHONY];
    float fund_freq[MAX_POLYPHONY];
    float interval_ratios[NUM_OSC];

    // 0 - attack
    // 1 - decay
    // 2 - sustain
    // 3 - release
    // -1 - oscillator is not currently active for this note
    int adsr_stage[NUM_OSC][MAX_POLYPHONY];

    float attack[NUM_OSC];
    float decay[NUM_OSC];
    float sustain[NUM_OSC];
    float release[NUM_OSC];
    float envelope_volume[NUM_OSC][MAX_POLYPHONY];
    float global_volume = 0.1;
    float delta_angle[MAX_POLYPHONY];
    bool note_on[MAX_POLYPHONY];
};

#define PI2 (3.14159256f * 2.f)

}
