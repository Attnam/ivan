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



#include "midiplayback.h"
#include "midiparser.h"
#include <stdint.h>
#include "stack.h"
#include "linkedlist.h"


#include "MIDICodes.h"
#include "MIDIDebug.h"
#include "MIDIUtils.h"

#include <stdio.h>
#include <string.h>

void* MPBarray[sizeof(void*) * MPB_EVENT_STACK_SIZE];
STACK_t MPBStack = { MPBarray, 0, 0, MPB_EVENT_STACK_SIZE };

FILE* midifile;

void MIDI_Tx(uint8_t byte)
{
   // printf("0x%X, \n", byte);
}

void MPB_ResetMIDI(void)
{

   /*MIDI_Tx(MIDI_SYSEX_START);
    MIDI_Tx(0x7E);
    MIDI_Tx(0x7F);
    MIDI_Tx(0x09);
    MIDI_Tx(0x01);
    MIDI_Tx(MIDI_SYSEX_STOP);*/

   MPB_StopAllSounds();

   _clearEventBuffer();
}

void MPB_StopAllSounds(void)
{
   MIDI_EVENT_t event;
   uint8_t i;
   for (i = 0; i < MIDI_MAX_CHANNELS; i++)
   {
      event.event.eventType = MIDI_CONTROL_CHANGE | i;
      event.event.chanEvent.parameter1 = ALL_NOTES_OFF;
      event.event.chanEvent.parameter2 = 0;
      MPB_PlayEvent(&event, MPB_PB_ALL_ON);

      event.event.chanEvent.parameter1 = SUSTAIN;
      event.event.chanEvent.parameter2 = 0x00;
      MPB_PlayEvent(&event, MPB_PB_ALL_ON);
   }
}

//returns the number of seconds.
uint16_t MPB_CurrentTimePosition(MIDI_HEADER_CHUNK_t* MIDIHdr)
{

   uint32_t currentPosition = MIDIHdr->currentState.tickTime + ((uint32_t) ((MIDIHdr->masterClock - MIDIHdr->currentState.lastTempoChange) * 60) / MIDIHdr->currentState.BPM);
   currentPosition = currentPosition / MIDIHdr->PPQ;
   return currentPosition;
}

uint16_t MPB_CurrentBarPosition(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   return (MIDIHdr->masterClock / MIDIHdr->PPQ) >> 2;
}

//Finds out which MIDI channels are used in the MIDI file
//So later the FindEvent can be used to find all the On_Notes
//for a particular channel.
//We determine the instrument based off the MIDI_PROGRAM_CHANGE bytes.
//Should also keep track of number of Notes for each track.
void MPB_BuildChannelStats(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_EVENT_t* event)
{
   if ((event->event.eventType & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON && (event->event.chanEvent.parameter2 != 0))
   {
      uint8_t channel;
      channel = event->event.eventType & MIDI_CHANNEL_MASK;
      MIDIHdr->channelStateBitmap |= (1 << channel);
      MIDIHdr->channelStats[channel].noteCount++;
   }

   if ((event->event.eventType & MIDI_MSG_TYPE_MASK) == MIDI_PROGRAM_CHANGE)
   {
      uint8_t channel;
      channel = event->event.eventType & MIDI_CHANNEL_MASK;
      MIDIHdr->channelStats[channel].programNumber = event->event.chanEvent.parameter1;
   }

}

void MPB_DetermineMIDIFileStats(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   uint8_t i;
   uint32_t* maxLength;

   //Determine the MIDI channel constitution.
   MPB_SetPlaybackState(MIDIHdr, STATE_TRACK_BUILDING);

   MIDIHdr->masterClock = 0xFFFFFFFF;
   while (MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_OFF) != MPB_FILE_FINISHED)
   {
      if (MIDIHdr->trackCount == 0)
      {
         break;
      }
   }
   maxLength = &MIDIHdr->currentState.maxLength;
   for (i = 0; i < MIDIHdr->trackCount; i++)
   {
      if (MIDIHdr->Track[i].trackClock > *maxLength)
      {
         *maxLength = MIDIHdr->Track[i].trackClock;
      }
   }

   if (MIDIHdr->currentState.BPM == 0)
   {
      MIDIHdr->currentState.BPM = MPB_DEFAULT_TEMPO;
   }

   if (MIDIHdr->PPQ == 0)
   {
      MIDIHdr->PPQ = MPB_DEFAULT_PPQ;
   }

   MIDIHdr->NoteOnTime = MIDIHdr->PPQ / 8;

   MIDIHdr->currentState.tickTime = MIDIHdr->currentState.tickTime + ((uint32_t) ((*maxLength - MIDIHdr->currentState.lastTempoChange) * 60) / MIDIHdr->currentState.BPM);

   MIDIHdr->currentState.trackLengthSecs = MIDIHdr->currentState.tickTime / MIDIHdr->PPQ;
   MIDI_Printf("Min: ", MIDIHdr->currentState.trackLengthSecs / 60);
   MIDI_Printf("Sec: ", MIDIHdr->currentState.trackLengthSecs % 60);
   MIDI_Printf("MaxLength: ", *maxLength >> 16);
   MIDI_Printf("MaxLength: ", *maxLength);

   for (i = 0; i < MIDI_MAX_CHANNELS; i++)
   {
      if (MIDIHdr->channelStateBitmap & (1 << i))
      {
         MIDI_Printf("Channel %d::ACTIVE\n", i);
         //MIDI_Printf("Patch::%s\n", MIDIUtils_GetInstrumentName(MIDIHdr->channelStats[i].programNumber));
         MIDI_Printf("NoteCount %d::\n", MIDIHdr->channelStats[i].noteCount);
      }
      else
      {
         MIDI_Printf("Channel %d::DISABLED\n", i);
      }
   }

}

