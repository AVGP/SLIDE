#include "Slidewindow.h"

SlideWindow::SlideWindow(Display *d, Window w, Window parent, int group, bool sticky, unsigned char desk)
{
    XWindowAttributes attr;
    char *t_name;

    XGetWindowAttributes(d,w,&attr);
    XFetchName(d,w,&t_name);
//Debug
    char msg[255];
    sprintf(msg,"Mapping Request for %s",t_name);
    Logger::getInstance()->log(msg);
//
    this->wndWindow     = w;
    if(strncmp(t_name,"__SLIDE__",9) != 0)
    {
        this->wndDecoration = XCreateSimpleWindow(d,
                                                DefaultRootWindow(d),
                                                attr.x,
                                                attr.y,
                                                attr.width+2,
                                                attr.height+22,
                                                1,
                                                RGB(180,180,180),
                                                RGB(240,240,240));
        this->width         = attr.width+2;
        this->height        = attr.height+22;

        XSetStandardProperties(d,wndDecoration,t_name,t_name,None,NULL,0,NULL);
//        XClassHint classHint;
        XReparentWindow(d,w,wndDecoration,0,20);
        XMapRaised(d,wndDecoration);

    }
    else
    {
        this->wndDecoration = None;
        this->width         = attr.width;
        this->height        = attr.height;
    }

    this->groupID       = group;
    this->sticky        = sticky;
    this->desk          = desk;
    this->disp          = d;
    this->x             = attr.x;
    this->y             = attr.y;

    XMapRaised(d,w);

}

void SlideWindow::move(int newX, int newY)
{}

void SlideWindow::resize(int w, int h)
{}

void SlideWindow::putOnDesk(unsigned char newDesk)
{}

void SlideWindow::close()
{}

void SlideWindow::hide()
{}

void SlideWindow::makeSticky()
{}

void SlideWindow::makeUnsticky()
{}

void SlideWindow::setGroup(int groupID)
{}

void SlideWindow::show(bool maximized)
{}
