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

#pragma once

#define NUM_OSC 8
#define MAX_POLYPHONY 16

const float osc_volume_defaults[NUM_OSC] = {0.8, 0.4, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0};
const float adsr_defaults[] = {0.0, 0.0, 1.0, 0.1};

// the frequency of each oscillator for a given note is the fundamental frequency multiplied by the corresponding value in interval_ratios[].
// these defaults, all being integers (besides the semantic ".0"), correspond with frequencies in the harmonic series.
// 5, 7, 9, and 11 however are skipped, mimicking the first 8 drawbars of the typical Hammond organ.
// Unlike the Hammond organ, the first oscillator is the fundamental by default.
const float interval_defaults[NUM_OSC] = {1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0};
