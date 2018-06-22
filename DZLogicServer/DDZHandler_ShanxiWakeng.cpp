#include "DDZHandler_ShanxiWakeng.h"
#include "Desk.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"
#include <cmath>
extern unsigned int g_nGoldGameNums;

#define HEIWA_DIFEN 4

DDZHandler_ShanxiWakeng::DDZHandler_ShanxiWakeng(): m_firstPos(0)
{
}

bool DDZHandler_ShanxiWakeng::startup(Desk *desk)
{
	if (desk)
	{
		clearHandler();
		m_pDesk = desk;
	}

	return true;
}

void DDZHandler_ShanxiWakeng::clearHandler(void)
{
	_clearGame();
	//m_firstPos = 0;   //��Ҫ������Ϣ����һ�֣����ܳ�ʼ��;
	m_subType = EJiaoFen;
}

//��ʼ��ÿ����Ϸ�����ʼ����������������Ϣ��Ҫ����;
void DDZHandler_ShanxiWakeng::_clearGame()
{
	m_curPos = 0;
	m_ticktime = gWork.GetCurTime().MSecs();
	for (Lint i = 0; i < 3; ++i)
	{
		m_bomb[i] = 0;
		m_handCard[i].clear();
		m_playerCardType[i] = 0;
		m_outCard[i].clear();
		m_baojing[i] = 0;

		m_daobudao[i] = INVAILD_VAL;
		m_labula[i] = INVAILD_VAL;
		m_menzhua[i] = INVAILD_VAL;
		m_jiaoDizhu[i] = EJIao;
		m_heiwa[i] = EJIao;
		m_nUserBeishu[i] = 1;
	}
	m_nQiangDizhuState = INVAILD_VAL;
	m_nDizhuPlayCounts = 0;
	m_nDiZhuPos = INVAILD_VAL;
	m_leftCard.clear();
	m_nDifen = 1; // ��Ϸ�׷�Ĭ��Ϊ1;

	memset(m_baojing,0,sizeof(m_baojing)); //�ܵĿ챨����Ϣ;
}

void DDZHandler_ShanxiWakeng::SetDeskDeal()
{
	//init(û�ҵ����ʵ�λ�ã�;
	if(!m_pDesk) 
	{
		LLOG_ERROR("SetDeskDeal m_pDesk is nullptr");
		return;
	}
#if USEASSERT
	assert(m_pDesk->m_extValue.size() > 0);
#endif
	if(m_pDesk && m_pDesk->m_extValue.size() > 0)
	{
		m_subType = (ClassicalSubType)m_pDesk->m_extValue[0];
	}
	else
	{
		m_subType = EJiaoFen;
	}

	//end init

	if (m_pDesk && m_pDesk->m_vip)
	{
		m_pDesk->m_vip->m_subFlag = m_subType;	//��Ϸ������;
		m_pDesk->m_vip->SendInfo();
	}
	dealCard_new();
	Lint userId[USER_COUNT_CLASSICAL] = {0};
	Ldouble score[USER_COUNT_CLASSICAL]={0};
	std::vector<Lint> vec[USER_COUNT_CLASSICAL];

	for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
	{
		if(m_pDesk->m_user[i])
			userId[i] = m_pDesk->m_user[i]->GetUserDataId();

		score[i] = m_pDesk->m_vip->GetUserScore(m_pDesk->m_user[i]);

		//if (game_wakeng_shanxi == m_gameStyle)
		//{
		for(size_t j = 0 ; j < m_handCard[i].size(); ++j)
		{
			vec[i].push_back(m_handCard[i][j]->GetIndex());
		}
		//}
	}
	std::vector<Lint> vecGameType;
	vecGameType.push_back(game_para_maxfan);
	vecGameType.push_back(m_pDesk->m_nMaxFanshu);
	
	BYTE subType = m_subType;
	vecGameType.push_back(game_para_classic3_subType);
	vecGameType.push_back(subType);
	
	m_pDesk->m_video.DealCard(userId,vec,gWork.GetCurTime().Secs(),0,score,m_pDesk->m_id, m_pDesk->m_vip->m_curCircle, m_pDesk->m_vip->m_maxCircle, m_pDesk->m_gameStyle, vecGameType);
	m_curHelpInfo.clear();
	m_curHelpUnusable.clear();

	StartQiangDizhu();
}

void DDZHandler_ShanxiWakeng::CheckSameIP()
{
	if (m_pDesk->m_nUserCounts != USER_COUNT_CLASSICAL)
	{
		return;
	}
	Lstring szUserIP[USER_COUNT_CLASSICAL];
	for (Lint userNum = 0; userNum < m_pDesk->m_nUserCounts; ++userNum)
	{
		szUserIP[userNum] = m_pDesk->m_user[userNum]->GetIp();
	}
	Lint sameIPPos0;
	Lint sameIPPos1;
	Lint sameTimes = 0;
	for (Lint ii = 0; ii < USER_COUNT_CLASSICAL; ++ii)
	{
		for (Lint jj = ii + 1; jj < USER_COUNT_CLASSICAL; ++jj)
		{
			if (szUserIP[ii] == szUserIP[jj])
			{
				++sameTimes;
				sameIPPos0 = ii;
				sameIPPos1 = jj;
			}
		}
	}
	if (1 == sameTimes)   // ����IP����Ȳ��÷���Ϣ;
	{
		LMsgS2CSameIP msg;
		msg.m_pos[0] = sameIPPos0;
		msg.m_pos[1] = sameIPPos1;
		msg.m_nike[0] = m_pDesk->m_user[sameIPPos0]->m_userData.m_nike;
		msg.m_nike[1] = m_pDesk->m_user[sameIPPos1]->m_userData.m_nike;
		BoadCast(msg);
	}
}

void DDZHandler_ShanxiWakeng::QiangDiZhu(Lint nPos)
{
	LMsgS2CAskDiZhu msg;
	msg.m_pos = nPos;
	msg.m_difen = m_nDifen;
	msg.m_state = m_nQiangDizhuState;
	BoadCast(msg);

	//�������������ʱ;
	m_pDesk->setTimeOfWaitUser(2, nPos, 21);
	return;
}

void DDZHandler_ShanxiWakeng::HanderUserQiangDiZhu(User* pUser,LMsgC2SQiangDiZhu* msg)
{
	if(m_subType == EHeiwa)
	{
		if (m_nQiangDizhuState == EHeiwa)
		{
			HanderUserWakengOfHeiwa(pUser, msg);
		}
		else if(m_nQiangDizhuState == EJiaoFen)
		{
			HanderUserWakengOfJiaofen(pUser, msg);
		}
		else
		{
			assert(false);
			LLOG_ERROR("HanderUserQiangDiZhu Heiwa error");
		}
	}
	else if(m_subType == EJiaoFen)
	{
		HanderUserWakengOfJiaofen(pUser, msg);
	}
	/*else if(m_subType == EJIaoDizhu)
	{
		HanderUserQiangDiZhuOfJiaoDizhu(pUser, msg);
	}*/
}

