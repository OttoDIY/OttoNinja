#ifndef RemoteXYStream_CDCSerial_h
#define RemoteXYStream_CDCSerial_h

// Guard to prevent multiple inclusions of this header file

#if defined(USBCON)

// Include necessary header files only if USBCON is defined

#include <Stream.h>
#include "RemoteXYStream.h"

class CRemoteXYStream_CDCSerial : public CRemoteXYStream {
  
// Class definition for CRemoteXYStream_CDCSerial that inherits from CRemoteXYStream

  private:
  // Private member variables
  Stream * serial;
  long serialSpeed;

  public:
  // Public member functions and constructors

  CRemoteXYStream_CDCSerial (Stream * _serial, long _serialSpeed) : CRemoteXYStream () {
    // Constructor with initializer list
    // Sets the serial object and serial speed

    serial = _serial;
    serialSpeed = _serialSpeed;
    serial->begin(serialSpeed);

#if defined(REMOTEXY__DEBUGLOG)
    // Debug logging if REMOTEXY__DEBUGLOG is defined
    RemoteXYDebugLog.print("Init CDC serial ");
    RemoteXYDebugLog.print(serialSpeed);
    RemoteXYDebugLog.println(" baud");
#endif
  }

  void handler() override {
    // Overrides the handler function from CRemoteXYStream
    // Handles incoming serial data and notifies the read byte listener

    while (serial->available()) notifyReadByteListener(serial->read());
  }

  void write(uint8_t byte) override {
    // Overrides the write function from CRemoteXYStream
    // Writes a single byte to the serial object

    serial->write(byte);
  }

};

#endif  //USBCON

#endif //RemoteXYStream_CDCSerial_h

