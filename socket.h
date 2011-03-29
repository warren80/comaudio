#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
typedef unsigned int in_addr_t;
typedef int socklen_t;
#endif

/**
  Which mode the socket operates in.
  @author Nick Huber
  */
enum NetMode {
    kTCP = SOCK_STREAM,   /**< UDP Socket. */
    kUDP = SOCK_DGRAM,    /**< TCP Socket. */
};

/**
  Wrapper class for the standard function calls making use of sockets.

  @author Nick Huber
  */
class Socket {
public:

    /**
      Constructor for a Socket based on a connection mode.

      @throws QString Unable to create a socket.
      @param mode TCP or UDP socket.
      @author Nick Huber
      */
    explicit Socket(NetMode mode);

    /**
      Constructor for a given socket desrictor and peer (sockaddr_in) information.

      @param socket Socket descriptor.
      @param mode Whether the socket is for TCP or UDP.
      @param peer sockaddr_in for destination information.
      @author Nick Huber
      */
    Socket(int socket, NetMode mode, sockaddr_in peer) : socket_(socket), mode_(mode), peer_(peer) {}

    /**
      Destructor for the socket. Calls shutdown and close on the file descriptor.
      */
    ~Socket();

    /**
      Bind the socket to all incoming address

      @throws QString Unable to bind a socket.
      @param port Port to bind to in network byte ordering.
      @param listenTo IP address to listen to (network byte order). Defaults to INADDR_ANY
      @author Nick Huber
      */
    void bind(int port, in_addr_t listenTo = htonl(INADDR_ANY));

    /**
      Make socket willing to listen to new connections, with the specified
      backlog.

      @throws QString Unable to
      @param backlog The number of connections to queue.
      @author Nick Huber
      */
    void listen(int backlog);

    /**
      Read length bytes into buffer from the socket. Returns -1 if this Socket is
      a UDP socket.

      @param buffer Pre-allocated buffer of size length.
      @param length Number of bytes to receive.
      @return Number of bytes received.
      @author Nick Huber
      */
    int receive(char* buffer, int length) const;

    /**
      Transmit length bytes from buffer to the socket. Will transfer data to the
      peer this socket is associated with in the UDP scenario.

      @param buffer Bytes to transmit.
      @param length Number of bytes to transmit.
      @return Number of bytes transmitted.
      @author Nick Huber
      */
    int transmit(char* buffer, int length) const;

    /**
      Accept and create a new socket from the accepted connection.

      @return New socket for the accepted connection.
      @author Nick Huber
      */
    Socket accept() const;

    /**
      Connect to the address specified. Returns immediatly with -1 on UDP.

      @param adddress IP address in network byte ordering.
      @param port Port # of the server in network byte ordering.
      */
    bool connect(in_addr_t address, uint16_t port) const;

    /**
      Conversion operator for Socket, acts as a c-style socket descriptor.
      @return The socket descriptor.
      */
    operator const int() const { return socket_; }

private:
    int socket_;        /**< Socket file descriptor. */
    NetMode mode_;      /**< Network mode this socket works in. */
    sockaddr_in peer_;  /**< Information about the peer for this socket. */
};

#endif
