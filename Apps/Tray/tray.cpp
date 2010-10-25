#include "tray.h"

Tray::Tray(int width, int height) : wxFrame(NULL,wxID_ANY,wxT("__SLIDE__Tray"),wxPoint(0,0),wxSize(width,40))
{
    this->width = width;
    this->startButton = new wxButton(this,wxID_ANY,wxT("Start"),wxPoint(0,0),wxSize(100,40),wxBU_EXACTFIT);
    this->windowList  = new wxFrame(this,wxID_ANY,wxT(""),wxPoint(100,0),wxSize(width-100,40));
    Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Tray::OnClickStart));
}

void Tray::OnClickStart(wxCommandEvent& WXUNUSED(event))
{
    char sw[5],sh[5];
    snprintf(sw,5,"%d",width);
    snprintf(sh,5,"%d",height);
    if(fork() == 0)
    {
        execl("/usr/bin/SlideApps/Starter","Starter",sw,sh,(char *)0);
    }
}

