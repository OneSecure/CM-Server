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
    void DoWroldTalkMsg(const int& fd,WorldTalk_Msg* msg);

    /*
     *deal with private talk
     *@param fd:dest socket fd
     */
    void DoPrivateTalkMsg(const int& fd,PrivateTalk_Msg* msg);

    /*
     * deal with initdata Msg
     * @param fd:msg sender fd
     * @param msg:
     */
    void DoInitDataMsg(const int& fd,InitData_Msg* msg);

    /*
     * deal with initialize player position msg
     * @param fd:sender fd
     * @param msg:initialize position msg
     */
    void DoInitPosMsg(const int& fd,InitPos_Msg* msg);

    /*
     * deal player leave msg
     * @param fd:leave player fd
     */
    void DoPlayerLeaveMsg(const int& fd);
private:
    std::map<int,Player_Info> m_playermaps;
};

#endif // CMSERVER_H
