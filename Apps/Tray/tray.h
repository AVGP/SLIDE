#ifndef Tray_H_INCLUDED
#define Tray_H_INCLUDED

#include <wx/wx.h>

class Tray : public wxFrame
{
    public:
        Tray(int width, int height);
        void OnPaint(wxPaintEvent& WXUNUSED(event));
    private:
        wxImage bg;
        DECLARE_EVENT_TABLE();
        int width,height;
};
#endif // Tray_H_INCLUDED
