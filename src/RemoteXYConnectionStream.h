#ifndef REMOTEXY_CONNECTION_STREAM_H
#define REMOTEXY_CONNECTION_STREAM_H

#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"
#include "RemoteXYStream.h"

/**
 * @class CRemoteXYConnectionStream
 * @brief A class that represents a connection stream for RemoteXY.
 *
 * This class inherits from CRemoteXYConnection and provides a way to handle
 * a connection stream using a CRemoteXYStream object.
 */
class CRemoteXYConnectionStream : public CRemoteXYConnection {
  // Pointer to the stream object
  CRemoteXYStream *stream;

  public: 
  /**
   * @brief Constructor with a CRemoteXYStream object as parameter.
   *
   * @param stream A pointer to the stream object.
   *
   * The constructor initializes the stream pointer with the given stream object.
   * If the stream pointer is null, an error message is logged.
   */
  CRemoteXYConnectionStream(CRemoteXYStream *stream) : stream(stream) {
    // Check if the stream pointer is not null
    if (!stream) {
      REMOTEXY_DEBUG_LOG_ERROR("Stream pointer is null.");
      return;
    }
  }

  /**
   * @brief Override the init method from CRemoteXYConnection.
   *
   * @param data A pointer to the data object.
   *
   * The init method creates a new CRemoteXYWireStream object with data as parameter,
   * initializes the wire object with the stream object, and starts a new thread
   * with data, this (CRemoteXYConnectionStream object), wireStream, and 0 as parameters.
   */
  void init(CRemoteXYData *data) override {
    // Create a new CRemoteXYWireStream object with data as parameter
    CRemoteXYWireStream *wireStream = new CRemoteXYWireStream(data);
    // Initialize the wire object with the stream object
    wireStream->begin(stream);
    // Start a new thread with data, this (CRemoteXYConnectionStream object), wireStream, and 0 as parameters
    CRemoteXYThread::startThread(data, this, wireStream, 0);
  };

  /**
   * @brief Override the handleWire method from CRemoteXYConnection.
   *
   * @param wire A pointer to the wire object.
   *
   * The handleWire method calls the handler method of the wire object.
   */
  void handleWire(CRemoteXYWire *wire) override {
    // Call the handler method of the wire object
    wire->handler();
  };
};

#endif // REMOTEXY_CONNECTION_STREAM_H

