/*
    MIDIUtils : Utilities for looking up MIDI names, keys and octaves
    Copyright (c) 2004-2016 Adrian M. Gin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v2 as published by
    the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

*/

#ifdef __cplusplus
extern "C" {
#endif



#ifndef _MIDI_UTILS_
#define _MIDI_UTILS_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


uint8_t MIDIUtils_GetMusicNote(uint8_t note);
/* Returns the octave of the passed note */
uint8_t MIDIUtils_GetOctave(uint8_t note);
const char* MIDIUtils_GetInstrumentName(uint8_t programNumber);


#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/



#ifdef __cplusplus
}
#endif
