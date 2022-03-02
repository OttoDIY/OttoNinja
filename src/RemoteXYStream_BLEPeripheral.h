#ifndef RemoteXYStream_BLEPeripheral_h
#define RemoteXYStream_BLEPeripheral_h
                          
#if defined(_BLE_PERIPHERAL_H_) 

#include "RemoteXYStream.h"

#define REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE BLE_ATTRIBUTE_MAX_VALUE_LENGTH
#define REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE 1024
#define REMOTEXYCOMM_BLEPERIPHERAL__SERVICE_UUID        "0000FFE0-0000-1000-8000-00805F9B34FB" // UART service UUID
#define REMOTEXYCOMM_BLEPERIPHERAL__CHARACTERISTIC_UUID "0000FFE1-0000-1000-8000-00805F9B34FB"    
//#define REMOTEXYCOMM_BLEPERIPHERAL__DESCRIPTOR_UUID     "2902"    

#define REMOTEXYCOMM_BLEPERIPHERAL__CANNOTIFY_TIMEOUT 1000    

class CRemoteXYStream_BLEPeripheral : public CRemoteXYStream, public BLEPeripheral {

  private:
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
  BLEDescriptor *pDescriptor;
  
  uint8_t sendBuffer[REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE];
  uint16_t sendBufferCount;
  uint16_t sendBytesAvailable;

  uint8_t receiveBuffer[REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE];
  uint16_t receiveBufferStart;
  uint16_t receiveBufferPos;
  uint16_t receiveBufferCount;
  
  
  public:
  CRemoteXYStream_BLEPeripheral (const char * _bleDeviceName) : CRemoteXYStream (), BLEPeripheral () { 
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("Init NRF BLE on chip");
#endif

    receiveBufferCount = 0;        
    receiveBufferStart = 0;
    receiveBufferPos = 0;
    receiveBufferCount = 0;  
    
    pService = new BLEService (REMOTEXYCOMM_BLEPERIPHERAL__SERVICE_UUID);    
    pCharacteristic = new BLECharacteristic( REMOTEXYCOMM_BLEPERIPHERAL__CHARACTERISTIC_UUID, BLERead | BLEWriteWithoutResponse | BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    //pDescriptor = new BLEDescriptor(REMOTEXYCOMM_BLEPERIPHERAL__DESCRIPTOR_UUID);
    
    setLocalName(_bleDeviceName);
    setDeviceName(_bleDeviceName);
    setAdvertisedServiceUuid (pService->uuid());
    setAppearance(0x0080);
    
    addAttribute (*pService);
    addAttribute (*pCharacteristic);
    //addAttribute (*pDescriptor);

    pCharacteristic->setValue(0);
    begin();
    
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("BLE started");
#endif  

  }              
  
  
  void BLEDeviceConnected(BLEDevice& device, const unsigned char* address) override {
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("BLE client connected");
#endif
    receiveBufferStart = 0;
    receiveBufferPos = 0;
    receiveBufferCount = 0;  
  }
  
  void BLEDeviceDisconnected(BLEDevice& device) {
#if defined(REMOTEXY__DEBUGLOG)
    RemoteXYDebugLog.write("BLE client disconnected");
#endif
    receiveBufferCount = 0;
  }

  void BLEDeviceCharacteristicValueChanged (BLEDevice& device, BLECharacteristic& characteristic, const unsigned char* data, unsigned char size) override {

      for (uint16_t i = 0; i < size; i++) {                
        receiveBuffer[receiveBufferPos++] =  data[i];
        if (receiveBufferPos >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferPos=0; 
        if (receiveBufferCount < REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferCount++;
        else {
          receiveBufferStart++;
          if (receiveBufferStart >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferStart=0;
        }       
      } 

  }

    
  void startWrite (uint16_t len) override {
    sendBytesAvailable = len;
    sendBufferCount = 0;
  }    
    
  void write (uint8_t b) override {
    if (pCharacteristic->subscribed() == false) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.write("BLE: tx characteristic not subscribed");
#endif
      return;
    }
    sendBuffer[sendBufferCount++] = b;
    sendBytesAvailable--;
    if ((sendBufferCount == REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE) || (sendBytesAvailable == 0)) {
      uint32_t t = millis ();
      while (pCharacteristic->canNotify () == 0) {
        if (millis () - t > REMOTEXYCOMM_BLEPERIPHERAL__CANNOTIFY_TIMEOUT) break;
        poll();       
      }     
      pCharacteristic->setValue(sendBuffer, sendBufferCount);  
      poll();       
      sendBufferCount = 0;
      
    }
  }     

  void handler () override { 
    uint8_t b;  
    BLEPeripheral::poll();
    while (receiveBufferCount > 0) {     
      b =  receiveBuffer[receiveBufferStart++];
      if (receiveBufferStart >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferStart=0;
      receiveBufferCount--;
      notifyReadByteListener (b);
    }
  } 
};





#endif  // _BLE_PERIPHERAL_H_

#endif //RemoteXYStream_BLEPeripheral_h