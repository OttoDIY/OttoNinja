/*!
 * @file Adafruit_LEDBackpack.h
 *
 * Part of Adafruit's Arduino library for our I2C LED Backpacks:
 * ----> http://www.adafruit.com/products/
 * ----> http://www.adafruit.com/products/
 *
 * These displays use I2C to communicate, 2 pins are required to
 * interface. There are multiple selectable I2C addresses. For backpacks
 * with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
 * with 3 Address Select pins: 0x70 thru 0x77
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * MIT license, all text above must be included in any redistribution
 */

#ifndef Adafruit_LEDBackpack_h
#define Adafruit_LEDBackpack_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "Adafruit_GFX.h"

#define LED_ON 1  ///< GFX color of lit LED segments (single-color displays)
#define LED_OFF 0 ///< GFX color of unlit LED segments (single-color displays)

#define LED_RED 1    ///< GFX color for red LED segments (bi-color displays)
#define LED_YELLOW 2 ///< GFX color for yellow LED segments (bi-color displays)
#define LED_GREEN 3  ///< GFX color for green LED segments (bi-color displays)

#define HT16K33_BLINK_CMD 0x80       ///< I2C register for BLINK setting
#define HT16K33_BLINK_DISPLAYON 0x01 ///< I2C value for steady on
#define HT16K33_BLINK_OFF 0          ///< I2C value for steady off
#define HT16K33_BLINK_2HZ 1          ///< I2C value for 2 Hz blink
#define HT16K33_BLINK_1HZ 2          ///< I2C value for 1 Hz blink
#define HT16K33_BLINK_HALFHZ 3       ///< I2C value for 0.5 Hz blink

#define HT16K33_CMD_BRIGHTNESS 0xE0 ///< I2C register for BRIGHTNESS setting

#define SEVENSEG_DIGITS 5 ///< # Digits in 7-seg displays, plus NUL end

/*!
    @brief  Class encapsulating the raw HT16K33 controller device.
*/
class Adafruit_LEDBackpack {
public:
  /*!
    @brief  Constructor for HT16K33 devices.
  */
  Adafruit_LEDBackpack(void);

  /*!
    @brief  Start I2C and initialize display state (blink off, full
            brightness).
    @param  _addr  I2C address.
  */
  void begin(uint8_t _addr);

  /*!
    @brief  Set display brightness.
    @param  b  Brightness: 0 (min) to 15 (max).
  */
  void setBrightness(uint8_t b);

  /*!
    @brief  Set display blink rate.
    @param  b  One of:
               HT16K33_BLINK_DISPLAYON = steady on
               HT16K33_BLINK_OFF       = steady off
               HT16K33_BLINK_2HZ       = 2 Hz blink
               HT16K33_BLINK_1HZ       = 1 Hz blink
               HT16K33_BLINK_HALFHZ    = 0.5 Hz blink
  */
  void blinkRate(uint8_t b);

  /*!
    @brief  Issue buffered data in RAM to display.
  */
  void writeDisplay(void);

  /*!
    @brief  Clear display.
  */
  void clear(void);

  uint16_t displaybuffer[8]; ///< Raw display data

protected:
  uint8_t i2c_addr; ///< Device I2C address
};

/*!
    @brief  Class for 24-element bargraph displays.
*/
class Adafruit_24bargraph : public Adafruit_LEDBackpack {
public:
  /*!
    @brief  Constructor for 24-element bargraph displays.
  */
  Adafruit_24bargraph(void);

  /*!
    @brief  Set color a single bar (dot).
    @param  bar    Bar index (0 to 23).
    @param  color  Bar color: LED_OFF, LED_GREEN, LED_YELLOW or LED_RED.
  */
  void setBar(uint8_t bar, uint8_t color);
};

/*!
    @brief  Class for 8x16 pixel single-color matrices.
*/
class Adafruit_8x16matrix : public Adafruit_LEDBackpack, public Adafruit_GFX {
public:
  /*!
    @brief  Constructor for 8x16 pixel single-color matrices.
  */
  Adafruit_8x16matrix(void);

