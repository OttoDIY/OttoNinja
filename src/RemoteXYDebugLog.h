#ifndef _REMOTEXY_DEBUGLOG_H_
#define _REMOTEXY_DEBUGLOG_H_

#if defined(REMOTEXY__DEBUGLOG) 

#include <inttypes.h> 
#include <stdlib.h>
#include <Arduino.h>

#ifndef REMOTEXY__DEBUGLOG_SERIAL 
#define REMOTEXY__DEBUGLOG_SERIAL Serial
#endif
#ifndef REMOTEXY__DEBUGLOG_SPEED 
#define REMOTEXY__DEBUGLOG_SPEED 115200
#endif

class CRemoteXYDebugLog {
  public:
    HardwareSerial * serial;
    long speed;
    uint8_t enabled;

  public:
    CRemoteXYDebugLog (HardwareSerial *_serial = nullptr, long _speed = REMOTEXY__DEBUGLOG_SPEED) :
      serial(_serial ? _serial : &REMOTEXY__DEBUGLOG_SERIAL),
      speed(_speed),
      enabled(1)
    {
      if (_serial == nullptr) {
        serial->begin(speed);
      }
    }

    ~CRemoteXYDebugLog() {
      if (serial && enabled) {
        serial->flush();
        serial->end();
      }
    }

    void init() {
      if (serial && !enabled) {
        serial->begin(speed);
        enabled = 1;
      }
    }

    void setEnabled(uint8_t value) {
      enabled = value;
      if (!enabled && serial) {
        serial->flush();
        serial->end();
      }
    }

    void setSpeed(long baud) {
      if (serial && enabled) {
        serial->end();
        speed = baud;
        serial->begin(speed);
      }
    }

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

    void write(const char *s) {
      if (enabled) {
        writeTime();
        serial->print(s);
      }
    }

    void writeAdd(const char *s) {
      if (enabled) {
        serial->print(s);
      }
    }

    void writeAdd(uint16_t i) {
      if (enabled) {
        serial->print(i);
      }
    }

    void writeAdd(uint32_t i) {
      if (enabled) {
        serial->print(i);
      }
    }

    void writeAdd(int i) {
      if (enabled) {
        serial->print(i);
      }
    }

    void writeAdd(long i) {
      if (enabled) {
        serial->print(i);
      }
    }

    void writeLine(const char *s) {
      if (enabled) {
        writeTime();
        serial->println(s);
      }
    }

    void writeInput(const char *s) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<- ");
      }
      debug_flags = 0x01;
      serial->print(s);
    }

    void writeOutput(const char *s) {
      if ((debug_flags & 0x02) == 0 && enabled) {
        writeTime();
        serial->print("-> ");
      }
      debug_flags = 0x02;
      serial->print(s);
    }

    void writeInputHex(uint8_t b) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<-");
        debug_hexcounter = 0;
      }
      debug_flags = 0x01;
      writeHex(b);
    }

    void writeOutputHex(uint8_t b) {
      if ((debug_flags & 0x02) == 0 && enabled) {
        writeTime();
        serial->print("->");
        debug_hexcounter = 0;
      }
      debug_flags = 0x02;
      writeHex(b);
    }

    void writeInputChar(char s) {
      if ((debug_flags & 0x01) == 0 && enabled) {
        writeTime();
        serial->print("<- ");
      }
      debug_flags = 0x01;
      serial->print(s);
    }

    void writeInputNewString() {
      debug_flags = 0;
    }

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

    void writeDec(uint8_t b) {
      if (enabled) {
        writeTime();
        serial->print(b);
      }
    }

    void writeBinary(uint8_t b) {
      if (enabled) {
        writeTime();
        serial->print(b, BIN);
      }
    }

    void writeFloat(float f) {
      if (enabled) {
        writeTime();
        serial->print(f);
      }
    }

    void writeDouble(double d) {
      if (enabled) {
        writeTime();
        serial->print(d);
      }
    }

    void writeBool(bool b) {
      if (enabled) {
        writeTime();
        serial->print(b ? "true" : "false");
      }
    }

    void writeChar(char c) {
      if (enabled) {
        writeTime();
        serial->print(c);
      }
    }

    void writeNewline() {
      if (enabled) {
        serial->println();
      }
    }

    void writeSpace() {
      if (enabled) {
        serial->print(' ');
      }
    }

    void writeTab() {
      if (enabled) {
        serial->print('\t');
      }
    }

    void flush() {
      if (enabled) {
        serial->flush();
      }
    }

  private:
    uint8_t debug_flags;
    uint8_t debug_hexcounter;

    uint32_t availableMemory() {
#if defined(ESP8266) || defined(ESP32)
      return ESP.getFreeHeap();
#elif defined(__AVR__)
      uint16_t size = RAMEND;
      uint8_t *buf;
      while ((buf = (uint8_t *)malloc(size)) == NULL) size--;
      free(buf);
      return size;
#else
      return 0;
#endif
    }

};

CRemoteXYDebugLog RemoteXYDebugLog;

#endif  //REMOTEXY__DEBUGLOG

#endif //_REMOTEXY_DEBUGLOG_H_
