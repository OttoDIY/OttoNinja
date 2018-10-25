//----------------------------------------------------------------
//-- Otto All moves setup
//-- You can add functions, gestures or sounds in loop
//-- CC BY SA (http://ottodiy.com)
//-- 25 October 2018
//-----------------------------------------------------------------
#include <Servo.h> 
#include <Oscillator.h>
#include <US.h>
#include <Otto.h>
Otto Otto;  //This is Otto!
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
  #define PIN_YL 2 //servo[2]
  #define PIN_YR 3 //servo[3]
  #define PIN_RL 4 //servo[4]
  #define PIN_RR 5 //servo[5]
/*SOUNDS******************
 * S_connection  S_disconnection  S_buttonPushed S_mode1 S_mode2 S_mode3 S_surprise S_OhOoh  S_OhOoh2  S_cuddly 
 * S_sleeping  S_happy S_superHappy S_happy_short S_sad S_confused S_fart1 S_fart2  S_fart3 
 */
/*MOVEMENTS LIST**************
 * dir=1---> FORWARD/LEFT
 * dir=-1---> BACKWARD/RIGTH
 * T : amount of movement. HIGHER VALUE SLOWER MOVEMENT usually 1000 (from 600 to 1400)
 * h: height of mov. around 20
     jump(steps=1, int T = 2000);
     walk(steps, T, dir);
     turn(steps, T, dir);
     bend (steps, T, dir); //usually steps =1, T=2000
     shakeLeg (steps, T, dir);
     updown(steps, T, HEIGHT);
     swing(steps, T, HEIGHT);
     tiptoeSwing(steps, T, HEIGHT);
     jitter(steps, T, HEIGHT); (small T)
     ascendingTurn(steps, T, HEIGHT);
     moonwalker(steps, T, HEIGHT,dir);
     crusaito(steps, T, HEIGHT,dir);
     flapping(steps, T, HEIGHT,dir);
/*GESTURES LIST***************
OttoHappy OttoSuperHappy  OttoSad   OttoSleeping  OttoFart  OttoConfused OttoLove  OttoAngry   
OttoFretful OttoMagic  OttoWave  OttoVictory  OttoFail*/
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
bool obstacleDetected = false;
int distance = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  //Set the servo pins
  Otto.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
//--Uncomment lines or add you own-------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
          //OBSTACLE MODE ON!!!! 
          obstacleMode();
          Otto.sing(S_surprise);
          Otto.walk(2,1000,1); //2 steps FORWARD
          Otto.sing(S_OhOoh);
          Otto.walk(2,1000,-1); //2 steps BACKSWARD
          Otto.sing(S_OhOoh2);
          Otto.playGesture(OttoFretful);
          Otto.sing(S_cuddly);
          Otto.home();
          Otto.playGesture(OttoSleeping);
          Otto.sing(S_sleeping);
          delay(1000);
          Otto.sing(S_happy);
          Otto.playGesture(OttoHappy);
          Otto.turn(2,1000,1);//2 steps turning RIGHT  
          Otto.sing(S_happy_short);              
          delay(50);
          Otto.playGesture(OttoSuperHappy);
          Otto.sing(S_superHappy);
          Otto.turn(2,1000,-1);//2 steps turning RIGHT 
          Otto.playGesture(OttoSad);
          Otto.sing(S_sad);               
          delay(50);
          Otto.moonwalker(3, 1000, 25, 1);
          Otto.playGesture(OttoConfused);
          Otto.sing(S_confused);
          Otto.home();
          Otto.bend (1, 2000, 1);
          Otto.playGesture(OttoFart);
          Otto.sing(S_fart1);
          Otto.sing(S_fart2);
          Otto.sing(S_fart3);
          Otto.home();
          Otto.ascendingTurn(1, 2000, 22);
          Otto.playGesture(OttoVictory);
          Otto.home();
          Otto.updown(1, 2000, 22);
          Otto.playGesture(OttoMagic);
          Otto.home();
          Otto.playGesture(OttoFail);
          Otto.sing(S_disconnection);
}
///////////////////////////////////////////////////////////////////
//-- Function to avoid obstacles
void obstacleMode(){
      distance = Otto.getDistance();
      if(distance<15){
          Otto.sing(S_surprise); 
               Otto.walk(2,1300,-1); 
               Otto.turn(3,1000,-1);                
             delay(50); 
        }
        else{
          Otto.walk(2,1000,1); 
        }
}
