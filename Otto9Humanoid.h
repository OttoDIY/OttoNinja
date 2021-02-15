//----------------------------------------------------------------
//-- OTTO version 9 
//-----------------------------------------------------------------
#ifndef Otto9Humanoid_h
#define Otto9Humanoid_h

#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>

#include "BatReader9.h"
#include "Otto_Matrix9.h"
#include "Otto_mouth9.h"
#include "Otto_sound9.h"
#include "Otto_gestures9.h"
#include <TimerFreeTone.h>

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30




class Otto9Humanoid
{
  public:

    //-- Otto initialization
    void init(int YL, int YR, int RL, int RR, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho);
    void initHUMANOID(int YL, int YR, int RL, int RR,int LA, int RA, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho);
    void initDC(int NoiseSensor, int Buzzer, int USTrigger, int USEcho);
    //-- Attach & detach functions
    void attachServos();
    void detachServos();

    //-- Oscillator Trims
    void setTrims(int YL, int YR, int RL, int RR,int LA, int RA);
    void saveTrimsOnEEPROM();

    //-- Predetermined Motion Functions
    void _moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[6], int O[6], int T, double phase_diff[6], float cycle);

    //-- HOME = Otto at rest position
    void home();
    bool getRestState();
    void setRestState(bool state);
    
    //-- Predetermined Motion Functions
    void jump(float steps=1, int T = 2000);

    void walk(float steps=4, int T=1000, int dir = FORWARD);
    void turn(float steps=4, int T=2000, int dir = LEFT);
    void bend (int steps=1, int T=1400, int dir=LEFT);
    void shakeLeg (int steps=1, int T = 2000, int dir=RIGHT);

    void updown(float steps=1, int T=1000, int h = 20);
    void swing(float steps=1, int T=1000, int h=20);
    void tiptoeSwing(float steps=1, int T=900, int h=20);
    void jitter(float steps=1, int T=500, int h=20);
    void ascendingTurn(float steps=1, int T=900, int h=20);

    void moonwalker(float steps=1, int T=900, int h=20, int dir=LEFT);
    void crusaito(float steps=1, int T=900, int h=20, int dir=FORWARD);
    void flapping(float steps=1, int T=1000, int h=20, int dir=FORWARD);
    void handsup();
    void handwave(int dir =RIGHT);

    //-- Sensors functions
    float getDistance(); //US sensor
    int getNoise();      //Noise Sensor

    //-- Battery
    double getBatteryLevel();
    double getBatteryVoltage();
    
    //-- Mouth & Animations
    void putMouth(unsigned long int mouth, bool predefined = true);
    void putAnimationMouth(unsigned long int anim, int index);
    void clearMouth();

    //-- Sounds
    void _tone (float noteFrequency, long noteDuration, int silentDuration);
    void bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(int songName);

    //-- Gestures
    void playGesture(int gesture);
    void initMATRIX(int DIN, int CS, int CLK, int rotate);
    void matrixIntensity(int intensity);
    void initBatLevel(int batteryPIN);
    void setLed(byte X, byte Y, byte value);
    void writeText (const char * s, byte scrollspeed);
  private:
   
    BatReader9 battery;
    Oscillator servo[6];
    US us;
    Otto_Matrix ledmatrix;
    int servo_pins[6];
    int servo_trim[6];
    int servo_position[6];

    int pinBuzzer;
    int pinNoiseSensor;
    
    unsigned long final_time;
    unsigned long partial_time;
    float increment[6];

    bool isOttoResting;
    bool isHUMANOID;

    unsigned long int getMouthShape(int number);
    unsigned long int getAnimShape(int anim, int index);
    void _execute(int A[6], int O[6], int T, double phase_diff[6], float steps);

};

#endif


