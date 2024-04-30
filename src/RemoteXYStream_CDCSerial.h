#ifndef RemoteXYStream_CDCSerial_h
#define RemoteXYStream_CDCSerial_h

#if defined(USBCON)

#include <Stream.h>
#include "RemoteXYStream.h"

class CRemoteXYStream_CDCSerial : public CRemoteXYStream {
  
  private:
  Stream * serial;
  long serialSpeed;

  public:
  CRemoteXYStream_CDCSerial (Stream * _serial, long _serialSpeed) : CRemoteXYStream () {
    serial = _serial;
    serialSpeed = _serialSpeed;
    serial->begin(serialSpeed);

#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.print("Init CDC serial ");
    RemoteXYDebugLog.print(serialSpeed);
    RemoteXYDebugLog.println(" baud");
#endif
  }

  void handler() override {
    while (serial->available()) notifyReadByteListener(serial->read());
  }

  void write(uint8_t byte) override {
    serial->write(byte);
  }

};

#endif  //USBCON

#endif //RemoteXYStream_CDCSerial_h
