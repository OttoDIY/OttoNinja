#ifndef RemoteXYConnectionServer_h
#define RemoteXYConnectionServer_h

#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYWireStream.h"
#include "RemoteXYThread.h"



class CRemoteXYConnectionServer: public CRemoteXYConnectionComm, public CRemoteXYClientAvailableListener { 
  public:
  uint16_t port;
  CRemoteXYData * data;
  CRemoteXYServer * server;
  CRemoteXYClient * clients;
  CRemoteXYWireStream * wires;
  uint8_t serverRunning;
  
  
  CRemoteXYConnectionServer (CRemoteXYComm * _comm, uint16_t _port = 0) : CRemoteXYConnectionComm (_comm) {
    port = _port;
    clients = NULL;
    server = comm->createServer (port); 
    server->setClientAvailabListener (this);
#if defined(REMOTEXY__DEBUGLOG)
    if (!server) {
      RemoteXYDebugLog.write ("Server was not created");
    }
#endif     
    serverRunning = 0;
    wires = NULL;  
  }
  
  void init (CRemoteXYData * _data) {
    data = _data;
  }
  
  public:  
  void handler () override {
    if (!server) return;
    if (comm->configured ()) {
      if (serverRunning) clientAvailable ();
      else {
        if (server->begin ()) {
          serverRunning=1;
#if defined(REMOTEXY__DEBUGLOG)
          RemoteXYDebugLog.write ("Server opened on port ");
          RemoteXYDebugLog.writeAdd (port);
#endif                
        }
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
        CRemoteXYThread::startThread (data, this, wire, 1);        
      }
      else {
        client->stop ();
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("Client reject");
#endif  
      }
    }
      
  }
  
  void handleWire (CRemoteXYWire * wire) override {
    CRemoteXYClient * client = ((CRemoteXYWireStream*)wire)->getClient ();
    if (client) {
      if (client->connected () && serverRunning && comm->configured ()) wire->handler (); 
      else stopThreadListener (wire);
    }
  }

  void stopThreadListener (CRemoteXYWire * wire) override {
    CRemoteXYClient * client = ((CRemoteXYWireStream*)wire)->getClient ();
    if (client) {
      client->stop ();
      wire->stop ();
    }
  }
  

  
};


#endif // RemoteXYConnectionServer_h