/*
    audio.cpp : MIDI Audio Implementation for IVAN
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


#ifdef USE_SDL
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_timer.h"


#endif

#include "audio.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include "message.h"
#include "midiplayback.h"



musicfile::musicfile(char* filename, int LowThreshold, int HighThreshold) :
      LowThreshold(LowThreshold), HighThreshold(HighThreshold)
{
   isPlaying = false;
   Filename = new char[strlen(filename) + 1];
   memcpy(Filename, filename, strlen(filename) + 1);
}

musicfile::~musicfile()
{
   delete[] Filename;
}

int audio::Volume;
int audio::Intensity;
bool audio::isInit;

int audio::PlaybackState;
bool audio::isTrackPlaying;

int audio::CurrentPosition;
int audio::CurrentMIDIOutPort;

std::vector<musicfile*> audio::Tracks;
RtMidiOut* audio::midiout = 0;

void audio::error(RtMidiError::Type type, const std::string &errorText, void *userData )
{

}

void audio::Init()
{
   int audio_rate, audio_channels;
   unsigned short audio_format;
   int bits;

   int nPorts;
   std::string portName;
   std::vector<unsigned char> message;

   PlaybackState = DISABLED;
   CurrentMIDIOutPort = -1;
   // RtMidiOut constructor
   try
   {
      midiout = new RtMidiOut();
   } catch (RtMidiError &error)
   {
      error.printMessage();
      ABORT("MIDI Out Error");
   }


   midiout->setErrorCallback(audio::error);

   //LoadMIDIFile("Track1.mid", 0, 100);
   //LoadMIDIFile("Track2.mid", 0, 100);

   SetVolumeLevel(127);

   SDL_Thread *thread;
   int         threadReturnValue;

   // Simply create a thread
   thread = SDL_CreateThread( audio::Loop, "AudioThread", (void *)NULL);

   PlaybackState = STOPPED;

   isInit = true;
   isTrackPlaying = false;
   atexit(audio::DeInit);

}

void audio::DeInit(void)
{
   isInit = false;

   for (std::vector<musicfile*>::iterator it = Tracks.begin(); it != Tracks.end(); ++it)
   {
      delete *it;
   }

   if( midiout )
   {
      delete midiout;
   }

   Tracks.erase(Tracks.begin(), Tracks.end());

}

int audio::Loop(void *ptr)
{

   std::vector<unsigned char> message;
   // Note On: 144, 64, 90

   while(1)
   {
      if( Tracks.size() && (PlaybackState == PLAYING) )
      {
         isTrackPlaying = true;
//         PlaybackState = PLAYING;
         int randomIndex = rand() % Tracks.size();
         PlayMIDIFile(Tracks[randomIndex]->GetFilename(), 1);
      }

      isTrackPlaying = false;
      SDL_Delay(1);

   }
   return 0;
}

/*int audio::GetCurrentOutputDevice(void)
{
   return midiout->isPortOpen();
}*/

/* Port0 is NULL, and disabled. */
int audio::ChangeMIDIOutputDevice(int newPort)
{
   if( newPort != CurrentMIDIOutPort)
   {
      audio::SetPlaybackStatus(audio::PAUSED);

      try {
         if( midiout->isPortOpen() )
         {
            midiout->closePort();
         }

         if( newPort != 0)
         {
            midiout->openPort(newPort-1);
            audio::SetPlaybackStatus(audio::PLAYING);
         }
         CurrentMIDIOutPort = newPort;

      }
      catch (RtMidiError &error) {
        error.printMessage();
        ABORT("MIDI Out Error");
      }
   }

   return 0;
}

int audio::GetMIDIOutputDevices(std::vector<std::string>& deviceNames)
{
   int nPorts = midiout->getPortCount();
   std::string portName;

   for ( unsigned int i=0; i<nPorts; i++ ) {
     try {
       portName = midiout->getPortName(i);

     }
     catch (RtMidiError &error) {
       error.printMessage();
       ABORT("MIDI Out Error");
     }
     deviceNames.push_back(portName);
   }

   return nPorts;
}


