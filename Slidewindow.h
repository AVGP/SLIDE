/**
* @file Slidewindow.h
* @brief Contains the declarations for the Slidewindow-Class
*/

#ifndef SlideWINDOW_H_INCLUDED
#define SlideWINDOW_H_INCLUDED

#include "Global.h"
#include <cstring>

#define RGB(r,g,b) (r << 16 | g << 8 | b)

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} SLIDEWINDOW_GEOMETRY;

/**
* @class SlideWindow
* This class handles the concept of a window in the WindowManager-Component
* It keeps the decoration (window) belonging to a window and also the methods necessary
* for managing windows (move, resize, close, etc.)
*/
class SlideWindow
{
    public:
        static const unsigned char STATE_SHOWN;    // = 1,
        static const unsigned char STATE_MAXIMIZED;// = 1 << 1,
        static const unsigned char STATE_FOCUSED;  // = 1 << 2,
        /**
        * Creates a new Window from a given X-Window.
        * Internally it adds a decoration window (i.e. titlebar with stuff)
        * @param The X-Window to create a SlideWindow for
        * @param (Optional) The ID of the SlideWindowGroup it belongs to
        * @param (Optional) If the window should be sticky (i.e. present on all virtual desks)
        * @param (Optional) Which virtual desk the window should be created. Defaults to the current one.
        */
        SlideWindow(Display *d,Window w,Window parent, unsigned char desk = 0, int group = 0, bool sticky = false);

        /**
        * Moves the window to the given new coordinates
        * @param The new x-coordinate
        * @param The new y-coordinate
        */
        void move(int newX, int newY,bool updateGeometry = false);

        /**
        * Puts the window onto the specified virtual desk
        * @param The number of the virtual desk to put the window onto
        * @param The new desktop window
        */
        void putOnDesk(unsigned char newDesk,Window newDesktop);

        /**
        * Closes the Window
        * Internally cleans up.
        */
        void close();

        /**
        * Resize the window to the given dimensions
        * @param New Width
        * @param New Height
        */
        void resize(int w, int h,bool updateGeometry = false);

        /**
        * Add the window to the group with the given ID
        * @param ID of the WindowGroup to assign the window to.
        */
        void setGroup(int groupID);

        /**
        * Shows the window (on top)
        * @param (Optional) Tells, if the window should be shown maximized. Defaults to false.
        */
        void show(bool maximized = false);

        /**
        * Hides the window
        */
        void hide();

        /**
        * Makes the window "sticky" - i.e. appear on all virtual desks!
        */
        void makeSticky();

        /**
        * Remove "stickyness" from a window - i.e. make it appear only on its virtual desk.
        */
        void makeUnsticky();

        /**
        * Moves and resizes the window according to the recent geometry stored for it.
        */
        void restoreGeometry();

        /**
        * Draws the window decoration on exposure
        */
        void drawDecoration(bool focus);

        Window getWindow(bool decoWindow=false);
        unsigned char getDesk();
        char *getTitle();

        unsigned char state;
    private:
        Display *disp;
        Window wndWindow;
        Window wndDecoration;
        Window wndClose;
        Window wndMaximize;
        Window wndMinimize;
        unsigned short groupID;
        int x,y,width,height;
        SLIDEWINDOW_GEOMETRY recentGeometry;
        bool sticky; //Shows up on all desks
        unsigned char desk; //Which desk this window is on?
        char title[255];
};

#endif // SlideWINDOW_H_INCLUDED
