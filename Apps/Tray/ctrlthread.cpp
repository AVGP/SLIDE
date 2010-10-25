#include "ctrlthread.h"

CtrlThread::CtrlThread() : wxThread()
{
}

void *CtrlThread::Entry()
{
	while(1)
	{
		std::cout << "Running." << std::endl;
		sleep(1);
	}
	return 0;
}