int audio::PlayMIDIFile(char* filename, int32_t loops)
{
   std::vector<unsigned char> message;
   MIDI_HEADER_CHUNK_t MIDIHdr;


   MPB_PlayMIDIFile(&MIDIHdr, filename);

   int usPerTick = MPB_SetTickRate(MIDIHdr.currentState.BPM, MIDIHdr.PPQ);
   int cumulativeWait = 0;
   int position = 0;

   if( PlaybackState == PAUSED )
   {
      position = CurrentPosition;
   }

   for (int32_t i = 0; i < loops; i++)
   {
      MPB_RePosition(&MIDIHdr, position, MPB_PB_NO_VOL);
      for(;;)
      {
         cumulativeWait += usPerTick;
         if( cumulativeWait >= 1000 )
         {
            SDL_Delay(cumulativeWait / 1000);
            cumulativeWait = cumulativeWait - ((cumulativeWait / 1000)*1000);
         }

         if( PlaybackState == PLAYING )
         {
            MIDIHdr.masterClock += 1;
            CurrentPosition = MIDIHdr.masterClock;
         }

         if( PlaybackState == PAUSED )
         {
            CurrentPosition = MIDIHdr.masterClock;
            MPB_PausePlayback(&MIDIHdr);
            MPB_ResetMIDI();
            MPB_CloseFile();
            return 0;
         }

         if( PlaybackState == STOPPED )
         {
            CurrentPosition = 0;
            MPB_PausePlayback(&MIDIHdr);
            MPB_ResetMIDI();
            MPB_CloseFile();
            return 0;
         }

         if (MPB_ContinuePlay(&MIDIHdr, MPB_PB_NO_VOL) == MPB_FILE_FINISHED)
         {
            MPB_PausePlayback(&MIDIHdr);
            break;
         }
      }

      //Reset playback pointer
      position = 0;
      CurrentPosition = 0;
   }

   MPB_CloseFile();

   return 0;
}


void audio::SetVolumeLevel(int vol)
{
   Volume = vol;

   MIDI_CHAN_EVENT_t newVolume;
   newVolume.eventType = MIDI_CONTROL_CHANGE;
   newVolume.parameter1 = CHANNEL_VOLUME;
   newVolume.parameter2 = vol;

   for(int i = 0 ; i < MIDI_MAX_CHANNELS; ++i )
   {
      newVolume.eventType = MIDI_CONTROL_CHANGE | i;
      ::SendMIDIEvent(&newVolume);
   }

   //Mix_VolumeMusic(Volume);
}

int audio::GetVolumeLevel(void)
{
   return Volume;
}

void audio::IntensityLevel(int intensity)
{
   Intensity = intensity;

   /* Do a check to see if we change / cue MIDI file */
}


void audio::SetPlaybackStatus(eAudioPlaybackStates_t newState)
{
   PlaybackState = newState;
   if( (newState == PAUSED) || (newState == STOPPED))
   {
      //Wait until the track has finished playing
      while(isTrackPlaying)
      {

      }
   }
}


void audio::ClearMIDIPlaylist(void)
{
   for (std::vector<musicfile*>::iterator it = Tracks.begin(); it != Tracks.end(); ++it)
   {
      delete *it;
   }
   Tracks.erase(Tracks.begin(), Tracks.end());
}

void audio::LoadMIDIFile(char* filename, int intensitylow, int intensityhigh)
{
   musicfile* mf = new musicfile(filename, intensitylow, intensityhigh);
   Tracks.push_back(mf);
}



void audio::SendMIDIEvent(std::vector<unsigned char>* message)
{
   midiout->sendMessage( message );
}



void SendMIDIEvent(MIDI_CHAN_EVENT_t* event)
{
   std::vector<unsigned char> message;
   message.push_back(event->eventType);
   message.push_back(event->parameter1);
   message.push_back(event->parameter2);
   audio::SendMIDIEvent( &message );
}


