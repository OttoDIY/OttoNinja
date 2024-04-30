// PINOUT FOR ESP8266

// A0 = GPIO A0        TX = GPIO 1    
// D0 = GPIO 16        RX = GPIO 3 
// D5 = GPIO 14        D1 = GPIO 5
// D6 = GPIO 12        D2 = GPIO 4
// D7 = GPIO 13        D3 = GPIO 0
// D8 = GPIO 15        D4 = GPIO 2
//                     S3 = GPIO 9
//                     S2 = GPIO 10

#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Constants for pin numbers
const int kTrigPin = 12;
const int kEchoPin = 14;
const int kServoLeftFootPin = 13;
const int kServoLeftLegPin = 15;
const int kServoRightFootPin = 0;
const int kServoRightLegPin = 2;
const int kServoLeftArmPin = 16;
const int kServoRightArmPin = 3;
const int kServoHeadPin = 1;

// Constants for servo positions
const int kLeftLegStandingPosition = 60 + 0;
const int kRightLegStandingPosition = 120 + 0;
const int kLeftLegRollPosition = 180;
const int kRightLegRollPosition = 0;
const int kLeftLegTiltLeftWalkingPosition = kLeftLegStandingPosition + 40;
const int kRightLegTiltLeftWalkingPosition = kRightLegStandingPosition + 60;
const int kLeftLegTiltRightWalkingPosition = kLeftLegStandingPosition - 60;
const int kRightLegTiltRightWalkingPosition = kRightLegStandingPosition - 40;

const int kLeftFootForwardWalkingRotationSpeed = 15;
const int kRightFootForwardWalkingRotationSpeed = 15;
const int kLeftFootBackwardWalkingRotationSpeed = 15;
const int kRightFootBackwardWalkingRotationSpeed = 15;

const int kLeftFootForwardRollingRotationSpeed = 20;
const int kRightFootForwardRollingRotationSpeed = 20;
const int kLeftFootBackwardRollingRotationSpeed = 20;
const int kRightFootBackwardRollingRotationSpeed = 20;

// Constants for distances
const int kMinimumDistance = 2;
const int kMaximumDistance = 15;

// Constants for intervals
const int kInterval1 = 300;
const int kInterval2 = 600;
const int kInterval3 = 900;
const int kInterval4 = 1200;

// Global variables
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
Servo myservoLeftFoot;
Servo myservoLeftLeg;
Servo myservoRightFoot;
Servo myservoRightLeg;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;
int currentmillis1 = 0;
long duration;
int distance;

void setup() {
  Serial.begin(250000);
  pinMode(kTrigPin, OUTPUT);
  pinMode(kEchoPin, INPUT);
  matrix.begin(0x70);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);
  for (int8_t x = 7; x > -50; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("NINJA");
    matrix.writeDisplay();
    delayMicroseconds(50);
  }
  NinjaHome();
}

void loop() {
  Distance();
  if (distance >= kMaximumDistance) {
    NinjaRollForward();
    HeadScan();
  } else if (distance >= kMinimumDistance && distance < kMaximumDistance) {
    NinjaRollStop();
    myservoHead.attach(kServoHeadPin, 544, 2400);
    myservoHead.write(90);
    delay(300);
    NinjaArmWave();
    NinjaRollLeft();
    delay(500);
    NinjaRollStop();
  }
}

void NinjaHome() {
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  if (!myservoLeftArm.attach(kServoLeftArmPin, 544, 2400)) {
    Serial.println("Failed to attach left arm servo");
    return;
  }
  if (!myservoRightArm.attach(kServoRightArmPin, 544, 2400)) {
    Serial.println("Failed to attach right arm servo");
    return;
  }
  if (!myservoHead.attach(kServoHeadPin, 544, 2400)) {
    Serial.println("Failed to attach head servo");
    return;
  }
  myservoLeftArm.write(180);
  myservoRightArm.write(0);
  myservoHead.write(90);
  delayMicroseconds(400);
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);
  myservoLeftLeg.write(kLeftLegStandingPosition);
  myservoRightLeg.write(kRightLegStandingPosition);
  delayMicroseconds(400);
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
  myservoHead.detach();
}

void NinjaSetWalk() {
  if (!myservoLeftArm.attach(kServoLeftArmPin, 544, 2400)) {
    Serial.println("Failed to attach left arm servo");
    return;
  }
  if (!myservoRightArm.attach(kServoRightArmPin, 544, 2400)) {
    Serial.println("Failed to attach right arm servo");
    return;
  }
  myservoLeftArm.write(90);
  myservoRightArm.write(90);
  delayMicroseconds(200);
  myservoLeftArm.detach();
  myservoRightArm.detach();
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegStandingPosition);
  myservoRightLeg.write(kRightLegStandingPosition);
  delayMicroseconds(300);
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  if (!myservoLeftArm.attach(kServoLeftArmPin, 544, 2400)) {
    Serial.println("Failed to attach left arm servo");
    return;
  }
  if (!myservoRightArm.attach(kServoRightArmPin, 544, 2400)) {
    Serial.println("Failed to attach right arm servo");
    return;
  }
  myservoLeftArm.write(180);
  myservoRightArm.write(0);
  delayMicroseconds(200);
  myservoLeftArm.detach();
  myservoRightArm.detach();
}

