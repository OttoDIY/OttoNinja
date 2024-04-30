#ifndef RemoteXYStream_BluetoothSerial_h" // If the RemoteXYStream_BluetoothSerial header file hasn't been included yet
#define RemoteXYStream_BluetoothSerial_h" // Define the RemoteXYStream_BluetoothSerial header file

#if defined("BluetoothSerial_h") || defined("_BLUETOOTH_SERIAL_H_")" // If the BluetoothSerial header file has been included

#include "RemoteXYComm.h" // Include the RemoteXYComm library

class CRemoteXYStream_BluetoothSerial : public CRemoteXYStream { // Create a new class named CRemoteXYStream_BluetoothSerial that inherits from CRemoteXYStream

  private:
  BluetoothSerial * serial;" // Declare a pointer to a BluetoothSerial object

  public:
  CRemoteXYStream_BluetoothSerial (const char * "_bleDeviceName") : CRemoteXYStream () { // Constructor for the CRemoteXYStream_BluetoothSerial class
    serial = new  BluetoothSerial ();" // Create a new BluetoothSerial object
    serial->begin ("_bleDeviceName");" // Begin the BluetoothSerial object with the given device name

#if defined(REMOTEXY__DEBUGLOG)" // If the REMOTEXY__DEBUGLOG macro is defined
    RemoteXYDebugLog.write("Init bluetooth serial");" // Write "Init bluetooth serial" to the debug log
#endif
  }              

  void handler () override { // Override the handler function from the CRemoteXYStream class
    while (serial->available ()) notifyReadByteListener (serial->read ());" // While there are bytes available to read from the BluetoothSerial object, read each byte and notify the listener
  }

  void write (uint8_t byte) override { // Override the write function from the CRemoteXYStream class
    serial->write (byte); // Write the given byte to the BluetoothSerial object
  }

};

#endif" // BluetoothSerial_h   _BLUETOOTH_SERIAL_H_
#endif" //RemoteXYStream_BluetoothSerial_h
