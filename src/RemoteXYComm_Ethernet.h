#ifndef RemoteXYComm_Ethernet_h
#define RemoteXYComm_Ethernet_h

// Include necessary header files
#if defined (ethernet_h_) 

#include "RemoteXYDebugLog.h"
#include "RemoteXYComm.h"
#include "RemoteXYFunc.h"

// Define the maximum size of the send buffer
#define REMOREXYCOMM_ETHERNET__SEND_BUFFER_SIZE 32

// Define the Ethernet client class that inherits from the CRemoteXYClient class
class CRemoteXYClient_Ethernet : public CRemoteXYClient {
  public:
  EthernetClient client;
  
  // Send buffer and buffer count variables
  uint8_t sendBuffer[REMOREXYCOMM_ETHERNET__SEND_BUFFER_SIZE];
  uint16_t sendBufferCount; 
  uint16_t sendBytesAvailable;  

  // Implement the connect, connected, stop, handler, startWrite, and write methods
  // from the CRemoteXYClient class
  public:
  uint8_t connect (const char *host, uint16_t port) override {
    return client.connect(host, port);
  }; 
  
  public:
  uint8_t connected () override {
    return client.connected();
  };

  public:
  void stop () override {
    client.stop ();
  };
  
  public:
  void handler () override { 
    while (client.available ()) notifyReadByteListener (client.read ());
  }
         

  
  public:
  void startWrite (uint16_t len) override {
    sendBytesAvailable = len;
    sendBufferCount = 0;
  }  
  
  public:
  void write (uint8_t b) override {
    sendBuffer[sendBufferCount++] = b;
    sendBytesAvailable--;       
    if ((sendBufferCount == REMOREXYCOMM_ETHERNET__SEND_BUFFER_SIZE) || (sendBytesAvailable==0)) {  
      client.write (sendBuffer, sendBufferCount);
      sendBufferCount=0;    
    } 
  } 
  
};

// Define a custom EthernetServer class for ESP32
#if defined (ESP32)
class EthernetServerESP32: public EthernetServer {
  public:
  EthernetServerESP32 (uint16_t port) : EthernetServer (port) {}
  void begin(uint16_t port=0) {}
};
#endif

// Define the Ethernet server class that inherits from the CRemoteXYServer class
class CRemoteXYServer_Ethernet : public CRemoteXYServer {
  private:
  EthernetServer * server;
  uint8_t soketConnectArr[MAX_SOCK_NUM]; 

  public: 
  CRemoteXYServer_Ethernet (uint16_t _port)  {
#if defined (ESP32)
    server = new EthernetServerESP32 (_port); 
#else
    server = new EthernetServer (_port); 
#endif
    for (uint8_t i = 0; i < MAX_SOCK_NUM; i++) soketConnectArr[i] = 0;  
  }
  
  
  public:  
  virtual uint8_t begin () override {
    server->begin (); 
    return 1;   
  }
  

  uint8_t available (CRemoteXYClient * client) override {  
    EthernetClient cl; 
    uint8_t i;
    for (i = 0; i < MAX_SOCK_NUM; i++) {
      cl = EthernetClient (i);
      if (!cl.connected ()) soketConnectArr[i] = 0;
    }
    cl = server->available ();
    if (cl.connected ()) {
      i = cl.getSocketNumber();
      if (i<MAX_SOCK_NUM) {
        if (soketConnectArr[i] == 0) {
          soketConnectArr[i] = 1;
          ((CRemoteXYClient_Ethernet*) client)->client = cl;
          return 1;
        }
      }
    }
    return 0;
  } 
  
};

// Define the Ethernet communication class that inherits from the CRemoteXYComm class
class CRemoteXYComm_Ethernet : public CRemoteXYComm {

  private:
  uint8_t mac[6];
  enum {NoHardware, LinkDetecting, Work} state;
  uint32_t timeout;
  
  
  public:
  CRemoteXYComm_Ethernet (const char * macAddress) : CRemoteXYComm () {   
    rxy_getMacAddr (macAddress, mac);  
     
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Ethernet begin ...");
#endif 

    Ethernet.begin(mac, 1000);
    
    if (Ethernet.hardwareStatus() == EthernetNoHardware) { 
      state = NoHardware;   
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("Ethernet module was not found");    
#endif 
    }
    else {
      state = LinkDetecting;         
#if defined(REMOTEXY__DEBUGLOG)
      if (!linkON ()) { 
        RemoteXYDebugLog.write("Ethernet link OFF");
      }
#endif 
    }  
    timeout = millis ();  
  }
  

  private:
  uint8_t linkON () {
    return 1;
    if (Ethernet.hardwareStatus () == EthernetW5100) {
      if (uint32_t(Ethernet.localIP()) != 0) return 1;
    }
    else {
      if (Ethernet.linkStatus () == LinkON) return 1;
    }
    return 0;
  }
  
  
  public:
  void handler () override {          
  
    if (state == NoHardware) return;
    
    if (linkON ()) {
      if (state == LinkDetecting) {
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("Ethernet link ON");
        RemoteXYDebugLog.write("IP: ");
        RemoteXYDebugLog.serial->print(Ethernet.localIP());
#endif  
        state = Work;
      }
      timeout = millis (); 
    }
    else {  // LinkOFF
      if (state == Work) {
        state = LinkDetecting;
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("Ethernet link OFF");
#endif  
      }
      else {
        if (millis () - timeout > 5000) {
          Ethernet.begin(mac, 100);
          timeout = millis ();  
        }
      }
    }
  } 
  
         
  public:                   
  uint8_t configured () override {
    if (state == Work) return 1;
    return 0;
  }                
  
  public:  
  CRemoteXYServer * createServer (uint16_t _port) override {
    return new CRemoteXYServer_Ethernet (_port);
  }
  

  CRemoteXYClient * newClient () override {
    return new CRemoteXYClient_Ethernet ();
  }

};


#endif // ethernet_h_

#endif //RemoteXYComm_Ethernet_h
