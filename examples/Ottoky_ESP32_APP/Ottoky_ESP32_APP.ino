//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY APP Firmware Version 11 (V11) with standard baudrate of 9600 for Bluetooth BLE modules.
//-- Designed to work with the starter Otto or PLUS or other biped robots.
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//-------------------------------------------------------------------------
// 2021/06  Nca78 - Adapted from Otto App sketch to run on Ottoky with either ESP32 bluetooth or bluetooth module
#include <Oscillator.h>
#include <EEPROM.h>
#include <SerialCommand.h>
#include <Otto.h>
Otto Otto;

// Uncomment the #define below to use internal bluetooth function of the ESP32
//  Bluetooth name will be "OTTOKY"
// /!\ WARNING this is only compatible with Android and not iOS devices, iOS compatible version will comme later...
// If line is uncommented then the sonar is replaced with bluetooth module, RX must be connected on IN2 and TX on IN3
//#define USE_INTERNAL_BT 1

#define N_SERVOS 4
#define PIN_YL 23 //Left leg  (OUT 1)
#define PIN_YR 5 //Right leg  (OUT 4)
#define PIN_RL 26 //Left foot (OUT 2)
#define PIN_RR 27 //Right foot (OUT 5)
#define PIN_Buzzer  25  //BUZZER pin (OUT 3)
#define PIN_NoiseSensor 34 // IN 1
#define PIN_Button   2   // BTN B
#define PIN_Trigger  16   // Sonar Trigger
#define PIN_Echo     17   // Sonar Echo

#ifndef USE_INTERNAL_BT
  // no internal bluetooth, we use BT module on the IN2/IN3 connectors
  #define PIN_BT_TX 33 // IN 2
  #define PIN_BT_RX 32 // IN 3
  SoftwareSerial BTserial(PIN_BT_RX,PIN_BT_TX);
#else
  #include "BluetoothSerial.h"
  BluetoothSerial BTserial;

#endif
SerialCommand SCmd(BTserial);

const char programID[] = "Ottoky_APP";
const char name_fac = '$';
const char name_fir = '#';
int T = 1000;
int moveId = 0;
int moveSize = 15;
volatile bool buttonPushed=false;

