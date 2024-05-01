// Include necessary header files
#include "RemoteXYDebugLog.h"
#include "RemoteXYConnection.h"
#include "RemoteXYThread.h"
#include "RemoteXYWireCloud.h"

// Define the timeout value for retrying cloud connection
#define REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT 20000

// Define the maximum number of clients
#define REMOTEXY_MAX_CLIENTS 10

// Define the class for cloud connection
class CRemoteXYCloudServer;

class CRemoteXYConnectionCloud : public CRemoteXYConnectionComm, public CRemoteXYCloudClientAvailableListener {
public:
    // Public variables
    uint16_t port;
    CRemoteXYCloudServer* cloudServer;
    const char* cloudHost;
    const char* cloudToken;
    CRemoteXYData* data;
    CRemoteXYClient* client;
    uint32_t timeOut;

    // Constructor for the class
    CRemoteXYConnectionCloud(CRemoteXYComm* _comm, const char* _cloudHost, uint16_t _port, const char* _cloudToken)
        : CRemoteXYConnectionComm(_comm) {
        // Initialize the port, cloudHost, cloudToken, and communication object
        port = _port;
        cloudHost = _cloudHost;
        cloudToken = _cloudToken;
    }

    // Destructor for the class
    ~CRemoteXYConnectionCloud() {
        // Delete the cloudServer object
        delete cloudServer;
    }

    // Initialize the connection
    void init(CRemoteXYData* _data) override {
        data = _data;
        cloudServer = new CRemoteXYCloudServer(data, cloudToken, this);
        client = comm->newClient();
        timeOut = -REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT;
        // Initialize the cloud server with the data, token, and listener
        // Set the communication object and start the cloud server
    }

    // Handle the connection
    void handler() override {
        // Check if the communication is configured
        if (!comm->configured()) {
            cloudServer->stop();
            return;
        }

        // If the cloud server is running
        if (cloudServer->running()) {
            cloudServer->handler();
            timeOut = millis();
            // Handle the cloud server and update the timeout
        }
        // If the cloud server is not running
        else {
            // If the timeout has been reached
            if (millis() - timeOut > REMOTEXY_CLOUDCLIENT_RETRY_TIMEOUT) {
                // Try to connect to the cloud server
                if (client->connect(cloudHost, port)) {
                    cloudServer->begin(client);
                    timeOut = millis();
                }
            }
        }

        // If the cloud server is not running
        if (!cloudServer->running()) {
            // If the client is connected
            if (client->connected()) {
                client->stop();
            }
        }
    }

    // Handle the cloud wire when it is available
    void clientAvailable(CRemoteXYWireCloud* cloudWire) override {
        // If the number of running clients is less than the maximum limit
        if (CRemoteXYThread::runningCount(data) < REMOTEXY_MAX_CLIENTS) {
            CRemoteXYThread::startThread(data, this, cloudWire, 1);
            // Start a new thread with the data, listener, cloud wire, and priority
        }
        // If the number of running clients is greater than or equal to the maximum limit
        else {
            cloudWire->stop();
#if defined(REMOTEXY__DEBUGLOG)
            RemoteXYDebugLog.write("Client reject");
#endif
            // Reject the client if the maximum limit is reached
        }
    }

    // Handle the wire
    void handleWire(CRemoteXYWire* wire) override {
        // If the cloud server is running and the communication is configured
        if (cloudServer->running() && comm->configured()) {
            if (wire != nullptr) {
                CRemoteXYWireCloud* cloudWire = dynamic_cast<CRemoteXYWireCloud*>(wire);
                if (cloudWire != nullptr) {
                    cloudWire->handler();
                    // Handle the cloud wire if it is not null
                }
            }
        }
        // If the cloud server is not running
        else {
            if (wire != nullptr) {
                stopThreadListener(wire);
            }
        }
    }

    // Stop the thread listener
    void stopThreadListener(CRemoteXYWire* wire) override {
        if (wire != nullptr) {
            wire->stop();
            // Stop the wire if it is not null
        }
    }
};

#endif // RemoteXYConnectionCloud_h
