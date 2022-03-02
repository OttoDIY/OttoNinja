#ifndef RemoteXYComm_WiFi_h
#define RemoteXYComm_WiFi_h

#include "RemoteXYDebugLog.h"
#include "RemoteXYComm.h"
 
#if defined (WiFi_h) 

#if defined (ESP8266) || defined (ESP32)
#define REMOREXYCOMM_WIFI__SEND_BUFFER_SIZE 256
#else  // arduino shield
#define REMOREXYCOMM_WIFI__SEND_BUFFER_SIZE 64
#endif





class CRemoteXYClient_WiFi : public CRemoteXYClient {
  public:
  WiFiClient client;
  
  uint8_t sendBuffer[REMOREXYCOMM_WIFI__SEND_BUFFER_SIZE];
  uint16_t sendBufferCount; 
  uint16_t sendBytesAvailable;  

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
    if ((sendBufferCount == REMOREXYCOMM_WIFI__SEND_BUFFER_SIZE) || (sendBytesAvailable==0)) {
      client.write (sendBuffer, sendBufferCount);
      sendBufferCount=0;
    } 
  } 
  
};

class CRemoteXYServer_WiFi : public CRemoteXYServer {
  private:
  WiFiServer * server;

  public: 
  CRemoteXYServer_WiFi (uint16_t _port)  {
    server = new WiFiServer (_port); 
  }
  
  
  public:  
  virtual uint8_t begin () override {
    server->begin (); 
    return 1;   
  }
  
 
  void stop () override {
#if defined (ESP8266) || defined (ESP32)  
    server->stop ();  
#endif
  } 
 


  uint8_t available (CRemoteXYClient * client) override {     
#if defined (ESP8266) || defined (ESP32)  
    if (!server->hasClient()) return 0; 
#endif        
    WiFiClient cl = server->available ();
    if (cl) {
      if (cl.connected ()) {
#if defined (ESP8266) 
        cl.disableKeepAlive ();     // remove memory leak
#endif
        ((CRemoteXYClient_WiFi*) client)->client = cl;
        return 1;
      }
    }
    return 0;
  } 
  
};


class CRemoteXYComm_WiFi : public CRemoteXYComm {

  private:
  char * wifiSsid;
  char * wifiPassword;
  uint8_t wifiStatus;
  
  
  public:
  CRemoteXYComm_WiFi (const char * _wifiSsid, const char * _wifiPassword) : CRemoteXYComm () {
    wifiSsid = (char *)_wifiSsid;
    wifiPassword = (char *)_wifiPassword;
    

    wifiStatus = WiFi.status();

#if defined (ESP8266) || defined (ESP32)

    WiFi.disconnect();
    WiFi.softAPdisconnect(true);    
    WiFi.mode(WIFI_STA);
    
#else // NOT ESP

    if (wifiStatus == WL_NO_SHIELD) {    
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("WiFi module was not found");    
#endif 
      return;
    }
    
#endif // ESP
    
        
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Сonnecting to WiFi: ");
    RemoteXYDebugLog.writeAdd(wifiSsid);
    RemoteXYDebugLog.writeAdd(" ...");
#endif    

    WiFi.begin(wifiSsid, wifiPassword);   
     
#if defined (ESP8266) || defined (ESP32)
    WiFi.setAutoReconnect (true);
#endif  
     
  }
  
  
  void handler () override {            
  
    uint8_t prev_wifiStatus = wifiStatus;
    wifiStatus = WiFi.status();
    
    if (wifiStatus == WL_CONNECTED) {
      if (prev_wifiStatus != WL_CONNECTED) {
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("WiFi connected");
        RemoteXYDebugLog.write ("IP: ");
        RemoteXYDebugLog.serial->print (WiFi.localIP());    
#endif
        
      }    
    }
    else {  // != WL_CONNECTED 
      if (prev_wifiStatus == WL_CONNECTED) { 
#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.write ("WiFi disconnected");
#endif
#if defined (ESP32)
        WiFi.disconnect();  
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiSsid, wifiPassword); 
#endif
      }
    }
  } 
  
         
  public:                   
  uint8_t configured () override {
    if (wifiStatus == WL_CONNECTED) return 1;
    return 0;
  }                
  
  public:  
  CRemoteXYServer * createServer (uint16_t _port) override {
    return new CRemoteXYServer_WiFi (_port);
  }
  

  CRemoteXYClient * newClient () override {
    return new CRemoteXYClient_WiFi ();
  }

};



class CRemoteXYComm_WiFiPoint : public CRemoteXYComm {
  
  uint8_t state;
  
  public:
  CRemoteXYComm_WiFiPoint (const char * _wifiSsid, const char * _wifiPassword) : CRemoteXYComm () {

    
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Creating WiFi point: ");
    RemoteXYDebugLog.writeAdd(_wifiSsid);
    RemoteXYDebugLog.writeAdd(" ...");
#endif

#if defined (ESP8266) || defined (ESP32)
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_wifiSsid, _wifiPassword);
    state = 1;
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("WiFi point created");
    RemoteXYDebugLog.write ("IP: ");
    RemoteXYDebugLog.serial->print (WiFi.softAPIP());    
#endif
    
#elif defined (WiFiNINA_h)   // WiFiNINA

    state = 0;
    if (WiFi.status() == WL_NO_SHIELD) {    
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("WiFi module was not found");    
#endif 
      return;
    }
    if (WiFi.beginAP (_wifiSsid, _wifiPassword) != WL_AP_LISTENING) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("WiFi module does not support AP mode");    
#endif 
      return;    
    }
    state = 1;
    
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("WiFi point created");    
#endif  

#else  // other boards not support AP mode
    state = 0;
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("WiFi module does not support AP mode");    
#endif 
  
#endif // ESP

  }
              
  public:                   
  uint8_t configured () override {
    return state;
  } 
  
    
  public:  
  CRemoteXYServer * createServer (uint16_t _port) override {   
    return new CRemoteXYServer_WiFi (_port); 
  }
  

  CRemoteXYClient * newClient () override {
    return new CRemoteXYClient_WiFi ();
  }

};


#endif // WiFi_h

#endif //RemoteXYComm_WiFi_h