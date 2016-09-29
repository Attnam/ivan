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
#include "game.h"


musicfile::musicfile(cchar* filename, int LowThreshold, int HighThreshold) :
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



int audio::MasterVolume;
int audio::TargetIntensity;
int audio::CurrentIntensity;
bool audio::isInit;

int audio::PlaybackState;
bool audio::isTrackPlaying;

bool audio::volumeChangeRequest;

int audio::CurrentPosition;
int audio::CurrentMIDIOutPort;

std::vector<musicfile*> audio::Tracks;
RtMidiOut* audio::midiout = 0;

char* audio::CurrentTrack;

/** For each increase in intensity, the respective MIDI channel changes by the following amount */
int  audio::DeltaVolumePerIntensity[MAX_MIDI_CHANNELS] = {0, 0, 0, 0, 0, -1, -1, -1, -1, 0, -1, 1, 1, 1, 1, 1};
int  audio::IntensityVolume[MAX_MIDI_CHANNELS];

int  audio::InitialIntensityVolume[MAX_MIDI_CHANNELS] = {MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME,
                                                         MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME, MAX_INTENSITY_VOLUME,
                                                         MAX_INTENSITY_VOLUME, 0, 0, 0, 0, 0};


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

   PlaybackState = audio::RESUME_SONG;
   CurrentMIDIOutPort = -1;
   CurrentIntensity = 0;
   MasterVolume = 0;
   TargetIntensity = 0;
   volumeChangeRequest = false;
   CurrentTrack = 0;

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



   SDL_Thread *thread;
   int         threadReturnValue;

   // Simply create a thread
   thread = SDL_CreateThread( audio::Loop, "AudioThread", (void *)NULL);

   PlaybackState = 0x00;

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
      if( Tracks.size() && (PlaybackState & PLAYING) )
      {
         isTrackPlaying = true;
         int randomIndex = rand() % Tracks.size();
         CurrentTrack = Tracks[randomIndex]->GetFilename();

         festring MusDir = game::GetMusicDir();
         festring MusFile = MusDir + festring(CurrentTrack);

         PlayMIDIFile( (char*)MusFile.CStr(), 1);
      }
      isTrackPlaying = false;
      SDL_Delay(1);
   }
   return 0;
}


