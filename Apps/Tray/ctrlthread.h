#include <wx/wx.h>
#include "../../Slideconnection.h"
#include "../../Slidewindow.h"
#include "../../Logger.h"
#include <vector>
#include <cstring>
#include <unistd.h>

class CtrlThread : public wxThread
{
	public:
	CtrlThread(wxPanel *panel,int width);
	void *Entry();
	private:
	int panelwidth;
	wxPanel *windowList;
	std::vector<wxButton *> windowButtons;
	SlideConnection *connection;
};
