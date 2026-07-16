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
  if ((drum_piece >= 0) && (1 <= sens_value && sens_value <= 32))
  {
    map (sens_value, 1, 32, 0.2, 2.0);
    sensibility[drum_piece] = sens_value;
  }
}

void MatDrum::set_threshold(const int drum_piece, int thold_value);
{
  if ((drum_piece >= 0) && (0 <= thold_value && thold_value <= 31))
  {                             // bigger horizontal asymptote possible
    map (thold_value, 0, 31, 0, (0.8 * 127)); 
    threshold[drum_piece] = thold_value;
  }
}

void MatDrum::set_scan_time(const int drum_piece, int scan_value);
{
  if ((drum_piece >= 0) && (0 <= scan_value && scan_value <= 4))
  { scan_time[drum_piece] = scan_value; }
}

void MatDrum::set_mask_time(const int drum_piece, int mask_time);
{
  if ((drum_piece >= 0) && (0 <= mask_time && mask_time <= 64))
  { mask_time[drum_piece] = mask_time; }
}


