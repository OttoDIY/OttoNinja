// PINOUT FOR ESP8266

// A0 = GPIO A0        TX = GPIO 1    
// D0 = GPIO 16        RX = GPIO 3 
// D5 = GPIO 14        D1 = GPIO 5
// D6 = GPIO 12        D2 = GPIO 4
// D7 = GPIO 13        D3 = GPIO 0
// D8 = GPIO 15        D4 = GPIO 2
//                     S3 = GPIO 9
//                     SK = GPIO 10

/*
   -- Remote Control Otto Ninja --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "OTTO NINJA"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,6,0,0,0,66,0,13,8,0,
  5,32,3,12,41,41,1,26,31,1,
  3,79,16,16,12,1,31,82,240,159,
  166,190,0,1,3,56,39,18,12,1,
  31,240,159,146,191,0,1,3,79,39,
  17,12,1,31,240,159,166,191,0,1,
  3,56,16,17,12,1,31,76,240,159,
  166,190,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t J_x; // =-100..100 x-coordinate joystick position 
  int8_t J_y; // =-100..100 y-coordinate joystick position 
  uint8_t button_B; // =1 if button pressed, else =0 
  uint8_t button_X; // =1 if button pressed, else =0 
  uint8_t button_Y; // =1 if button pressed, else =0 
  uint8_t button_A; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

//CALIBRATION SETTINGS:

////////// 2:

// Left foot forward walking rotation Speed
int LFFWRS= 20;    // 0 = Slowest   90 = Fastest  Default = 12

// Right foot forward walking rotation Speed
int RFFWRS= 20;   // 0 = Slowest   90 = Fastest  Default = 12

////////// 3:

// Left foot Backward walking rotation Speed
int LFBWRS= 20;   // 0 = Slowest   90 = Fastest  Default = 12

// Right foot Backward walking rotation Speed
int RFBWRS= 20;   // 0 = Slowest   90 = Fastest  Default = 12

////////// 4:

// Left Leg standing Position
int LA0= 60;    // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 60

// Right Leg standing position
int RA0= 120;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 120

////////// 5:

// Left Leg tilt left walking position
int LATL= 100;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 85   Default HUMANOID = 80

// Right Leg tilt left walking position
int RATL= 175;  // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 175   Default HUMANOID = 150

// Left Leg tilt right walking position
int LATR= 5;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 5   Default HUMANOID = 30

// Right Leg tilt right walking position
int RATR= 80;  // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 95  Default HUMANOID = 100

////////// 6:

// Left Leg roll Position
int LA1= 180;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 170

// Right Leg roll position
int RA1= 0;    // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 10


////////////////////////////////////////

int currentmillis1 = 0;
int currentmillis2 = 0;
int currentmillis3 = 0;
// Mode counter for biped/wheel mode
int ModeCounter = 0;

const uint8_t ServoLeftFootPin   = 13;     //D7
const uint8_t ServoLeftLegPin  = 15;     //D8
const uint8_t ServoRightFootPin  = 0;      //D3
const uint8_t ServoRightLegPin = 2;      //D4
const uint8_t ServoLeftArmPin    = 16;     //D0
const uint8_t ServoRightArmPin   = 3;      //RX
const uint8_t ServoHeadPin       = 1;      //TX

Servo myservoLeftFoot;
Servo myservoLeftLeg;
Servo myservoRightFoot;
Servo myservoRightLeg;

Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;

void setup() 
{
  
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
  
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);

  myservoHead.attach(ServoHeadPin, 544, 2400);
  
  myservoHead.write(90);
  myservoLeftArm.write(90);
  myservoRightArm.write(90); 
  delay(300);
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);  
  myservoLeftLeg.write(60);
  myservoRightLeg.write(120);
  delay(300);
  myservoLeftArm.write(180);
  myservoRightArm.write(0); 

  delay(500);
  
  myservoLeftFoot.detach();
  myservoRightFoot.detach();  
  myservoLeftLeg.detach();
  myservoRightLeg.detach();

  myservoLeftArm.detach();
  myservoRightArm.detach();

  myservoHead.detach();

  Serial.begin(250000);
  
  RemoteXY_Init (); 
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  if (RemoteXY.button_X == HIGH)
  {        
      NinjaSetRoll(); 
      ModeCounter = 1;   
  }
  if (RemoteXY.button_Y == HIGH)
  {
      NinjaSetWalk();
      ModeCounter = 0;
  }
  if (RemoteXY.button_A == HIGH)
  { 
      NinjaLeftArm();
  }

  if (RemoteXY.button_A == LOW)
  { 
      NinjaLeftArmDown();
  }
   
  if (RemoteXY.button_B == HIGH)
  {            
      NinjaRightArm();
  }

  if (RemoteXY.button_B == LOW)
  {            
      NinjaRightArmDown();
  }

  if (ModeCounter == 0)
  {   
          if ((RemoteXY.J_x >= -10)&&(RemoteXY.J_x <= 10)&&(RemoteXY.J_y >= -10)&&(RemoteXY.J_y <= 10))
          {
             NinjaWalkStop(); 
          }
   
          if (RemoteXY.J_y > 0)
          {
             int lt= map(RemoteXY.J_x, 100, -100, 200, 700); 
             int rt= map(RemoteXY.J_x, 100, -100, 700, 200); 
             int Interval1 = 250;
             int Interval2 = 250 + rt;
             int Interval3 = 250 + rt + 250;
             int Interval4 = 250 + rt + 250 + lt;
             int Interval5 = 250 + rt + 250 + lt + 50;
             
             if(millis() > currentmillis1 + Interval5)
             {
              currentmillis1 = millis();
             }
             
             
             if(millis() - currentmillis1 <= Interval1)
             {   
                 myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
                 myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
                 myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
                 myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
                 
                 myservoLeftLeg.write(LATR); 
                 myservoRightLeg.write(RATR);
             }
                 
             if((millis() - currentmillis1 >= Interval1)&&(millis() - currentmillis1 <= Interval2))
             {      
                 myservoRightFoot.write(90-RFFWRS);
                 
             }

             if((millis() - currentmillis1 >= Interval2)&&(millis() - currentmillis1 <= Interval3))
             {  
                 myservoRightFoot.detach();
                 myservoLeftLeg.write(LATL); 
                 myservoRightLeg.write(RATL);
             }

             if((millis() - currentmillis1 >= Interval3)&&(millis() - currentmillis1 <= Interval4))
             {       
                 myservoLeftFoot.write(90+LFFWRS);      
             }  

             if((millis() - currentmillis1 >= Interval4)&&(millis() - currentmillis1 <= Interval5))
             {
              myservoLeftFoot.detach();  
             }
          }      

          if (RemoteXY.J_y < 0)
          {

             int lt= map(RemoteXY.J_x, 100, -100, 200, 700); 
             int rt= map(RemoteXY.J_x, 100, -100, 700, 200); 
             int Interval1 = 250;
             int Interval2 = 250 + rt;
             int Interval3 = 250 + rt + 250;
             int Interval4 = 250 + rt + 250 + lt;
             int Interval5 = 250 + rt + 250 + lt + 50;
             
             if(millis() > currentmillis1 + Interval5)
             {
              currentmillis1 = millis();
             }
             
             
             if(millis() - currentmillis1 <= Interval1)
             {   
                 myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
                 myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
                 myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
                 myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
                 
                 myservoLeftLeg.write(LATR); 
                 myservoRightLeg.write(RATR);
             }
                 
             if((millis() - currentmillis1 >= Interval1)&&(millis() - currentmillis1 <= Interval2))
             {      
                 myservoRightFoot.write(90+RFBWRS);
                 
             }

             if((millis() - currentmillis1 >= Interval2)&&(millis() - currentmillis1 <= Interval3))
             {  
                 myservoRightFoot.detach();
                 myservoLeftLeg.write(LATL); 
                 myservoRightLeg.write(RATL);
             }

             if((millis() - currentmillis1 >= Interval3)&&(millis() - currentmillis1 <= Interval4))
             {       
                 myservoLeftFoot.write(90-LFBWRS);      
             }  

             if((millis() - currentmillis1 >= Interval4)&&(millis() - currentmillis1 <= Interval5))
             {
              myservoLeftFoot.detach();  
             }
          }      
      }

  if (ModeCounter == 1)
  {  
      if ((RemoteXY.J_x >= -10)&&(RemoteXY.J_x <= 10)&&(RemoteXY.J_y >= -10)&&(RemoteXY.J_y <= 10))
      {
        NinjaRollStop();
      }  

      else
      {
        myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);  
        myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
        
        int LWS= map(RemoteXY.J_y, 100, -100, 135,    45); 
        int RWS= map(RemoteXY.J_y, 100, -100,  45,   135); 
        int LWD= map(RemoteXY.J_x, 100, -100,  45,     0); 
        int RWD= map(RemoteXY.J_x, 100, -100,   0,   -45);
        
        myservoLeftFoot.write(LWS+LWD);
        myservoRightFoot.write(RWS+RWD); 
      }
  }

 Serial.print("  X: ");  
 Serial.print(RemoteXY.J_x);
 Serial.print("  Y: ");  
 Serial.print(RemoteXY.J_y);
 Serial.print("  MC: ");  
 Serial.println(ModeCounter);
}

void NinjaStop()
{
  myservoLeftFoot.detach();
  myservoRightFoot.detach();  
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
}

void NinjaSetWalk()
{    
      myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      myservoLeftArm.write(90);
      myservoRightArm.write(90); 
      delay(200);
      myservoLeftArm.detach();
      myservoRightArm.detach();
      myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
      myservoRightLeg.attach(ServoRightLegPin, 544, 2400);                                                                          
      myservoLeftLeg.write(LA0); 
      myservoRightLeg.write(RA0); 
      delay(300);
      myservoLeftLeg.detach();
      myservoRightLeg.detach();
      myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      myservoLeftArm.write(180);
      myservoRightArm.write(0); 
      delay(300);
      myservoLeftArm.detach();
      myservoRightArm.detach();
}   

void NinjaSetRoll()
{  
      myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      myservoLeftArm.write(90);
      myservoRightArm.write(90); 
      delay(200);
      myservoLeftArm.detach();
      myservoRightArm.detach();
      myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
      myservoRightLeg.attach(ServoRightLegPin, 544, 2400);                                                                          
      myservoLeftLeg.write(LA1); 
      myservoRightLeg.write(RA1); 
      delay(300);
      myservoLeftLeg.detach();
      myservoRightLeg.detach();
      myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      myservoLeftArm.write(180);
      myservoRightArm.write(0); 
      delay(300);
      myservoLeftArm.detach();
      myservoRightArm.detach();      
}

void NinjaWalkStop()
{
    myservoLeftFoot.write(90);
    myservoRightFoot.write(90);  
    myservoLeftLeg.write(LA0); 
    myservoRightLeg.write(RA0);  
}

void NinjaRollStop()
{
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);  
  myservoLeftFoot.detach();
  myservoRightFoot.detach();  
}

void NinjaLeftArm()
{  
      myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      myservoLeftArm.write(90);    
}

void NinjaRightArm()
{  
      myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      myservoRightArm.write(90);      
}

void NinjaLeftArmDown()
{    
      myservoLeftArm.write(180);     
}

void NinjaRightArmDown()
{      
      myservoRightArm.write(0);    
}
