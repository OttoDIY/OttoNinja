// The Adafriend sketch is derived from P. Burgess's 'roboface' example sketch for Adafruit I2C 8x8 LED backpacks:
//
//  www.adafruit.com/products/870   www.adafruit.com/products/1049
//  www.adafruit.com/products/871   www.adafruit.com/products/1050
//  www.adafruit.com/products/872   www.adafruit.com/products/1051
//  www.adafruit.com/products/959   www.adafruit.com/products/1052
//
// Requires Adafruit_LEDBackpack and Adafruit_GFX libraries.
// For a simpler introduction, see the 'matrix8x8' example.
//
// Adafruit invests time and resources providing this open source code,
// please support Adafruit and open-source hardware by purchasing
// products from Adafruit!
//
// Eye algorithm written by P. Burgess for Adafruit Industries.
// Code/Guide written by John Wall for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.
// Arduino Flappy Bird homage by augustzf@gmail.com

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Tone.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

static const uint8_t  PROGMEM// Bitmaps are stored in program memory
sadBlinkImg[][8] = {    // Eye animation frames
  { 
    B00100100,         // Fully open sad eye
    B01000010,
    B10000001,
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B11111111       }
  ,
  { 
    B00100100,         
    B01000010,
    B10000001,
    B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111        }
  ,
  { 
    B00100100,         
    B01000010,
    B10000001,
    B00000000,
    B00000000,
    B01111110,
    B11111111,
    B11111111       }
  ,
  {
    B00100100,         
    B01000010,
    B10000001,
    B00000000,
    B00000000,
    B00000000,
    B01111110,
    B11111111        }
  ,
  { 
    B00100100,         // Fully closed sad eye
    B01000010,
    B10000001,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111       } 

};

