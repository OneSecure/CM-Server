#include "cmserver.h"
#include<string.h>

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
    {
        M_Type type=*((M_Type*)msg);
        switch (type)
        {
        case M_EntryGame:
        {
            EntryGame_Msg* rlmsg=(EntryGame_Msg*)msg;
            Player_Info info;
            m_playermaps[clientfd]=info;
            m_playermaps[clientfd].playername=rlmsg->playername;
            m_playermaps[clientfd].rolename=rlmsg->rolename;
            perror("EntryGame");
        }
            break;
        case M_InitData:
        {
            InitData_Msg* rlmsg=(InitData_Msg*)msg;
            m_playermaps[clientfd].blood=rlmsg->blood;
            m_playermaps[clientfd].mana=rlmsg->mana;
            m_playermaps[clientfd].attack=rlmsg->attack;
            m_playermaps[clientfd].defense=rlmsg->defense;
            perror("InitData");
        }
            break;
        case M_WorldTalk:
        {

            WorldTalk_Msg* rlmsg=(WorldTalk_Msg*)msg;
            perror(rlmsg->msg);
        }
            break;
        case M_PrivateTalk:
        {
            PrivateTalk_Msg* rlmsg=(PrivateTalk_Msg*)msg;
            perror(rlmsg->msg);
        }
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

void CMServer::DoWroldTalkMsg(const int& fd)
{
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {

        }
    }
}

void CMServer::DoPrivateTalkMsg(const int& fd)
{
    for(auto var:m_playermaps)
    {
        if(var.first==fd)
        {

        }
    }
}
