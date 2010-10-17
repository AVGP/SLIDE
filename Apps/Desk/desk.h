#ifndef Desk_H_INCLUDED
#define Desk_H_INCLUDED

#include <wx/wx.h>

class Desk : public wxFrame
{
    public:
        Desk(int width, int height, wxString bgFile);
        void OnPaint(wxPaintEvent& WXUNUSED(event));
    private:
        wxImage bg;
        DECLARE_EVENT_TABLE();
        int width,height;
};
#endif // Desk_H_INCLUDED
