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

    /*
     * deal with player move to msg
     * @param fd:sender fd
     * @param msg:sender's msg
     */
    void DoPlayerMoveToMsg(const int& fd,MoveTo_Msg* msg);

    /*
     * deal with player position verify msg
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoPlayerVerifyPosMsg(const int& fd,VerifyPos_Msg* msg);

    /*
     * deal with update player data msg
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoUpdateDataMsg(const int& fd,UpdateData_Msg* msg);

    /*
     * deal with team apply message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void doTeamApplyMsg(const int& fd,TeamApply_Msg* msg);

    /*
     * deal with refuse team message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoRefuseTeamMsg(const int& fd,RefuseTeam_Msg* msg);

    /*
     * deal with agree team message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoAgreeTeamMsg(const int& fd,AgreeTeam_Msg* msg);

    /*
     * deal with team move message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoTeamMoveMsg(const int& fd,TeamMove_Msg* msg);

    /*
     * deal with team go to map message
     * @param fd:message sender's fd
     * @param msg:messafe context
     */
    void DoTeamGotoMapMsg(const int& fd,TeamGotoMap_Msg* msg);
    
    /*
     * deal with dissolve team message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoDissolveTeamMsg(const int& fd,TeamManage_Msg* msg);

    /*
     * deal with team entry fight message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoTeamFightMsg(const int& fd,TeamFight_Msg* msg);

    /*
     * deal with player attack monster message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoPlayerAtkMsg(const int& fd,PlayerAtk_Msg* msg);

    /*
     * deal with Monster attack player message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoMonsterAtkMsg(const int& fd,MonsterAtk_Msg* msg);

    /*
     * deal with player run away message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoPlayerRunMsg(const int& fd,PlayerRun_Msg* msg);

    /*
     * deal with use medication message
     * @param fd:message sender's
     * @param msg:message context
     */
    void DoUseMedicationMsg(const int& fd,UseMedication_Msg* msg);

    /*
     * deal with player die message
     * @param fd:message sender's fd
     * @param msg:message context
     */
    void DoPlayerDieMsg(const int& fd,PlayerDie_Msg* msg);
private:
    std::map<int,Player_Info> m_playermaps;
};

#endif // CMSERVER_H
