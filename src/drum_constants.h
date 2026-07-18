/*
  drum_constants.h - Library focused on transform Arduino microcontroller into a MIDI drum module
  
  With MatDrum library you able to transform an Arduino circuit into a functional MIDI drum module, which is possible
  to modify important electronic drum parameters settings, like sensitivity, threshold, scan time and mask time.

  Author  : Mattah
  Version : 1.0.0
  Created : 16/07/2026
  Modified: 18/07/2026
  License : MIT 
*/

// total drum pieces/notes to use
#define QUANTITY_PIECES    16

#define LOWEST_MIDI_VALUE  35

#define HIGHEST_MIDI_VALUE 81

#define MIDI_NOTE_TOTAL ((HIGHEST_MIDI_VALUE - LOWEST_MIDI_VALUE) + 1)

/*
// DRUM_PIECE
#define KICK_POS         0
#define SNARE_POS        1
#define RIM_POS          2
#define TOM_1_POS        3
#define TOM_2_POS        4
#define TOM_3_POS        5
#define TOM_4_POS        6
#define HH_BOW_POS       7
#define HH_EDGE_POS      8
#define CRASH_1_POS      9
#define CRASH_2_POS     10
#define CYMBAL_1_POS    11
#define CYMBAL_2_POS    12
#define RIDE_BOW_POS    13
#define RIDE_BELL_POS   14
#define RIDE_EDGE_POS   15
*/

// MIDI_NOTE - That you need to use in any function who requires the number of drum piece
  // REMEMBER: that's the General MIDI notes specifications. If necessary, you can change the number of these
  // constants to ensure the program transmits data correctly.
#define ACOUSTIC_BASS_DRUM 35
#define BASS_DRUM          36 
#define RIM                37
#define SNARE              38 
#define HAND_CLAP          39
#define RIM_SHOT           40 
#define LOW_FLOOR_TOM      41 
#define HIGH_FLOOR_TOM     43 
#define LOW_TOM            45 
#define LOW_MID_TOM        47
#define LOW_HIGH_MID_TOM   48
#define HIGH_TOM           50
#define CLOSED_HH          42 
#define OPEN_HH            46 
#define FOOT_HH            44 
#define FOOT_SPLASH_HH     28
#define CRASH_1_BOW        49
#define CRASH_1_EDGE       // define
#define CRASH_2_BOW        57
#define CRASH_2_EDGE       // define 
#define RIDE_1             51 
#define RIDE_2             59 
#define RIDE_BELL          53 
#define SPLASH             55 
#define CHINESE            52 
#define COWBELL            56
#define TAMBOURINE         54
#define VIBRASLAP          58
#define HI_BONGO           60
#define LOW_BONGO          61
#define LOW_CONGA          64
#define MUTE_HI_CONGA      62
#define OPEN_HI_CONGA      63
#define LOW_TIMBALE        66
#define HIGH_TIMBALE       65
#define LOW_AGOGO          68
#define HIGH_AGOGO         67
#define CABASA             69
#define MARACAS            70
#define SHORT_WHISTLE      71
#define LONG_WHISTLE       72
#define SHORT_GUIRO        73
#define LONG_GUIRO         74
#define CLAVES             75
#define HI_WOOD_BLOCK      76
#define LOW_WOOD_BLOCK     77
#define MUTE_CUICA         78
#define OPEN_CUICA         79
#define MUTE_TRIANGLE      80
#define OPEN_TRIANGLE      81
