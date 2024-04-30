#ifndef RemoteXYComm_h
#define RemoteXYComm_h  

#include <inttypes.h> 
#include "RemoteXYDebugLog.h"
#include "RemoteXYStream.h"

/**
 * Listener interface for client availability events.
 */
class CRemoteXYClientAvailableListener {
  public:
    virtual void clientAvailable () = 0;
};

/**
 * Base class for remote clients.
 */
class CRemoteXYClient: public CRemoteXYStream {
  public:
    CRemoteXYClient * next;

  public:
    CRemoteXYClient (): CRemoteXYStream() {
      next = nullptr;
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
    SOCKET socket_;
};

/**
 * Base class for remote servers.
 */
class CRemoteXYServer {

  private:
    CRemoteXYClientAvailableListener * clientAvailableListener;

  public:
    void setClientAvailabListener (CRemoteXYClientAvailableListener * listener) {
      clientAvailableListener = listener;
    }    

    void notifyClientAvailableListener () {
      if (clientAvailableListener) clientAvailableListener->clientAvailable ();
    }     

  public:     
    virtual uint8_t begin () {return 0;} 
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
    CRemoteXYClient * client_list_;
};

/**
 * Base class for remote communication.
 */
class CRemoteXYComm  {
  public:
    CRemoteXYComm * next;

  public:
    CRemoteXYComm () {
#if defined(REMOTEXY__DEBUGLOG)
      RemoteXYDebugLog.init ();
#endif
      next = nullptr;
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
    CRemoteXYServer * server_;
    CRemoteXYClient * client_list_;
};

#endif //RemoteXYComm_h
