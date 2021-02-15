# About OTTO ESP32:
The Esp32 version of OTTO library  ( OTTOESP ) required some changes from the original library in the references of Oscillator.h and Servo.h library to make compatible with ESP32Servo library.
You can use those libraries alongside the standard otto library.

## Installation:
1) Copy "libraries" folder in your Arduino/libraries folder
2) Edit Otto_esp32.ino and choose your leg pins (default are IO 14,27,16,25)
3) Press the "Flash" ( or "Boot") button on your ESP32 board before uploading the code.

## Schematics:
Same as OTTO DIY :) ! 

## Todo :
1. Standardize library
2. Add bluetooth support 
3. Add wifi controlling capability 
