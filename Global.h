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
* @enum CTRLMSG_TYPE Enumeration of available Slide-Message-Types to be used when communicating.
* @brief Defines the possible type of a CTRLMSG sent through SlideConnection
*/
enum CTRLMSG_TYPE
{
    STARTREQUEST,           /**< Requesting component to start*/
    SHUTDOWNREQUEST,        /**< Requesting environment shutdown*/
    CONFIGRELOADREQUEST,    /**< Slide is wishing to refresh the config*/
    MOVEREQUEST,            /**< Sent from Client-Application notifying WM to move a given window*/
    RESIZEREQUEST,          /**< Sent from Client-Application notifying WM to resize a given window*/
    CLOSEWINDOWREQUEST,     /**< Sent from Client-Application notifying the wish to close a given window*/
    HIDEWINDOWREQUEST,      /**< Sent from Client-Application notifying the wish to hide a given window*/
    SHOWWINDOWREQUEST,      /**< Sent from Client-Application notifying the wish to show a given window*/
    GEOMETRYREQUEST,        /**< Sent from a client to notify the WM to send the geometry of the screen*/
    GEOMETRYREPLY,          /**< Sent after GEOMETRYREQUEST - contains the dimensions as 2 ints*/
    WINDOWLISTINTEREST,     /**< Sent from a client to ask for Window-List changes (Window destroyed/created)*/
    WINDOWLISTCREATEWND,    /**< Sent to the WINDOWLISTINTEREST-Listeners on window creation.*/
    WINDOWLISTDESTROYWND,   /**< Sent to the WINDOWLISTINTEREST-Listeners on window destruction.*/
    NONE                    /**< Indicates that no message is available.*/
};

/**
* Structure used for messages sent via SlideConnection
* @brief Used for sending messages via SlideConnection
*/
typedef struct
{
    CTRLMSG_TYPE type;          /**< Type of the message; see \link CTRLMSG_TYPE CTRLMSG_TYPE*/
    struct sockaddr_un addr;    /**< The address of the sender's socket*/
    int len;                    /**< The length of the msg-field*/
    unsigned char msg[500];     /**< The message's content. Without any trailing NUL chars!*/
} CTRLMSG;

#endif