int randomDance = 0;
int randomSteps = 0;
bool obstacleDetected = false;
unsigned long int matrix;
void receiveStop() 
{ sendAck(); Otto.home(); sendFinalAck(); }
void receiveLED()
{ sendAck(); Otto.home(); unsigned long int matrix; char *arg; char *endstr; arg = SCmd.next(); if (arg != NULL) { matrix = strtoul(arg, &endstr, 2); Otto.putMouth(matrix, false); } else { Otto.putMouth(xMouth); delay(2000); Otto.clearMouth(); } sendFinalAck(); }
void recieveBuzzer() 
{ sendAck(); Otto.home(); bool error = false; int frec; int duration; char *arg; arg = SCmd.next(); if (arg != NULL) frec = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) duration = atoi(arg); else error = true; if (error == true) { Otto.putMouth(xMouth); delay(2000); Otto.clearMouth(); } else Otto._tone(frec, duration, 1); sendFinalAck(); }
void receiveTrims() 
{ sendAck(); Otto.home(); int trim_YL, trim_YR, trim_RL, trim_RR; bool error = false; char *arg; arg = SCmd.next(); if (arg != NULL) trim_YL = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) trim_YR = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) trim_RL = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) trim_RR = atoi(arg); else error = true; if (error == true) { Otto.putMouth(xMouth); delay(2000); Otto.clearMouth(); } else { Otto.setTrims(trim_YL, trim_YR, trim_RL, trim_RR); Otto.saveTrimsOnEEPROM(); } sendFinalAck(); }
void receiveServo() 
{ sendAck(); moveId = 30; bool error = false; char *arg; int servo_YL, servo_YR, servo_RL, servo_RR; arg = SCmd.next(); if (arg != NULL) servo_YL = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) servo_YR = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) servo_RL = atoi(arg); else error = true; arg = SCmd.next(); if (arg != NULL) { servo_RR = atoi(arg); } else error = true; if (error == true) { Otto.putMouth(xMouth); delay(2000); Otto.clearMouth(); } else { int servoPos[4] = {servo_YL, servo_YR, servo_RL, servo_RR}; Otto._moveServos(200, servoPos); } sendFinalAck(); }
void receiveMovement() 
{ sendAck(); if (Otto.getRestState() == true) Otto.setRestState(false); char *arg; arg = SCmd.next(); if (arg != NULL) moveId = atoi(arg); else { Otto.putMouth(xMouth); delay(2000); Otto.clearMouth(); moveId = 0; } arg = SCmd.next(); if (arg != NULL) T = atoi(arg); else T = 1000; arg = SCmd.next(); if (arg != NULL) moveSize = atoi(arg); else moveSize = 15; }
void move(int moveId) 
{ bool manualMode = false; switch (moveId) { case 0: Otto.home(); break; case 1: Otto.walk(1, T, 1); break; case 2: Otto.walk(1, T, -1); break; case 3: Otto.turn(1, T, 1); break; case 4: Otto.turn(1, T, -1); break; case 5: Otto.updown(1, T, moveSize); break; case 6: Otto.moonwalker(1, T, moveSize, 1); break; case 7: Otto.moonwalker(1, T, moveSize, -1); break; case 8: Otto.swing(1, T, moveSize); break; case 9: Otto.crusaito(1, T, moveSize, 1); break; case 10: Otto.crusaito(1, T, moveSize, -1); break; case 11: Otto.jump(1, T); break; case 12: Otto.flapping(1, T, moveSize, 1); break; case 13: Otto.flapping(1, T, moveSize, -1); break; case 14: Otto.tiptoeSwing(1, T, moveSize); break; case 15: Otto.bend(1, T, 1); break; case 16: Otto.bend(1, T, -1); break; case 17: Otto.shakeLeg(1, T, 1); break; case 18: Otto.shakeLeg(1, T, -1); break; case 19: Otto.jitter(1, T, moveSize); break; case 20: Otto.ascendingTurn(1, T, moveSize); break; default: manualMode = true; break; } if (!manualMode) sendFinalAck(); }
void receiveGesture()
{ sendAck(); Otto.home();  int gesture = 0; char *arg; arg = SCmd.next(); if (arg != NULL) gesture = atoi(arg); else     delay(2000); switch (gesture) { case 1: Otto.playGesture(OttoHappy); break; case 2: Otto.playGesture(OttoSuperHappy); break; case 3: Otto.playGesture(OttoSad); break; case 4: Otto.playGesture(OttoSleeping); break; case 5: Otto.playGesture(OttoFart); break; case 6: Otto.playGesture(OttoConfused); break; case 7: Otto.playGesture(OttoLove); break; case 8: Otto.playGesture(OttoAngry); break; case 9: Otto.playGesture(OttoFretful); break; case 10: Otto.playGesture(OttoMagic); break; case 11: Otto.playGesture(OttoWave); break; case 12: Otto.playGesture(OttoVictory); break; case 13: Otto.playGesture(OttoFail); break; default: break; } sendFinalAck(); }
void receiveSing() 
{ sendAck(); Otto.home(); int sing = 0; char *arg; arg = SCmd.next(); if (arg != NULL) sing = atoi(arg); else     delay(2000); switch (sing) { case 1: Otto.sing(S_connection); break; case 2: Otto.sing(S_disconnection); break; case 3: Otto.sing(S_surprise); break; case 4: Otto.sing(S_OhOoh); break; case 5: Otto.sing(S_OhOoh2); break; case 6: Otto.sing(S_cuddly); break; case 7: Otto.sing(S_sleeping); break; case 8: Otto.sing(S_happy); break; case 9: Otto.sing(S_superHappy); break; case 10: Otto.sing(S_happy_short); break; case 11: Otto.sing(S_sad); break; case 12: Otto.sing(S_confused); break; case 13: Otto.sing(S_fart1); break; case 14: Otto.sing(S_fart2); break; case 15: Otto.sing(S_fart3); break; case 16: Otto.sing(S_mode1); break; case 17: Otto.sing(S_mode2); break; case 18: Otto.sing(S_mode3); break; case 19: Otto.sing(S_buttonPushed); break; default: break; } sendFinalAck(); }
void receiveName() 
{ sendAck(); Otto.home(); char newOttoName[11] = ""; int eeAddress = 5; char *arg; arg = SCmd.next(); if (arg != NULL) { int k = 0; while ((*arg) && (k < 11)) { newOttoName[k] = *arg++; k++; } EEPROM.put(eeAddress, newOttoName); } else { delay(2000); } sendFinalAck(); }
void requestName() 
{ Otto.home(); char actualOttoName[11] = ""; int eeAddress = 5; EEPROM.get(eeAddress, actualOttoName); Serial.print(F("&&")); Serial.print(F("E ")); Serial.print(actualOttoName); Serial.println(F("%%")); Serial.flush(); }
void requestProgramId() 
{ Otto.home(); Serial.print(F("&&")); Serial.print(F("I ")); Serial.print(programID); Serial.println(F("%%")); Serial.flush(); }
void sendAck() 
{ delay(30); Serial.print(F("&&")); Serial.print(F("A")); Serial.println(F("%%")); Serial.flush(); }
void sendFinalAck() 
{ delay(30); Serial.print(F("&&")); Serial.print(F("F")); Serial.println(F("%%")); Serial.flush(); }
void ButtonPushed()
{ if(!buttonPushed){ buttonPushed=true; Otto.putMouth(smallSurprise); } }