static const uint8_t  PROGMEM// Bitmaps are stored in program memory
blinkImg[][8] = {    // Eye animation frames
  { 
    B00111100,         // Fully open eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100       }
  ,
  { 
    B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100       }
  ,
  { 
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000       }
  ,
  { 
    B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000       }
  ,
  { 
    B00000000,         // Fully closed eye
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000       } 
};

static const uint8_t  PROGMEM// Bitmaps are stored in program memory
happyBlinkImg[][8] = {    // Eye animation frames
  { 
    B00111100,         // Fully open happy eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B00000000,
    B10000001,
    B01111110       }
  ,
  { 
    B00000000,         
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B00000000,
    B10000001,
    B01111110       }
  ,
  { 
    B00000000,         
    B00000000,
    B01111110,
    B11111111,
    B11111111,
    B00000000,
    B10000001,
    B01111110       }
  ,
  { 
    B00000000,         
    B00000000,
    B00000000,
    B01111110,
    B11111111,
    B00000000,
    B10000001,
    B01111110       }
  ,
  { 
    B00000000,         // Fully closed happy eye      
    B00000000,
    B00000000,
    B01111110,
    B10000001,
    B00000000,
    B10000001,
    B01111110       } 
};

static const uint8_t PROGMEM // Bitmaps are stored in program memory
annoyedBlinkImg[][8] = {    // Eye animation frames
  { 
    B10000001,         // Fully open annoyed eye
    B01100110,
    B00000000,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100       }
  ,
  { 
    B10000001,       
    B01100110,
    B00000000,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00000000        }
  ,
  { 
    B10000001,       
    B01100110,
    B00000000,
    B11111111,
    B11111111,
    B01111110,
    B00000000,
    B00000000       }
  ,
  {
    B10000001,       
    B01100110,
    B00000000,
    B11111111,
    B01111110,
    B00000000,
    B00000000,
    B00000000        }
  ,
  { 
    B10000001,         // Fully closed annoyed eye
    B01100110,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000,
    B00000000       } 

};

uint8_t
blinkIndex[] = { 
  1, 2, 3, 4, 3, 2, 1 },
// Blink bitmap sequence
blinkCountdown = 100, // Countdown to next blink (in frames)
gazeCountdown  =  75, // Countdown to next eye movement
gazeFrames     =  50; // Duration of eye movement (smaller = faster)
int8_t
eyeX = 3, eyeY = 3,   // Current eye position
newX = 3, newY = 3,   // Next eye position
dX   = 0, dY   = 0;   // Distance from prior to new position

Tone tone1;

#define OCTAVE_OFFSET 0

const int notes[] = { 
  0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4, //scale of notes and their frequencies
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

const char *songs[] = { // A selection of songs that the Adafriend will sing when happy. Watch for memory limits! These 7 are tested and working.
"Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b",
"Indiana Jones:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
"Take On Me:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5",
"Tetris:d=4,o=5,b=150:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a",
"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5",
"Mahna Mahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.",
"The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6"
};

//char *song = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
//char *song = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
//char *song = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char *song = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//char *song = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
//char *song = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
//char *song = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//char *song = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
//char *song = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//char *song = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
//char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//char *song = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//char *song = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//char *song = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//char *song = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//char *song = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//char *song = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//char *song = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//char *song = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
//char *song = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
//char *song = "Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
//char *song = "tetris:d=4,o=5,b=150:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a";

int tapNum = 15; // logged number of taps
byte mood = 1; // current mood
const byte vibration PROGMEM = A0; // vibration sensor
const int tapLevel PROGMEM = 512;
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
const int decay PROGMEM = 30000;           // interval at which to decay emotions

unsigned long 
checkMillis,
 tapMillis,
 songMillis,
 currentMillis,
gameMillis;

void setup() {

  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(A7));
  pinMode(vibration, INPUT_PULLUP);

  matrix.begin(0x70);
  matrix.setRotation(3);
  matrix.setBrightness(4);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  
  matrix.setTextColor(LED_ON);
  matrix.clear();
  matrix.writeDisplay();
  tone1.begin(10);
}

void loop() {

  while(mood==0) // sad mood
  {
    // Draw eyeball in current state of blinkyness (no pupil).
    matrix.clear();
    // When counting down to the next blink, show the eye in the fully-
    // open state.  On the last few counts (during the blink), look up
    // the corresponding bitmap index.
    matrix.drawBitmap(0, 0,
    sadBlinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
    blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
    0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

    // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
    // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; 
        eyeY = newY; // Yes.  What's new is old, then...
        do { // Pick random positions until one is within the eye circle
          newX = random(0,7); 
          newY = random(5,7);
          dX   = newX-3;  
          dY   = newY-3;
        } 
        while((dX * dX + dY * dY) >= 10);        // Thank you Pythagoras
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeFrames    = random(3, 15);           // Duration of eye movement
        gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
      }
    }
    else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }


    // Refresh all of the matrices in one quick pass
    matrix.writeDisplay();

    if(millis()-checkMillis > random(10000,30000))
    {
      sadNoise();
      checkMillis = millis();
    }

    tapMillis = millis();

    while(millis()-tapMillis < 40)
    {
      checkTaps();
    }  
    songMillis = millis();
  }

  while(mood==1) // neutral mood
  {
    // Draw eyeball in current state of blinkyness (no pupil).
    matrix.clear();
    // When counting down to the next blink, show the eye in the fully-
    // open state.  On the last few counts (during the blink), look up
    // the corresponding bitmap index.
    matrix.drawBitmap(0, 0,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
    blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
    0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

    // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
    // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; 
        eyeY = newY; // Yes.  What's new is old, then...
        do { // Pick random positions until one is within the eye circle
          newX = random(7); 
          newY = random(7);
          dX   = newX - 3;  
          dY   = newY - 3;
        } 
        while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeFrames    = random(3, 15);           // Duration of eye movement
        gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
      }
    } 
    else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }


    // Refresh all of the matrices in one quick pass
    matrix.writeDisplay();
    
     if(millis()-checkMillis > random(10000,30000))
    {
      neutralNoise();
      checkMillis = millis();
    }


    tapMillis = millis();

    while(millis()-tapMillis < 40)
    {
      checkTaps();
    }  
    songMillis = millis();
  }

  while(mood == 2) // happy mood
  {
    // Draw eyeball in current state of blinkyness (no pupil).
    matrix.clear();
    // When counting down to the next blink, show the eye in the fully-
    // open state.  On the last few counts (during the blink), look up
    // the corresponding bitmap index.
    matrix.drawBitmap(0, 0,
    happyBlinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
    blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
    0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

    // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
    // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; 
        eyeY = newY; // Yes.  What's new is old, then...
        do { // Pick random positions until one is within the eye circle
          newX = random(7); 
          newY = random(4);
          dX   = newX - 3;  
          dY   = newY - 3;
        } 
        while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeFrames    = random(3, 15);           // Duration of eye movement
        gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
      }
    } 
    else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }


    // Refresh all of the matrices in one quick pass
    matrix.writeDisplay();

    if(millis()-checkMillis > random(10000,30000))
    {
      happyNoise();
      checkMillis = millis();
    }

    tapMillis = millis();

    while(millis()-tapMillis < 40)
    {
      checkTaps();
    }  

  }

  while(mood==3) // annoyed mood
  {
    // Draw eyeball in current state of blinkyness (no pupil).
    matrix.clear();
    // When counting down to the next blink, show the eye in the fully-
    // open state.  On the last few counts (during the blink), look up
    // the corresponding bitmap index.
    matrix.drawBitmap(0, 0,
    annoyedBlinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
    blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
    0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

    // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
    // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; 
        eyeY = newY; // Yes.  What's new is old, then...
        do { // Pick random positions until one is within the eye circle
          newX = random(7); 
          newY = random(3,7);
          dX   = newX - 3;  
          dY   = newY - 3;
        } 
        while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeFrames    = random(3, 15);           // Duration of eye movement
        gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
      }
    }
    else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }

    // Refresh all of the matrices in one quick pass
    matrix.writeDisplay();
    
    if(millis()-checkMillis > random(10000,30000))
    {
      angryNoise();
      checkMillis = millis();
    }

    tapMillis = millis();

    while(millis()-tapMillis < 40)
    {
      checkTaps();
    }    
  }
}

