/**
* @file Slideconnection.h
* @brief Contains declaration for the Slideconnection class and the enumeration for the component-identifiers.
*/
#ifndef _SlideCONNECTION_H_
#define _SlideCONNECTION_H_

#include <cstdio>
#include <signal.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#include "Logger.h"
#include "Global.h"

/**
* @class This class contains the functionality for a Connection between the CommServer and a component or a client.
*/
class SlideConnection
{
  public:
    /**
    * Constructs a new SlideConnection, which is basically a UNIX-Domain socket
    * connected to a running server-process (SlideIPCServer)
    * @param Path of the server to connect to
    * @param Path of the socket to create on (locally)
    * @param (Optional) If the local socket shoud be bound, thus turning it to a local server, defaults to true
    */
    SlideConnection(char *localPath,SlideComponent component,bool bindTo = true);

    /**
    * Get the current content of the message buffer or an empty buffer.
    * This method returns immediately, even if the buffer is empty.
    * The internal buffer will be cleared then.
    * @param Pointer to a sockaddr_un structure to receive the sender's address. Undefined, when no message was received.
    * @return Returns a CTRLMSG-Structure. If no message was available, the type-field will be NONE
    */
    CTRLMSG peekMessage(struct sockaddr_un *addr);

    /**
    * Get the content of the message buffer or wait until something has arrived
    * This method blocks in case of an empty buffer, until something has been
    * received. If there is something in the buffer, it gets returned.
    * @param Pointer to a sockaddr_un structure to receive the sender's address.
    * @return Returns a CTRLMSG-Structure
    */
    CTRLMSG getMessage(struct sockaddr_un *addr);

    /**
    * Send a message to the server to be redirected
    * to all known clients from the given type
    * @param Component to send message to
    * @param Message to send - has to be NULL-terminated!
    * @param Length of the message to send.
    */
    void sendMessage(CTRLMSG *msg,char *to);

    /**
    * Returns the number of bytes currently readable from the socket.
    * @todo Currently only a stub, returns 0 anyway.
    */
    unsigned int bytesReadable();

  private:
    int sock;
    struct sockaddr_un localAddr;
    char *inBuffer;
    char *outBuffer;
    char *localPath;
    SlideComponent component;
};

#endif
