#ifndef __DESK_MANAGER_H___
#define __DESK_MANAGER_H___
// ��������״̬���� [2017-8-2 willing]
#include "LSingleton.h"
#include "GameDefine.h"
#include <vector>
#include <list>
#include <map>

struct STeamDeskInfo
{
	Lint nTeamID = 0;
	Lint nDeskID = 0;
	Lint nUserID[DEFAULT_DESK_USER_COUNT] = {0};
	Lstring m_nike[DEFAULT_DESK_USER_COUNT] = { "" };
	Lstring strHeadImageUrl[DEFAULT_DESK_USER_COUNT] = {""};
	Lint nMajongType = 0;							// �淨���� [2017-8-2 willing]
	Lint nMaxCircle = 0;							// ������ [2017-8-3 willing]
	Lint nMaxUserCount = 0;							// ���ӵ����������� [2017-8-4 willing]
	Lint nCurUserCount = 0;							// ��ǰ������������
	//Lint nUpDownJi = 0;								// ���¼�
	//Lint nAllJi = 0;								// ���ü�
	//Lint nLianZhuang = 0;							// ��ׯ�Ƿ���� [2016-9-12 willing]
	Lint nPlayTypeCount = 0 ;						// С�淨���� [2017-8-3 willing]
	std::vector<Lint>	tPlayType;					// С�淨ѡ�� [2017-8-2 willing]
	Llong nCreateTime = 0;								// ���Ӵ�����ʱ��� [2017-8-5 willing]
	Lint nCircleGoldLimit = 0;                       //ÿһȦ���������
	Lint nStake = 0;                                 //Ѻע����
	Lint nMaxFanshu = 0;                             //�ͻ��� ը��
};
struct STeamFastDesk
{
	Lint	m_deskid;
	Lint	m_userCount;
	STeamFastDesk() :m_deskid(0), m_userCount(0)
	{

	}
	bool operator == (const STeamFastDesk& desk) const
	{
		return m_deskid == desk.m_deskid;
	}
	bool operator == (const Lint& deskId) const
	{
		return m_deskid == deskId;
	}

};
typedef std::list<STeamDeskInfo>			tTeamDeskInfoList;
typedef std::map<Lint, tTeamDeskInfoList>	tTeamDeskInfoListMap;
typedef std::map<Lint, Lint>				tDeskIDTeamIDMap;

class CDeskManager : public LSingleton<CDeskManager>
{
public:
	CDeskManager();
	virtual ~CDeskManager();

	virtual	bool Init();
	virtual	bool Final();

	void	AddDeskInfo(const STeamDeskInfo& teamDeskInfo,bool is_fastDesk = false);
	void	DeskUserChg(Lint nTeamID,Lint nDesk,Lint nUserID, Lint nIsJoin);
	void	DelDesk(Lint nTeamID, Lint nDesk);
	bool    CheckDeskInfoLimit(Lint nTeamID, Lint nDesk, Ldouble card_num, Lint& limit_num);
	void	SendAllDesk(Lint nTeamID, Llong nUserUnique);
	bool	IsInDesk(Lint nUserID);
	bool	IsInDesk(Lint nUserID, Lint& nTeamId);
	bool    GetDeskInfoByUserId(Lint nUserID, Lint& nTeamId, Lint& nDesk);
	Lint	GetTeamIDByDeskID(Lint nDeskId);
	Lint    GetDeskCount(Lint nTeamID);

	// ������8Сʱ������ [2017-8-9 willing]
	void	ClearTimeOutDesk();

	void	PushFastDesk(std::vector<Lint>& addVec);
	Lint	PopFastDesk();
	Lint	GetFreeDeskId(bool& isCreate,Lint teamId, std::vector<Lint>& deskIds, std::vector<Lint>& limitGolds);
	Lint	CheckDeskFull(Lint nTeamID, Lint deskId);

public:
	tDeskIDTeamIDMap		m_tDeskIDTeamIDMap;


private:
	tTeamDeskInfoListMap	m_tTeamDeskInfoListMap;
	
	std::vector<int>		m_tFastFreeDesk;
	std::map<Lint,std::vector<STeamFastDesk>>	m_tFastUseDeskMap;   //key: teamId
	std::multimap<Lint, STeamDeskInfo>	m_tFastDeskInfoMap;
};

#define gDeskManager CDeskManager::Instance()

#endif
