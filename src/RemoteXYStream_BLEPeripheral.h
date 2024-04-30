// Include guard to prevent multiple inclusions of this header file
#ifndef REMOTEXY_STREAM_BLE_PERIPHERAL_H
#define REMOTEXY_STREAM_BLE_PERIPHERAL_H

// If the _BLE_PERIPHERAL_H_ macro is defined, include the RemoteXYStream.h header
#if defined(_BLE_PERIPHERAL_H_)

//------------------------------------------------------------------------------
// Constants and macros for buffer sizes, UUIDs, and timeouts
//------------------------------------------------------------------------------

// ... (Constants and macros for buffer sizes, UUIDs, and timeouts)

//------------------------------------------------------------------------------
// CRemoteXYStreamBLEPeripheral class definition
//------------------------------------------------------------------------------
class CRemoteXYStreamBLEPeripheral : public CRemoteXYStream, public BLEPeripheral {
  private:
    //--------------------------------------------------------------------------
    // Member variables for BLE service, characteristics, and descriptors
    //--------------------------------------------------------------------------

    // ...

    // Other private member variables, functions, and class definitions
};

#endif // defined(_BLE_PERIPHERAL_H_)

#endif // REMOTEXY_STREAM_BLE_PERIPHERAL_H
