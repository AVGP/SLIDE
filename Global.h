/**
* @file Global.h
* @brief Contains globally useful includes and defines the message-types
*/
#ifndef _Slide_GLOBAL_H_
#define _Slide_GLOBAL_H_

#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xatom.h>
//#include <X11/Xkeysym.h>

#include "Logger.h"

enum CTRLMSG
{
    STARTREQUEST,           //Sent from Slide or WM, requesting component to start
    SHUTDOWNREQUEST,        //Sent from Clientapplication, requesting environment shutdown
    CONFIGRELOADREQUEST,    //Sent from Clientapplication, wishing to refresh the config
    CLOSENOTIFY,            //Sent from the Serversocket when closing
    MOVEREQUEST,            //Sent from Clientapplication notifying WM to move a given window
    RESIZEREQUEST,          //Sent from Clientapplication notifying WM to resize a given window
    CLOSEWINDOWREQUEST,     //Sent from Clientapplication notifying the wish to close a given window
    HIDEWINDOWREQUEST,      //Sent from Clientapplication notifying the wish to hide a given window
    SHOWWINDOWREQUEST       //Sent from Clientapplication notifying the wish to show a given window

};

#endif
