#ifndef _USER_H_
#define _USER_H_

#include "LUser.h"
#include "LMsgS2S.h"
#include "LActive.h"

class Desk;

class  User
{
public:
	User(LUser data, Lint gateId, Lint gateUserId);
	virtual ~User();

	virtual void	Tick();

	//��ȡ������ݿ���id
	virtual Lint GetUserDataId(bool bRandId = false);

	//��ȡ���Ψһ��ʾ
	Llong   GetUserUnique();

	void	SetUserGateId(Lint gateId);
	Lint	GetUserGateId();

	void	SetUserGateUserId(const Lint gateUserId);
	Lint    GetUserGateUserId();

	bool	GetOnline();
	void	SendLoginInfo(Lstring& buyInfo, Lint hide);
	void	SendItemInfo();
	void    SendManaged();

	bool	Login();
	void	Logout();

	void	Send(LMsg& msg);

	Desk*	GetDesk();
	void	SetDesk(Desk* desk);

	void	HanderMsg(LMsg* msg);
	//��Ҵ�������
	Lint	HanderUserCreateDesk(LMsgLMG2LCreateDesk* msg);

	//��Ҵ������� �쳣����
	void	HanderUserCreateDeskStateError(LMsgC2SCreateDesk* msg);

	//��ҽ��뷿��
	Lint	HanderUserAddDesk(Lint nDeskID);

	//��ҽ��뷿�� �쳣����
	void	HanderUserAddDeskStateError(LMsgC2SAddDesk* msg);

	//����˳�����
	void	HanderUserLeaveDesk(LMsgC2SLeaveDesk* msg);

	//����������
	void	HanderUserPlayCard(LMsgC2SUserPlay* msg);

	//�������˼��
	void	HanderUserThink(LMsgC2SUserOper* msg);

	//���ѡ�����ֺ�
	void	HanderStartHuSelect(LMsgC2SUserStartHuSelect* msg);

	//���ѡ�񺣵���
	void	HanderEndCardSelect(LMsgC2SUserEndCardSelect* msg);

	//�������֪ͨ����
	void	HanderUserReady(LMsgC2SUserReady* msg);
	
	//������㷿��
	void	HanderResutDesk(LMsgC2SResetDesk* msg);

	//���ѡ��������
	void	HanderSelectResutDesk(LMsgC2SSelectResetDesk* msg);

	void    HanderSelectPiao(LMsgC2SPiaoSelect* msg);
	//�����������
	void	HanderUserSpeak(LMsgC2SUserSpeak* msg);

	//������Ϣ
	void	HanderUserHeart(LMsgC2SHeart* msg);

	void    HanderAttackManaged(LMsgC2SCancelManaged * msg);

	void    CheckSportStatus();

	Lstring GetIp();

	void	SetIp(Lstring& ip);

//	void	Save();
	
	//������ز���
	//��ȡ��������
	bool	IfCardEnough(Lint cardType, Lint num);

	//���ӷ���;
	void	AddCardCount(Lint cardType, Ldouble count, Lint operType, const Lstring& admin, bool bNeedSave = true);

	//ɾ������;
	void	DelCardCount(Lint cardType, Ldouble count, Lint operType, Lint nGameType, const Lstring& admin);

	//�ı����״̬
	void	ModifyUserState(bool bLeaveServer = false);
	Lint	getUserState(){return m_userState;}
	void	setUserState(Lint nValue, Lint maYouQuan = 0);
	Lint	getUserGateID(){return m_gateId;}
	void	setUserGateID(Lint nValue){m_gateId = nValue;}
	Lint	getUserGateIdx(){return m_gateUserId;}
	void	setUserGateIdx(Lint nValue){m_gateUserId = nValue;}
	void    setLongitueLatitude(const Lstring& longitue, const Lstring& latitude) { m_longitue = longitue;  m_latitude = latitude; }
	void    getLongitueLatitude(Lstring& longitue, Lstring& latitude) { longitue = m_longitue;  latitude = m_latitude; }

	inline void  attarchManaged() { m_managed = true; }
	inline void  cancelManaged() { m_managed = false; }
	inline bool  isManaged() { return m_managed; }

	virtual bool IsRobot();

public:
	LUser	m_userData;
	Lint	m_gateId;		//������ĸ�gate����
	Lint	m_gateUserId;	//�����gate�����id
	Lint	m_userState;	//��ҵ�ǰ״̬
	bool	m_online;
	Lstring m_longitue;     //����
	Lstring m_latitude;		//γ��
	std::vector< LActive >	m_Active;
	Desk*	m_desk;

	Lstring	m_ip;
	bool    m_managed; //�Ƿ��й�
public://��������
	Lint    m_rank;
	Lint	m_score;
	Lint	m_overScore;
	Lint    m_sportId;
	Lint    m_process; //�׶�
	void	initPlayGroundData(Lint rank,Lint score,Lint overScore,  Lint sportId, Lint process);
	void    clearPlayGroundData();
};

#endif