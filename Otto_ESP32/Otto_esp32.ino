#include <ESP32Servo.h>

//----------------------------------------------------------------
//-- CC BY SA (http://ottodiy.com)
//-----------------------------------------------------------------
//-- Otto will run fast straight forward with this code!
//-----------------------------------------------------------------

#include <OscillatorESP.h>
#include <US.h>
#include <OttoESP.h>
Otto Otto; 
//---------------------------------------------------------
//-- First step: Make sure the pins for servos are in the right position
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
  #define PIN_YL 14 //servo[2] !!Choose your ESP32 pin!! 
  #define PIN_YR 27 //servo[3] !!Choose your ESP32 pin!! 
  #define PIN_RL 16 //servo[4] !!Choose your ESP32 pin!!
  #define PIN_RR 25 //servo[5] !!Choose your ESP32 pin!!
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
int piezo=A0;

//---------------------------------------------------------
bool obstacleDetected = false;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
   Serial.begin(9600);
  //Set the servo pins
  Otto.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,piezo,piezo);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(500);
}

///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
    
    Otto.walk(10,500,0); //change T for lower value to run faster!    
    
}
