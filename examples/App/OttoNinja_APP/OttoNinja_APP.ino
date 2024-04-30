// PINOUT FOR ESP8266
const uint8_t ServoLeftFootPin   = 13;     //D7
const uint8_t ServoLeftLegPin  = 15;     //D8
const uint8_t ServoRightFootPin  = 0;      //D3
const uint8_t ServoRightLegPin = 2;      //D4
const uint8_t ServoLeftArmPin    = 16;     //D0
const uint8_t ServoRightArmPin   = 3;      //RX
const uint8_t ServoHeadPin       = 1;      //TX

// RemoteXY include library
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

// Calibration settings
int LFFWRS = 20;
int RFFWRS = 20;
int LFBWRS = 20;
int RFBWRS = 20;
int LA0 = 60;
int RA0 = 120;
int LATL = 100;
int RATL = 175;
int LATR = 5;
int RATR = 80;
int LA1 = 180;
int RA1 = 0;

// Mode counter for biped/wheel mode
int ModeCounter = 0;

// Servo objects
Servo myservoLeftFoot;
Servo myservoRightFoot;
Servo myservoLeftLeg;
Servo myservoRightLeg;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;

void setup() {
  Serial.begin(250000);

  // Initialize servos
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoHead.attach(ServoHeadPin, 544, 2400);

  // Calibration
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

  // Detach servos to save power
  detachServos();

  // Initialize RemoteXY
  RemoteXY_Init();
}

void loop() {
  RemoteXY_Handler();

  // Handle button presses
  if (RemoteXY.button_X == HIGH) {
    NinjaSetRoll();
    ModeCounter = 1;
  }
  if (RemoteXY.button_Y == HIGH) {
    NinjaSetWalk();
    ModeCounter = 0;
  }
  if (RemoteXY.button_A == HIGH) {
    NinjaLeftArm();
  }
  if (RemoteXY.button_A == LOW) {
    NinjaLeftArmDown();
  }
  if (RemoteXY.button_B == HIGH) {
    NinjaRightArm();
  }
  if (RemoteXY.button_B == LOW) {
    NinjaRightArmDown();
  }

  // Update servos based on joystick position
  if (ModeCounter == 0) {
    if ((RemoteXY.J_x >= -10) && (RemoteXY.J_x <= 10) && (RemoteXY.J_y >= -10) && (RemoteXY.J_y <= 10)) {
      NinjaWalkStop();
    } else {
      updateServos();
    }
  } else if (ModeCounter == 1) {
    if ((RemoteXY.J_x >= -10) && (RemoteXY.J_x <= 10) && (RemoteXY.J_y >= -10) && (RemoteXY.J_y <= 10)) {
      NinjaRollStop();
    } else {
      updateServos();
    }
  }

  // Print joystick values for debugging
  Serial.print("  X: ");
  Serial.print(RemoteXY.J_x);
  Serial.print("  Y: ");
  Serial.print(RemoteXY.J_y);
  Serial.print("  MC: ");
  Serial.println(ModeCounter);
}

void updateServos() {
  if (ModeCounter == 0) {
    // Walking mode
    if (RemoteXY.J_y > 0) {
      int lt = map(RemoteXY.J_x, 100, -100, 200, 700);
      int rt = map(RemoteXY.J_x, 100, -100, 700, 200);
      int lf = 90 + map(RemoteXY.J_y, 100, -100, -20, 20);
      int rf = 90 - map(RemoteXY.J_y, 100, -100, -20, 20);

      myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
      myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
      myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
      myservoRightLeg.attach(ServoRightLegPin, 544, 2400);

      myservoLeftLeg.write(LATR);
      myservoRightLeg.write(RATR);
      myservoLeftFoot.write(lf);
      myservoRightFoot.write(rf);

      detachServos();
    } else if (RemoteXY.J_y < 0) {
      int lt = map(RemoteXY.J_x, 100, -100, 200, 700);
      int rt = map(RemoteXY.J_x, 100, -100, 700, 200);
      int lf = 90 + map(RemoteXY.J_y, 100, -100, 20, -20);
      int rf = 90 - map(RemoteXY.J_y, 100, -100, 20, -20);

      myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
      myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
      myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
      myservoRightLeg.attach(ServoRightLegPin, 544, 2400);

      myservoLeftLeg.write(LATR);
      myservoRightLeg.write(RATR);
      myservoLeftFoot.write(lf);
      myservoRightFoot.write(rf);

      detachServos();
    }
  } else if (ModeCounter == 1) {
    // Rolling mode
    int ls = map(RemoteXY.J_y, 100, -100, 45, 135);
    int rs = map(RemoteXY.J_y, 100, -100, 135, 45);
    int ld = map(RemoteXY.J_x, 100, -100, 0, -45);
    int rd = map(RemoteXY.J_x, 100, -100, -45, 0);

    myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
    myservoRightFoot.attach(ServoRightFootPin, 544, 2400);

    myservoLeftFoot.write(ls + ld);
    myservoRightFoot.write(rs + rd);

    detachServos();
  }
}

void detachServos() {
  myservoLeftFoot.detach();
  myservoRightFoot.detach();
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
}

void NinjaStop() {
  detachServos();
}

void NinjaSetWalk() {
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

void NinjaSetRoll() {
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

void NinjaWalkStop() {
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);
  myservoLeftLeg.write(LA0);
  myservoRightLeg.write(RA0);
}

void NinjaRollStop() {
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);
  detachServos();
}

void NinjaLeftArm() {
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoLeftArm.write(90);
}

void NinjaRightArm() {
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoRightArm.write(90);
}

void NinjaLeftArmDown() {
  myservoLeftArm.write(180);
}

void NinjaRightArmDown() {
  myservoRightArm.write(0);
}