void DDZHandler_ShanxiWakeng::HanderUserPlayCard(User* pUser,LMsgC2SUserPlay* msg)
{
	Lchar pos = GetUserPos(pUser);
	if(pos >= INVAILD_POS || getDeskState() != DESK_PLAY)
	{
		LLOG_ERROR("invalid pos:%d", pos);
		return;
	}

	if(m_curPos != pos)
	{
		return;
	}

	switch(msg->m_flag)
	{
	case not_play_card:
		{
			if(m_check == play_card_need_cheek)
			{
				//���ȿ�ʼ�����ܲ���;
				LLOG_ERROR("not_play_card error pos = %d",pos);
				return;
			}
			NotPlayCard(pos);
		}
		break;
	case play_card:
		{
			//����ɾ���ظ���;
			std::sort(msg->m_card.begin(), msg->m_card.end());
			std::vector<Lint>::iterator it = std::unique(msg->m_card.begin(),msg->m_card.end());
			msg->m_card.erase(it, msg->m_card.end());

			//Ѱ������;
			CardVec_Wakeng tmp;
			for(Lsize i = 0 ; i < msg->m_card.size(); ++i)
			{
				for(Lsize j = 0 ; j < m_handCard[pos].size(); ++j)
				{
					if(msg->m_card[i] ==  m_handCard[pos][j]->GetIndex())
					{
						tmp.push_back(m_handCard[pos][j]);
						break;
					}
				}
			}
			CardStyle_Shanxi_Wakeng::Instance()->SortCard(tmp);

			//�ж��ܲ��ܳ�;
			CardStyleNumberVec style;
			CardStyle_Shanxi_Wakeng::Instance()->CheckCardStyle(tmp,style,tmp.size() == m_handCard[pos].size());
			int index = CardStyle_Shanxi_Wakeng::Instance()->CanBig(m_curPlayCount,m_curCardMode,m_curCardType,style, IsUseBomb());
			if(index != -1)
			{
				PlayCard(pos,tmp,style[index].m_bCardStyle,style[index].m_bCardNumber);
			}
			else
			{
				LMsgS2CUserPlay pack;
				pack.m_errorCode=1;
				pUser->Send(pack);
				//				LLOG_ERROR("play card_mode_error pos = %d",pos);     // �������ʹ������ӡ��־;
				if (pUser->IsRobot())
				{
					m_curHelpInfo.clear();
				}
			}
		}
		break;
	}
}

//˽�к���;
Lint DDZHandler_ShanxiWakeng::getDeskState()
{
	return m_pDesk->m_deskState;
}

void DDZHandler_ShanxiWakeng::NotPlayCard(Lint pos)
{
	//¼��;
	std::vector<Lint> vi;
	m_pDesk->m_video.AddOper(VIDEO_OPER_NULL,pos, vi);

	CardVec_Wakeng vTemp;
	SendPlayCard(pos,play_card_success,not_play_card,vTemp);
	m_playerCardType[pos] = card_number_type_null;
	m_outCard[pos].clear();
	CheckPlayState(pos);
}

void DDZHandler_ShanxiWakeng::PlayCard(Lint pos, CardVec_Wakeng& card, Lchar card_mode, Lchar card_type)
{
	//¼��;
	std::vector<Lint> vec;
	for(Lsize i = 0 ; i < card.size(); ++i)
	{
		vec.push_back(card[i]->GetIndex());
	}
	m_pDesk->m_video.AddOper(card_mode,pos,vec);

	bool isFisrtOut = false;
	if (m_curCardMode == card_style_error)
	{
		isFisrtOut = true;
	}
	m_curCardMode = card_mode;
	m_curCardType = card_type;
	m_curPlayCount = (Lchar)card.size();
	if(IsUseBomb() &&(m_curCardMode == card_style_rocket || m_curCardMode == card_style_bomb1 ||
		m_curCardMode == card_style_bomb2 || m_curCardMode == card_style_bomb3))
	{
		m_bombCount += 1;
		m_bomb[pos] += 1;
	}
	SendPlayCard(pos,play_card_success,play_card,card, isFisrtOut);

	m_playerCardType[pos] = card_type;
	CardStyle_Shanxi_Wakeng::Instance()->EarseCard(m_handCard[pos],card);
	m_putCard.insert(m_putCard.end(),card.begin(),card.end());
	m_outCard[pos] = card;

	//m_outCard[GetPrevPos(pos)].clear();
	CheckPlayState(pos);
}

void DDZHandler_ShanxiWakeng::CheckPlayState(Lint pos)
{
	if(CheckPlayEnd(pos))
	{
		return;
	}

	bool islast = false;  // �����ж���һ����������Ƿ����һ����;
	if(m_handCard[pos].size() <= 2 && m_handCard[pos].size() > 0  && m_baojing[pos] == 0 )
	{
		LMsgS2COnlyOneCard msg;
		msg.m_pos = pos;
		msg.m_leftCards = m_handCard[pos].size();
		BoadCast(msg);
		m_baojing[pos] = 1;
	}

	m_curHelpInfo.clear();

	m_curPos = GetNextPos(pos);
	if(m_playerCardType[m_curPos] == m_curCardType )
	{   // ���ƺ������ܴ󵽴�;
		//if(m_curCardMode == card_style_rocket || m_curCardMode == card_style_bomb1 ||
		//m_curCardMode == card_style_bomb2 || m_curCardMode == card_style_bomb3)
		//{
		//	m_bomb[m_curPos] += 1;
		//}

		memset(m_playerCardType,card_number_type_null,sizeof(m_playerCardType));
		m_curCardMode = card_style_error;
		m_curCardType = card_number_type_null;
		m_check = play_card_need_cheek;
		m_curPlayCount = 0;

		m_outCard[0].clear();
		m_outCard[1].clear();
		m_outCard[2].clear();


		// һ�ֳ���;
		CardStyleNumberVec style;
		CardStyle_Shanxi_Wakeng::Instance()->CheckCardStyle(m_handCard[m_curPos], style, true);
		if (style.size() > 0)  // ����ܷ�һ�ֳ���,�ܹ�һ�ֳ���;
		{
			islast = true;
			if (true)/*(m_handCard[m_curPos].size() > 4 && ( CardStyle_Shanxi_Wakeng::Instance()->CheckHasBomb(m_handCard[m_curPos]) || CardStyle_Shanxi_Wakeng::Instance()->HasRocket(m_handCard[m_curPos]) ) )
			{
				islast = false;
				m_curHelpInfo.clear();
			}
			else*/
			{
				Lint nLen = m_handCard[m_curPos].size();
				std::vector<int>  yishoupai;
				for (Lint i = 0; i < nLen; ++i)
				{
					yishoupai.push_back(m_handCard[m_curPos][i]->GetIndex());
				}
				m_curHelpInfo.push_back(yishoupai);
			}
		}
	}
	else
	{   // �����ܹ�Ҫ�ƣ���������¸�����ж�;
		m_check = play_card_not_check;
		m_curHelpInfo = m_help.HelpMe(m_curCardMode, m_curCardType, m_curPlayCount, m_handCard[m_curPos], m_curHelpUnusable, m_handCard[GetNextPos(m_curPos)].size() == 1, IsUseBomb());

		// ��ʾ�ͻ����Զ�����;
		//if (m_curHelpInfo.size() == 1 && m_curHelpInfo[0].size() == m_handCard[m_curPos].size() )  // �ܹ�һ�ֳ���;
		//{
		//	islast = true;
		//}
	}

	SendPlayInfo(islast);
}

