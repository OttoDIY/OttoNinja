/***************************************************
  Designed specifically to work with the Otto DIY 16x8 LED Matrix eyes

  These display use I2C to communicate, 2 pins are required to
  interface. There are multiple selectable I2C addresses. For
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Otto DIY invests time and resources providing this open source code,
  please support Otto DIY and open-source hardware by purchasing
  products from ottodiy.com!

  Written by Camilo Parra Palacio for Otto DIY.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "OLED_Zoom.h"
OLED_Zoom matrix = OLED_Zoom(128, 64, OLEDZoomX8);

#ifndef LED_ON
#define LED_ON 1
#endif



void setup() {
  Wire.begin(SDA, SCL, 400000);
  Serial.begin(9600);
  Serial.println("16x8 LED Matrix Test");
  matrix.begin(SSD1306_SWITCHCAPVCC, 0x3D, true, true, false); // pass in the address

    matrix.clear();
  matrix.display();
}

void delayBetweenDisplays(byte mult) {
  delay(mult * 0);
}

void loop() {
  static const uint8_t PROGMEM
  logo18_bmp[] = {  B00000000, B01111110, B10000001, B10111001, B10101001, B10111001, B10010001, B10111001, B10010001, B10010001, B10111001, B10010001, B10111001, B10101001, B10111001, B10000001, B01111110, B00000000},
  logo_bmp[] = {  B01111110, B10000001, B10111001, B10101001, B10111001, B10010001, B10111001, B10010001, B10010001, B10111001, B10010001, B10111001, B10101001, B10111001, B10000001, B01111110},
  happy_bmp[] = {  B00000000, B00111100, B00000010, B00000010, B00000010, B00000010, B00111100, B00000000, B00000000, B00111100, B00000010, B00000010, B00000010, B00000010, B00111100, B00000000},
  eyes_bmp[] = {  B00000000, B00111100, B01000010, B01001010, B01000010, B01000010, B00111100, B00000000, B00000000, B00111100, B01000010, B01001010, B01000010, B01000010, B00111100, B00000000},
  sad_bmp[] =  {  B00000000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00000000, B00000000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00000000},
  xx_bmp[] =  {  B00000000, B00100010, B00010100, B00001000, B00010100, B00100010, B00000000, B00000000, B00000000, B00000000, B00100010, B00010100, B00001000, B00010100, B00100010, B00000000},
  XX_bmp[] = {  B01000001, B00100010, B00010100, B00001000, B00010100, B00100010, B01000001, B00000000, B00000000, B01000001, B00100010, B00010100, B00001000, B00010100, B00100010, B01000001},
  angry_bmp[] = {  B00000000, B00011110, B00111100, B01111000, B01110000, B00100000, B00000000, B00000000, B00000000, B00000000, B00100000, B01110000, B01111000, B00111100, B00011110, B00000000},
  angry2_bmp[] = {  B00000000, B00000010, B00000100, B00001000, B00010000, B00100000, B00000000, B00000000, B00000000, B00000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000000},
  sleep_bmp[] = {  B00000000, B00100010, B00110010, B00101010, B00100110, B00100010, B00000000, B00000000, B00000000, B00000000, B00100010, B00110010, B00101010, B00100110, B00100010, B00000000},
  freetful_bmp[] = {  B00000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000000, B00000000, B00000000, B00000000, B00000010, B00000100, B00001000, B00010000, B00100000, B00000000},
  love_bmp[] = {  B00000000, B00001100, B00011110, B00111100, B00111100, B00011110, B00001100, B00000000, B00000000, B00001100, B00011110, B00111100, B00111100, B00011110, B00001100, B00000000},
  confused_bmp[] = {  B00000000, B01111100, B10000010, B10111010, B10101010, B10001010, B01111000, B00000000, B00000000, B11111111, B10000001, B10111101, B10100101, B10000101, B00000101, B11111101},
  wave_bmp[] = {  B00000000, B00100000, B00010000, B00001000, B00010000, B00100000, B00010000, B00000000, B00000000, B00100000, B00010000, B00001000, B00010000, B00100000, B00010000, B00000000},
  magic_bmp[] = {  B00000000, B00000000, B01111110, B11111111, B01111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01111110, B11111111, B01111110, B00000000, B00000000},
  fail_bmp[] = {  B00000000, B00110000, B01111000, B01111000, B01111100, B00111100, B00001000, B00000000, B00000000, B00001000, B00111100, B01111100, B01111000, B01111000, B00110000, B00000000},
  full_bmp[] =  {   B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111 };
  //matrix.setRotation(3);


Serial.println("Starting...");
long startMS = millis();
matrix.setRotation(3);
  matrix.clear();
  matrix.drawBitmap(0, 0, logo_bmp, 8,16, LED_ON);
  matrix.drawPixel(0,0,LED_ON);
  matrix.display();
  delay(2000);
   matrix.clear();
  matrix.drawBitmap(0, 0, freetful_bmp, 8,16, LED_ON);
  matrix.display();
  delay(2000);
/*
for (int i = 0; i<110; i++) {
  matrix.displayBitmap(110-i, 0, logo18_bmp, 18,8, OLEDZoomNoZoom, false, false);
  matrix.displayBitmap((i*6)/7, 1, logo18_bmp, 18,8, OLEDZoomX2, false, false);
  matrix.displayBitmap(0, 0, (i / 8) % 2 ? angry_bmp : angry2_bmp, 16,8, OLEDZoomX8, true, true);
}
for (int j = 110; j>0; j--) {
  matrix.displayBitmap(110-j, 0, logo18_bmp, 18,8, OLEDZoomNoZoom, false, false);
  matrix.displayBitmap((j*6/7), 1, logo18_bmp, 18,8, OLEDZoomX2, false, false);
  matrix.displayBitmap( 0, 3,  (j / 8) % 2 ? sleep_bmp : wave_bmp, 16,8, OLEDZoomX8, (j / 8) % 2, false);
}
//delay(5000);
*/



/*

for (int i=0;i<1;i++) {
  matrix.drawBitmap(0, 8, full_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(10);
  matrix.clear();
  matrix.drawBitmap(0, 0, full_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(10);
  matrix.clear();
  matrix.drawBitmap(0, 0, happy_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, eyes_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, sad_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, angry_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, sleep_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, freetful_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, love_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, confused_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, wave_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, magic_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, fail_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, xx_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);
  matrix.clear();
  matrix.drawBitmap(0, 0, XX_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(20);

  matrix.clear();
  matrix.drawBitmap(0, 0, logo_bmp, 8, 16, LED_ON);
  matrix.writeDisplay();
  delayBetweenDisplays(10);
}
Serial.print(millis() - startMS);
Serial.println("ms");

  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delayBetweenDisplays(5);

  matrix.clear();
  matrix.drawLine(0, 0, 7, 15, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delayBetweenDisplays(5);

  matrix.clear();
  matrix.drawRect(0, 0, 8, 16, LED_ON);
  matrix.fillRect(2, 2, 4, 12, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delayBetweenDisplays(5);

  matrix.clear();
  matrix.drawCircle(3, 8, 3, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delayBetweenDisplays(5);

/*
  matrix.setRotation(0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  for (int8_t x = 0; x >= -64; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("_nN_#--!=+-");
    matrix.writeDisplay();
    delayBetweenDisplays(10);
  }

  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  for (int8_t x = 7; x >= -36; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("Otto DIY");
    matrix.writeDisplay();
    delayBetweenDisplays(10);
  }
  */
}
