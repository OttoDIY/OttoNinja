// CRemoteXYData class
// This class handles data related to a remote connection.
// It includes variables for configuration data, input and output data,
// a connection flag, and various other settings.

class CRemoteXYData {
  public:
  // confVersion: The version of the configuration data.
  uint8_t confVersion;

  // conf: A pointer to the configuration data.
  uint8_t *conf;

  // var: A pointer to the variable data.
  uint8_t *var;

  // accessPassword: A pointer to the access password for the remote connection.
  uint8_t *accessPassword;

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


  public:
  // init: Initializes the CRemoteXYData object with configuration, variable, and access password data.
  // _conf: A pointer to the configuration data.
  // _var: A pointer to the variable data.
  // _accessPassword: A pointer to the access password for the remote connection.
  void init (const void * _conf, void * _var, const char * _accessPassword);

  // getConfByte: Returns a byte of configuration data at the specified address.
  // p: A pointer to the address of the configuration data.
  inline uint8_t getConfByte (uint8_t* p);

};
