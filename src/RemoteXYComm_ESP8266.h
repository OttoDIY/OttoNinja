// Constants and macros
#define REMOTEXYCOMM_ESP8266__SERVER_TIMEOUT 10000
#define REMOTEXYCOMM_ESP8266__WIFICONNECT_TIMEOUT 30000
#define REMOTEXYCOMM_ESP8266__MAX_WIFI_ATTEMPTS 5

#define REMOTEXYCOMM_ESP8266__ID_WIFICONNECT 1

#define REMOTEXYCOMM_ESP8266__MAX_SEND_BYTE_SIZE 2048

const char * AT_MESSAGE_WIFI_DISCONNECT = "WIFI DISCONNECT";


// Base class for ESP8266 communication
class CRemoteXYComm_ESP8266_Proto : public CRemoteXYComm_AT {
  protected:  
  const char * wifiSsid;
  const char * wifiPassword;
  enum { Search, Reset, SearchAfterReset, Init, WaitWiFi, WaitReconnect, Configured } state;
  uint32_t timeOut;  
  
  public:
  CRemoteXYComm_ESP8266_Proto (CRemoteXYStream *_serial, const char * _wifiSsid, const char * _wifiPassword) : CRemoteXYComm_AT (_serial, REMOTEXYCOMM_ESP8266__MAX_SEND_BYTE_SIZE) {
    wifiSsid = _wifiSsid;
    wifiPassword = _wifiPassword;
    state = Search;
    findModule ();
  }
  
  // ... (other functions)
};


// ESP8266 communication class
class CRemoteXYComm_ESP8266 : public CRemoteXYComm_ESP8266_Proto {

  public:
  CRemoteXYComm_ESP8266 (CRemoteXYStream *_serial, const char * _wifiSsid, const char * _wifiPassword) : CRemoteXYComm_ESP8266_Proto (_serial, _wifiSsid, _wifiPassword) {
  }

  protected:
  void moduleReset () override {        
    if (!initModule ()) {
      state = Search;
      findModule ();
    }
  }
  
  // ... (other functions)
};


// ESP8266 access point class
class CRemoteXYComm_ESP8266Point : public CRemoteXYComm_ESP8266_Proto {

  public:
  CRemoteXYComm_ESP8266Point (CRemoteXYStream *_serial, const char * _wifiSsid, const char * _wifiPassword) : CRemoteXYComm_ESP8266_Proto (_serial, _wifiSsid, _wifiPassword) {
  }
  
  protected:
  void moduleReset () override {        
    initModule ();
    if (state != Configured) {
      state = Search;
      findModule ();
    }
  }
  
  // ... (other functions)
};
