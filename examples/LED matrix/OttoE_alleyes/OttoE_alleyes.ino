/***************************************************
 *  Designed specifically to work with the Otto DIY 16x8 LED Matrix eyes
 *
 *  These displays use I2C to communicate
 *
 ***************************************************/

#include <Wire.h>

// Define the I2C address of the left and right eyes
#define LEFT_EYE_ADDRESS 0x3E
#define RIGHT_EYE_ADDRESS 0x3F

// Define the number of rows and columns in the display
#define NUM_ROWS 8
#define NUM_COLS 16

// Function to print a string on the left eye
void printLeft(String text) {
  Wire.beginTransmission(LEFT_EYE_ADDRESS);
  for (int i = 0; i < text.length(); i++) {
    Wire.write(text[i]);
  }
  Wire.endTransmission();
}

// Function to print a string on the right eye
void printRight(String text) {
  Wire.beginTransmission(RIGHT_EYE_ADDRESS);
  for (int i = 0; i < text.length(); i++) {
    Wire.write(text[i]);
  }
  Wire.endTransmission();
}

void setup() {
  // Start the I2C communication
  Wire.begin();

  // Clear both eyes
  printLeft("          ");
  printRight("          ");
}

void loop() {
  // Blink both eyes
  printLeft("oooooooooo");
  printRight("oooooooooo");
  delay(500);
  printLeft("        ");
  printRight("        ");
  delay(500);

  // Display a happy face on both eyes
  printLeft("oooooo..oo");
  printRight("oooooo..oo");
  delay(500);
  printLeft("o....o..o");
  printRight("o....o..o");
  delay(500);
  printLeft("oooooo..oo");
  printRight("oooooo..oo");
  delay(500);

  // Display a sad face on both eyes
  printLeft("oooooo.ooo");
  printRight("oooooo.ooo");
  delay(500);
  printLeft("o.....ooo");
  printRight("o.....ooo");
  delay(500);
  printLeft("oooooo.ooo");
  printRight("oooooo.ooo");
  delay(500);
}
