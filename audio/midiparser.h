/*
    MIDIParser : SMF MIDI File parser suitable for 8 - 32bit targets
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




#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H


#include <stdint.h>
#include <stddef.h>
#include "MIDICodes.h"

#define MIDI_HEADER_STRING   "MThd"
#define MIDI_TRACK_STRING    "MTrk"

//MIDI Header / Track byte offsets
#define MIDI_HEADER_FMT          (0x08)
#define MIDI_HEADER_TRACK_COUNT  (0x0A)
#define MIDI_HEADER_PPQ          (0x0C)

#define MIDI_HEADER_SIZE         (0x0E)
#define MIDI_TRACK_HEADER_SIZE   (0x08)
#define MIDI_TRACK_LENGTH        (0x04)

#define MIDI_META_MSG            (0xFF)
#define MIDI_META_SEQUENCE       (0x00)
#define MIDI_META_TEXT           (0x01)
#define MIDI_META_COPY_WRITE     (0x02)
#define MIDI_META_TRACK_NAME     (0x03)
#define MIDI_META_INSTNAME       (0x04)
#define MIDI_META_LYRICS         (0x05)
#define MIDI_META_MARKER         (0x06)
#define MIDI_META_CUEPOINT       (0x07)
#define MIDI_META_CHANPREFIX     (0x20)
#define MIDI_META_TRACKEND       (0x2F)
#define MIDI_META_TEMPO          (0x51)
#define MIDI_META_SMPTE          (0x54)
#define MIDI_META_TIME_SIG       (0x58)
#define MIDI_META_KEY_SIG        (0x59)
#define MIDI_META_SEQ_SPEC       (0x7F)

#define US_PER_MINUTE            (60000000)
#define BPM(x)                   (US_PER_MINUTE/(x))

#define MIDI_TRACK_BUFFER_SIZE   (32)
#define MAX_MIDI_TRACKS          (50)

#define TRACK_MAX_NAME_LENGTH    (32)
#define TRACK_EVENT_BUFFER_SIZE  (2)



#define PROGRAM_CHAR const char
#define PROGRAM_SPACE
#define FLASH_GET_PGM_BYTE

typedef enum
{
    STATE_INACTIVE = 0,
    STATE_ACTIVE = 1,
    STATE_FAST_FWD,
    STATE_TRACK_BUILDING,
} MidiPlaybackState_t;



enum
{
    MAJOR_KEY = 0,
    MINOR_KEY,
};

#define MIDI_MAJOR_SCALE_OFFSET (1)
#define MIDI_MINOR_SCALE_OFFSET (4)
#define MIDI_CIRCLE_ELEMENTS    (7)

typedef struct {

   uint8_t MIDI_Commands;
   PROGRAM_CHAR* MIDI_CommandString;
   uint8_t MIDI_MsgSize;

} MidiLookup_t;


typedef struct
{
	uint8_t   eventType;
    uint8_t   parameter1;
    uint8_t   parameter2;   
} MIDI_CHAN_EVENT_t;

typedef struct
{
	uint8_t   eventType;
    uint32_t  length;
    uint8_t*  data;   
} MIDI_SYSEX_EVENT_t;

typedef struct
{
	uint8_t   eventType;
    uint8_t   metaType;
    uint32_t  length;
    uint8_t*  data;   
} MIDI_META_EVENT_t;

typedef struct
{
    uint32_t  deltaTime;
    union
    {
    	uint8_t   eventType;
    	MIDI_CHAN_EVENT_t  chanEvent;
    	MIDI_SYSEX_EVENT_t sysExEvent;
       	MIDI_META_EVENT_t  metaEvent;
    } event;
    
} MIDI_EVENT_t;


typedef struct
{
    uint8_t* startPtr;
    uint32_t length;
    uint32_t position;
    uint32_t trackClock;
    MIDI_EVENT_t trackEvent;
    uint8_t  eventCount;
    uint8_t  trackIndex;
    uint8_t  bufferOffset;
    uint8_t  runningStatus;
    uint8_t  name[TRACK_MAX_NAME_LENGTH];
    uint8_t  buffer[MIDI_TRACK_BUFFER_SIZE];
    
} MIDI_TRACK_CHUNK_t;

typedef struct
{
    uint32_t maxLength;
    uint32_t tickTime;
    uint32_t lastTempoChange;
    uint32_t BPM;
    uint32_t trackLengthSecs;
    uint16_t trackState; //represents which tracks are active
    uint8_t  timeSignature; //BCD 4bit. (high4=numerator)
    int8_t   keySignature; //BCD 4bit. high4=major/minor, low4=-7 -> +7
    uint8_t  keyScale;
} MIDI_CURRENT_TRACK_STATE_t;

#define FAST_FWD_ACTIVE	(0x1)
#define FAST_FWD_STATUS_MASK (0x07)

#define FAST_FWD_IGNORE_CHANNEL_MASK		(0x80)

#define FAST_FWD_IGNORE_PARAM1		(0xFF)
#define FAST_FWD_IGNORE_PARAM2		(0xFF)
#define FAST_FWD_NON_ZERO_PARAM2	(FAST_FWD_IGNORE_PARAM2-1)

typedef enum {
   FAST_FW_FIND_NULL = 0,
	FAST_FWD_FIND_COMMAND = 1,
	FAST_FWD_FIND_PARAM1,
	FAST_FWD_FIND_PARAM2,
} MPB_FF_MODE_t;

typedef struct
{
	MPB_FF_MODE_t foundEventStatus :3; //MPB_FF_MODE_t type
	MPB_FF_MODE_t searchMode :3; //MPB_FF_MODE_t type
	uint8_t foundEventFlag	 :2;
} MPB_FastFwd_t;

typedef struct
{
	//Number of notes per track
	uint16_t noteCount;
	//Even with multiple program changes, it records the first one
	uint8_t	 programNumber;
} MIDI_CHANNEL_STATS_t;

#define MIDI_MAX_FILENAME (32)
#define MIDI_MAX_POLYPHONY (32)
typedef struct
{
    uint16_t format;
    uint16_t trackCount;
    uint16_t PPQ;
    uint32_t masterClock;
    //uint32_t maxLength;
    uint8_t  fileName[MIDI_MAX_FILENAME];
    MIDI_TRACK_CHUNK_t Track[MAX_MIDI_TRACKS];
    MIDI_CURRENT_TRACK_STATE_t currentState;

    //This is for the drum track.
    //Once a drum is hit, it will stay on for a set period of time.
    uint16_t  NoteOnTimer[MIDI_MAX_KEY+1];
    
    //TimeOn = PPQ / NoteOnDivisor.
    uint16_t  NoteOnTime;
    //Just for the drum track.
    uint8_t  NoteOnList[MIDI_MAX_POLYPHONY];
    uint8_t  channelPolyphony[MIDI_MAX_CHANNELS];
    uint8_t  totalPolyphony;

    MidiPlaybackState_t playbackState;
    int8_t transpose;

    MPB_FastFwd_t	 FastFwd_Status;
    MIDI_CHAN_EVENT_t FastFwd_Event;

    uint16_t channelStateBitmap;
    MIDI_CHANNEL_STATS_t channelStats[MIDI_MAX_CHANNELS];

} MIDI_HEADER_CHUNK_t;




void rawDump(uint8_t* data, uint16_t len);
void rawDumpStr(uint8_t* data, uint16_t len);


char* MIDIParse_KeySignature(int8_t keySig, uint8_t keyScale);

size_t MIDIParse_Header(MIDI_HEADER_CHUNK_t* header, void* data, uint32_t size);
uint8_t* MIDIPopulate_HeaderTrack(MIDI_HEADER_CHUNK_t* header, uint8_t trackNo, uint8_t* filePos, void* data, uint32_t size);
void* MIDIParse_Track(MIDI_TRACK_CHUNK_t* track, void* data, uint32_t size);
void* MIDIParse_Event(MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, uint8_t* data);

void MIDI_PrintEventInfo(MIDI_EVENT_t* event);

uint8_t MIDI_CommandSize(uint8_t command);
//Returns the length of the variable length field
uint8_t midiparse_variableLength(uint8_t* data, uint32_t* retValue);


void reverseOrder(char* array, uint8_t len);
void* findSubString(char* data, const char* string, uint32_t numBytes);






#endif



#ifdef __cplusplus
}
#endif
