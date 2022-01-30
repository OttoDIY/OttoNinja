
//----------------------------------------------------------------
//-- Zowi basic firmware v2 adapted to Otto
//-- (c) BQ. Released under a GPL licencse
//-----------------------------------------------------------------
//  If you wish to use this software under Open Source Licensing, 
//   you must contribute all your source code to the open source
//   community in accordance with the GPL Version 2 when your application is
//   distributed. See http://www.gnu.org/copyleft/gpl.html
//   https://www.facebook.com/groups/ottodiy/
//------------------------------------------------------------------
// -- ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen OCT 2018
// -- Added PIN definitions for ease of use: Jason Snow NOV 2018
// -- ADDED NEOPIXEL: Paul Van De Veen NOV 2018
// -- ADDED Eye Matrix Progmem and control: Jason Snow NOV 2018
//-------------------------------------------------------------------
// Otto libraries - Otto biped version 9        July 2019
// some of these functions will need a good power source such as a LIPO battery
//-------------------------------------------------------------------

#include <EEPROM.h>
#include <EnableInterrupt.h>
// Library to manage the Neopixel RGB led
#include <Adafruit_NeoPixel.h>
//-- Library to manage serial commands
#include <OttoSerialCommand.h>
OttoSerialCommand SCmd;  // The SerialCommand object
//-- Otto Library version 9
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
// -- library to handle eye MATRIX if installed
#include <OttoEyeMatrix.h>

