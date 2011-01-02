#include "ctrlthread.h"

CtrlThread::CtrlThread(Tray *t)
{
    tray = t;
    conn = new SlideConnection("/tmp/Slide_tray.sock");
    connect(this,SIGNAL(addedWindow(unsigned long,QString)),t,SLOT(addWindowButton(unsigned long,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(deletedWindow(unsigned long)),t,SLOT(deleteWindowButton(unsigned long)),Qt::QueuedConnection);
    
    CTRLMSG msg;
    msg.type = WINDOWLISTINTEREST;
    msg.len  = 0;
    conn->sendMessage(&msg,"/tmp/Slide_wm.sock");
    
}

void CtrlThread::run()
{
    CTRLMSG msg;
    while(1)
    {
        msg = conn->getMessage(NULL);
        if(msg.type == WINDOWLISTCREATEWND)
        {
            SlideWindow *w = (SlideWindow *)malloc(msg.len);
            memcpy(w,msg.msg,msg.len);
            emit addedWindow(w->getWindow(), QString(w->getTitle()));
        }
        else if(msg.type == WINDOWLISTDESTROYWND)
        {
            SlideWindow *w = (SlideWindow *)malloc(msg.len);
            memcpy(w,msg.msg,msg.len);
            emit deletedWindow(w->getWindow());
        }
    }
}
