#ifndef    _DDZ_HANDLER_SICHAUAN3_PAIZI_H_
#define    _DDZ_HANDLER_SICHAUAN3_PAIZI_H_
#pragma once

#include "GameHandler.h"
#include "Desk.h"
#include "gr_card_manager_sichuan3_pizi.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"

#define USER_COUNT_PiZi3			(3)


//�Ĵ�Ʀ�ӼƷ��߼�(ֻ��ũ����м��㣩
class Score3	
{
public:
	enum TurnOverType
	{
		ETInvalid,
		ETAddPoint, //�ӵ�(�ӵף�
		ETTurn,	//����
	};

	enum OperatorType
	{
		ENone,
		EAdd,
		EMult,
	};

	Score3()
	{
		Reset();
	}
	~Score3()
	{

	}

	void Reset()
	{
		m_type = ETInvalid;
		m_turnoverCountLimit = 0;
		m_currentTurnoverCount = 0;
		m_baseScore = 0;
		memset(m_currentScore, 0, sizeof(m_currentScore));
		m_dizhuPos = INVAILD_VAL;
		m_daos.clear();
		m_nongmins.clear();
	}
	void Init(BYTE type, BYTE turnOverCountLimit, Lint baseScore) 
	{
#if USEASSERT
		assert(type > 0);
#endif
		m_type = type;
		m_turnoverCountLimit = turnOverCountLimit;
		m_baseScore = baseScore;
		for(int i = 0; i < USER_COUNT_PiZi3; ++i)
		{
			m_currentScore[i] = 1;
		}
	}
	//��
	void AddDao(BYTE turnCount, Lint pos)	//������
	{
#if USEASSERT
		assert(turnCount == 1);
		assert(turnCount > 0);
#endif
		if(pos == m_dizhuPos)
		{
			return;
		}

		if(pos == INVAILD_VAL)
		{
			return;
		}

		Lint score = 0;
		OperatorType oType = ENone;

		if(m_type == 1)		//�ӵ�
		{
			score = turnCount; 
			oType = EAdd;
		} 
		else if(m_type == 2)	//����
		{
			if(m_currentTurnoverCount < m_turnoverCountLimit)
			{
				score = (Lint) pow((double)2, turnCount);
				oType = EMult;
			}
			else
			{
				score = turnCount;
				oType = EAdd;
			}
		}
		OperScore(oType, score, pos);
		m_daos.push_back(pos);
		m_currentTurnoverCount += turnCount;
		
	}

	//��
	void AddLa(BYTE turnCount, Lint pos)
	{
#if USEASSERT
		assert(turnCount == 1);
		assert(turnCount > 0);
#endif

		if(pos == INVAILD_VAL)
		{
			return;
		}

		Lint score = 0;
		OperatorType oType = ENone;

		if(m_type == 1)		//�ӵ�
		{
			score = turnCount;
			oType = EAdd;
			
		} 
		else if(m_type == 2)	//����
		{
			if(m_currentTurnoverCount < m_turnoverCountLimit)
			{
				score = (Lint) pow((double)2, turnCount);
				oType = EMult;
			}
			else
			{
				score = turnCount;
				oType = EAdd;
			}

		}

		if(pos == m_dizhuPos)
		{
			if(m_daos.empty())
			{
				assert(false);
				return;
			}
			for(size_t i = 0; i< m_daos.size(); i++)
			{
				OperScore(oType, score, m_daos[i]);
			}
		}
		else
		{
			OperScore(oType, score, pos);
		}

		m_currentTurnoverCount += turnCount;
	}

	//��ץ
	void AddMen(BYTE count, Lint pos)
	{
		Lint score = (Lint) pow((double)2, count);
		for(int i = 0; i< USER_COUNT_PiZi3; ++i)
		{
			MultScore(score, i);
		}
	}

	void SetDizhuPos(Lint pos)
	{
#if USEASSERT
		assert(m_dizhuPos == INVAILD_VAL);
#endif
		if(m_dizhuPos != INVAILD_VAL) return;

		m_dizhuPos = pos;
		
		//��ʼ��
		for(int i = 0; i < USER_COUNT_PiZi3; i++)
		{
			if(i != m_dizhuPos)
			{
				m_currentScore[i] = m_baseScore;
				m_nongmins.push_back(i);
			}
		}
	}

