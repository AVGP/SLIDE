#include "desk.h"

Desk::Desk(int width, int height,wxString bgFile) : wxFrame(NULL,wxID_ANY,wxT("__SLIDE__Desktop"),wxPoint(0,0),wxSize(width,height))
{
    this->width = width;
    this->height = height;
    bg.AddHandler(new wxXPMHandler());
    bg.AddHandler(new wxJPEGHandler());
    bg.AddHandler(new wxPNGHandler());
    bg.LoadFile(bgFile);
    bg.Rescale(width,height);
}

void Desk::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    wxBitmap bm(bg);
    dc.DrawBitmap(bm,0,0,false);
}

BEGIN_EVENT_TABLE(Desk, wxFrame)
    EVT_PAINT(Desk::OnPaint)
END_EVENT_TABLE()
