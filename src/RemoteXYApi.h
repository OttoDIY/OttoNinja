// Include necessary header files
#ifndef REMOTEXY_H
#define REMOTEXY_H

#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

#include "RemoteXYDebugLog.h"
#include "RemoteXYApiData.h"
#include "RemoteXYConnection.h"
#include "RemoteXYConnectionStream.h"
#include "RemoteXYConnectionServer.h"
#include "RemoteXYConnectionCloud.h"

#include "RemoteXYComm.h"
#include "RemoteXYThread.h"
#include "RemoteXYWire.h"
#include "RemoteXYWireStream.h"

// Define the class CRemoteXY
class CRemoteXY {
  public:
    // Property: data - an instance of the CRemoteXYData class
    CRemoteXYData data;

    // Constructor: Initialize the CRemoteXY object with a configuration, variables, and an access password
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword);

    // Constructor: Initialize the CRemoteXY object with a configuration, variables, an access password, and a connection
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword, CRemoteXYConnectionComm *_conn);

    // Constructor: Initialize the CRemoteXY object with a configuration, variables, an access password, and a communication stream
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword, CRemoteXYStream *_comm);

    // Destructor: Delete any dynamically allocated memory
    ~CRemoteXY();

    // Method: init - Initialize the CRemoteXY object with a configuration, variables, and an access password
    void init(const void *_conf, void *_var, const char *_accessPassword);

    // Method: addConnection - Add a connection to the CRemoteXY object
    void addConnection(CRemoteXYConnectionComm *conn);

    // Method: addConnection - Add a communication stream to the CRemoteXY object
    void addConnection(CRemoteXYStream *comm);

    // Method: connect - Establish a connection with a given configuration
    void connect(const void *_conf);

    // Method: disconnect - Close the current connection
    void disconnect();

    // Method: handler - Handle communication and connections
    void handler();

    // Method: isConnected - Check if a connection is established
    uint8_t isConnected();

    // Method: getConnectionStatus - Get the current connection status
    const char* getConnectionStatus();

  private:
    // Method: init - Initialize the CRemoteXY object with a configuration, variables, and an access password
    void init(const void *_conf, void *_var, const char *_accessPassword, CRemoteXYConnectionComm *_conn, CRemoteXYStream *_comm);

    // Pointer to the current connection
    CRemoteXYConnectionComm *connection;

    // Pointer to the current communication stream
    CRemoteXYStream *comm;

    // Pointer to the list of threads
    CRemoteXYThread *threads;

    // Pointer to the list of communications
    CRemoteXYComm *comms;

    // Pointer to the list of connections
    CRemoteXYConnectionComm *connections;

    // Pointer to the connect flag
    uint8_t *connect_flag;
};

#endif // REMOTEXY_H
