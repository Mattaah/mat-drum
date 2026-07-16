/*
  MatDrum.cpp - Library focused on transform Arduino microcontroller into a MIDI drum module
  
  With MatDrum library you able to transform an Arduino circuit into a functional MIDI drum module, which is possible
  to modify important electronic drum parameters settings, like sensitivity, threshold, scan time and mask time.

  Author  : Mattah
  Version : 1.0.0
  Created : 16/07/2026
  Modified: 16/07/2026
  License : MIT 
*/

#include "Arduino.h"
#include "MyMIDI.h"
#include "drum_constants.h"

MatDrum::MatDrum()
{
  // -1 == arbitrary value

  for (int x = 0; x < QUANTITY_PIECES; x = x + 1)
  {
    sensibility[x] = -1;
    threshold[x]   = -1;
    scan_time[x]   = -1;
    mask_time[x]   = -1;
    velo_curve[x]  = -1;
  }

  signal_kick      = -1;
  signal_snare     = -1;
  signal_rim       = -1;
  signal_tom_1     = -1;
  signal_tom_2     = -1;
  signal_tom_3     = -1;
  signal_tom_4     = -1;
  signal_hh_bow    = -1;
  signal_hh_edge   = -1;
  signal_crash_1   = -1;
  signal_crash_2   = -1;
  signal_crash_3   = -1;
  signal_crash_4   = -1;
  signal_ride_bow  = -1;
  signal_ride_bell = -1;
  signal_ride_edge = -1;

  read_start = false;
}

void MatDrum::begin()
{
  if (!read_start)
  { 
    MyMIDI MIDI;
    MIDI.begin(); 

    read_start = true; 
  }
}

void MatDrum::set_sensitivity(const int drum_piece, int sens_value)
{

}




