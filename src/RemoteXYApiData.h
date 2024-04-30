// CRemoteXYData class
// This class handles data related to a remote connection.
// It includes variables for configuration data, input and output data,
// a connection flag, and various other settings.

class CRemoteXYData {
  public:
  // confVersion: The version of the configuration data.
  uint8_t confVersion;

  // conf: A pointer to the constant configuration data.
  const uint8_t* conf;

  // var: A pointer to the variable data.
  uint8_t *var;

  // accessPassword: A pointer to the constant access password for the remote connection.
  const char* accessPassword;

  // outputLength: The length of the output data.
  uint16_t outputLength;

  // inputLength: The length of the input data.
  uint16_t inputLength;

  // confLength: The length of the configuration data.
  uint16_t confLength;

  // connect_flag: A flag indicating whether the device is currently connected to the remote system.
  uint8_t *connect_flag;

  // receiveBufferSize: The size of the receive buffer.
  uint16_t receiveBufferSize;  

  // threads: A pointer to an instance of the CRemoteXYThread class.
  CRemoteXYThread * threads;  

  // comms: A pointer to an instance of the CRemoteXYComm class.
  CRemoteXYComm * comms;    

  // connections: A pointer to an instance of the CRemoteXYConnectionComm class.
  CRemoteXYConnectionComm * connections;    

  // Constructor
  CRemoteXYData() : conf(nullptr), var(nullptr), accessPassword(nullptr), connect_flag(nullptr) {}

  // Destructor
  ~CRemoteXYData() {
    if (var != nullptr) {
      delete[] var;
      var = nullptr;
    }
    if (connect_flag != nullptr) {
      delete[] connect_flag;
      connect_flag = nullptr;
    }
  }

  // init: Initializes the CRemoteXYData object with configuration, variable, and access password data.
  // _conf: A pointer to the configuration data.
  // _var: A pointer to the variable data.
  // _accessPassword: A pointer to the access password for the remote connection.
  void init (const void * _conf, void * _var, const char * _accessPassword) {
    if (_conf == nullptr || _var == nullptr || _accessPassword == nullptr) {
      return;
    }
    confVersion = *((const uint8_t*)_conf);
    conf = static_cast<const uint8_t*>(_conf);
    var = static_cast<uint8_t*>(_var);
    accessPassword = _accessPassword;
    outputLength = *((const uint16_t*)(_conf + 1));
    inputLength = *((const uint16_t*)(_conf + 3));
    confLength = *((const uint16_t*)(_conf + 5));
    connect_flag = new uint8_t;
    *connect_flag = 0;
    receiveBufferSize = *((const uint16_t*)(_conf + 7));
  }

  // getConfByte: Returns a byte of configuration data at the specified address.
  // p: A pointer to the address of the configuration data.
  inline uint8_t getConfByte (uint8_t* p) {
    return conf[p - (const uint8_t*)conf];
  }

  // Getters for all member variables
  inline const uint8_t* getConf() const { return conf; }
  inline uint8_t* getVar() const { return var; }
  inline const char* getAccessPassword() const { return accessPassword; }
  inline uint16_t getOutputLength() const { return outputLength; }
  inline uint16_t getInputLength() const { return inputLength; }
  inline uint16_t getConfLength() const { return confLength; }
  inline uint8_t* getConnectFlag() const { return connect_flag; }
  inline uint16_t getReceiveBufferSize() const { return receiveBufferSize; }
  inline CRemoteXYThread* getThreads() const { return threads; }
  inline CRemoteXYComm* getComms() const { return comms; }
  inline CRemoteXYConnectionComm* getConnections() const { return connections; }
};
