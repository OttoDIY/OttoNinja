// Otto DIY invests time and resources providing this open source code,
// please support Otto DIY and open-source hardware by purchasing products from Otto DIY! www.ottodiy.com
// Modified by Camilo Parra Palacio for Otto DIY.
// BSD license, all text above must be included in any redistribution.
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <Servo.h> 
#include <Oscillator.h>
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
#define MATRIX_EYES         0
#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
const int sensorPin = A0;
Adafruit_8x16matrix matrix[4] = { // Array of Adafruit_8x16matrix objects
  Adafruit_8x16matrix(), Adafruit_8x16matrix(),
  Adafruit_8x16matrix(), Adafruit_8x16matrix() };
  
// Rather than assigning matrix addresses sequentially in a loop, each
// has a spot in this array.  This makes it easier if you inadvertently
// install one or more matrices in the wrong physical position --
// re-order the addresses in this table and you can still refer to
// matrices by index above, no other code or wiring needs to change.
static const uint8_t matrixAddr[] = { 0x70, 0x71, 0x72, 0x73 };
static const uint8_t PROGMEM // Bitmaps are stored in program memory
  blinkImg[][8] = {    // Eye animation frames
  { B00000000,         // Fully open eye
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B00000000 },
  { B00011000,
    B00011000,
    B00111110,
    B01111110,
    B01111110,
    B00111110,
    B00111100,
    B00011000 },
  { B00011000,
    B00011000,
    B00011100,
    B00111100,
    B00111100,
    B00011100,
    B00011000,
    B00011000 },
  { B00001000,
    B00011000,
    B00001100,
    B00001100,
    B00001100,
    B00001100,
    B00011000,
    B00001000 },
  { B00001000,         // Fully closed eye
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00001000 } },
   logo_bmp[] =
  {  B01111110,B10000001,B10111001,B10101001,B10111001,B10010001,B10111001,B10010001,B10010001,B10111001,B10010001,B10111001,B10101001,B10111001,B10000001,B01111110},
  happy_bmp[] =
  {  B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000,B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000},
  eyes_bmp[] =
  {  B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000,B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000},
   sad_bmp[] =
  {  B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000,B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000},
   xx_bmp[] =
  {  B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000},
  XX_bmp[] =
  {  B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001,B00000000,B00000000,B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001},
  angry_bmp[] =
  {  B00000000,B00011110,B00111100,B01111000,B01110000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B01110000,B01111000,B00111100,B00011110,B00000000},
   angry2_bmp[] =
  {  B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000},
  sleep_bmp[] =
  {  B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000}, 
  freetful_bmp[] =
  {  B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000},
   love_bmp[] =
  {  B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000,B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000},
  confused_bmp[] =
  {  B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000,B00000000,B11111111,B10000001,B10111101,B10100101,B10000101,B00000101,B11111101},
   wave_bmp[] =
  {  B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000,B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000},
   magic_bmp[] =
  {  B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000},
   fail_bmp[] =
  {  B00000000,B00110000,B01111000,B01111000,B01111100,B00111100,B00001000,B00000000,B00000000,B00001000,B00111100,B01111100,B01111000,B01111000,B00110000,B00000000},
  full_bmp[] =
  {   B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111 };

uint8_t
  blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }, // Blink bitmap sequence
  blinkCountdown = 200, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  100; // Duration of eye movement (smaller = faster)
int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position

void setup() {
  randomSeed(analogRead(A0));   // Seed random number generator from an unused analog input
 Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.sing(S_connection); //Otto wake up!

     for(uint8_t i=0; i<4; i++) {   // Initialize each matrix object:
    matrix[i].begin(matrixAddr[i]); 
  }
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].writeDisplay();
    delay(500);
    
