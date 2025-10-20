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

#pragma once

#include "pluginterfaces/base/fplatform.h"

#include "projectversion.h"

#define stringOriginalFilename	"rubidium.vst3"
#if SMTG_PLATFORM_64
#define stringFileDescription	"Rubidium VST3 (64Bit)"
#else
#define stringFileDescription	"Rubidium VST3"
#endif
#define stringCompanyName		"Venus Brock\0"
#define stringLegalCopyright	"Copyright(c) 2025 Venus Brock."
#define stringLegalTrademarks	"VST is a trademark of Steinberg Media Technologies GmbH"
