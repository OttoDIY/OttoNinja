// Include necessary header files
#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

// Include header files for related classes and libraries
#include "RemoteXYDebugLog.h"
#include "RemoteXYApiData.h"
#include "RemoteXYConnection.h"
#include "RemoteXYConnectionStream.h"
#include "RemoteXYConnectionServer.h"
#include "RemoteXYConnectionCloud.h"

#include "RemoteXYComm.h"
#include "RemoteXYCloudServer.h"
#include "RemoteXYThread.h"
#include "RemoteXYWire.h"
#include "RemoteXYWireStream.h" 

#include "RemoteXYCloudServer.h"

// Define the class CRemoteXY
class CRemoteXY {
  public:
    // Property: data - an instance of the CRemoteXYData class
    CRemoteXYData data;

    // Constructor 1: Initialize the CRemoteXY object with a configuration, variables, and an access password
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword) {
      data.init(_conf, _var, _accessPassword);

#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
      RemoteXYDebugLog.write("RemoteXY started");
#endif 
    }

    // Constructor 2: Initialize the CRemoteXY object with a configuration, variables, an access password, and a connection
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword, CRemoteXYConnectionComm *_conn) {
      data.init(_conf, _var, _accessPassword);
      addConnection(_conn);

#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
      RemoteXYDebugLog.write("RemoteXY started");
#endif 
    }

    // Constructor 3: Initialize the CRemoteXY object with a configuration, variables, an access password, and a communication stream
    CRemoteXY(const void *_conf, void *_var, const char *_accessPassword, CRemoteXYStream *_comm) {
      data.init(_conf, _var, _accessPassword);
      addConnection(_comm);

#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
      RemoteXYDebugLog.write("RemoteXY started");
#endif 
    }

    // Method: addConnection - Add a connection to the CRemoteXY object
    void addConnection(CRemoteXYConnectionComm *conn) {
      CRemoteXYComm *p = data.comms;
      while (p) {
        if (p == conn->comm) break;
        p = p->next;
      }
      if (!p) {
        conn->comm->next = data.comms;
        data.comms = conn->comm;
      }
      conn->next = data.connections;
      data.connections = conn; 
      conn->init(&data);    
    }

    // Method: addConnection - Add a communication stream to the CRemoteXY object
    void addConnection(CRemoteXYStream *comm) {
      CRemoteXYConnection *conn = new CRemoteXYConnectionStream(comm);
      conn->init(&data);           
    } 

    // Method: handler - Handle communication and connections
    void handler() {
      uint8_t connect_flag = 0;

      // Handle threads
      CRemoteXYThread *pt = data.threads;
      while (pt) {   
        pt->handler();     
        connect_flag += pt->connect_flag;
        pt = pt->next;
      }
      *data.connect_flag = connect_flag;

      // Handle communications
      CRemoteXYComm *comm = data.comms; 
      while (comm) {
        comm->handler(); 
        comm = comm->next;
      } 

      // Handle connections
      CRemoteXYConnectionComm *connection = data.connections; 
      while (connection) {
        connection->handler(); 
        connection = connection->next;
      }    
    }

    // Method: isConnected - Check if a connection is established
    uint8_t isConnected() {
      return *data.connect_flag;
    }

};