char* audio::GetCurrentlyPlayedFile(void)
{
   return CurrentTrack;
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
      audio::SetPlaybackStatus(0x00);
      try {
         if( midiout->isPortOpen() )
         {
            midiout->closePort();
         }

         if( newPort != 0)
         {
            midiout->openPort(newPort-1);
            audio::SetPlaybackStatus(audio::PLAYING | audio::RESUME_SONG);
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


void audio::SendVolumeMessage(int targetVolume)
{
   MIDI_CHAN_EVENT_t newVolume;


   for(int i = 0 ; i < MAX_MIDI_CHANNELS; ++i )
   {
      newVolume.eventType = MIDI_CONTROL_CHANGE | i;
      newVolume.parameter1 = CHANNEL_VOLUME;

      int midivolume  = (IntensityVolume[i] * targetVolume) / MAX_MASTER_VOLUME;
      if( midivolume >= MAX_MASTER_VOLUME )
      {
         midivolume = MAX_MASTER_VOLUME;
      }

      if( midivolume <= 0 )
      {
         midivolume = 0;
      }
      newVolume.parameter2 = midivolume;


      ::SendMIDIEvent(&newVolume);
   }

}


int audio::PlayMIDIFile(char* filename, int32_t loops)
{
   std::vector<unsigned char> message;
   MIDI_HEADER_CHUNK_t MIDIHdr;


   MPB_PlayMIDIFile(&MIDIHdr, filename);

   int usPerTick = MPB_SetTickRate(MIDIHdr.currentState.BPM, MIDIHdr.PPQ);
   int cumulativeWait = 0;
   int position = CurrentPosition;

   int volumeChangeDelay = US_PER_VOLUME_CHANGE;


   if( !(PlaybackState & RESUME_SONG) )
   {
      position = 0;
   }

   for (int32_t i = 0; i < loops; i++)
   {
      MPB_RePosition(&MIDIHdr, position, MPB_PB_NO_VOL);
      for(;;)
      {
         cumulativeWait += usPerTick;
         volumeChangeDelay -= cumulativeWait;

         if( cumulativeWait >= 1000 )
         {
            SDL_Delay(cumulativeWait / 1000);
            cumulativeWait = cumulativeWait - ((cumulativeWait / 1000)*1000);
         }

         if( (volumeChangeDelay < 0) || volumeChangeRequest)
         {
            volumeChangeDelay = US_PER_VOLUME_CHANGE;
            if( (CurrentIntensity != TargetIntensity) || volumeChangeRequest )
            {
               if( TargetIntensity > CurrentIntensity )
               {
                  CurrentIntensity++;
               }

               if( CurrentIntensity > TargetIntensity )
               {
                  CurrentIntensity--;
               }
               CalculateChannelVolumes(CurrentIntensity, &DeltaVolumePerIntensity[0]);
               SendVolumeMessage(MasterVolume);

               if( volumeChangeRequest )
               {
                  volumeChangeRequest = false;
               }

            }
         }

         if( PlaybackState & PLAYING )
         {
            MIDIHdr.masterClock += 1;
            CurrentPosition = MIDIHdr.masterClock;
         }
         else
         {
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
   MasterVolume = vol;
   volumeChangeRequest = true;
}

int audio::GetVolumeLevel(void)
{
   return MasterVolume;
}

void audio::IntensityLevel(int intensity)
{
   if( intensity != TargetIntensity )
   {
      TargetIntensity = intensity;
   }
   /* Do a check to see if we change / cue MIDI file */
}


void audio::SetPlaybackStatus(uint8_t newStateBitmap)
{
   PlaybackState = newStateBitmap;
   if( !(PlaybackState & PLAYING))
   {
      //Wait until the track has finished playing
      while(isTrackPlaying)
      {

      }
   }
}


void audio::ClearMIDIPlaylist(char* exceptFilename)
{
   for (std::vector<musicfile*>::iterator it = Tracks.begin(); it != Tracks.end(); )
   {
      musicfile* p = *it;
      if( exceptFilename && strcmp(exceptFilename, p->GetFilename() ) == 0 )
      {
         ++it;
      }
      else
      {
         Tracks.erase(it);
      }
   }
}

void audio::RemoveMIDIFile(char* filename)
{
   for (std::vector<musicfile*>::iterator it = Tracks.begin(); it != Tracks.end(); ++it)
   {
      musicfile* p = *it;
      if( strcmp(filename, p->GetFilename() ) == 0 )
      {
         Tracks.erase(it);
      }
   }

}

void audio::LoadMIDIFile(cchar* filename, int intensitylow, int intensityhigh)
{
  musicfile* mf = new musicfile(filename, intensitylow, intensityhigh);
  Tracks.push_back(mf);
}

void audio::SendMIDIEvent(std::vector<unsigned char>* message)
{
   midiout->sendMessage( message );
}


void audio::CalculateChannelVolumes(int intensity, int* deltaIntensity)
{
   for(int i = 0 ; i < MAX_MIDI_CHANNELS; ++i)
   {
      IntensityVolume[i] = InitialIntensityVolume[i] + (deltaIntensity[i] * intensity);
      if(IntensityVolume[i] >= MAX_INTENSITY_VOLUME)
      {
         IntensityVolume[i]  = MAX_INTENSITY_VOLUME;
      }

      if( IntensityVolume[i] <= 0 )
      {
         IntensityVolume[i] = 0;
      }


   }



}



void SendMIDIEvent(MIDI_CHAN_EVENT_t* event)
{
   std::vector<unsigned char> message;
   message.push_back(event->eventType);
   message.push_back(event->parameter1);
   if( ((event->eventType & MIDI_MSG_TYPE_MASK) != MIDI_PROGRAM_CHANGE) && ((event->eventType & MIDI_MSG_TYPE_MASK) != MIDI_CHANNEL_PRESSURE))
   {
	   message.push_back(event->parameter2);
   }
   audio::SendMIDIEvent( &message );
}
