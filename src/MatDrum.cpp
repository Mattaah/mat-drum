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
  // -1  && 0b11111111 == arbitrary value

  for (int x = 0; x < QUANTITY_PIECES; x = x + 1)
  {
    sensitivity[x] = -1.0f;
    threshold[x]   = -1;
    scan_time[x]   = -1;
    mask_time[x]   = -1;
    velo_curve[x]  = -1;

    hit_previous_note[x] = false;
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

  previous_time_scan = 0UL;
  current_time_scan  = 0UL;
  previous_time_mask = 0UL;
  current_time_mask  = 0UL;

  channel = 0b11111111;
}

void MatDrum::begin(byte channel)
{
                    // 1                          // 16
  if (!read_start && 0b1 <= channel && channel <= 0b1111)
  {
    MIDI.begin(); 
    read_start = true;
  }
}

void MatDrum::end()
{
  if (read_start)
  { MIDI.end(); }
}

void MatDrum::set_sensitivity(const int DRUM_PIECE, int sens_value)
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && (1 <= sens_value && sens_value <= 32))
  {
    map (sens_value, 1, 32, 0.2, 2.0);
    sensitivity[DRUM_PIECE] = sens_value;
  }
}

void MatDrum::set_threshold(const int DRUM_PIECE, int thold_value);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && (0 <= thold_value && thold_value <= 31))
  {                             // bigger horizontal asymptote (adjust if necessary)
    map (thold_value, 0, 31, 0, (0.8 * 127)); 
    threshold[DRUM_PIECE] = thold_value;
  }
}

void MatDrum::set_scan_time(const int DRUM_PIECE, int scan_value);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && (0 <= scan_value && scan_value <= 4))
  { scan_time[DRUM_PIECE] = scan_value; }
}

void MatDrum::set_mask_time(const int DRUM_PIECE, int mask_time);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && (0 <= mask_time && mask_time <= 64))
  { mask_time[DRUM_PIECE] = mask_time; }
}

void MatDrum::set_velocity_curve(const int DRUM_PIECE, const int velo_curve);

bool MatDrum::is_out_scan_time(const int DRUM_PIECE)
{
  bool answer = false;

  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && !hit_previous_note[DRUM_PIECE])
  {
    current_time_scan = millis();

    if (current_time_scan - previous_time_scan > scan_time[DRUM_PIECE])
    { answer = true; previous_time_scan = current_time_scan; }
  }

  return (answer);
}

bool MatDrum::is_out_mask_time(const int DRUM_PIECE)
{
  bool answer = false;

  if ((0 <= DRUM_PIECE && DRUM_PIECE < QUANTITY_PIECES) && hit_previous_note[DRUM_PIECE])
  {
    current_time_mask = millis();

    if (current_time_mask - previous_time_mask > mask_time[DRUM_PIECE])
    { answer = true; previous_time_mask = current_time_mask; }
  }

  return (answer);
}

int MatDrum::read_sensor(const byte ANALOG_PIN)
{ return (analogWrite(ANALOG_PIN); }

byte MatDrum::filter_signal(const byte DRUM_PIECE, int raw_signal)
{
  byte  new_signal     =  0b0;
  float new_sensitivty = 0.0f;

  if (0 <= raw_signal && raw_signal <= 1023)
  {
    velocity = map(raw_signal, 0, 1023, 0, 127);
    new_sensitivty = map(sensitivity[DRUM_PIECE], 1, 32, 0.2, 2.0);

    new_signal = velocity * new_sensitivty;
    if (new_signal > 127)
    { new_signal = 127; }
  }
  else
  { new_signal = BYTE_ERROR; }

  return (new_signal);
}

void MatDrum::send_note(const byte ANALOG_PIN, const byte MIDI_NOTE)
{
  byte scan_tmp = 0b0, mask_tmp = 0b0, thold_tmp = 0b0, velocity_tmp = 0b0;
  int sensor_tmp = 0;

  scan_tmp  = scan_time[MIDI_NOTE];
  mask_tmp  = mask_time[MIDI_NOTE];
  thold_tmp = threshold[MIDI_NOTE];

  sensor_tmp   =   read_sensor(ANALOG_PIN);
  velocity_tmp = filter_signal(sensor_tmp);

  if ((is_out_scan_time(MIDI_NOTE) && is_out_mask_time(MIDI_NOTE) && is_out_threshold(MIDI_NOTE)) || 
      (scan_tmp == 0 && mask_tmp == 0 && thold_tmp == 0))
  {
    if (velocity_tmp != 0)
    { send_note_on(10, MIDI_NOTE, velocity_tmp);  hit_previous_note[MIDI_NOTE] = true;  }
    else
    { send_note_off(10, MIDI_NOTE, velocity_tmp); hit_previous_note[MIDI_NOTE] = false; }
  }
}
