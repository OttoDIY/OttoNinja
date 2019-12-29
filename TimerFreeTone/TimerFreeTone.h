// ---------------------------------------------------------------------------
// TimerFreeTone Library - v1.5 - 09/12/2016
//
// AUTHOR/LICENSE:
// Created by Tim Eckel - teckel@leethost.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: https://bitbucket.org/teckel12/arduino-timer-free-tone
// Blog: http://forum.arduino.cc/index.php?topic=235774.0
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Doesn't use timers which frees up conflicts with other libraries. Compatible
// with all ATmega, ATtiny and ARM-based microcontrollers. About 1,500 bytes
// smaller binary sketch size than the standard tone library. Exclusive use of
// port registers for AVR-based microcontrollers for fastest and smallest code.
// Close to a plug-in replacement for the standard Tone library.
//
// SYNTAX:
//   TimerFreeTone( pin, frequency, duration [, volume ] ) - Play a note on pin at frequency in Hz for duration in milliseconds.
//     Parameters:
//       * pin        - Pin speaker is wired to (other wire to ground, be sure to add an inline 100 ohm resistor).
//       * frequency  - Play the specified frequency (should work fairly well in the 100 to 15000 Hz range).
//       * duration   - Set the duration to play in milliseconds. Range: 0 to 65535 (65.5 seconds).
//       * volume     - Optionally set the tone volume level (from 1 to 10), defaults to full volume (10).
//
// HISTORY:
// 09/12/2016 v1.5 - Fixed problem with latest release of the Arduino IDE which
// caused the library to totally stop functioning. Adjusted note duration to
// not fail on timer rollover. Now delays for note duration when frequency or
// volume are zero.
//
// 08/05/2016 v1.4 - Added optional volume parameter.
//
// 07/23/2016 v1.3 - Fixed problem with long tone play durations. Changed the
// way the note duration is calculated from a suggestion by Paul Stoffregen
// (http://www.pjrc.com/teensy/).
//
// 01/14/2015 v1.2 - Calculates duration differently for higher tone accuracy
// and smaller code size.
//
// 04/30/2014 v1.1 - Automatically sets mode of pin to OUTPUT as does the
// standard Tone library. Sets pinOutput variable to volatile to work with
// certain microcontrollers. Removed overhead parameter and calculation, fairly
// accurate anyway at audible frequencies. Even smaller binary sketch size.
//
// 04/25/2014 v1.0 - Initial release.
//
// ---------------------------------------------------------------------------

#ifndef TimerFreeTone_h
#define TimerFreeTone_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#if defined(ESP32)
#include "ESP32Tone.h"
#endif

void TimerFreeTone(uint8_t pin, unsigned long frequency, unsigned int duration, uint8_t volume = 10);
#endif
