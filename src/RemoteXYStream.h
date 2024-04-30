#ifndef RemoteXYStream_h
#define RemoteXYStream_h

#include <inttypes.h>
#include "RemoteXYDebugLog.h"

#define UNUSED(x) (void)(x)

class CRemoteXYReadByteListener;

class CRemoteXYStream {
  private:
    CRemoteXYReadByteListener* readByteListener;

  public:
    CRemoteXYStream() : readByteListener(nullptr) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init();
#endif
    }

    void setReadByteListener(CRemoteXYReadByteListener* listener) {
      readByteListener = listener;
    }

    void notifyReadByteListener(uint8_t byte) {
      if (readByteListener) readByteListener->readByte(byte);
    }

    virtual void handler() {}
    virtual void startWrite(uint16_t size) {UNUSED(size);}
    virtual void write(uint8_t byte) {UNUSED(byte);}
};

#endif //RemoteXYStream_h
