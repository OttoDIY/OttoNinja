// The CRemoteXYClient_WiFi class represents a WiFi client for the RemoteXY platform.
// It includes functions for connecting to a server, checking if the client is connected,
// stopping the client, handling incoming data, and writing data to be sent to the server.
class CRemoteXYClient_WiFi : public CRemoteXYClient {
public:
  // The constructor initializes a new WiFi client.
  CRemoteXYClient_WiFi();

  // The connect function connects the client to a server with the given host and port.
  // It returns 1 if the connection was successful, or 0 if it failed.
  uint8_t connect(const char *host, uint16_t port);

  // The connected function returns 1 if the client is connected to a server, or 0 if it is not.
  uint8_t connected();

  // The stop function stops the client and closes the connection to the server.
  void stop();

  // The handler function handles incoming data from the server.
  // It should be called regularly in the main loop of the program.
  void handler();

  // The startWrite function initializes the client's send buffer for writing data.
  // It takes the length of the data to be sent as an argument.
  void startWrite(uint16_t len);

  // The write function writes a single byte to the client's send buffer.
  // It automatically sends the data in the buffer when it is full or when all the data has been written.
  void write(uint8_t b);
};

// The CRemoteXYServer_WiFi class represents a WiFi server for the RemoteXY platform.
// It includes functions for starting the server, stopping the server, checking if a client is available,
// and handling client connections.
class CRemoteXYServer_WiFi : public CRemoteXYServer {
private:
  // The server object is used to manage the WiFi server.
  WiFiServer *server;

public:
  // The constructor initializes a new WiFi server with the given port.
  CRemoteXYServer_WiFi(uint16_t _port);

  // The begin function starts the WiFi server.
  // It returns 1 if the server was started successfully, or 0 if it failed.
  virtual uint8_t begin() override;

  // The stop function stops the WiFi server.
  void stop();

  // The available function checks if a client is available to connect to the server.
  // It returns 1 if a client is available, or 0 if no clients are available.
  uint8_t available(CRemoteXYClient * client) override;
};

// The CRemoteXYComm_WiFi class manages WiFi connections for the RemoteXY platform.
// It includes functions for connecting to a WiFi network, handling WiFi connection events,
// checking if the WiFi connection is configured, and creating clients and servers.
class CRemoteXYComm_WiFi : public CRemoteXYComm {
private:
  // The wifiSsid and wifiPassword variables store the SSID and password of the WiFi network.
  const char *wifiSsid;
  const char *wifiPassword;

  // The wifiStatus variable stores the current status of the WiFi connection.
  uint8_t wifiStatus;

public:
  // The constructor initializes a new WiFi connection with the given SSID and password.
  CRemoteXYComm_WiFi(const char * _wifiSsid, const char * _wifiPassword);

  // The handler function handles WiFi connection events.
  // It should be called regularly in the main loop of the program.
  void handler() override;

  // The configured function returns 1 if the WiFi connection is configured and connected,
  // or 0 if it is not.
  uint8_t configured() override;

  // The createServer function creates a new WiFi server with the given port.
  CRemoteXYServer *createServer(uint16_t _port) override;

  // The newClient function creates a new WiFi client.
  CRemoteXYClient *newClient() override;
};

// The CRemoteXYComm_WiFiPoint class creates a WiFi access point for the RemoteXY platform.
// It includes functions for creating an access point and checking if the access point is configured.
class CRemoteXYComm_WiFiPoint : public CRemoteXYComm {
private:
  // The state variable stores the current state of the access point.
  uint8_t state;

public:
  // The constructor initializes a new WiFi access point with the given SSID and password.
  CRemoteXYComm_WiFiPoint(const char * _wifiSsid, const char * _wifiPassword);

  // The configured function returns 1 if the access point is configured,
  // or 0 if it is not.
  uint8_t configured() override;

  // The createServer function creates a new WiFi server with the given port.
  CRemoteXYServer *createServer(uint16_t _port) override;

  // The newClient function creates a new WiFi client.
  CRemoteXYClient *newClient() override;
};
