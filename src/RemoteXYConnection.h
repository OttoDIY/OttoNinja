#ifndef RemoteXYConnection_h
#define RemoteXYConnection_h

#include "RemoteXYComm.h"    // Include RemoteXY Communication class header
#include "RemoteXYWire.h"     // Include RemoteXY Wire communication library header
#include "RemoteXYCloudServer.h"  // Include RemoteXY Cloud Server library header

// Macro to suppress warnings for unused parameters
#define UNUSED(x) (void)(x)

class CRemoteXYConnection {   

  public:   
  // Initialize the connection with data
  virtual void init (CRemoteXYData * _data);  

  public:
  // Empty handler function
  virtual void handler () {};  
  
  // Handle Wire communication
  virtual void handleWire (CRemoteXYWire * wire) {UNUSED (wire);};  

  // Stop thread listener
  virtual void stopThreadListener (CRemoteXYWire * wire) {UNUSED (wire);};  
  
};

// Connection object for communication
class CRemoteXYConnectionComm : public CRemoteXYConnection { 
  public:   
  // Pointer to the next connection object
  CRemoteXYConnectionComm * next;  
  // Pointer to the communication object
  CRemoteXYComm * comm;  

  public:
  // Initialize the communication object
  CRemoteXYConnectionComm (CRemoteXYComm * _comm) {
    comm = _comm;  
  }  
  
};

#endif //RemoteXYConnection_h  
// End of RemoteXYConnection header file