bool DDZHandler_ShanxiWakeng::CheckPlayEnd(Lint pos)
{
	if(m_handCard[pos].size() == 0)
	{
		//���һ��ը��;�ܵĿ�ը��û�б��˴�ʱ����ը�����������һ��ը���ټ��㣬���ﲹ��;
		//if(m_curCardMode == card_style_rocket || m_curCardMode == card_style_bomb1 ||
		//	m_curCardMode == card_style_bomb2 || m_curCardMode == card_style_bomb3)
		//{
		//	m_bomb[pos] += 1;
		//}
		// �������������ʣ������;
		LMsgS2CCard msg;
		for (Lint i = 0; i < USER_COUNT_CLASSICAL; ++i)   // �������ƶ���Ϣ;
		{
			for (size_t j = 0; j < m_handCard[i].size(); ++j)
			{
				msg.m_cards[i].push_back(m_handCard[i][j]->GetIndex());
			}
		}
		for (Lint i = 0; i < m_pDesk->m_nUserCounts; ++i)
		{
			msg.m_pos = i;
			m_pDesk->m_user[i]->Send(msg);
		}
		SetDeskEnd(pos);
		return true;
	}

	return false;
}

void DDZHandler_ShanxiWakeng::SetDeskEnd(Lchar pos)
{
	if (game_zone_gold == m_pDesk->m_gameZone)
	{
		SetGoldDeskEnd(pos);
		return;
	}
	Lint win[USER_COUNT_CLASSICAL] = {0};

	Ldouble gold[USER_COUNT_CLASSICAL] = {0};
	Lint chuntian[USER_COUNT_CLASSICAL] = {0};
	LMsgS2CGameOver pack;
	pack.m_win =  (pos);

	Lint nBombs = 0;
	for (Lint i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		nBombs += m_bomb[i];
	}
	size_t maxFanshu = (size_t)m_pDesk->m_nMaxFanshu;	//ؓ���鲻����
	//nMaxFanshu = nMaxFanshu < 1 ? 3 : nMaxFanshu;  //�ͻ���û�������������������������;
	nBombs = nBombs > maxFanshu ? maxFanshu : nBombs;
	if (pos == m_nDiZhuPos) //����ʤ��;
	{
		m_firstPos = pos;
		win[pos] = 1;
		Lint nextPos1 = GetNextPos(pos);
		Lint nextPos2 = GetNextPos(nextPos1);
		if (m_nCardCounts == m_handCard[nextPos1].size() && m_nCardCounts == m_handCard[nextPos2].size()) //��������;
		{
			/*nBombs += 1;		//û�д����߼�
			chuntian[pos] = 1;
			std::vector<Lint> vec;
			m_pDesk->m_video.AddOper(VIDEO_OPER_CHUNTIAN, pos, vec);*/
		}
		gold[nextPos1] = (Lint) (-m_nDifen * pow((double)2, nBombs) * m_nUserBeishu[nextPos1]);
		gold[nextPos2] = (Lint) (-m_nDifen * pow((double)2, nBombs) * m_nUserBeishu[nextPos2]);
		gold[pos] = -gold[nextPos1] - gold[nextPos2];

		for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
		{
			pack.m_count[i] = m_handCard[i].size();
			pack.m_nike[i] = m_pDesk->m_user[i]->m_userData.m_nike;
		}
	}
	//ũ��ʤ��;
	else if(pos != INVAILD_POS)
	{
		m_firstPos = pos;
		if (m_nDizhuPlayCounts == 1) //����ֻ����һ��,ũ����;
		{
			/*nBombs += 1;  //û�д����߼�
			for (Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
			{
				if (m_handCard[i].size() == 0)
				{
					chuntian[i] = 1;
					std::vector<Lint> vec;
					m_pDesk->m_video.AddOper(VIDEO_OPER_FANCHUN, i, vec);
				}
			}*/
		}
		gold[m_nDiZhuPos] = 0;
		for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
		{
			pack.m_count[i] = m_handCard[i].size();
			pack.m_nike[i] = m_pDesk->m_user[i]->m_userData.m_nike;
			if (i != m_nDiZhuPos)
			{
				win[i] = 1;
				gold[i] = (Lint) (m_nDifen * pow((double)2, nBombs) * m_nUserBeishu[i]);
				gold[m_nDiZhuPos] -= gold[i];
			}
		}	
	}
	else	{}  // pos == INVAILD_POS, do noting;

	for (Lint i = 0; i < USER_COUNT_CLASSICAL; ++i)
	{
		gold[i] = gold[i] * m_pDesk->GetStake();
	}
	memcpy(pack.m_score,gold,sizeof(gold));
	memcpy(pack.m_bomb,m_bomb,sizeof(m_bomb));
	memcpy(pack.m_chuntian,chuntian,sizeof(chuntian));

	memset(m_pDesk->m_readyState,0,sizeof(m_pDesk->m_readyState));

	m_curPos = pos;

	Ldouble cal_gold[4] = { 0,0,0,0 };
	if (m_pDesk->m_video.m_time > 0)   // ��ֹδ��ʼ�ľּ���¼��;
	{
		//����¼��;
		m_pDesk->m_video.m_Id = gVipLogMgr.GetVideoId();
		m_pDesk->m_vip->AddLog(m_pDesk->m_user, gold, cal_gold, win, m_bomb, m_pDesk->m_video.m_Id);

		LMsgL2LDBSaveVideo video;
		video.m_type = 0;
		m_pDesk->m_video.SetEveryResult(3, gold[0], gold[1], gold[2]);
		m_pDesk->m_video.SetEveryResult2(3, cal_gold[0], cal_gold[1], cal_gold[2]);
		video.m_sql = m_pDesk->m_video.GetInsertSql();
		gWork.SendMsgToDb(video);

		m_pDesk->m_video.Clear();   //
	}

	//���۷���;
	memcpy(pack.m_gold, cal_gold, sizeof(cal_gold));

	SetDeskWait();
	if(m_pDesk->m_vip->isEnd())
	{
		if(m_pDesk->m_vip->isFinalEnd())
		{
			//BoadCast(pack);
			m_pDesk->FinalEnd();
		}

		m_pDesk->m_vip->SendEnd();
		m_pDesk->m_vip->m_desk = NULL;
		SetDeskFree();
		ClearUser();
		m_pDesk->SetVip(NULL);
		ResetClear();
	}
	else
	{
		BoadCast(pack);
	}
}

