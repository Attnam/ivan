/*
    midiplayback : SMF MIDI File player suitable for 8 - 32bit targets
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



#ifndef MIDI_PLAYBACK_H
#define MIDI_PLAYBACK_H

#include "midiparser.h"
#include <stdint.h>

#define MPB_EVENT_STACK_SIZE (16)
#define MPB_DEFAULT_TEMPO (120)
#define MPB_DEFAULT_PPQ (96)

#define MPB_NULL_ON_ARRAY (255)
#define MPB_DEFAULT_NOTE_ON_DIVISOR (8)
//Continue Playing Status
enum
{
    MPB_FILE_STILL_PLAYING = 0,
    MPB_FILE_FINISHED,
    
};

//Playback Modes
typedef enum
{
    MPB_PB_ALL_ON = 0,
    MPB_PB_NO_VOL,
    MPB_PB_NO_NOTES, //no notes and no lyrics too
    MPB_PB_SAVE_MIDI_STATUS,
    MPB_PB_ALL_OFF,
} MIDI_PB_MODE;




enum
{
    MPB_TRACK_STOPPED = (0),
    MPB_NEW_DATA,
    MPB_PLAY_NEXT_EVENT,
    MPB_QUEUED_EVENT,
    MPB_REPOSITION_EVENT,
};


extern void SendMIDIEvent(MIDI_CHAN_EVENT_t* event);

uint8_t MPB_PlayMIDIFile(MIDI_HEADER_CHUNK_t* MIDIHdr, char* filename);
void    MPB_ResetMIDITracks(MIDI_HEADER_CHUNK_t* MIDIHdr);

uint8_t MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_PB_MODE mode);
uint8_t MPB_AddEvent(MIDI_EVENT_t* event);
void MPB_ProcessMetaEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event);
void MPB_PlayEvent(MIDI_EVENT_t* event, MIDI_PB_MODE mode);
void MPB_OutputMIDIChanEvent(MIDI_CHAN_EVENT_t* chanEvent);

void MPB_SetTranspose(MIDI_HEADER_CHUNK_t* MIDIHdr, int8_t transpose);
void MPB_ApplyTranspose(MIDI_EVENT_t* event, int8_t transpose);


uint16_t MPB_SetTickRate(uint16_t BPM, uint16_t PPQ);
void MPB_ResetMIDI(void);
void MPB_StopAllSounds(void);
void MPB_ProcessGenericEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, MIDI_PB_MODE mode);

void MPB_ReplayStatusBuffer(void);
void MPB_SaveMIDIStatus(MIDI_CHAN_EVENT_t* chanEvent);

uint8_t MPB_FastFwd_ToEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode, MIDI_CHAN_EVENT_t* event, MPB_FF_MODE_t ffMode);
void MPB_FastFwd_TestEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_EVENT_t* event);

uint8_t MPB_RePositionTime(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t timePosSec, MIDI_PB_MODE mode);
uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode);
uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_PB_MODE mode);

void MPB_DetermineMIDIFileStats(MIDI_HEADER_CHUNK_t* MIDIHdr);
uint16_t MPB_CurrentTimePosition(MIDI_HEADER_CHUNK_t* MIDIHdr);
uint16_t MPB_CurrentBarPosition(MIDI_HEADER_CHUNK_t* MIDIHdr);

void   _MIDI_readbuf(uint8_t* position, uint8_t* buf, uint16_t size);
uint8_t _MIDI_fileopen(char* filename);
void _MIDI_fileclose(void);

#define MPB_ReadToBuffer(position, buf) _MIDI_readbuf(position, buf, MIDI_TRACK_BUFFER_SIZE)
#define MPB_OpenFile(x) _MIDI_fileopen(x)
#define MPB_CloseFile() _MIDI_fileclose()

//Define in hardwareSpecific.h
//#define MIDI_Tx(x)          uartTx(PrimaryUART, x)
//#define MIXI_TxDump(x, n)   uartTxDump(PrimaryUART, x, n)

MIDI_EVENT_t* MPB_GetNextEvent(void);
MIDI_EVENT_t* MPB_ConfirmEventTx(void);


void MPB_EnablePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr);
void MPB_PausePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr);
void MPB_TogglePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr);
void MPB_SetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr, MidiPlaybackState_t state);
MidiPlaybackState_t MPB_GetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr);

//Private Functions for StatusBuffer control
void _addEventToBuffer(MIDI_CHAN_EVENT_t* chanEvent);
MIDI_CHAN_EVENT_t* _getEventFromBuffer(uint8_t channel, uint16_t index);
void _clearEventBuffer(void);
MIDI_CHAN_EVENT_t* _findChanEventInBuffer(uint8_t firstByte, uint8_t secondByte);


#endif



#ifdef __cplusplus
}
#endif


