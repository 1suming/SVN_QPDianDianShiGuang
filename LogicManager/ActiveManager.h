#ifndef _ACTIVE_MANAGER_H
#define _ACTIVE_MANAGER_H

#include "LSingleton.h"
#include "LMsgS2S.h"
#define MAX_GIFT_NUM	(12)

enum	GIFTTYPE
{
	GT_NONE = 0,
	GT_ITEM = 1,
	GT_PHONECARD = 2,//����
	GT_ROOMCARD = 3, //����
	GT_GOLD = 4,     //���
};

struct ActiveGift
{
	Lstring giftname;
	Lint gifttype;
	Lint giftcount;
	Lint giftpercent;
	Lint gifttotalcount;
	Lint giftsurpluscount;
	ActiveGift()
	{
		gifttype = 0;
		giftcount = 0;
		giftpercent = 0;
		gifttotalcount = 0;
		giftsurpluscount = 0;
	}
};

struct ActiveInfo
{
	Lint	mActiveID;
	Lstring	mActiveName;
	Lstring	mActiveIntroduction;
	Lstring	mBeginTime;					// ʱ���ʽΪ: ������  �� 20160506;
	Lstring	mEndTime;
	Lint	mRewardNum;
	ActiveGift	mGift[MAX_GIFT_NUM];
	Lint	mAllPercent;
	Lint    mTimesPerday;              //������ÿ�����齱����;
	Lint    mAlltimes;                 //�������ܵ����齱����;
};

// �齱����;
struct ActiveRun_RetGift
{
	Lint	giftID;
	Lint	Gift_type;	
	Ldouble	Gift_count;
	ActiveRun_RetGift()
	{
		giftID = 0;
		Gift_type = 0;
		Gift_count = 0;
	}
};

class ActiveManager:public LSingleton<ActiveManager>
{
public:
	virtual	bool		Init();
	virtual	bool		Final();
	void				ReloadConfig();
	Lint				GetLastActiveID();
	bool				IsActiveOpoen(Lint mDate);
	bool				IsNeedPhoneNum(Lint activeID, Lint giftID);
	Lint                getTimesPerday();   //��ȡÿ�����齱����;
	Lint                getAlltimes();      //��ȡ�ܵ����齱����;
	
	
	//ʮһ�
	enum ActiveType
	{
		EAWin = 2,
	};

	enum ActiveCount
	{
		EWinMaxSize = 100,
	};

	void				LoadAllWinData(ActiveType type, std::vector<LActiveSignCollectSS> &actives);
	bool				AddWin(LActiveSignCollectSS& info);
	Lint				GetWinSize(ActiveType type);
	std::vector<LActiveSignCollectSS> GetActive(ActiveType type);
	void				SetLoaded(ActiveType type, bool isLoaded);
	bool				GetLoaded(ActiveType type);
	void				SetWait(bool isWait);
	bool				GetWait();
	

	//end ʮһ�
	
	

	// ��ҳ齱;
	// �������ID���鵽�Ľ�Ʒ��Ϣ���Ƿ�ʣ�������Զ���һ;
	// ���أ��Ƿ�鵽;
	bool				Run(Lint ActiveId, ActiveRun_RetGift& Ret, bool IsAutoDecOne=true);
	bool				CheckTime(Lstring&  mBeginTime, Lstring&  mEndTime);

	void                CheckGiftSafe(Lint activeID, ActiveRun_RetGift& drawInfo);  //���Ƴ齱���ޣ���ֹ����;
private:
	void LoadActiveConfg();	// װ������;
	void LoadActiveData();	// װ������;		δʵ��
	void SaveActiveData();	// ��д�����;		δʵ��
private:
	std::map<Lint, ActiveInfo*>	mActiveList;
	
	//ʮһ�
	std::vector<LActiveSignCollectSS> mAllWinList;	//������
	bool m_isLoaded;
	bool m_isWait;
};

#define gActiveManager ActiveManager::Instance()

#endif