	void AddBomb(BYTE bombCount)	//ը������
	{
#if USEASSERT
		assert(bombCount > 0);
#endif
		Lint score = 0;
		OperatorType oType = ENone;

		if(m_type == 1 || m_type == 2)
		{
			if(m_currentTurnoverCount < m_turnoverCountLimit)
			{
				if(m_currentTurnoverCount + bombCount > m_turnoverCountLimit)
				{
					for(int i = 0; i < USER_COUNT_PiZi3; ++i)
					{
						m_currentScore[i] = (Lint) (m_currentScore[i] * pow((double)2, ( m_turnoverCountLimit - m_currentTurnoverCount)) + (m_currentTurnoverCount + bombCount - m_turnoverCountLimit));
					}
					m_currentTurnoverCount += bombCount;
					return;
				}
				else
				{
					score = (Lint) pow((double)2, bombCount);
					oType = EMult;
				}
			}
			else
			{
				score = bombCount;
				oType = EAdd;
			}
		}
		

	
		for(int i = 0; i < USER_COUNT_PiZi3; ++i)
		{
			OperScore(oType, score, i);
		}

		m_currentTurnoverCount += bombCount;
	}

	void AddScore(Lint score, Lint pos)
	{
		m_currentScore[pos] += score;
	}

	void MultScore(Lint score, Lint pos)
	{
		m_currentScore[pos] *= score;
	}

	void OperScore(OperatorType type, Lint score, Lint pos)
	{
		if(type == EAdd)
		{
			AddScore(score, pos);
		}
		else if(type == EMult)
		{
			MultScore(score, pos);
		}
	}

	
	Ldouble GetScore(Lint pos)
	{
		if(pos == m_dizhuPos)
		{
			Ldouble score = 0;
			for(auto itor = m_nongmins.begin(); itor != m_nongmins.end(); ++itor)
			{
				score += m_currentScore[*itor];
			}
			return score;
		}
		return m_currentScore[pos];
	}

	Lint GetTurnCount() 
	{
		return m_currentTurnoverCount;
	}

private:
	BYTE	m_type;		//0��Ч�� �ӵ�1������2
	BYTE	m_turnoverCountLimit;	//��������
	BYTE	m_currentTurnoverCount;	//��ǰ������

	Lint	m_baseScore;				//�׷�
	Ldouble	m_currentScore[USER_COUNT_PiZi3];			//��ǰ����
	Lint	m_dizhuPos;	//����pos
	std::vector<Lint>	m_daos;
	std::vector<Lint>   m_nongmins;
};

class GameHandler_DDZ_SiChuan3_PiZi : public GameHandler
{
public:
	bool startup(Desk *desk);
	void SetDeskDeal();
	void CheckSameIP();
	void QiangDiZhu(Lint nPos);
	void SetDeskPlay();
	bool OnUserReconnect(User* pUser);
	void dealCard_new();
	void ProcessRobot(Lint pos, User * pUser);

	//handler
	void HanderUserQiangDiZhuOper(User* pUser,LMsgC2SQdzOper_Sichuan* msg);
	void HanderUserPlayCard(User* pUser,LMsgC2SUserPlay* msg);



	//////////////////////////////////////////////////////////////////////////
protected:
	void clearHandler(void);
	void _clearGame();
	Lint getDeskState();

	void NotPlayCard(Lint pos);
	void PlayCard(Lint pos, CardVec& card, Lchar card_mode, Lchar card_type);
	void CheckPlayState(Lint pos);
	bool CheckPlayEnd(Lint pos);
	void SetDeskEnd(Lchar pos);
	void SendPlayInfo(bool islast = false);
	void SendPlayCard(Lchar pos, Lchar ret, Lchar action, CardVec& vec, bool isFirstOut = false);

	//
	bool IsPlayByFilter( Lint type, bool last);	//�Ƿ��ܳ���
	bool IsLeftLaizi(const CardVec & tmp, Lint pos);

