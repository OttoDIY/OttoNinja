#ifndef RemoteXYWireCloud_h
#define RemoteXYWireCloud_h

#include "RemoteXYWire.h"

/**
 * Free ID value for the cloud wire.
 */
#define REMOTEXYWIRECLOUD_FREE_ID 0xff

/**
 * Interface for sending packages to the cloud wire.
 */
class CRemoteXYSendPackageListener {
  public:
    /**
     * Sends a package to the cloud wire.
     * @param command The command byte of the package.
     * @param buf The data buffer of the package.
     * @param length The length of the data buffer.
     * @param fromPgm Whether the data buffer is in program memory.
     */
    virtual void sendPackage(uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) = 0;
};

/**
 * A cloud-based implementation of the RemoteXY wire.
 */
class CRemoteXYWireCloud : public CRemoteXYWire {
  public:
    /**
     * Constructs a new cloud wire object with the given package listener.
     * @param _sendPackageListener The package listener to use for sending packages.
     */
    CRemoteXYWireCloud(CRemoteXYSendPackageListener *_sendPackageListener) : CRemoteXYWire(), sendPackageListener(_sendPackageListener), id(REMOTEXYWIRECLOUD_FREE_ID), newConnection(0) {}

    /**
     * Initializes the cloud wire with the given ID.
     * @param _id The ID to use for the cloud wire.
     */
    void init(uint8_t _id) {
      id = _id;
      newConnection = 1;
    }

    /**
     * Stops the cloud wire and releases its resources.
     */
    virtual ~CRemoteXYWireCloud() {}

    /**
     * Stops the cloud wire and releases its resources.
     */
    void stop() {
      setReceivePackageListener(NULL);
      id = REMOTEXYWIRECLOUD_FREE_ID;
      newConnection = 0;
    }

    /**
     * Checks whether the cloud wire is running.
     * @return True if the cloud wire is running, false otherwise.
     */
    uint8_t running() {
      return id != REMOTEXYWIRECLOUD_FREE_ID;
    }

    /**
     * Checks whether there is a new connection for the cloud wire.
     * @return True if there is a new connection, false otherwise.
     */
    uint8_t isNewConnection() {
      return newConnection;
    }

    /**
     * Sends a package to the cloud wire.
     * @param command The command byte of the package.
     * @param buf The data buffer of the package.
     * @param length The length of the data buffer.
     * @param fromPgm Whether the data buffer is in program memory.
     */
    void sendPackage(uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) override {
      sendPackageListener->sendPackage(command | (id << 1), buf, length, fromPgm);
    }

    /**
     * Receives a package from the cloud wire.
     * @param package The package to receive.
     */
    void receivePackage(CRemoteXYPackage *package) {
      sendPackageListener->receivePackage(package);
    }

  private:
    CRemoteXYSendPackageListener *sendPackageListener;
    uint8_t id;
    uint8_t newConnection;
};

/**
 * Represents a package of data to be sent or received over the cloud wire.
 */
class CRemoteXYPackage {
  public:
    /**
     * Constructs a new package object with the given parameters.
     * @param command The command byte of the package.
     * @param buf The data buffer of the package.
     * @param length The length of the data buffer.
     * @param fromPgm Whether the data buffer is in program memory.
     */
    CRemoteXYPackage(uint8_t command, uint8_t *buf, uint16_t length, uint8_t fromPgm) : command(command), buf(buf), length(length), fromPgm(fromPgm) {}

    /**
     * Destructor for the package object.
     */
    virtual ~CRemoteXYPackage() {}

    /**
     * Creates a copy of the package.
     * @return A copy of the package.
     */
    virtual CRemoteXYPackage *clone() const = 0;

    /**
     * Gets the command byte of the package.
     * @return The command byte of the package.
     */
    uint8_t getCommand() const {
      return command;
    }

    /**
     * Gets the data buffer of the package.
     * @return The data buffer of the package.
     */
    const uint8_t *getData() const {
      return buf;
    }

    /**
     * Gets the length of the data buffer.
     * @return The length of the data buffer.
     */
    uint16_t getLength() const {
      return length;
    }

    /**
     * Checks whether the data buffer is in program memory.
     * @return True if the data buffer is in program memory, false otherwise.
     */
    uint8_t isFromPgm() const {
      return fromPgm;
    }

  private:
    uint8_t command;
    const uint8_t *buf;
    uint16_t length;
    uint8_t fromPgm;
};

#endif //RemoteXYWireCloud_h
