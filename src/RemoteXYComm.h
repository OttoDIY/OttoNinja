#ifndef RemoteXYComm_h
#define RemoteXYComm_h  

#include <inttypes.h> 
#include "RemoteXYDebugLog.h"
#include "RemoteXYStream.h"

/**
 * Listener interface for client availability events.
 * Users of this class can implement this interface to be notified when a client becomes available.
 */
class CRemoteXYClientAvailableListener {
  public:
    /**
     * Method to be implemented by the user of this class to be notified when a client becomes available.
     */
    virtual void clientAvailable () = 0;
};

/**
 * Base class for remote clients.
 * This class provides the basic functionality for remote clients.
 */
class CRemoteXYClient: public CRemoteXYStream {
  public:
    /**
     * Pointer to the next client in a linked list of clients.
     * This is used for internal bookkeeping and is not intended to be used by users of this class.
     */
    CRemoteXYClient * next;

  public:
    /**
     * Default constructor for CRemoteXYClient.
     */
    CRemoteXYClient (): CRemoteXYStream() {
      next = nullptr; // Initialize the next pointer to nullptr
    }

  public:
    /**
     * Connects to the remote host.
     * @param host The remote host to connect to.
     * @param port The remote port to connect to.
     * @return 0 on success, or an error code on failure.
     */
    virtual uint8_t connect (const char *host, uint16_t port);

    /**
     * Disconnects from the remote host.
     */
    virtual void disconnect ();

    /**
     * Checks if the client is connected to the remote host.
     * @return 1 if connected, 0 otherwise.
     */
    virtual uint8_t connected () {return (socket_ != INVALID_SOCKET);};

    /**
     * Compares this client with another client.
     * @param cl The other client to compare with.
     * @return 1 if equal, 0 otherwise.
     */
    virtual uint8_t equal (CRemoteXYClient * cl);

  private:
    SOCKET socket_; // Socket used for communication with the remote host
};

/**
 * Base class for remote servers.
 * This class provides the basic functionality for remote servers.
 */
class CRemoteXYServer {

  private:
    CRemoteXYClientAvailableListener * clientAvailableListener; // Listener for client availability events

  public:
    /**
     * Sets the listener for client availability events.
     * @param listener The listener to set.
     */
    void setClientAvailabListener (CRemoteXYClientAvailableListener * listener) {
      clientAvailableListener = listener;
    }    

    /**
     * Notifies the client availability listener that a client is available.
     */
    void notifyClientAvailableListener () {
      if (clientAvailableListener) clientAvailableListener->clientAvailable ();
    }     

  public:     
    /**
     * Initializes the server.
     * @return 0 on success, or an error code on failure.
     */
    virtual uint8_t begin () {return 0;} 

    /**
     * Stops the server.
     */
    virtual void stop () {}; 

    /**
     * Checks if there are any clients available.
     * @param client The client to check.
     * @return 1 if available, 0 otherwise.
     */
    virtual uint8_t available (CRemoteXYClient * client) {
      UNUSED (client);
      return (client_list_ != nullptr);
    }

    /**
     * Gets the first available client.
     * @return The first available client, or nullptr if there are no clients available.
     */
    virtual CRemoteXYClient * getClient ();

  private:
    CRemoteXYClient * client_list_; // Linked list of clients connected to the server
};

/**
 * Base class for remote communication.
 * This class provides the basic functionality for remote communication.
 */
class CRemoteXYComm  {
  public:
    /**
     * Pointer to the next communication object in a linked list of communications.
     * This is used for internal bookkeeping and is not intended to be used by users of this class.
     */
    CRemoteXYComm * next;

  public:
    /**
     * Default constructor for CRemoteXYComm.
     */
    CRemoteXYComm () {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init (); // Initialize the debug log
#endif
      next = nullptr; // Initialize the next pointer to nullptr
      server_ = nullptr;
      client_list_ = nullptr;
    }

    /**
     * Handles incoming data.
     */
    virtual void handler () {}; 

    /**
     * Checks if the communication is configured.
     * @return 1 if configured, 0 otherwise.
     */
    virtual uint8_t configured () {return 1;};                 

    /**
     * Creates a new server.
     * @param port The port to listen on.
     * @return The new server, or nullptr if the server could not be created.
     */
    virtual CRemoteXYServer * createServer (uint16_t port);

    /**
     * Creates a new client.
     * @return The new client, or nullptr if the client could not be created.
     */
    virtual CRemoteXYClient * newClient ();

    /**
     * Removes a client from the list.
     * @param client The client to remove.
     */
    virtual void removeClient (CRemoteXYClient * client);

    /**
     * Gets the number of clients.
     * @return The number of clients.
     */
    virtual uint8_t getClientCount ();

  private:
    CRemoteXYServer * server_; // Pointer to the server object
    CRemoteXYClient * client_list_; // Linked list of client objects
};

#endif //RemoteXYComm_h
