#ifndef RemoteXYConnection_h
#define RemoteXYConnection_h

#include "RemoteXYComm.h"
#include "RemoteXYWire.h"
#include "RemoteXYCloudServer.h"

#define UNUSED(x) (void)(x)

class CRemoteXYConnection { 
  
  public:
  virtual void init (CRemoteXYData * _data);
  
  public:
  virtual void handler () {};
  virtual void handleWire (CRemoteXYWire * wire) {UNUSED (wire);};
  virtual void stopThreadListener (CRemoteXYWire * wire) {UNUSED (wire);};
  
};


class CRemoteXYConnectionComm : public CRemoteXYConnection { 
  public:   
  CRemoteXYConnectionComm * next;  
  CRemoteXYComm * comm;
  
  public:
  CRemoteXYConnectionComm (CRemoteXYComm * _comm) {
    comm = _comm;
  }  
  
};

#endif //RemoteXYConnection_h