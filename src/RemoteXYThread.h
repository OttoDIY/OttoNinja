// Include necessary header files
#ifndef RemoteXYThread_h
#define RemoteXYThread_h

// Include other required header files
#include "RemoteXYApiData.h"
#include "RemoteXYComm.h"
#include "RemoteXYConnection.h"
#include "RemoteXYWire.h"

// Constants used in the class
#define REMOTEXY_INIT_CRC 0xffff
#define REMOTEXY_PACKAGE_START_BYTE 0x55
#define REMOTEXY_THREAD_TIMEOUT 8000

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
    CRemoteXYThread (CRemoteXYData * _data);

  public:
    // Method to initialize and start the thread
    void begin (CRemoteXYConnection * _conn, CRemoteXYWire * _wire, uint8_t _stopByTimeOut);

    // Method to stop the thread
    void stop ();

    // Method to check if the thread is running
    uint8_t running ();

  public:
    // Overridden method from CRemoteXYReceivePackageListener
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
    static void startThread (CRemoteXYData * data, CRemoteXYConnection * conn,  CRemoteXYWire * wire, uint8_t stopByTimeOut);

    // Static method to get the running thread count
    static uint8_t runningCount (CRemoteXYData * data);

};

// RemoteXYThread_h guard
#endif //RemoteXYThread_h
