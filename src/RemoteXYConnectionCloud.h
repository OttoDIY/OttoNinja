#ifndef RemoteXYConnectionCloud_h
#define RemoteXYConnectionCloud_h

// Include necessary header files
#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"

// Define the timeout value for retrying cloud connection
#define REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT 20000

// Define the class for cloud connection
class CRemoteXYConnectionCloud: public CRemoteXYConnectionComm, CRemoteXYCloudClientAvailableListener {
  public:
    // Public variables
    uint16_t port;
    CRemoteXYCloudServer * cloudServer;
    const char * cloudHost; 
    const char * cloudToken;
    CRemoteXYData * data;
    CRemoteXYClient * client;
    uint32_t timeOut;

  public:
    // Constructor for the class
    CRemoteXYConnectionCloud (CRemoteXYComm * _comm, const char * _cloudHost, uint16_t _port, const char * _cloudToken) : CRemoteXYConnectionComm (_comm) {
      port = _port;
      cloudHost = _cloudHost;
      cloudToken = _cloudToken;
    }

  public:
    // Initialize the connection
    void init (CRemoteXYData * _data) override {
      data = _data;
      cloudServer = new CRemoteXYCloudServer (data, cloudToken, this);
      client = comm->newClient ();
      timeOut = -REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT;
    }

    // Handle the connection
    void handler () override {
      // Check if the communication is configured
      if (comm->configured ()) {
        // If the cloud server is running
        if (cloudServer->running ()) {
          cloudServer->handler();     
          timeOut = millis();      
        }
        // If the cloud server is not running
        else { // not serverRunning
          // If the timeout has been reached
          if (millis() - timeOut > REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT) {
            // Try to connect to the cloud server
            if (client->connect (cloudHost, port)) {
              cloudServer->begin (client);            
              timeOut = millis();   
            }
          }
        }
      }
      // If the communication is not configured
      else cloudServer->stop ();
      // If the cloud server is not running
      if (!cloudServer->running ()) {
        // If the client is connected
        if (client->connected ()) {
          client->stop ();
        }
      }
    }  

    // Handle the cloud wire when it is available
    void clientAvailable (CRemoteXYWireCloud * cloudWire) override {
      // If the number of running clients is less than the maximum limit
      if (CRemoteXYThread::runningCount (data) < REMOTEXY_MAX_CLIENTS) { 
        CRemoteXYThread::startThread (data, this, cloudWire, 1);  
      }
      // If the number of running clients is greater than or equal to the maximum limit
      else {
        cloudWire->stop ();
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("Client reject");
#endif           
      }
    }

    // Handle the wire
    void handleWire (CRemoteXYWire * wire) override {  
      // If the cloud server is running and the communication is configured
      if (cloudServer->running () && comm->configured ()) wire->handler (); 
      // If the cloud server is not running
      else stopThreadListener (wire);
    }

    // Stop the thread listener
    void stopThreadListener (CRemoteXYWire * wire) override {
      wire->stop ();
    }
};


#endif // RemoteXYConnectionCloud_h
