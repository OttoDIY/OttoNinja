//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY Humanoid APP Firmware version 9 (V9) with standard baudrate of 9600 for Bluetooth modules.
//-- This code will have all modes and functions therefore memory is almost full but ignore the alert it works perfectly.
//-- Designed to work with the basic Otto or PLUS or Humanoid or other biped robots. some of these functions will need a good power source such as a LIPO battery.
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -- ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen October 2018
// -- ADDED PIN definitions for ease of use: Jason Snow November 2018
// -- ADDED Battery meassurementin in mode 3 Jason Snow August 2019
// -- ADDED TEXT display on matrix Jason Snow September 2019
//-------------------------------------------------------------------------

#include <EnableInterrupt.h>
#include <Adafruit_NeoPixel.h> // Library to manage the Neopixel RGB led
#include <OttoSerialCommand.h> //-- Library to manage serial commands
OttoSerialCommand SCmd;  // The SerialCommand object
#include <Otto9Humanoid.h> //-- Otto Library version 9
Otto9Humanoid Otto;  //This is Otto!
//---------------------------------------------------------
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
RA 7==> |               | <== LA 6
        |               |
LR 3==> |               | <== LL 2
         --------------- 
            ||     ||
            ||     ||
FR 5==>   -----   ------  <== FL 4
         |-----   ------|
*/
#if defined(ESP32)
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 33 //servo[0]  left leg
#define PIN_YR 25 //servo[1]  right leg
#define PIN_RL 26 //servo[2]  left foot
#define PIN_RR 27 //servo[3]  right foot
#define PIN_LA 23 //servo[4]  Left arm if enabled
#define PIN_RA 22 //servo[5]  Right arm if enabled
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  2  //TRIGGER pin (2)
#define PIN_Echo     15  //ECHO pin (15)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  4 //BUZZER pin (4)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor 35  //SOUND SENSOR   ESP32 pin(35)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    19   //DIN pin (19)
#define CS_PIN      5   //CS pin (5)
#define CLK_PIN    18   //CLK pin (18)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
boolean BATTcheck = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define PIN_Battery   36      //3v7 BATTERY MONITOR   ANALOG pin (36)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   T4 // TOUCH SENSOR Pin T4 GPIO13
#define TOUCH_THRESHOLD 40
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
boolean enableRGB = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define NeopixelRGB_PIN  12 // NEOPIXEL pin   DIGITAL PIN (12)
#define NUMPIXELS       1   // There is only one Neopixel use in MY Otto, chnage for more than 1
Adafruit_NeoPixel NeopixelLed = Adafruit_NeoPixel(NUMPIXELS, NeopixelRGB_PIN, NEO_RGB + NEO_KHZ800);
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 10 and GND
#define PIN_ASSEMBLY    10   //ASSEMBLY pin (10) LOW = assembly    HIGH  = normal operation
#define SERIAL_BAUD 57600  // we are not using this for bluetooth
#else
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_LA 6 //servo[4]  Left arm if enabled
#define PIN_RA 7 //servo[5]  Right arm if enabled
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
boolean BATTcheck = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define PIN_Battery   A7  //3v7 BATTERY MONITOR   ANALOG pin (A7)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
boolean enableRGB = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define NeopixelRGB_PIN  12 // NEOPIXEL pin   DIGITAL PIN (12)
#define NUMPIXELS       1   // There is only one Neopixel use in MY Otto, chnage for more than 1
Adafruit_NeoPixel NeopixelLed = Adafruit_NeoPixel(NUMPIXELS, NeopixelRGB_PIN, NEO_RGB + NEO_KHZ800);
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 10 and GND
#define PIN_ASSEMBLY    10   //ASSEMBLY pin (10) LOW = assembly    HIGH  = normal operation
#define SERIAL_BAUD 9600  // so we can use this for bluetooth
#endif
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////

