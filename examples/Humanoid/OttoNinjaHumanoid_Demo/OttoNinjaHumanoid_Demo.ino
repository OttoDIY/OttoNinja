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
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <SPI.h>
#include <Wire.h>

constexpr std::uint8_t kMatrixAddress = 0x70;
constexpr std::uint8_t kServoLeftFootPin = 13;
constexpr std::uint8_t kServoLeftLegPin = 15;
constexpr std::uint8_t kServoRightFootPin = 0;
constexpr std::uint8_t kServoRightLegPin = 2;
constexpr std::uint8_t kServoLeftArmPin = 16;
constexpr std::uint8_t kServoRightArmPin = 3;
constexpr std::uint8_t kServoHeadPin = 1;
constexpr std::uint8_t kEchoPin = 14;
constexpr std::uint8_t kTrigPin = 12;

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

//CALIBRATION SETTINGS:

constexpr int kLeftLegStandingPosition = 60 + 0; // Left Leg standing Position - = Tilt Right + = Tilt Left
constexpr int kRightLegStandingPosition = 120 + 0; // Right Leg standing position - = Tilt Right + = Tilt Left
constexpr int kLeftLegRollPosition = 180; // Left Leg roll Position - = Tilt Right + = Tilt Left
constexpr int kRightLegRollPosition = 0; // Right Leg roll position - = Tilt Right + = Tilt Left
constexpr int kLeftLegTiltLeftWalkingPosition = kLeftLegStandingPosition + 40; // Left Leg tilt left walking position - = Tilt Right + = Tilt Left
constexpr int kRightLegTiltLeftWalkingPosition = kRightLegStandingPosition + 60; // Right Leg tilt left walking position - = Tilt Right + = Tilt Left
constexpr int kLeftLegTiltRightWalkingPosition = kLeftLegStandingPosition - 60; // Left Leg tilt right walking position - = Tilt Right + = Tilt Left
constexpr int kRightLegTiltRightWalkingPosition = kRightLegStandingPosition - 40; // Right Leg tilt right walking position - = Tilt Right + = Tilt Left

constexpr int kLeftFootForwardWalkingRotationSpeed = 15; // Left foot forward walking rotation Speed 0 = SLOW 90 = FAST
constexpr int kRightFootForwardWalkingRotationSpeed = 15; // Right foot forward walking rotation Speed 0 = SLOW 90 = FAST
constexpr int kLeftFootBackwardWalkingRotationSpeed = 15; // Left foot Backward walking rotation Speed 0 = SLOW 90 = FAST
constexpr int kRightFootBackwardWalkingRotationSpeed = 15; // Right foot Backward walking rotation Speed 0 = SLOW 90 = FAST

void setup() {
  Serial.begin(250000);

  pinMode(kTrigPin, OUTPUT);
  pinMode(kEchoPin, INPUT);

  matrix.begin(kMatrixAddress);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);

  for (int8_t x = 7; x > -50; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("NINJA");
    matrix.writeDisplay();
    std::chrono::milliseconds(50);
  }

  initializeServos();
}

void loop() {
  int distance = measureDistance();

  if (distance >= 2 && distance < 10) {
    robotHeadShake();
    robotLeftArmWave();
    iAmNinja();
  }
}

void initializeServos() {
  myservoLeftFoot.attach(kServoLeftFootPin, 544, 2400);
  myservoLeftLeg.attach(kServoLeftLegPin, 544, 2400);
  myservoRightFoot.attach(kServoRightFootPin, 544, 2400);
  myservoRightLeg.attach(kServoRightLegPin, 544, 2400);
  myservoLeftArm.attach(kServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(kServoRightArmPin, 544, 2400);
  myservoHead.attach(kServoHeadPin, 544, 2400);

  setServoPositions(
      kLeftLegStandingPosition, kRightLegStandingPosition,
      kLeftLegRollPosition, kRightLegRollPosition,
      kLeftArmStandingPosition, kRightArmStandingPosition,
      kHeadStandingPosition);

  myservoLeftFoot.detach();
  myservoLeftLeg.detach();
  myservoRightFoot.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
  myservoHead.detach();
}

void setServoPositions(int leftLeg, int rightLeg, int leftArm, int rightArm, int head) {
  myservoLeftLeg.write(leftLeg);
  myservoRightLeg.write(rightLeg);
  myservoLeftArm.write(leftArm);
  myservoRightArm.write(rightArm);
  myservoHead.write(head);
}

void robotHeadShake() {
  myservoHead.attach(kServoHeadPin, 544, 2400);
  setServoPositions(kHeadStandingPosition, kHeadStandingPosition, kRightArmStandingPosition, kRightArmStandingPosition, kHeadShakingPosition);
  std::chrono::milliseconds(200);
  setServoPositions(kHeadStandingPosition, kHeadStandingPosition, kRightArmStandingPosition, kRightArmStandingPosition, kHeadRestingPosition);
  std::chrono::milliseconds(400);
  setServoPositions(kHeadStandingPosition, kHeadStandingPosition, kRightArmStandingPosition, kRightArmStandingPosition, kHeadRestingPosition);
  std::chrono::milliseconds(200);
}

void robotLeftArmWave() {
  myservoRightArm.attach(kServoRightArmPin, 544, 2400);
  setServoPositions(kLeftArmStandingPosition, kRightArmWavingPosition, kRightArmStandingPosition, kRightArmWavingPosition, kHeadStandingPosition);
  std::chrono::milliseconds(400);
  setServoPositions(kLeftArmStandingPosition, kRightArmRestingPosition, kRightArmStandingPosition, kRightArmRestingPosition, kHeadStandingPosition);
  std::chrono::milliseconds(200);
  setServoPositions(kLeftArmStandingPosition, kRightArmWavingPosition, kRightArmStandingPosition, kRightArmWavingPosition, kHeadStandingPosition);
  std::chrono::milliseconds(200);
  setServoPositions(kLeftArmStandingPosition, kRightArmRestingPosition, kRightArmStandingPosition, kRightArmRestingPosition, kHeadStandingPosition);
  std::chrono::milliseconds(200);
}

void iAmNinja() {
  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.print("I AM NINJA");
  matrix.writeDisplay();
  for (int8_t x = 7; x > -80; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("I AM NINJA");
    matrix.writeDisplay();
    std::chrono::milliseconds(50);
  }
}

int measureDistance() {
  // Clears the trigPin condition
  digitalWrite(kTrigPin, LOW);
  std::chrono::microseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(kTrigPin, HIGH);
  std::chrono::microseconds(10);
  digitalWrite(kTrigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  auto duration = pulseIn(kEchoPin, HIGH);
  // Calculating the distance
  auto distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
