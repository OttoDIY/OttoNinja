/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 * Adapted for OTTO version 9 use
 */

#ifndef _Otto_Matrix9_H_
#define _Otto_Matrix9_H_

#include "Arduino.h"
#include "Otto_gestures9.h"

#if defined(ESP32)
# include <SPI.h>
#endif

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

class Otto_Matrix
{
  private:
    byte data;
    byte load;
    byte clock;
    byte num;
    byte buffer[8];
    byte CHARbuffer[80];
    int  rotation;
    void reload();
    char rotation2;
    
  public:
    Otto_Matrix();
    
    void init(byte data, byte load, byte clock, byte num, int rotation);
    void clearMatrix();
    void setCommand(byte command, byte value);
    void setIntensity(byte intensity);
    void setColumn(byte col, byte value);
    void setColumnAll(byte col, byte value);
    void setDot(byte col, byte row, byte value);
    void writeFull(unsigned long value);
    void sendChar ( const byte data, byte pos, byte number, byte scrollspeed);
};

#endif
