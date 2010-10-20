#include "desk.h"

Desk::Desk(int width, int height,wxString bgFile) : wxFrame(NULL,wxID_ANY,wxT("__SLIDE__Desktop"),wxPoint(0,40),wxSize(width,height-40))
{
    this->width = width-40;
    this->height = height;
    bg.AddHandler(new wxXPMHandler());
    bg.AddHandler(new wxJPEGHandler());
    bg.AddHandler(new wxPNGHandler());
    bg.LoadFile(bgFile);
    bg.Rescale(width,height-40);
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