const char programID[] = "OttoHumanoid_V9"; //Each program will have a ID
const char message1[] = "I AM OTTO"; //9 characters MAXIMUM
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
//--    * MODE = 3: Battery chek mode for Otto with LED matrix mouth
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
double batteryCHECK = 0;
unsigned long int matrix;
unsigned long timerMillis = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  //Serial communication initialization
  SCmd.begin(SERIAL_BAUD);
  Otto.initHUMANOID(PIN_YL, PIN_YR, PIN_RL, PIN_RR, PIN_LA, PIN_RA, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin - MUST BE AN ANALOG PIN
  randomSeed(analogRead(PIN_NoiseSensor));   //Set a random seed
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
#if defined(ESP32)
  // there is a real Touch interface. use it.
  touchAttachInterrupt(T4, ButtonPushed, TOUCH_THRESHOLD);
#else
  pinMode(PIN_Button,INPUT); // - ensure pull-down resistors are used
  //Interrupts
  enableInterrupt(PIN_Button, ButtonPushed, RISING);
#endif
  if (enableRGB == true) {  // ONLY IF RGB NEOPIXEL OPTION IS SELECTED
  NeopixelLed.begin();
  NeopixelLed.show(); // Initialize all pixels to 'off'
  NeopixelLed.setBrightness(64); // Op Brightness 
  }
  //Setup callbacks for SerialCommand commands
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
  SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("B", requestBattery);   // 3v7 lipo battery
  SCmd.addCommand("I", requestProgramId);
  SCmd.addCommand("J", requestMode);
  SCmd.addCommand("P", requestRGB);
  SCmd.addDefaultHandler(receiveStop);

#if defined(ESP32)
  // Startup Bluetooth serial port
  SCmd.begin("ESP32_OttoHumanoid");
  Serial.println("Staring BTSerial");
#endif

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
  previousMillis = millis();
// if Pin 10 is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link between pin 10 and GND
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(5000);
  }
delay (4000);
Otto.clearMouth();
// test for matrix
 matrix = 0b000000000000000000000000000001;// set the variable to the first LED bottom RHS again
  for (int i = 0; i < 30; i++) { // this FOR NEXT LOOP repeats the code following it 30 times
      Otto.putMouth(matrix, false); // display the single LED
      matrix = matrix << 1 ;// shift the single LED one to the left (to the next LED)
      delay(250);// wait for 1/4 second, this is so that you can see the image on the Matrix 
      Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
    }
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second before repeating
  Otto.clearMouth();  
// write a text string of no more than nine limited characters and scroll at a speed between 50 and 150 (FAST and SLOW)
 // limited characters are : CAPITALS A to Z   NUMBERS 0 to 9    'SPACE'  : ; < >  = @ 
