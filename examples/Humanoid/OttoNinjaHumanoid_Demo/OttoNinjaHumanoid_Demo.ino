// Constants and definitions

// PINOUT FOR ESP8266
// ...

#include <RemoteXY.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <SPI.h>
#include <Wire.h>

// Constants
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

// Objects
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

// Calibration settings
constexpr int kLeftLegStandingPosition = 60 + 0;
// ...

// Function prototypes
void setup();
void loop();
void initializeServos();
void setServoPositions(int, int, int, int, int, int, int);
void robotHeadShake();
void robotLeftArmWave();
void iAmNinja();
int measureDistance();



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
  // ...
  setServoPositions(
      kLeftLegStandingPosition, kRightLegStandingPosition,
      kLeftLegRollPosition, kRightLegRollPosition,
      kLeftArmStandingPosition, kRightArmStandingPosition,
      kHeadStandingPosition);

  myservoLeftFoot.detach();
  // ...
}



void setServoPositions(int leftLeg, int rightLeg, int leftArm, int rightArm, int head) {
  myservoLeftLeg.write(leftLeg);
  // ...
  myservoHead.write(head);
}



void robotHeadShake() {
  // ...
}

void robotLeftArmWave() {
  // ...
}

void iAmNinja() {
  // ...
}



int measureDistance() {
  // ...
}