Otto.home();
    matrix[MATRIX_EYES].setTextSize(1);
  matrix[MATRIX_EYES].setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix[MATRIX_EYES].setTextColor(LED_ON);
  matrix[MATRIX_EYES].setRotation(1);
  for (int8_t x=25; x>=-80; x--) {
    matrix[MATRIX_EYES].clear();
    matrix[MATRIX_EYES].setCursor(x,0);
    matrix[MATRIX_EYES].print("Hello I am Otto");
    matrix[MATRIX_EYES].writeDisplay();
    delay(100);       
  }
  delay(1000);
  Otto.shakeLeg(1,1000,1);
    matrix[MATRIX_EYES].setRotation(0);
 matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, happy_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(2000);
    Otto.playGesture(OttoSuperHappy); 
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, eyes_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, sad_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoSad);
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, angry_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  Otto.playGesture(OttoAngry); 
  delay(1000);
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, sleep_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoSleeping);
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, freetful_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoFretful);
  delay(1000);
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, love_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoLove);
  delay(1000);
  matrix[MATRIX_EYES].clear();
   matrix[MATRIX_EYES].drawBitmap(0, 0, confused_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  Otto.playGesture(OttoConfused);
  delay(1000);
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, wave_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, magic_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoMagic);
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, fail_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, xx_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
    Otto.playGesture(OttoFart);
  delay(1000);
   matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, XX_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);
  Otto.playGesture(OttoVictory);
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, logo_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);
  
 matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 8, full_bmp, 8, 8, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);

  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, full_bmp, 8, 8, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(1000);
 
  matrix[MATRIX_EYES].clear();      // clear display
  matrix[MATRIX_EYES].drawPixel(0, 0, LED_ON);  
  matrix[MATRIX_EYES].writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawLine(0,0, 7,15, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawRect(0,0, 8,16, LED_ON);
  matrix[MATRIX_EYES].fillRect(2,2, 4,12, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawCircle(3,8, 3, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();  // write the changes we just made to the display
  delay(500);
  Otto.playGesture(OttoHappy);
matrix[MATRIX_EYES].setRotation(0);

}
void loop() {

   int estado = digitalRead(sensorPin);
   if (estado == HIGH) {
    Otto.bend(1,1000,-1);
    delay(100);
    matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, love_bmp, 8, 16, LED_ON);
  matrix[MATRIX_EYES].writeDisplay();
  delay(500);
   Otto.playGesture(OttoLove); 
   Otto.jump(1,1000);
   delay(1000);
    
    }
    else{
  // Draw eyeball in current state of blinkyness (no pupil).  Note that
  // only one eye needs to be drawn.  Because the two eye matrices share
  // the same address, the same data will be received by both.
  matrix[MATRIX_EYES].clear();
  // When counting down to the next blink, show the eye in the fully-
  // open state.  On the last few counts (during the blink), look up
  // the corresponding bitmap index.
  matrix[MATRIX_EYES].drawBitmap(0, 0,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
      blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
      0                                       // No, show bitmap 0
    ], 8, 8, LED_ON);
     matrix[MATRIX_EYES].drawBitmap(0, 8,
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
    matrix[MATRIX_EYES].fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      matrix[MATRIX_EYES].fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      8+newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
    if(gazeCountdown == 0) {    // Last frame?
      eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
      do { // Pick random positions until one is within the eye circle
        newX = random(7); newY = random(7);
        dX   = newX - 3;  dY   = newY - 3;
      } while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
      dX            = newX - eyeX;             // Horizontal distance to move
      dY            = newY - eyeY;             // Vertical distance to move
      gazeFrames    = random(3, 15);           // Duration of eye movement
      gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
             Otto.walk(2,1000,1);
             delay(500);
              matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0, happy_bmp, 8, 16, LED_ON);
   matrix[MATRIX_EYES].writeDisplay();
   Otto.playGesture(OttoHappy); 
   Otto.moonwalker(2,1000,30,1);
      Otto.sing(S_happy_short);
   Otto.moonwalker(2,1000,30,-1);
   delay(1000);
    }
  } else {
    // Not in motion yet -- draw pupil at current static position
    matrix[MATRIX_EYES].fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    matrix[MATRIX_EYES].fillRect(eyeX, 8+eyeY, 2, 2, LED_OFF);

  }
  // Refresh all of the matrices in one quick pass
  for(uint8_t i=0; i<4; i++) matrix[i].writeDisplay();
  delay(20); // ~50 FPS
    }}