uint8_t MPB_PlayMIDIFile(MIDI_HEADER_CHUNK_t* MIDIHdr, char* filename)
{
   uint8_t ret;
   ret = MPB_OpenFile(filename);
   if (ret)
   {
      return ret;
   }

   memset(MIDIHdr, 0, sizeof(MIDI_HEADER_CHUNK_t));
   MPB_ResetMIDITracks(MIDIHdr);
   //Need to reset the MIDI Tracks twice, one for Initialisation
   //Another time because determining file stats requires a complete
   //run through the file.
   MPB_DetermineMIDIFileStats(MIDIHdr);
   MPB_ResetMIDITracks(MIDIHdr);
   MPB_SetPlaybackState(MIDIHdr, STATE_ACTIVE);

   return 0;
}

void MPB_EnablePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   MIDIHdr->playbackState = STATE_ACTIVE;
}

void MPB_PausePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   MIDIHdr->playbackState = STATE_INACTIVE;
}

void MPB_SetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr, MidiPlaybackState_t state)
{
   MIDIHdr->playbackState = state;
}

MidiPlaybackState_t MPB_GetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   return MIDIHdr->playbackState;
}

void MPB_TogglePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   if( MIDIHdr->playbackState == STATE_INACTIVE )
   {
      MIDIHdr->playbackState = STATE_ACTIVE;
   }
   else if( MIDIHdr->playbackState == STATE_ACTIVE )
   {
      MIDIHdr->playbackState = STATE_INACTIVE;
   }

}

void MPB_ResetMIDITracks(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
   uint8_t i;
   uint8_t* position = 0;
   uint8_t* buf;
   uint8_t* ptr;

   buf = (uint8_t*) &MIDIHdr->Track[0].buffer;
   MPB_ReadToBuffer(position, buf);

   ptr = (uint8_t*)findSubString((char*) buf, MIDI_HEADER_STRING, MIDI_TRACK_BUFFER_SIZE);
   if (ptr)
   {
      if ((ptr - buf + MIDI_HEADER_SIZE) > MIDI_TRACK_BUFFER_SIZE)
      {
         MPB_ReadToBuffer( (uint8_t*)(ptr - buf), buf);
         position = (uint8_t*)(ptr - buf);
         ptr = buf;
      }
   }

   position = position + MIDIParse_Header(MIDIHdr, buf, MIDI_TRACK_BUFFER_SIZE);
   position = position + MIDI_HEADER_SIZE;

   for (i = 0; i < MIDIHdr->trackCount; i++)
   {
      MPB_ReadToBuffer( position, buf);
      position = (uint8_t*)MIDIPopulate_HeaderTrack(MIDIHdr, i, position, buf, MIDI_TRACK_BUFFER_SIZE);
      MIDIHdr->Track[i].eventCount = MPB_NEW_DATA;
      MIDIHdr->Track[i].trackIndex = i;
      MIDIHdr->Track[i].trackClock = 0;
   }

   MIDIHdr->currentState.trackState = 0;
}

uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode)
{
   MPB_ResetMIDITracks(MIDIHdr);
   MIDIHdr->masterClock = position;
   return MPB_ContinuePlay(MIDIHdr, mode);
}

//Finds the next 'MIDI Command' from the given position.
//
uint8_t MPB_FastFwd_ToEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode, MIDI_CHAN_EVENT_t* event, MPB_FF_MODE_t ffMode)
{
   MPB_RePosition(MIDIHdr, position, MPB_PB_ALL_OFF);

   MIDIHdr->FastFwd_Status.foundEventStatus = FAST_FW_FIND_NULL;
   MIDIHdr->FastFwd_Status.foundEventFlag = FAST_FWD_ACTIVE;
   MIDIHdr->FastFwd_Status.searchMode = ffMode;

   MIDIHdr->FastFwd_Event.eventType = event->eventType;
   MIDIHdr->FastFwd_Event.parameter1 = event->parameter1;
   MIDIHdr->FastFwd_Event.parameter2 = event->parameter2; //A non zero velocity.

   while (MIDIHdr->FastFwd_Status.foundEventFlag)
   {
      MIDIHdr->masterClock++;
      if (MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_OFF) == MPB_FILE_FINISHED)
      {
         break;
      }
   }

   //Need to minus some position, so that when play resumes
   //the message is picked up.
   if (MIDIHdr->FastFwd_Status.foundEventFlag == 0)
   {
      position = MIDIHdr->masterClock - 1;
   }

   if (position == 0)
   {
      position = 1;
   }

   MPB_RePosition(MIDIHdr, position - 1, mode);

   return MIDIHdr->FastFwd_Status.foundEventStatus;
}

void MPB_FastFwd_TestEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_EVENT_t* event)
{

   MPB_FastFwd_t* fastFwd_Status;
   MIDI_CHAN_EVENT_t* fastFwd_Event;

   fastFwd_Status = &MIDIHdr->FastFwd_Status;
   fastFwd_Event = &MIDIHdr->FastFwd_Event;

   if (event->event.eventType >= MIDI_NOTE_OFF && event->event.eventType <= MIDI_PITCH_CHANGE)
   {
      if ((fastFwd_Event->eventType == event->event.chanEvent.eventType))
      {
         fastFwd_Status->foundEventStatus = FAST_FWD_FIND_COMMAND;

         if ((fastFwd_Event->parameter1 == event->event.chanEvent.parameter1) || (fastFwd_Event->parameter1 == FAST_FWD_IGNORE_PARAM1))
         {
            fastFwd_Status->foundEventStatus = FAST_FWD_FIND_PARAM1;

            if ((fastFwd_Event->parameter2 == event->event.chanEvent.parameter2) || (fastFwd_Event->parameter2 == FAST_FWD_IGNORE_PARAM2) || ((fastFwd_Event->parameter2 == FAST_FWD_NON_ZERO_PARAM2) && (event->event.chanEvent.parameter2 != 0)))
            {
               fastFwd_Status->foundEventStatus = FAST_FWD_FIND_PARAM2;
            }
         }

      }

      if (fastFwd_Status->searchMode == fastFwd_Status->foundEventStatus)
      {
         fastFwd_Status->foundEventFlag = 0;
      }
   }
}

uint8_t MPB_RePositionTime(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t timePosSec, MIDI_PB_MODE mode)
{
   uint16_t currentPos;
   uint32_t myMasterClock;
   uint32_t myMaxLength = MIDIHdr->currentState.maxLength;
   uint16_t i;
   MPB_ResetMIDITracks(MIDIHdr);

   for (i = 0; i < 30000; i++)
   {
      MPB_ContinuePlay(MIDIHdr, mode);
      currentPos = MPB_CurrentTimePosition(MIDIHdr);

      if (currentPos >= timePosSec)
      {
         break;
      }

      if (currentPos < timePosSec)
      {
         myMasterClock = myMasterClock + (MIDIHdr->PPQ << 2);
         MIDIHdr->masterClock = myMasterClock;
      }

      if (myMasterClock > myMaxLength)
      {
         MIDIHdr->masterClock = myMaxLength;
         break;
      }
   }

   MIDI_Printf("Min: ", MPB_CurrentTimePosition(MIDIHdr) / 60);
   MIDI_Printf("Sec: ", MPB_CurrentTimePosition(MIDIHdr) % 60);
   MIDI_Printf("Iter: ", i);
   return 0;
}

uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_PB_MODE mode)
{
   uint8_t i;
   uint8_t ret = 0;

   for (i = 0; i < MIDIHdr->trackCount; i++)
   {
      ret = 0;
      if ((MIDIHdr->Track[i].eventCount != MPB_TRACK_STOPPED))
      {
         while (1)
         {
            ret = MPB_PlayTrack(MIDIHdr, &MIDIHdr->Track[i], mode);
            if ((ret == MPB_REPOSITION_EVENT))
            {
               break;
            }
            else if (ret == MPB_TRACK_STOPPED)
            {
               MIDIHdr->currentState.trackState++;
               if (MIDIHdr->currentState.trackState >= MIDIHdr->trackCount)
               {
                  return MPB_FILE_FINISHED;
               }
               break;
            }
            else
            {

            }
         }
      }
   }

   if (MIDIHdr->trackCount == 0)
   {
      return MPB_FILE_FINISHED;
   }

   return MPB_FILE_STILL_PLAYING;
}

#define MPB_ON_TIME (1)
void MPB_ProcessGenericEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, MIDI_PB_MODE mode)
{
   uint8_t midiChannel;

   MPB_ApplyTranspose(event, MIDIHdr->transpose);
   MPB_PlayEvent(event, mode);
   track->eventCount = MPB_PLAY_NEXT_EVENT;

   midiChannel = (event->event.eventType & 0x0F);

   if (MIDIHdr->FastFwd_Status.foundEventFlag)
   {
      MPB_FastFwd_TestEvent(MIDIHdr, event);
   }

   if (MIDIHdr->playbackState == STATE_TRACK_BUILDING)
   {
      MPB_BuildChannelStats(MIDIHdr, event);
   }

   //Keep track of polyphony here
   if ((event->event.eventType & 0xF0) == MIDI_NOTE_ON)
   {
      //A note on with a velocity of 0, can be NOTE_OFFs.
      if (event->event.chanEvent.parameter2 == 0)
      {
         if (MIDIHdr->channelPolyphony[midiChannel])
         {
            MIDIHdr->channelPolyphony[midiChannel]--;
         }
         if (MIDIHdr->totalPolyphony)
         {
            MIDIHdr->totalPolyphony--;
         }
      }
      else
      {
         MIDIHdr->channelPolyphony[midiChannel]++;
         MIDIHdr->totalPolyphony++;
      }
      //The drum channel has an exception because sometimes there
      //might not be any offnotes. etc
      if (midiChannel == MIDI_DRUM_CHANNEL)
      {
         MIDIHdr->NoteOnTimer[event->event.chanEvent.parameter1] = MIDIHdr->NoteOnTime;
      }
   }
   else if ((event->event.eventType & 0xF0) == MIDI_NOTE_OFF)
   {
      if (MIDIHdr->channelPolyphony[midiChannel])
      {
         MIDIHdr->channelPolyphony[midiChannel]--;
      }
      if (MIDIHdr->totalPolyphony)
      {
         MIDIHdr->totalPolyphony--;
      }
      //If however we receive a Note Off before the timer has
      //elapsed, then reset it all.
      if (midiChannel == MIDI_DRUM_CHANNEL)
      {
         MIDIHdr->NoteOnTimer[event->event.chanEvent.parameter1] = 0;
      }
      //Turn off the respective LED.
      //LEDArray_SetLED(0, 0, 0, 0);
   }

   if (event->event.eventType == MIDI_META_MSG)
   {
      MPB_ProcessMetaEvent(MIDIHdr, track, event);
   }
}

