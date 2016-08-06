/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
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
** Descriptions: Lighting subsystem to interface with MIDI data and output it to the LED array / lighting system
** It is possible that each MIDI Channel will have it's own colour.
** Or that Velocity will control the colour of the LED.
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "MIDICodes.h"
#include "MIDIUtils.h"

const char	ACC_GRAND_PIANO_STRING[]	=	"Acc. Grand Piano";
const char	BRIGHT_ACC_PIANO_STRING[]	=	"Bright Acc. Piano";
const char	ELEC_GRAND_PIANO_STRING[]	=	"Elec. Grand Piano";
const char	HONKY_TONK_PIANO_STRING[]	=	"Honky-tonk Piano";
const char	ELECTRIC_PIANO_1_STRING[]	=	"Electric Piano 1";
const char	ELECTRIC_PIANO_2_STRING[]	=	"Electric Piano 2";
const char	HARPSICHORD_STRING[]	=	"Harpsichord";
const char	CLAVI_STRING[]	=	"Clavi";
const char	CELESTA_STRING[]	=	"Celesta";
const char	GLOCKENSPIEL_STRING[]	=	"Glockenspiel";
const char	MUSIC_BOX_STRING[]	=	"Music Box";
const char	VIBRAPHONE_STRING[]	=	"Vibraphone";
const char	MARIMBA_STRING[]	=	"Marimba";
const char	XYLOPHONE_STRING[]	=	"Xylophone";
const char	TUBULAR_BELLS_STRING[]	=	"Tubular Bells";
const char	DULCIMER_STRING[]	=	"Dulcimer";
const char	DRAWBAR_ORGAN_STRING[]	=	"Drawbar Organ";
const char	PERCUSSIVE_ORGAN_STRING[]	=	"Percussive Organ";
const char	ROCK_ORGAN_STRING[]	=	"Rock Organ";
const char	CHURCH_ORGAN_STRING[]	=	"Church Organ";
const char	REED_ORGAN_STRING[]	=	"Reed Organ";
const char	ACCORDION_STRING[]	=	"Accordion";
const char	HARMONICA_STRING[]	=	"Harmonica";
const char	TANGO_ACCORDION_STRING[]	=	"Tango Accordion";
const char	NYLON_GUITAR_STRING[]	=	"Nylon Guitar";
const char	STEEL_GUITAR_STRING[]	=	"Steel Guitar";
const char	JAZZ_GUITAR_STRING[]	=	"Jazz Guitar";
const char	CLEAN_GUITAR_STRING[]	=	"Clean Guitar";
const char	MUTED_GUITAR_STRING[]	=	"Muted Guitar";
const char	OVERDRIVEN_GUITAR_STRING[]	=	"Overdriven Guitar";
const char	DISTORTION_GUITAR_STRING[]	=	"Distortion Guitar";
const char	GUITAR_HARMONICS_STRING[]	=	"Guitar harmonics";
const char	ACOUSTIC_BASS_STRING[]	=	"Acoustic Bass";
const char	FINGERED_BASS_STRING[]	=	"Fingered Bass";
const char	PICKED_BASS_STRING[]	=	"Picked Bass";
const char	FRETLESS_BASS_STRING[]	=	"Fretless Bass";
const char	SLAP_BASS_1_STRING[]	=	"Slap Bass 1";
const char	SLAP_BASS_2_STRING[]	=	"Slap Bass 2";
const char	SYNTH_BASS_1_STRING[]	=	"Synth Bass 1";
const char	SYNTH_BASS_2_STRING[]	=	"Synth Bass 2";
const char	VIOLIN_STRING[]	=	"Violin";
const char	VIOLA_STRING[]	=	"Viola";
const char	CELLO_STRING[]	=	"Cello";
const char	CONTRABASS_STRING[]	=	"Contrabass";
const char	TREMOLO_STRINGS_STRING[]	=	"Tremolo Strings";
const char	PIZZICATO_STRINGS_STRING[]	=	"Pizzicato Strings";
const char	ORCHESTRAL_HARP_STRING[]	=	"Orchestral Harp";
const char	TIMPANI_STRING[]	=	"Timpani";
const char	STRING_ENSEMBLE_1_STRING[]	=	"String Ensemble 1";
const char	STRING_ENSEMBLE_2_STRING[]	=	"String Ensemble 2";
const char	SYNTHSTRINGS_1_STRING[]	=	"SynthStrings 1";
const char	SYNTHSTRINGS_2_STRING[]	=	"SynthStrings 2";
const char	CHOIR_AAHS_STRING[]	=	"Choir Aahs";
const char	VOICE_OOHS_STRING[]	=	"Voice Oohs";
const char	SYNTH_VOICE_STRING[]	=	"Synth Voice";
const char	ORCHESTRA_HIT_STRING[]	=	"Orchestra Hit";
const char	TRUMPET_STRING[]	=	"Trumpet";
const char	TROMBONE_STRING[]	=	"Trombone";
const char	TUBA_STRING[]	=	"Tuba";
const char	MUTED_TRUMPET_STRING[]	=	"Muted Trumpet";
const char	FRENCH_HORN_STRING[]	=	"French Horn";
const char	BRASS_SECTION_STRING[]	=	"Brass Section";
const char	SYNTHBRASS_1_STRING[]	=	"SynthBrass 1";
const char	SYNTHBRASS_2_STRING[]	=	"SynthBrass 2";
const char	SOPRANO_SAX_STRING[]	=	"Soprano Sax";
const char	ALTO_SAX_STRING[]	=	"Alto Sax";
const char	TENOR_SAX_STRING[]	=	"Tenor Sax";
const char	BARITONE_SAX_STRING[]	=	"Baritone Sax";
const char	OBOE_STRING[]	=	"Oboe";
const char	ENGLISH_HORN_STRING[]	=	"English Horn";
const char	BASSOON_STRING[]	=	"Bassoon";
const char	CLARINET_STRING[]	=	"Clarinet";
const char	PICCOLO_STRING[]	=	"Piccolo";
const char	FLUTE_STRING[]	=	"Flute";
const char	RECORDER_STRING[]	=	"Recorder";
const char	PAN_FLUTE_STRING[]	=	"Pan Flute";
const char	BLOWN_BOTTLE_STRING[]	=	"Blown Bottle";
const char	SHAKUHACHI_STRING[]	=	"Shakuhachi";
const char	WHISTLE_STRING[]	=	"Whistle";
const char	OCARINA_STRING[]	=	"Ocarina";
const char	SQUARE_WAVE_STRING[]	=	"Square Wave";
const char	SAW_WAVE_STRING[]	=	"Saw Wave";
const char	CALLIOPE_STRING[]	=	"Calliope";
const char	CHIFF_STRING[]	=	"Chiff";
const char	CHARANG_STRING[]	=	"Charang";
const char	VOICE_STRING[]	=	"Voice";
const char	FIFTH_WAVE_STRING[]	=	"5th Wave";
const char	BASS_LEAD_STRING[]	=	"Bass Lead";
const char	NEW_AGE_STRING[]	=	"New Age";
const char	WARM_STRING[]	=	"Warm";
const char	POLYSYNTH_STRING[]	=	"PolySynth";
const char	CHOIR_STRING[]	=	"Choir";
const char	BOWED_STRING[]	=	"Bowed";
const char	METALLIC_STRING[]	=	"Metallic";
const char	HALO_STRING[]	=	"Halo";
const char	SWEEP_STRING[]	=	"Sweep";
const char	RAIN_STRING[]	=	"Rain";
const char	SOUNDTRACK_STRING[]	=	"Soundtrack";
const char	CRYSTAL_STRING[]	=	"Crystal";
const char	ATMOSPHERE_STRING[]	=	"Atmosphere";
const char	BRIGHTNESS_STRING[]	=	"Brightness";
const char	GOBLINS_STRING[]	=	"Goblins";
const char	ECHOES_STRING[]	=	"Echoes";
const char	SCI_FI_STRING[]	=	"Sci-Fi";
const char	SITAR_STRING[]	=	"Sitar";
const char	BANJO_STRING[]	=	"Banjo";
const char	SHAMISEN_STRING[]	=	"Shamisen";
const char	KOTO_STRING[]	=	"Koto";
const char	KALIMBA_STRING[]	=	"Kalimba";
const char	BAG_PIPE_STRING[]	=	"Bag pipe";
const char	FIDDLE_STRING[]	=	"Fiddle";
const char	SHANAI_STRING[]	=	"Shanai";
const char	TINKLE_BELL_STRING[]	=	"Tinkle Bell";
const char	AGOGO_STRING[]	=	"Agogo";
const char	STEEL_DRUMS_STRING[]	=	"Steel Drums";
const char	WOODBLOCK_STRING[]	=	"Woodblock";
const char	TAIKO_DRUM_STRING[]	=	"Taiko Drum";
const char	MELODIC_TOM_STRING[]	=	"Melodic Tom";
const char	SYNTH_DRUM_STRING[]	=	"Synth Drum";
const char	REVERSE_CYMBAL_STRING[]	=	"Reverse Cymbal";
const char	GUITAR_FRET_NOISE_STRING[]	=	"Guitar Fret Noise";
const char	BREATH_NOISE_STRING[]	=	"Breath Noise";
const char	SEASHORE_STRING[]	=	"Seashore";
const char	BIRD_TWEET_STRING[]	=	"Bird Tweet";
const char	TELEPHONE_RING_STRING[]	=	"Telephone Ring";
const char	HELICOPTER_STRING[]	=	"Helicopter";
const char	APPLAUSE_STRING[]	=	"Applause";
const char	GUNSHOT_STRING[]	=	"Gunshot";


