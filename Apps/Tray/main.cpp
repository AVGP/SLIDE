#include "main.h"
#include "tray.h"
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

    Tray *d = new Tray(w,h);
    d->Show(true);
    return true;
}

