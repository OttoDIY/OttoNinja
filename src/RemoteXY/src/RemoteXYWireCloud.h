#ifndef RemoteXYWireCloud_h
#define RemoteXYWireCloud_h

#include "RemoteXYWire.h"


#define REMOTEXYWIRECLOUD_FREE_ID 0xff


class CRemoteXYSendPackageListener {
  public:
  virtual void sendPackage (uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) = 0;
};

class CRemoteXYWireCloud: public CRemoteXYWire {

  public:
  CRemoteXYWireCloud * next;
  uint8_t id;  // 0..7
  uint8_t newConnection;
  
  private:
  CRemoteXYSendPackageListener * sendPackageListener;
  
  public:
  CRemoteXYWireCloud (CRemoteXYSendPackageListener * _sendPackageListener) : CRemoteXYWire () {
    sendPackageListener = _sendPackageListener;
    id = REMOTEXYWIRECLOUD_FREE_ID;
    newConnection = 0; 
  }
          
  public:
  void init (uint8_t _id) {
    id = _id;
    newConnection = 1;
  }  
  
  /*
  public:
  void begin () {
    newConnection = 0; 
  }  
  */ 
  
  public:    
  void stop () override {
    setReceivePackageListener (NULL);
    id = REMOTEXYWIRECLOUD_FREE_ID;
    newConnection = 0;
  }
  
  public:
  uint8_t running () override {
    if (id == REMOTEXYWIRECLOUD_FREE_ID) return 0;
    return 1;
  }

  public:
  uint8_t isNewConnection () {
    return newConnection;
  }
  
        
  public:
  void sendPackage (uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) override {
    sendPackageListener->sendPackage (command | (id<<1), buf, length, fromPgm);
  }
  

  public:
  void receivePackage (CRemoteXYPackage * package) {
    notifyReceivePackageListener (package);
  }

};

#endif //RemoteXYWireCloud_h