void DDZHandler_ShanxiWakeng::SetGoldDeskEnd(Lchar pos)
{
	Lint actuBeishu = m_nDifen;
	m_nDifen = 5;  //��ҳ��׷�Ϊ5��; 
	Lint win[USER_COUNT_CLASSICAL] = {0};
	Lint gold[USER_COUNT_CLASSICAL] = {0};
	Lint chuntian[USER_COUNT_CLASSICAL] = {0};
	LMsgS2CGameOver pack;
	pack.m_win =  (pos);

	Lint nBombs = 0;
	for (Lint i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		nBombs += m_bomb[i];
	}
	if (pos == m_nDiZhuPos) //����ʤ��;
	{
		m_firstPos = pos;
		win[pos] = 1;
		Lint nextPos1 = GetNextPos(pos);
		Lint nextPos2 = GetNextPos(nextPos1);
		if (m_nCardCounts == m_handCard[nextPos1].size() && m_nCardCounts == m_handCard[nextPos2].size()) //��������;
		{
			nBombs += 1;
			chuntian[pos] = 1;
			std::vector<Lint> vec;
			m_pDesk->m_video.AddOper(VIDEO_OPER_CHUNTIAN, pos, vec);
		}
		Lint nMaxBeishu1 = (Lint) pow((double)2, nBombs) * actuBeishu;
		Lint nMaxBeishu2 = (Lint) pow((double)2, nBombs) * actuBeishu;
		nMaxBeishu1 = nMaxBeishu1 > 256 ? 256 : nMaxBeishu1; 
		nMaxBeishu2 = nMaxBeishu2 > 256 ? 256 : nMaxBeishu2; 
		gold[nextPos1] = -m_nDifen * nMaxBeishu1;
		gold[nextPos2] = -m_nDifen * nMaxBeishu2;
		gold[pos] = -gold[nextPos1] - gold[nextPos2];

		for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
		{
			pack.m_count[i] = m_handCard[i].size();
			pack.m_nike[i] = m_pDesk->m_user[i]->m_userData.m_nike;
		}
	}
	//ũ��ʤ��;
	else if(pos != INVAILD_POS)
	{
		m_firstPos = pos;
		if (m_nDizhuPlayCounts == 1) //����ֻ����һ��,ũ����;
		{
			nBombs += 1;
			for (Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
			{
				if (m_handCard[i].size() == 0)
				{
					chuntian[i] = 1;
					std::vector<Lint> vec;
					m_pDesk->m_video.AddOper(VIDEO_OPER_FANCHUN, i, vec);
				}
			}
		}
		gold[m_nDiZhuPos] = 0;
		for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
		{
			pack.m_count[i] = m_handCard[i].size();
			pack.m_nike[i] = m_pDesk->m_user[i]->m_userData.m_nike;
			if (i != m_nDiZhuPos)
			{
				win[i] = 1;
				Lint nMaxBeishu = (Lint) pow((double)2, nBombs) * actuBeishu;
				nMaxBeishu = nMaxBeishu > 256 ? 256 : nMaxBeishu; 
				gold[i] = m_nDifen * nMaxBeishu;
				gold[m_nDiZhuPos] -= gold[i];
			}
		}	
	}
	else	{}  // pos == INVAILD_POS, do noting;

	memcpy(pack.m_score,gold,sizeof(gold));
	memcpy(pack.m_bomb,m_bomb,sizeof(m_bomb));
	memcpy(pack.m_chuntian,chuntian,sizeof(chuntian));
	memset(m_pDesk->m_readyState,0,sizeof(m_pDesk->m_readyState));

	//��ұ仯;֪ͨ�洢��֪ͨ�ͻ���;
	for (Lint ii = 0; ii < m_pDesk->m_nUserCounts; ++ii)
	{
		m_pDesk->m_user[ii]->m_userData.m_Gold += gold[ii];
		if (m_pDesk->m_user[ii]->m_userData.m_Gold < 0)
		{
			gold[ii] -= m_pDesk->m_user[ii]->m_userData.m_Gold ;
			m_pDesk->m_user[ii]->m_userData.m_Gold = 0;
		}
		m_pDesk->m_user[ii]->send2LMGGoldChange(gold[ii], GAMERESULT, m_pDesk->m_user[ii]->m_userData.m_Gold);
	}
	m_pDesk->m_vip->m_curCircle += 1;
	SetDeskWait();
	BoadCast(pack);
	if(m_pDesk->m_vip->isEnd())
	{
		if(m_pDesk->m_vip->isFinalEnd())
		{
			m_pDesk->FinalEnd();
		}
		//m_pDesk->m_vip->SendEnd();
		m_pDesk->m_vip->m_desk = NULL;
		SetDeskFree();
		ClearUser();
		m_pDesk->SetVip(NULL);
		ResetClear();

		//ͳ�ƽ�ҳ���;
		g_nGoldGameNums++;
	}
}

void DDZHandler_ShanxiWakeng::SetDeskPlay()
{
	m_pDesk->m_deskState = DESK_PLAY;

	for(Lsize i = 0 ; i < USER_COUNT_CLASSICAL; ++i)
	{
		m_outCard[i].clear();
		m_bomb[i] = 0;
	}
	memset(m_playerCardType,card_number_type_null,sizeof(m_playerCardType));
	m_curCardMode = card_style_error;
	m_curCardType = card_number_type_null;
	m_check = play_card_need_cheek;

	//����4�����ȳ���;
	m_curPos = m_bIsHongsi? m_nHongFirstIndex: m_nDiZhuPos;

	m_bombCount = 0;
	m_putCard.clear();
	m_curHelpInfo.clear();
	m_curPlayCount = 0;

	SendPlayInfo();
}

bool DDZHandler_ShanxiWakeng::OnUserReconnect(User* pUser)
{
	Lint pos = GetUserPos(pUser);
	if (pos == INVAILD_POS)
	{
		LLOG_ERROR("Desk::OnUserReconnect pos error %d", pUser->GetUserDataId());
		return false;
	}

	if(!m_pDesk)
	{
		LLOG_ERROR("OnUserReconnect m_pDesk is nullptr");
		return false;
	}


	//��������״̬;
	LMsgS2CDeskState pack2;
	pack2.m_gameStyle = m_pDesk->m_gameStyle;
	pack2.m_diZhuPos = m_nDiZhuPos;
	pack2.m_difen = m_nDifen;
	Lint nBombs = 0;
	pack2.m_state = m_pDesk->m_deskState;
	for (Lint i = 0; i < 3; ++i)
	{
		pack2.m_CurBomb[i] = m_bomb[i];
	}
	if (m_pDesk->m_deskState == QIANG_DIZHU)
	{
		pack2.m_diZhuPos = INVAILD_VAL;
		
		if(m_subType == EJiaoFen || (m_subType == EHeiwa && m_nQiangDizhuState == EJiaoFen))
		{
			for (size_t i = 0; i < m_handCard[pos].size(); ++i) //�Լ�����;
			{
				pack2.m_card.push_back(m_handCard[pos][i]->GetIndex());
			}
		}
		
	}
	else
	{
		for (size_t i = 0; i < m_handCard[pos].size(); ++i) //�Լ�����;
		{
			pack2.m_card.push_back(m_handCard[pos][i]->GetIndex());
		}

		for (size_t i = 0; i < m_leftCard.size(); ++i)
		{
			pack2.m_dipai.push_back(m_leftCard[i]->GetIndex()); 
		}
	}

	for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i) //������ҳ���;
	{
		pack2.m_cardNum[i] = m_handCard[i].size();
		for(size_t j = 0 ; j < m_outCard[i].size(); ++j)
		{
			pack2.m_out[i].push_back(m_outCard[i][j]->GetIndex());
		}
		pack2.m_nUserBeishu[i] = m_nUserBeishu[i];

		//Gold���g��������Ƿ�Ӛ�ܱ�ʾ;
		if (m_pDesk->m_user[i] && game_zone_gold == m_pDesk->m_gameZone)
		{
			pack2.m_IsTuoguan.push_back(m_pDesk->m_user[i]->m_bTuoGuan);
		}
	}

	pUser->Send(pack2);

	if (QIANG_DIZHU == m_pDesk->m_deskState)  //�������ڼ��������;
	{
		QiangDiZhu(m_curPos);
	}
	else
	{
		//��ǰ���Ƶ����;
		LMsgS2COutCard msg;
		msg.m_pos = (m_curPos);
		msg.m_flag = (m_check);
		msg.m_last = 0;
		msg.m_time = 20;

		if(m_curPos == pos)
		{
			msg.m_card = m_curHelpInfo;
			if (play_card_not_check == msg.m_flag)
			{
				msg.m_cardUnusable = m_curHelpUnusable;
			}
		}
		pUser->Send(msg);

		for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
		{
			if(m_handCard[i].size() <= 2)
			{
				LMsgS2COnlyOneCard cardmsg;
				cardmsg.m_leftCards = m_handCard[i].size();
				cardmsg.m_pos = i;
				pUser->Send(cardmsg);
			}
		}
	}
	return true;
}

