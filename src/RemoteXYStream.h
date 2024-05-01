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
    /// This is a pointer to an instance of CRemoteXYReadByteListener,
    /// which will receive notifications when a byte is read from the stream.
    CRemoteXYReadByteListener* readByteListener;

  public:
    /// Constructs a new CRemoteXYStream object.
    /// The constructor initializes the readByteListener member variable
    /// to nullptr, indicating that no listener has been set yet.
    /// It also initializes the RemoteXYDebugLog object if REMOTEXY__DEBUGLOG
    /// is defined.
    CRemoteXYStream() : readByteListener(nullptr) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
#endif
    }

    /// Destructor for the CRemoteXYStream class.
    /// The destructor does not perform any actions.
    virtual ~CRemoteXYStream() {}

    /// Sets the read byte listener object.
    /// @param listener The listener object to set.
    /// This method sets the readByteListener member variable to the
    /// provided listener object. The listener object will receive
    /// notifications when a byte is read from the stream.
    void setReadByteListener(CRemoteXYReadByteListener* listener) {
      readByteListener = listener;
    }

    /// Notifies the read byte listener object that a byte has been read.
    /// @param byte The byte that was read.
    /// This method checks if the readByteListener member variable is
    /// not nullptr, and if it is not, it calls the readByte method
    /// of the listener object, passing the read byte as a parameter.
    void notifyReadByteListener(uint8_t byte) {
      if (readByteListener) readByteListener->readByte(byte);
    }

    /// The handler method should be implemented by derived classes to
    /// handle incoming data.
    /// This is a pure virtual method, which means that it must be
    /// implemented by any class that derives from CRemoteXYStream.
    /// The handler method is called when data is available to be read
    /// from the stream.
    virtual void handler() {}

    /// Starts writing data to the stream.
    /// @param size The size of the data to be written.
    /// This method is a virtual method that can be overridden by
    /// derived classes to provide specific implementation for writing
    /// data to the stream.
    virtual void startWrite(uint16_t size) {
      // Intentionally left blank.
      (void)size;
    }

    /// Writes a byte to the stream.
    /// @param byte The byte to write.
    /// This method is a virtual method that can be overridden by
    /// derived classes to provide specific implementation for writing
    /// a byte to the stream.
    virtual void write(uint8_t byte) {
      // Intentionally left blank.
      (void)byte;
    }
};

#endif //RemoteXYStream_h
