//----------------------------------------------------------------
//-- Otto Easy calibration
//-- With this code you can calibrate the servo motors butit will require to adjust the TRIM values
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
//-- September 2019: Added Interactive interface
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/                                                    
//-- VERY IMPORTANT only calibrate ONE TIME per board!to avoid damage EEPROM memory
//-----------------------------------------------------------------
#include <Otto.h>
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
Otto Otto;

int positions[] = {90, 90, 90, 90};

uint8_t servo = 0;
int8_t trims[4] = {0,0,0,0};

// Set all 4 trim values

void setTrims(void)
{
  Otto.setTrims(trims[0], trims[1], trims[2], trims[3]);
  Serial.print("Setting trim [");
  for( int x = 0; x < 4 ; x++)
  {
    Serial.print(trims[x]);
    Serial.print(", ");
  }
  Serial.println("]");
}

// printout the help screen

void help(void)
{
  Serial.println("This is used to set the trim on the servos for 'Otto'");
  Serial.println("The interface uses single character controls");
  Serial.println("");
  Serial.println("'e' - read EEPROM trims and use them");
  Serial.println("'h' - this message");
  Serial.println("'+' - add to the trim");
  Serial.println("'-' - subtract from the trim");
  Serial.println("'L' - trim the Left Leg");
  Serial.println("'l' - trim the Left Foot");
  Serial.println("'R' - trim the Right Leg");
  Serial.println("'r' - trim the Right Foot");
  Serial.println("'w' - write the current trims to  EEPROM");
  Serial.println("");
}

// process the user input char

void processChar(char c)
{
  // see if the first char is a number or a command char

  switch(c) {
    case 'h':
    case 'H':
      help();
      break;
    case 'L':
      // set the servo to left leg
      Serial.println("Selected Left Leg");
      servo = 0;
      break;
    case 'l':
      Serial.println("Selected Left Foot");
      servo = 2;
      break;
    case 'R':
      Serial.println("Selected Right Leg");
      servo = 1;
      break;
    case 'r':
      Serial.println("Selected Right Foot");
      servo = 3;
      break;
    case '+':
      trims[servo]++;
      setTrims();
      break;
    case '-':
      trims[servo]--;
      setTrims();
      break;
    case 'e':
      for (int i = 0; i < 4; i++)
      {
        int servo_trim = EEPROM.read(i);
        
        if (servo_trim > 128)
          servo_trim -= 256;

        trims[i] = servo_trim;
      }
      setTrims();
      break;
    case 'w':
    case 'W':
      // write the values out to EEPROM
      Otto.saveTrimsOnEEPROM();
      Serial.println("Values written to EEPROM");
      break;
    case '\n':
    case '\r':
      break;
    default:
      Serial.print("Unknown command (");
      Serial.print(c);
      Serial.println(")");
      break;
  }
  Otto._moveServos(10, positions);
}

// init all things

void setup(void)
{
  // Init Serial port

  Serial.begin(57600);
  
  // init Otto
  
  Otto.init(2, 3, 4, 5, false);

  help();

  // read the EEPROM and use those values

  for (int i = 0; i < 4; i++)
  {
    int servo_trim = EEPROM.read(i);
    
    if (servo_trim > 128)
      servo_trim -= 256;

    trims[i] = servo_trim;
  }

  Serial.println("Getting EEPROM Trim values");
  // set the trims

  setTrims();

  Otto._moveServos(10, positions);
}

// see if the user wants anything

void loop()  // test comparing before & after function
{
  Otto.home();

  // get input from Serial port

  while(Serial.available() > 0)
  {
    processChar(Serial.read());
  }
  
 // delay(4000);
 // Otto.updown(6, 500, BIG);   
 // delay(2000);
 //   
 // Otto.walk(4,1800);
 //delay(2000);
}
