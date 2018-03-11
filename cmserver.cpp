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
            DoInitDataMsg(clientfd,(InitData_Msg*)msg);
            break;
        case M_WorldTalk:
            DoWroldTalkMsg(clientfd,(WorldTalk_Msg*)msg);
            break;
        case M_PrivateTalk:
            DoPrivateTalkMsg(clientfd,(PrivateTalk_Msg*)msg);
            break;
        case M_InitPos:
            DoInitPosMsg(clientfd,(InitPos_Msg*)msg);
            break;
        case M_PlayerLeave:
            DoPlayerLeaveMsg(clientfd);
            break;
        case M_MoveTo:
            DoPlayerMoveToMsg(clientfd,(MoveTo_Msg*)msg);
            break;
        case M_VerifyPos:
            DoPlayerVerifyPosMsg(clientfd,(VerifyPos_Msg*)msg);
            break;
        case M_UpdateData:
            DoUpdateDataMsg(clientfd,(UpdateData_Msg*)msg);
            break;
        case M_TeamApply:
            doTeamApplyMsg(clientfd,(TeamApply_Msg*)msg);
            break;
        case M_RefuseTeam:
            DoRefuseTeamMsg(clientfd,(RefuseTeam_Msg*)msg);
            break;
        case M_AgreeTeam:
            DoAgreeTeamMsg(clientfd,(AgreeTeam_Msg*)msg);
            break;
        case M_TeamMove:
            DoTeamMoveMsg(clientfd,(TeamMove_Msg*)msg);
            break;
        case M_TeamGotoMap:
            DoTeamGotoMapMsg(clientfd,(TeamGotoMap_Msg*)msg);
            break;
        case M_DissolveTeam:
            DoDissolveTeamMsg(clientfd,(TeamManage_Msg*)msg);
            break;
        case M_TeamFight:

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
        cout<<m_playermaps[fd].rolename<<" say "<<msg->msg<<" to "<<m_playermaps[msg->dest].rolename<<endl;
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
    m_playermaps[fd].curmap=msg->curmap;
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
            smsg.curmap=m_playermaps[var.first].curmap;
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

 void CMServer::doTeamApplyMsg(const int& fd,TeamApply_Msg* msg)
 {
     msg->fd=fd;
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<m_playermaps[fd].rolename<<" apply to make a team with "<<m_playermaps[msg->dest].rolename<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(TeamApply_Msg));
     }
 }

 void CMServer::DoRefuseTeamMsg(const int& fd,RefuseTeam_Msg* msg)
 {
     msg->fd=fd;
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<m_playermaps[fd].rolename<<" refused "<<m_playermaps[msg->dest].rolename<<" team apply"<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(RefuseTeam_Msg));
     }
 }


 void CMServer::DoAgreeTeamMsg(const int& fd,AgreeTeam_Msg* msg)
 {
     msg->fd=fd;
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<m_playermaps[fd].rolename<<" agree "<<m_playermaps[msg->dest].rolename<<" team apply"<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(AgreeTeam_Msg));
     }
 }

 void CMServer::DoTeamMoveMsg(const int& fd,TeamMove_Msg* msg)
 {
     msg->fd=fd;
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<m_playermaps[fd].rolename<<" team header move "<<m_playermaps[msg->dest].rolename<<" to ("<<msg->x<<","<<msg->y<<")"<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(TeamMove_Msg));
     }
 }

 void CMServer::DoTeamGotoMapMsg(const int& fd,TeamGotoMap_Msg* msg)
 {
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<m_playermaps[fd].rolename<<" team header move "<<m_playermaps[msg->dest].rolename<<" to "<<msg->map<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(TeamGotoMap_Msg));
     }
 }

 void CMServer::DoDissolveTeamMsg(const int& fd,TeamManage_Msg* msg)
 {
     msg->fd=fd;
     if(m_playermaps.find(msg->dest)!=m_playermaps.end())
     {
         cout<<"Dissolve Team "<<m_playermaps[fd].rolename<<" and "<<m_playermaps[msg->dest].rolename<<endl;
         SendMsg(msg->dest,(char*)msg,sizeof(TeamManage_Msg));
     }
 }
