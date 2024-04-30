#ifndef REMOTEXY_CONNECTION_STREAM_H
#define REMOTEXY_CONNECTION_STREAM_H

#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"
#include "RemoteXYStream.h"

// Define the CRemoteXYConnectionStream class which inherits from CRemoteXYConnection
class CRemoteXYConnectionStream : public CRemoteXYConnection {
  // Pointer to the stream object
  CRemoteXYStream *stream;

  public: 
  // Constructor with a CRemoteXYStream object as parameter
  CRemoteXYConnectionStream(CRemoteXYStream *stream) : stream(stream) {}

  // Override the init method from CRemoteXYConnection
  void init(CRemoteXYData *data) override {
    // Create a new CRemoteXYWireStream object with data as parameter
    CRemoteXYWireStream *wireStream = new CRemoteXYWireStream(data);
    // Initialize the wire object with the stream object
    wireStream->begin(stream);
    // Start a new thread with data, this (CRemoteXYConnectionStream object), wireStream, and 0 as parameters
    CRemoteXYThread::startThread(data, this, wireStream, 0);
  };

  // Override the handleWire method from CRemoteXYConnection
  void handleWire(CRemoteXYWire *wire) override {
    // Call the handler method of the wire object
    wire->handler();
  };
};

#endif // REMOTEXY_CONNECTION_STREAM_H
