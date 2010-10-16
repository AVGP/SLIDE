/**
* @file Slidecommserver.h
* @brief Contains the declaration for the SlideCommServer-Component.
*/
#ifndef _Slide_COMMSERVER_H_
#define _Slide_COMMSERVER_H_
#include <list>
#include <cstdio>
#include "Slideconnection.h"
#include "Global.h"

class SlideCommServer
{
  public:
    /**
    * Creates a new instance of a SlideCommServer, which is server-side
    * @param Path of the socket-file (Unix-Domain Socket!)
    */
    SlideCommServer(const char *path);

    /**
    * The destructor notifies all known clients about its shutdown and frees the resources.
    */
    ~SlideCommServer();

    /**
    * Setting up the SlideCommServer socket, binds and listens.
    * @param Path of the socket-file (Unix-Domain Socket!)
    * @return Returns true, if setup was successful, else returns false
    */
    bool setup(const char *path);

    /**
    * Starts the accept-loop. As this blocks, its usually a good idea to
    * call it from a separate thread or process.
    * @return Returns true, when it has run and got shut down correctly, false in case of error
    */
    bool run();
  private:
    int sock;
    std::list<SlideConnection *> clients;

};

#endif
