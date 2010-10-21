#include "icon.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

Icon::Icon(wxWindow *parent,wxImage img,const char *cmd,int x, int y) : wxBitmapButton(parent,wxID_ANY,wxBitmap(img),wxPoint(x,y))
{
	this->cmd = (char *)malloc(strlen(cmd)+1);
	memset(this->cmd,0,strlen(cmd)+1);
	strncpy(this->cmd,cmd,strlen(cmd));	
	Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Icon::onClick));
}

void Icon::onClick(wxCommandEvent& WXUNUSED(event))
{
	std::cout << this->cmd << std::endl;
	std::cout << "Now..." << std::endl;
	wxExecute(wxT(this->cmd));
}
