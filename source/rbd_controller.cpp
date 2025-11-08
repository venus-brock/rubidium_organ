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
#include "params.h"

using namespace Steinberg;

namespace brock {

tresult PLUGIN_API CRubidiumController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	parameters.addParameter(STR16("OSC1"), nullptr, 0, default_osc_1, Vst::ParameterInfo::kCanAutomate, osc_1);
	parameters.addParameter(STR16("OSC2"), nullptr, 0, default_osc_2, Vst::ParameterInfo::kCanAutomate, osc_2);
	parameters.addParameter(STR16("OSC3"), nullptr, 0, default_osc_3, Vst::ParameterInfo::kCanAutomate, osc_3);
	parameters.addParameter(STR16("OSC4"), nullptr, 0, default_osc_4, Vst::ParameterInfo::kCanAutomate, osc_4);
	parameters.addParameter(STR16("OSC5"), nullptr, 0, default_osc_5, Vst::ParameterInfo::kCanAutomate, osc_5);
	parameters.addParameter(STR16("OSC6"), nullptr, 0, default_osc_6, Vst::ParameterInfo::kCanAutomate, osc_6);
	parameters.addParameter(STR16("OSC7"), nullptr, 0, default_osc_7, Vst::ParameterInfo::kCanAutomate, osc_7);
	parameters.addParameter(STR16("OSC8"), nullptr, 0, default_osc_8, Vst::ParameterInfo::kCanAutomate, osc_8);
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
	return nullptr;
}

}
