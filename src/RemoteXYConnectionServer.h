#ifndef RemoteXYConnectionServer_h
#define RemoteXYConnectionServer_h

// Include necessary header files
#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYWireStream.h"
#include "RemoteXYThread.h"

class CRemoteXYConnectionServer: public CRemoteXYConnectionComm, public CRemoteXYClientAvailableListener { 
  public:
  // Port number for the server to listen on
  uint16_t port;
  // Pointer to the data object
  CRemoteXYData * data;
  // Pointer to the server object
  CRemoteXYServer * server;
  // Pointer to the clients list
  CRemoteXYClient * clients;
  // Pointer to the wires list
  CRemoteXYWireStream * wires;
  // Flag to check if the server is running
  uint8_t serverRunning;

  // Constructor for the CRemoteXYConnectionServer class
  CRemoteXYConnectionServer (CRemoteXYComm * _comm, uint16_t _port = 0) : CRemoteXYConnectionComm (_comm) {
    port = _port;
    clients = NULL;
    server = comm->createServer (port); 
    server->setClientAvailabListener (this);

    // Debug logging for server creation
#if defined(REMOTEXY__DEBUGLOG)
    if (!server) {
      RemoteXYDebugLog.write ("Server was not created");
    }
#endif     

    serverRunning = 0;
    wires = NULL;  
  }

  // Initialize the data object
  void init (CRemoteXYData * _data) {
    data = _data;
  }

  // Handler function to manage server and client connections
  public:  
  void handler () override {
    if (!server) return;
    if (comm->configured ()) {
      if (serverRunning) clientAvailable ();
      else {
        if (server->begin ()) {
          serverRunning=1;

          // Debug logging for server start
#if defined(REMOTEXY__DEBUGLOG)
          RemoteXYDebugLog.write ("Server opened on port ");
          RemoteXYDebugLog.writeAdd (port);
#endif                
        }

        // Debug logging for server start failure
#if defined(REMOTEXY__DEBUGLOG)
        else {
          RemoteXYDebugLog.write ("Server was not started");
        }
#endif                
      }
    }
    else {
      if (serverRunning) {      
        server->stop ();
        serverRunning =0;
      }
    }
  }

  // Client availability handler
  void clientAvailable () override {  
    CRemoteXYClient * client = clients;
    while (client) {
      if (!client->connected ()) break;
      client = client->next;
    }
    if (!client) {   
      client = comm->newClient ();
      client->next = clients;
      clients = client;
    }
    if (server->available (client)) {

      // Check if the maximum number of clients is not exceeded
      if (CRemoteXYThread::runningCount (data) < REMOTEXY_MAX_CLIENTS) { 
        CRemoteXYWireStream * wire = wires;
        while (wire) {         
          if (!wire->running()) break;  
          wire = wire->next;
        }
        if (!wire) {
          wire = new CRemoteXYWireStream (data);
          wire->next = wires;
          wires = wire;        
        }
        wire->begin (client);

        // Start the thread for the new client
        CRemoteXYThread::startThread (data, this, wire, 1);        
      }
      else {
        client->stop ();

        // Debug logging for client rejection
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("Client reject");
#endif  
      }
    }
      
  }

  // Wire handler function
  void handleWire (CRemoteXYWire * wire) override {
    CRemoteXYClient * client = ((CRemoteXYWireStream*)wire)->getClient ();
    if (client) {
      if (client->connected () && serverRunning && comm->configured ()) wire->handler (); 
      else stopThreadListener (wire);
    }
  }

  // Stop the thread listener function
  void stopThreadListener (CRemoteXYWire * wire) override {
    CRemoteXYClient * client = ((CRemoteXYWireStream*)wire)->getClient ();
    if (client) {
      client->stop ();
      wire->stop ();
    }
  }


};


#endif // RemoteXYConnectionServer_h
