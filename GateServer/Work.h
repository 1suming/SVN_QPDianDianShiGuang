#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "GateUser.h"
#include "LMsgS2S.h"
#include "GarbageCollectionService.h"

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
	
public:
	//����http��Ϣ 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);
	//����LogicManagerͬ����Logic��Ϣ
	void			HanderLogicInfo(LMsgLMG2GateLogicInfo* msg);
	//�����������Ϣ
	void			HanderHorseInfo(LMsgLMG2GHorseInfo* msg);
	//�޸�User״̬
	void			HanderModifyUserState(LMsgL2GModifyUserState* msg);
	//�յ�����
	void			HanderLogicHeartBeat(LMsgL2GHEARTBEAT_BACK* msg);
	//�յ�����
	void			HanderLogicManagerHeartBeat(LMsgLMG2GHEARTBEAT_BACK* msg);

	//����TeamServer��Ϣ
	void			HanderTeamServerInfo(LMsgLMG2G_TeamServer* msg);
public:
	//����������
	void			SendHeartBeatToLogic();	

	void			SendHeartBeatToLogicManager();

	void			ConnectToLogicManager();
	
	//������Ϣ������User
	void			SendMessageToAllUse(LMsg* msg);

	void			SendToLogicUserOut(GateUser* user);
	//void			HanderLogicUserOut(Lint id);

	void			HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg);

	LogicInfo*		GetLogicInfoBySp(LSocketPtr sp);
	LogicInfo*		GetLogicInfoById(Lint id);

	//socket����;
	void			AddSp(LSocketPtr sp);
	void			DelSp(LSocketPtr sp);
protected:
	//true: ���ҵ�����ɾ���� false��û�в�ѯ��;
	bool			_findLogicSPAndDel(LSocketPtr sp); 
	bool			_isLogicSP(LSocketPtr sp);
	TCPClientPtr	GetLogicClientBySP(LSocketPtr sp);

private:
	void			SendGateLoginToLM();
	void			SendGateLoginToLogic(TCPClientPtr logicSP);

	void			SendMsgToTeamServer(LMsg& msg);
	TCPClientPtr	GetTeamServerTCPClientBySP(LSocketPtr sp);
	bool			IsTeamServerSP(LSocketPtr sp);
	void			SendGateLoginToTeamServer(TCPClientPtr sp);

private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��;

	Llong		m_200MSceTick;//200����ѭ��һ��;

	Llong		m_1SceTick;//1��ѭ��һ��;

	Llong		m_5SceTick;//5��ѭ��һ��;
	
	Llong		m_30SceTick;//30��ѭ��һ��;
	
	Llong		m_600SceTick;//5������ѭ��һ��;

private:
	TCPClientPtr  m_logicManager;
	std::map<Lint, LogicInfo>	m_logicInfo;
	std::map<LSocketPtr, Lint>	m_spMap;
	GarbageCollectionService	m_gcService;
	std::map<Lint, TeamServerInfo>	m_TeamServerInfoMap;

};


#define gWork Work::Instance()

#endif