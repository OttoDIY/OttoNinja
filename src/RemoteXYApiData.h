#ifndef RemoteXYApiData_h
#define RemoteXYApiData_h  

#include <inttypes.h> 


#define REMOTEXY_PASSWORD_LENGTH_MAX 26

class CRemoteXYThread;
class CRemoteXYComm;
class CRemoteXYConnectionComm;


class CRemoteXYData {

  public:
  uint8_t confVersion;
  uint8_t *conf;
  uint8_t *var;
  uint8_t *accessPassword;
  uint16_t outputLength;
  uint16_t inputLength;
  uint16_t confLength;
  uint8_t *connect_flag;

  uint16_t receiveBufferSize;  
  
  CRemoteXYThread * threads;  
  CRemoteXYComm * comms;    
  CRemoteXYConnectionComm * connections;    


  public:
  void init (const void * _conf, void * _var, const char * _accessPassword) {
  
    uint8_t i;
    uint8_t* p = (uint8_t*)_conf;
    uint8_t b = getConfByte (p++);

    if (b==0xff) {
      inputLength = getConfByte (p++);
      inputLength |= getConfByte (p++)<<8;
      outputLength = getConfByte (p++);
      outputLength |= getConfByte (p++)<<8;
    }
    else {
      inputLength = b;
      outputLength = getConfByte (p++);    
    }
    confLength = getConfByte (p++);
    confLength |= getConfByte (p++)<<8;
    conf = p;
    confVersion = getConfByte (p);
    var = (uint8_t*)_var;
    uint16_t varLength = outputLength + inputLength;
    connect_flag = var + varLength;
    *connect_flag = 0;   
        
    accessPassword = (uint8_t*)_accessPassword;          
    
    p = var;
    i = varLength;
    while (i--) *p++=0;    
    
    receiveBufferSize = inputLength;
    if ((*accessPassword != 0) && (receiveBufferSize < REMOTEXY_PASSWORD_LENGTH_MAX)) receiveBufferSize = REMOTEXY_PASSWORD_LENGTH_MAX;
    receiveBufferSize +=6;   
    
    comms = NULL;
    connections = NULL;    
    threads = NULL;
  }
  
  public:
  inline uint8_t getConfByte (uint8_t* p) {
    return pgm_read_byte_near (p);                                     
  }
  
  
};




#endif //RemoteXYData_h