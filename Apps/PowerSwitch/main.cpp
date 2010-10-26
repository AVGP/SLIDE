#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
	cout << "Too few arguments. Usage: powerswitch (halt | reboot | stopx | restartx)" << endl;
	return 0;
    }

    if(strncmp(argv[1],"halt",4) == 0)
    {
	cout << "Shutting down..." << endl;
	execl("/sbin/halt","halt",(char*)0);
    }
    else if(strncmp(argv[1],"reboot",6) == 0)
    {
	cout << "Rebooting..." << endl;
	execl("/sbin/reboot","reboot",(char *)0);
    }
    else if(strncmp(argv[1],"stopx",5) == 0)
    {
	cout << "Stop X" << endl;
	system("kill `pgrep X`");
    }
    else if(strncmp(argv[1],"restartx",8) == 0)
    {
	cout << "Restarting X..." << endl;
	system("kill `pgrep X` && startx");
    }
    return 0;
}
