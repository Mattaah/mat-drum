/*
  MatDrum.cpp - Library focused on transform Arduino microcontroller into a MIDI drum module
  
  With MatDrum library you able to transform an Arduino circuit into a functional MIDI drum module, which is possible
  to modify important electronic drum parameters settings, like sensitivity, threshold, scan time and mask time.

  Author  : Mattah
  Version : 1.0.0
  Created : 16/07/2026
  Modified: 18/07/2026
  License : MIT 
*/

#include "Arduino.h"
#include "MyMIDI.h"
#include "drum_constants.h"

MatDrum::MatDrum()
{
  // -1  && 0b11111111 == arbitrary value

  for (int x = 0; x < HIGHEST_MIDI_VALUE; x = x + 1)
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
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (1 <= sens_value && sens_value <= 32))
  {
    map (sens_value, 1, 32, 0.2, 2.0);
    sensitivity[DRUM_PIECE-1] = sens_value;
  }
}

void MatDrum::set_threshold(const int DRUM_PIECE, int thold_value);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (0 <= thold_value && thold_value <= 31))
  {                             // bigger horizontal asymptote (adjust if necessary)
    map (thold_value, 0, 31, 0, (0.8 * 127)); 
    threshold[DRUM_PIECE-1] = thold_value;
  }
}

void MatDrum::set_scan_time(const int DRUM_PIECE, int scan_value);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (0 <= scan_value && scan_value <= 4))
  { scan_time[DRUM_PIECE-1] = scan_value; }
}

void MatDrum::set_mask_time(const int DRUM_PIECE, int mask_value);
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (0 <= mask_value && mask_value <= 64))
  { mask_time[DRUM_PIECE-1] = mask_value; }
}

void MatDrum::set_velocity_curve(const int DRUM_PIECE, const int velo_curve);

void MatDrum::set_rim_edge_gain(const int DRUM_PIECE, float gain_value)
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (1 <= gain_value && gain_value <= 3.2))
  {
    map (gain_value, 0, 3.2, 0.2, 2.0);
    sensitivity[DRUM_PIECE-1] = gain_value;
  }
}

void MatDrum::set_xstick_threshold(const int DRUM_PIECE, int thold_value)
{
  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && (0 <= thold_value && thold_value <= 127))
  { threshold[DRUM_PIECE-1] = thold_value; }
}

bool MatDrum::is_out_scan_time(const int DRUM_PIECE)
{
  bool answer = false;

  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && !hit_previous_note[DRUM_PIECE-1])
  {
    current_time_scan = millis();

    if (current_time_scan - previous_time_scan > scan_time[DRUM_PIECE-1])
    { answer = true; previous_time_scan = current_time_scan; }
  }

  return (answer);
}

bool MatDrum::is_out_mask_time(const int DRUM_PIECE)
{
  bool answer = false;

  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && hit_previous_note[DRUM_PIECE-1])
  {
    current_time_mask = millis();

    if (current_time_mask - previous_time_mask > mask_time[DRUM_PIECE-1])
    { answer = true; previous_time_mask = current_time_mask; }
  }

  return (answer);
}

bool MatDrum::is_out_threshold(const int DRUM_PIECE, int velocity)
{
  bool answer = false;

  if ((0 <= DRUM_PIECE && DRUM_PIECE < HIGHEST_MIDI_VALUE) && velocity > threshold[DRUM_PIECE-1])
  { answer = true; }

  return (answer);
}

