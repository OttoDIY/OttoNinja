// Include guard to prevent multiple inclusions of this header file
#ifndef RemoteXYStream_BLEPeripheral_h
#define RemoteXYStream_BLEPeripheral_h

// If the _BLE_PERIPHERAL_H_ macro is defined, include the RemoteXYStream.h header
#if defined(_BLE_PERIPHERAL_H_) 

// Constants and macros for buffer sizes, UUIDs, and timeouts
// ...

// CRemoteXYStream_BLEPeripheral class definition
class CRemoteXYStream_BLEPeripheral : public CRemoteXYStream, public BLEPeripheral {
  private:
    // Member variables for BLE service, characteristics, and descriptors
    // ...

    // Buffers for sending and receiving data
    // ...

  public:
    // Constructor for CRemoteXYStream_BLEPeripheral
    CRemoteXYStream_BLEPeripheral (const char * _bleDeviceName);

    // Overridden methods from BLEPeripheral
    void BLEDeviceConnected(BLEDevice& device, const unsigned char* address) override;
    void BLEDeviceDisconnected(BLEDevice& device) override;
    void BLEDeviceCharacteristicValueChanged (BLEDevice& device, BLECharacteristic& characteristic, const unsigned char* data, unsigned char size) override;

    // Methods for handling data transmission and reception
    // ...

    // Overridden method from CRemoteXYStream
    void handler () override;
};

// Close the include guard
#endif  // _BLE_PERIPHERAL_H_

#endif //RemoteXYStream_BLEPeripheral_h
