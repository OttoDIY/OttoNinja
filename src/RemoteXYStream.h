#ifndef RemoteXYStream_h
#define RemoteXYStream_h   // Include guard to prevent multiple inclusions

#include <inttypes.h>  // Include standard integer type definitions
#include "RemoteXYDebugLog.h"  // Include debug logging utility header

#define UNUSED(x) (void)(x)  // Macro to suppress warnings for unused variables

// Forward declaration of the listener interface
class CRemoteXYReadByteListener;

// The main RemoteXYStream class
class CRemoteXYStream {
  private:
    CRemoteXYReadByteListener * readByteListener;  // Pointer to the read byte listener

  public:
    CRemoteXYStream () {
      readByteListener = NULL;

#if defined(REMOTEXY__DEBUGLOG)
      // Initialize the debug logging utility if debug logging is enabled
      RemoteXYDebugLog.init ();
#endif
    }

  public:
    // Set the read byte listener
    void setReadByteListener (CRemoteXYReadByteListener * listener) {
      readByteListener = listener;
    }

  public:
    // Notify the read byte listener with a received byte
    void notifyReadByteListener (uint8_t byte) {
      if (readByteListener) readByteListener->readByte (byte);
    }

  public:     
    // Placeholder functions for the abstract class
    virtual void handler () {};
    virtual void startWrite (uint16_t size) {UNUSED (size);};
    virtual void write (uint8_t byte) {UNUSED (byte);};
};





#endif //RemoteXYStream_h
