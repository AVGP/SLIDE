#include "Slideconnection.h"

SlideConnection::SlideConnection(char *localPath,SlideComponent component,bool bindTo)
{
    this->localPath  = localPath;
    this->component  = component;

    memset(&localAddr,0,sizeof(localAddr));
    localAddr.sun_family = AF_UNIX;
    sprintf(localAddr.sun_path,localPath);

    sockIn = socket(AF_UNIX,SOCK_STREAM,0);
    if(sockIn < 0)
    {
        Logger::getInstance()->log((std::string)"Cannot setup incoming socket");
        return;
    }

    if(bindTo)
    {
        if(bind(sockIn,(struct sockaddr *)&localAddr,sizeof(localAddr)) < 0)
        {
            Logger::getInstance()->log((std::string)"Cannot setup bind socket of CommServer to the given path.");
            return;
        }
        listen(sockIn,10);
    }
}

void SlideConnection::sendMessage(CTRLMSG *msg,char *to)
{
    sockOut = socket(AF_UNIX,SOCK_STREAM,0);
    if(sockOut < 0)
    {
        Logger::getInstance()->log((std::string)"Cannot setup outgoing socket");
        return;
    }

    int flags;
    flags = fcntl(sockIn, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sockIn,F_SETFL,flags);
    flags = fcntl(sockOut, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sockOut,F_SETFL,flags);

    struct sockaddr_un addr;
	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path,"%s",to);
    memcpy(&(msg->addr),&localAddr,sizeof(struct sockaddr_un));

    int status;
    char dbg[500];
    sprintf(dbg,"Trying to connect to %s (we're %s)...",addr.sun_path,localAddr.sun_path);
    Logger::getInstance()->log(dbg);
    do
    {
        status = connect(sockOut,(struct sockaddr*)&addr,sizeof(struct sockaddr_un));
    }while(status < 0);
    sprintf(dbg,"Connect returned %i",status);
    Logger::getInstance()->log(dbg);
    if(status < 0)
    {
        return;
    }


	unsigned int sent = 0;
	do
	{
	    sent = send(sockOut,msg,sizeof(CTRLMSG),0);
	} while(sent < sizeof(CTRLMSG));

	sprintf(dbg,"SENT %i of %i bytes",sent,sizeof(CTRLMSG));
	Logger::getInstance()->log(dbg);
	close(sockOut);
}

void SlideConnection::putMessage(CTRLMSG *msg,char *to)
{
    sockOut = socket(AF_UNIX,SOCK_STREAM,0);
    if(sockOut < 0)
    {
        Logger::getInstance()->log((std::string)"Cannot setup outgoing socket");
        return;
    }

    int flags;
    flags = fcntl(sockIn, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sockIn,F_SETFL,flags);
    flags = fcntl(sockOut, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sockOut,F_SETFL,flags);

    struct sockaddr_un addr;
	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path,"%s",to);
    memcpy(&(msg->addr),&localAddr,sizeof(struct sockaddr_un));

    int status;
    char dbg[500];
    sprintf(dbg,"Trying to connect to %s (we're %s)...",addr.sun_path,localAddr.sun_path);
    Logger::getInstance()->log(dbg);
    status = connect(sockOut,(struct sockaddr*)&addr,sizeof(struct sockaddr_un));
    sprintf(dbg,"Connect returned %i",status);
    Logger::getInstance()->log(dbg);
    if(status < 0)
    {
        return;
    }


	unsigned int sent = 0;
    sent = send(sockOut,msg,sizeof(CTRLMSG),0);

	sprintf(dbg,"SENT %i of %i bytes",sent,sizeof(CTRLMSG));
	Logger::getInstance()->log(dbg);
	close(sockOut);
}

CTRLMSG SlideConnection::getMessage(struct sockaddr_un *addr)
{
    /*
    int flags = fcntl(sockIn, F_GETFL,0);
    flags ^= O_NONBLOCK;
    fcntl(sockIn,F_SETFL,flags);
*/
    char dbg[500];

    CTRLMSG msg;
    int clientSock;
    struct sockaddr_un cliAddr;
    socklen_t cliLen = sizeof(cliAddr);
    do
    {
        clientSock = accept(sockIn,(struct sockaddr *)&cliAddr,&cliLen);
    } while(clientSock < 0);
    /*
    flags = fcntl(sockIn, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sockIn,F_SETFL,flags);
*/
/*    sprintf(dbg,"Accept returned: %i",clientSock);
    Logger::getInstance()->log(dbg);*/
    int read = 0;
    do
    {
        read = recv(clientSock,&msg,sizeof(msg),0);//,(struct sockaddr *)addr,&addrlen);
    } while(read < 1);

    sprintf(dbg,"Read %i bytes from %s",read,msg.addr.sun_path);
    Logger::getInstance()->log(dbg);
    close(clientSock);
    return msg;
}

CTRLMSG SlideConnection::peekMessage(struct sockaddr_un *addr)
{
    CTRLMSG msg;
    //Logger::getInstance()->log("Peeks.");
    int clientSock;
    struct sockaddr_un cliAddr;
    socklen_t cliLen = sizeof(cliAddr);

    clientSock = accept(sockIn,NULL,NULL);
    char dbg[500];
    //sprintf(dbg,"Accept returned %i",clientSock);
    //Logger::getInstance()->log(dbg);

    if(clientSock < 0)
    {
        msg.type = NONE;
    }
    else
    {
        int r = recv(clientSock,&msg,sizeof(msg),0);
        if(r < 0)
        {
            msg.type = NONE;
        }
        else
        {
    sprintf(dbg,"Read from %s",msg.addr.sun_path);
    Logger::getInstance()->log(dbg);

        }
        close(clientSock);
    }
    return msg;
}
