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


#include "midiparser.h"
#include "MIDIDebug.h"
#include <string.h>


PROGRAM_CHAR MIDI_NOTE_OFF_STRING[] PROGRAM_SPACE = "NOTE OFF";
PROGRAM_CHAR MIDI_NOTE_ON_STRING[] PROGRAM_SPACE = "NOTE ON";
PROGRAM_CHAR MIDI_POLY_STRING[] PROGRAM_SPACE = "AFTRTCH";
PROGRAM_CHAR MIDI_CONTROL_CHANGE_STRING[] PROGRAM_SPACE = "CONT.CHNG";
PROGRAM_CHAR MIDI_PROGRAM_CHANGE_STRING[] PROGRAM_SPACE = "PROG.CHNG";
PROGRAM_CHAR MIDI_CHANNEL_PRESSURE_STRING[] PROGRAM_SPACE = "CHAN.PRES";
PROGRAM_CHAR MIDI_PITCH_WHEEL_STRING[] PROGRAM_SPACE = "PITCH W.";
PROGRAM_CHAR MIDI_SYSEX_STRING[] PROGRAM_SPACE = "SYSEX";

PROGRAM_CHAR MIDI_TIME_CODE_STRING[] PROGRAM_SPACE = "TIMECODE";
PROGRAM_CHAR MIDI_SONG_POS_STRING[] PROGRAM_SPACE = "SONG.POS";
PROGRAM_CHAR MIDI_SONG_SEL_STRING[] PROGRAM_SPACE = "SONG.SEL";

PROGRAM_CHAR MIDI_TUNE_REQUEST_STRING[] PROGRAM_SPACE = "TUNE REQ";
PROGRAM_CHAR MIDI_END_SYSEX_STRING[] PROGRAM_SPACE = "END SYSX";

PROGRAM_CHAR MIDI_TIMING_CLOCK_STRING[] PROGRAM_SPACE = "CLOCK";
PROGRAM_CHAR MIDI_SEQ_START_STRING[] PROGRAM_SPACE = "START";
PROGRAM_CHAR MIDI_SEQ_CONTINUE_STRING[] PROGRAM_SPACE = "CONTIN.";
PROGRAM_CHAR MIDI_SEQ_STOP_STRING[] PROGRAM_SPACE = "STOP";

PROGRAM_CHAR MIDI_ACTIVE_SENSING_STRING[] PROGRAM_SPACE = "ACT.SENS";
PROGRAM_CHAR MIDI_RESET_STRING[] PROGRAM_SPACE = "RESET";

PROGRAM_CHAR MIDI_UNDEFINED_STRING[] PROGRAM_SPACE = "UNDEF";


const MidiLookup_t MidiLookUpTable[] PROGRAM_SPACE = {
    {MIDI_NOTE_ON, MIDI_NOTE_ON_STRING, 3},
    {MIDI_NOTE_OFF, MIDI_NOTE_OFF_STRING, 3},
    {MIDI_AFTERTOUCH, MIDI_POLY_STRING, 3},
    {MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE_STRING, 3},
    {MIDI_PROGRAM_CHANGE, MIDI_PROGRAM_CHANGE_STRING, 2},
    {MIDI_CHANNEL_PRESSURE, MIDI_CHANNEL_PRESSURE_STRING, 2},
    {MIDI_PITCH_CHANGE, MIDI_PITCH_WHEEL_STRING, 3},
    {MIDI_SYSEX_START, MIDI_SYSEX_STRING, 3},
    {MIDI_TIME_CODE, MIDI_TIME_CODE_STRING, 2},
    {MIDI_SONG_POSITION, MIDI_SONG_POS_STRING, 3},
    {MIDI_SONG_SELECT, MIDI_SONG_SEL_STRING, 2},
    {MIDI_TUNE_REQUEST, MIDI_TUNE_REQUEST_STRING, 1},
    {MIDI_SYSEX_STOP, MIDI_END_SYSEX_STRING, 1},
    {MIDI_TIMING_CLOCK, MIDI_TIMING_CLOCK_STRING, 1},
    {MIDI_RT_TICK, MIDI_UNDEFINED_STRING, 1},
    {MIDI_RT_START, MIDI_SEQ_START_STRING, 1},
    {MIDI_RT_CONTINUE, MIDI_SEQ_CONTINUE_STRING, 1},
    {MIDI_RT_STOP, MIDI_SEQ_STOP_STRING, 1},
    {MIDI_RT_ACTIVE_SENSE, MIDI_ACTIVE_SENSING_STRING, 1},
    {MIDI_RT_RESET, MIDI_RESET_STRING, 1},
    {0, 0}
};

