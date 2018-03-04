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
     *deal with world talk msg
     *@param fd:send msg's socket fd
     */
    void DoWroldTalkMsg(const int& fd,WorldTalk_Msg& msg);

    /*
     *deal with private talk
     *@param fd:dest socket fd
     */
    void DoPrivateTalkMsg(const int& fd,PrivateTalk_Msg& msg);
private:
    std::map<int,Player_Info> m_playermaps;
};

#endif // CMSERVER_H
