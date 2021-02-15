/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 * Adapted for OTTO version 9 use
 */


#include "Arduino.h"
#include "Otto_Matrix9.h"

Otto_Matrix::Otto_Matrix() 
{
	//data = _data;
	//load = _load;
	//clock = _clock;
	//num = _num;
}

void Otto_Matrix::init(byte _data, byte _load, byte _clock, byte _num, int _rotation)
{
  data = _data;
  load = _load;
  clock = _clock;
  num = _num;
  rotation = _rotation;
  if ((rotation > 4) || (rotation == 0)) rotation = 1; // we have to have number between 1 and 4
  for (int i=0; i<8; i++)
    buffer[i] = 0;

    for (int i=0; i<80; i++)
    CHARbuffer[i] = 0;
    
#if defined(ESP32)
    SPI.begin ( clock,  -1,  data, load);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setHwCs(true);
#else
    pinMode(data,  OUTPUT);
    pinMode(clock, OUTPUT);
    pinMode(load, OUTPUT);
    digitalWrite(clock, HIGH); 
#endif

        setCommand(max7219_reg_scanLimit, 0x07);      
	setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setCommand(max7219_reg_displayTest, 0x00); // no display test
	
	// empty registers, turn all LEDs off
	clearMatrix();
	
	setIntensity(0x0f);    // the first 0x0f is the value you can set
}

void Otto_Matrix::setIntensity(byte intensity)
{
	setCommand(max7219_reg_intensity, intensity);
}

void Otto_Matrix::clearMatrix()
{
	for (int i=0; i<8; i++) 
		setColumnAll(i,0);
		
	for (int i=0; i<8; i++)
		buffer[i] = 0;

    for (int i=0; i<80; i++)
    CHARbuffer[i] = 0;
}

void Otto_Matrix::setCommand(byte command, byte value)
{
#if defined(ESP32)
    SPI.transfer16(command << 8 | value);
#else
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		shiftOut(data, clock, MSBFIRST, command);
		shiftOut(data, clock, MSBFIRST, value);
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
#endif
}


void Otto_Matrix::setColumn(byte col, byte value)
{
	int n = col / 8;
	int c = col % 8;
#if defined(ESP32)
    for (int i=0; i<num; i++)
    {
	if (i == n)
        {
	    SPI.transfer16((c + 1) << 8 | value);
	}
	//else
	//{
	//    SPI.transfer16(0);
	//}
    }
#else
	digitalWrite(load, LOW);    
        for (int i=0; i<num; i++) 
	{
		if (i == n)
		{
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, value);
		}
		//else
		//{
			//shiftOut(data, clock, MSBFIRST, 0);
			//shiftOut(data, clock, MSBFIRST, 0);
		//}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
#endif
	
	buffer[col] = value;
}

void Otto_Matrix::setColumnAll(byte col, byte value)
{
#if defined(ESP32)
    for (int i=0; i<num; i++) 
    {
	SPI.transfer16((col + 1) << 8 | value);
	buffer[col * i] = value;
    }
#else
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		shiftOut(data, clock, MSBFIRST, col + 1);
		shiftOut(data, clock, MSBFIRST, value);
		buffer[col * i] = value;
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
#endif
}

void Otto_Matrix::setDot(byte col, byte row, byte value)
{
    bitWrite(buffer[col], row, value);

	int n = col / 8;
	int c = col % 8;
#if defined(ESP32)
    for (int i=0; i<num; i++) 
    {
	if (i == n)
	{
	    SPI.transfer16((c + 1) << 8 | buffer[col]);
	}
	else
	{
	    SPI.transfer16(0);
	}
    }
#else
	digitalWrite(load, LOW);    
        for (int i=0; i<num; i++) 
	{
		if (i == n)
		{
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, buffer[col]);
		}
		else
		{
			shiftOut(data, clock, MSBFIRST, 0);
			shiftOut(data, clock, MSBFIRST, 0);
		}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
#endif
}

// routine for OTTO and ZOWI, for the 6 x 5 matrix
void Otto_Matrix::writeFull(unsigned long value) {
  if (rotation == 1) {
	for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                setDot(6-c,7-r,(1L & (value >> r*6+c)));
                }
            }
  }
if (rotation == 2) {
  for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(1+c,r,    (1L & (value >> r*6+c)));
                }
            }
  }
  if (rotation == 3) {
  for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(r,6-c,    (1L & (value >> r*6+c)));
                }
            }
  }
  if (rotation == 4) {
  for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(7-r,1+c, (1L & (value >> r*6+c)));  
                }
            }
  }
}

void Otto_Matrix::sendChar (const byte data, byte pos, byte number, byte scrollspeed){
  if (scrollspeed < 50 ) scrollspeed = 50;
   if (scrollspeed > 150 ) scrollspeed = 150;
  int charPos;
charPos = pos * 8;
//Serial.print ("sendchar  ");
//Serial.print (pos);
//Serial.print (" -  ");
//Serial.print (number);
//Serial.print (" -  ");
//Serial.print (charPos);
//Serial.print (" -  ");
//Serial.println (data);
//we need to add 8 for each character
  CHARbuffer[0 + charPos] = 0;
  CHARbuffer[1 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[0]);
  CHARbuffer[2 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[1]);
  CHARbuffer[3 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[2]);
  CHARbuffer[4 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[3]);
  CHARbuffer[5 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[4]);
  CHARbuffer[6 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[5]);
  CHARbuffer[7 + charPos] = 0;

 if (number == (pos + 1)){ // last character so display the total text
// we need to display first character and scroll left until each charater is shown.
    for (int c=0; c<8;c++){ // show first character
         byte value = CHARbuffer[c];
            for (int r=0; r<8; r++){
              if (rotation == 1) {
                setDot(c,7-r,(0b00000001 & (value >> r)));//       
                }
                if (rotation == 2) {
                 setDot(7-c,r,(0b00000001 & (value >> r)));//  
                }
                if (rotation == 3) {
                 //setDot(r,c,(1));// top LH corner
                 setDot(r,c,(0b00000001 & (value >> r)));//  
                }
                if (rotation == 4) {
                 setDot(7-r,7-c,(0b00000001 & (value >> r)));//  
                }
           }
      }
      delay(500); // show first digit for longer
      for (int i=0; i<((number*8)-1); i++){   // shift buffer the correct number of characters (8 lines per character)
        CHARbuffer[i] = CHARbuffer[i+1];
         for (int c=0; c<8;c++){ // 
             byte value = CHARbuffer[(1+c)+i];
                for (int r=0; r<8; r++){
                  if (rotation == 1) {
                  setDot(c,7-r,(0b00000001 & (value >> r)));//       
                   }
                  if (rotation == 2) {
                  setDot(7-c,r,(0b00000001 & (value >> r)));//  
                  }
                  if (rotation == 3) {
                   setDot(r,c,(0b00000001 & (value >> r)));//  
                 }
                  if (rotation == 4) {
                 setDot(7-r,7-c,(0b00000001 & (value >> r)));//  
                }
              }
        }
     delay(scrollspeed);// this sets the scroll speed
  
      }
      clearMatrix();
 }   
}


