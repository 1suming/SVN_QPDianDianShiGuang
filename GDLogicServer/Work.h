#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LNet.h"
#include "LMsgS2S.h"
#include "TCPClient.h"
#include "GarbageCollectionService.h"

class User;

struct UserLoginInfo
{
	Lint	m_seed;
	Lint	m_time;
	LUser	m_user;
};

class Work:public LRunnable,public LSingleton<Work>
{
public:
	virtual bool	Init();
	virtual bool	Final();

	virtual void	Start();
	virtual void	Run();
	virtual void	Clear();
	virtual	void	Join();
	virtual	void	Stop();

	void			Tick(LTime& cur);
	void			HanderMsg(LMsg* msg);

	LTime&			GetCurTime();
	//md5�����㷨
	Lstring			Md5Hex(const void* data,Lsize size);
public:
	//����http��Ϣ 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);

	//������ҵ�½����Ϣ 
	void			HanderUserLogin(LMsgLMG2LLogin* msg);
	//������Ҷ�����������Ϣ 
	void			HanderUserReLogin(Lint id,Lint seed,Lint gateId,Llong gateUserId,Lstring& md5,Lstring& ip);
	//������ҵ�½����Ϣ
	void			HanderDeskUserLogin(User* pUser, Lint gateId, Llong gateUserId, Lstring& ip, bool firstLogin = false, Lstring buyInfo = "", Lint hide = 0);

	//������ҵǳ�����Ϣ 
	void			HanderUserLogout(Lint gateId, Llong geteUserId);

	//������Ҵ�������
	void			handerLMGCreateDesk(LMsgLMG2LCreateDesk* msg);

	//������Ҽ�������
	void			handerLMGAddToDesk(LMsgLMG2LAddToDesk* msg);

	//������,
	void			handerMatchServerMsg_CreateMatchDesk(LMsg_Match2L_CreateMatchDesk* msg);
	void			handerMatchServerMsg_ContralMatchDesk(LMsg_Match2L_ContralMatchDesk* msg);

	//��������ID������ʧ�ܣ����������
	void			RecycleDeskId(Lint deskID, Lint userID = 0, Lint nMatchID = 0);
public:
	void			ConnectToLogicManager();

	void			SendToLogicManager(LMsg& msg);

	//center�������ر�
	void			HanderCenterLogout();

	void			HanderDBServerLogout();

public:
	void			HanderGateLogin(LMsgG2LLogin* msg);
	void			HanderGateLogout(LMsgKick* msg);
	GateInfo*		GetGateInfoBySp(LSocketPtr sp);
	GateInfo*		GetGateInfoById(Lint id);
	void			DelGateInfo(Lint id);

	void			HanderGateUserMsg(LMsgG2LUserMsg* msg,GateInfo* gate);

	void			HanderKeepAlive(LMsgS2SKeepAlive* msg);
	void			HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg);

public:
	//DB�������
	void			ConnectToDb();
	void			SendMsgToDb(LMsg& msg);

public://������------------------------------------------------------------------------
	TCPClientPtr	GetMatchServerTCPClientBySP(LSocketPtr sp);
	void			SendLogicLoginToMatchServer(TCPClientPtr sp);
	void			SendMsgToMatchServer(LMsg& msg, int nMatchServerID);
	bool			IsMatchServerSP(LSocketPtr sp);
	void			HanderMatchServerInfoMsg(LMsgLM2Gate2Logic_MatchServerInfo* msg);

	std::map<Lint, MatchServerInfo>			m_MatchServerInfoMap;
	//������------------------------------------------------------------------------

public://���ֲ�------------------------------------------------------------------------
	void			HanderTsAddDeskRet(LMsgTS2L_AddDeskRet* msg);
	void			ConnectToTeamServer(Lstring strServerIp, Lshort nPort);
	void			SendMsgToTeamServer(LMsg& msg);
	void			HanderTeamServerInfo(LMsgLMG2L_TeamServer* msg);
	   //���ֲ�------------------------------------------------------------------------

private:
	void			SendLogicLoginToLM();
	void			SendLogicLoginToDB();

private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_5SceTick;//5��ѭ��һ��

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

private:
	TCPClientPtr	m_logicManager;
	TCPClientPtr	m_dbClient;

	std::map<Lint, GateInfo> m_gateInfo;

	bool				m_loginLMFirst;
	GarbageCollectionService m_gcService;
	TCPClientPtr	m_teamServer;	// ���� team Server [2017-7-10 willing]

public:
	void HanderUserModifyRoomCard(LMsgLMG2LModifyRoomCard* msg);
	
};

#define gWork Work::Instance()

#endif