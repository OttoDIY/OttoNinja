#ifndef RemoteXYComm_h
#define RemoteXYComm_h  

#include <inttypes.h> 
#include "RemoteXYDebugLog.h"
#include "RemoteXYStream.h"


#define UNUSED(x) (void)(x)

class CRemoteXYClientAvailableListener {
  public:
  virtual void clientAvailable () = 0;
};



class CRemoteXYClient: public CRemoteXYStream {
  public:
  CRemoteXYClient * next;
  
  public:
  CRemoteXYClient (): CRemoteXYStream() {
  }
    
  public: 
  virtual uint8_t connect (const char *host, uint16_t port) {UNUSED (host); UNUSED (port); return 0;};  
  virtual void stop () {};  
  virtual uint8_t connected () {return 1;};  
  virtual uint8_t equal (CRemoteXYClient * cl) {UNUSED (cl); return 0;} 
};


class CRemoteXYServer {

  private:
  CRemoteXYClientAvailableListener * clientAvailableListener;
  
  public:
  void setClientAvailabListener (CRemoteXYClientAvailableListener * listener) {
    clientAvailableListener = listener;
  }    
  
  public:
  void notifyClientAvailableListener () {
    if (clientAvailableListener) clientAvailableListener->clientAvailable ();
  }     
  
   
  public:     
  virtual uint8_t begin () {return 0;} 
  virtual void stop () {}; 
  virtual uint8_t available (CRemoteXYClient * client) {UNUSED (client); return 0;}    

};



class CRemoteXYComm { 
  public:
  CRemoteXYComm * next;

  public:
  CRemoteXYComm () {
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.init ();
#endif
  }
  
  public:
  virtual void handler () {}; 
  virtual uint8_t configured () {return 1;};                 
  virtual CRemoteXYServer * createServer (uint16_t _port) {UNUSED (_port); return NULL;}  
  virtual CRemoteXYClient * newClient () {return NULL;}
};
 


#endif //RemoteXYComm_h