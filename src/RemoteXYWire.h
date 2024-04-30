// RemoteXYWire.h
// ----------------
// This is a header file for the CRemoteXYWire class which provides an interface for handling communication with RemoteXY devices.

#ifndef RemoteXYWire_h
#define RemoteXYWire_h

#include "RemoteXYComm.h"

// Define the initial CRC value for the RemoteXY package
#define REMOTEXY_INIT_CRC 0xffff

// Define the start byte for the RemoteXY package
#define REMOTEXY_PACKAGE_START_BYTE 0x55

// Define the minimum length of the RemoteXY package
#define REMOTEXY_PACKAGE_MIN_LENGTH 6

// Define a struct for the RemoteXY package
struct CRemoteXYPackage {
  // The command byte of the package
  uint8_t command;
  // A pointer to the buffer containing the package data
  uint8_t * buffer;
  // The length of the package data
  uint16_t length;
};

// Define an interface for classes that want to receive RemoteXY packages
class CRemoteXYReceivePackageListener {
  public:
  // Constructor
  CRemoteXYReceivePackageListener() : receivePackageListener(nullptr) {}

  // A pure virtual function that must be implemented by classes that inherit from this interface
  // This function is called when a RemoteXY package is received
  virtual void receivePackage(CRemoteXYPackage *package) = 0;

  protected:
  // Pointer to the object that will receive RemoteXY packages
  CRemoteXYReceivePackageListener *receivePackageListener;
};

// Define the RemoteXYWire class
class CRemoteXYWire {

  public:
  // Default constructor
  CRemoteXYWire() : receivePackageListener(nullptr) {}

  // Destructor
  ~CRemoteXYWire() {
    receivePackageListener = nullptr;
  }

  public: 
  // A function to set the object that will receive RemoteXY packages
  void setReceivePackageListener(CRemoteXYReceivePackageListener *listener) {
    // Set the receivePackageListener to the provided listener
    receivePackageListener = listener;
  }

  // A function to clear the object that will receive RemoteXY packages
  void clearReceivePackageListener() {
    receivePackageListener = nullptr;
  }

  // A function to check if the RemoteXYWire class has a receive package listener
  bool hasReceivePackageListener() {
    return receivePackageListener != nullptr;
  }

  public:
  // A function to notify the receivePackageListener that a package has been received
  void notifyReceivePackageListener(CRemoteXYPackage *package) {
    // If the receivePackageListener is not NULL, call its receivePackage function
    if (receivePackageListener) receivePackageListener->receivePackage(package);
  } 

  public:
  // A pure virtual function that must be implemented by classes that inherit from this class
  // This function is responsible for handling incoming data
  virtual void handler() {
    if (hasReceivePackageListener() && packageReceived) {
      notifyReceivePackageListener(&package);
      packageReceived = false;
    }
  }

  // A function to check if the RemoteXYWire class is currently running
  virtual uint8_t running() {
    // Implementation not provided in the original code
  }

  // A function to stop the RemoteXYWire class
  virtual void stop() {
    // Implementation not provided in the original code
  }

  // A function to send a RemoteXY package
  virtual void sendPackage(uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) {
    // Implementation not provided in the original code
  }

  // A function to set the packageReceived flag
  void setPackageReceived(bool value) {
    packageReceived = value;
  }

  // A function to get the packageReceived flag
  bool getPackageReceived() {
    return packageReceived;
  }

  protected:
  // Pointer to the object that will receive RemoteXY packages
  CRemoteXYReceivePackageListener *receivePackageListener;

  // Package received flag
  bool packageReceived = false;

  // Package data
  CRemoteXYPackage package;

};


#endif  //  RemoteXYWire_h