void setup() {
  Serial.begin(9600);
#ifndef USE_INTERNAL_BT
  BTserial.begin(9600);
#else
  BTserial.begin("OTTOKY"); //Bluetooth device name using internal bluetooth of ESP32
#endif

Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Buzzer);
// not available on Ottoky // Otto.initMATRIX(DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);
// not available on Ottoky // Otto.matrixIntensity(7);
randomSeed(analogRead(PIN_NoiseSensor));
// not available on Ottoky // pinMode(PIN_ASSEMBLY, INPUT_PULLUP);
pinMode(PIN_Button, INPUT);

SCmd.addCommand("S", receiveStop);
SCmd.addCommand("L", receiveLED);
SCmd.addCommand("T", recieveBuzzer);
SCmd.addCommand("M", receiveMovement);
SCmd.addCommand("H", receiveGesture);
SCmd.addCommand("K", receiveSing);
SCmd.addCommand("C", receiveTrims);
SCmd.addCommand("G", receiveServo);
SCmd.addCommand("R", receiveName);
SCmd.addCommand("E", requestName);
SCmd.addCommand("I", requestProgramId);
SCmd.addDefaultHandler(receiveStop);

Otto.sing(S_connection);
Otto.home();
for (int i = 0; i < 2; i++) {
  for (int i = 0; i < 8; i++) {
  Otto.putAnimationMouth(littleUuh, i);
  delay(150);
  }
}
Otto.putMouth(smile);
Otto.sing(S_happy);
delay(200);
if (EEPROM.read(5) == name_fir) {
  Otto.jump(1, 700);
  delay(200);
  Otto.shakeLeg(1, T, 1);
  Otto.putMouth(smallSurprise);
  Otto.swing(2, 800, 20);
  Otto.home();
}
Otto.putMouth(happyOpen);

/* not available on Ottoky // 
while (digitalRead(PIN_ASSEMBLY) == LOW) {
  Otto.home();
  Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
delay(5000);}
 not available on Ottoky */ 
 
}


void loop() {
  SCmd.readSerial();     if (Otto.getRestState()==false){ move(moveId); }
}
