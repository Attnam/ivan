/*
    audio.h : MIDI Audio Implementation for IVAN
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
#ifndef __AUDIO_H__
#define __AUDIO_H__


#ifdef USE_SDL
#include "SDL.h"
#endif

#include "error.h"
#include <vector>
#include "RtMidi.h"

class musicfile
{
public:
   musicfile(cchar* filename, int LowThreshold, int HighThreshold);
   ~musicfile();

   inline bool IsPlaying(void) { return isPlaying; }
   inline void SetPlayState(bool state) { isPlaying = state;}
   inline char* GetFilename(void) { return Filename; }

private:
   char* Filename;
   int LowThreshold;
   int HighThreshold;

   bool isPlaying;

};



class audio
{
public:

   /** Bitmap for different states*/
   typedef enum
   {
      STOPPED       = 0x00,
      PLAYING       = 0x01,
      RESUME_SONG  = 0x02,
   } eAudioPlaybackStates_t ;

   enum
   {
      MAX_MIDI_CHANNELS = 16
   };

   enum
   {
      MAX_MASTER_VOLUME  = 127,
      MAX_INTENSITY_VOLUME = 127
   };

   enum
   {
      US_PER_VOLUME_CHANGE  = 15000
   };

   static void error(RtMidiError::Type type, const std::string &errorText, void *userData );

   /**
    * @param musicDirectory path to the directory containing the MIDI files to load.
    */
   static void Init(cfestring& musicDirectory);
   static void DeInit(void);

   static int Loop(void *ptr);

   static int PlayMIDIFile(char* filename, int32_t loops);

   static void SendMIDIEvent(std::vector<unsigned char>* message);

   static int GetMIDIOutputDevices(std::vector<std::string>& deviceNames);

   static int ChangeMIDIOutputDevice(int newPort);

   static char* GetCurrentlyPlayedFile(void);

   /**
    * @param vol 0 - 128
    */
   static void SetVolumeLevel(int vol);

   static int GetVolumeLevel(void);

   static void SendVolumeMessage(int targetVolume);

   /**
    * @param intensity 0 - 100
    */
   static void IntensityLevel(int intensity);

   static void RemoveMIDIFile(char* filename);

   /**
    * @param filename MIDI file location
    * @param intensitylow
    */
   static void LoadMIDIFile(cchar* filename, int intensitylow, int intensityhigh);


   static void ClearMIDIPlaylist(char* exceptFilename = 0);

   static int IsPlaybackStopped(void);

   static void SetPlaybackStatus(uint8_t newStateBitmap);

   static void CalculateChannelVolumes(int intensity, int* deltaIntensity);

private:


   static bool isInit;
   static bool volumeChangeRequest;
   static int  MasterVolume; /** 0 - 127 */
   static int  IntensityVolume[MAX_MIDI_CHANNELS];

   static int  InitialIntensityVolume[MAX_MIDI_CHANNELS];
   static int  DeltaVolumePerIntensity[MAX_MIDI_CHANNELS];

   static int  TargetIntensity;
   static int  CurrentIntensity;

   static volatile bool isTrackPlaying;

   static int CurrentPosition;

   static int  PlaybackState;
   static char* CurrentTrack;
   static festring MusDir;

   static std::vector<musicfile*> Tracks;

   static RtMidiOut* midiout;
   static int CurrentMIDIOutPort;

};

#endif
