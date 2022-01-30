//----------------------------------------------------------------
//-- https://github.com/OttoDIY
//-- CC BY SA (http://ottodiy.com)
//-----------------------------------------------------------------
#include <Otto.h>
Otto Otto;  //This is Otto!
//---------------------------------------------------------
#define PIN_YL D3 //Left Leg
#define PIN_YR D4 //Right Foot
#define PIN_RL D7 //Left Leg
#define PIN_RR D8 //Right Foot
#define PIN_Buzzer D0
#define PIN_Sensor A0
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  //Set the servo pins
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Sensor, PIN_Buzzer);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
          Otto.walk(2,1000,1); //2 steps FORWARD
          Otto.playGesture(OttoFretful);
          Otto.home();
          Otto.sing(S_sleeping);
          delay(50);
          Otto.moonwalker(3, 1000, 25, 1);
  Otto.sing(S_cuddly);
  Otto.sing(S_OhOoh);
  Otto.sing(S_surprise);
  Otto.sing(S_buttonPushed);
  Otto.sing(S_mode1);
  Otto.sing(S_mode2);
  Otto.sing(S_mode3);
}
