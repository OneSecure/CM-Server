#ifndef CMSERVER_H
#define CMSERVER_H
#include"tcpserver.h"
#include"sharedata.h"
#include<map>

class CMServer:public TCPServer
{
public:
    CMServer();
    ~CMServer();

     virtual void OnRecv(int clientfd,char* msg,int flag) override;

    /*
     *
     */
    void DoWroldTalkMsg(const int& fd);

    /*
     *
     */
    void DoPrivateTalkMsg(const int& fd);
private:
    std::map<int,Player_Info> m_playermaps;
};

#endif // CMSERVER_H