void DDZHandler_ShanxiWakeng::SendPlayInfo(bool islast)
{
	for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
	{
		LMsgS2COutCard msg;
		msg.m_pos= (m_curPos);
		msg.m_flag = (m_check);
		msg.m_time = 20;
		msg.m_last = islast;

		if(m_curPos == i)
		{
			msg.m_card = m_curHelpInfo;
			msg.m_cardUnusable = m_curHelpUnusable;
		}
		if(m_pDesk->m_user[i])
			m_pDesk->m_user[i]->Send(msg);

		//��ҳ��Ƽ�ʱ;
		m_pDesk->setTimeOfWaitUser(1, m_curPos, 21);
	}
}

void DDZHandler_ShanxiWakeng::SendPlayCard(Lchar pos, Lchar ret, Lchar action, CardVec_Wakeng& vec, bool isFirstOut)
{
	std::vector<Lint>vv;
	for(Lsize i = 0 ; i < vec.size(); ++i)
	{
		vv.push_back(vec[i]->GetIndex());
	}

	LMsgS2CUserPlay pack;
	pack.m_errorCode=(ret);
	if(ret == 1)
	{
		m_pDesk->m_user[pos]->Send(pack);
	}
	else
	{
		pack.m_pos= pos;
		pack.m_flag = action;
		if (action != 1 && pos == m_nDiZhuPos)
		{
			++m_nDizhuPlayCounts;
		}
		if (true == isFirstOut)
		{
			pack.m_flag = 2;
		}

		for(size_t i = 0 ; i < vec.size(); ++i)
		{
			pack.m_card.push_back(vec[i]->GetIndex());
		}
		if(action == 0)
		{
			pack.m_type = m_curCardMode;
		}
		else
		{
			pack.m_type = 0;
		}
		BoadCast(pack);

		//��ҳ���ȡ������Ӛ�ܵ�Ӌ�r;
		m_pDesk->unSetTimeOfWaitUser(1, pos);
	}
}

void DDZHandler_ShanxiWakeng::dealCard_new()
{
	_clearGame();

	if (!gConfig.GetDebugModel())  // ���ڲ�ģʽ���������;
	{
		m_pDesk->m_tiaopaishu = 0;
	}
	m_nCardCounts = 16;
	if (1) //0 == m_gameStyle || 3 == m_gameStyle;
	{
		m_dealCard.DealCard(m_handCard[0], m_handCard[1], m_handCard[2], m_nCardCounts, m_pDesk->m_specialCard, m_pDesk->m_tiaopaishu);
		m_dealCard.GetLeftCard(m_leftCard);   //ʣ������Ϣ;
	}

	CardStyle_Shanxi_Wakeng::Instance()->SortCard(m_handCard[0]);
	CardStyle_Shanxi_Wakeng::Instance()->SortCard(m_handCard[1]);
	CardStyle_Shanxi_Wakeng::Instance()->SortCard(m_handCard[2]);

	for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		if(m_pDesk->m_user[i])
		{
			LMsgS2CPlayStart msg;
			msg.m_gameStyle = (Lint)m_pDesk->m_gameStyle;
			/*for(size_t j = 0 ; j < m_handCard[i].size(); ++j)		//
			{
				msg.m_card.push_back(m_handCard[i][j]->GetIndex());
			}*/
			m_pDesk->m_user[i]->Send(msg);
		}
	}

	//��������λ��
	DecideWhoisFirst();
}