uint8_t MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_PB_MODE mode)
{
   uint8_t* position = (uint8_t*)track->startPtr;
   uint8_t* oldPosition;
   uint8_t* readPtr = &track->buffer[0] + track->bufferOffset;
   uint8_t* originPtr = &track->buffer[0];
   MIDI_EVENT_t* event = &track->trackEvent;

   if (track->eventCount == MPB_TRACK_STOPPED)
   {
      return MPB_TRACK_STOPPED;
   }

   if (track->eventCount == MPB_NEW_DATA)
   {
   }
   else
   {
      if (track->trackClock <= MIDIHdr->masterClock)
      {
         MPB_ProcessGenericEvent(MIDIHdr, track, event, mode);
      }
      else
      {
         //Not yet time to play;
         //event->deltaTime--;
         return MPB_REPOSITION_EVENT;
      }
   }

   while ((track->eventCount != MPB_TRACK_STOPPED))
   {
      //If the buffer doesn't contain enough information for a full parse
      //Or if we are requested for new data.
      if ((track->eventCount == MPB_NEW_DATA) || ((track->bufferOffset + 6) > MIDI_TRACK_BUFFER_SIZE))
      {
         MPB_ReadToBuffer(track->startPtr, &track->buffer[0]);
         originPtr = &track->buffer[0];
         readPtr = originPtr;
         track->bufferOffset = 0;
      }

      oldPosition = readPtr;
      //MIDI Parse Event returns the ptr to the next event.
      readPtr = (uint8_t*)MIDIParse_Event(track, event, readPtr);
      track->bufferOffset = readPtr - originPtr;

      //On successful parse, we save the position.
      if (track->bufferOffset <= MIDI_TRACK_BUFFER_SIZE)
      {
         position = position + (readPtr - oldPosition);
         track->startPtr = (position);
         if (event->deltaTime)
         {
            track->trackClock = track->trackClock + event->deltaTime;
            track->eventCount = MPB_QUEUED_EVENT;

            //event->deltaTime--;
            return MPB_QUEUED_EVENT;
         }
         else
         {
            MPB_ProcessGenericEvent(MIDIHdr, track, event, mode);
         }
      }
      else
      {
         //If the parse was not fully completed due to incomplete read data,
         //We must read it again.
         track->eventCount = MPB_NEW_DATA;
         //We skip if the lengths are too long
         if (event->event.eventType == MIDI_META_MSG)
         {
            if (event->event.metaEvent.length > (MIDI_TRACK_BUFFER_SIZE - 6))
            {
               MPB_ProcessMetaEvent(MIDIHdr, track, event);
               position = position + (readPtr - oldPosition);
               track->startPtr = position;
            }
         }
         if (event->event.eventType == MIDI_SYSEX_START)
         {
            if (event->event.sysExEvent.length > (MIDI_TRACK_BUFFER_SIZE - 6))
            {
               position = position + (readPtr - oldPosition);
               track->startPtr = position;
            }
         }
         //Need to put in a check here since some events could be
         //larger than the buffer size, in which case we shall ignore.
      }
   }

   return track->eventCount;
}

uint8_t MPB_AddEvent(MIDI_EVENT_t* event)
{
   uint8_t ret;
   ret = STACK_PushData(&MPBStack, event);
   return ret;
}

MIDI_EVENT_t* MPB_GetNextEvent(void)
{
   MIDI_EVENT_t* event;
   event = (MIDI_EVENT_t*)FIFO_PeekData(&MPBStack);
   return event;
}

MIDI_EVENT_t* MPB_ConfirmEventTx(void)
{
   MIDI_EVENT_t* event;
   event = (MIDI_EVENT_t*)FIFO_PopData(&MPBStack);
   return event;
}

#define MIN_PPQ	(48)
#define MIN_BMP	(20)
#define MIN_BMP_PPQ_PRODUCT (MIN_BMP*MIN_PPQ)
#define MIN_BMP_PPQ_PRESCALER	(64)
uint16_t MPB_SetTickRate(uint16_t bpm, uint16_t PPQ)
{
   //*to send a Timing signal we need to send 24 0xF8's per quarter note
   uint16_t usPerTick;
   //Use a 1x prescaler.
   usPerTick = ((US_PER_MINUTE) / (bpm) / PPQ);

   //Use a 64x prescaler if the speed is too slow
   if ((bpm * PPQ) < MIN_BMP_PPQ_PRODUCT)
   {
      usPerTick = ((US_PER_MINUTE / MIN_BMP_PPQ_PRESCALER) / (bpm) / PPQ);
      //SET_TIMER_PRESCALER(64);
   }
   else
   {
      //SET_TIMER_PRESCALER(1);
   }
   //SET_TIMER_INTERVAL(usPerTick);

   //PR1 = 41*usPerTick;
   //T1CONbits.TCKPS0 = 0;
   //T1CONbits.TCKPS1 = 1;
   MIDI_Printf("BPM: ", bpm);
   MIDI_Printf("PPQ: ", PPQ);
   MIDI_Printf("usPerTick: ", usPerTick);
   return usPerTick;
}