//---------------------------------------------------------
//-- First step: Configure the pins where the servos are attached
/*
         ---------------
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         ---------------
            ||     ||
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  10 //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
#define PIN_Battery   A7  //3v7 BATTERY MONITOR   ANALOG pin (A7)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
boolean enableRGB = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define NeopixelRGB_PIN  12 // NEOPIXEL pin   DIGITAL PIN (12)
#define NUMPIXELS       1   // There is only one Neopixel use in MY Otto, chnage for more than 1
Adafruit_NeoPixel NeopixelLed = Adafruit_NeoPixel(NUMPIXELS, NeopixelRGB_PIN, NEO_RGB + NEO_KHZ800);
// EYE MATRIX PINs   /////////////////////////////////////////////////////////////////////
boolean enableEYES = true;    // SET TO FALSE IF NOT USING THIS OPTION - option to be available from OTTO website
#define EYE_CLK    7   //CLK pin   (11)
#define EYE_DIN    6   //DIN pin   (10)
OTTOeyeMatrix ledMx(EYE_CLK,EYE_DIN);
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 7 and GND
#define PIN_ASSEMBLY    11   //ASSEMBLY pin (7) LOW = assembly    HIGH  = normal operation
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////

const char programID[] = "Otto_BIPED9"; //Each program will have a ID
const char name_fac = '$'; //Factory name
const char name_fir = '#'; //First name
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int modeId = 0;          //Number of mode
int moveSize = 15;       //Asociated with the height of some movements
volatile bool buttonPushed=false;  //Variable to remember when a button has been pushed
//---------------------------------------------------------
//-- Otto has 5 modes:
//--    * MODE = 0: Otto is awaiting
//--    * MODE = 1: Dancing mode!
//--    * MODE = 2: Obstacle detector mode
//--    * MODE = 3: Noise detector mode
//--    * MODE = 4: OttoPAD or any Teleoperation mode (listening SerialPort).
//---------------------------------------------------------
volatile int MODE = 0; //State of Otto in the principal state machine.

unsigned long previousMillis = 0;
int randomDance = 0;
int randomSteps = 0;
bool obstacleDetected = false;
int REDled = 0;
int GREENled = 0;
int BLUEled = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  //Serial communication initialization
  Serial.begin(9600);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin - MUST BE AN ANALOG PIN
  randomSeed(analogRead(PIN_NoiseSensor));   //Set a random seed
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
  pinMode(PIN_Button,INPUT); // - ensure pull-down resistors are used
  //Interrupts
  enableInterrupt(PIN_Button, ButtonPushed, RISING);
  if (enableRGB == true) {  // ONLY IF RGB NEOPIXEL OPTION IS SELECTED
  NeopixelLed.begin();
  NeopixelLed.show(); // Initialize all pixels to 'off'
  NeopixelLed.setBrightness(64); // Op Brightness 
  }
  if (enableEYES == true){
  ledMx.setBrightness(8);
  ledMx.setColorIndex(1);
  }
  //Setup callbacks for SerialCommand commands
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
  SCmd.addCommand("G", receiveServo);     //  sendAck & sendFinalAck
  SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);   // 3v7 lipo battery
  SCmd.addCommand("I", requestProgramId);
  SCmd.addCommand("J", requestMode);
  SCmd.addCommand("P", requestRGB);
  SCmd.addDefaultHandler(receiveStop);
  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
  // Animation Uuuuuh - A little moment of initial surprise
  //-----
  for (int i = 0; i < 2; i++) {
    for (int i = 0; i < 8; i++) {
      Otto.putAnimationMouth(littleUuh, i);
      delay(150);
    }
  }
  //Smile for a happy Otto :)
  Otto.putMouth(smile);
  Otto.sing(S_happy);
  delay(200);

  //If Otto's name is '#' means that Otto hasn't been baptized
  //In this case, Otto does a longer greeting
  //5 = EEPROM address that contains first name character
  if (EEPROM.read(5) == name_fir) {
    Otto.jump(1, 700);
    delay(200);
    Otto.shakeLeg(1, T, 1);
    Otto.putMouth(smallSurprise);
    Otto.swing(2, 800, 20);
    Otto.home();
  }

  Otto.putMouth(happyOpen);
  previousMillis = millis();
// if Pin 7 is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link between pin 7 and GND
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(5000);
  }

}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  if (Serial.available() > 0 && MODE != 4) {
    // test
    //Disable Pin Interruptions
    disableInterrupt(PIN_Button);
    SCmd.readSerial();
    //MODE=4;
    Otto.putMouth(happyOpen);
  }
  // interrupt code, here we do something if TOUCH sensor or BUTTON pressed
  if (buttonPushed){ 
    MODE = MODE +1; 
    if (MODE == 4) MODE = 0;
    Otto.sing(S_mode1);
    if (MODE == 0) Otto.putMouth(zero);
    if (MODE == 1) Otto.putMouth(one);
    if (MODE == 2) Otto.putMouth(two);
    if (MODE == 3) Otto.putMouth(three);
    delay(500);
    Otto.putMouth(happyOpen);
    buttonPushed = false;
  }
  switch (MODE) {

    //-- MODE 0 - Otto is awaiting
    //---------------------------------------------------------
    case 0:
      //Every 80 seconds in this mode, Otto falls asleep
      if (millis() - previousMillis >= 80000) {
        OttoSleeping_withInterrupts(); //ZZzzzzz...
        previousMillis = millis();
      }

      break;

    //-- MODE 1 - Dance Mode!
    //---------------------------------------------------------
    case 1:
      randomDance = random(5, 21); //5,20
      if ((randomDance > 14) && (randomDance < 19)) {
        randomSteps = 1;
        T = 1600;
      }
      else {
        randomSteps = random(3, 6); //3,5
        T = 1000;
      }

      Otto.putMouth(random(10, 21));
      for (int i = 0; i < randomSteps; i++) move(randomDance);
      break;

    //-- MODE 2 - Obstacle detector mode
    //---------------------------------------------------------
    case 2:
      if (obstacleDetected) {
        Otto.putMouth(bigSurprise);
        Otto.sing(S_surprise);
        Otto.jump(5, 500);
        Otto.putMouth(confused);
        Otto.sing(S_cuddly);
        //Otto takes two steps back
        for (int i = 0; i < 3; i++) Otto.walk(1, 1300, -1);
        delay(100);
        obstacleDetector();
        delay(100);
        //If there are no obstacles and no button is pressed, Otto shows a smile
        if (obstacleDetected == true) break;
        else {
          Otto.putMouth(smile);
          delay(50);
          obstacleDetector();
        }

        //If there are no obstacles and no button is pressed, Otto shows turns left
        for (int i = 0; i < 3; i++) {
          if (obstacleDetected == true) break;
          else {
            Otto.turn(1, 1000, 1);
            obstacleDetector();
          }
        }

        //If there are no obstacles and no button is pressed, Otto is happy
        if (obstacleDetected == true) break;
        else {
          Otto.home();
          Otto.putMouth(happyOpen);
          Otto.sing(S_happy_short);
          delay(200);
        }
      }
      else {
        Otto.walk(1, 1000, 1); //Otto walk straight
        obstacleDetector();
      }
      break;

    //-- MODE 3 - Noise detector mode
    //---------------------------------------------------------
    case 3:
    
      //Serial.println(Otto.getNoise());
      if (Otto.getNoise() >= 650) { //740

        delay(50);  //Wait for the possible 'lag' of the button interruptions.
        //Sometimes, the noise sensor detect the button before the interruption takes efect
        Otto.putMouth(bigSurprise);
        Otto.sing(S_OhOoh);
        Otto.putMouth(random(10, 21));
        randomDance = random(5, 21);
        move(randomDance);
        Otto.home();
        delay(500); //Wait for possible noise of the servos while get home
        Otto.putMouth(happyOpen);
      }
      break;

    //-- MODE 4 - OttoPAD or any Teleoperation mode (listening SerialPort)
    //---------------------------------------------------------
    case 4:
      SCmd.readSerial();
      //If Otto is moving yet
      if (Otto.getRestState() == false) move(moveId);
      break;
    default:
          MODE=0;
          break;
    }
    
    if ((MODE !=2) && (enableEYES == true)){
      //Otto.putMouth(smile);
    //Every 50 seconds in this mode, Tito shows time 
        if (millis()-previousMillis>=5000){
          ledMx.drawBitmapEYE(Bitmap_Blink);
             delay(200);
            previousMillis=millis();         
        }
        else{
          ledMx.drawBitmapEYE(Bitmap_Happy);
          delay(200);
        }
    }
}  

///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

//-- Function to read distance sensor & to actualize obstacleDetected variable
void obstacleDetector() {
  int distance = Otto.getDistance();
  if (distance < 15) obstacleDetected = true;
  else obstacleDetected = false;
}

//-- Function to receive Stop command.
void receiveStop() {
  sendAck();
  Otto.home();
  sendFinalAck();
}

//-- Function to receive LED commands
void receiveLED() {

  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  //Examples of receiveLED Bluetooth commands
  //L 000000001000010100100011000000000
  //L 001111111111111111111111111111111 (todos los LED encendidos)
  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg = SCmd.next();
  //Serial.println (arg);
  if (arg != NULL) {
    matrix = strtoul(arg, &endstr, 2); // Converts a char string to unsigned long integer
    Otto.putMouth(matrix, false);
  }
  else {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  sendFinalAck();
}

//-- Function to receive buzzer commands
void recieveBuzzer() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  bool error = false;
  int frec;
  int duration;
  char *arg;

  arg = SCmd.next();
  if (arg != NULL) frec = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) duration = atoi(arg);  // Converts a char string to an integer
  else error = true;
  if (error == true) {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  else Otto._tone(frec, duration, 1);
  sendFinalAck();
}

//-- Function to receive TRims commands
void receiveTrims() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();
  int trim_YL, trim_YR, trim_RL, trim_RR;

  //Definition of Servo Bluetooth command
  //C trim_YL trim_YR trim_RL trim_RR
  //Examples of receiveTrims Bluetooth commands
  //C 20 0 -8 3
  bool error = false;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) trim_YL = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) trim_YR = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) trim_RL = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) trim_RR = atoi(arg);  // Converts a char string to an integer
  else error = true;
  if (error == true) {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();

  } else { //Save it on EEPROM
    Otto.setTrims(trim_YL, trim_YR, trim_RL, trim_RR);
    Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
  }
  sendFinalAck();
}

//-- Function to receive Servo commands
void receiveServo() {
  sendAck();
  moveId = 30;

  //Definition of Servo Bluetooth command
  //G  servo_YL servo_YR servo_RL servo_RR
  //Example of receiveServo Bluetooth commands
  //G 90 85 96 78
  bool error = false;
  char *arg;
  int servo_YL, servo_YR, servo_RL, servo_RR;

  arg = SCmd.next();
  if (arg != NULL) servo_YL = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) servo_YR = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) servo_RL = atoi(arg);  // Converts a char string to an integer
  else error = true;

  arg = SCmd.next();
  if (arg != NULL) {
    servo_RR = atoi(arg);  // Converts a char string to an integer
  }
  else error = true;
  if (error == true) {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  else { //Update Servo:
    int servoPos[4] = {servo_YL, servo_YR, servo_RL, servo_RR};
    Otto._moveServos(200, servoPos);   //Move 200ms
  }
  sendFinalAck();
}

//-- Function to receive movement commands
void receiveMovement() {
  sendAck();
  if (Otto.getRestState() == true) Otto.setRestState(false);

  //Definition of Movement Bluetooth commands
  //M  MoveID  T   MoveSize
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) moveId = atoi(arg);
  else {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
    moveId = 0; //stop
  }
  arg = SCmd.next();
  if (arg != NULL) T = atoi(arg);
  else T = 1000;
  arg = SCmd.next();
  if (arg != NULL) moveSize = atoi(arg);
  else moveSize = 15;
}

//-- Function to execute the right movement according the movement command received.
void move(int moveId) {
  bool manualMode = false;

  switch (moveId) {
    case 0:
      Otto.home();
      break;
    case 1: //M 1 1000
      Otto.walk(1, T, 1);
      break;
    case 2: //M 2 1000
      Otto.walk(1, T, -1);
      break;
    case 3: //M 3 1000
      Otto.turn(1, T, 1);
      break;
    case 4: //M 4 1000
      Otto.turn(1, T, -1);
      break;
    case 5: //M 5 1000 30
      Otto.updown(1, T, moveSize);
      break;
    case 6: //M 6 1000 30
      Otto.moonwalker(1, T, moveSize, 1);
      break;
    case 7: //M 7 1000 30
      Otto.moonwalker(1, T, moveSize, -1);
      break;
    case 8: //M 8 1000 30
      Otto.swing(1, T, moveSize);
      break;
    case 9: //M 9 1000 30
      Otto.crusaito(1, T, moveSize, 1);
      break;
    case 10: //M 10 1000 30
      Otto.crusaito(1, T, moveSize, -1);
      break;
    case 11: //M 11 1000
      Otto.jump(1, T);
      break;
    case 12: //M 12 1000 30
      Otto.flapping(1, T, moveSize, 1);
      break;
    case 13: //M 13 1000 30
      Otto.flapping(1, T, moveSize, -1);
      break;
    case 14: //M 14 1000 20
      Otto.tiptoeSwing(1, T, moveSize);
      break;
    case 15: //M 15 500
      Otto.bend(1, T, 1);
      break;
    case 16: //M 16 500
      Otto.bend(1, T, -1);
      break;
    case 17: //M 17 500
      Otto.shakeLeg(1, T, 1);
      break;
    case 18: //M 18 500
      Otto.shakeLeg(1, T, -1);
      break;
    case 19: //M 19 500 20
      Otto.jitter(1, T, moveSize);
      break;
    case 20: //M 20 500 15
      Otto.ascendingTurn(1, T, moveSize);
      break;
    default:
      manualMode = true;
      break;
  }
  if (!manualMode) sendFinalAck();
}

//-- Function to receive gesture commands
void receiveGesture() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  //Definition of Gesture Bluetooth commands
  //H  GestureID
  int gesture = 0;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) gesture = atoi(arg);
  else     delay(2000); //Otto.putMouth(xMouth); //Otto.clearMouth();

  switch (gesture) {
    case 1: //H 1
      Otto.playGesture(OttoHappy);
      break;
    case 2: //H 2
      Otto.playGesture(OttoSuperHappy);
      break;
    case 3: //H 3
      Otto.playGesture(OttoSad);
      break;
    case 4: //H 4
      Otto.playGesture(OttoSleeping);
      break;
    case 5: //H 5
      Otto.playGesture(OttoFart);
      break;
    case 6: //H 6
      Otto.playGesture(OttoConfused);
      break;
    case 7: //H 7
      Otto.playGesture(OttoLove);
      break;
    case 8: //H 8
      Otto.playGesture(OttoAngry);
      break;
    case 9: //H 9
      Otto.playGesture(OttoFretful);
      break;
    case 10: //H 10
      Otto.playGesture(OttoMagic);
      break;
    case 11: //H 11
      Otto.playGesture(OttoWave);
      break;
    case 12: //H 12
      Otto.playGesture(OttoVictory);
      break;
    case 13: //H 13
      Otto.playGesture(OttoFail);
      break;
    default:
      break;
  }
  sendFinalAck();
}

//-- Function to receive sing commands
void receiveSing() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  //Definition of Sing Bluetooth commands
  //K  SingID
  int sing = 0;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) sing = atoi(arg);
  else     delay(2000); // Otto.putMouth(xMouth); //Otto.clearMouth();

  switch (sing) {
    case 1: //K 1
      Otto.sing(S_connection);
      break;
    case 2: //K 2
      Otto.sing(S_disconnection);
      break;
    case 3: //K 3
      Otto.sing(S_surprise);
      break;
    case 4: //K 4
      Otto.sing(S_OhOoh);
      break;
    case 5: //K 5
      Otto.sing(S_OhOoh2);
      break;
    case 6: //K 6
      Otto.sing(S_cuddly);
      break;
    case 7: //K 7
      Otto.sing(S_sleeping);
      break;
    case 8: //K 8
      Otto.sing(S_happy);
      break;
    case 9: //K 9
      Otto.sing(S_superHappy);
      break;
    case 10: //K 10
      Otto.sing(S_happy_short);
      break;
    case 11: //K 11
      Otto.sing(S_sad);
      break;
    case 12: //K 12
      Otto.sing(S_confused);
      break;
    case 13: //K 13
      Otto.sing(S_fart1);
      break;
    case 14: //K 14
      Otto.sing(S_fart2);
      break;
    case 15: //K 15
      Otto.sing(S_fart3);
      break;
    case 16: //K 16
      Otto.sing(S_mode1);
      break;
    case 17: //K 17
      Otto.sing(S_mode2);
      break;
    case 18: //K 18
      Otto.sing(S_mode3);
      break;
    case 19: //K 19
      Otto.sing(S_buttonPushed);
      break;
    default:
      break;
  }
  sendFinalAck();
}

//-- Function to receive Name command
void receiveName() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();
  char newOttoName[11] = "";  //Variable to store data read from Serial.
  int eeAddress = 5;          //Location we want the data to be in EEPROM.
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) {
    //Complete newOttoName char string
    int k = 0;
    while ((*arg) && (k < 11)) {
      newOttoName[k] = *arg++;
      k++;
    }
    EEPROM.put(eeAddress, newOttoName);
  }
  else
  {
    //Otto.putMouth(xMouth);
    delay(2000);
    //Otto.clearMouth();
  }
  sendFinalAck();
}

//-- Function to send Otto's name
void requestName() {
  Otto.home(); //stop if necessary
  char actualOttoName[11] = ""; //Variable to store data read from EEPROM.
  int eeAddress = 5;            //EEPROM address to start reading from
  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get(eeAddress, actualOttoName);

  Serial.print(F("&&"));
  Serial.print(F("E "));
  Serial.print(actualOttoName);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send ultrasonic sensor measure (distance in "cm")
void requestDistance() {
  Otto.home();  //stop if necessary
  int distance = Otto.getDistance();
  Serial.print(F("&&"));
  Serial.print(F("D "));
  Serial.print(distance);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send noise sensor measure
void requestNoise() {
  Otto.home();  //stop if necessary
  int microphone = Otto.getNoise(); //analogRead(PIN_NoiseSensor);
  Serial.print(F("&&"));
  Serial.print(F("N "));
  Serial.print(microphone);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send battery voltage percent
void requestBattery() {
  Otto.home();  //stop if necessary
  //The first read of the battery is often a wrong reading, so we will discard this value.
  double batteryLevel = Otto.getBatteryLevel();
  Serial.print(F("&&"));
  Serial.print(F("B "));
  Serial.print(batteryLevel);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send program ID
void requestProgramId() {
  Otto.home();   //stop if necessary
  Serial.print(F("&&"));
  Serial.print(F("I "));
  Serial.print(programID);
  Serial.println(F("%%"));
  Serial.flush();
}


//-- Function to send Ack comand (A)
void sendAck() {
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send final Ack comand (F)
void sendFinalAck() {
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}
//-- Function to receive mode selection.
void requestMode() {
  sendAck();
  Otto.home();
  //Definition of Mode Bluetooth commands
  //J ModeID
  char *arg;
  arg = SCmd.next();
  if (arg != NULL)
  {
    modeId = atoi(arg);
    Otto.putMouth(heart);
  }
  else {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
    modeId = 0; //stop
  }
  switch (modeId) {
    case 0: //
      MODE = 0;
      break;
    case 1: //
      MODE = 1;
      Otto.sing(S_mode1);
      Otto.putMouth(one);
      delay(1000);
      delay(200);
      break;
    case 2: //
      MODE = 2;
      Otto.sing(S_mode2);
      Otto.putMouth(two);
      delay(1000);
      break;
    case 3: //
      MODE = 3;
      Otto.sing(S_mode3);
      Otto.putMouth(three);
      delay(1000);
      break;
    case 4: //
      Otto.sing(S_mode1);
      Otto.putMouth(four);
      delay(1000);
      MODE = 4;
      break;
    default:
      MODE = 0;
      break;
  }
  sendFinalAck();
}


  //-- Function to receive RGB colours.
  void requestRGB(){

    sendAck();
    Otto.home();
    //P red breen blue
    char *arg;
    arg = SCmd.next();
    if (arg != NULL)
    {
      REDled=atoi(arg);
      }
      else{
      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();
      REDled=0; //stop
    }

    arg = SCmd.next();
    if (arg != NULL) {GREENled=atoi(arg);}
    else{
      GREENled=0;
    }

    arg = SCmd.next();
    if (arg != NULL) {BLUEled=atoi(arg);}
    else{
      BLUEled =0;
    }
    Otto.putMouth(okMouth);
      delay(2000);
      if (enableRGB == true) { // ONLY IF RGB NEOPIXEL OPTION IS CHOSEN
         //Conversion(ColorValue);
      NeopixelLed.setPixelColor(0, NeopixelLed.Color(REDled, GREENled, BLUEled));
      NeopixelLed.show();
      delay(50);
      }
      Otto.clearMouth();
      Otto.putMouth(happyOpen);
  }

//-- Functions with animatics
//--------------------------------------------------------

void OttoLowBatteryAlarm() {
  double batteryLevel = Otto.getBatteryLevel();
  if (batteryLevel < 45) {
    Otto.putMouth(thunder);
    Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
    delay(30);
    Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
    Otto.clearMouth();
    delay(500);

  }
}

void OttoSleeping_withInterrupts() {
  int bedPos_0[4] = {100, 80, 60, 120};
  Otto._moveServos(700, bedPos_0);
  for (int i = 0; i < 4; i++) {
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (100, 200, 1.04, 10, 10);
    Otto.putAnimationMouth(dreamMouth, 1);
    Otto.bendTones (200, 300, 1.04, 10, 10);
    Otto.putAnimationMouth(dreamMouth, 2);
    Otto.bendTones (300, 500, 1.04, 10, 10);
    delay(500);
    Otto.putAnimationMouth(dreamMouth, 1);
    Otto.bendTones (400, 250, 1.04, 10, 1);
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (250, 100, 1.04, 10, 1);
    delay(500);
  }
  Otto.putMouth(lineMouth);
  Otto.sing(S_cuddly);
  Otto.home();
  Otto.putMouth(happyOpen);

}
//-- Function executed when  button is pushed / Touch sensor VIA INTERRUPT routine
void ButtonPushed(){ 
    if(!buttonPushed){
        buttonPushed=true;
        Otto.putMouth(smallSurprise);
    } 
} 
