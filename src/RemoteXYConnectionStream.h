#ifndef RemoteXYConnectionStream_h
#define RemoteXYConnectionStream_h

#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"

class CRemoteXYConnectionStream: public CRemoteXYConnection { 
  CRemoteXYStream * stream;
    
  public: 
  CRemoteXYConnectionStream (CRemoteXYStream * _stream) {
    stream = _stream;
  }    
  
  void init (CRemoteXYData * _data) override {
    CRemoteXYWireStream * wire = new CRemoteXYWireStream (_data);
    wire->begin (stream);  
    CRemoteXYThread::startThread (_data, this, wire, 0);
  };
  
  void handleWire (CRemoteXYWire * wire) override {
    wire->handler ();
  }; 
  
};

#endif // CRemoteXYConnectionStream