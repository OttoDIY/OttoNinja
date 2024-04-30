// Include guard to prevent multiple inclusions of this header file
#ifndef RemoteXYStream_BLEPeripheral_h
#define RemoteXYStream_BLEPeripheral_h

// If the _BLE_PERIPHERAL_H_ macro is defined, include the RemoteXYStream.h header
#if defined(_BLE_PERIPHERAL_H_) 

//------------------------------------------------------------------------------
// Constants and macros for buffer sizes, UUIDs, and timeouts
//------------------------------------------------------------------------------

// ... (Constants and macros for buffer sizes, UUIDs, and timeouts)

//------------------------------------------------------------------------------
// CRemoteXYStream_BLEPeripheral class definition
//------------------------------------------------------------------------------
class CRemoteXYStream_BLEPeripheral : public CRemoteXYStream, public BLEPeripheral {
  private:
    //--------------------------------------------------------------------------
    // Member variables for BLE service, characteristics, and descriptors
    //------------------------------------------------
