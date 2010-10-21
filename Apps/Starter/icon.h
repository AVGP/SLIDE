#include <wx/wx.h>
#include <unistd.h>

class Icon : public wxBitmapButton
{
	public:
	Icon(wxWindow *parent,wxImage img,const char *cmd,int x, int y);
	void onClick(wxCommandEvent& WXUNUSED(event));
	private:
	char *cmd;
};

