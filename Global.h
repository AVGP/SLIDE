/**
* @file Global.h
* @brief Contains globally useful includes and defines the message-types
*/
#ifndef _Slide_GLOBAL_H_
#define _Slide_GLOBAL_H_

#include <cstdlib>
#include <cstdio>
#include <sys/un.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xatom.h>
//#include <X11/Xkeysym.h>

#include "Logger.h"

/**
* @enum Enumeration of available Slide-Message-Types to be used when communicating.
*/
enum CTRLMSG_TYPE
{
    CONNECTNOTIFY,          //Sent from every client to notify the CommServer what kind of client it is.
    STARTREQUEST,           //Sent from Slide or WM, requesting component to start
    SHUTDOWNREQUEST,        //Sent from Clientapplication, requesting environment shutdown
    CONFIGRELOADREQUEST,    //Sent from Clientapplication, wishing to refresh the config
    CLOSENOTIFY,            //Sent from the Serversocket when closing
    MOVEREQUEST,            //Sent from Clientapplication notifying WM to move a given window
    RESIZEREQUEST,          //Sent from Clientapplication notifying WM to resize a given window
    CLOSEWINDOWREQUEST,     //Sent from Clientapplication notifying the wish to close a given window
    HIDEWINDOWREQUEST,      //Sent from Clientapplication notifying the wish to hide a given window
    SHOWWINDOWREQUEST,      //Sent from Clientapplication notifying the wish to show a given window
    GEOMETRYREQUEST,        //Sent from a client or component to notify the WM to send the geometry of the screen
    GEOMETRYREPLY,          //Sent after GEOMETRYREQUEST - contains the dimensions as 2 ints
    WINDOWLISTINTEREST,     //Sent from a client/component to ask for Window-List changes (Window destroyed/created)
    WINDOWLISTCREATEWND,    //Sent to the WINDOWLISTINTEREST-Listeners on window creation.
    WINDOWLISTDESTROYWND,   //Sent to the WINDOWLISTINTEREST-Listeners on window destruction.
    NONE                    //Indicates that no message is available.
};

/**
* @typedef Structure used for messages sent via SlideConnection
*/
typedef struct
{
    CTRLMSG_TYPE type;
    struct sockaddr_un addr;
    int len;
    unsigned char msg[500];
} CTRLMSG;

#endif
