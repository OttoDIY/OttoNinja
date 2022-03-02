/*
This sketch uses a combination of ultrasonic sensor and matrix
to enable Ninja humanoid to say hello Iam Ninja when you wave 
your hands in front of him.
*/

// PINOUT FOR ESP8266

// A0 = GPIO A0        TX = GPIO 1    
// D0 = GPIO 16        RX = GPIO 3 
// D5 = GPIO 14        D1 = GPIO 5
// D6 = GPIO 12        D2 = GPIO 4
// D7 = GPIO 13        D3 = GPIO 0
// D8 = GPIO 15        D4 = GPIO 2
//                     S3 = GPIO 9
//                     S2 = GPIO 10

#include <RemoteXY.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

//CALIBRATION SETTINGS:

int LA0= 60 +0;     // Left Leg standing Position             - = Tilt Right   + = Tilt Left  
int RA0= 120 +0;    // Right Leg standing position            - = Tilt Right   + = Tilt Left  
int LA1= 180;       // Left Leg roll Position                 - = Tilt Right   + = Tilt Left   
int RA1= 0;         // Right Leg roll position                - = Tilt Right   + = Tilt Left   
int LATL= LA0 +40;  // Left Leg tilt left walking position    - = Tilt Right   + = Tilt Left 
int RATL= RA0 +60;  // Right Leg tilt left walking position   - = Tilt Right   + = Tilt Left 
int LATR= LA0 -60;  // Left Leg tilt right walking position   - = Tilt Right   + = Tilt Left 
int RATR= RA0 -40;  // Right Leg tilt right walking position  - = Tilt Right   + = Tilt Left 

int LFFWRS=15;  // Left foot forward walking rotation Speed   0 = SLOW   90 = FAST  
int RFFWRS=15 ; // Right foot forward walking rotation Speed  0 = SLOW   90 = FAST  
int LFBWRS= 15; // Left foot Backward walking rotation Speed  0 = SLOW   90 = FAST  
int RFBWRS= 15; // Right foot Backward walking rotation Speed 0 = SLOW   90 = FAST  


////////////////////////////////////////

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

#define echoPin 14 // attach pin D5 ESP8266 to pin Echo of HC-SR04
#define trigPin 12 //attach pin D6 ESP8266 to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() 
{
  
  NinjaHome();

  Serial.begin(250000);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  matrix.begin(0x70);  // pass in the address

  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);
  for (int8_t x=7; x>=-50; x--) 
  {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("NINJA");
    matrix.writeDisplay();
    delay(50);
  }
  
}







void loop() 
{
  
  Distance();

  if ((distance >= 2)&&(distance < 10))
  {  
    RobotHeadShake();
    RobotLeftArmWave();
    IAmNinja();
  }
 
}


void NinjaHome()
{ 
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400); 
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);  
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);  
  myservoHead.attach(ServoHeadPin, 544, 2400); 
  myservoLeftArm.write(180); 
  myservoRightArm.write(0);
  myservoHead.write(90); 
  delay(400);    
  myservoLeftLeg.write(60); 
  myservoRightLeg.write(120);
  delay(400);
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
  myservoHead.detach();
}


void RobotHeadShake()
{  
  myservoHead.attach(ServoHeadPin, 544, 2400);
  myservoHead.write(135);  
  delay(200);
  myservoHead.write(45);  
  delay(400);
  myservoHead.write(90);  
  delay(200);
   
}


void RobotLeftArmWave()
{  
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoRightArm.write(180);
  delay(400);  
  myservoRightArm.write(135);
  delay(200);  
  myservoRightArm.write(180);
  delay(200);  
  myservoRightArm.write(135);
  delay(200);
  myservoRightArm.write(180);
  delay(200);  
  myservoRightArm.write(0);
  delay(400);     
}



void IAmNinja()
{ 
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);
  for (int8_t x=7; x>=-80; x--) 
  {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("I AM NINJA");
    matrix.writeDisplay();
    delay(50);
  }
}  

void Distance() 
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
