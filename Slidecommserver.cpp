#include "Slidecommserver.h"

SlideCommServer::SlideCommServer(const char *path)
{
	struct sockaddr_un addr;
	sock = socket(AF_UNIX,SOCK_STREAM,0);
	if(sock < 0)
	{
	    Logger::getInstance()->log("Cannot setup socket for CommServer");
		exit(EXIT_FAILURE);
	}

	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path,"/tmp/TEST_sock_serv");
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
	    Logger::getInstance()->log("Cannot setup bind socket of CommServer to the given path.");
		exit(EXIT_FAILURE);
	}
    listen(sock,100);
}

SlideCommServer::~SlideCommServer()
{
    close(sock);
}

bool SlideCommServer::setup(const char *path)
{
	struct sockaddr_un addr;
	sock = socket(AF_UNIX,SOCK_STREAM,0);
	if(sock < 0)
	{
	    Logger::getInstance()->log("Cannot setup socket for CommServer");
		exit(EXIT_FAILURE);
	}

	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path,"/tmp/TEST_sock_serv");
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
	    Logger::getInstance()->log("Cannot setup bind socket of CommServer to the given path.");
		exit(EXIT_FAILURE);
	}
    listen(sock,100);
}

bool SlideCommServer::run()
{

}
