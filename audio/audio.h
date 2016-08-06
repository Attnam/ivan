/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C)
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
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
   musicfile(char* filename, int LowThreshold, int HighThreshold);
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
   static void Init();
   static void DeInit(void);

   static int Loop(void *ptr);

   static int PlayMIDIFile(char* filename, int32_t loops);

   static void SendMIDIEvent(std::vector<unsigned char>* message);

   /**
    * @param vol 0 - 128
    */
   static void SetVolumeLevel(int vol);

   static int GetVolumeLevel(void);

   /**
    * @param intensity 0 - 100
    */
   static void IntensityLevel(int intensity);

   /**
    * @param filename MIDI file location
    * @param intensitylow
    */
   static void LoadMIDIFile(char* filename, int intensitylow, int intensityhigh);

private:


   static bool isInit;
   static int  Volume;
   static int  Intensity;

   static std::vector<musicfile*> Tracks;

   static RtMidiOut* midiout;

};

#endif
