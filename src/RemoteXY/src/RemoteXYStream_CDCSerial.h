#ifndef RemoteXYStream_CDCSerial_h
#define RemoteXYStream_CDCSerial_h

#if defined(USBCON)

#include "RemoteXYStream.h"


class CRemoteXYStream_CDCSerial : public CRemoteXYStream {
  
  private:
  Serial_ * serial;
  
  public:
  CRemoteXYStream_CDCSerial (Serial_ * _serial, long _serialSpeed) : CRemoteXYStream () {
    serial = _serial;
    serial->begin (_serialSpeed);
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Init CDC serial ");
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


#endif  //USBCON

#endif //RemoteXYStream_CDCSerial_h