const char*	MIDI_GM_INSTRUMENT_NAMES[] = {
		ACC_GRAND_PIANO_STRING,
		BRIGHT_ACC_PIANO_STRING,
		ELEC_GRAND_PIANO_STRING,
		HONKY_TONK_PIANO_STRING,
		ELECTRIC_PIANO_1_STRING,
		ELECTRIC_PIANO_2_STRING,
		HARPSICHORD_STRING,
		CLAVI_STRING,
		CELESTA_STRING,
		GLOCKENSPIEL_STRING,
		MUSIC_BOX_STRING,
		VIBRAPHONE_STRING,
		MARIMBA_STRING,
		XYLOPHONE_STRING,
		TUBULAR_BELLS_STRING,
		DULCIMER_STRING,
		DRAWBAR_ORGAN_STRING,
		PERCUSSIVE_ORGAN_STRING,
		ROCK_ORGAN_STRING,
		CHURCH_ORGAN_STRING,
		REED_ORGAN_STRING,
		ACCORDION_STRING,
		HARMONICA_STRING,
		TANGO_ACCORDION_STRING,
		NYLON_GUITAR_STRING,
		STEEL_GUITAR_STRING,
		JAZZ_GUITAR_STRING,
		CLEAN_GUITAR_STRING,
		MUTED_GUITAR_STRING,
		OVERDRIVEN_GUITAR_STRING,
		DISTORTION_GUITAR_STRING,
		GUITAR_HARMONICS_STRING,
		ACOUSTIC_BASS_STRING,
		FINGERED_BASS_STRING,
		PICKED_BASS_STRING,
		FRETLESS_BASS_STRING,
		SLAP_BASS_1_STRING,
		SLAP_BASS_2_STRING,
		SYNTH_BASS_1_STRING,
		SYNTH_BASS_2_STRING,
		VIOLIN_STRING,
		VIOLA_STRING,
		CELLO_STRING,
		CONTRABASS_STRING,
		TREMOLO_STRINGS_STRING,
		PIZZICATO_STRINGS_STRING,
		ORCHESTRAL_HARP_STRING,
		TIMPANI_STRING,
		STRING_ENSEMBLE_1_STRING,
		STRING_ENSEMBLE_2_STRING,
		SYNTHSTRINGS_1_STRING,
		SYNTHSTRINGS_2_STRING,
		CHOIR_AAHS_STRING,
		VOICE_OOHS_STRING,
		SYNTH_VOICE_STRING,
		ORCHESTRA_HIT_STRING,
		TRUMPET_STRING,
		TROMBONE_STRING,
		TUBA_STRING,
		MUTED_TRUMPET_STRING,
		FRENCH_HORN_STRING,
		BRASS_SECTION_STRING,
		SYNTHBRASS_1_STRING,
		SYNTHBRASS_2_STRING,
		SOPRANO_SAX_STRING,
		ALTO_SAX_STRING,
		TENOR_SAX_STRING,
		BARITONE_SAX_STRING,
		OBOE_STRING,
		ENGLISH_HORN_STRING,
		BASSOON_STRING,
		CLARINET_STRING,
		PICCOLO_STRING,
		FLUTE_STRING,
		RECORDER_STRING,
		PAN_FLUTE_STRING,
		BLOWN_BOTTLE_STRING,
		SHAKUHACHI_STRING,
		WHISTLE_STRING,
		OCARINA_STRING,
		SQUARE_WAVE_STRING,
		SAW_WAVE_STRING,
		CALLIOPE_STRING,
		CHIFF_STRING,
		CHARANG_STRING,
		VOICE_STRING,
		FIFTH_WAVE_STRING,
		BASS_LEAD_STRING,
		NEW_AGE_STRING,
		WARM_STRING,
		POLYSYNTH_STRING,
		CHOIR_STRING,
		BOWED_STRING,
		METALLIC_STRING,
		HALO_STRING,
		SWEEP_STRING,
		RAIN_STRING,
		SOUNDTRACK_STRING,
		CRYSTAL_STRING,
		ATMOSPHERE_STRING,
		BRIGHTNESS_STRING,
		GOBLINS_STRING,
		ECHOES_STRING,
		SCI_FI_STRING,
		SITAR_STRING,
		BANJO_STRING,
		SHAMISEN_STRING,
		KOTO_STRING,
		KALIMBA_STRING,
		BAG_PIPE_STRING,
		FIDDLE_STRING,
		SHANAI_STRING,
		TINKLE_BELL_STRING,
		AGOGO_STRING,
		STEEL_DRUMS_STRING,
		WOODBLOCK_STRING,
		TAIKO_DRUM_STRING,
		MELODIC_TOM_STRING,
		SYNTH_DRUM_STRING,
		REVERSE_CYMBAL_STRING,
		GUITAR_FRET_NOISE_STRING,
		BREATH_NOISE_STRING,
		SEASHORE_STRING,
		BIRD_TWEET_STRING,
		TELEPHONE_RING_STRING,
		HELICOPTER_STRING,
		APPLAUSE_STRING,
		GUNSHOT_STRING,
};




uint8_t MIDIUtils_GetMusicNote(uint8_t note)
{
	note = note - (NOTE_COUNT * MIDIUtils_GetOctave(note));
	return note;
}


/* Returns the octave of the passed note */
uint8_t MIDIUtils_GetOctave(uint8_t note)
{
  return (note / NOTE_COUNT);
}


const char* MIDIUtils_GetInstrumentName(uint8_t programNumber)
{
	return MIDI_GM_INSTRUMENT_NAMES[programNumber];
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
