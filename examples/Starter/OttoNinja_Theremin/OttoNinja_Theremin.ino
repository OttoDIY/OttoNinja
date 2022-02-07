/*
This sketch uses a combination of button buzzer and ultrasonic sensor 
to replicate a theremin upload the sketch and press the button to make 
sound with the buzzer adjust the tone of the sound by placing your other 
hand at different distances from the ultrasonic sensor.
in this example the push buttons +- connections need to be inverted for the 
code to work properly
*/



// PINOUT FOR ESP8266

// A0 = GPIO A0        TX = GPIO 1    
// D0 = GPIO 16        RX = GPIO 3 
// D5 = GPIO 14        D1 = GPIO 5
// D6 = GPIO 12        D2 = GPIO 4
// D7 = GPIO 13        D3 = GPIO 0
// D8 = GPIO 15        D4 = GPIO 2
//                     S3 = GPIO 9
//                     SK = GPIO 10







#include <Servo.h>
#include <SPI.h>
#include <Wire.h>




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
const uint8_t ServoLeftLegPin    = 15;     //D8
const uint8_t ServoRightFootPin  = 0;      //D3
const uint8_t ServoRightLegPin   = 2;      //D4
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




#define echoPin 4 // attach pin D2 ESP8266 to pin Echo of HC-SR04
#define trigPin 5 //attach pin D1 ESP8266 to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement



#define ButtonPin 14 //attach pin D5 ESP8266 to pin S Button
int ButtonState = 0;   // variable for reading the pushbutton status



#define BuzzerPin 12  //attach pin D6 ESP8266 to pin S Buzzer

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978





void setup() 
{
  
  NinjaHome();

  pinMode(BuzzerPin, OUTPUT);//buzzer
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(BuzzerPin, OUTPUT);

  Serial.begin(250000);
  
}







void loop() 
{ 
  
  Distance();
  ButtonState = digitalRead(ButtonPin);

  if ((ButtonState == LOW)&&(distance >= 2)&&(distance < 7))
  {  
    tone(BuzzerPin, NOTE_C5); 
  }

  if ((ButtonState == LOW)&&(distance >= 7)&&(distance < 12))
  {  
    tone(BuzzerPin, NOTE_D5); 
  }

  if ((ButtonState == LOW)&&(distance >= 12)&&(distance < 17))
  {  
    tone(BuzzerPin, NOTE_E5); 
  }

  if ((ButtonState == LOW)&&(distance >= 17)&&(distance < 22))
  {  
    tone(BuzzerPin, NOTE_F5); 
  }

  if ((ButtonState == LOW)&&(distance >= 22)&&(distance < 27))
  {  
    tone(BuzzerPin, NOTE_G5);
  }

  if ((ButtonState == LOW)&&(distance >= 27)&&(distance < 32))
  {  
    tone(BuzzerPin, NOTE_A5); 
  }

  if ((ButtonState == LOW)&&(distance >= 32)&&(distance < 37))
  {  
    tone(BuzzerPin, NOTE_B5);
  }

  if ((ButtonState == LOW)&&(distance >= 37)&&(distance < 42))
  {  
    tone(BuzzerPin, NOTE_C6); 
  }

  if (ButtonState == HIGH)
  {
    noTone(BuzzerPin);     // Stop sound...
  }



}

////////////////////////////////////////////////////






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
  myservoLeftFoot.write(90); 
  myservoRightFoot.write(90);   
  myservoLeftLeg.write(60); 
  myservoRightLeg.write(120);
  delay(400);
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
  myservoLeftArm.detach();
  myservoRightArm.detach();
  myservoHead.detach();
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
