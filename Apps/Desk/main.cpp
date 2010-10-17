#include "main.h"
#include "desk.h"
#include <iostream>

IMPLEMENT_APP(DeskApp);

bool DeskApp::OnInit()
{
    if(argc < 4)
    {
	std::cout << "Error: Not enough parameters given." << std::endl;
	return false;
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    wxInitAllImageHandlers();

    Desk *d = new Desk(w,h,argv[3]);
    d->Show(true);
    return true;
}