	///////////////////////////////////////////// ����desk����; //////////////////////////////////////////////////
protected:
	//�㲥
	void BoadCast(LMsg& msg){ m_pDesk->BoadCast(msg); }
	void BoadCastWithOutUser(LMsg& msg, User* user){ m_pDesk->BoadCastWithOutUser(msg, user); }
	
	Lint GetUserPos(User* user){ return m_pDesk->GetUserPos(user); }
	Lint GetNextPos(Lint m_curPos){ return m_pDesk->GetNextPos(m_curPos); }
	Lint GetPrevPos(Lint m_curPos){ return m_pDesk->GetPrevPos(m_curPos); }

	void SetDeskWait(){ m_pDesk->SetDeskWait(); }
	void ClearUser(){ m_pDesk->ClearUser(); };
	void SetDeskFree(){m_pDesk->SetDeskFree();}
	void ResetClear(){m_pDesk->ResetClear();}


	//
	void InitScore();
	bool IsNongMinSpring(Lint winPos);
	bool IsLandlordSpring(Lint winPos);
	void DoBomb(Lint pos);
	void GetScore(Lint winPos, Ldouble * gold);

	//�費��Ҫ�Ĺ㲥???
	void BoadCastUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType, Lint nOperRel);
	void QueryUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType);
	void ShowCard2User(Lint nCurPos);
	//
private:
	Desk* m_pDesk;

private:
	//User*			m_user[USER_COUNT_PiZi];
	//Lint            m_nUserCounts;
	Lint            m_nCardCounts;

	//	PK_GAME_STYLE   m_gameStyle;      // ��Ϸ����;
	//Lint            m_nFirstCardIndex;  // ȷ��ׯ��λ�õ���������;û������ʱΪ����3��������ʱ������������������к�������С��һ��;
	Lint            m_nDiZhuPos;       //����λ��;
	//	Lint            m_nMaxFanshu;      //��Ϸ��󷭱���;
	Lint            m_nDifen;          //��Ϸ�׷�;
	Lint            m_nDizhuPlayCounts;//�������ƴ��������ڼ���ũ����;
	Lint            m_nQiangDizhuState;//�������A�Σ�0�Ǔ������A�Σ�1��ץ�A�Σ�2ץ��ץ�A�Σ�3���A�Σ�4���A��;

	Lint            m_menzhua[USER_COUNT_PiZi3];
	Lint            m_daobudao[USER_COUNT_PiZi3];
	Lint            m_labula[USER_COUNT_PiZi3];
	//Lint            m_nUserBeishu[USER_COUNT_PiZi3];

	CardVec         m_putCard;//��ȥ����;
	CardVec         m_handCard[USER_COUNT_PiZi3];//���ϵ��� ;
	CardVec         m_leftCard;//����������;

	CardVec			m_outCard[USER_COUNT_PiZi3];

	Lchar           m_playerCardType[USER_COUNT_PiZi3];//��������;
	Lchar           m_curCardType;	//number
	Lchar           m_curCardMode;	//style
	Lchar           m_check;
	Lchar			m_curPlayCount;
	Lchar           m_win;
	Lchar			m_firstPos;// ��������λ��,��һ��Ϊ���������Ϊ�Ͼ�Ӯ��;

	Lint			m_bomb[USER_COUNT_PiZi3];	//��¼ÿһ���˵�ʹ��ը����

	CardDeal_SiChuan3_PiZi		m_dealCard;

	Lint			m_curPos;//��ǰ�������;
	Lint			m_playPos;	//�������

	Llong			m_ticktime;

	CardHelp_SiChuan3_PiZi		m_help;
	std::vector<std::vector<Lint>>			m_curHelpInfo;
	std::vector<Lint>                       m_curHelpUnusable;
	Lint			m_baojing[USER_COUNT_PiZi3];

	//��¼������Ϣ
	enum springType
	{
		EInvalid,
		EDizhuSpring,
		ENongminSpring,
	};
	springType			m_springType;	//0, ��Ч�� 1�������죬2ũ����
	//end

	Score3			m_score;
};

DECLARE_GAME_HANDLER_CREATOR(game_ddz_sichuan3_pz, GameHandler_DDZ_SiChuan3_PiZi)

#endif