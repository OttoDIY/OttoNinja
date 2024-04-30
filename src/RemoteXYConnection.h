#ifndef RemoteXYConnection_h
#define RemoteXYConnection_h

#include "RemoteXYComm.h"    // Include RemoteXY Communication class header
#include "RemoteXYWire.h"     // Include RemoteXY Wire communication library header
#include "RemoteXYCloudServer.h"  // Include RemoteXY Cloud Server library header

#define UNUSED(x) (void)(x)   // Macro to suppress warnings for unused parameters

class CRemoteXYConnection {   

  public:   
  virtual void init (CRemoteXYData * _data);   // Initialize the connection with data

  public:
  virtual void handler () {};   // Empty handler function
  virtual void handleWire (CRemoteXYWire * wire) {UNUSED (wire);};   // Handle Wire communication
  virtual void stopThreadListener (CRemoteXYWire * wire) {UNUSED (wire);};   // Stop thread listener
  
};


class CRemoteXYConnectionComm : public CRemoteXYConnection { 
  public:   
  CRemoteXYConnectionComm * next;   // Pointer to the next connection object
  CRemoteXYComm * comm;   // Pointer to the communication object

  public:
  CRemoteXYConnectionComm (CRemoteXYComm * _comm) {
    comm = _comm;   // Initialize the communication object
  }  
  
};

#endif //RemoteXYConnection_h  
// End of RemoteXYConnection header file
