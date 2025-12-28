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
*    Contact me at venus@brock-v.dev
*/

#include "rbd_processor.h"
#include "rbd_controller.h"
#include "rbd_cids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "Rubidium"

using namespace Steinberg::Vst;
using namespace brock;

BEGIN_FACTORY_DEF ("Venus Brock", 
                   "https://github.com/venus-brock", 
                   "mailto:venus@brock-v.dev")

    DEF_CLASS2 (INLINE_UID_FROM_FUID(kCRubidiumProcessorUID),
                PClassInfo::kManyInstances,             // cardinality
                kVstAudioEffectClass,                   // the component category (do not changed this)
                stringPluginName,                       // here the Plug-in name (to be changed)
                Vst::kDistributable,                    // means that component and controller could be distributed on different computers
                CRubidiumVST3Category,                  // Subcategory for this Plug-in (to be changed)
                FULL_VERSION_STR,                       // Plug-in version (to be changed)
                kVstVersionString,                      // the VST 3 SDK version (do not changed this, use always this define)
                CRubidiumProcessor::createInstance)     // function pointer called when this component should be instantiated

    // its kVstComponentControllerClass component
    DEF_CLASS2 (INLINE_UID_FROM_FUID (kCRubidiumControllerUID),
                PClassInfo::kManyInstances,             // cardinality
                kVstComponentControllerClass,           // the Controller category (do not changed this)
                stringPluginName "Controller",          // controller name (could be the same than component name)
                0,                                      // not used here
                "",                                     // not used here
                FULL_VERSION_STR,                       // Plug-in version (to be changed)
                kVstVersionString,                      // the VST 3 SDK version (do not changed this, use always this define)
                CRubidiumController::createInstance)    // function pointer called when this component should be instantiated

    //----for others Plug-ins contained in this factory, put like for the first Plug-in different DEF_CLASS2---

END_FACTORY
