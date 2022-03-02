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
  uint8_t inited;
  
  private:
  uint8_t debug_flags;
  uint8_t debug_hexcounter;
  
  public:
  CRemoteXYDebugLog (HardwareSerial * _serial, long _speed) {
    debug_flags=0;
    serial = _serial;
    speed = _speed;
    inited = 0;
  }
  
  public:
  void init () {
    if (!inited) {
      serial->begin (speed);
      serial->println ();
      write ("Debug log started");
      inited = 1;
    }
  }


  public:
  void writeTime () {
    uint32_t d = millis();
    long ds = d/1000;
    long dm = d%1000;
    char s[15];
    sprintf (s, "[%5ld.%03ld] ",ds, dm);       
    serial->println ();    
    serial->print (s);
  }
  
  
  public:
  void write (const char *s) {
    debug_flags = 0;
    writeTime (); 
    serial->print(s);
  }

  public:
  void writeAdd (const char *s) {
    serial->print(s);
  }


  public:
  void writeAdd (uint16_t i) {
    serial->print(i);
  }

  public:
  void writeAdd (uint32_t i) {
    serial->print(i);
  }
      
  public:
  void writeAdd (int i) {
    serial->print(i);
  }
  
  public:
  void writeAdd (long i) {
    serial->print(i);
  }
    
  public:
  void writeInput (const char *s) {
    if ((debug_flags & 0x01)==0) {
      writeTime ();
      serial->print("<- ");
    }
    debug_flags = 0x01;   
    serial->print(s);
  }

  public:
  void writeOutput (const char *s) {
    if ((debug_flags & 0x02)==0) {
      writeTime ();
      serial->print("-> ");
    }
    debug_flags = 0x02;   
    serial->print(s);
  }

  public:
  void writeInputHex (uint8_t b) {
    if ((debug_flags & 0x01)==0) {
      writeTime ();
      serial->print("<-");
      debug_hexcounter=0;
    }
    debug_flags = 0x01;   
    writeHex (b);
  }

  public:
  void writeOutputHex (uint8_t b) {
    if ((debug_flags & 0x02)==0) {
      writeTime ();
      serial->print("->");
      debug_hexcounter=0;
    }
    debug_flags = 0x02;
    writeHex (b);
  }
  
  public:
  void writeInputChar (char s) {
    if ((debug_flags & 0x01)==0) {
      writeTime ();
      serial->print("<- ");
    }
    debug_flags = 0x01;   
    serial->print(s);
  }

  public:
  void writeInputNewString () {
    debug_flags = 0;
  }
  
  public:
  void writeHex (uint8_t b) {
    debug_hexcounter++;
    if (debug_hexcounter>16) {
      serial->println();
      serial->print("              ");
      debug_hexcounter=1;
    }
    serial->print(' ');
    serial->print(b>>4, HEX); 
    serial->print(b&0x0f, HEX);     
  }   
  
  public:
  void writeAvailableMemory () {
    write ( "Available memory: " );
    writeAdd (availableMemory());
  }
  
  
  
  private:
  uint32_t availableMemory() {
#if defined (ESP8266) || defined (ESP32)
    return ESP.getFreeHeap ();
#elif defined (__AVR__)
    uint16_t size = RAMEND;
    uint8_t *buf;
    while ((buf = (uint8_t *)malloc(size)) == NULL)  size--;
    free(buf);
    return size;
#else
    return 0;
#endif
  }


};


CRemoteXYDebugLog RemoteXYDebugLog (& REMOTEXY__DEBUGLOG_SERIAL, REMOTEXY__DEBUGLOG_SPEED);


#endif  //REMOTEXY__DEBUGLOG

#endif //_REMOTEXY_DEBUGLOG_H_