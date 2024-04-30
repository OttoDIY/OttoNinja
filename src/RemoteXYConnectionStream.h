#ifndef RemoteXYConnectionStream_h
#define RemoteXYConnectionStream_h

#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"

// Define the CRemoteXYConnectionStream class which inherits from CRemoteXYConnection
class CRemoteXYConnectionStream: public CRemoteXYConnection {
  // Stream object of type CRemoteXYStream
  CRemoteXYStream * stream;

  public: 
  // Constructor with a CRemoteXYStream object as parameter
  CRemoteXYConnectionStream (CRemoteXYStream * _stream) {
    stream = _stream;
  }    
  
  // Override the init method from CRemoteXYConnection
  void init (CRemoteXYData * _data) override {
    // Create a new CRemoteXYWireStream object with _data as parameter
    CRemoteXYWireStream * wire = new CRemoteXYWireStream (_data);
    // Initialize the wire object with the stream object
    wire->begin (stream);  
    // Start a new thread with _data, this (CRemoteXYConnectionStream object), wire, and 0 as parameters
    CRemoteXYThread::startThread (_data, this, wire, 0);
  };
  
  // Override the handleWire method from CRemoteXYConnection
  void handleWire (CRemoteXYWire * wire) override {
    // Call the handler method of the wire object
    wire->handler ();
  }; 
  
};

#endif // CRemoteXYConnectionStream