void DDZHandler_ShanxiWakeng::ProcessRobot(Lint pos, User * pUser)
{
	switch (m_pDesk->m_deskState)
	{
	case DESK_PLAY:
		{
			if (m_curPos == pos)
			{
				LMsgC2SUserPlay msg;
				if( m_check == 0)
				{
					msg.m_flag = 0;
					msg.m_card.push_back(m_handCard[pos].back()->GetIndex());
				}
				else
				{
					if (game_zone_gold == m_pDesk->m_gameZone)
					{
						msg.m_flag = 1;
					}
					else if(m_curHelpInfo.size())
					{
						msg.m_flag = 0;
						msg.m_card = m_curHelpInfo.back();
					}
					else
					{
						msg.m_flag = 1;
					}
				}
				HanderUserPlayCard(pUser, &msg);
			}
		}
		break;
	case QIANG_DIZHU:
		if (pos == m_curPos)
		{
			LMsgC2SQiangDiZhu msg;
			msg.m_pos = pos;

			if(m_subType == EJiaoFen)
			{
				
				msg.m_difen = m_nDifen + 1;
			}
			else
			{
				msg.m_difen = 4;
			}
			
			msg.m_yaobu = 1;
			HanderUserQiangDiZhu(pUser, &msg);
		}
		break;
	}
}

void DDZHandler_ShanxiWakeng::HanderUserWakengOfHeiwa(User* pUser,LMsgC2SQiangDiZhu* msg)
{
	if (m_pDesk->m_deskState != QIANG_DIZHU)
	{
		LLOG_ERROR("Classical Heiwa: not QiangDIzhu Time");
		return;
	}
	if (GetUserPos(pUser) != msg->m_pos)
	{
		LLOG_ERROR("user position is not msg' position: %d!", msg->m_pos);
		return;
	}
	if (msg->m_pos != m_curPos)
	{
		LLOG_ERROR("user position error! %d, is not pos: %d", m_curPos, msg->m_pos);
		return;
	}

	Lint pos = m_curPos;

	if (1 == msg->m_yaobu && m_heiwa[pos] != ERefuse)	//�л���
	{
		if (m_heiwa[pos] == EJIao)
		{
			m_nDifen = HEIWA_DIFEN;
			m_heiwa[pos] = EReady;

			//¼��
			std::vector<Lint> vec;
			vec.push_back(m_nDifen);
			RecordVideoQiangDizhu(pos, true, vec);
		}
		else
		{
			//û��������
#if USEASSERT
			assert(false);
#endif
		}
		m_firstPos = pos;
	}
	else
	{
		RecordVideoQiangDizhu(pos, false);
		m_heiwa[pos] = ERefuse;
	}

	UsersJiaoState usersState = CheckUsersHeiwaState();
	BroadCastJiaoDizhu(pos, msg->m_yaobu, usersState == EAllFail);

	if (usersState == EWait)
	{
		Lint nextPos = GetNextValidPos(pos);
#if USEASSERT
		assert(nextPos != INVAILD_VAL && m_jiaoDizhu[nextPos] != ERefuse && m_jiaoDizhu[nextPos] != EReady);
#endif
		//�쳣����//�����ܷ�������
		if (nextPos == INVAILD_VAL)
		{
#if USEASSERT
			assert(false);
#endif
			//����ϴ��
			ReStartJiaoDizhu(pos);	
			return;
		}

		//�¼Ҽ���������;
		m_curPos = nextPos;
		QiangDiZhu(nextPos);
	}
	else if (usersState == EAllFail)
	{
		//
		StartWakengOfJiaofen();
	}
	else if (usersState == EOk)
	{
		ShowCard2UserAll();
		DecideDizhu(m_firstPos);
	}

	return;
}

void DDZHandler_ShanxiWakeng::HanderUserWakengOfJiaofen(User* pUser,LMsgC2SQiangDiZhu* msg)
{
	if (m_pDesk->m_deskState != QIANG_DIZHU)
	{
		LLOG_ERROR("Classical Jiaofen: not QiangDIzhu Time");
		return;
	}
	if (GetUserPos(pUser) != msg->m_pos)
	{
		LLOG_ERROR("user position is not msg' position: %d!", msg->m_pos );
		return;
	}
	if (msg->m_pos != m_curPos)
	{
		LLOG_ERROR("user position error! %d, is not pos: %d",m_curPos, msg->m_pos );
		return;
	}

	Lint pos = m_curPos;

	if (1 == msg->m_yaobu && m_jiaoDizhu[pos] != ERefuse)	//�л���
	{
		if(m_jiaoDizhu[pos] == EJIao)
		{
			Lint jiaoFen = msg->m_difen;

			//ǿ��������
			if(jiaoFen <= m_nDifen)
				jiaoFen = m_nDifen + 1;

			m_nDifen = jiaoFen;


			//¼��
			std::vector<Lint> vec;
			vec.push_back(jiaoFen);
			RecordVideoQiangDizhu(pos, true, vec);


			if(jiaoFen < 3)
			{
				if(m_jiaoDizhu[pos] != ERefuse)
				{
					m_jiaoDizhu[pos] = EReady;
				}
			}
			else
			{
#if USEASSERT
				assert(jiaoFen <=3);
#endif

				for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
				{
					if(m_jiaoDizhu[i] != ERefuse)
					{
						m_jiaoDizhu[i] = EReady;
					}
				}
			}
		}
		else
		{
			//û��������
#if USEASSERT
			assert(false);
#endif
		}


		m_firstPos = pos;

	}
	else
	{
		RecordVideoQiangDizhu(pos, false);

		m_jiaoDizhu[pos] = ERefuse;	
	}

	UsersJiaoState usersState = CheckUsersJiaoDizhuState();

	BroadCastJiaoDizhu(pos, msg->m_yaobu, usersState == EAllFail);

	if(usersState == EWait)
	{
		Lint nextPos = GetNextValidPos(pos);
#if USEASSERT
		assert(nextPos != INVAILD_VAL && m_jiaoDizhu[nextPos] != ERefuse && m_jiaoDizhu[nextPos] != EReady );
#endif
		//�쳣����
		if(nextPos == INVAILD_VAL)
		{
#if USEASSERT
			assert(false);
#endif
			//����ϴ��(���ᷢ����
			ReStartJiaoDizhu(pos);
			return;
		}

		//�¼Ҽ���������;
		m_curPos = nextPos;
		QiangDiZhu(nextPos);
	}
	else if(usersState == EAllFail)
	{
		//�����У�Ĭ�Ϻ���Ϊ����
		m_nDifen = 1;
		DecideDizhu(m_firstPos);
	}
	else if(usersState == EOk)
	{
		DecideDizhu(m_firstPos);
	}

	return;
}

