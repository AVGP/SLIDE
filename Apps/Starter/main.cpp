#include "main.h"
#include "starter.h"

IMPLEMENT_APP(StarterApp);

bool StarterApp::OnInit()
{
    if(argc < 3) return false;

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    Starter *s = new Starter(w,h,"Starter");
    s->Show(true);
    return true;
}
