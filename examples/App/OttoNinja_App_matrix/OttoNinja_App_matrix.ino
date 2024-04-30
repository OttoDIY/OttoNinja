// PINOUT FOR ESP8266

// A0 = GPIO A0        TX = GPIO 1    
// D0 = GPIO 16        RX = GPIO 3 
// D5 = GPIO 14        D1 = GPIO 5
// D6 = GPIO 12        D2 = GPIO 4
// D7 = GPIO 13        D3 = GPIO 0
// D8 = GPIO 15        D4 = GPIO 2
//                     S3 = GPIO 9
//                     SK = GPIO 10

#ifndef REMOTEXY_H
#define REMOTEXY_H

#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "OTTO NINJA"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

// Pin definitions
const uint8_t ServoLeftFootPin = 13; // D7
const uint8_t ServoLeftAnklePin = 15; // D8
const uint8_t ServoRightFootPin = 0; // D3
const uint8_t ServoRightAnklePin = 2; // D4
const uint8_t ServoLeftArmPin = 16; // D0
const uint8_t ServoRightArmPin = 3; // RX
const uint8_t ServoHeadPin = 1; // TX

// Servo instances
Servo myservoLeftFoot;
Servo myservoLeftAnkle;
Servo myservoRightFoot;
Servo myservoRightAnkle;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;

// Matrix instance
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

// RemoteXY configurate
struct {
  // Input variables
  int8_t J_x;
  int8_t J_y;
  uint8_t button_B;
  uint8_t button_X;
  uint8_t button_Y;
  uint8_t button_A;

  // Other variable
  uint8_t connect_flag;
} RemoteXY;

#endif // REMOTEXY_H

void setup() {
  // Initialize servos
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
  myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);

  // Initialize matrix
  matrix.begin(0x70);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);

  // Initialize serial
  Serial.begin(250000);

  // Initialize WiFi
  WiFi.begin(REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Initialize RemoteXY
  RemoteXY_Init();

  // Initialize servo positions
  myservoHead.write(90);
  myservoLeftArm.write(90);
  myservoRightArm.write(90);
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);
  myservoLeftAnkle.write(60);
  myservoRightAnkle.write(120);

  // Calibration settings

  // ...

  // Display "NINJA" on matrix
  for (int8_t x = 7; x > -50; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("NINJA");
    matrix.writeDisplay();
    delay(50);
  }
}

void loop() {
  RemoteXY_Handler();

  // Handle button events
  if (RemoteXY.button_X == HIGH) {
    RobotSetRoll();
  }
  if (RemoteXY.button_Y == HIGH) {
    RobotSetWalk();
  }
  if (RemoteXY.button_A == HIGH) {
    RobotLeftArm();
  }
  if (RemoteXY.button_B == HIGH) {
    RobotRightArm();
  }

  // Handle joystick events
  if (RemoteXY.J_y > 0) {
    // Forward walking
  } else if (RemoteXY.J_y < 0) {
    // Backward walking
  }

  // Handle servo positions
  // ...
}

// Robot functions
void RobotSetWalk() {
  // ...
}

void RobotSetRoll() {
  // ...
}

void RobotLeftArm() {
  // ...
}

void RobotRightArm() {
  // ...
}

void RobotLeftArmDown() {
  // ...
}

void RobotRightArmDown() {
  // ...
}

void RobotStop() {
  // ...
}
