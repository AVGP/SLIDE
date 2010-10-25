#include "main.h"
#include "tray.h"
#include "ctrlthread.h"
#include <iostream>

IMPLEMENT_APP(TrayApp);

bool TrayApp::OnInit()
{
    if(argc < 3)
    {
	std::cout << "Error: Not enough parameters given." << std::endl;
	return false;
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    wxInitAllImageHandlers();
    CtrlThread *ctrl = new CtrlThread();
    if(ctrl->Create() == wxTHREAD_NO_ERROR)
    {
        ctrl->Run();
    }

    Tray *d = new Tray(w,h);
    d->Show(true);
    return true;
}

