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
#include <sys/time.h>
#include <error.h>
#include <fcntl.h>
#include <errno.h>

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
    * @param localPath Path to use for receiving.
    * @param bindTo (Optional) If the local socket shoud be bound, thus turning it to a local server, defaults to true
    */
    SlideConnection(char *localPathbool bindTo = true);

    /**
    * Get the current content of the message buffer or an empty buffer.
    * This method returns immediately, even if the buffer is empty.
    * The internal buffer will be cleared then.
    * @param addr Pointer to a sockaddr_un structure to receive the sender's address. Undefined, when no message was received.
    * @return Returns a CTRLMSG-Structure. If no message was available, the type-field will be NONE
    */
    CTRLMSG peekMessage(struct sockaddr_un *addr);

    /**
    * Get the content of the message buffer or wait until something has arrived
    * This method blocks in case of an empty buffer, until something has been
    * received. If there is something in the buffer, it gets returned.
    * @param addr Pointer to a sockaddr_un structure to receive the sender's address.
    * @return Returns a CTRLMSG-Structure
    */
    CTRLMSG getMessage(struct sockaddr_un *addr);

    /**
    * Send a message to the given receipient (blocking)
    * @param msg Message to send - has to be NULL-terminated!
    * @param to Receipient for the message (Path of the socket-file)
    */
    void sendMessage(CTRLMSG *msg,char *to);

    /**
    * Send a message to the given receipient (non-blocking)
    * @param msg Message to send - has to be NULL-terminated!
    * @param to Receipient for the message (Path of the socket-file)
    */
    void putMessage(CTRLMSG *msg,char *to);

    /**
    * Returns the number of bytes currently readable from the socket.
    * @todo Currently only a stub, returns 0 anyway.
    */
    unsigned int bytesReadable();

  private:
    int sockIn,sockOut;
    struct sockaddr_un localAddr;
    char *localPath;
};

#endif
