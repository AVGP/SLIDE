#include "Slideconnection.h"

SlideConnection::SlideConnection(char *localPath,SlideComponent component,bool bindTo)
{
    this->localPath  = localPath;
    this->component  = component;

    sock = socket(AF_UNIX,SOCK_DGRAM,0);
    if(sock < 0)
    {
        Logger::getInstance()->log((std::string)"Cannot setup socket for CommServer");
        return;
    }

    int flags;
    flags = fcntl(sock, F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(sock,F_SETFL,flags);

    if(bindTo)
    {
        memset(&localAddr,0,sizeof(localAddr));
        localAddr.sun_family = AF_UNIX;
        sprintf(localAddr.sun_path,localPath);
        if(bind(sock,(struct sockaddr *)&localAddr,sizeof(localAddr)) < 0)
        {
            Logger::getInstance()->log((std::string)"Cannot setup bind socket of CommServer to the given path.");
            return;
        }
        listen(sock,2);
    }
}

unsigned int SlideConnection::bytesReadable()
{
    return 0;
}

void SlideConnection::sendMessage(CTRLMSG *msg,char *to)
{
    struct sockaddr_un addr;
	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path,"%s",to);
    memcpy(&(msg->addr),&localAddr,sizeof(struct sockaddr_un));
	sendto(sock,msg,sizeof(CTRLMSG),0,(struct sockaddr *)&addr,sizeof(addr));
}

CTRLMSG SlideConnection::getMessage(struct sockaddr_un *addr)
{
    CTRLMSG msg;
    socklen_t addrlen;
    int read = 0;
    do
    {
        read = recvfrom(sock,&msg,sizeof(msg),0,(struct sockaddr *)addr,&addrlen);
    } while(read < 1);
    char dbg[100];
    sprintf(dbg,"Read %i bytes from %i",read,sizeof(CTRLMSG));
    Logger::getInstance()->log(dbg);
    return msg;
}

CTRLMSG SlideConnection::peekMessage(struct sockaddr_un *addr)
{
    CTRLMSG msg;
    socklen_t addrlen;
    int r = recvfrom(sock,&msg,sizeof(msg),0,(struct sockaddr *)addr,&addrlen);
    if(r < 0)
    {
        msg.type = NONE;
    }
    return msg;
}
