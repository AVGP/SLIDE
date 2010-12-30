#include "tray.h"

Tray::Tray(int width, int height)
{
    this->setWindowTitle("__SLIDE__Tray");
    this->width  = width;
    this->height = height;
    
    startButton = new QPushButton("Start",this);
    startButton->resize(100,40);
    
    QWidget *windowList = new QWidget(this);
    windowList->resize(width-100,40);
    windowList->move(100,0);
    
    layout = new QHBoxLayout(windowList);
    windowList->setLayout(layout);
    
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    resize(width,40);
    show();
}

void Tray::addWindowButton(unsigned long window, QString str)
{
    WindowButton wb;
    wb.window = window;
    wb.button = new QPushButton(str,this);
    layout->addWidget(wb.button);
    wb.button->show();
    windows.push_back(wb);
}

void Tray::deleteWindowButton(unsigned long window)
{
    std::vector<WindowButton>::iterator iter = windows.begin();
    while(iter != windows.end())
    {
        if((*iter).window == window)
        {
            layout->removeWidget((*iter).button);
            delete (*iter).button;
            windows.erase(iter);
            break;
        }
        ++iter;
    }
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