void MPB_ProcessMetaEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event)
{

   uint8_t length = TRACK_MAX_NAME_LENGTH;
   uint16_t existingBPM;
   if ( TRACK_MAX_NAME_LENGTH > event->event.metaEvent.length)
   {
      length = event->event.metaEvent.length;
   }

   switch (event->event.metaEvent.metaType)
   {
   case MIDI_META_TRACK_NAME:
      memcpy(&track->name, event->event.metaEvent.data, length);
      MIDI_Printf("Track ", 0);
      MIDI_Printf("", track->trackIndex);
      //MIDI_Printfn((char*)&track->name, length);
      MIDI_Printf("\n", 0);
      break;

   case MIDI_META_TEMPO:
      existingBPM = MIDIHdr->currentState.BPM;
      reverseOrder((char*) event->event.metaEvent.data, event->event.metaEvent.length);
      memcpy(&MIDIHdr->currentState.BPM, event->event.metaEvent.data, event->event.metaEvent.length); //*bpmptr;

      MIDIHdr->currentState.BPM = BPM(MIDIHdr->currentState.BPM);
      MPB_SetTickRate(MIDIHdr->currentState.BPM, MIDIHdr->PPQ);

      if ((existingBPM != MIDIHdr->currentState.BPM) && existingBPM)
      {
         MIDIHdr->currentState.tickTime += ((track->trackClock - MIDIHdr->currentState.lastTempoChange) * 60) / (existingBPM);
         MIDIHdr->currentState.lastTempoChange = track->trackClock;
      }

      break;

   case MIDI_META_LYRICS:
      //Lyrics parsed in play events.
      //MIDI_Printfn(event->event.metaEvent.data, event->event.metaEvent.length);
      break;

   case MIDI_META_TIME_SIG:
      MIDIHdr->currentState.timeSignature = 0;
      MIDIHdr->currentState.timeSignature |= event->event.metaEvent.data[0] << 4;
      MIDIHdr->currentState.timeSignature |= 1 << event->event.metaEvent.data[1];
      MIDI_Printf("Numerator: ", MIDIHdr->currentState.timeSignature >> 4);
      MIDI_Printf("Denominator: ", MIDIHdr->currentState.timeSignature & 0x0F);
      break;

   case MIDI_META_KEY_SIG:
      MIDIHdr->currentState.keySignature = event->event.metaEvent.data[0];
      //The major or minor scale
      MIDIHdr->currentState.keyScale = event->event.metaEvent.data[1];
      MIDI_Printf("Key Signature: ", 0);
      MIDI_Printf(MIDIParse_KeySignature(MIDIHdr->currentState.keySignature, MIDIHdr->currentState.keyScale), 0);
      MIDI_Printf("\n", 0);
      break;

   case MIDI_META_TRACKEND:
      MIDI_Printf("TrackFinished: ", track->trackIndex);
      track->eventCount = MPB_TRACK_STOPPED;
      break;

   default:
      break;
   }
}

void MPB_SetTranspose(MIDI_HEADER_CHUNK_t* MIDIHdr, int8_t transpose)
{
   MIDIHdr->transpose = transpose;
}

void MPB_ApplyTranspose(MIDI_EVENT_t* event, int8_t transpose)
{
   //Apply transpose here
   if (((event->event.eventType & 0xF0) == MIDI_NOTE_ON) || ((event->event.eventType & 0xF0) == MIDI_NOTE_OFF))
   {
      uint8_t chan;
      chan = event->event.eventType & 0x0F;
      if (chan != MIDI_DRUM_CHANNEL)
      {
         event->event.chanEvent.parameter1 += transpose;
      }
   }

}