PROGRAM_CHAR MIDI_CIRCLE5TH0[] PROGRAM_SPACE = "F";
PROGRAM_CHAR MIDI_CIRCLE5TH1[] PROGRAM_SPACE = "C";
PROGRAM_CHAR MIDI_CIRCLE5TH2[] PROGRAM_SPACE = "G";
PROGRAM_CHAR MIDI_CIRCLE5TH3[] PROGRAM_SPACE = "D";
PROGRAM_CHAR MIDI_CIRCLE5TH4[] PROGRAM_SPACE = "A";
PROGRAM_CHAR MIDI_CIRCLE5TH5[] PROGRAM_SPACE = "E";
PROGRAM_CHAR MIDI_CIRCLE5TH6[] PROGRAM_SPACE = "B";

const char* MIDI_CIRCLE5TH[] PROGRAM_SPACE = {
    MIDI_CIRCLE5TH0,
    MIDI_CIRCLE5TH1,
    MIDI_CIRCLE5TH2,
    MIDI_CIRCLE5TH3,
    MIDI_CIRCLE5TH4,
    MIDI_CIRCLE5TH5,
    MIDI_CIRCLE5TH6,
};

PROGRAM_CHAR MIDI_ACCIDENT0[] PROGRAM_SPACE = "b";
PROGRAM_CHAR MIDI_ACCIDENT1[] PROGRAM_SPACE = "#";

const char* MIDI_ACCIDENT[] PROGRAM_SPACE = {
    MIDI_ACCIDENT0,
    MIDI_ACCIDENT1,
};



char* MIDIParse_KeySignature(int8_t keySig, uint8_t keyScale)
{
    static char returnValue[4];
    uint8_t uKeySig = MIDI_MAJOR_SCALE_OFFSET;
    switch(keyScale)
    {
        case MINOR_KEY:
            uKeySig = MIDI_MINOR_SCALE_OFFSET;
        case MAJOR_KEY:
            uKeySig = uKeySig + MIDI_CIRCLE_ELEMENTS + keySig;
            if( uKeySig < MIDI_CIRCLE_ELEMENTS)
            {
                strcpy(returnValue, MIDI_CIRCLE5TH[uKeySig]);
                strcat(returnValue, MIDI_ACCIDENT[0]);
            }
            else if( uKeySig >= 2*MIDI_CIRCLE_ELEMENTS)
            {
                strcpy(returnValue, MIDI_CIRCLE5TH[uKeySig%MIDI_CIRCLE_ELEMENTS]);
                strcat(returnValue, MIDI_ACCIDENT[1]);
            }
            else
            {
                strcpy(returnValue, MIDI_CIRCLE5TH[uKeySig-MIDI_CIRCLE_ELEMENTS]);
            }
            break;            
        default:
            break;
    }
    return returnValue;
}

size_t MIDIParse_Header(MIDI_HEADER_CHUNK_t* header, void* data, uint32_t size)
{
    uint8_t* ptr = (uint8_t*)data;
    ptr = (uint8_t*)findSubString( (char*)data, (char*)MIDI_HEADER_STRING, MIDI_TRACK_BUFFER_SIZE);

    if (ptr)
    {
        uint16_t* tmp;
        tmp = (uint16_t*)&ptr[MIDI_HEADER_FMT];
        reverseOrder( (char*)tmp, 2);
        header->format = *tmp;

        tmp = (uint16_t*)&ptr[MIDI_HEADER_TRACK_COUNT];
        reverseOrder((char*)tmp, 2);
        header->trackCount = *tmp;

        tmp = (uint16_t*)&ptr[MIDI_HEADER_PPQ];
        reverseOrder((char*)tmp, 2);
        header->PPQ = *tmp;

        uint8_t* diff = ptr;
        diff = (uint8_t*)((uint8_t*)diff - (uint8_t*)(data));

        return  (size_t)(diff);
    }

    return 0;
}


/* Example */
//    position = position + MIDI_HEADER_SIZE;
//    for (i = 0; i<MIDIHdr.trackCount; i++)
//    {
//        f_lseek(&testFIL, position);
//        f_read(&testFIL, buffer, BUFFER_READ_SIZE, &br);
//        position = MIDIPopulate_HeaderTrack(&MIDIHdr, i, position, &buffer[0], BUFFER_READ_SIZE);
//    }
/* Returns the position of the MIDI file that the top level should read to */
uint8_t* MIDIPopulate_HeaderTrack(MIDI_HEADER_CHUNK_t* header, uint8_t trackNo, uint8_t* filePos, void* data, uint32_t size)
{
    header->Track[trackNo].startPtr = (uint8_t*)(filePos + MIDI_TRACK_LENGTH + 4);
    MIDIParse_Track(&header->Track[trackNo], data, size);
    return (uint8_t*)(filePos + MIDI_TRACK_HEADER_SIZE + header->Track[trackNo].length);
}

