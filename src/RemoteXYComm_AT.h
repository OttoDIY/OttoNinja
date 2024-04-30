#ifndef REMOTEXYCOMM_AT_h
#define REMOTEXYCOMM_AT_h

#include <stdarg.h>
#include "RemoteXYDebugLog.h"
#include "RemoteXYComm.h"
#include "RemoteXYFunc.h"

#define UNUSED(x) (void)(x)

// Constants for AT command responses
const char *AT_ANSWER_ERROR = "ERROR";
const char *AT_ANSWER_FAIL = "FAIL";
const char *AT_ANSWER_BUSY = "BUSY *";
const char *AT_ANSWER_OK = "OK";
const char *AT_ANSWER_SEND_OK = "SEND OK";
const char *AT_ANSWER_SEND_FAIL = "SEND FAIL";
const char *AT_MESSAGE_READY = "READY";
const char *AT_MESSAGE_AT = "AT";
const char *AT_MESSAGE_CONNECT = "?,CONNECT";
const char *AT_MESSAGE_CLOSED = "?,CLOSED";
const char *AT_MESSAGE_CONNECT_FAIL = "?,CONNECT FAIL";
const char *AT_MESSAGE_IPD = "+IPD,?,*:";
const char *AT_MESSAGE_CIPSEND = "AT+CIPSEND=";
const char *AT_MESSAGE_CIPCLOSE = "AT+CIPCLOSE=";
const char *AT_MESSAGE_CIPSTART = "AT+CIPSTART=";

// Class for managing AT commands and responses
class CRemoteXYComm_AT : public CRemoteXYComm, public CRemoteXYReadByteListener {
  public:
    CRemoteXYComm_AT(CRemoteXYStream *_serial, uint16_t _sendByteMax);
    ~CRemoteXYComm_AT();

    // Functions for handling AT commands and responses
    virtual void moduleReset() {};
    virtual void moduleLost() {};
    virtual void moduleFound() {};
    virtual uint8_t handleATMessage() { return 0; };
    virtual void commandATListener(uint8_t identifier, uint8_t result) { UNUSED(identifier); UNUSED(result); };

    // Variables for managing AT commands and responses
    CRemoteXYStream *serial;
    CRemoteXYServer *server;
    CRemoteXYClient_AT_Proto *clients;
    char receiveBuffer[REMOTEXYCOMM_AT__RECEIVE_BUFFER_SIZE];
    uint8_t receiveBufferIndex;
    char *params[3];
    uint8_t paramsLength[3];
    uint32_t lastAnswerTime;
    uint16_t connectAvailable;
    uint16_t freeAvailable;
    uint32_t commandTimeOut;
    uint32_t commandDelay;
    uint8_t commandIdentifier;
    void (*commandListener)(uint8_t, uint8_t);
    volatile uint8_t commandResult;
    volatile uint8_t commandBlocking;
    uint8_t commandFlag;
    uint16_t ipdId;
    uint16_t ipdSize;
    CRemoteXYClient_AT_Proto *ipdClient;
    int8_t availableClientID;
    uint8_t findModuleTryCount;
    uint8_t haveEcho;
    uint16_t sendByteMax;
    uint16_t sendByteSize;
    uint16_t sendByteNext;
    uint8_t clientConnectingID;
    uint8_t detected;
};

// Class for managing AT command clients
class CRemoteXYClient_AT_Proto : public CRemoteXYClient {
  public:
    CRemoteXYClient_AT_Proto(CRemoteXYComm_AT *_comm);
    ~CRemoteXYClient_AT_Proto();

    // Functions for managing clients
    virtual void notifyReadByteListener(uint8_t byte) override;

    // Variables for managing clients
    CRemoteXYComm_AT *comm;
    uint8_t id;
    uint8_t _connected;
    CRemoteXYClient_AT_Proto *next;
};

// Class for managing the remote server
class CRemoteXYServer_AT : public CRemoteXYServer {
  public:
    CRemoteXYServer_AT(CRemoteXYComm_AT *_comm, uint16_t _port);
    ~CRemoteXYServer_AT();

    // Functions for managing the server
    uint8_t available(CRemoteXYClient *client) override;
    uint8_t begin() override;
    void stop() override;

    // Variables for managing the server
    CRemoteXYComm_AT *comm;
    uint16_t port;
};

// Class for managing the remote client
class CRemoteXYClient_AT : public CRemoteXYClient_AT_Proto {
  public:
    CRemoteXYClient_AT(CRemoteXYComm_AT *_comm);
    ~CRemoteXYClient_AT();

    // Functions for managing the client
    uint8_t connect(const char *host, uint16_t port) override;
    uint8_t connected() override;
    void stop() override;
    void startWrite(uint16_t size) override;
    void write(uint8_t byte) override;
};

#endif // RemoteXYComm_AT_h
