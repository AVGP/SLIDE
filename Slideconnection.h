/**
* @file Slideconnection.h
* @brief Contains declaration for the Slideconnection class and the enumeration for the component-identifiers.
*/
#ifndef _SlideCONNECTION_H_
#define _SlideCONNECTION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <cstdio>
#include <signal.h>
#include <netdb.h>

#include "Logger.h"

enum SlideComponent
{
    ALL,
    CORE,
    CLIENTAPPS,
    DESKTOP,
    WM
};

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
    * @param (Optional) If the local socket shoud be bound, thus turning it to a local server, defaults to false
    */
    SlideConnection(const char *remotePath,const char *localPath,bool bindTo = false);

    /**
    * Get the current content of the message buffer or an empty buffer.
    * This method returns immediately, even if the buffer is empty.
    * The internal buffer will be cleared then.
    * @param A pointer to your buffer receiving the contents. You need to
    * allocate it on your own. This is because the internal buffer will be
    * cleared after this call.
    * The contents passed will be NULL-terminated
    */
    void peekMessages(const char *buffer);

    /**
    * Get the content of the message buffer or wait until something has arrived
    * This method blocks in case of an empty buffer, until something has been
    * received. If there is something in the buffer, it gets returned.
    * @param A pointer to your buffer receiving the contents. You need to
    * allocate it on your own. This is because the internal buffer will be
    * cleared after this call. The contents passed will be NULL-terminated
    */
    void getMessages(const char *buffer);

    /**
    * Send a message to the server to be redirected
    * to all known clients from the given type
    * @param Component to send message to
    * @param Message to send - has to be NULL-terminated!
    * @param Length of the message to send.
    */
    void sendMessage(SlideComponent componentType, char *msg,unsigned int msglen);

    /**
    * Returns the number of bytes currently readable from the socket.
    */
    unsigned int bytesReadable();

  private:
    int sock;
    char *inBuffer;
    char *outBuffer;
    char *remotePath;
    char *localPath;
};

#endif