void MPB_PlayEvent(MIDI_EVENT_t* event, MIDI_PB_MODE mode)
{
   if (mode == MPB_PB_ALL_OFF)
   {
      return;
   }

   switch (event->event.eventType)
   {
   uint8_t i;
case MIDI_SYSEX_START:
   MIDI_Tx(MIDI_SYSEX_START);
   //MIXI_TxDump(event->event.sysExEvent.data, event->event.sysExEvent.length);
   MIDI_Printf("MIDISYSX FOUND:", event->event.sysExEvent.length);

   for (i = 0; i < event->event.sysExEvent.length; i++)
   {
      MIDI_Tx(event->event.sysExEvent.data[i]);
      MIDI_Printf("B:", event->event.sysExEvent.data[i]);
   }

   //MIDI_Printfn(event->event.sysExEvent.data, event->event.sysExEvent.length);

   break;

case MIDI_META_MSG:

   if (mode == MPB_PB_NO_NOTES)
   {
      break;
   }

   switch (event->event.metaEvent.metaType)
   {
   case MIDI_META_TEXT:
   case MIDI_META_LYRICS:

      //replace_char( (char*)event->event.metaEvent.data, '\\', '\n');
      //replace_char( (char*)event->event.metaEvent.data, '/', '\n');

      //MIDI_Printfn(event->event.metaEvent.data, event->event.metaEvent.length);
      //MIDI_Printf(event->event.metaEvent.data, event->event.metaEvent.length);

      break;
   }

   break;

default:
   //MIDI_PrintEventInfo(event);

   if (mode == MPB_PB_NO_NOTES)
   {
      if (((event->event.eventType & 0xF0) == MIDI_NOTE_ON) || ((event->event.eventType & 0xF0) == MIDI_NOTE_OFF))
      {
         break;
      }
   }
   else if (mode == MPB_PB_SAVE_MIDI_STATUS)
   {
      if (((event->event.eventType & 0xF0) >= MIDI_AFTERTOUCH) && ((event->event.eventType & 0xF0) < MIDI_SYSEX_START))
      {
         MPB_SaveMIDIStatus(&event->event.chanEvent);
      }
      break;
   }

   SendMIDIEvent(&event->event.chanEvent);
   MPB_OutputMIDIChanEvent(&event->event.chanEvent);

   break;
   }
}

void MPB_OutputMIDIChanEvent(MIDI_CHAN_EVENT_t* chanEvent)
{
   static uint8_t runningStatus = 0x00;
   if (chanEvent->eventType == runningStatus)
   {

   }
   else
   {
      runningStatus = chanEvent->eventType;
      MIDI_Tx(chanEvent->eventType);
   }
   MIDI_Tx(chanEvent->parameter1);
   if (MIDI_CommandSize(chanEvent->eventType & 0xF0) == 3)
   {
      MIDI_Tx(chanEvent->parameter2);
   }
}

#define MPB_IGNORE_BYTE	(0xFF)
#define MPB_SAVE_BUFFER_SIZE	(16)

//#define MPB_STATUS_BUFFER_ARRAY

#ifdef MPB_STATUS_BUFFER_ARRAY
MIDI_CHAN_EVENT_t StatusBuffer[MIDI_MAX_CHANNELS][MPB_SAVE_BUFFER_SIZE];
#else
LINKED_LIST_t StatusBuffer[MIDI_MAX_CHANNELS];
#endif

uint16_t StatusBufferCount[MIDI_MAX_CHANNELS];

//Saves the channel event. Does not save Note On / Offs
void MPB_SaveMIDIStatus(MIDI_CHAN_EVENT_t* chanEvent)
{

   /*   MIDI_AFTERTOUCH = 0xA0,
    MIDI_CONTROL_CHANGE = 0xB0,
    MIDI_PROGRAM_CHANGE = 0xC0,
    MIDI_CHANNEL_PRESSURE = 0xD0,
    MIDI_PITCH_CHANGE = 0xE0,*/

   MIDI_CHAN_EVENT_t* foundEvent = NULL;

   //All messages are channel specific.
   switch (chanEvent->eventType & 0xF0)
   {

   case MIDI_AFTERTOUCH:
   case MIDI_CONTROL_CHANGE:
      foundEvent = _findChanEventInBuffer(chanEvent->eventType, chanEvent->parameter1);
      if (foundEvent)
      {
         foundEvent->parameter2 = chanEvent->parameter2;
      }
      break;

   case MIDI_PROGRAM_CHANGE:
   case MIDI_CHANNEL_PRESSURE:
   case MIDI_PITCH_CHANGE:
      foundEvent = _findChanEventInBuffer(chanEvent->eventType, MPB_IGNORE_BYTE);
      if (foundEvent)
      {
         foundEvent->parameter1 = chanEvent->parameter1;
         foundEvent->parameter2 = chanEvent->parameter2;
      }
      break;

   default:
      foundEvent = NULL;
      break;
   }

   if (foundEvent == NULL)
   {
      _addEventToBuffer(chanEvent);
   }

}

