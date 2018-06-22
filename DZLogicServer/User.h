#ifndef _USER_H_
#define _USER_H_

#include "LUser.h"
#include "LMsgS2S.h"
#include "LActive.h"

class Desk;

class  User
{
public:
	User(LUser data, Lint gateId,Lint gateUserId);
	virtual ~User();

	virtual void	Tick();

	//��ȡ������ݿ���id;
	Lint	GetUserDataId();

	//��ȡ���Ψһ��ʾ;
	Llong	GetUserUnique();

	void	SetUserGateId(Lint gateId);
	Lint	GetUserGateId();

	void	SetUserGateUserId(Lint gateUserId);
	Lint	GetUserGateUserId();

	bool	GetOnline();
	void	SendLoginInfo(Lstring& buyInfo, Lint hide);
	void	SendItemInfo();
    void    send2LMGGoldChange(Lint changeNum, Lint changeReason, Lint goldNum);
	bool	Login(Lint gameZone = game_zone_roomcard);
	void	Logout();

	void	Send(LMsg& msg);

	Desk*	GetDesk();
	void	SetDesk(Desk* desk);
	Desk*   GetGoldDesk();
	void    SetGoldDesk(Desk* desk);

	void	HanderMsg(LMsg* msg);
	//��Ҵ�������;
	Lint	HanderUserCreateDesk(LMsgLMG2LCreateDesk* msg);

	//��ҽ��뷿��;
	Lint	HanderUserAddDesk(Lint nDeskID);

	Lint    HanderUserIntoGoldDesk(LMsgLMG2LIntoGoldDesk* msg);

	//����˳�����;
	void	HanderUserLeaveDesk(LMsgC2SLeaveDesk* msg);

	//�����������������;
	void HanderUserQiangDiZhu(LMsgC2SQiangDiZhu* msg);
	
	//��������Ĵ��������淨��������ز�������;
	void HanderUserQiangDiZhuOper(LMsgC2SQdzOper_Sichuan* msg);

	//����������;
	void	HanderUserPlayCard(LMsgC2SUserPlay* msg);

	//�������֪ͨ����;
	void	HanderUserReady(LMsgC2SUserReady* msg);
	
	//������㷿��;
	void	HanderResutDesk(LMsgC2SResetDesk* msg);

	//�������ز���;
	void    HanderAskGoldDeskState(LMsgC2SAskGoldDeskState* msg);

	//���ѡ��������;
	void	HanderSelectResutDesk(LMsgC2SSelectResetDesk* msg);

	//�����������;
	void	HanderUserSpeak(LMsgC2SUserSpeak* msg);

	//������Ϣ;
	void	HanderUserHeart(LMsgC2SHeart* msg);

	Lstring GetIp();

	void	SetIp(Lstring& ip);

	void	Save();
	
	//������ز���;
	//��ȡ��������;
	bool	IfCardEnough(Lint cardType, Ldouble num);
	//���ӷ���;
	void	AddCardCount(Lint cardType, Ldouble count,Lint operType,const Lstring& admin, bool bNeedSave = true);
	//ɾ������;
	void	DelCardCount(Lint cardType, Ldouble count, Lint operType, Lint nGameType, const Lstring& admin);
	


	//�ı����״̬;
	void	ModifyUserState(bool bLeaveServer = false);
	Lint	getUserState(){return m_userState;}
	void	setUserState(Lint nValue, Lint maYouQuan = 0);
	Lint	getUserGateID(){return m_gateId;}
	void	setUserGateID(Lint nValue){m_gateId = nValue;}
	Lint	getUserGateIdx(){return m_gateUserId;}
	void	setUserGateIdx(Lint nValue){m_gateUserId = nValue;}
	void    setTuoguan();
	void    unSetTuoguan();

	void    setLeaveGoldDesk(){m_bLeaveGoldDesk = true;}
	void    unsetLeaveGoldDesk(){m_bLeaveGoldDesk = false;}
	bool    getLeaveGoldDesk(){return m_bLeaveGoldDesk;}

	//
	virtual bool	IsRobot();
public:
	LUser	m_userData;
	Lint	m_gateId;		//������ĸ�gate����;
	Lint	m_gateUserId;	//�����gate�����id;
	Lint	m_userState;	//��ҵ�ǰ״̬;
	bool	m_online;
	std::vector< LActive >	m_Active;
	Desk*	m_desk;
	Desk*   m_goldDesk;
	bool    m_bTuoGuan;  //�Ƿ����������й�;
	bool    m_bLeaveGoldDesk;  //����Ƿ��뿪��ҳ�����;

	Lint    m_nWaitTime;  //�ȴ���Ҳ���ʱ��,��ʱ���Զ��й�;

	Lstring	m_ip;

//��ҷ���״̬;
public:
	enum UserRoomState
	{
		EInvalid,
		EReady,
		EHuaSe,
		EAcceptTurnOver,
		EPlayStart,
		EMax,
	};

	UserRoomState GetRoomState(){ return m_userRoomState;}

	bool GetDone(UserRoomState state)
	{ 
		if(m_userRoomState != state) 	
		{
			return false;
		} 
		return m_isDone[state];
	}

	void SetDone(UserRoomState state, bool isDone) { m_isDone[state] = isDone;};
	void SetRoomState(UserRoomState state) { m_userRoomState = state; }
private:
	UserRoomState m_userRoomState;
	bool m_isDone[EMax];

//ʮһ�
public:
	//�������
	void SavePlayCount(const Lstring& admin);
	void addBashu(Lint baishu);
private:
	Lint m_bashu;
	
};

#endif