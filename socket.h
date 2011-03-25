#ifndef SOCKET_H
#define SOCKET_H

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
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
      Constructor for the Socket based on a connection mode.

      @throws QString Unable to create a socket.
      @param mode TCP or UDP socket.
      @author Nick Huber
      */
    explicit Socket(NetMode mode);

    /**
      Constructor for a given socket desrictor and socaddr_in information.

      @param socket Socket descriptor.
      @param mode Whether the socket is for TCP or UDP.
      @param info sockaddr_in information.
      @author Nick Huber
      */
    Socket(int socket, NetMode mode, sockaddr_in info) : socket_(socket), mode_(mode), information_(info) {}

    /**
      Destructor for the socket. Calls shutdown and close on the file descriptor.
      */
    ~Socket();

    /**
      Bind the socket to all incoming address

      @throws QString Unable to bind a socket.
      @param port The port to bind to.
      @author Nick Huber
      */
    void bind(int port);

    /**
      Make socket willing to listen to new connections, with the specified
      backlog.

      @throws QString Unable to
      @param backlog The number of connections to queue.
      @author Nick Huber
      */
    void listen(int backlog);

    /**
      Read length bytes into buffer from the socket.

      @param buffer pre-allocated buffer of size length.
      @param length number of bytes to receive.
      @return Number of bytes received.
      @author Nick Huber
      */
    int receive(char* buffer, int length) const;

    /**
      Transmit length bytes from buffer to the socket.

      @param buffer bytes to transmit.
      @param length number of bytes to transmit.
      @return Number of bytes transmitted.
      @author Nick Huber
      */
    int transmit(char* buffer, int length) const;

    /**
      Accept and create a new socket from the accepted connection.

      @return New socket for the accepted connection.
      @author Nick Huber
      */
    Socket accept();

    /**
      Conversion operator for Socket, acts as a c-style socket descriptor.
      @return The socket descriptor.
      */
    operator const int() const { return socket_; }

private:
    int socket_;
    NetMode mode_;
    sockaddr_in information_;

};

#endif
