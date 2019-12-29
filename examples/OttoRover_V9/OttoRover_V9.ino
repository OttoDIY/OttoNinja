//-----------------------------------------------------------------
//-- Otto DIY Rover APP Firmware version 9 (V9) with standard baudrate of 9600 for Bluetooth modules.
//-- This code will have all modes and functions therefore memory is almost full but ignore the alert it works perfectly.
//-- Designed to work with Rover or other wheel robots. some of these functions will need a good power source such as a LIPO battery.
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//------------------------------------------------------------------
// -- ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen OCT 2018
// -- Added PIN definitions for ease of use: Jason Snow NOV 2018
// -- ADDED NEOPIXEL: Paul Van De Veen NOV 2018
// -- ADDED Eye Matrix Progmem and control: Jason Snow NOV 2018
// -- REMOVED Eye Matrix Progmem and control: Jason Snow AUG 2019
// -- ADDED TEXT display on matrix Jason Snow September 2019
//-------------------------------------------------------------------
// Otto libraries version 9        June 2019
//-------------------------------------------------------------------
// Otto Rover version 9 sketch   July 2019
//-------------------------------------------------------------------
#include <EEPROM.h>
#include <EnableInterrupt.h> // Library to manage the Neopixel RGB led
#include <Adafruit_NeoPixel.h> //-- Library to manage serial commands
#include <OttoSerialCommand.h>
OttoSerialCommand SCmd;  // The SerialCommand object
#include <Otto9.h> //-- Otto Library version
Otto9 Otto;  //This is Otto!
//---------------------------------------------------------
//--Configure the pins where the motor drivers are attached
/*
          --------------- 
         |     O   O     |
         |---------------|
PWM 6==>||               || <== PWM 5
DIR 7==>| ------  ------  | <== DIR 4
        |                 |
*/
// MOTOR CONTROL PINs //////////////////////////////////////////////////////////////////////////
#define Motor_1_PWM 6 // motor 1 speed PWM pin (6)   (L9110 driver module mE1)
#define Motor_2_PWM 5 // motor 2 speed PWM pin (5)   (L9110 driver module mE2)
#define Motor_1_DIR 7 // motor 1 direction pin (7)   (L9110 driver module mI1)
#define Motor_2_DIR 4 // motor 2 direction pin (4)   (L9110 driver module mI2)
boolean DCMotor = true;    // SET TO TRUE for DC motor SMARS version or Sketch will fail to control the motors
boolean L9110 = true;     // select this option when using the L9110 driver module or DRV8833 module
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13  //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
boolean BATTcheck = true;    // SET TO FALSE IF NOT USING THIS OPTION
#define PIN_Battery   A7  //3v7 BATTERY MONITOR   ANALOG pin (A7)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
boolean enableRGB = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define NeopixelRGB_PIN  12 // NEOPIXEL pin   DIGITAL PIN (12)
#define NUMPIXELS       1   // There is only one Neopixel use in MY Otto, chnage for more than 1
Adafruit_NeoPixel NeopixelLed = Adafruit_NeoPixel(NUMPIXELS, NeopixelRGB_PIN, NEO_RGB + NEO_KHZ800);
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
const char programID[] = "OttoRover_V9"; //Each program will have an ID 
const char message1[] = "I AM OTTO"; //9 characters MAXIMUM
///////////////////////////////////////////////////////////////////
bool goingforward = false; // motor direction logic
bool goingreverse = false; // motor direction logic
bool goingleft = false; // motor direction logic
bool goingright = false; // motor direction logic
bool APPleftFORWARD = false; // motor direction logic for APP mode 4
bool APPleftREVERSE = false; // motor direction logic for APP mode 4
bool APPrightFORWARD = false; // motor direction logic for APP mode 4
bool APPrightREVERSE = false; // motor direction logic for APP mode 4
int leftSPEED = 0; //  left motor speed variable
int rightSPEED = 0; //  right motor speed variable
int mDelay = 10; // motor delay before changing movement
int lSpeed = 150; // motor speed (higher is faster) MAX 255 - for mode 2 speed
int rSpeed = 150; // motor speed (higher is faster) MAX 255 - for mode 2 speed
int APPleftSPEED = 0; //  left motor speed variable from APP mode 4
int APPrightSPEED = 0; //  right motor speed variable from APP mode 4
bool AUXcontrol1 = false; // logic for aux control switch 1 from APP
bool AUXcontrol2 = false; // logic for aux control switch 2 from APP 
///////////////////////////////////////////////////////////////////
#define OUT1 2 // Aux output 1 - pin 2
#define OUT2 3 // Aux output 2 - pin 3
///////////////////////////////////////////////////////////////////
int calibration = 0; // used if one motor is faster than the other due to mechanical differences
volatile int MODE = 0; //State of Otto in the principal state machine.
unsigned long previousMillis = 0;
int randomDance = 0;
int randomSteps = 0;
bool obstacleDetected = false;
int REDled = 0;
int GREENled = 0;
int BLUEled = 0;
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int modeId = 0;          //Number of mode
int moveSize = 15;       //Asociated with the height of some movements
volatile bool buttonPushed=false;  //Variable to remember when a button has been pushed
double batteryCHECK = 0;
unsigned long int matrix;
unsigned long timerMillis = 0;
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  rSpeed = (rSpeed - calibration); // make calibration adjustment to one motor speed value only
  pinMode(Motor_1_PWM, OUTPUT);
  pinMode(Motor_2_PWM, OUTPUT);
  pinMode(Motor_1_DIR, OUTPUT);
  pinMode(Motor_2_DIR, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  digitalWrite (OUT1, LOW);
  digitalWrite (OUT2, LOW);
  ///////////////////////////////////////////////////////////////////
  //Serial communication initialization
  Serial.begin(9600);
  ///////////////////////////////////////////////////////////////////
  // SELECT ONE OF THE BELOW INIT based on your Otto
  //Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins for standard Otto biped
  Otto.initDC(PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the ultrasonic pins for the DC motor version such as SMARS
  ////////////////////////////////////////////////////////////////
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin MUST BE ANALOG PIN
  randomSeed(analogRead(PIN_NoiseSensor));   //Set a random seed
  pinMode(PIN_Button,INPUT); // - ensure pull-down resistors are used
  //Interrupts
  enableInterrupt(PIN_Button, ButtonPushed, RISING);
  if (enableRGB == true) {  // ONLY IF RGB NEOPIXEL OPTION IS SELECTED
  NeopixelLed.begin();
  NeopixelLed.show(); // Initialize all pixels to 'off'
  NeopixelLed.setBrightness(64); // Op Brightness 
  }
  //Setup callbacks for SerialCommand commands
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("T", receiveAUX);     // receive aux switches
  if (DCMotor == true){
  SCmd.addCommand("M", receivePWM);  //  receive DC motor commands
  }
  else
  {
   SCmd.addCommand("M", receiveMovement);  //  receive servo commands for Otto biped version
  }
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


  Otto.putMouth(happyOpen);
  previousMillis = millis();
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second before repeating
  Otto.clearMouth();  
  // write a text string of no more than nine limited characters and scroll at a speed between 50 and 150 (FAST and SLOW)
 // limited characters are : CAPITALS A to Z   NUMBERS 0 to 9    'SPACE'  : ; < >  = @ 
  Otto.writeText (message1, 75);
  delay (4000);
  Otto.clearMouth();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop()
{
    if (Serial.available()>0 && MODE!=4)
    {
    SCmd.readSerial();
    Otto.putMouth(happyOpen);
   }
   //Every 60 seconds check battery level
   if (BATTcheck == true) {
      if (millis() - timerMillis >= 60000) {
        OttoLowBatteryAlarm();
        timerMillis = millis();
      }
   }
 if (buttonPushed){ 
   motorstop(); // stop motors or they continuously turn
    MODE = MODE +1; 
    if (MODE == 5) MODE = 0;
    if (MODE == 0) Otto.putMouth(zero);
    if (MODE == 1) Otto.putMouth(one);
    if (MODE == 2) Otto.putMouth(two);
    if (MODE == 3) Otto.putMouth(three);
    if (MODE == 4) Otto.putMouth(four);
    delay(1000);
    Otto.putMouth(happyOpen);
    buttonPushed = false;
      }
  
    switch (MODE) 
     {
            //-- MODE 0 - Otto is awaiting
      //---------------------------------------------------------
    case 0:
      
        //Every 80 seconds in this mode, Otto falls asleep as he is bored!
        if (millis()-previousMillis>=80000)
        {
            OttoSleeping_withInterrupts(); //ZZzzzzz...
            previousMillis=millis();         
        }

        break;
      

      //-- MODE 1 - Dance Mode!
      //---------------------------------------------------------
      case 1:
        // THIS Otto can not dance - he has no legs, so here he displays a random mouth
        // change this for what ever you would like Otto to do in mode 1
        // This would be ideal for a line following mode if you can add some additional 
        // sensors to Otto instead of the AUX outputs, you will need to make some changes to the sketch
        // to ba able to do this.
        Otto.putMouth(random(10,21));
        delay(1000);
        break;


      //-- MODE 2 - Obstacle detector mode
      //---------------------------------------------------------
      case 2:
        if(obstacleDetected)
        {
          motorstop(); // stop motors
            Otto.sing(S_OhOoh);
            delay(100);
              Otto.putMouth(bigSurprise);
              delay(500);
              Otto.putMouth(confused);
              delay(500);
              Otto.sing(S_OhOoh2);
            //Otto drives back
            Otto.putMouth(xMouth);
             //delay(1000);
             motormoveBackward(); // moveBackwards
              delay(1500); 
              motorstop();
              delay(500);           
              Otto.putMouth(smile);
              delay(1500);
                motorturnLeft(); // turn small amount left
                delay(300);
                motorstop();
                obstacleDetector();
                delay(250);
                
           
            //If there are no obstacles, Otto is happy
            if(obstacleDetected==true){break;}           
            
                obstacleDetected=false;
                Otto.putMouth(happyOpen);
                Otto.sing(S_happy_short);
                delay(500);
                
            }
        else
            {

    //select a random number between 1 and 5
      if (randomDance > 20)
          {
      randomSteps = random(1,5);
      // display the relevant gesture for the random number selected
      switch (randomSteps) {
      case 1: //H 1 
        Otto.putMouth(happyOpen);
        Otto.sing(S_happy);
        Otto.putMouth(happyClosed);
        delay(500);
        Otto.putMouth(happyOpen);
        Otto.sing(S_superHappy);
        Otto.putMouth(happyClosed);
        break;
      case 2: //H 2 
        Otto.putMouth(smile);
        Otto.sing(S_cuddly);
        break;
      case 3: //H 3 
        Otto.putMouth(happyClosed);
        break;
      case 4: //H 4 
         Otto.putMouth(tongueOut);  
         Otto.sing(13);  
        break;
      case 5: //H 5  
        Otto.putMouth(lineMouth);
        break;
        
          }
      randomDance = 0;
          }

            //Otto Drive straight
            motormoveForward(); // moveForward
            obstacleDetector(); // check for obstacles
            delay(100);
            obstacleDetector(); // check for obstacles
            delay(100);
            randomDance = (randomDance + 1);

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
      //-- MODE 4 - Otto APP mode (listening SerialPort) 
      //---------------------------------------------------------
      case 4:

        SCmd.readSerial();

        break;      

      default:
          MODE=0;
          break;
 }
      
}

void motormoveForward() {
// STOP motors before direction change to help protect motor drivers
if (L9110 == true){
  if (goingforward == false) motorstop();
 // DIR motor
    digitalWrite(Motor_1_DIR, HIGH);
    digitalWrite(Motor_2_DIR, HIGH);
    // PWM motor
    analogWrite(Motor_1_PWM, lSpeed);
    analogWrite(Motor_2_PWM, rSpeed);
    goingreverse = false;
    goingforward = true;
    goingleft = false;
    goingright = false; 
  }
else{
if (goingforward == false) motorstop();
    // DIR motor
    digitalWrite(Motor_1_DIR, HIGH);//left motor direction
    digitalWrite(Motor_2_DIR, HIGH);//right motor direction
    // PWM motor
    analogWrite( Motor_1_PWM, lSpeed);//left motor speed
    analogWrite(Motor_2_PWM, rSpeed);//right motor speed
    goingreverse = false;
    goingforward = true;
    goingleft = false;
    goingright = false; 
  }
}
/**
 * Move backward
 */
void motormoveBackward() {
// STOP motors before direction change to help protect motor drivers
if (L9110 == true){
if (goingreverse == false) motorstop();  
    // DIR motor
    digitalWrite(Motor_1_DIR, LOW);//left motor direction
    digitalWrite(Motor_2_DIR, LOW);//right motor direction
     // PWM motor
    analogWrite(Motor_1_PWM, lSpeed);//left motor speed
    analogWrite(Motor_2_PWM, rSpeed);//right motor speed
    goingreverse = true;
    goingforward = false;
    goingleft = false;
    goingright = false; 
  }
else
{
if (goingreverse == false) motorstop();  
    // DIR motor
    digitalWrite(Motor_1_DIR, LOW);//left motor direction
    digitalWrite(Motor_2_DIR, LOW);//right motor direction
     // PWM motor
    analogWrite( Motor_1_PWM, lSpeed);//left motor speed
    analogWrite(Motor_2_PWM, rSpeed);//right motor speed
    goingreverse = true;
    goingforward = false;
    goingleft = false;
    goingright = false; 
  }
}

/**
 * Turn Left
 */
void motorturnLeft() {
  //map(APPrightSPEED, 0, -100, 255, 55);
// STOP motors before direction change to help protect motor drivers
if (L9110 == true){
  if (goingleft == false) motorstop();
    // DIR motor   
    digitalWrite(Motor_1_DIR, LOW);
    digitalWrite(Motor_2_DIR, HIGH);
     // PWM motor
    analogWrite(Motor_1_PWM, lSpeed);
    analogWrite(Motor_2_PWM, 255 - rSpeed);
    goingreverse = false;
    goingforward = false;
    goingleft = true;
    goingright = false; 
}
else
{
 
if (goingleft == false) motorstop();
    // DIR motor   
    digitalWrite(Motor_1_DIR, LOW);
    digitalWrite(Motor_2_DIR, HIGH);
    // PWM motor
    analogWrite( Motor_1_PWM, lSpeed);
    analogWrite(Motor_2_PWM, rSpeed);
    goingreverse = false;
    goingforward = false;
    goingleft = true;
    goingright = false; 
  }
}

/**
 * Turn Right
 */
void motorturnRight() {
 // STOP motors before direction change to help protect motor drivers
 if (L9110 == true){
  if (goingright == false) motorstop();   
    // DIR motor 
    digitalWrite(Motor_1_DIR, HIGH);
    digitalWrite(Motor_2_DIR, LOW);
    // PWM motor
    analogWrite(Motor_1_PWM, 255 - lSpeed);
    analogWrite(Motor_2_PWM, rSpeed);
    goingreverse = false;
    goingforward = false;
    goingleft = false;
    goingright = true;
 }
 else
 {
  
if (goingright == false) motorstop();   
    // DIR motor 
    digitalWrite(Motor_1_DIR, HIGH);
    digitalWrite(Motor_2_DIR, LOW);
     // PWM motor
    analogWrite( Motor_1_PWM, lSpeed);
    analogWrite(Motor_2_PWM, rSpeed);
    goingreverse = false;
    goingforward = false;
    goingleft = false;
    goingright = true;  
  }
 }

/**
 * Stop MOTORS
 */
void motorstop() {
    // PWM motor
    digitalWrite(Motor_1_PWM, LOW);
    digitalWrite(Motor_2_PWM, LOW);
    // DIR motor 
    digitalWrite(Motor_1_DIR, LOW);
    digitalWrite(Motor_2_DIR, LOW);
}



//-- Function to receive motor speed and direction commands
void receivePWM(){
    sendAck();
    //Definition of Motor Bluetooth commands
    //M ModeID    
    char *arg;
    arg = SCmd.next(); 
    if (arg != NULL) 
    {
      APPleftSPEED=atoi(arg);
      }
      else{
        motorstop();
     Otto.putMouth(xMouth);
      delay(2000);
       Otto.clearMouth();
      APPleftSPEED=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {APPrightSPEED=atoi(arg);}
    else{
      Otto.putMouth(xMouth);
      delay(2000);
       Otto.clearMouth();
      APPrightSPEED=0;
    }
    //if ( Channel == ChannelNumber){
// D0 PWM motor control
// first map the value received from the app to a value between 0 and full speed
// the value from the app is negative for forward and positive for reverse
 if (APPleftSPEED < -1 ){
  leftSPEED = map(APPleftSPEED, 0, -100, 255, 105);
 }
  else
  {
   leftSPEED = map(APPleftSPEED, 0, 100, 0, 150);
  }
 if (APPrightSPEED < -1 ){
  rightSPEED = map(APPrightSPEED, 0, -100, 255, 105);
 }
 else 
 {
  rightSPEED = map(APPrightSPEED, 0, 100, 0, 150);
 }

// just a little deadband

if (APPleftSPEED > 5 ){
    if (APPleftFORWARD == false){;
      digitalWrite( Motor_1_PWM, LOW);
      digitalWrite(Motor_1_DIR, LOW);
      delay(250);
    }
    // DIR motor
    digitalWrite(Motor_1_DIR, LOW);
    // PWM motor
    analogWrite( Motor_1_PWM,leftSPEED);
    APPleftREVERSE = false;
    APPleftFORWARD = true;
  }
  else  {
  if (APPleftSPEED < -5 ){
    if (APPleftREVERSE == false){;
      digitalWrite( Motor_1_PWM, LOW);
      digitalWrite(Motor_1_DIR, LOW);
      delay(250);
    }
    // DIR motor
    digitalWrite(Motor_1_DIR, HIGH);
    // PWM motor
    analogWrite( Motor_1_PWM,leftSPEED);
    APPleftREVERSE = true;
    APPleftFORWARD = false;
  }
  else{
  digitalWrite(Motor_1_DIR, LOW);
    // PWM motor
    analogWrite( Motor_1_PWM, 0);
    goingreverse = false;
    goingforward = true;
  }
}
if (APPrightSPEED > 5 ){
  if (APPrightFORWARD == false){;
      digitalWrite( Motor_1_PWM, LOW);
      digitalWrite(Motor_1_DIR, LOW);
      delay(250);
    }
    // DIR motor
    digitalWrite(Motor_2_DIR, LOW);
    // PWM motor
    analogWrite(Motor_2_PWM,rightSPEED);
    APPrightREVERSE = false;
    APPrightFORWARD = true;
  }
  else  {
  if (APPrightSPEED < -5 ){
    if (APPrightREVERSE == false){;
      digitalWrite( Motor_1_PWM, LOW);
      digitalWrite(Motor_1_DIR, LOW);
      delay(250);
    }
    digitalWrite(Motor_2_DIR, HIGH);
    // PWM motor
    analogWrite(Motor_2_PWM,rightSPEED);
    APPrightREVERSE = true;
    APPrightFORWARD = false;
  }
  else{
  digitalWrite(Motor_2_DIR, LOW);
    // PWM motor
    analogWrite(Motor_2_PWM, 0);
    APPrightFORWARD = false;
    APPrightFORWARD = true;
      }
    }

}

void receiveAUX()
{
sendAck();
    //Definition of Mode Bluetooth commands
    //T aux switches from app   
    char *arg; 
    int Aux1;
    int Aux2;
    arg = SCmd.next(); 
    if (arg != NULL) 
    {
      Aux1=atoi(arg);
      }
      else{
      Otto.putMouth(xMouth);
      delay(2000);
       Otto.clearMouth();
      Aux1=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {Aux2=atoi(arg);}
    else{
      Otto.putMouth(xMouth);
      delay(2000);
       Otto.clearMouth();
      Aux2=0;
    }
Otto.putMouth(okMouth);
delay(750);
 Otto.clearMouth();
if (Aux1 == 1){
    AUXcontrol1 = true;
  }
  else{
    AUXcontrol1 = false;
  }
if (Aux2 == 1){
    AUXcontrol2 = true;
  }
  else{
    AUXcontrol2 = false;
  }
    sendFinalAck();
digitalWrite (OUT1, AUXcontrol1); // turn ON OFF Aux 1 output
digitalWrite (OUT2, AUXcontrol2); // turn ON OFF Aux 2 output
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
  Serial.print(F("&&"));
  Serial.print(F("D "));
  Serial.print(distance);
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
