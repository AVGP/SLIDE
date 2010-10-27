#include "ctrlthread.h"

CtrlThread::CtrlThread(wxPanel *panel,int width) : wxThread()
{
    connection = new SlideConnection((char *)"/tmp/Slide_tray.sock",COMP_CLIENTAPPS);
    windowList = panel;
    panelwidth = width;
    //windowButtons.clear();
}

void *CtrlThread::Entry()
{
    CTRLMSG msg;
    msg.type = WINDOWLISTINTEREST;
    msg.len  = 0;
    connection->sendMessage(&msg,(char *)"/tmp/Slide_wm.sock");
    struct sockaddr_un addr;
    char dbg[500];

	while(1)
	{

	    CTRLMSG msg = connection->getMessage(&addr);
	    sprintf(dbg,"Got msg. LEN %i",msg.len);
	    Logger::getInstance()->log(dbg);
	    if(msg.type == WINDOWLISTCREATEWND)
	    {
	        SlideWindow *w = (SlideWindow*)malloc(sizeof(*w));
	        memcpy(w,msg.msg,msg.len);

            int width = 100;//panelwidth / (windowButtons.size()+1);

            for(unsigned int i=0;i<windowButtons.size();i++)
            {
                windowButtons[i]->SetSize(wxRect(i*100,0,100,40));//width,40));
            }

            wxButton *windowText = new wxButton(windowList,wxID_ANY,wxT(w->getTitle()),wxPoint(width*windowButtons.size(),0),wxSize(width,40),wxBU_EXACTFIT);
            windowButtons.push_back(windowText);
        }
	}
	return 0;
}