  /*!
    @brief  Lowest-level pixel drawing function required by Adafruit_GFX.
            Does not have an immediate effect -- must call writeDisplay()
            after any drawing operations to refresh display contents.
    @param  x      Pixel column (horizontal).
    @param  y      Pixel row (vertical).
    @param  color  Pixel color (1 = pixel on, 0 = pixel off).
  */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
};

/*!
    @brief  Class for 8x16 pixel single-color mini matrices.
*/
class Adafruit_8x16minimatrix : public Adafruit_LEDBackpack,
                                public Adafruit_GFX {
public:
  /*!
    @brief  Constructor for 8x16 pixel single-color mini matrices.
  */
  Adafruit_8x16minimatrix(void);

  /*!
    @brief  Lowest-level pixel drawing function required by Adafruit_GFX.
            Does not have an immediate effect -- must call writeDisplay()
            after any drawing operations to refresh display contents.
    @param  x      Pixel column (horizontal).
    @param  y      Pixel row (vertical).
    @param  color  Pixel color (1 = pixel on, 0 = pixel off).
  */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
};

/*!
    @brief  Class for 8x8 pixel single-color matrices.
*/
class Adafruit_8x8matrix : public Adafruit_LEDBackpack, public Adafruit_GFX {
public:
  /*!
    @brief  Constructor for 8x8 pixel single-color matrices.
  */
  Adafruit_8x8matrix(void);

  /*!
    @brief  Lowest-level pixel drawing function required by Adafruit_GFX.
            Does not have an immediate effect -- must call writeDisplay()
            after any drawing operations to refresh display contents.
    @param  x      Pixel column (horizontal).
    @param  y      Pixel row (vertical).
    @param  color  Pixel color (1 = pixel on, 0 = pixel off).
  */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
};

/*!
    @brief  Class for bi-color matrices.
*/
class Adafruit_BicolorMatrix : public Adafruit_LEDBackpack,
                               public Adafruit_GFX {
public:
  /*!
    @brief  Constructor for 8x8 pixel bi-color matrices.
  */
  Adafruit_BicolorMatrix(void);

  /*!
    @brief  Lowest-level pixel drawing function required by Adafruit_GFX.
            Does not have an immediate effect -- must call writeDisplay()
            after any drawing operations to refresh display contents.
    @param  x      Pixel column (horizontal).
    @param  y      Pixel row (vertical).
    @param  color  Pixel color (LED_OFF, LED_GREEN, LED_YELLOW or LED_RED).
  */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
};

#define DEC 10 ///< Print value in decimal format (base 10)
#define HEX 16 ///< Print value in hexadecimal format (base 16)
#define OCT 8  ///< Print value in octal format (base 8)
#define BIN 2  ///< Print value in binary format (base 2)
#define BYTE 0 ///< Issue 7-segment data as raw bits

/*!
    @brief  Class for 7-segment numeric displays.
*/
class Adafruit_7segment : public Adafruit_LEDBackpack {
public:
  /*!
    @brief  Constructor for 7-segment numeric displays.
  */
  Adafruit_7segment(void);

  /*!
    @brief   Issue single digit to display.
    @param   c  Digit to write (ASCII character, not numeric).
    @return  1 if character written, else 0 (non-digit characters).
  */
  size_t write(uint8_t c);

  /*!
    @brief  Print byte-size numeric value to 7-segment display.
    @param  c     Numeric value.
    @param  base  Number base (default = BYTE = raw bits)
  */
  void print(char c, int base = BYTE);

  /*!
    @brief  Print unsigned byte-size numeric value to 7-segment display.
    @param  b     Numeric value.
    @param  base  Number base (default = BYTE = raw bits)
  */
  void print(unsigned char b, int base = BYTE);