void MPB_ReplayStatusBuffer(void)
{
   int i, j;
   int bufferCount;

   MIDI_CHAN_EVENT_t* chanEvent;

   for (bufferCount = 0, j = 0; j < MIDI_MAX_CHANNELS; j++)
   {
      for (i = 0; i < StatusBufferCount[j]; i++)
      {
         chanEvent = _getEventFromBuffer(j, i);
         if (chanEvent)
         {
            MPB_OutputMIDIChanEvent(chanEvent);
         }
      }
      bufferCount = bufferCount + StatusBufferCount[j];
   }

   MIDI_Printf("TotalStatusCount=", bufferCount);
}

void _addEventToBuffer(MIDI_CHAN_EVENT_t* chanEvent)
{

   uint8_t channel;
   MIDI_CHAN_EVENT_t* newChanEvent;

   channel = chanEvent->eventType & 0x0F;

#ifdef MPB_STATUS_BUFFER_ARRAY
   if( StatusBufferCount[channel] < MPB_SAVE_BUFFER_SIZE)
   {
      memcpy(&StatusBuffer[channel][StatusBufferCount[channel]], chanEvent, sizeof(MIDI_CHAN_EVENT_t));
      StatusBufferCount[channel]++;
   }
#else
   newChanEvent = (MIDI_CHAN_EVENT_t*)LL_Malloc(sizeof(MIDI_CHAN_EVENT_t));
   memcpy(newChanEvent, chanEvent, sizeof(MIDI_CHAN_EVENT_t));
   LL_AppendData(&StatusBuffer[channel], (void*) newChanEvent);
   StatusBufferCount[channel]++;
#endif

}

MIDI_CHAN_EVENT_t* _getEventFromBuffer(uint8_t channel, uint16_t index)
{
#ifdef MPB_STATUS_BUFFER_ARRAY
   return &StatusBuffer[channel][index];
#else
   return (MIDI_CHAN_EVENT_t*) LL_ReturnNodeDataFromIndex(&StatusBuffer[channel], index);
#endif
}

void _clearEventBuffer(void)
{
   int i;

#ifdef MPB_STATUS_BUFFER_ARRAY
   for( i = 0; i < MIDI_MAX_CHANNELS; i++)
   {
      memset(&StatusBuffer[i], 0, sizeof(MIDI_CHAN_EVENT_t)*MPB_SAVE_BUFFER_SIZE);
      StatusBufferCount[i] = 0;
   }

#else
   for (i = 0; i < MIDI_MAX_CHANNELS; i++)
   {
      LL_DeleteListAndData(&StatusBuffer[i]);
      StatusBufferCount[i] = 0;
   }
#endif
}

MIDI_CHAN_EVENT_t* _findChanEventInBuffer(uint8_t firstByte, uint8_t secondByte)
{
   int i;
   MIDI_CHAN_EVENT_t* foundEvent;
   uint8_t channel;

   channel = firstByte & 0x0F;

   for (i = 0; i < StatusBufferCount[channel]; i++)
   {
      foundEvent = _getEventFromBuffer(channel, i);
      if (foundEvent->eventType == firstByte)
      {
         if (secondByte == MPB_IGNORE_BYTE)
         {
            return foundEvent;
         }
         else
         {
            if (foundEvent->parameter1 == secondByte)
            {
               return foundEvent;
            }
         }
      }
   }

   return NULL;
}

uint8_t _MIDI_fileopen(char* filename)
{
   midifile = fopen(filename, "rb");
   return 0;
}

void _MIDI_fileclose(void)
{
   fclose(midifile);
}

void _MIDI_readbuf(uint8_t* position, uint8_t* buf, uint16_t size)
{
   fseek(midifile, (long long) position, 0);
   fread(buf, 1, size, midifile);
}

