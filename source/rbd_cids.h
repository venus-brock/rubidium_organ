#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace brock {
static const Steinberg::FUID kCRubidiumProcessorUID (0x33E23552, 0x96985690, 0xB55039E2, 0xE27F182C);
static const Steinberg::FUID kCRubidiumControllerUID (0x1B95854F, 0xFEF75EB3, 0x9D28D0FF, 0x769498AB);

#define CRubidiumVST3Category "synth"
}
