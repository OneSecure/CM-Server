#include "cmserver.h"
#include<iostream>
#include<string.h>

using namespace std;

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
        break;
    case NORMAL_FLAG:
    {
        M_Type type=*((M_Type*)msg);
        switch (type)
        {
        case M_InitData:
        {
            DoInitDataMsg(clientfd,(InitData_Msg*)msg);
        }
            break;
        case M_WorldTalk:
        {
            DoWroldTalkMsg(clientfd,(WorldTalk_Msg*)msg);
        }
            break;
        case M_PrivateTalk:
        {
            DoPrivateTalkMsg(clientfd,(PrivateTalk_Msg*)msg);
        }
            break;
        case M_InitPos:
        {
            DoInitPosMsg(clientfd,(InitPos_Msg*)msg);
        }
            break;
        case M_PlayerLeave:
        {
            DoPlayerLeaveMsg(clientfd);
        }
            break;
        case M_MoveTo:
        {
            DoPlayerMoveToMsg(clientfd,(MoveTo_Msg*)msg);
        }
            break;
        case M_VerifyPos:
        {
            DoPlayerVerifyPosMsg(clientfd,(VerifyPos_Msg*)msg);
        }
            break;
        case M_UpdateData:
        {
            DoUpdateDataMsg(clientfd,(UpdateData_Msg*)msg);
        }
            break;
        case M_UpdateMap:
        {
            DoUpdateMapMsg(clientfd,(UpdateMap_Msg*)msg);
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
    cout<<m_playermaps[fd].rolename<<" in the world say: "<<msg->msg<<endl;
    msg->fd=fd;
    for(auto var:m_playermaps)
    {
        if(var.first!=fd)
        {
            write(var.first,(char*)msg,sizeof(WorldTalk_Msg));
        }
    }
}

void CMServer::DoPrivateTalkMsg(const int& fd,PrivateTalk_Msg* msg)
{
    if(m_playermaps.find(msg->dest)!=m_playermaps.end())
    {
        msg->fd=fd;
        cout<<m_playermaps[fd].rolename<<"say "<<msg->msg<<" to "<<m_playermaps[msg->dest].rolename<<endl;
        write(msg->dest,(char*)msg,sizeof(PrivateTalk_Msg));
    }
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
    m_playermaps[fd].grade=rlmsg->grade;
    cout<<rlmsg->playername<<":"<<rlmsg->rolename<<" online!"<<endl;
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
           smsg.grade=m_playermaps[var.first].grade;
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
    cout<<m_playermaps[fd].rolename<<" initialize position("<<msg->x<<","<<msg->y<<")"<<endl;
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
    cout<<m_playermaps[fd].rolename<<" off line"<<endl;
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

 void CMServer::DoPlayerMoveToMsg(const int& fd,MoveTo_Msg* msg)
 {
     cout<<m_playermaps[fd].rolename<<" move to position("<<msg->x<<","<<msg->y<<")"<<endl;
     for(auto var:m_playermaps)
     {
         if(var.first!=fd)
         {
             msg->fd=fd;
             SendMsg(var.first,(char*)msg,sizeof(MoveTo_Msg));
         }
     }
 }

 void CMServer::DoPlayerVerifyPosMsg(const int& fd,VerifyPos_Msg* msg)
 {
     cout<<m_playermaps[fd].rolename<<" verify position("<<msg->x<<","<<msg->y<<")"<<endl;
     for(auto var:m_playermaps)
     {
         if(var.first!=fd)
         {
             msg->fd=fd;
             SendMsg(var.first,(char*)msg,sizeof(VerifyPos_Msg));
         }
         else
         {
             m_playermaps[var.first].x=msg->x;
             m_playermaps[var.first].y=msg->y;
         }
     }
 }

 void CMServer::DoUpdateDataMsg(const int& fd,UpdateData_Msg* msg)
 {
     m_playermaps[fd].attack=msg->attack;
     m_playermaps[fd].blood=msg->blood;
     m_playermaps[fd].defense=msg->defense;
     m_playermaps[fd].grade=msg->grade;
     m_playermaps[fd].mana=msg->mana;
     msg->fd=fd;
     cout<<m_playermaps[fd].rolename<<" update data"<<endl;
     for(auto var:m_playermaps)
     {
         if(var.first!=fd)
         {
             SendMsg(var.first,(char*)msg,sizeof(UpdateData_Msg));
         }
     }
 }

 void CMServer::DoUpdateMapMsg(const int& fd,UpdateMap_Msg* msg)
 {
     m_playermaps[fd].curmap=msg->curmap;
     msg->fd=fd;
     cout<<m_playermaps[fd].rolename<<" update current map"<<endl;
     for(auto var:m_playermaps)
     {
         if(var.first!=fd)
         {
             SendMsg(var.first,(char*)msg,sizeof(UpdateMap_Msg));
         }
     }
 }