int MatDrum::read_analog_sensor(const byte ANALOG_PIN)
{ return (analogWrite(ANALOG_PIN); }

int MatDrum::read_digital_sensor(const byte DIGITAL_PIN)
{ return (digitalWrite(DIGITAL_PIN); }

byte MatDrum::filter_signal(const byte DRUM_PIECE, unsigned int raw_signal)
{
  byte new_signal =  0b0;

  if (0 <= raw_signal && raw_signal <= 1023)
  {
    velocity = map(raw_signal, 0, 1023, 0, 127);

    new_signal = velocity * sensitivity[DRUM_PIECE-1];
    if (new_signal > 127)
    { new_signal = 127; }
  }

  return (new_signal);
}

void MatDrum::adjust_head(const int DRUM_PIECE, int sense, int thold, int scan_value, int mask_value)
{
  set_sensitivity(DRUM_PIECE, sense);
  set_threshold(DRUM_PIECE, thold);
  set_scan_time(DRUM_PIECE, scan_value);
  set_mask_time(DRUM_PIECE, mask_value);
}

void MatDrum:adjust_rim(const int DRUM_PIECE, int rim_gain, int xstick_thold)
{
  set_rim_edge_gain(rim_gain);
  set_xstick_threshold(xstick_thold);
}

void MatDrum:adjust_edge(const int DRUM_PIECE, int edge_gain)
{ set_rim_edge_gain(edge_gain); }

void MatDrum::hit_snare(const byte ANALOG_PIN_SNARE)
{ hit_note(ANALOG_PIN_SNARE, SNARE); }

void MatDrum::hit_rim(const byte ANALOG_PIN_RIM)
{
  byte velocity_tmp = 0b0;
  int sensor_tmp = 0;

  sensor_tmp   = read_analog_sensor(ANALOG_PIN_RIM);
  velocity_tmp = filter_signal(sensor_tmp);

  if (velocity_tmp != 0)
  {
    if (velocity_tmp >= threshold[RIM-1])
    { send_note_on(10, RIM_SHOT, velocity_tmp); hit_previous_note[RIM_SHOT-1] = true; }
    else
    { send_note_on(10, RIM,      velocity_tmp); hit_previous_note[RIM-1] = true; }
  }
  else
  { send_note_off(10, RIM, velocity_tmp); hit_previous_note[RIM-1] = false; }
}

void MatDrum::hit_edge(const byte ANALOG_PIN_EDGE, const int DRUM_PIECE)
{
  byte velocity_tmp = 0b0;
  int sensor_tmp = 0;

  sensor_tmp   = read_analog_sensor(ANALOG_PIN_EDGE);
  velocity_tmp = filter_signal(sensor_tmp);

  if (velocity_tmp != 0)
  { send_note_on(10, velocity_tmp); hit_previous_note[DRUM_PIECE-1] = true; }
  else
  { send_note_off(10, DRUM_PIECE, velocity_tmp); hit_previous_note[DRUM_PIECE-1] = false; }
}

void MatDrum::hit_edge_ride(const byte ANALOG_PIN_EDGE)
{
  byte velocity_tmp = 0b0;
  int sensor_tmp = 0;

  sensor_tmp   = read_analog_sensor(ANALOG_PIN_EDGE);
  velocity_tmp = filter_signal(sensor_tmp);

  if (velocity_tmp != 0)
  { send_note_on(10, velocity_tmp); hit_previous_note[RIM-1] = true; }
  else
  { send_note_off(10, RIM, velocity_tmp); hit_previous_note[RIM-1] = false; }
}

void MatDrum::hit_bell(const byte ANALOG_PIN_BELL)
{
  byte velocity_tmp = 0b0;
  int sensor_tmp = 0;

  sensor_tmp   = read_analog_sensor(ANALOG_PIN_BELL);
  velocity_tmp = filter_signal(sensor_tmp);

  if (velocity_tmp != 0)
  { send_note_on(10, velocity_tmp); hit_previous_note[RIDE_BELL-1] = true; }
  else
  { send_note_off(10, RIDE_BELL, velocity_tmp); hit_previous_note[RIDE_BELL-1] = false; }
}

void MatDrum::hit_ride(const byte ANALOG_PIN_BOW)
{ hit_note(ANALOG_PIN_BOW, RIDE_1); }

void MatDrum::hit_ride(const byte ANALOG_PIN_BOW, const byte ANALOG_PIN_BELL)
{ 
  hit_note(ANALOG_PIN_BOW, RIDE_1); 
  hit_bell(ANALOG_PIN_BELL);
}

void MatDrum::hit_ride(const byte ANALOG_PIN_BOW, const byte ANALOG_PIN_BELL, const byte ANALOG_PIN_EDGE)
{ 
  hit_note(ANALOG_PIN_BOW, RIDE_1); 
  hit_bell(ANALOG_PIN_BELL);
  hit_edge_ride(ANALOG_PIN_EDGE);
}

void MatDrum::hit_tom_1(const byte ANALOG_PIN_TOM_1)
{ hit_note(ANALOG_PIN_TOM_1, HIGH_TOM); }

void MatDrum::hit_tom_2(const byte ANALOG_PIN_TOM_2)
{ hit_note(ANALOG_PIN_TOM_2, LOW_HIGH_MID_TOM); }

void MatDrum::hit_tom_3(const byte ANALOG_PIN_TOM_3)
{ hit_note(ANALOG_PIN_TOM_3, HIGH_FLOOR_TOM); }

void MatDrum::hit_tom_4(const byte ANALOG_PIN_TOM_4)
{ hit_note(ANALOG_PIN_TOM_4, LOW_FLOOR_TOM); }

void MatDrum::hit_crash_1(const byte ANALOG_PIN_EDGE)
{ hit_edge(ANALOG_PIN_EDGE, CRASH_1_EDGE); }

void MatDrum::hit_crash_1(const byte ANALOG_PIN_EDGE, const byte ANALOG_PIN_BOW)
{
  hit_edge(ANALOG_PIN_EDGE, CRASH_1_EDGE);
  hit_note(ANALOG_PIN_BOW, CRASH_1_BOW);
}

void MatDrum::hit_crash_2(const byte ANALOG_PIN_EDGE)
{ hit_edge(ANALOG_PIN_EDGE, CRASH_2_EDGE); }

void MatDrum::hit_crash_2(const byte ANALOG_PIN_EDGE, const byte ANALOG_PIN_BOW)
{
  hit_edge(ANALOG_PIN_EDGE, CRASH_2_EDGE);
  hit_note(ANALOG_PIN_BOW, CRASH_2_BOW);
}

void MatDrum::hit_kick(const byte ANALOG_PIN_KICK)
{ hit_note(ANALOG_PIN_KICK, ACOUSTIC_BASS_DRUM); }

void MatDrum::hit_note(const byte ANALOG_PIN, const byte DRUM_PIECE)
{
  byte scan_tmp = 0b0, mask_tmp = 0b0, thold_tmp = 0b0, velocity_tmp = 0b0;
  int sensor_tmp = 0;

  scan_tmp  = scan_time[DRUM_PIECE-1];
  mask_tmp  = mask_time[DRUM_PIECE-1];
  thold_tmp = threshold[DRUM_PIECE-1];

  sensor_tmp   =   read_sensor(ANALOG_PIN);
  velocity_tmp = filter_signal(sensor_tmp);

  if ((is_out_scan_time(DRUM_PIECE) && is_out_mask_time(DRUM_PIECE) && is_out_threshold(DRUM_PIECE, velocity)) || 
      (scan_tmp == 0 && mask_tmp == 0 && thold_tmp == 0))
  {
    if (velocity_tmp != 0)
    { send_note_on(10, DRUM_PIECE, velocity_tmp);  hit_previous_note[DRUM_PIECE-1] = true;  }
    else
    { send_note_off(10, DRUM_PIECE, velocity_tmp); hit_previous_note[DRUM_PIECE-1] = false; }
  }
}
