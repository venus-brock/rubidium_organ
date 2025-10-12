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

	parameters.addParameter(STR16("OSC1"), nullptr, 0, default_Osc1, Vst::ParameterInfo::kCanAutomate, kOsc1);
	parameters.addParameter(STR16("OSC2"), nullptr, 0, default_Osc2, Vst::ParameterInfo::kCanAutomate, kOsc2);
	parameters.addParameter(STR16("OSC3"), nullptr, 0, default_Osc3, Vst::ParameterInfo::kCanAutomate, kOsc3);
	parameters.addParameter(STR16("OSC4"), nullptr, 0, default_Osc4, Vst::ParameterInfo::kCanAutomate, kOsc4);
	parameters.addParameter(STR16("OSC5"), nullptr, 0, default_Osc5, Vst::ParameterInfo::kCanAutomate, kOsc5);
	parameters.addParameter(STR16("OSC6"), nullptr, 0, default_Osc6, Vst::ParameterInfo::kCanAutomate, kOsc6);
	parameters.addParameter(STR16("OSC7"), nullptr, 0, default_Osc7, Vst::ParameterInfo::kCanAutomate, kOsc7);
	parameters.addParameter(STR16("OSC8"), nullptr, 0, default_Osc8, Vst::ParameterInfo::kCanAutomate, kOsc8);
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
