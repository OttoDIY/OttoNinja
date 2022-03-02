#ifndef RemoteXYComm_ESP8266_h
#define RemoteXYComm_ESP8266_h

#include "RemoteXYDebugLog.h"
#include "RemoteXYComm_AT.h"

#define REMOTEXYCOMM_ESP8266__SERVER_TIMEOUT 10000
#define REMOTEXYCOMM_ESP8266__WIFICONNECT_TIMEOUT 30000

#define REMOTEXYCOMM_ESP8266__ID_WIFICONNECT 1

#define REMOTEXYCOMM_ESP8266__MAX_SEND_BYTE_SIZE 2048      

const char * AT_MESSAGE_WIFI_DISCONNECT = "WIFI DISCONNECT";


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
  
  
  void moduleFound () override {       
    if ((state == Reset) || (state == SearchAfterReset)) moduleReset ();
    else {
      state = Reset;
      timeOut = millis ();
      sendATCommandForResult ("AT+RST",NULL);   // reset module  
    }  
  }
  
  protected:
  void moduleLost () override {         
    state = Search;
    findModule ();    
  }
  
  
  
  uint8_t configured () override {      
    return (state == Configured);
  }; 
  
  virtual CRemoteXYServer * createServer (uint16_t _port) override {  
    if (!server) {
      server = new CRemoteXYServer_AT (this, _port);
      return server;    
    }
    return NULL;
  } 
  
  public:
  CRemoteXYClient * newClient () override {  
    return new CRemoteXYClient_AT (this);
  }
  
};

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
  


  private:
  uint8_t initModule () {
    state = Init;
    if (sendATCommandForResult ("ATE0",NULL) != AT_RESULT_OK) return 0;   
    if (sendATCommandForResult ("AT+CWMODE=1",NULL) != AT_RESULT_OK) return 0;     
    if (sendATCommandForResult ("AT+CWQAP",NULL) != AT_RESULT_OK) return 0; 
    if (sendATCommandForResult ("AT+CWDHCP=1,1",NULL) != AT_RESULT_OK) return 0; 
    if (sendATCommandForResult ("AT+CIPMODE=0",NULL) != AT_RESULT_OK) return 0;
    if (sendATCommandForResult ("AT+CIPMUX=1",NULL) != AT_RESULT_OK) return 0;
    beginWiFi ();
    return 1;
  }
  
  private:
  void beginWiFi () {
    state = Init;
    setATTimeOut (REMOTEXYCOMM_ESP8266__WIFICONNECT_TIMEOUT);
    if (sendATCommand (REMOTEXYCOMM_ESP8266__ID_WIFICONNECT, "AT+CWJAP=\"",wifiSsid,"\",\"",wifiPassword,"\"",NULL)) state = WaitWiFi;  
  }
  
  
  protected:
  void commandATListener (uint8_t identifier, uint8_t result) override  {
    if (identifier == REMOTEXYCOMM_ESP8266__ID_WIFICONNECT) {
      if (result == AT_RESULT_OK) {
#if defined(REMOTEXY__DEBUGLOG)
        sendATCommandForResult ("AT+CIPSTA?",NULL);
#endif
        state = Configured;
      }
      else {
        state = WaitReconnect; 
        timeOut = millis();
      }
    }  
  }  
    
  virtual void handler () override {   // override CRemoteXYComm_AT  
    CRemoteXYComm_AT::handler ();
    if (state == Reset) {
      if (millis() - timeOut > 5000) {
        state = SearchAfterReset;
        findModule ();
      }
    }
    else if (state == Init) beginWiFi ();
    else if (state == WaitReconnect) {
      if (millis() - timeOut > REMOTEXYCOMM_ESP8266__WIFICONNECT_TIMEOUT) beginWiFi ();
    }
  }
  
  uint8_t handleATMessage () override {    // override CRemoteXYComm_AT  
    if (strcmpReceiveBuffer (AT_MESSAGE_WIFI_DISCONNECT)==0) {
      if (state == Configured) state = Init;
      return 1;
    }
    return 0;
  }
  
  
};       


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
  
  private:
  void initModule () {
    state = Init;
    if (sendATCommandForResult ("ATE0",NULL) != AT_RESULT_OK) return;   
    if (sendATCommandForResult ("AT+CWMODE=2",NULL) != AT_RESULT_OK) return;    
    if (sendATCommandForResult ("AT+CWDHCP=0,1",NULL) != AT_RESULT_OK) return;
    
    char crypt[2] = {*wifiPassword?'4':'0',0};
    setATTimeOut (5000);
    if (sendATCommandForResult ("AT+CWSAP=\"",wifiSsid,"\",\"",wifiPassword,"\",10,",crypt,NULL) != AT_RESULT_OK) return;     
    if (sendATCommandForResult ("AT+CIPMODE=0",NULL) != AT_RESULT_OK) return;
    if (sendATCommandForResult ("AT+CIPMUX=1",NULL) != AT_RESULT_OK) return;
    state = Configured; 
  }
  
  virtual void handler () override {
    CRemoteXYComm_AT::handler ();
    if (state == Reset) {
      if (millis() - timeOut > 5000) {
        state = SearchAfterReset;
        findModule ();
      }
    }
  }
  
};      




#endif // RemoteXYComm_ESP8266_h
