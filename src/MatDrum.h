/*
  MatDrum.h - Library focused on transform Arduino microcontroller into a MIDI drum module
  
  With MatDrum library you able to transform an Arduino circuit into a functional MIDI drum module, which is possible
  to modify important electronic drum parameters settings, like sensitivity, threshold, scan time and mask time.

  Author  : Mattah
  Version : 1.0.0
  Created : 16/07/2026
  Modified: 18/07/2026
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
  // variables to store trigger setings (sensitivity, threshold, scan time, mask time, velocity curve)
   // index == drum pieces quantity
  float    sensitivity[HIGHEST_MIDI_VALUE];
  int        threshold[HIGHEST_MIDI_VALUE]; 
  int        scan_time[HIGHEST_MIDI_VALUE]; 
  int        mask_time[HIGHEST_MIDI_VALUE]; 
  int   velocity_curve[HIGHEST_MIDI_VALUE];
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
  bool hit_previous_note[HIGHEST_MIDI_VALUE];
  bool read_start;
  // time variables
  unsigned long previous_time_scan;
  unsigned long  current_time_scan;
  unsigned long previous_time_mask;
  unsigned long  current_time_mask;
  // channel variable
  byte channel;
  // MIDI communication
  MyMIDI MIDI;

public:

  // constructor
  MatDrum();

  // initialize MIDI communication
  void begin(byte channel);

  void end();

  void set_sensitivity(const int DRUM_PIECE, int sens_value);

  void set_threshold(const int DRUM_PIECE, int thold_value);

  void set_scan_time(const int DRUM_PIECE, int scan_value);

  void set_mask_time(const int DRUM_PIECE, int mask_time);

  void set_velocity_curve(const int DRUM_PIECE, const int velo_curve);

  bool is_out_scan_time(const int DRUM_PIECE);

  bool is_out_mask_time(const int DRUM_PIECE);

  bool is_out_threshold(const int DRUM_PIECE, int velocity);

  byte read_sensor(const byte ANALOG_PIN);

  byte filter_signal(int raw_signal);

  void send_note(const byte ANALOG_PIN, const byte DRUM_PIECE);

}

#endif