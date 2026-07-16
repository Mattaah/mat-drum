/*
  MatDrum.h - Library focused on transform Arduino microcontroller into a MIDI drum module
  
  With MatDrum library you able to transform an Arduino circuit into a functional MIDI drum module, which is possible
  to modify important electronic drum parameters settings, like sensitivity, threshold, scan time and mask time.

  Author  : Mattah
  Version : 1.0.0
  Created : 16/07/2026
  Modified: 16/07/2026
  License : MIT 
*/

#ifndef MAT_DRUM_H
#define MAT_DRUM_H

#include "Arduino.h"
#include "MyMIDI.h"
#include "drum_constants.h"

class MatDrum
{
private:
  // variables to store trigger setings (sensibility, threshold, scan time, mask time, velocity curve)
   // index == drum pieces quantity
  int    sensibility[QUANTITY_PIECES];
  int      threshold[QUANTITY_PIECES]; 
  int      scan_time[QUANTITY_PIECES]; 
  int      mask_time[QUANTITY_PIECES]; 
  int velocity_curve[QUANTITY_PIECES];
  // variables to store data read
  int      signal_kick;
  int     signal_snare;
  int       signal_rim;
  int     signal_tom_1;
  int     signal_tom_2;
  int     signal_tom_3;
  int     signal_tom_4;
  int    signal_hh_bow;
  int   signal_hh_edge;
  int   signal_crash_1;
  int   signal_crash_2;
  int   signal_crash_3;
  int   signal_crash_4;
  int  signal_ride_bow;
  int signal_ride_bell;
  int signal_ride_edge;
  // control state variables
  bool read_start;

public:

  // constructor
  MatDrum();

  void begin();

  void end();

  void set_sensitivity(const int drum_piece, int sens_value);

  void set_threshold(const int drum_piece, int thold_value);

  void set_scan_time(const int drum_piece, int scan_value);

  void set_mask_time(const int drum_piece, int mask_time);
  
  void set_velocity_curve(const int drum_piece, const int velo_curve);
};

#endif