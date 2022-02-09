#ifndef RemoteXYStream_HardSerial_h
#define RemoteXYStream_HardSerial_h

#include "RemoteXYStream.h"


class CRemoteXYStream_HardSerial : public CRemoteXYStream {
  
  private:
  HardwareSerial * serial;
  
  public:
  CRemoteXYStream_HardSerial (HardwareSerial * _serial, long _serialSpeed) : CRemoteXYStream () {
    serial = _serial;
    serial->begin (_serialSpeed);
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Init hardware serial ");
    RemoteXYDebugLog.writeAdd(_serialSpeed);
    RemoteXYDebugLog.writeAdd(" baud");
#endif
  }              
  
  void handler () override {   
    while (serial->available ()) notifyReadByteListener (serial->read ());
  }

  void write (uint8_t byte) override {
    serial->write (byte);
  }
  
};




#endif //RemoteXYStream_HardSerial_h