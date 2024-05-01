// RemoteXYThread.h: Header file for the CRemoteXYThread class
// This class is responsible for managing a thread that handles RemoteXY
// communication and input variables.

// Include necessary header files
#ifndef RemoteXYThread_h
#define RemoteXYThread_h

// Include other required header files
#include "RemoteXYApiData.h"        // For CRemoteXYApiData class
#include "RemoteXYComm.h"           // For CRemoteXYComm class
#include "RemoteXYConnection.h"     // For CRemoteXYConnection class
#include "RemoteXYWire.h"            // For CRemoteXYWire class

// Constants used in the class
#define REMOTEXY_INIT_CRC 0xffff    // Initial CRC value
#define REMOTEXY_PACKAGE_START_BYTE 0x55  // Start byte for a RemoteXY package
#define REMOTEXY_THREAD_TIMEOUT 8000  // Timeout for the RemoteXY thread

// Class definition: CRemoteXYThread
class CRemoteXYThread : public CRemoteXYReceivePackageListener {
  public:
    // Pointer to the next thread object
    CRemoteXYThread * next;

  private:
    // Pointer to the data object
    CRemoteXYData* data;

    // Pointer to the connection object
    CRemoteXYConnection* conn;

    // Pointer to the wire object
    CRemoteXYWire * wire;  

    // Input variables
    uint8_t *inputVar;  

    // Timeout-related variables
    uint32_t timeOut;
    uint8_t stopByTimeOut;

    // Input variables flag
    uint8_t inputVarNeedSend;

    // Connection flag
    uint8_t connect_flag;

  public:
    // Constructor
    // Parameters:
    //   _data: Pointer to the CRemoteXYData object
    CRemoteXYThread (CRemoteXYData * _data);

  public:
    // Method to initialize and start the thread
    // Parameters:
    //   _conn: Pointer to the CRemoteXYConnection object
    //   _wire: Pointer to the CRemoteXYWire object
    //   _stopByTimeOut: Stop the thread by timeout (1) or not (0)
    void begin (CRemoteXYConnection * _conn, CRemoteXYWire * _wire, uint8_t _stopByTimeOut);

    // Method to stop the thread
    void stop ();

    // Method to check if the thread is running
    // Returns:
    //   1 if the thread is running, 0 otherwise
    uint8_t running ();

  public:
    // Overridden method from CRemoteXYReceivePackageListener
    // Parameters:
    //   package: Pointer to the received RemoteXY package
    void receivePackage (CRemoteXYPackage * package) override;

  private:
    // Method to copy input variables
    void copyInputVars ();

    // Method to check input variables
    void checkInputVars ();

  public:
    // Method to handle the thread
    void handler ();

  public:
    // Static method to start a thread
    // Parameters:
    //   data: Pointer to the CRemoteXYData object
    //   conn: Pointer to the CRemoteXYConnection object
    //   wire: Pointer to the CRemoteXYWire object
    //   stopByTimeOut: Stop the thread by timeout (1) or not (0)
    static void startThread (CRemoteXYData * data, CRemoteXYConnection * conn,  CRemoteXYWire * wire, uint8_t stopByTimeOut);

    // Static method to get the running thread count
    // Parameters:
    //   data: Pointer to the CRemoteXYData object
    // Returns:
    //   The number of running threads
    static uint8_t runningCount (CRemoteXYData * data);

};

// RemoteXYThread_h guard
#endif //RemoteXYThread_h
