#ifndef CMSERVER_H
#define CMSERVER_H
#include"tcpserver.h"

class CMServer:public TCPServer
{
public:
    CMServer();
    ~CMServer();

     virtual void OnRecv(int clientfd,char* msg,int flag) override;
};

#endif // CMSERVER_H
