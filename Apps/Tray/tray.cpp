#include "tray.h"

Tray::Tray(int width, int height) : wxFrame(NULL,wxID_ANY,wxT("__SLIDE__Tray"),wxPoint(0,0),wxSize(width,height))
{
    this->width = width;
    this->height = height;
    bg.AddHandler(new wxXPMHandler());
    bg.AddHandler(new wxJPEGHandler());
    bg.AddHandler(new wxPNGHandler());
    bg.LoadFile(bgFile);
    bg.Rescale(width,height);
}

void Tray::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    wxBitmap bm(bg);
    dc.DrawBitmap(bm,0,0,false);
}

BEGIN_EVENT_TABLE(Tray, wxFrame)
    EVT_PAINT(Tray::OnPaint)
END_EVENT_TABLE()
