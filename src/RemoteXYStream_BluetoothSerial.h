#ifndef RemoteXYStream_BluetoothSerial_h
#define RemoteXYStream_BluetoothSerial_h

#if defined (BluetoothSerial_h) || defined (_BLUETOOTH_SERIAL_H_)

#include "RemoteXYComm.h"


class CRemoteXYStream_BluetoothSerial : public CRemoteXYStream {
  
  private:
  BluetoothSerial * serial;
  
  public:
  CRemoteXYStream_BluetoothSerial (const char * _bleDeviceName) : CRemoteXYStream () {
    serial = new  BluetoothSerial ();
    serial->begin (_bleDeviceName);

#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Init bluetooth serial");
#endif
  }              
  
  
  void handler () override {   
    while (serial->available ()) notifyReadByteListener (serial->read ());
  }


  void write (uint8_t byte) override {
    serial->write (byte);
  }


};

#endif // BluetoothSerial_h   _BLUETOOTH_SERIAL_H_
#endif //RemoteXYStream_BluetoothSerial_h