//Returns the pointer to the first byte of the track data
void* MIDIParse_Track(MIDI_TRACK_CHUNK_t* track, void* data, uint32_t size)
{
    char* ptr = (char*)findSubString( (char*)data, (char*)MIDI_TRACK_STRING, size);
    if (ptr)
    {
        uint32_t* tmp;
        tmp = (uint32_t*)&ptr[MIDI_TRACK_LENGTH];
        reverseOrder( (char*)tmp, 4);
        track->length = *tmp;
        //MIDI_Printf("TRACKLENG: ", track->length);
        return &ptr[MIDI_TRACK_LENGTH+4];
    }

    return 0;
}

void* MIDIParse_Event(MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, uint8_t* data)
{
    uint32_t byteOffset = 0;
    uint8_t bytesToSend;
    //uint8_t runningStatus;
    
    if( ((event->event.eventType & 0xF0) >= MIDI_NOTE_OFF) &&
        ((event->event.eventType & 0xF0) < MIDI_SYSEX_START))
    {
        track->runningStatus = event->event.eventType;
    }

    byteOffset = midiparse_variableLength(data, &event->deltaTime);
    event->event.eventType = data[byteOffset];
    //for running status
    if (event->event.eventType<MIDI_NOTE_OFF)
    {
        event->event.eventType = track->runningStatus;
        byteOffset--;
    }

    if (event->event.eventType==MIDI_SYSEX_START)
    {
        midiparse_variableLength(&data[byteOffset+1], &event->event.sysExEvent.length);
        
        event->event.sysExEvent.data = &data[byteOffset+2];
        //rawDump(event->event.sysExEvent.data, event->event.sysExEvent.length);
        return &data[byteOffset+2+event->event.sysExEvent.length];
    }

    if (event->event.eventType==MIDI_META_MSG)
    {
        event->event.metaEvent.metaType = data[byteOffset+1];
        
        midiparse_variableLength(&data[byteOffset+2], &event->event.metaEvent.length);
        
        event->event.metaEvent.data = &data[byteOffset+3];
        //MIDI_Printf("DATA: %s\n",event.event->metaEvent.data);
        //rawDumpStr(event.event->metaEvent.data, event.event->metaEvent.length);
        return &data[byteOffset+3+event->event.metaEvent.length];
    }




    bytesToSend = MIDI_CommandSize(event->event.eventType&0xF0);
    if (bytesToSend>1)
    {
        //runningStatus = event->event.eventType;

        event->event.chanEvent.parameter1 = data[byteOffset+1];
        if (bytesToSend>2)
        {
            event->event.chanEvent.parameter2 = data[byteOffset+2];
        }
    }

    return &data[byteOffset+bytesToSend];

}

void MIDI_PrintEventInfo(MIDI_EVENT_t* event)
{
    MIDI_Printf("DT: ", event->deltaTime);
    MIDI_Printf("ET: ", event->event.eventType);

    switch (event->event.eventType)
    {
        case MIDI_SYSEX_START:
            MIDI_Printf("LEN: ", event->event.sysExEvent.length);
            break;
            
        case MIDI_META_MSG:
            MIDI_Printf("TYPE: ", event->event.metaEvent.metaType);
            MIDI_Printf("LEN: ", event->event.metaEvent.length);
            MIDI_Printf("DATA0: ", event->event.metaEvent.data[0]);
            MIDI_Printf("DATA1: ", event->event.metaEvent.data[1]);
            MIDI_Printf("DATA2: ", event->event.metaEvent.data[2]);
            break;

        default:
            MIDI_Printf("P1: ", event->event.chanEvent.parameter1);
            MIDI_Printf("P2: ", event->event.chanEvent.parameter2);
            break;
    }
}

/* Obtain the control code size */
uint8_t MIDI_CommandSize(uint8_t command)
{
    uint8_t i;
    for (i = 0; FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_Commands); i++)
    {
        if (command==FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_Commands))
        {
            return FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_MsgSize);
        }
    }
    return 1;
}


//Returns the length of the variable length field

uint8_t midiparse_variableLength(uint8_t* data, uint32_t* retValue)
{
    uint32_t value = 0;
    uint8_t i;
    for (i = 0; i<4; i++)
    {
        value = value+(data[i] & 0x7F);
        if (data[i] & 0x80)
        {
            value = value<<7;
        }
        else
        {
            break;
        }
    }
    *retValue = value;
    return i+1;
}

void reverseOrder(char* array, uint8_t len)
{
    uint8_t i;
    char tempByte;
    for (i = 0; i<len/2; i++)
    {
        tempByte = array[i];
        array[i] = array[len-i-1];
        array[len-i-1] = tempByte;
    }
}

void* findSubString(char* data, const char* string, uint32_t numBytes)
{
    uint32_t i;
    for (i = 0; i<numBytes; i++)
    {
        char* ptr = 0;
        if (data[i]==string[0])
        {
            ptr = strstr(&data[i], string);
            if (ptr)
            {
                return ptr;
            }
        }
    }
    return 0;
}















