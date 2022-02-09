#ifndef RemoteXYThread_h
#define RemoteXYThread_h


#include "RemoteXYApiData.h"
#include "RemoteXYComm.h"
#include "RemoteXYConnection.h"
#include "RemoteXYWire.h"


#define REMOTEXY_INIT_CRC 0xffff
#define REMOTEXY_PACKAGE_START_BYTE 0x55
#define REMOTEXY_THREAD_TIMEOUT 8000


class CRemoteXYThread : public CRemoteXYReceivePackageListener {

  public:
  CRemoteXYThread * next;
  
  private:
  CRemoteXYData* data;
  CRemoteXYConnection* conn;
  CRemoteXYWire * wire;  
  
  uint8_t *inputVar;  
  
  uint32_t timeOut;
  uint8_t stopByTimeOut;
  
  uint8_t inputVarNeedSend;
    
  
  public:
  uint8_t connect_flag;

  public:
  CRemoteXYThread (CRemoteXYData * _data) {
  
    data = _data;    
    wire = NULL;

    inputVar = (uint8_t*)malloc (data->inputLength); 
    copyInputVars ();
  }
  


  public:
  void begin (CRemoteXYConnection * _conn, CRemoteXYWire * _wire, uint8_t _stopByTimeOut) {
    conn = _conn;
    wire = _wire;         
    wire->setReceivePackageListener (this);
    stopByTimeOut = _stopByTimeOut;
    timeOut = millis ();
    connect_flag = 0;

#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Client started");
    //RemoteXYDebugLog.writeAvailableMemory ();
#endif
  }  
  
  
  public:
  void stop () {
    if (wire) {
      conn->stopThreadListener (wire);
      wire = NULL;
      connect_flag = 0;
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("Client stoped");
#endif 
    }
  }  
  
  public:
  uint8_t running () {
    if (wire) return 1;
    return 0;
  }


  public:
  void receivePackage (CRemoteXYPackage * package) override {
    uint16_t i, length;
    uint8_t *p, *kp, *ip; 
    
    if (wire == NULL) return;  
    if ((package->command != 0x00) && (!connect_flag)) return;
    switch (package->command) {  
      case 0x00:  
        uint8_t available;
        if (package->length==0) {
          if (*data->accessPassword==0) available=1;
          else available=0;
        }
        else {
          uint8_t ch;
          available = 1;
          p = package->buffer;
          kp = data->accessPassword; 
          while (true) {
            ch=*kp++;
            if (ch!=*p++) available=0;
            if (!ch) break;
          }                               
        } 
        if (available!=0) {
          wire->sendPackage (0x00, data->conf, data->confLength,  1);
          connect_flag = 1;
        }
        else {
          uint8_t buf[4];
          p = buf;
          kp = data->conf;         
          i=data->confVersion>=5?3:2;
          length = i+1;
          while (i--) *p++ = data->getConfByte(kp++);
          *p++ = 0xf0;
          wire->sendPackage (0x00, buf, length,  0);
        }          
        break;   
      case 0x40:  
        copyInputVars ();
        wire->sendPackage (0x40, data->var, data->inputLength + data->outputLength, 0); 
        break;   
      case 0x80:  
        checkInputVars ();
        if ((package->length == data->inputLength) && (inputVarNeedSend==0)) {
          p=package->buffer;
          kp=data->var;
          ip=inputVar;
          i= data->inputLength;
          while (i--) *ip++=*kp++=*p++;
        }
        wire->sendPackage (0x80, 0, 0, 0);
        break;   
      case 0xC0: 
        checkInputVars ();
        uint8_t c;
        if (inputVarNeedSend==0) c = 0xC0; 
        else c = 0xC1; 
        wire->sendPackage (c, data->var + data->inputLength, data->outputLength, 0);
        break;        
    }  
    timeOut = millis ();  
  }
  
  private:
  void copyInputVars () {
    inputVarNeedSend = 0;
    uint8_t * pc = data->var;
    uint8_t * p = inputVar;
    uint16_t ilen = data->inputLength;
    while (ilen--) *p++ = *pc++;
  }

  private:
  void checkInputVars () {
    if (inputVarNeedSend) return;
    uint8_t * pc = data->var;
    uint8_t * p = inputVar;
    uint16_t ilen = data->inputLength;
    while (ilen--) {
      if (*p++ != *pc++) {
        inputVarNeedSend = 1;
        break;
      }
    }
  }

  
  
  public:
  void handler () {
    if (wire) {       
      conn->handleWire (wire);
      if (wire->running ()) {  
        if (millis () - timeOut > REMOTEXY_THREAD_TIMEOUT) {
          timeOut = millis ();
          connect_flag = 0;
          if (stopByTimeOut) stop (); 
        }     
      }  
      else stop ();
    }
  }
  
  public:
  static void startThread (CRemoteXYData * data, CRemoteXYConnection * conn,  CRemoteXYWire * wire, uint8_t stopByTimeOut) {    
    CRemoteXYThread * pt = data->threads;            
    while (pt) {
      if (!pt->running ()) {
        pt->begin (conn, wire, stopByTimeOut);
        return;
      }
      pt = pt->next;
    }
    pt = new CRemoteXYThread (data);
    pt->next = data->threads;
    data->threads = pt;
    pt->begin (conn, wire, stopByTimeOut);  
  }
  
  public:
  static uint8_t runningCount (CRemoteXYData * data) {
    uint8_t c = 0;
    CRemoteXYThread * pt = data->threads;            
    while (pt) {
      if (pt->running ()) c++;
      pt = pt->next;
    } 
    return c; 
  }

};


#endif //RemoteXYThread_h