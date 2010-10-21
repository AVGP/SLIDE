#include "starter.h"

Starter::Starter(int width, int height, const wxString& title) : wxFrame(NULL,wxID_ANY,title,wxPoint(20,20),wxSize(width-40,height-40))
{
	Raise();
	
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
