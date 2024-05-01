// If the macro REMOTEXY__DEBUGLOG is defined, this header file defines the
// CRemoteXYDebugLog class, which provides a simple and configurable debug
// logging functionality for an embedded system using the Arduino platform.
#ifndef _REMOTEXY_DEBUGLOG_H_
#define _REMOTEXY_DEBUGLOG_H_

// If REMOTEXY__DEBUGLOG is defined, include the necessary headers for the
// HardwareSerial class and other standard libraries.
#if defined(REMOTEXY__DEBUGLOG) 

#include <inttypes.h>  // For fixed-width integer types
#include <stdlib.h>    // For dynamic memory management
#include <Arduino.h>  // For the HardwareSerial class

// Define default values for the serial port and baud rate if they are not
// explicitly set by the user.
#ifndef REMOTEXY__DEBUGLOG_SERIAL
#define REMOTEXY__DEBUGLOG_SERIAL Serial
#endif

#ifndef REMOTEXY__DEBUGLOG_SPEED
#define REMOTEXY__DEBUGLOG_SPEED 115200
#endif

// Define the CRemoteXYDebugLog class.
class CRemoteXYDebugLog {
  public:
    // The serial port to log messages to. It can be explicitly set during
    // object construction or use the default value of REMOTEXY__DEBUGLOG_SERIAL.
    HardwareSerial * serial;

    // The baud rate of the serial port. It can be explicitly set during
    // object construction or use the default value of REMOTEXY__DEBUGLOG_SPEED.
    long speed;

    // A flag indicating whether logging is enabled or disabled.
    uint8_t enabled;

  public:
    // The constructor initializes the object with the given serial port and
    // baud rate. If the serial port is not explicitly set, it defaults to
    // REMOTEXY__DEBUGLOG_SERIAL. If the baud rate is not explicitly set, it
    // defaults to REMOTEXY__DEBUGLOG_SPEED. The enabled flag is set to 1,
    // indicating that logging is enabled.
    CRemoteXYDebugLog (HardwareSerial *_serial = nullptr, long _speed = REMOTEXY__DEBUGLOG_SPEED) :
      serial(_serial ? _serial : &REMOTEXY__DEBUGLOG_SERIAL),
      speed(_speed),
      enabled(1)
    {
      // If the serial port is not explicitly set, begin serial communication
      // at the specified baud rate.
      if (_serial == nullptr) {
        serial->begin(speed);
      }
    }

    // The destructor ends serial communication and frees any resources used by
    // the object.
    ~CRemoteXYDebugLog() {
      if (serial && enabled) {
        serial->flush();
        serial->end();
      }
    }

    // The init() method initializes the serial port with the specified baud
    // rate and enables logging.
    void init() {
      if (serial && !enabled) {
        serial->begin(speed);
        enabled = 1;
      }
    }

    // The setEnabled() method sets the enabled flag to the given value,
    // enabling or disabling logging as appropriate.
    void setEnabled(uint8_t value) {
      enabled = value;
      if (!enabled && serial) {
        serial->flush();
        serial->end();
      }
    }

    // The setSpeed() method sets the baud rate of the serial port to the
    // given value.
    void setSpeed(long baud) {
      if (serial && enabled) {
        serial->end();
        speed = baud;
        serial->begin(speed);
      }
    }

    // The writeTime() method writes the current time in milliseconds to the
    // serial port, prefixed by a timestamp string.
    void writeTime() {
      uint32_t d = millis();
      long ds = d / 1000;
      long dm = d % 1000;
      char s[15];
      sprintf(s, "[%5ld.%03ld] ", ds, dm);
      if (enabled) {
        serial->println();
        serial->print(s);
      }
    }

    // The write() method writes the given string to the serial port,
    // prefixed by a timestamp string.
    void write(const char *s) {
      if (enabled) {
        writeTime();
        serial->print(s);
      }
    }

    // The writeAdd() method writes the given value to the serial port,
    // appending it to the current output buffer.
    void writeAdd(const char *s) {
      if (enabled) {
        serial->print(s);
      }
    }

    // The writeAdd() method writes the given integer value to the serial
    // port, appending it to the current output buffer.
    void writeAdd(uint16_t i) {
      if (enabled) {
        serial->print(i);
      }
    }

    // The writeAdd() method writes the given integer value to the serial
    // port, appending it to the current output buffer.
    void writeAdd(uint32_t i) {
      if (enabled) {
        serial->print(i);
      }
    }

