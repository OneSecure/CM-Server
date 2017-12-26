#include "cmserver.h"
#include"string.h"

CMServer::CMServer():
    TCPServer(NULL,NULL,6036,EPOLL_MODE,10)
{

}

CMServer::~CMServer()
{

}

void CMServer::OnRecv(int clientfd, char *msg, int flag)
{
    switch (flag)
    {
    case DISCONN_FLAG:
        break;
    case NORMAL_FLAG:
        write(clientfd,"recv",strlen("recv"));
        break;
    default:
        break;
    }
}
