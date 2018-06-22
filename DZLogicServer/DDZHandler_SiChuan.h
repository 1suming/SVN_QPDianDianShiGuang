#ifndef    _DDZ_HANDLER_SICHUAN_H_
#define    _DDZ_HANDLER_SICHUAN_H_

#include "GameHandler.h"
#include "Desk.h"
#include "gr_card_manager.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"

#define USER_COUNT_SICHUAN  (3)

class GameHandler_DDZ_SiChuan : public GameHandler
{
public:
	bool startup(Desk *desk);
	void SetDeskDeal();
	void CheckSameIP();
	void QiangDiZhu(Lint nPos);
	void HanderUserQiangDiZhuOper(User* pUser,LMsgC2SQdzOper_Sichuan* msg);
	void dealCard_new();
	void HanderUserPlayCard(User* pUser,LMsgC2SUserPlay* msg);
	void SetDeskPlay();
	void ProcessRobot(Lint pos, User * pUser);
	bool OnUserReconnect(User* pUser);

	//////////////////////////////////////////////////////////////////////////
protected:
	void clearHandler(void);
	void QueryUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType);
	void BoadCastUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType, Lint nOperRel);
	void UpdateUserBeishu();
	void ShowCard2User(Lint nCurPos);
	void _clearGame();

	Lint getDeskState();
	void NotPlayCard(Lint pos);
	void PlayCard(Lint pos, CardVec& card, Lchar card_mode, Lchar card_type);
	void CheckPlayState(Lint pos);
	bool CheckPlayEnd(Lint pos);
	void SetDeskEnd(Lchar pos);
	//void SetVip(VipLogItem* vip){ m_vip = vip; }
	void SendPlayInfo(bool islast = false);
	void SendPlayCard(Lchar pos, Lchar ret, Lchar action, CardVec& vec, bool isFirstOut = false);

///////////////////////////////////////////// ����desk����; //////////////////////////////////////////////////
protected:
	void BoadCast(LMsg& msg){ m_pDesk->BoadCast(msg); }
	void BoadCastWithOutUser(LMsg& msg, User* user){ m_pDesk->BoadCastWithOutUser(msg, user); }
	Lint GetUserPos(User* user){ return m_pDesk->GetUserPos(user); }
	Lint GetNextPos(Lint m_curPos){ return m_pDesk->GetNextPos(m_curPos); }
	Lint GetPrevPos(Lint m_curPos){ return m_pDesk->GetPrevPos(m_curPos); }
	void SetDeskWait(){ m_pDesk->SetDeskWait(); }
	void ClearUser(){ m_pDesk->ClearUser(); };
	void SetDeskFree(){m_pDesk->SetDeskFree();}
	void ResetClear(){m_pDesk->ResetClear();}


private:
	Desk* m_pDesk;

private:
// 	User*			m_user[USER_COUNT_SICHUAN];
// 	Lint            m_nUserCounts;
	Lint            m_nCardCounts;

//	PK_GAME_STYLE   m_gameStyle;      // ��Ϸ����;
	Lint            m_nFirstCardIndex;  // ȷ��ׯ��λ�õ���������;û������ʱΪ����3��������ʱ������������������к�������С��һ��;
	Lint            m_nDiZhuPos;       //����λ��;
//	Lint            m_nMaxFanshu;      //��Ϸ��󷭱���;
	Lint            m_nDifen;          //��Ϸ�׷�;
	Lint            m_nDizhuPlayCounts;//�������ƴ��������ڼ���ũ����;
	Lint            m_nQiangDizhuState;//�������A�Σ�0�Ǔ������A�Σ�1��ץ�A�Σ�2ץ��ץ�A�Σ�3���A�Σ�4���A��;

	Lint            m_menzhua[USER_COUNT_SICHUAN];
	Lint            m_daobudao[USER_COUNT_SICHUAN];
	Lint            m_labula[USER_COUNT_SICHUAN];
	Lint            m_nUserBeishu[USER_COUNT_SICHUAN];

	CardVec         m_putCard;//��ȥ����;
	CardVec         m_handCard[USER_COUNT_SICHUAN];//���ϵ��� ;
	CardVec         m_leftCard;//����������;

	CardVec			m_outCard[USER_COUNT_SICHUAN];

	Lchar           m_playerCardType[USER_COUNT_SICHUAN];//��������;
	Lchar           m_curCardType;
	Lchar           m_curCardMode;
	Lchar           m_check;
	Lchar			m_curPlayCount;
	Lchar           m_win;
//	Lchar           m_readyState[USER_COUNT_SICHUAN];//���׼��״̬;

	Lchar           m_bombCount;//ը������;
	Lchar			m_firstPos;// ��������λ��,��һ��Ϊ���������Ϊ�Ͼ�Ӯ��;

	Lint			m_bomb[USER_COUNT_SICHUAN];

	CardDeal		m_dealCard;

//	Lint			m_deskState;//����״̬;

	Lint			m_curPos;//��ǰ�������;

	Llong			m_ticktime;

	//VipLogItem*		m_vip;

	//Lint			m_reset[USER_COUNT_SICHUAN];
	//Lstring			m_resetUser;//��������;
	//Lint			m_resetTime;//��������ʱ��;

// 	Lint            m_tiaopaishu;       //������ҵ���������0��������;
// 	CardValue       m_specialCard[56];  //�洢����ض������ƾ�;
// 
 	//VideoLog		m_video;//¼��;

	CardHelp		m_help;
	std::vector<std::vector<Lint>>			m_curHelpInfo;
	std::vector<Lint>                       m_curHelpUnusable;
	Lint			m_baojing[USER_COUNT_SICHUAN];
};

DECLARE_GAME_HANDLER_CREATOR(game_ddz_sichuan, GameHandler_DDZ_SiChuan)

#endif