    // The writeAdd() method writes the given integer value to the serial
    // port, appending it to the current output buffer.
    void writeAdd(int i) {
      if (enabled) {
        serial->print(i);
      }
    }

    // The writeAdd() method writes the given integer value to the serial
    // port, appending it to the current output buffer.
    void writeAdd(long i) {
      if (enabled) {
        serial->print(i);
      }
    }

    // The writeLine() method writes the given string to the serial port,
    // prefixed by a timestamp string and followed by a newline character.
    void writeLine(const char *s) {
      if (enabled) {
        writeTime();
        serial->println(s);
      }
    }

    // The writeInput() method writes the given string to the serial port,
    // prefixed by a timestamp string, a left arrow, and a space.
    void writeInput(const char *s) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<- ");
      }
      debug_flags = 0x01;
      serial->print(s);
    }

    // The writeOutput() method writes the given string to the serial port,
    // prefixed by a timestamp string, a right arrow, and a space.
    void writeOutput(const char *s) {
      if ((debug_flags & 0x02) == 0 && enabled) {
        writeTime();
        serial->print("-> ");
      }
      debug_flags = 0x02;
      serial->print(s);
    }

    // The writeInputHex() method writes the given byte value to the serial
    // port in hexadecimal format, prefixed by a timestamp string, a left
    // arrow, and a space.
    void writeInputHex(uint8_t b) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<-");
        debug_hexcounter = 0;
      }
      debug_flags = 0x01;
      writeHex(b);
    }

    // The writeOutputHex() method writes the given byte value to the serial
    // port in hexadecimal format, prefixed by a timestamp string, a right
    // arrow, and a space.
    void writeOutputHex(uint8_t b) {
      if ((debug_flags & 0x02) == 0 && enabled) {
        writeTime();
        serial->print("->");
        debug_hexcounter = 0;
      }
      debug_flags = 0x02;
      writeHex(b);
    }

    // The writeInputChar() method writes the given character to the serial
    // port, prefixed by a timestamp string, a left arrow, and a space.
    void writeInputChar(char s) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<- ");
      }
      debug_flags = 0x01;
      serial->print(s);
    }

    // The writeInputNewString() method resets the debug_flags variable to
    // 0, indicating that the current input message has ended.
    void writeInputNewString() {
      debug_flags = 0;
    }

    // The writeHex() method writes the given byte value to the serial port in
    // hexadecimal format, with a space prefix and a space separator every 16
    // bytes.
    void writeHex(uint8_t b) {
      debug_hexcounter++;
      if (debug_hexcounter > 16) {
        serial->println();
        serial->print("              ");
        debug_hexcounter = 1;
      }
      serial->print(' ');
      serial->print(b >> 4, HEX);
      serial->print(b & 0x0f, HEX);
    }

    // The writeDec() method writes the given byte value to the serial port in
    // decimal format.
    void writeDec(uint8_t b) {
      if (enabled) {
        writeTime();
        serial->print(b);
      }
    }

    // The writeBinary() method writes the given byte value to the serial
    // port in binary format.
    void writeBinary(uint8_t b) {
      if (enabled) {
        writeTime();
        serial->print(b, BIN);
      }
    }

    // The writeFloat() method writes the given floating-point value to the
    // serial port.
    void writeFloat(float f) {
      if (enabled) {
        writeTime();
        serial->print(f);
      }
    }

    // The writeDouble() method writes the given double-precision floating-
    // point value to the serial port.
    void writeDouble(double d) {
      if (enabled) {
        writeTime();
        serial->print(d);
      }
    }

    // The writeBool() method writes the given boolean value to the serial
    // port as either "true" or "false".
    void writeBool(bool b) {
      if (enabled) {
        writeTime();
        serial->print(b ? "true" : "false");
      }
    }

    // The writeChar() method writes the given character to the serial port.
    void writeChar(char c) {
      if (enabled) {
        writeTime();
        serial->print(c);
      }
    }

    // The writeNewline() method writes a newline character to the serial
    // port.
    void writeNewline() {
      if (enabled) {
        serial->println();
      }
    }

    // The writeSpace() method writes a space character to the serial port.
    void writeSpace() {
      if (enabled) {
        serial->print(' ');
      }
    }

    // The writeTab() method writes a tab character to the serial port.
    void writeTab() {
      if (enabled) {
        serial->print('\t');
      }
    }

    // The flush() method flushes the serial port buffer.
    void flush() {
      if (enabled) {
        serial->flush();
      }
    }

  private:
    // The debug_flags variable is used to keep track of the current input
    // or output message being written to the serial port.
    uint8_t debug_flags;


