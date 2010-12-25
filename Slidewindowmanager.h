/**
* @file Slidewindowmanager.h
* @brief Contains the Window Manager for Slide
*/
#ifndef SlideWINDOWMANAGER_H_INCLUDED
#define SlideWINDOWMANAGER_H_INCLUDED

#include "Global.h"
#include "Slideconnection.h"
#include "./Slidewindow.h"
#include <vector>
#include <string>
#include <cmath>

#define RGB(r,g,b) (r << 16 | g << 8 | b)

/**
* @class SlideWindowManager
* @brief The Window-Manager, handling all the windows and virtual desks
* This class encapsules the WindowManager for Slide.
* It runs in a separate process, communicating via its ctrl-member
*/
class SlideWindowManager
{
    public:
        /**
        * Creates a new instance of the SlideWindowManager
        * @param Specifies, if we're in debug-mode. Defaults to false
        */
        SlideWindowManager(bool debug = false);

        /**
        * Runs the WindowManager - includes the event- and CtrlComm-Loop
        * @return Returns true after regular shutdown, else returns false
        */
        bool run();

        /**
        * This method allows the Window-Manager to receive X11-Error-Notifications.
        * It also prevents the X-Server to crash, when some of those errors occur.
        */
        static int *errorHandler(Display *d, XErrorEvent *e);
    private:
        void focusWindow(XEvent *e);
        void createWindow(XEvent *e);
        void closeWindow(XEvent *e);
        void moveWindow(XEvent *e);
        void resizeWindow(XEvent *e);
        void maximizeWindow(XEvent *e);
        void tileWindows();
        void untileWindows();
        void tileWorkspaces();

        SlideConnection *ctrl;
        std::vector<SlideWindow *> windows;
        Display *disp;
        Screen *screen;
        Window *desktop;
        SlideWindow *focusedWindow;
        std::vector<std::string> windowChangeListeners; //Apps listening for changes in window list (create/destroy)
        unsigned int screenWidth,screenHeight;
        unsigned char workspaces,numWorkspaces,currentWorkspace;
};

#endif // SlideWINDOWMANAGER_H_INCLUDED
