#include "tray.h"

Tray::Tray(int width, int height)
{
    this->setWindowTitle("__SLIDE__Tray");
    this->width  = width;
    this->height = height;
    this->startButton = new QPushButton("Start",this);
    
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    resize(width,40);
    show();
}

void Tray::startClicked()
{
    char sw[5],sh[5];
    snprintf(sw,5,"%d",width);
    snprintf(sh,5,"%d",height);
    if(fork() == 0)
    {
        execl("/usr/bin/SlideApps/Starter","Starter",sw,sh,(char *)0);
    }
}

