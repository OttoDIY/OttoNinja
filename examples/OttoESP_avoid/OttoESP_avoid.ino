#include <Otto.h>
Otto Otto;  //This is Otto!

#define PIN_YL D3 //Left Leg
#define PIN_YR D4 //Right Foot
#define PIN_RL D7 //Left Leg
#define PIN_RR D8 //Right Foot
#define PIN_Buzzer D0
#define PIN_Sensor A0
#define PIN_Trig D5
#define PIN_Echo D6

long Ultrasonic() {
   long duration, distance;
   digitalWrite(PIN_Trig,LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_Trig, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_Trig, LOW);
   duration = pulseIn(PIN_Echo, HIGH);
   distance = duration/58;
   return distance;
}

void setup() {
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Sensor, PIN_Buzzer);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
  Otto.sing(S_happy); // a happy Otto :)
  Serial.begin(9600);
  pinMode(PIN_Trig, OUTPUT);
  pinMode(PIN_Echo, INPUT);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
    Serial.print("Ultrasonic sensor reading:");
    Serial.print(Ultrasonic());
    Serial.print("cm");
    Serial.println("");
  if (Ultrasonic() < 15) {
    Otto.playGesture(OttoConfused);
  }
   else {
    Otto.walk(1,1000,1); // FORWARD
  }
}
