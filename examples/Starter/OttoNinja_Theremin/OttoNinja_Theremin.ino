/*
This sketch uses a combination of button, buzzer, and ultrasonic sensor
to create a theremin-like device. Press the button to make sound with
the buzzer, and adjust the tone of the sound by placing your hand at
different distances from the ultrasonic sensor.
*/

#include <Servo.h>

// PINOUT FOR ESP8266
const uint8_t trigPin = 5;
const uint8_t echoPin = 4;
const uint8_t ButtonPin = 14;
const uint8_t BuzzerPin = 12;

const uint8_t ServoLeftFootPin = 13;
const uint8_t ServoLeftLegPin = 15;
const uint8_t ServoRightFootPin = 0;
const uint8_t ServoRightLegPin = 2;
const uint8_t ServoLeftArmPin = 16;
const uint8_t ServoRightArmPin = 3;
const uint8_t ServoHeadPin = 1;

// Constants for ultrasonic sensor
const int maxDistance = 42;
const int minDistance = 2;
const int buzzerFrequencyMultiplier = 2;

// Constants for servos
const int leftLegStandingPosition = 60;
const int rightLegStandingPosition = 120;
const int leftLegRollPosition = 180;
const int rightLegRollPosition = 0;
const int leftLegTiltLeftWalkingPosition = 100;
const int rightLegTiltLeftWalkingPosition = 160;
const int leftLegTiltRightWalkingPosition = 20;
const int rightLegTiltRightWalkingPosition = 80;

const int leftFootForwardWalkingRotationSpeed = 15;
const int rightFootForwardWalkingRotationSpeed = 15;
const int leftFootBackwardWalkingRotationSpeed = 15;
const int rightFootBackwardWalkingRotationSpeed = 15;

// Servo objects
Servo myservoLeftFoot;
Servo myservoLeftLeg;
Servo myservoRightFoot;
Servo myservoRightLeg;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;

void setup() {
  Serial.begin(250000);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(BuzzerPin, OUTPUT);

  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoHead.attach(ServoHeadPin, 544, 2400);

  // Move servos to initial positions
  myservoLeftLeg.write(leftLegStandingPosition);
  myservoRightLeg.write(rightLegStandingPosition);
  myservoLeftArm.write(leftLegRollPosition);
  myservoRightArm.write(rightLegRollPosition);
  myservoHead.write(90);
  myservoLeftFoot.write(90);
  myservoRightFoot.write(90);

  delay(400);

  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
  myservoHead.detach();
}

void loop() {
  int ButtonState = digitalRead(ButtonPin);
  int distance = getDistance();

  if (ButtonState == LOW && distance >= minDistance && distance < maxDistance) {
    int buzzerFrequency = map(distance, minDistance, maxDistance, NOTE_C5, NOTE_C6) * buzzerFrequencyMultiplier;
    tone(BuzzerPin, buzzerFrequency);
  } else {
    noTone(BuzzerPin);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}
