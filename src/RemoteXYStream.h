#ifndef RemoteXYStream_h
#define RemoteXYStream_h

#include <inttypes.h>
#include "RemoteXYDebugLog.h"

/// The CRemoteXYStream class provides a base class for implementing
/// stream classes that can be used for communication between a
/// RemoteXY server and client.
class CRemoteXYStream {
  private:
    /// The listener object that will be notified when a byte is read.
    CRemoteXYReadByteListener* readByteListener;

  public:
    /// Constructs a new CRemoteXYStream object.
    CRemoteXYStream() : readByteListener(nullptr) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
#endif
    }

    /// Destructor for the CRemoteXYStream class.
    virtual ~CRemoteXYStream() {}

    /// Sets the read byte listener object.
    /// @param listener The listener object to set.
    void setReadByteListener(CRemoteXYReadByteListener* listener) {
      readByteListener = listener;
    }

    /// Notifies the read byte listener object that a byte has been read.
    /// @param byte The byte that was read.
    void notifyReadByteListener(uint8_t byte) {
      if (readByteListener) readByteListener->readByte(byte);
    }

    /// The handler method should be implemented by derived classes to
    /// handle incoming data.
    virtual void handler() {}

    /// Starts writing data to the stream.
    /// @param size The size of the data to be written.
    virtual void startWrite(uint16_t size) {
      // Intentionally left blank.
      (void)size;
    }

    /// Writes a byte to the stream.
    /// @param byte The byte to write.
    virtual void write(uint8_t byte) {
      // Intentionally left blank.
      (void)byte;
    }
};

#endif //RemoteXYStream_h
