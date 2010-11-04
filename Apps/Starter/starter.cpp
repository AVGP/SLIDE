#include "starter.h"

Starter::Starter(int width, int height, const wxString& title) //: wxWindow(NULL,wxID_ANY,wxPoint(20,20),wxSize(width-40,height-40),0,title)
{

	wxTopLevelWindow::Create(NULL,wxID_ANY,title,wxPoint(20,20),wxSize(width-40,height-40));

	//Get the config-file:
	char filenamebuffer[512];
	snprintf(filenamebuffer,512,"%s/.slidestarter",getenv("HOME"));
	std::ifstream conf(filenamebuffer);
	if(conf.is_open())
	{
		int x=5,y=5;
		while(!conf.eof())
		{
			std::string iconfile,cmd;
			conf >> iconfile;
			conf >> cmd;
			if(iconfile.length() > 0)
			{
				wxImage img;
				img.AddHandler(new wxPNGHandler());
				img.LoadFile(iconfile.c_str());
                img.ConvertAlphaToMask();
				Icon *icon = new Icon(this,img,cmd.c_str(),x,y);
				icons.push_back(icon);
				if(x < width-58) x+=58;
				else
				{
					x  =  5;
					y += 58;
				}
			}
		}
	}
}

