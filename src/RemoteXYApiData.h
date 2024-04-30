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

    // Default constructor
    CRemoteXYData() :
      conf(nullptr), var(nullptr), accessPassword(nullptr), connect_flag(nullptr),
      threads(nullptr), comms(nullptr), connections(nullptr) {}

    // Constructor
    CRemoteXYData(const void * _conf, void * _var, const char * _accessPassword) :
      confVersion(0), conf(_conf), var(_var), accessPassword(_accessPassword),
      outputLength(0), inputLength(0), confLength(0), connect_flag(nullptr),
      receiveBufferSize(0), threads(nullptr), comms(nullptr), connections(nullptr) {
      init(_conf, _var, _accessPassword);
    }

    // Destructor
    ~CRemoteXYData() {
      if (var != nullptr) {
        delete[] var;
        var = nullptr;
      }
      if (connect_flag != nullptr) {
        delete[] connect_flag;
        connect_flag = nullptr;

