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
  //  _data: Pointer to the CRemoteXYData object containing connection data
  virtual void init (CRemoteXYData * _data);  

  public:
  // Empty handler function
  // This function is meant to be overridden by derived classes to handle specific events
  virtual void handler () {};  
  
  // Handle Wire communication
  //  wire: Pointer to the CRemoteXYWire object for communication
  //
