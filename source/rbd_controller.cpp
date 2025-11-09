/*	Rubidium Organ - A simple additive synthesizer using VST3
*	Copyright (C) 2025 Venus Brock
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
*	 Contact me at venus_brock@venusmusic.ca
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
	char16_t param_names[40][10] = {
		u"osc_1", u"osc_2", u"osc_3", u"osc_4", u"osc_5", u"osc_6", u"osc_7", u"osc_8",
		u"osc_1_a", u"osc_2_a", u"osc_3_a", u"osc_4_a", u"osc_5_a", u"osc_6_a", u"osc_7_a", u"osc_8_a",
		u"osc_1_d", u"osc_2_d", u"osc_3_d", u"osc_4_d", u"osc_5_d", u"osc_6_d", u"osc_7_d", u"osc_8_d",
		u"osc_1_s", u"osc_2_s", u"osc_3_s", u"osc_4_s", u"osc_5_s", u"osc_6_s", u"osc_7_s", u"osc_8_s",
		u"osc_1_r", u"osc_2_r", u"osc_3_r", u"osc_4_r", u"osc_5_r", u"osc_6_r", u"osc_7_r", u"osc_8_r"
	};

	float default_osc_volumes[NUM_OSC] = {
	    default_osc_1, default_osc_2, default_osc_3, default_osc_4, default_osc_5, default_osc_6, default_osc_7, default_osc_8
	};

	for(int i = 0; i < NUM_OSC; i++){
		parameters.addParameter(param_names[i], nullptr, 0, default_osc_volumes[i], Vst::ParameterInfo::kCanAutomate, i);
	}

	for(int i = 0; i < NUM_OSC; i++){
		parameters.addParameter(param_names[i + NUM_OSC], nullptr, 0, default_a, Vst::ParameterInfo::kCanAutomate, i + NUM_OSC);
	}

	for(int i = 0; i < NUM_OSC; i++){
		parameters.addParameter(param_names[i + 2 * NUM_OSC], nullptr, 0, default_d, Vst::ParameterInfo::kCanAutomate, i + 2 * NUM_OSC);
	}

	for(int i = 0; i < NUM_OSC; i++){
		parameters.addParameter(param_names[i + 3 * NUM_OSC], nullptr, 0, default_s, Vst::ParameterInfo::kCanAutomate, i + 3 * NUM_OSC);
	}

	for(int i = 0; i < NUM_OSC; i++){
		parameters.addParameter(param_names[i + 4 * NUM_OSC], nullptr, 0, default_r, Vst::ParameterInfo::kCanAutomate, i + 4 * NUM_OSC);
	}

	return result;
}

tresult PLUGIN_API CRubidiumController::terminate ()
{
	return EditControllerEx1::terminate ();
}

tresult PLUGIN_API CRubidiumController::setComponentState (IBStream* state)
{	
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