#define isdigit(n) (n >= '0' && n <= '9')

void play_rtttl(const char *p) // the method to play a song
{
  // Absolutely no error checking in here

  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; 
    p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }



  // get default octave
  if(*p == 'o')
  {
    p++; 
    p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }



  // get BPM
  if(*p == 'b')
  {
    p++; 
    p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }



  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)




  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }

    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

      // now get the note
    note = 0;

    switch(*p)
    {
    case 'c':
      note = 1;
      break;
    case 'd':
      note = 3;
      break;
    case 'e':
      note = 5;
      break;
    case 'f':
      note = 6;
      break;
    case 'g':
      note = 8;
      break;
    case 'a':
      note = 10;
      break;
    case 'b':
      note = 12;
      break;
    case 'p':
    default:
      note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      tone1.play(notes[(scale - 4) * 12 + note]);
      delay(duration);
      tone1.stop();
    }
    else
    {

      delay(duration);
    }
  }
}

void checkTaps()
{
  if(analogRead(vibration)<tapLevel)
  {
    tapNum++;
    tone1.play(NOTE_C5);
    delay(200);
    tone1.stop();
  }

  if(tapNum <= 10) mood = 0;
  else if(tapNum <= 20) mood = 1;
  else if(tapNum <= 30) mood = 2;
  else if(tapNum > 30) mood = 3;

  currentMillis = millis();

  if(currentMillis - previousMillis > decay) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    tapNum--;
    if(tapNum < 0) tapNum = 0;
    if(tapNum == 30) tapNum = 15;
    if(tapNum > 40) tapNum = 40;
  }

  currentMillis = millis();

  if(currentMillis - songMillis > random(30000,60000) && mood == 2)
  {
    matrix.clear();

    matrix.drawBitmap(0, 0, happyBlinkImg[0], 8, 8, LED_ON);
    eyeX = 3;
    eyeY = 3;
    matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    matrix.writeDisplay();

    play_rtttl(songs[random(7)]);
    previousMillis = millis();
    checkMillis = millis();
    songMillis = millis();
  }
}

void happyNoise()
{
  int note = (int)random(13,39);
  tone1.play(notes[note]);
  delay(200);
  tone1.stop();
  tone1.play(notes[note+1]);
  delay(100);
  tone1.stop();
  tone1.play(notes[note+2]);
  delay(100);
  tone1.stop();
}

void sadNoise()
{
  int note = (int)random(2,26);
  tone1.play(notes[note]);
  delay(200);
  tone1.stop();
  tone1.play(notes[note-1]);
  delay(100);
  tone1.stop();
  tone1.play(notes[note-2]);
  delay(100);
  tone1.stop();
}

void angryNoise()
{
  int note = (int)random(2,7);
  tone1.play(notes[note]);
  delay(100);
  tone1.stop();
  tone1.play(notes[note-1]);
  delay(200);
  tone1.stop();
}

void neutralNoise()
{
  int note = (int)random(2,26);
  tone1.play(notes[note]);
  delay(200);
  tone1.stop();
}