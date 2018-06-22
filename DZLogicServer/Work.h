#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LNet.h"
#include "LMsgS2S.h"
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
	
	//md5�����㷨;
	Lstring			Md5Hex(const void* data,Lsize size);
public:
	//����http��Ϣ; 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ;
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ; 
	void			HanderUserKick(LMsgKick* msg);

	//������ҵ�½����Ϣ; 
	void			HanderUserLogin(LMsgLMG2LLogin* msg);
	void            HanderUserLoginGold(LMsgLMG2LLogin* msg);

	//������Ҷ�����������Ϣ;
	void			HanderUserReLogin(Lint id,Lint seed,Lint gateId,Lint gateUserId,Lstring& md5,Lstring& ip);
	
	//������ҵ�½����Ϣ;
	void			HanderDeskUserLogin(User* pUser, Lint gateId, Lint gateUserId, Lstring& ip, bool firstLogin = false, Lstring buyInfo = "", Lint hide = 0, Lint gameZone = game_zone_roomcard);

	//������ҵǳ�����Ϣ; 
	void			HanderUserLogout(Lint gateId, Lint geteUserId);

	//������Ҵ�������;
	void			handerLMGCreateDesk(LMsgLMG2LCreateDesk* msg);

	//��Ҵ�����ҳ�����;
	void            handerLMGIntoGoldDesk(LMsgLMG2LIntoGoldDesk* msg);

	//������Ҽ�������;
	void			handerLMGAddToDesk(LMsgLMG2LAddToDesk* msg);

	//����Gate����;
	void			HanderGateHeartBeat(LMsgG2LHEARTBEAT* msg);
	
	//��������;
	void			HanderLogicManagerHeartBeat(LMsgLMG2LHEARTBEAT_BACK* msg);

	//����LDB����
	void			HanderLogicDBHeartBeat(LMsgLDB2LHEARTBEAT_BACK* msg);
	//��������ID������ʧ�ܣ����������;
	void			RecycleDeskId(Lint deskID);

	//���ս�ҳ����ӵ�һ��λ�ã�����ʧ�ܣ����������;
	void			RecycleGoldDeskPlace(Lint deskID, Lint gameType);
public:
	void			ConnectToLogicManager();

	void			SendToLogicManager(LMsg& msg);

	void			SendHeartBeatToLogicManager();

	void			SendHeartBeatToLogicDB();
	//center�������ر�;
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
	TCPClientPtr	m_logicManager;//����center
	TCPClientPtr	m_dbClient;

	std::map<Lint, GateInfo> m_gateInfo;

	bool				m_loginLMFirst;
	
	//////////////////////////////////////////////////////////////////////////
	bool		m_isConnect;
	GarbageCollectionService	m_gcService;
	TCPClientPtr	m_teamServer;	// ���� team Server [2017-7-10 willing]

public:
	void HanderUserModifyRoomCard(LMsgLMG2LModifyRoomCard* msg);
};

#define gWork Work::Instance()

#endif