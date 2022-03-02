#ifndef RemoteXYWireStream_h
#define RemoteXYWireStream_h


#include "RemoteXYWire.h"
#include "RemoteXYApiData.h"


class CRemoteXYWireStream : public CRemoteXYWire, public CRemoteXYReadByteListener {

  public:
  CRemoteXYWireStream * next;  
  CRemoteXYStream * stream; 
  
  private: 
  uint16_t sendCRC;
 
  uint8_t *receiveBuffer;
  uint16_t receiveBufferSize;
  uint16_t receiveIndex;
  uint8_t receiveModified;
  volatile uint8_t receiveLock;  // =1 only add to receive buffer
    
  
  public:
  CRemoteXYWireStream (CRemoteXYData * data) : CRemoteXYWire () {
    stream = NULL;
    receiveBufferSize = data->receiveBufferSize;
    receiveBuffer = (uint8_t*)malloc (receiveBufferSize);    
  }

  public:
  CRemoteXYWireStream (CRemoteXYData * data, uint8_t multiple) : CRemoteXYWire () {
    stream = NULL;
    receiveBufferSize = data->receiveBufferSize * multiple;
    receiveBuffer = (uint8_t*)malloc (receiveBufferSize);    
  }  
  
  public:
  void begin (CRemoteXYStream * _stream) {    
    stream = _stream;
    stream->setReadByteListener (this);  
    receiveIndex = 0;  
    receiveModified = 0;
    receiveLock = 0;
  }   
  
  public:    
  void stop () override {
    setReceivePackageListener (NULL);
    stream = NULL;
  }
  
  public:
  uint8_t running () override {   
    if (stream) return 1;
    else return 0;
  }

  CRemoteXYClient * getClient () {
    return (CRemoteXYClient*)stream;
  }

  
  
  public:
  void handler () override {
    if (stream) {
      stream->handler ();
      receivePackage ();
    }
  }


  private:
  void updateCRC (uint16_t *crc, uint8_t b) {
    *crc ^= b;
    for (uint8_t i=0; i<8; ++i) {
      if ((*crc) & 1) *crc = ((*crc) >> 1) ^ 0xA001;
      else *crc >>= 1;
    }
  } 
  
  
  private:  
  inline void sendByteUpdateCRC (uint8_t b) {
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.writeOutputHex (b);  
#endif 
    stream->write (b);
    updateCRC (&sendCRC, b);   
  } 

  
  public:
  void sendPackage (uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) {
    uint16_t packageLength;
    if (stream) {
      sendCRC = REMOTEXY_INIT_CRC;
      packageLength = length+6;    
      stream->startWrite (packageLength);
            
      sendByteUpdateCRC (REMOTEXY_PACKAGE_START_BYTE);
      sendByteUpdateCRC (packageLength);
      sendByteUpdateCRC (packageLength>>8);
      sendByteUpdateCRC (command);  
      uint8_t b;
      while (length--) {
        if (fromPgm) b=pgm_read_byte_near (buf++);
        else b=*buf++;
        sendByteUpdateCRC (b);
      }
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.writeOutputHex (sendCRC);  
      RemoteXYDebugLog.writeOutputHex (sendCRC>>8);  
#endif  
      stream->write (sendCRC);
      stream->write (sendCRC>>8);  
    }
  }

  public:
  void readByte (uint8_t byte) override {
    uint16_t pi, i;
       
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.writeInputHex (byte);  
#endif   
    if ((receiveIndex==0) && (byte!=REMOTEXY_PACKAGE_START_BYTE)) return; 
    if (receiveIndex >= receiveBufferSize) {
      if (receiveLock) return;      
      pi = 1;
      while (pi < receiveBufferSize) {
        if (receiveBuffer[pi] == REMOTEXY_PACKAGE_START_BYTE) break;
        pi++;
      }      
      receiveIndex = receiveBufferSize - pi;
      i=0;
      while (pi < receiveBufferSize) receiveBuffer[i++] = receiveBuffer[pi++];       
    }
    receiveBuffer[receiveIndex++]=byte;  
    receiveModified = 1; 
  }
  
  private:
  void receivePackage () {
    if (receiveModified) {
      receiveModified = 0;
      
      uint16_t crc; 
      uint16_t si, i;
      uint16_t packageLength;
      si = 0;
      while (si + REMOTEXY_PACKAGE_MIN_LENGTH <= receiveIndex) {   
        if (receiveBuffer[si] == REMOTEXY_PACKAGE_START_BYTE) {
          packageLength = receiveBuffer[si+1]|(receiveBuffer[si+2]<<8);
          if ((packageLength <= receiveIndex - si) && (packageLength >=6)) {
            crc=REMOTEXY_INIT_CRC;
            for (i = si; i < si + packageLength; i++) updateCRC (&crc, receiveBuffer[i]); 
            if (crc == 0) {
              CRemoteXYPackage package;
              package.command = receiveBuffer[si+3];
              package.buffer = receiveBuffer+si+4;
              package.length = packageLength-6;

              receiveLock = 1;
              notifyReceivePackageListener (&package);   
              si += packageLength;
              i = 0;
              while (si < receiveIndex) receiveBuffer[i++] = receiveBuffer[si++];
              receiveIndex = i;
              receiveLock = 0;
              si = 0;
              continue;
            }
          }
        }
        si++; 
      }
    }   
  }
  
};


#endif  //  RemoteXYWireStream_h