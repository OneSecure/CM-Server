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
        DoPlayerLeaveMsg(clientfd);
        perror("PlayerLeave");
        break;
    case NORMAL_FLAG:
    {
        M_Type type=*((M_Type*)msg);
        switch (type)
        {
        case M_InitData:
        {
            DoInitDataMsg(clientfd,(InitData_Msg*)msg);
            perror("InitData");
        }
            break;
        case M_WorldTalk:
        {
            DoWroldTalkMsg(clientfd,(WorldTalk_Msg*)msg);
            perror("worldtalk");
        }
            break;
        case M_PrivateTalk:
        {
            DoPrivateTalkMsg(clientfd,(PrivateTalk_Msg*)msg);
            perror("privatetalk");
        }
            break;
        case M_InitPos:
        {
            DoInitPosMsg(clientfd,(InitPos_Msg*)msg);
            perror("InitsPos");
        }
            break;
        case M_PlayerLeave:
        {
            DoPlayerLeaveMsg(clientfd);
            perror("PlayerLeave");
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

void CMServer::DoWroldTalkMsg(const int& fd,WorldTalk_Msg* msg)
{
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {
            write(var.first,(char*)&msg,sizeof(WorldTalk_Msg));
        }
    }
}

void CMServer::DoPrivateTalkMsg(const int& fd,PrivateTalk_Msg* msg)
{
    write(fd,(char*)&msg,sizeof(PrivateTalk_Msg));
}

void CMServer::DoInitDataMsg(const int& fd,InitData_Msg* rlmsg)
{
    m_playermaps[fd].playername=rlmsg->playername;
    m_playermaps[fd].rolename=rlmsg->rolename;
    m_playermaps[fd].playertype=rlmsg->playertype;
    m_playermaps[fd].curmap=rlmsg->curmap;
    m_playermaps[fd].blood=rlmsg->blood;
    m_playermaps[fd].mana=rlmsg->mana;
    m_playermaps[fd].attack=rlmsg->attack;
    m_playermaps[fd].defense=rlmsg->defense;
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {
           InitData_Msg smsg;
           smsg.type=M_InitData;
           strcpy(smsg.playername,m_playermaps[var.first].playername.c_str());
           strcpy(smsg.rolename,m_playermaps[var.first].rolename.c_str());
           strcpy(smsg.playertype,m_playermaps[var.first].playertype.c_str());
           smsg.curmap=m_playermaps[var.first].curmap;
           smsg.blood=m_playermaps[var.first].blood;
           smsg.mana=m_playermaps[var.first].mana;
           smsg.attack=m_playermaps[var.first].attack;
           smsg.defense=m_playermaps[var.first].defense;
           smsg.fd=var.first;
           SendMsg(fd,(char*)&smsg,sizeof(smsg));
           rlmsg->fd=fd;
           SendMsg(var.first,(char*)rlmsg,sizeof(InitData_Msg));
        }
    }
}

void CMServer::DoInitPosMsg(const int& fd,InitPos_Msg* msg)
{
    m_playermaps[fd].x=msg->x;
    m_playermaps[fd].y=msg->y;
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {
            msg->fd=fd;
            SendMsg(var.first,(char*)msg,sizeof(InitPos_Msg));
            InitPos_Msg smsg;
            smsg.fd=var.first;
            smsg.type=M_InitPos;
            smsg.x=m_playermaps[var.first].x;
            smsg.y=m_playermaps[var.first].y;
            SendMsg(fd,(char*)&smsg,sizeof(smsg));
        }
    }
}

void CMServer::DoPlayerLeaveMsg(const int& fd)
{
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {
            PlayerLeave_Msg msg;
            msg.type=M_PlayerLeave;
            msg.fd=fd;
            SendMsg(var.first,(char*)&msg,sizeof(msg));
        }
    }
    m_playermaps.erase(fd);
}
