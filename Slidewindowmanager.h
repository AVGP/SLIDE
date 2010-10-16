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

/**
* @class SlideWindowManager
* This class encapsules the WindowManager for Slide.
* It runs in a separate process, communicating via its ctrl-member
*/
class SlideWindowManager
{
    public:
        /**
        * Creates a new instance of the SlideWindowManager
        */
        SlideWindowManager();

        /**
        * Runs the WindowManager - includes the event- and CtrlComm-Loop
        * @return Returns true after regular shutdown, else returns false
        */
        bool run();
    private:
        void focusWindow(XEvent *e);
        void createWindow(XEvent *e);
        void closeWindow(XEvent *e);
        void moveWindow(XEvent *e);
        void resizeWindow(XEvent *e);

        SlideConnection *ctrl;
        std::vector<SlideWindow *> windows;
        Display *disp;
        Screen *screen;
        Window *desktop;
};

#endif // SlideWINDOWMANAGER_H_INCLUDED