  /*!
    @brief  Print integer value to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void print(int n, int base = DEC);

  /*!
    @brief  Print unsigned integer value to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void print(unsigned int n, int base = DEC);

  /*!
    @brief  Print long integer value to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void print(long n, int base = DEC);

  /*!
    @brief  Print unsigned long integer value to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void print(unsigned long n, int base = DEC);

  /*!
    @brief  Print double-precision float value to 7-segment display.
    @param  n       Numeric value.
    @param  digits  Fractional-part digits.
  */
  void print(double n, int digits = 2);

  /*!
    @brief  Print byte-size numeric value w/newline to 7-segment display.
    @param  c     Numeric value.
    @param  base  Number base (default = BYTE = raw bits)
  */
  void println(char c, int base = BYTE);

  /*!
    @brief  Print unsigned byte-size numeric value w/newline to 7-segment
            display.
    @param  b     Numeric value.
    @param  base  Number base (default = BYTE = raw bits)
  */
  void println(unsigned char b, int base = BYTE);

  /*!
    @brief  Print integer value with newline to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void println(int n, int base = DEC);

  /*!
    @brief  Print unsigned integer value with newline to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void println(unsigned int n, int base = DEC);

  /*!
    @brief  Print long integer value with newline to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void println(long n, int base = DEC);

  /*!
    @brief  Print unsigned long integer value w/newline to 7-segment display.
    @param  n     Numeric value.
    @param  base  Number base (default = DEC = base 10)
  */
  void println(unsigned long n, int base = DEC);

  /*!
    @brief  Print double-precision float value to 7-segment display.
    @param  n       Numeric value.
    @param  digits  Fractional-part digits.
  */
  void println(double n, int digits = 2);

  /*!
    @brief  Print newline to 7-segment display (rewind position to start).
  */
  void println(void);

  /*!
    @brief  Write raw segment bits into display buffer.
    @param  x        Character position (0-3).
    @param  bitmask  Segment bits.
  */
  void writeDigitRaw(uint8_t x, uint8_t bitmask);

  /*!
    @brief  Set specific digit # to a numeric value.
    @param  x    Character position.
    @param  num  Numeric (not ASCII) value.
    @param  dot  If true, light corresponding decimal.
  */
  void writeDigitNum(uint8_t x, uint8_t num, bool dot = false);

  /*!
    @brief  Set or unset colon segment.
    @param  state  'true' to enable colon, 'false' for off.
  */
  void drawColon(bool state);

  /*!
    @brief  General integer-printing function used by some of the print()
            variants.
    @param  n     Numeric value.
    @param  base  Base (2 = binary).
  */
  void printNumber(long n, uint8_t base = 2);

  /*!
    @brief  General float-printing function used by some of the print()
            variants.
    @param  n           Numeric value.
    @param  fracDigits  Fractional-part digits.
    @param  base        Base (default DEC = base 10).
  */
  void printFloat(double n, uint8_t fracDigits = 2, uint8_t base = DEC);

  /*!
    @brief  Light display segments in an error-indicating configuration.
  */
  void printError(void);

  /*!
    @brief  Issue colon-on directly to display (bypass buffer).
  */
  void writeColon(void);

private:
  uint8_t position; ///< Current print position, 0-3
};

/*!
    @brief  Class for four-digit alphanumeric displays.
*/
class Adafruit_AlphaNum4 : public Adafruit_LEDBackpack {
public:
  /*!
    @brief  Constructor for four-digit alphanumeric displays.
  */
  Adafruit_AlphaNum4(void);

  /*!
    @brief  Write single character of alphanumeric display as raw bits
            (not a general print function).
    @param  n        Character index (0-3).
    @param  bitmask  Segment bitmask.
  */
  void writeDigitRaw(uint8_t n, uint16_t bitmask);

  /*!
    @brief  Write single ASCII character to alphanumeric display.
    @param  n      Character index (0-3).
    @param  ascii  ASCII character.
    @param  dot    If true, also light corresponding dot segment.
  */
  void writeDigitAscii(uint8_t n, uint8_t ascii, bool dot = false);
};

#endif // Adafruit_LEDBackpack_h
