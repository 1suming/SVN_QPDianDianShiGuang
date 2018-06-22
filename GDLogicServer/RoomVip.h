#ifndef _ROOM_VIP_H
#define _ROOM_VIP_H

#include "LMsg.h"
#include "Desk.h"
#include "LVipLog.h"

//�ƾ�����
enum EPaiJuType
{
	PJT_HAO_YOU_ZHUO = 1, //������
	PJT_JIN_BI_CHANG = 2, //��ҳ�
	PJT_BI_SAI_CHANG = 3, //������
};

//������λ
enum EBaShuUnit
{
	BSU_JU = 1,		//��
	BSU_QUAN = 2,	//Ȧ
	BSU_GUO = 3,	//��
};

struct VipLogItem :public LVipLogItem
{
public:
	Desk*			m_desk;
public:
	VipLogItem();
	~VipLogItem();
	Lint		GetOwerId();
	
	void		AddTuiDaoLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo,Lint zhuangPos,Lint* agang,Lint* mgang, Lint *bomb,Lstring& videoId, Lint* maNum, Lint *ghostZimo, Lint* noghostZimo);
	void		AddJiPingLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo,Lint zhuangPos,Lint* agang,Lint* mgang, Lint *bomb,Lint *fanshu, Lint* baohu, Lstring& videoId);
	void		AddZhuanZhuanLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo,Lint zhuangPos,Lint* agang,Lint* mgang, Lint *bomb,Lstring& videoId, Lint* maNum);
	void		AddJokerChickenLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo,Lint zhuangPos,Lint* agang,Lint* mgang, Lint *bomb,Lstring& videoId, Lint* maNum, Lint *normal, Lint* singleflower);
	void		AddChaoZhouLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo,Lint zhuangPos,Lint* agang,Lint* mgang, Lint *bomb,Lstring& videoId, Lint* maNum);
	void		AddHongkongLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo, Lint zhuangPos, Lint* agang, Lint* mgang, Lint *bomb, Lstring& videoId, Lint* maNum);
	void		AddHongZhongLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo, Lint zhuangPos, Lint* agang, Lint* mgang, Lint *bomb, Lint *fanshu, Lint *zimo, Lint maNum[], Lstring& videId);
	void		AddJieyangLog(User** user, Ldouble* gold, Ldouble* cal_gold, Lint* winInfo, Lint zhuangPos, Lint* agang, Lint* mgang, Lint *bomb, Lstring& videoId, Lint* maNum);
	bool		ExiestUser(Lint id);

	void		InsertToDb();

	void		UpdateToDb();

	bool		AddUser(User* user);

	Lint		GetUserPos(User* user);

	Ldouble		GetUserScore(User* user);
	void		SetUserScore(User* user, Ldouble nScore);

	bool		isEnd();
	void		onCircleEnd(User** user);
	void		ReportHorseInfo2LogicManager(User** user, Lint* maNum);

	bool		IsFull(User* user);
	//������
	void		Report2MatchServer(const LMsgS2CVipEnd & endmsg);
	void		Report2MatchServer(const LMsgS2CGameOver & endmsg);

	void		SendEnd();

	void		SendInfo();

	bool		IsBegin();

	void		RemoveUser(Lint id);

	int         GetCurrDifen()const { return m_nCurDifen; }
	void        SetCurrDifen();
	Lint		GetAppId()const {return m_nAppId;}
	void		SetAppId(Lint appId) { m_nAppId = appId; }

	/*Զ����־���begin*/

	//�ƾ����� 1:������ 2:��ҳ� 3:������
	EPaiJuType GetPaiJuType();
	//������λ 1:�� 2:Ȧ 3:��
	EBaShuUnit GetBaShuUnit();
	//Ӧ�۷�����
	Lint GetCardCountToDel();
	//ʵ�۷�����
	Lint GetCardCountRealDel();
	//����ʵ�ʿۿ���
	void AddCardCountRealDel(Ldouble nCardDel);
	/*Զ����־���end*/

private:
	Lint	m_nCardDel = 0; //ʵ�ʿۿ���
	Lint	m_nAppId = 0;

};

class VipLogMgr :public LSingleton<VipLogMgr>
{
public:	
	virtual	bool	Init();
	virtual	bool	Final();

	void			SetVipId(Lint id);

	Lstring			GetVipId();

	void			SetVideoId(Lint id);

	Lstring			GetVideoId();

	VipLogItem*		GetNewLogItem(Lint maxCircle,Lint usrId);

	VipLogItem*		GetLogItem(Lstring& logId);

	VipLogItem*	FindUserPlayingItem(Lint id);//��ѯ������ڽ����е�����

	void		AddPlayingItem(VipLogItem* item);//���δ��ɵ�����

	void		Tick();
private:
	std::map<Lstring,VipLogItem*> m_item;
	Lint	m_id;

	Lint	m_videoId;
};

#define gVipLogMgr VipLogMgr::Instance()


//vip����
class RoomVip:public LSingleton<RoomVip>
{
public:
	virtual	bool	Init();
	virtual	bool	Final();

	void			Tick(LTime& cur);

	Desk*			GetFreeDesk(Lint nDeskID, Lint nAppId);

	Desk*			GetDeskById(Lint id);

	bool			CreateVipDesk(LMsgLMG2LCreateDesk* pMsg,User* pUser);
	//������
	VipLogItem*		CreateMatchDesk(LMsg_Match2L_CreateMatchDesk* pMsg);
	
	bool			AddToVipDesk(User* pUser, Lint nDeskID);

	bool			LeaveToVipDesk(LMsgC2SLeaveDesk* pMsg, User* pUser);
	
private:
	DeskMap		m_deskMapUseing;
	DeskVec		m_deskMapFree;

	Lint		m_curDeskId;

	std::queue<Lint>  m_deskId;
};

#define gRoomVip RoomVip::Instance()



#endif