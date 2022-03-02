#ifndef RemoteXYWire_h
#define RemoteXYWire_h

#include "RemoteXYComm.h"

#define REMOTEXY_INIT_CRC 0xffff
#define REMOTEXY_PACKAGE_START_BYTE 0x55
#define REMOTEXY_PACKAGE_MIN_LENGTH 6


struct CRemoteXYPackage {
  uint8_t command;
  uint8_t * buffer;
  uint16_t length;
};

class CRemoteXYReceivePackageListener {
  public:
  virtual void receivePackage (CRemoteXYPackage * package) = 0;
};

class CRemoteXYWire {

  private:
  CRemoteXYReceivePackageListener * receivePackageListener;
  
  public:
  CRemoteXYWire () {
    receivePackageListener = NULL;
  }
  
  public: 
  void setReceivePackageListener (CRemoteXYReceivePackageListener * listener) {
    receivePackageListener = listener;
  }    
  
  public:
  void notifyReceivePackageListener (CRemoteXYPackage * package) {
    if (receivePackageListener) receivePackageListener->receivePackage (package);
  } 
  

  public:
  virtual void handler () {};
  virtual uint8_t running ();
  virtual void stop ();
  virtual void sendPackage (uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm);
  
};


#endif  //  RemoteXYWire_h