//void DDZHandler_ShanxiWakeng::HanderUserQiangDiZhuOfJiaoDizhu(User* pUser,LMsgC2SQiangDiZhu* msg)
//{
//	if (m_pDesk->m_deskState != QIANG_DIZHU)
//	{
//		LLOG_ERROR("Classical JiaoDIzhu:not QiangDIzhu Time");
//		return;
//	}
//	if (GetUserPos(pUser) != msg->m_pos)
//	{
//		LLOG_ERROR("user position is not msg' position: %d!", msg->m_pos );
//		return;
//	}
//	if (msg->m_pos != m_curPos)
//	{
//		LLOG_ERROR("user position error! %d, is not pos: %d",m_curPos, msg->m_pos );
//		return;
//	}
//	
//	Lint pos = m_curPos;
//
//	do {
//		if (1 == msg->m_yaobu)	//�л���;
//		{
//			if(m_jiaoDizhu[pos] == ERefuse){ /*assert(false);*/return;} //�ܾ����Ĳ�����Ҫ;
//			if(m_jiaoDizhu[pos] == EReady) { /*assert(false);*/return;} //Ҫ����׼������;
//
//			if(m_jiaoDizhu[pos] == EJIao)
//			{
//
//				//¼�񣨽е�����;
//				RecordVideoQiangDizhu(pos, true);
//
//
//				for(int i = 0; i < m_pDesk->m_nUserCounts ; ++i)
//				{
//					if(m_jiaoDizhu[i] != ERefuse)	//ˢ����״̬�������ܾ��ĳ��⣩;
//					{
//						m_jiaoDizhu[i] = EQiang;
//					}
//				}
//				//��
//				m_nDifen *= 2;
//			}
//			else if (m_jiaoDizhu[pos] == EQiang)
//			{
//				//¼����������;
//				RecordVideoQiangDizhu(pos, true);
//
//				m_jiaoDizhu[pos] = EReady;
//		
//				//����;
//				m_nDifen *= 2;
//			}
//			else
//			{
//				//û��������;
//#if USEASSERT
//				assert(false);
//#endif
//			}
//
//			//
//			m_firstPos = pos;
//		}
//		else
//		{
//			if(m_jiaoDizhu[pos] == EReady) {/*assert(false); */return;} //׼���õĲ����پܾ���Ҫ;
//		
//			
//			//��¼�񣨲������߲��У�;
//			RecordVideoQiangDizhu(pos, false);
//
//			//��Ҫ;
//			m_jiaoDizhu[pos] = ERefuse;	
//		}
//	}
//	while(0);
//
//	UsersJiaoState usersState = CheckUsersJiaoDizhuState();
//
//	BroadCastJiaoDizhu(pos, msg->m_yaobu, usersState == EAllFail);
//
//	//�յ��ͻ��˲�����Ϣ�����ÿͻ��˵ȴ�ʱ��;
//	m_pDesk->unSetTimeOfWaitUser(2, msg->m_pos);
//
//	if(usersState == EWait)
//	{
//		Lint nextPos = GetNextValidPos(pos);
//#if USEASSERT
//		assert(nextPos != INVAILD_VAL && m_jiaoDizhu[nextPos] != ERefuse && m_jiaoDizhu[nextPos] != EReady );
//#endif
//		if(nextPos == INVAILD_VAL)
//		{
//#if USEASSERT
//			assert(false);
//#endif
//			//����ϴ��;
//			ReStartJiaoDizhu(pos);
//			return;
//		}
//
//		m_curPos = nextPos;
//		QiangDiZhu(nextPos);
//	}
//	else if(usersState == EAllFail)
//	{
//		//����ϴ��;
//		ReStartJiaoDizhu(pos);
//	}
//	else if(usersState == EOk)
//	{
//		DecideDizhu(m_firstPos);
//	}
//		
//	return;
//}

void DDZHandler_ShanxiWakeng::StartQiangDizhu()
{
	if(m_subType == EHeiwa)
	{
		StartWakengOfHeiwa();
	}
	else if(m_subType == EJiaoFen)
	{
		StartWakengOfJiaofen();		
	}
	/*else if(m_subType == EJIaoDizhu)
	{
		StartQiangDizhuOfJiaoDizhu();
	}*/
}

void DDZHandler_ShanxiWakeng::StartWakengOfHeiwa()
{
	m_pDesk->m_deskState = QIANG_DIZHU;
	m_nQiangDizhuState = EHeiwa;

	if (m_pDesk->m_vip->m_curCircle == 0 && !m_pDesk->m_vip->m_reset) //��һ���������
	{
		Lint firstPos = L_Rand(0, 2);
		m_firstPos = firstPos;
	}
	
	m_curPos = m_firstPos;
	QiangDiZhu(m_curPos); // �ӷ�����ʼѯ���Ƿ�������;
}

void DDZHandler_ShanxiWakeng::StartWakengOfJiaofen()
{
	//�з�ǰ�׷�Ϊ��
	m_nDifen = 0;

	ShowCard2UserAll();
	
	m_pDesk->m_deskState = QIANG_DIZHU;
	m_nQiangDizhuState = EJiaoFen;
	m_curPos = m_nHongFirstIndex;
	QiangDiZhu(m_curPos); // �ӷ�����ʼѯ���Ƿ�������;
}

//void DDZHandler_ShanxiWakeng::StartQiangDizhuOfJiaoDizhu()
//{
//	m_pDesk->m_deskState = QIANG_DIZHU;
//	Lint firstPos = L_Rand(0,2);
//	m_firstPos = firstPos;
//	m_curPos = m_firstPos;
//	QiangDiZhu(m_curPos); // �ӷ�����ʼѯ���Ƿ�������;
//}

Lint DDZHandler_ShanxiWakeng::GetNextValidPos(Lint pos)
{
	Lint currPos = pos;
	Lint nextPos = INVAILD_VAL;
	
	Lint i = 0;

	while((nextPos = GetNextPos(currPos)) != pos)
	{
		if(m_jiaoDizhu[nextPos] != ERefuse && m_jiaoDizhu[nextPos] != EReady)
		{
			return nextPos;
		}
		else
		{
			currPos = nextPos;
		}

		i++;
	}

#if USEASSERT
	assert(i < 3);
#endif

	return INVAILD_VAL;
}

