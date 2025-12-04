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

#include "rbd_controller.h"
#include "rbd_cids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginterfaces/base/ibstream.h"
#include "base/source/fstreamer.h"
#include "defaults.h"

using namespace Steinberg;

namespace brock {

tresult PLUGIN_API CRubidiumController::initialize (FUnknown* context)
{
    tresult result = EditControllerEx1::initialize (context);
    if (result != kResultOk)
    {
        return result;
    }

    // apparently we need to use utf-16
    char16_t param_names[48][10] = {
        u"osc_1", u"osc_2", u"osc_3", u"osc_4", u"osc_5", u"osc_6", u"osc_7", u"osc_8",
        u"osc_1_a", u"osc_2_a", u"osc_3_a", u"osc_4_a", u"osc_5_a", u"osc_6_a", u"osc_7_a", u"osc_8_a",
        u"osc_1_d", u"osc_2_d", u"osc_3_d", u"osc_4_d", u"osc_5_d", u"osc_6_d", u"osc_7_d", u"osc_8_d",
        u"osc_1_s", u"osc_2_s", u"osc_3_s", u"osc_4_s", u"osc_5_s", u"osc_6_s", u"osc_7_s", u"osc_8_s",
        u"osc_1_r", u"osc_2_r", u"osc_3_r", u"osc_4_r", u"osc_5_r", u"osc_6_r", u"osc_7_r", u"osc_8_r",
        u"osc_1_i", u"osc_2_i", u"osc_3_i", u"osc_4_i", u"osc_5_i", u"osc_6_i", u"osc_7_i", u"osc_8_i"
    };

    for(int i = 0; i < NUM_OSC; i++){
        parameters.addParameter(param_names[i], nullptr, 0, osc_volume_defaults[i], Vst::ParameterInfo::kCanAutomate, i);
    }

    for(int i = 0; i < NUM_OSC * 4; i++){
        parameters.addParameter(param_names[i + NUM_OSC], nullptr, 0, adsr_defaults[i / NUM_OSC], Vst::ParameterInfo::kCanAutomate, i + NUM_OSC);
    }

    for(int i = 0; i < NUM_OSC; i++){
        // all vst3 parameters are normalised, which I can't help but think is retarded. the default value for this parameter is set to the reciprocal of the actual number that we want. this also means that an oscillator's pitch cannot be lower than the fundamental. thanks obama
        parameters.addParameter(param_names[i + NUM_OSC * 5], nullptr, 0, 1 / interval_defaults[i], Vst::ParameterInfo::kCanAutomate, i + NUM_OSC * 5);
    }
    return result;
}

tresult PLUGIN_API CRubidiumController::terminate ()
{
    return EditControllerEx1::terminate ();
}

tresult PLUGIN_API CRubidiumController::setComponentState (IBStream* state)
{    
    if(!state){
        return kResultFalse;
    }

    IBStreamer streamer(state, kLittleEndian);
    for(int i = 0; i < NUM_OSC; i++){
        float val = 0;
        for(int j = 0; j < 5; j++){
            if(streamer.readFloat(val) == false){
                return kResultFalse;
            }
            if(auto param = parameters.getParameter(i + j * NUM_OSC)){
                param->setNormalized(val);
            }
        }
    }
    return kResultOk;
}

tresult PLUGIN_API CRubidiumController::setState (IBStream* state)
{
    return kResultTrue;
}

tresult PLUGIN_API CRubidiumController::getState (IBStream* state)
{
        return kResultTrue;
}

IPlugView* PLUGIN_API CRubidiumController::createView (FIDString name)
{
    if(strcmp(name, Vst::ViewType::kEditor) == 0){
        return new VSTGUI::VST3Editor(this, "view", "rbd_editor.uidesc");
    }
    return 0;
}

}
