#ifndef RemoteXYWireStream_h
#define RemoteXYWireStream_h

// Include necessary header files
#include "RemoteXYWire.h"
#include "RemoteXYApiData.h"

// Define the class CRemoteXYWireStream
class CRemoteXYWireStream : public CRemoteXYWire, public CRemoteXYReadByteListener {
  public:
    // Pointer to the next CRemoteXYWireStream object
    CRemoteXYWireStream * next;  
    // Pointer to the CRemoteXYStream object
    CRemoteXYStream * stream; 

  private: 
    // CRC value for sent data
    uint16_t sendCRC;
    // Buffer to store received data
    uint8_t *receiveBuffer;
    // Size of the receive buffer
    uint16_t receiveBufferSize;
    // Index to keep track of the current position in the receive buffer
    uint16_t receiveIndex;
    // Flag to indicate if the receive buffer has been modified
    uint8_t receiveModified;
    // Lock to prevent concurrent access to the receive buffer
    volatile uint8_t receiveLock;  // =1 only add to receive buffer

  public:
    // Constructor for CRemoteXYWireStream class with one parameter
    CRemoteXYWireStream (CRemoteXYData * data) : CRemoteXYWire () {
      stream = NULL;
      receiveBufferSize = data->receiveBufferSize;
      receiveBuffer = (uint8_t*)malloc (receiveBufferSize);    
    }

    // Constructor for CRemoteXYWireStream class with two parameters
    CRemoteXYWireStream (CRemoteXYData * data, uint8_t multiple) : CRemoteXYWire () {
      stream = NULL;
      receiveBufferSize = data->receiveBufferSize * multiple;
      receiveBuffer = (uint8_t*)malloc (receiveBufferSize);    
    }  

    // Method to initialize the stream
    public:
    void begin (CRemoteXYStream * _stream) {    
      stream = _stream;
      stream->setReadByteListener (this);  
      receiveIndex = 0;  
      receiveModified = 0;
      receiveLock = 0;
    }   

    // Method to stop the stream
    public:    
    void stop () override {
      setReceivePackageListener (NULL);
      stream = NULL;
    }

    // Method to check if the stream is running
    public:
    uint8_t running () override {   
      if (stream) return 1;
      else return 0;
    }

    // Method to get the client associated with the stream
    CRemoteXYClient * getClient () {
      return (CRemoteXYClient*)stream;
    }

    // Method to handle data transmission and reception
    public:
    void handler () override {
      if (stream) {
        stream->handler ();
        receivePackage ();
      }
    }

  private:
    // Method to update the CRC value
    void updateCRC (uint16_t *crc, uint8_t b) {
      *crc ^= b;
      for (uint8_t i=0; i<8; ++i) {
        if ((*crc) & 1) *crc = ((*crc) >> 1) ^ 0xA001;
        else *crc >>= 1;
      }
    } 

  private:  
    // Method to send data with updated CRC value
    inline void sendByteUpdateCRC (uint8_t b) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.writeOutputHex (b);  
#endif 
      stream->write (b);
      updateCRC (&sendCRC, b);   
    } 

  public:
    // Method to send a package of data with specified command, buffer, length, and fromPgm flag
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

    // Method to read a byte of data and store it in the receive buffer
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
    // Method to process received data and call the receivePackageListener with the received package
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
