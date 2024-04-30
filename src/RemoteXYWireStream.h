#ifndef REMOTEXYWIRESTREAM_H
#define REMOTEXYWIRESTREAM_H

#include <Arduino.h>
#include "RemoteXYWire.h"
#include "RemoteXYApiData.h"
#include <array>
#include <optional>
#include <functional>

class CRemoteXYWireStream : public CRemoteXYWire, public CRemoteXYReadByteListener {
  public:
    // Default constructor
    CRemoteXYWireStream() = default;

    // Constructor with one parameter
    explicit CRemoteXYWireStream(CRemoteXYData* data) : CRemoteXYWire(), stream(nullptr) {
      receiveBufferSize = data->receiveBufferSize;
      receiveBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[receiveBufferSize]);
    }

    // Constructor with two parameters
    explicit CRemoteXYWireStream(CRemoteXYData* data, uint8_t multiple) : CRemoteXYWire(), stream(nullptr) {
      receiveBufferSize = data->receiveBufferSize * multiple;
      receiveBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[receiveBufferSize]);
    }

    // Method to initialize the stream
    void begin(std::optional<CRemoteXYStream*> _stream) {
      stream = _stream;
      if (stream) {
        stream->setReadByteListener(this);
      }
      receiveIndex = 0;
      receiveModified = 0;
      receiveLock = 0;
    }

    // Method to stop the stream
    void stop() noexcept override {
      setReceivePackageListener(nullptr);
      stream.reset();
    }

    // Method to check if the stream is running
    uint8_t running() const noexcept override {
      return stream ? 1 : 0;
    }

    // Method to get the client associated with the stream
    template <typename T>
    std::enable_if_t<std::is_base_of<CRemoteXYClient, T>::value, T*> getClient() const noexcept {
      return static_cast<T*>(stream.get());
    }

    // Method to handle data transmission and reception
    void handler() override {
      if (stream) {
        stream->handler();
        receivePackage();
      }
    }

  private:
    // Method to update the CRC value
    void updateCRC(uint16_t* crc, uint8_t b) {
      *crc ^= b;
      for (uint8_t i = 0; i < 8; ++i) {
        if ((*crc) & 1) *crc = ((*crc) >> 1) ^ 0xA001;
        else *crc >>= 1;
      }
    }

  private:
    // Method to send data with updated CRC value
    inline void sendByteUpdateCRC(uint8_t b) {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.writeOutputHex(b);
#endif
      if (stream) {
        stream->write(b);
        updateCRC(&sendCRC, b);
      }
    }

  public:
    // Method to send a package of data with specified command, buffer, length, and fromPgm flag
    void sendPackage(uint8_t command, const uint8_t* buf, uint16_t length, uint8_t fromPgm) {
      uint16_t packageLength;
      if (stream) {
        sendCRC = REMOTEXY_INIT_CRC;
        packageLength = length + 6;
        stream->startWrite(packageLength);

        sendByteUpdateCRC(REMOTEXY_PACKAGE_START_BYTE);
        sendByteUpdateCRC(packageLength);
        sendByteUpdateCRC(packageLength >> 8);
        sendByteUpdateCRC(command);

        uint8_t b;
        while (length--) {
          if (fromPgm) {
            b = pgm_read_byte_near(buf);
          } else {
            b = *buf;
          }
          sendByteUpdateCRC(b);
          buf++;
        }

#if defined(REMOTEXY__DEBUGLOG)
        RemoteXYDebugLog.writeOutputHex(sendCRC);
        RemoteXYDebugLog.writeOutputHex(sendCRC >> 8);
#endif

        stream->write(sendCRC);
        stream->write(sendCRC >> 8);
      }
    }

    // Method to read a byte of data and store it in the receive buffer
    void readByte(uint8_t byte) override {
      uint16_t pi, i;

#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.writeInputHex(byte);
#endif

      if ((receiveIndex == 0) && (byte != REMOTEXY_PACKAGE_START_BYTE)) return;

      if (receiveIndex >= receiveBufferSize) {
        if (receiveLock) return;

        pi = 1;
        while (pi < receiveBufferSize) {
          if (receiveBuffer[pi] == REMOTEXY_PACKAGE_START_BYTE) break;
          pi++;
        }

        receiveIndex = receiveBufferSize - pi;
        i = 0;
        while (pi < receiveBufferSize) {
          receiveBuffer[i] = receiveBuffer[pi];
          i++;
          pi++;
        }
      }

      std::lock_guard<std::mutex> lock(receiveMutex);
      receiveBuffer[receiveIndex++] = byte;
      receiveModified = 1;
    }

  private:
    // Method to process received data and call the receivePackageListener with the received package
    void receivePackage() {
      if (receiveModified) {
        receiveModified = 0;

        uint16_t crc;
        uint16_t si, i;
        uint16_t packageLength;
        si = 0;
        while (si + REMOTEXY_PACKAGE_MIN_LENGTH <= receiveIndex) {
          if (receiveBuffer[si] == REMOTEXY_PACKAGE_START_BYTE) {
            packageLength = receiveBuffer[si + 1] | (receiveBuffer[si + 2] << 8);
            if ((packageLength <= receiveIndex - si) && (packageLength >= 6)) {
              crc = REMOTEXY_INIT_CRC;
              for (i = si; i < si + packageLength; i++) {
                updateCRC(&crc, receiveBuffer[i]);
              }

              if (crc == 0) {
                std::array<uint8_t, REMOTEXY_MAX_PACKAGE_LENGTH> packageBuffer;
                packageBuffer.fill(0);
                std::copy(receiveBuffer.get() + si + 4, receiveBuffer.get() + si + 4 + packageLength - 6, packageBuffer.begin());

                CRemoteXYPackage package{receiveBuffer[si + 3], packageBuffer.data(), packageLength - 6};

                receiveLock = 1;
                receivePackageListener(package);
                si += packageLength;

                std::lock_guard<std::mutex> lock(receiveMutex);
                receiveIndex -= si;
                if (receiveIndex > 0) {
                  std::copy(receiveBuffer.get() + si, receiveBuffer.get() + receiveIndex, receiveBuffer.get());
                }
                si = 0;
                continue;
              }
            }
          }
          si++;
        }
      }
    }

    std::function<void(const CRemoteXYPackage&)> receivePackageListener;
    std::mutex receiveMutex;

    uint16_t receiveIndex = 0;
    uint16_t receiveModified = 0;
    uint16_t receiveLock = 0;
    uint16_t sendCRC = 0;
    uint16_t receiveBufferSize = 0;
    std::unique_ptr<uint8_t[]> receiveBuffer;
};

#endif  //  RemoteXYWireStream_h
