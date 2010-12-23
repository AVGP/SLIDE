#ifndef Tray_H_INCLUDED
#define Tray_H_INCLUDED

#include <wx/wx.h>

class Tray : public wxFrame
{
    public:
        Tray(int width, int height);
        void OnClickStart(wxCommandEvent& WXUNUSED(event));
    private:
        wxButton *startButton;
	wxFrame *windowList;
        int width,height;
};
#endif // Tray_H_INCLUDED