DDZHandler_ShanxiWakeng::UsersJiaoState DDZHandler_ShanxiWakeng::CheckUsersJiaoDizhuState()
{
	Lint readyCount = 0;
	Lint refuseCount = 0;
	Lint qiangCount = 0;

	for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		if(m_jiaoDizhu[i] == EJIao)
		{
			return EWait;
		}
		else if(m_jiaoDizhu[i] == EQiang)
		{
			qiangCount += 1;
		}
		else if(m_jiaoDizhu[i] == EReady)
		{
			readyCount += 1;
		}
		else if(m_jiaoDizhu[i] == ERefuse)
		{
			refuseCount += 1;
		}
	}

	if(qiangCount > 1 || (qiangCount == 1 && readyCount > 0))	//������������������ready
	{
		return EWait;
	}

	if(qiangCount == 1 && refuseCount == 2)	//ֻ��һ��������
	{
		return EOk;
	}

	if(readyCount > 0)
	{
		return EOk;
	}
	else
	{
#if USEASSERT
		assert(refuseCount == m_pDesk->m_nUserCounts);
#endif
		return EAllFail;
	}
}

DDZHandler_ShanxiWakeng::UsersJiaoState DDZHandler_ShanxiWakeng::CheckUsersHeiwaState()
{
	Lint readyCount = 0;
	Lint refuseCount = 0;
	Lint qiangCount = 0;

	for (int i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{

		if (m_heiwa[i] == EReady)
		{
			return EOk;
		}
		else if (m_heiwa[i] == ERefuse)
		{
			refuseCount += 1;
		}
	}

	assert(m_pDesk);
	if (refuseCount >= m_pDesk->m_nUserCounts)
	{
		return EAllFail;
	}

	return EWait;
}

void DDZHandler_ShanxiWakeng::DecideDizhu(Lint pos)
{
#if USEASSERT
	assert(pos >=0 && pos < m_pDesk->m_nUserCounts);
#endif

	m_nDiZhuPos = pos;
	LMsgS2CWhoIsDiZhu dizhuposMsg;
	dizhuposMsg.m_pos = m_nDiZhuPos;
	dizhuposMsg.m_difen = m_nDifen;
	for(size_t i = 0 ; i < m_leftCard.size(); ++i)
	{
		dizhuposMsg.m_LeftCard.push_back(m_leftCard[i]->GetIndex());
		m_handCard[m_nDiZhuPos].push_back(m_leftCard[i]);
	}
	CardStyle_Shanxi_Wakeng::Instance()->SortCard(m_handCard[m_nDiZhuPos]);
	BoadCast(dizhuposMsg);
	

	//�Ƚ�����������¼��;
	// ����¼��;
	std::vector<Lint> vec;
	for(Lsize i = 0 ; i < m_leftCard.size(); ++i)
	{
		vec.push_back(m_leftCard[i]->GetIndex());
	}
	m_pDesk->m_video.AddOper(VIDEO_OPER_DIPAI,m_nDiZhuPos,vec);
	//��ʼ����;
	SetDeskPlay();
}

void DDZHandler_ShanxiWakeng::ReStartJiaoDizhu(Lint pos)
{
	//����������,���·���;
	dealCard_new();
	//���·���¼��;
	std::vector<Lint> vec;
	for (Lint i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		for (size_t j = 0; j < m_handCard[i].size(); ++j)
		{
			vec.push_back(m_handCard[i][j]->GetIndex());
		}
	}
	m_pDesk->m_video.AddOper(VIDEO_OPER_REDEALCARD, 0, vec);

	StartQiangDizhu();
}

void DDZHandler_ShanxiWakeng::RecordVideoQiangDizhu(Lint pos, bool isQiang, const std::vector<Lint> & vec)
{
#if USEASSERT
	assert(pos >= 0 && pos < 3);
#endif

	JiaoState state = m_jiaoDizhu[pos];
	Lint subQiangType = INVAILD_VAL;

	if(isQiang)	//��
	{
		if(state == EQiang)
		{
			subQiangType = VIDEO_OPER_QIANGDIZHU;
		}
		else if(state == EJIao)
		{
			subQiangType = VIDEO_OPER_JIAODIZHU;
		}
	}
	else //����
	{
		if(state == EQiang)
			subQiangType = VIDEO_OPER_BUQIANG;
		else if(state == EJIao)
			subQiangType = VIDEO_OPER_BUJIAODIZHU;
	}

#if USEASSERT
	assert(subQiangType != INVAILD_VAL);
#endif

	m_pDesk->m_video.AddOper(subQiangType, pos, vec);
}

void DDZHandler_ShanxiWakeng::BroadCastJiaoDizhu(Lint pos, BYTE yaobu, BYTE restart)
{
	//�㲥���ѡ����;
	LMsgS2CAnsDiZhu AnsMsg;
	AnsMsg.m_pos = pos;
	AnsMsg.m_yaobu = yaobu; //�в���
	AnsMsg.m_difen = m_nDifen;
	AnsMsg.m_restart = restart;
	AnsMsg.m_state = m_nQiangDizhuState;
	
	BoadCast(AnsMsg);
}

#define CardInvalid  255
void DDZHandler_ShanxiWakeng::DecideWhoisFirst()
{
	Lint nSize = m_nCardCounts;
	assert(m_pDesk);
	std::pair<Lint, Lint> minCardUser = { CardInvalid, CardInvalid };	//user index, card index

	Card_Wakeng* pCard = nullptr;
	for (auto i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		for (auto j = nSize - 1; j >= 0; --j)
		{
			if(m_handCard[i][j]->GetColorType() == card_color_type_red)
			{
				if (minCardUser.second > m_handCard[i][j]->GetIndex())
				{
					minCardUser.second = m_handCard[i][j]->GetIndex();
					minCardUser.first = i;

					//���������
					pCard = m_handCard[i][j];
				}
				break;
			}
		}
	}

	m_bIsHongsi = pCard->GetNumberType() == card_number_type_4;
	m_nHongFirstIndex = minCardUser.first;
	assert(m_nHongFirstIndex != CardInvalid);
}

void DDZHandler_ShanxiWakeng::ShowCard2User(Lint nCurPos)
{
	LMsgS2CShowCards msg;
	msg.m_pos = nCurPos;
	std::vector<Lint> vec;
	for (size_t i = 0; i < m_handCard[nCurPos].size(); ++i)
	{
		msg.m_MyCard.push_back(m_handCard[nCurPos][i]->GetIndex());
		vec.push_back(m_handCard[nCurPos][i]->GetIndex());
	}

	assert(m_pDesk);
	auto user = m_pDesk->m_user[nCurPos];
	if(user)
	{
		user->Send(msg);
	}
		
	// ¼��;
	m_pDesk->m_video.AddOper(VIDEO_OPER_SHOWCARDS, nCurPos, vec);
}

void DDZHandler_ShanxiWakeng::ShowCard2UserAll()
{
	//չʾ����
	for (int i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		ShowCard2User(i);
	}
}

bool DDZHandler_ShanxiWakeng::IsUseBomb()
{
	assert(m_pDesk);
	if (!m_pDesk) return false;
	return m_pDesk->m_nMaxFanshu != 0;
}

///////////////////////////////////////////// ����desk����; //////////////////////////////////////////////////