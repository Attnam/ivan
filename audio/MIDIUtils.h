/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/


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