Otto.writeText (message1, 70);
delay (2000);
Otto.clearMouth();
Otto.putMouth(smile);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
 if (SCmd.available() > 0 && MODE != 4) {
    MODE=4;
    Otto.putMouth(happyOpen);
  }
   //Every 60 seconds check battery level
   if (BATTcheck == true) {
      if (millis() - timerMillis >= 60000) {
        OttoLowBatteryAlarm();
        timerMillis = millis();
      }
   }
  // interrupt code, here we do something if TOUCH sensor or BUTTON pressed
  if (buttonPushed){ 
    MODE = MODE +1; 
    if (MODE == 5) MODE = 0;
    Otto.sing(S_mode1);
    if (MODE == 0) Otto.putMouth(zero);
    if (MODE == 1) Otto.putMouth(one);
    if (MODE == 2) Otto.putMouth(two);
    if (MODE == 3) Otto.putMouth(three);
    if (MODE == 4) Otto.putMouth(four);
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
      randomDance = random(5, 23); //5,20
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
    // battery display as an icon on the mouth, battery icon will has three levels of power
      if (BATTcheck == true) {
      batteryCHECK = Otto.getBatteryLevel();
        Otto.clearMouth();
        if (batteryCHECK < 40)
        {
          matrix = 0b00001100010010010010010010011110; // show empty battery symbol
          Otto.putMouth(matrix, false);
        }
        if (batteryCHECK > 45)
        {
          matrix = 0b00001100010010010010011110011110; // show empty battery symbol
          Otto.putMouth(matrix, false);
        }
       
        if (batteryCHECK > 65)
        {
          matrix = 0b00001100010010011110011110011110; // show empty battery symbol
          Otto.putMouth(matrix, false);
        }
        if (batteryCHECK > 80)
        {
          matrix = 0b00001100011110011110011110011110; // show empty battery symbol
          Otto.putMouth(matrix, false);
        }
        delay(1500);
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


//-- Function to receive TRims commands
void receiveTrims() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();
  int trim_YL, trim_YR, trim_RL, trim_RR, trim_LA, trim_RA;

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
  
 arg = SCmd.next();
  if (arg != NULL) trim_LA = atoi(arg);  // Converts a char string to an integer
  else error = true;


  arg = SCmd.next();
  if (arg != NULL) trim_RA = atoi(arg);  // Converts a char string to an integer
  else error = true;
  if (error == true) {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();

  } else { //Save it on EEPROM
    Otto.setTrims(trim_YL, trim_YR, trim_RL, trim_RR, trim_LA, trim_RA);
    Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
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
    case 21: //M 21
      Otto.handsup();
      break;
    case 22: //M 22 right arm
      Otto.handwave(1);
      break;
    case 23: //M 23 left arm
      Otto.handwave(-1);
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
  char newOttoName[9] = "";  //Variable to store data read from Serial.
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) {
    //Complete newOttoName char string
    int k = 0;
    while ((*arg) && (k < 9)) {
      newOttoName[k] = *arg++;
      k++;
    }
    // write a text string of no more than nine limited characters and scroll at a speed between 50 and 150 (FAST and SLOW)
    // limited characters are : CAPITALS A to Z   NUMBERS 0 to 9    'SPACE'  : ; < >  = @ 
    Otto.clearMouth();
    Otto.writeText (newOttoName, 75);
    delay (1000);
    Otto.clearMouth();
  }
  else
  {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  sendFinalAck();
}


//-- Function to send ultrasonic sensor measure (distance in "cm")
void requestDistance() {
  Otto.home();  //stop if necessary
  int distance = Otto.getDistance();
  SCmd.print(F("&&"));
  SCmd.print(F("D "));
  SCmd.print(distance);
  SCmd.println(F("%%"));
  SCmd.flush();
}


//-- Function to send battery voltage percent
void requestBattery() {
  Otto.home();  //stop if necessary
  //The first read of the battery is often a wrong reading, so we will discard this value.
  double batteryLevel = Otto.getBatteryLevel();
  SCmd.print(F("&&"));
  SCmd.print(F("B "));
  SCmd.print(batteryLevel);
  SCmd.println(F("%%"));
  SCmd.flush();
}

//-- Function to send program ID
void requestProgramId() {
  Otto.home();   //stop if necessary
  SCmd.print(F("&&"));
  SCmd.print(F("I "));
  SCmd.print(programID);
  SCmd.println(F("%%"));
  SCmd.flush();
}


//-- Function to send Ack comand (A)
void sendAck() {
  delay(30);
  SCmd.print(F("&&"));
  SCmd.print(F("A"));
  SCmd.println(F("%%"));
  SCmd.flush();
}

//-- Function to send final Ack comand (F)
void sendFinalAck() {
  delay(30);
  SCmd.print(F("&&"));
  SCmd.print(F("F"));
  SCmd.println(F("%%"));
  SCmd.flush();
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

//-- Function to read battery level - if it is low then show low battery
void OttoLowBatteryAlarm() {
  //
   double batteryLevel = Otto.getBatteryLevel();
  if (batteryLevel < 35) {
    Otto.putMouth(thunder);
    Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
    delay(30);
    Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
    delay(30);
    Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
    delay(30);
    Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
    Otto.clearMouth();
    matrix = 0b00001100010010010010010010011110; // show empty battery symbol
     Otto.putMouth(matrix, false);
    delay(2000);
    Otto.clearMouth();
     delay(1000);
    matrix = 0b00001100010010010010010010011110; // show empty battery symbol
    Otto.putMouth(matrix, false);
    delay(2000);
    Otto.clearMouth();
    Otto.putMouth(happyOpen);
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
void checkBATT(void* context) 
{
OttoLowBatteryAlarm();
}
