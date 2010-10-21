#ifndef Starter_H_INCLUDED
#define Starter_H_INCLUDED
#include <wx/wx.h>
#include <vector>
#include <fstream>
#include "icon.h"

class Starter : public wxFrame
{
    public:
        Starter(int width, int height, const wxString& title);
    private:
	std::vector<Icon *> icons;
};
#endif // Starter_H_INCLUDED
