/**
* @file Slidewindow.h
* @brief Contains the declarations for the Slidewindow-Class an SLIDEWINDOW-GEOMETRY type.
*/

#ifndef SlideWINDOW_H_INCLUDED
#define SlideWINDOW_H_INCLUDED

#include "Global.h"
#include <cstring>
#include <signal.h>

#define RGB(r,g,b) (r << 16 | g << 8 | b)

/**
* This type is used for storing the "recent geometry"
* (i.e. the window's geometry before a certain
* transformating action took place.
* @brief Used to store information about a window's geometry
*/
typedef struct
{
    int x;      /**< The x-coordinate of the window*/
    int y;      /**< The y-coordinate of the window*/
    int width;  /**< The window's width*/
    int height; /**< The window's height*/
} SLIDEWINDOW_GEOMETRY;

/**
* @class SlideWindow
* @brief Defines the Window-Object used in the WM
* This class handles the concept of a window in the WindowManager-Component
* It keeps the decoration (window) belonging to a window and also the methods necessary
* for managing windows (move, resize, close, etc.)
*/
class SlideWindow
{
    public:
        static const unsigned char STATE_HIDDEN;   // = 0,
        static const unsigned char STATE_SHOWN;    // = 1,
        static const unsigned char STATE_MAXIMIZED;// = 1 << 1,
        static const unsigned char STATE_FOCUSED;  // = 1 << 2,
        /**
        * Creates a new Window from a given X-Window.
        * Internally it adds a decoration window (i.e. titlebar with stuff)
        * @param d XDisplay the operation takes place on.
        * @param w The XWindow to create a SlideWindow for
        * @param parent The Parent of this window
        * @param group (Optional) The ID of the SlideWindowGroup it belongs to (default is 0 for none)
        * @param sticky (Optional) If the window should be sticky (i.e. present on all virtual desks) Default is false
        * @param desk (Optional) Which virtual desk the window should be created. Defaults to the current one.
        */
        SlideWindow(Display *d,Window w,Window parent, unsigned char desk = 0, int group = 0, bool sticky = false);

        /**
        * Moves the window to the given new coordinates
        * @param newX The new x-coordinate
        * @param newY The new y-coordinate
        * @param updateGeometry (Optional) True, if the "recent geometry" should be updated to match new coordinates. Default is false.
        */
        void move(int newX, int newY,bool updateGeometry = false);

        /**
        * Puts the window onto the specified virtual desk
        * @param newDesk The number of the virtual desk to put the window onto
        * @param newDesktop The new desktop window
        */
        void putOnDesk(unsigned char newDesk,Window newDesktop);

        /**
        * Closes the Window
        * Internally cleans up.
        * @param killProcess Bool, indicates if the window's process should
        *        be killed. Defaults to true.
        */
        void close(bool killProcess = true);

        /**
        * Resize the window to the given dimensions
        * @param w New Width
        * @param h New Height
        * @param updateGeometry (Optional) Set this to "true", if you want the "recent geometry" to be updated
        *           to reflect the new dimensions.
        */
        void resize(int w, int h,bool updateGeometry = false);

        /**
        * Resize the window by the given factors
        * @param fx The new width will be the old width multiplied by this factor
        * @param fy The new height will be the old height multiplied by this factor
        * @param updateGeometry (Optional) Set this to "true", if you want the "recent geometry" to be updated
        *           to reflect the new dimensions.
        */
        void resizeBy(double fx, double fy,bool updateGeometry = false);

        /**
        * Add the window to the group with the given ID
        * @param groupID ID of the WindowGroup to assign the window to.
        * @todo Stub yet.
        */
        void setGroup(int groupID);

        /**
        * Shows the window (on top)
        * @param maximized (Optional) Tells, if the window should be shown maximized. Defaults to false.
        * @todo Stub yet.
        */
        void show(bool maximized = false);

        /**
        * Hides the window
        * @todo Stub yet.
        */
        void hide();

        /**
        * Makes the window "sticky" - i.e. appear on all virtual desks!
        * @todo Stub yet.
        */
        void makeSticky();

        /**
        * Remove "stickyness" from a window - i.e. make it appear only on its virtual desk.
        * @todo Stub yet.
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

        /**
        * Returns the XWindow to this SlideWindow
        * @param decoWindow (Optional) If "true" is specified here, the deco-window will be returned instead of the actual content window.
        * @return XWindow (handle) of the window (or, if decoWindow is true, the handle for the titlebar-decoration)
        */
        Window getWindow(bool decoWindow=false);

        /**
        * Returns the desk the window is on.
        * @return Number of the desk the window is situated on.
        */
        unsigned char getDesk();

        /**
        * Returns the Title of the window
        */
        char *getTitle();

        /**
        * Returns the x-coordinate of the window
        */
        unsigned int getX();

        /**
        * Returns the y-coordinate of the window
        */
        unsigned int getY();

        /**
        * The current state of the window
        */
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