void NinjaWalkForward() {
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegTiltRightWalkingPosition);
  myservoRightLeg.write(kRightLegTiltRightWalkingPosition);
  delayMicroseconds(300);
  myservoRightFoot.write(90 - kRightFootForwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoRightFoot.detach();
  delayMicroseconds(100);
  myservoLeftLeg.write(kLeftLegTiltLeftWalkingPosition);
  myservoRightLeg.write(kRightLegTiltLeftWalkingPosition);
  delayMicroseconds(300);
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  myservoLeftFoot.write(90 + kLeftFootForwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoLeftFoot.detach();
  delayMicroseconds(100);
}

void NinjaWalkBackward() {
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegTiltRightWalkingPosition);
  myservoRightLeg.write(kRightLegTiltRightWalkingPosition);
  delayMicroseconds(300);
  myservoRightFoot.write(90 + kRightFootBackwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoRightFoot.detach();
  delayMicroseconds(100);
  myservoLeftLeg.write(kLeftLegTiltLeftWalkingPosition);
  myservoRightLeg.write(kRightLegTiltLeftWalkingPosition);
  delayMicroseconds(300);
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  myservoLeftFoot.write(90 - kLeftFootBackwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoLeftFoot.detach();
  delayMicroseconds(100);
}

void NinjaWalkLeft() {
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegTiltRightWalkingPosition);
  myservoRightLeg.write(kRightLegTiltRightWalkingPosition);
  delayMicroseconds(300);
  myservoRightFoot.write(90 - kRightFootForwardWalkingRotationSpeed);
  delayMicroseconds(50);
  myservoRightFoot.detach();
  delayMicroseconds(100);
  myservoLeftLeg.write(kLeftLegTiltLeftWalkingPosition);
  myservoRightLeg.write(kRightLegTiltLeftWalkingPosition);
  delayMicroseconds(300);
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  myservoLeftFoot.write(90 + kLeftFootForwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoLeftFoot.detach();
  delayMicroseconds(100);
}

void NinjaWalkRight() {
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegTiltRightWalkingPosition);
  myservoRightLeg.write(kRightLegTiltRightWalkingPosition);
  delayMicroseconds(300);
  myservoRightFoot.write(90 - kRightFootForwardWalkingRotationSpeed);
  delayMicroseconds(300);
  myservoRightFoot.detach();
  delayMicroseconds(100);
  myservoLeftLeg.write(kLeftLegTiltLeftWalkingPosition);
  myservoRightLeg.write(kRightLegTiltLeftWalkingPosition);
  delayMicroseconds(300);
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  myservoLeftFoot.write(90 + kLeftFootForwardWalkingRotationSpeed);
  delayMicroseconds(50);
  myservoLeftFoot.detach();
  delayMicroseconds(100);
}

void NinjaWalkStop() {
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);
  myservoLeftLeg.write(kLeftLegStandingPosition);
  myservoRightLeg.write(kRightLegStandingPosition);
  myservoLeftFoot.detach();
  myservoRightFoot.detach();
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
}

void NinjaSetRoll() {
  if (!myservoLeftArm.attach(kServoLeftArmPin, 544, 2400)) {
    Serial.println("Failed to attach left arm servo");
    return;
  }
  if (!myservoRightArm.attach(kServoRightArmPin, 544, 2400)) {
    Serial.println("Failed to attach right arm servo");
    return;
  }
  myservoLeftArm.write(90);
  myservoRightArm.write(90);
  delayMicroseconds(200);
  myservoLeftArm.detach();
  myservoRightArm.detach();
  if (!myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400)) {
    Serial.println("Failed to attach left leg servo");
    return;
  }
  if (!myservoRightLeg.attach(kServoRightLegPin, 544, 2400)) {
    Serial.println("Failed to attach right leg servo");
    return;
  }
  myservoLeftLeg.write(kLeftLegRollPosition);
  myservoRightLeg.write(kRightLegRollPosition);
  delayMicroseconds(300);
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  if (!myservoLeftArm.attach(kServoLeftArmPin, 544, 2400)) {
    Serial.println("Failed to attach left arm servo");
    return;
  }
  if (!myservoRightArm.attach(kServoRightArmPin, 544, 2400)) {
    Serial.println("Failed to attach right arm servo");
    return;
  }
  myservoLeftArm.write(180);
  myservoRightArm.write(0);
  delayMicroseconds(300);
  myservoLeftArm.detach();
  myservoRightArm.detach();
}

void NinjaRollForward() {
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftFoot.write(90 + kLeftFootForwardRollingRotationSpeed);
  myservoRightFoot.write(90 - kRightFootForwardRollingRotationSpeed);
}

void NinjaRollBackward() {
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
    return;
  }
  myservoLeftFoot.write(90 - kLeftFootBackwardRollingRotationSpeed);
  myservoRightFoot.write(90 + kRightFootBackwardRollingRotationSpeed);
}

void NinjaRollLeft() {
  if (!myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400)) {
    Serial.println("Failed to attach left foot servo");
    return;
  }
  if (!myservoRightFoot.attach(kServoRightFootPin, 544, 2400)) {
    Serial.println("Failed to attach right foot servo");
   
