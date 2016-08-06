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
std::vector<musicfile*> audio::Tracks;
RtMidiOut* audio::midiout = 0;

void audio::Init()
{
   int audio_rate, audio_channels;
   unsigned short audio_format;
   int bits;

   int nPorts;
   std::string portName;
   std::vector<unsigned char> message;

   // RtMidiOut constructor
   try
   {
      midiout = new RtMidiOut();
   } catch (RtMidiError &error)
   {
      error.printMessage();
      ABORT("MIDI Out Error");
   }

   // Check outputs.
   nPorts = midiout->getPortCount();
   std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
   for ( unsigned int i=0; i<nPorts; i++ ) {
     try {
       portName = midiout->getPortName(i);
     }
     catch (RtMidiError &error) {
       error.printMessage();
       ABORT("MIDI Out Error");
     }
     std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
   }
   std::cout << '\n';

   midiout->openPort(0);

   LoadMIDIFile("Track1.mid", 0, 100);
   LoadMIDIFile("Track2.mid", 0, 100);

   SetVolumeLevel(127);

   SDL_Thread *thread;
   int         threadReturnValue;

   // Simply create a thread
   thread = SDL_CreateThread( audio::Loop, "AudioThread", (void *)NULL);

   isInit = true;
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
   bool trackIsPlaying = false;


   std::vector<unsigned char> message;
   // Note On: 144, 64, 90

   while(1)
   {
      int randomIndex = rand() % Tracks.size();
      PlayMIDIFile(Tracks[randomIndex]->GetFilename(), 1);
   }
   return 0;
}


int audio::ChangeMIDIOutputDevice(std::string portName)
{
   std::vector<std::string> devicenames;
   audio::GetMIDIOutputDevices(devicenames);

   midiout->closePort();

   for (int i = 0; i < devicenames.size(); ++i)
   {
      if( portName.compare( devicenames[i] ) == 0 )
      {
         midiout->openPort(i);
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
   for (uint32_t i = 0; i < loops; i++)
   {
      MPB_RePosition(&MIDIHdr, 0, MPB_PB_NO_VOL);
      for(;;)
      {
         cumulativeWait += usPerTick;
         if( cumulativeWait >= 1000 )
         {
            SDL_Delay(cumulativeWait / 1000);
            cumulativeWait = cumulativeWait - ((cumulativeWait / 1000)*1000);
         }

         MIDIHdr.masterClock += 1;
         if (MPB_ContinuePlay(&MIDIHdr, MPB_PB_NO_VOL) == MPB_FILE_FINISHED)
         {
            MPB_PausePlayback(&MIDIHdr);
            break;
         }
      }
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


