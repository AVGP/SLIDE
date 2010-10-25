#include <wx/wx.h>
#include "../../Slideconnection.h"
#include "../../Slidewindow.h"
#include <iostream>
#include <unistd.h>

class CtrlThread : public wxThread
{
	public:
	CtrlThread();
	void *Entry();
	private:
	SlideConnection *connection;
};
