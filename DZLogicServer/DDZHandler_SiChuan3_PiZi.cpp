#include "DDZHandler_SiChuan3_PiZi.h"
#include "Desk.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"
#include <cmath>

bool GameHandler_DDZ_SiChuan3_PiZi::startup(Desk *desk)
{
	if (desk)
	{
		clearHandler();
		m_pDesk = desk;
	}

	//�Ʒ�������
	m_score.Reset();
	
	return true;
}

void GameHandler_DDZ_SiChuan3_PiZi::clearHandler(void)
{
	_clearGame();
	m_firstPos = 0;   //��Ҫ������Ϣ����һ�֣����ܳ�ʼ��;
	m_nDifen = 1; // ��Ϸ�׷�Ĭ��Ϊ1;
}

//��ʼ��ÿ����Ϸ�����ʼ����������������Ϣ��Ҫ����;
void GameHandler_DDZ_SiChuan3_PiZi::_clearGame()
{
	m_score.Reset();

	//m_dianshu = INVAILD_VAL;
	//m_pokerPoint = INVAILD_VAL;
	//m_pokerStyle = INVAILD_VAL;
	//m_lightPokerIndex = INVAILD_VAL;	//����
	//m_dizhuFriendPokerIndex = INVAILD_VAL;	//�������ӵ���
	//m_nTuiZiPos = INVAILD_VAL;	//��������λ��
	m_springType = EInvalid,

	m_curPos = 0;
	m_playPos = -1;
	m_ticktime = gWork.GetCurTime().MSecs();
	for (Lint i = 0; i < USER_COUNT_PiZi3; ++i)
	{
		m_bomb[i] = 0;
		m_handCard[i].clear();
		m_playerCardType[i] = 0;
		m_outCard[i].clear();
		m_baojing[i] = 0;

		m_daobudao[i] = INVAILD_VAL;
		m_labula[i] = INVAILD_VAL;
		m_menzhua[i] = INVAILD_VAL;
		//m_nUserBeishu[i] = 1;
	}
	m_nQiangDizhuState = INVAILD_VAL;
	m_nDizhuPlayCounts = 0;
	m_nDiZhuPos = INVAILD_VAL;
	m_leftCard.clear();
	memset(m_baojing,0,sizeof(m_baojing)); //�ܵĿ챨����Ϣ;
}

void GameHandler_DDZ_SiChuan3_PiZi::SetDeskDeal()
{
	if (m_pDesk && m_pDesk->m_vip)
		m_pDesk->m_vip->SendInfo();

	dealCard_new();
	Lint userId[USER_COUNT_PiZi3] = {0};
	Ldouble score[USER_COUNT_PiZi3]={0};
	std::vector<Lint> vec[USER_COUNT_PiZi3];

	for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
	{
		if(m_pDesk->m_user[i])
			userId[i] = m_pDesk->m_user[i]->GetUserDataId();

		score[i] = m_pDesk->m_vip->GetUserScore(m_pDesk->m_user[i]);
		for(size_t j = 0 ; j < m_handCard[i].size(); ++j)
		{
			vec[i].push_back(m_handCard[i][j]->GetIndex());
		}
	}
	std::vector<Lint> vecGameType;
	vecGameType.push_back(game_para_maxfan);
	vecGameType.push_back(m_pDesk->m_nMaxFanshu);
	
#if USEASSERT
	assert(m_pDesk->m_extValue.size() > 0);
#endif
	if(m_pDesk->m_extValue.size() > 0)
	{
		vecGameType.push_back(game_para_diOrFan);
		vecGameType.push_back(m_pDesk->m_extValue[0]);	//�ӵ׼ӷ�
	}
	InitScore();
	
	m_pDesk->m_video.DealCard(userId,vec,gWork.GetCurTime().Secs(),0,score,m_pDesk->m_id, m_pDesk->m_vip->m_curCircle, m_pDesk->m_vip->m_maxCircle, m_pDesk->m_gameStyle, vecGameType);
	m_curHelpInfo.clear();
	m_curHelpUnusable.clear();

	srand((int)time(0));
	m_firstPos = rand() % 3;
	m_curPos = m_firstPos;

	m_pDesk->m_deskState = QIANG_DIZHU;
	LMsgS2CQDZQuery_Sichuan msg;
	msg.m_pos = m_curPos;
	msg.m_operType = OPER_MENZHUA;
	m_nQiangDizhuState = OPER_MENZHUA;
	BoadCast(msg);
	return;
}

void GameHandler_DDZ_SiChuan3_PiZi::CheckSameIP()
{
	if (m_pDesk->m_nUserCounts != 3)
	{
		return;
	}
	Lstring szUserIP[USER_COUNT_PiZi3];
	for (Lint userNum = 0; userNum < m_pDesk->m_nUserCounts; ++userNum)
	{
		szUserIP[userNum] = m_pDesk->m_user[userNum]->GetIp();
	}
	Lint sameIPPos0;
	Lint sameIPPos1;
	Lint sameTimes = 0;
	for (Lint ii = 0; ii < USER_COUNT_PiZi3; ++ii)
	{
		for (Lint jj = ii + 1; jj < USER_COUNT_PiZi3; ++jj)
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

void GameHandler_DDZ_SiChuan3_PiZi::QiangDiZhu(Lint nPos)
{
	LMsgS2CAskDiZhu msg;
	msg.m_pos = nPos;
	msg.m_difen = 0;
	BoadCast(msg);
	return;
}

void GameHandler_DDZ_SiChuan3_PiZi::HanderUserQiangDiZhuOper(User* pUser,LMsgC2SQdzOper_Sichuan* msg)
{
#if USEASSERT
	assert(m_pDesk);
	assert(msg);
#endif

	if (m_pDesk->m_deskState != QIANG_DIZHU)
	{
		LLOG_ERROR("Sichuan3 Game:not QiangDIzhu Time");
		return;
	}
	if (GetUserPos(pUser) != msg->m_pos)
	{
		LLOG_ERROR("user position is not msg' position: %d!", msg->m_pos );
		return;
	}
	if (m_curPos == msg->m_pos)
	{
		if (1 == msg->m_operType && 1 == msg->m_playerOper) // ��ץ;
		{
			m_nDiZhuPos = m_curPos;
			m_score.SetDizhuPos(m_nDiZhuPos);
			//�����ץ;
			m_menzhua[m_curPos] = 1;
			m_score.AddMen(1, m_curPos);
			//֪ͨ������Ҹ������ץ�˵���;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MENZHUA, OPER_YES);
			//�����������;
			LMsgS2CWhoIsDiZhu dizhuposMsg;
			dizhuposMsg.m_pos = m_nDiZhuPos;
			for(size_t i = 0 ; i < m_leftCard.size(); ++i)
			{
				dizhuposMsg.m_LeftCard.push_back(m_leftCard[i]->GetIndex());
				m_handCard[m_nDiZhuPos].push_back(m_leftCard[i]);
			}
			CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[m_nDiZhuPos]);
			ShowCard2User(m_curPos);
			m_pDesk->m_video.AddOper(VIDEO_OPER_DIPAI, m_nDiZhuPos, dizhuposMsg.m_LeftCard);
			m_pDesk->m_user[m_nDiZhuPos]->Send(dizhuposMsg);
			//ѯ��������ҵ�����;
			m_curPos = GetNextPos(m_curPos);
			QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_DAO);
		}
		else if (1 == msg->m_operType && 0 == msg->m_playerOper) //����;
		{
			//û����ץ;
			m_menzhua[m_curPos] = 0;
			//֪ͨ������Ҹ���ҿ���;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MENZHUA, OPER_NO);
			//�����������;
			ShowCard2User(m_curPos);
			QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_MINGZHUA);
		}
		else if (2 == msg->m_operType && 1 == msg->m_playerOper) //ץ;
		{
			m_nDiZhuPos = m_curPos;
			m_score.SetDizhuPos(m_nDiZhuPos);
			//֪ͨ�����˸�������˵���;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MINGZHUA, OPER_YES);
			LMsgS2CWhoIsDiZhu dizhuposMsg;
			dizhuposMsg.m_pos = m_nDiZhuPos;
			for(size_t i = 0 ; i <m_leftCard.size(); ++i)
			{
				dizhuposMsg.m_LeftCard.push_back(m_leftCard[i]->GetIndex());
				m_handCard[m_nDiZhuPos].push_back(m_leftCard[i]);
			}
			CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[m_nDiZhuPos]);
			m_pDesk->m_user[m_nDiZhuPos]->Send(dizhuposMsg);
			m_pDesk->m_video.AddOper(VIDEO_OPER_DIPAI, m_nDiZhuPos, dizhuposMsg.m_LeftCard);
			//�������������;
			for (Lint nPos = GetNextPos(m_curPos); nPos != m_curPos;nPos = GetNextPos(nPos))
			{
				if (INVAILD_VAL == m_menzhua[nPos]) //�Ѿ����Ƶ���Ҳ��ڿ���;
				{
					ShowCard2User(nPos);
				}
			}
			//ѯ��������ҵ�����;
			m_curPos = GetNextPos(m_curPos);
			QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_DAO);
		}
		else if (2 == msg->m_operType && 0 == msg->m_playerOper && (m_curPos == m_firstPos))//��ץ;
		{
			//�жϱ�ץ��;
			if (CardStyle_SiChuan3_PiZi::Instance()->IsBiZhua(m_handCard[m_curPos]))
			{
				QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_MINGZHUA);
				return;
			}
			//֪ͨ������Ҹ���Ҳ�ץ;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MINGZHUA, OPER_NO);
			//ѯ����һ������Ƿ���ץ;
			m_curPos = GetNextPos(m_curPos);
			QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_MENZHUA);
		}
		else if (2 == msg->m_operType && 0 == msg->m_playerOper && (m_curPos != m_firstPos))//��ץ;
		{
			//�жϱ�ץ��;
			if (CardStyle_SiChuan3_PiZi::Instance()->IsBiZhua(m_handCard[m_curPos]))
			{
				QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_MINGZHUA);
				return;
			}
			//֪ͨ������Ҹ���Ҳ�ץ;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MINGZHUA, OPER_NO);
			//���һ������Զ���ץ;
			m_curPos = GetNextPos(m_curPos);
			m_nDiZhuPos = m_curPos;
			m_score.SetDizhuPos(m_nDiZhuPos);
			m_menzhua[m_curPos] = 1;
			m_score.AddMen(1, m_curPos);
			//֪ͨ������Ҹ������ץ�˵���;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_MENZHUA, OPER_YES);

			//�����������;
			LMsgS2CWhoIsDiZhu dizhuposMsg;
			dizhuposMsg.m_pos = m_nDiZhuPos;
			for(size_t i = 0 ; i < m_leftCard.size(); ++i)
			{
				dizhuposMsg.m_LeftCard.push_back(m_leftCard[i]->GetIndex());
				m_handCard[m_nDiZhuPos].push_back(m_leftCard[i]);
			}
			CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[m_nDiZhuPos]);
			ShowCard2User(m_curPos);
			m_pDesk->m_video.AddOper(VIDEO_OPER_DIPAI, m_nDiZhuPos, dizhuposMsg.m_LeftCard);
			m_pDesk->m_user[m_nDiZhuPos]->Send(dizhuposMsg);

			//��ʼ��Ϸ;
			SetDeskPlay();
		}
		else if (OPER_LA == msg->m_operType && OPER_YES == msg->m_playerOper && (m_curPos == m_nDiZhuPos))//��;
		{
			//֪ͨ�������ׯ����;
			m_labula[m_nDiZhuPos] = 1;
			m_score.AddLa(1, m_nDiZhuPos);
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_LA, OPER_YES);
			//��ʼ����;
			SetDeskPlay();
		}
		else if (OPER_LA == msg->m_operType && OPER_NO == msg->m_playerOper && (m_curPos == m_nDiZhuPos))//����;
		{
			//֪ͨ�������ׯ�Ҳ���;
			m_labula[m_nDiZhuPos] = 0;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_LA, OPER_NO);
			//��ʼ����;
			SetDeskPlay();
		}
		else if (3 == msg->m_operType && 1 == msg->m_playerOper)  //��;
		{
			//֪ͨ������Ҹ���ҵ�;
			m_daobudao[msg->m_pos] = 1;
			m_score.AddDao(1, msg->m_pos);
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_DAO, OPER_YES);
			//�����ץ�������������;
			if (1 == m_menzhua[m_nDiZhuPos] && -1 == m_menzhua[m_curPos])
			{
				ShowCard2User(msg->m_pos);
			}
			m_curPos = GetNextPos(m_curPos);
			if (m_curPos != m_firstPos )
			{
				QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_DAO);
				return;
			}
			if (1 == m_menzhua[m_nDiZhuPos])
			{
				//��ץ���������[���_ʼ;
				SetDeskPlay();
				return;
			}
			m_curPos = m_nDiZhuPos;
			QueryUserOper(m_nDiZhuPos, MSG_S_2_C_QDZQUERY_NEW, OPER_LA);
		}
		else if (3 == msg->m_operType && 0 == msg->m_playerOper) //����;
		{
			//�жϱص���;
			if (1 != m_menzhua[m_nDiZhuPos] && CardStyle_SiChuan3_PiZi::Instance()->IsBiDao(m_handCard[m_curPos]))
			{
				QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_DAO);
				return;
			}
			m_daobudao[msg->m_pos] = 0;
			//֪ͨ������Ҹ���Ҳ���;
			BoadCastUserOper(m_curPos, MSG_S_2_C_QDZOPER_NEW, OPER_DAO, OPER_NO);
			//�����ץ�������δ����;�����������;
			if (1 == m_menzhua[m_nDiZhuPos] && -1 == m_menzhua[m_curPos])
			{
				ShowCard2User(msg->m_pos);
			}
			m_curPos = GetNextPos(m_curPos);
			if (m_curPos != m_firstPos )
			{
				QueryUserOper(m_curPos, MSG_S_2_C_QDZQUERY_NEW, OPER_DAO);
				return;
			}
			if (1 == m_menzhua[m_nDiZhuPos])
			{
				//��ץ���������[���_ʼ;
				SetDeskPlay();
				return;
			}
			//������ҵ����;���͵���������;���߿�ʼ��Ϸ;
			if (OPER_YES == m_daobudao[GetNextPos(m_nDiZhuPos)] || OPER_YES == m_daobudao[GetPrevPos(m_nDiZhuPos)] )
			{
				QueryUserOper(m_nDiZhuPos, MSG_S_2_C_QDZQUERY_NEW, OPER_LA);
			}
			else
			{
				//��ʼ����;
				SetDeskPlay();
			}
		}
	}
	else   //������Ϣ;
	{
	}
}

void GameHandler_DDZ_SiChuan3_PiZi::HanderUserPlayCard(User* pUser,LMsgC2SUserPlay* msg)
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
			CardVec tmp;
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
			CardStyle_SiChuan3_PiZi::Instance()->SortCard(tmp);


			CardStyleNumberVec style;

			//����������ж�;
			if(card_style_error != msg->m_style)
			{
				BYTE nStype = msg->m_style;
				BYTE cardNumber = card_number_type_null;
				
				if(CardStyle_SiChuan3_PiZi::Instance()->CheckCardStyle(tmp, nStype, cardNumber))
				{
					CardStyleNumber node;
					node.m_bCardStyle = nStype;
					node.m_bCardNumber = cardNumber;
					node.m_bCardCount = BYTE(tmp.size());
					style.push_back(node);
				}
			}
			else
			{
				CardStyle_SiChuan3_PiZi::Instance()->CheckCardStyle(tmp,style,tmp.size() == m_handCard[pos].size(), m_curCardMode);
			}

			//�ж��ܲ��ܳ�;
			int index = CardStyle_SiChuan3_PiZi::Instance()->CanBig(m_curPlayCount, m_curCardMode, m_curCardType, style);
			if(index != -1 && IsPlayByFilter(style[index].m_bCardStyle, tmp.size() == m_handCard[pos].size()) && !IsLeftLaizi(tmp, pos))
			{
				PlayCard(pos,tmp,style[index].m_bCardStyle,style[index].m_bCardNumber);
			}
			else
			{
				LMsgS2CUserPlay pack;
				pack.m_errorCode=1;
				pUser->Send(pack);
				//				LLOG_ERROR("play card_mode_error pos = %d",pos);     // �������ʹ������ӡ��־;
			}
		}
		break;
	}
}

//˽�к���;
Lint GameHandler_DDZ_SiChuan3_PiZi::getDeskState()
{
	return m_pDesk->m_deskState;
}

void GameHandler_DDZ_SiChuan3_PiZi::NotPlayCard(Lint pos)
{
	//¼��;
	std::vector<Lint> vi;
	m_pDesk->m_video.AddOper(VIDEO_OPER_NULL,pos, vi);

	CardVec vTemp;
	SendPlayCard(pos,play_card_success,not_play_card,vTemp);
	m_playerCardType[pos] = card_number_type_null;
	m_outCard[pos].clear();
	CheckPlayState(pos);
}

void GameHandler_DDZ_SiChuan3_PiZi::PlayCard(Lint pos, CardVec& card, Lchar card_mode, Lchar card_type)
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

	//assert(pos == m_curPos);
	m_playPos = pos;

	m_curPlayCount = (Lchar)card.size();

	DoBomb(pos);

	SendPlayCard(pos,play_card_success,play_card,card, isFisrtOut);

	m_playerCardType[pos] = card_type;
	CardStyle_SiChuan3_PiZi::Instance()->EarseCard(m_handCard[pos],card);
	m_putCard.insert(m_putCard.end(),card.begin(),card.end());
	m_outCard[pos] = card;

	//m_outCard[GetPrevPos(pos)].clear();
	CheckPlayState(pos);
}

void GameHandler_DDZ_SiChuan3_PiZi::CheckPlayState(Lint pos)
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
	if(m_playPos == m_curPos )
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
		CardStyle_SiChuan3_PiZi::Instance()->CheckCardStyle(m_handCard[m_curPos], style, true);
		if (style.size() > 0)  // ����ܷ�һ�ֳ���,�ܹ�һ�ֳ���;
		{
			islast = true;
			if (m_handCard[m_curPos].size() > 4 && ( CardStyle_SiChuan3_PiZi::Instance()->CheckHasBomb(m_handCard[m_curPos]) || CardStyle_SiChuan3_PiZi::Instance()->HasRocket(m_handCard[m_curPos]) ) )
			{
				islast = false;
				m_curHelpInfo.clear();
			}
			else
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

		//Ŀǰֻ����ͨ�ƽ�����ʾ;
		m_curHelpInfo.clear();
		//m_curHelpInfo = m_help.HelpMe(m_curCardMode, m_curCardType, m_curPlayCount, m_handCard[m_curPos], m_curHelpUnusable, m_handCard[GetNextPos(m_curPos)].size() == 1);

		//������Ƶ����;
		/*if(m_curHelpInfo.size() == 0 && m_pDesk && m_pDesk->m_user && m_pDesk->m_user[m_curPos] && !m_pDesk->m_user[m_curPos]->IsRobot())
		{
			CardVec laizi, normal;
			CardStyle_SiChuan3_PiZi::Instance()->GetLaizi(m_handCard[m_curPos], laizi, normal);
			if(laizi.size() >= 1)
			{
				std::vector<Lint> ret;
				ret.push_back(laizi[0]->GetIndex());
				m_curHelpInfo.push_back(ret);
			}
		}*/

		// ��ʾ�ͻ����Զ�����;
		//if (m_curHelpInfo.size() == 1 && m_curHelpInfo[0].size() == m_handCard[m_curPos].size() )  // �ܹ�һ�ֳ���;
		//{
		//	islast = true;
		//}
	}

	SendPlayInfo(islast);
}

bool GameHandler_DDZ_SiChuan3_PiZi::CheckPlayEnd(Lint pos)
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
		for (Lint i = 0; i < USER_COUNT_PiZi3; ++i)   // �������ƶ���Ϣ;
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

void GameHandler_DDZ_SiChuan3_PiZi::SetDeskEnd(Lchar pos)
{
	Lint win[USER_COUNT_PiZi3] = {0};
	if (pos < m_pDesk->m_nUserCounts && m_pDesk->m_nUserCounts >= 0)
	{
		//ʤ������;
		for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
		{
			if(pos == m_nDiZhuPos)
			{
				win[m_nDiZhuPos] = 1;
				break;
			}
			else if(i != m_nDiZhuPos)
			{
				win[i] = 1;
			}
		}
	}
	
	LMsgS2CGameOver pack;
	pack.m_win =  (pos);

	Ldouble gold[USER_COUNT_PiZi3] = {0};

	GetScore(pos, gold);



	//�������
	Lint chuntian[USER_COUNT_PiZi3] = {0};
	for (Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i)
	{
		pack.m_count[i] = m_handCard[i].size();
		pack.m_nike[i] = m_pDesk->m_user[i]->m_userData.m_nike;

		if (i == pos && m_springType >= EDizhuSpring)
		{
			chuntian[i] = 1;
		}
	}

	memcpy(pack.m_score,gold,sizeof(gold));
	memcpy(pack.m_bomb,m_bomb,sizeof(m_bomb));
	memcpy(pack.m_chuntian, chuntian, sizeof(chuntian));
	memset(m_pDesk->m_readyState,0,sizeof(m_pDesk->m_readyState));

	m_curPos = pos;

	//����������¼���¼
	if(m_springType >= EDizhuSpring)
	{
		Lint videoChunType = m_springType == EDizhuSpring? VIDEO_OPER_CHUNTIAN : VIDEO_OPER_FANCHUN ;
		std::vector<Lint> vec;
		m_pDesk->m_video.AddOper(videoChunType, pos, vec);
	}
	for (Lint i = 0; i < USER_COUNT_PiZi3; ++i)
	{
		gold[i] = gold[i] * m_pDesk->GetStake();
	}
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

void GameHandler_DDZ_SiChuan3_PiZi::SetDeskPlay()
{
	//�Ĵ����������������ץ��������Ϸ��ʼǰ���������չʾ��������;
	if (0 == m_menzhua[m_nDiZhuPos])
	{
		LMsgS2CWhoIsDiZhu dizhuposMsg;
		dizhuposMsg.m_pos = m_nDiZhuPos;
		for(size_t i = 0 ; i <m_leftCard.size(); ++i)
		{
			dizhuposMsg.m_LeftCard.push_back(m_leftCard[i]->GetIndex());
		}
		for (Lint j = 0; j < m_pDesk->m_nUserCounts; ++j)
		{
			if (m_nDiZhuPos != j)
			{
				m_pDesk->m_user[j]->Send(dizhuposMsg);
			}
		}
	}

	m_pDesk->m_deskState = DESK_PLAY;

	for(Lsize i = 0 ; i < USER_COUNT_PiZi3; ++i)
	{
		m_outCard[i].clear();
		m_bomb[i] = 0;
	}
	memset(m_playerCardType,card_number_type_null,sizeof(m_playerCardType));
	m_curCardMode = card_style_error;
	m_curCardType = card_number_type_null;
	m_check = play_card_need_cheek;

	//�������ȳ���;
	m_curPos = m_nDiZhuPos;
	m_firstPos = m_nDiZhuPos;

	//m_bombCount = 0;
	m_putCard.clear();
	m_curHelpInfo.clear();
	m_curPlayCount = 0;

	SendPlayInfo();
}

bool GameHandler_DDZ_SiChuan3_PiZi::OnUserReconnect(User* pUser)
{
	Lint pos = GetUserPos(pUser);
	if (pos == INVAILD_POS)
	{
		LLOG_ERROR("Desk::OnUserReconnect pos error %d", pUser->GetUserDataId());
		return false;
	}

	//��������״̬;
	LMsgS2CDeskState pack2;
	pack2.m_gameStyle = m_pDesk->m_gameStyle;
	pack2.m_diZhuPos = m_nDiZhuPos;
	pack2.m_pokerStyle = m_curCardMode;

	//����������Ϣ;
	pack2.m_Int_1.push_back(m_score.GetTurnCount());	//��ǰ����;
	Lint nBombs = 0;
	pack2.m_state = m_pDesk->m_deskState;
	assert(m_pDesk->m_nUserCounts >= 0);
	pack2.m_daobudao.resize(m_pDesk->m_nUserCounts);
	pack2.m_labula.resize(m_pDesk->m_nUserCounts);
	for (Lint i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		pack2.m_CurBomb[i] = m_bomb[i];
		pack2.m_daobudao[i] = m_daobudao[i];
		pack2.m_labula[i] = m_labula[i];
	}
	if (m_pDesk->m_deskState == QIANG_DIZHU)
	{
			pack2.m_diZhuPos = m_nDiZhuPos;
			if ( m_menzhua[pos] != INVAILD_VAL || INVAILD_VAL != m_daobudao[pos] || (INVAILD_VAL != m_nDiZhuPos && m_menzhua[m_nDiZhuPos] == OPER_NO) )
			{
				for(size_t i = 0 ; i < m_handCard[pos].size(); ++i) //�Լ�����;
				{	
					pack2.m_card.push_back(m_handCard[pos][i]->GetIndex());
				}
			}
	}
	else
	{
		if (pos == m_nDiZhuPos || 0 == m_menzhua[m_nDiZhuPos])  //��ץ����չʾ��������;
		{
			for (size_t i = 0; i < m_leftCard.size(); ++i)
			{
				pack2.m_dipai.push_back(m_leftCard[i]->GetIndex());  //�Ĵ�����������ֻ�е���֪��;
			}
		}
		for(size_t i = 0 ; i < m_handCard[pos].size(); ++i) //�Լ�����;
		{	
			pack2.m_card.push_back(m_handCard[pos][i]->GetIndex());
		}
	}

	for(Lint i = 0 ; i < m_pDesk->m_nUserCounts; ++i) //������ҳ���;
	{
		pack2.m_cardNum[i] = m_handCard[i].size();
		for(size_t j = 0 ; j < m_outCard[i].size(); ++j)
		{
			if(m_outCard[i][j]->GetLaizi())
			{
				pack2.m_laiziNumber.push_back(m_outCard[i][j]->GetLaiziNumber());
			}
			
			pack2.m_out[i].push_back(m_outCard[i][j]->GetIndex());
		}
		pack2.m_nUserBeishu[i] = m_score.GetScore(pos);
	}

	pUser->Send(pack2);

	if (QIANG_DIZHU == m_pDesk->m_deskState && pos == m_curPos)  //�������ڼ��������;
	{
		LMsgS2CQDZQuery_Sichuan msg;
		msg.m_pos = m_curPos;
		msg.m_operType = m_nQiangDizhuState;
		BoadCast(msg);
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

void GameHandler_DDZ_SiChuan3_PiZi::SendPlayInfo(bool islast)
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
	}
}

void GameHandler_DDZ_SiChuan3_PiZi::SendPlayCard(Lchar pos, Lchar ret, Lchar action, CardVec& vec, bool isFirstOut)
{
	/*std::vector<Lint>vv;
	for(Lsize i = 0 ; i < vec.size(); ++i)
	{
		vv.push_back(vec[i]->GetIndex());
	}*/ //û���õ�

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
			if(vec[i]->GetLaizi())
			{
				pack.m_laiziNumber.push_back(vec[i]->GetLaiziNumber());
			}
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
	}
}

void GameHandler_DDZ_SiChuan3_PiZi::dealCard_new()
{
#if USEASSERT
	assert(m_pDesk);
#endif

	if (!m_pDesk)
	{
		LLOG_ERROR("dealCard_new m_pDesk is nullptr");
		return;
	}

	_clearGame();

	if (!gConfig.GetDebugModel())  // ���ڲ�ģʽ���������;
	{
		m_pDesk->m_tiaopaishu = 0;
	}

	//�л�����ʱ������ҵ����
	if (gConfig.GetIfAddRobot() && m_pDesk->HasRobot())  
	{
		CardValue cardV;
		cardV.m_color = 0;
		cardV.m_number = card_number_type_LZ + 2;

#if USEASSERT
		assert(m_pDesk->m_tiaopaishu <= 54 && m_pDesk->m_tiaopaishu >= 0);
#endif
		for(int i = m_pDesk->m_tiaopaishu; i >= 0 ; --i)
		{
			m_pDesk->m_specialCard[i+1] = m_pDesk->m_specialCard[i];
		}

		m_pDesk->m_specialCard[0] = cardV;
		m_pDesk->m_tiaopaishu += 1;
	}
	//end
	
	//������
	m_nCardCounts = 17;
	if (1) //0 == m_gameStyle || 3 == m_gameStyle;
	{
		m_dealCard.DealCard(m_handCard, 3, m_pDesk->m_specialCard, m_pDesk->m_tiaopaishu);
		m_dealCard.GetLeftCard(m_leftCard);   //ʣ������Ϣ;
	}

	//����&����17�ţ�
	CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[0]);
	CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[1]);
	CardStyle_SiChuan3_PiZi::Instance()->SortCard(m_handCard[2]);

	for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
	{
		if(m_pDesk->m_user[i])
		{
			LMsgS2CPlayStart msg;
			msg.m_gameStyle = (Lint)m_pDesk->m_gameStyle;
			m_pDesk->m_user[i]->Send(msg);
		}
	}
}

void GameHandler_DDZ_SiChuan3_PiZi::DoBomb(Lint pos)
{
#if USEASSERT
	assert(pos == m_curPos);
#endif

	if(m_curCardMode == card_style_rocket ||
		m_curCardMode == card_style_bomb1 ||	//��ͨ1
		m_curCardMode == card_style_bomb2 ||
		m_curCardMode == card_style_bomb3)
	{
		m_score.AddBomb(1);
		m_bomb[pos] += 1;
	}
	else if(m_curCardMode == card_style_supper_rocket ||
		m_curCardMode == card_style_super_bomb)	//��ը
	{
		m_score.AddBomb(2);
		m_bomb[pos] += 1;
	}
	else if(m_curCardMode == card_style_bomb_double)	//��ը3
	{
		m_score.AddBomb(3);
		m_bomb[pos] += 1;
	}
}

//����ʱʵ��
bool GameHandler_DDZ_SiChuan3_PiZi::IsPlayByFilter( Lint type, bool last){

	if(type == card_style_three && !last)	//Ʀ�Ӷ�����һ�����򣨲��ܵ���3�ԣ�����
	{
		return false;
	}

	if(type == card_style_three_double)		//������2 
	{
		return false;
	}

	if(type == card_style_bomb_and_single || type == card_style_four2)	//4��2
	{
		return false;
	}

	if(type == card_style_three_list_double)	//�ɻ�����
	{
		return false;
	}

	if(last && type == card_style_three_list)	//���õķɻ���������
	{
		return false;
	}

	if(type == card_style_laizi)		//����Ʋ��ܵ���
	{
		return false;
	}

	return true;
}


//bool GameHandler_DDZ_SiChuan3_PiZi::IsAllUserDone(User::UserRoomState state)
//{
//#if USEASSERT
//	assert(m_pDesk);
//#endif
//
//	for(int i = 0; i < m_pDesk->m_nUserCounts; i++)
//	{
//		if(m_pDesk->m_user[i])
//		{
//			if(!m_pDesk->m_user[i]->GetDone(state))
//			{
//				return false;
//			}
//		}
//	}
//
//	return true;
//}

void GameHandler_DDZ_SiChuan3_PiZi::BoadCastUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType, Lint nOperRel)
{
	//UpdateUserBeishu();
	if (MSG_S_2_C_QDZOPER_NEW == nMsgID)
	{
		LMsgS2CQdzOper_Sichuan msg;
		msg.m_pos = nCurPos;
		msg.m_operType = nOperType;
		msg.m_playerOper = nOperRel;

		for (Lint i = 0; i  < m_pDesk->m_nUserCounts; ++i)
		{
			msg.m_nUserBeishu[i] = m_score.GetScore(i);
		}
		
		BoadCast(msg);
	}
	std::vector<Lint> vec;
	m_pDesk->m_video.AddOper( nOperType*100 + nOperRel, nCurPos,vec);
}


void GameHandler_DDZ_SiChuan3_PiZi::QueryUserOper(Lint nCurPos, Lint nMsgID, Lint nOperType)
{
	if (MSG_S_2_C_QDZQUERY_NEW == nMsgID)
	{
		m_nQiangDizhuState = nOperType;
		LMsgS2CQDZQuery_Sichuan msg;
		msg.m_pos = nCurPos;
		msg.m_operType = nOperType;
		BoadCast(msg);
	}
}

void GameHandler_DDZ_SiChuan3_PiZi::ProcessRobot(Lint pos, User * pUser)
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
					msg.m_style = 0;
				}
				else
				{
					if(m_curHelpInfo.size())
					{
						msg.m_flag = 0;
						msg.m_card = m_curHelpInfo.back();
						msg.m_style = 0;
					}
					else
					{
						msg.m_flag = 1;
						msg.m_style = 0;
					}
				}
				HanderUserPlayCard(pUser, &msg);
			}
		}
		break;
	case QIANG_DIZHU:
		if (pos == m_curPos)
		{
			LMsgC2SQdzOper_Sichuan msg;
			msg.m_pos = pos;
			msg.m_operType = m_nQiangDizhuState;
			msg.m_playerOper = rand()%2;
			HanderUserQiangDiZhuOper(pUser, &msg);
		}
		break;
	}
}

//ũ���죬��������;
bool GameHandler_DDZ_SiChuan3_PiZi::IsNongMinSpring(Lint winPos)	
{
	if (winPos == m_nDiZhuPos)
	{
		return false;
	}

	bool isSpring = m_nDizhuPlayCounts == 1;
	
	if(isSpring)
	{
		m_springType = ENongminSpring;
	}

	return isSpring;
}

//�����Ƿ���;
bool GameHandler_DDZ_SiChuan3_PiZi::IsLandlordSpring(Lint winPos)	
{
	if (winPos != m_nDiZhuPos) //����ûʤ��;
	{
		return false;
	}

	std::vector<BYTE> vecDizhu;
	std::vector<BYTE> vecNongmin;

	for(int i = 0; i< m_pDesk->m_nUserCounts; ++i)
	{
		if(m_pDesk->m_user[i])
		{
			if(i == m_nDiZhuPos)
			{
				vecDizhu.push_back(i);
			}
			else
			{
				vecNongmin.push_back(i);
			}
		}
	}

#if USEASSERT
	assert(vecDizhu.size() + vecNongmin.size() == 4);
#endif

	int nongminSize = vecNongmin.size();

	for (int i = 0; i <nongminSize ; ++i)
	{
		if(m_nCardCounts != m_handCard[vecNongmin[i]].size())
		{
			return false;
		}
	}

	m_springType = EDizhuSpring;

	return true;
}

void GameHandler_DDZ_SiChuan3_PiZi::GetScore(Lint winPos, Ldouble * gold)
{
	if (winPos == m_nDiZhuPos) //����ʤ��;
	{
#if USEASSERT
		assert(m_nDiZhuPos != INVAILD_VAL);
#endif

		//
		bool isDizhuSpr = IsLandlordSpring(winPos);
		if(isDizhuSpr)	//��������
		{
			m_score.AddBomb(1);
		}

		//
		if(true) //һ��2
		{


			//int score = m_score.GetScore();
			//int scoreWin = score * 2;
#if USEASSERT
			assert(score > 0);
#endif

			gold[m_nDiZhuPos] = m_score.GetScore(m_nDiZhuPos);
			for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
			{
				if(i != m_nDiZhuPos && m_pDesk->m_user[i])
				{
					gold[i] = - m_score.GetScore(i);
				}
			}
		}
	}
	//ũ��ʤ��;
	else if(winPos != INVAILD_POS)
	{
#if USEASSERT
		assert(m_nDiZhuPos != INVAILD_VAL);
#endif

		m_firstPos = winPos;

		if (IsNongMinSpring(winPos)) //����ֻ����һ��,ũ����;
		{
			m_score.AddBomb(2);
		}
		gold[m_nDiZhuPos] = 0;
	

		//int score = m_score.GetScore();

#if USEASSERT
		assert(score > 0);
#endif

		if(true)	//һ��2
		{
			//int score = m_score.GetScore();
			//int scoreLost = score * 2; 

#if USEASSERT
			assert(score > 0);
#endif

			gold[m_nDiZhuPos] = -m_score.GetScore(m_nDiZhuPos);
			for(int i = 0; i < m_pDesk->m_nUserCounts; ++i)
			{
				if(i != m_nDiZhuPos && m_pDesk->m_user[i])
				{
					gold[i] = +m_score.GetScore(i);
				}
			}
		}
	}
	else	{}  // pos == INVAILD_POS, do noting;
}

void GameHandler_DDZ_SiChuan3_PiZi::InitScore()
{
	//�Ʒ�������
	m_score.Reset();

	if(m_pDesk->m_extValue.size() == 0)
	{
		m_score.Init(1, m_pDesk->m_nMaxFanshu, 1); //Ĭ��
	}
	else
	{
		Lint type = m_pDesk->m_extValue[0];
		m_score.Init(type, m_pDesk->m_nMaxFanshu, 1); //
	}

	
	
}

bool GameHandler_DDZ_SiChuan3_PiZi::IsLeftLaizi(const CardVec & out, Lint pos)
{
#if USEASSERT
	assert(pos >= 0 && pos < m_pDesk->m_nUserCounts);
#endif
	
	Lint allSize = m_handCard[pos].size();
	if(out.size() + 1 == allSize)
	{
		Lint size = out.size();
		bool isInOut = false;
		bool isInAll = false;
		for(int i = 0; i < allSize; ++i)
		{
			if(i < size && out[i]->GetLaizi())
			{
				isInOut = true;
				return false;
			}

			if(m_handCard[pos][i]->GetLaizi())
			{
				isInAll = true;
			}
		}

		if(!isInOut && isInAll)
		{
			return true;
		}
	}
	
	return false;
}

void GameHandler_DDZ_SiChuan3_PiZi::ShowCard2User(Lint nCurPos)
{
	LMsgS2CShowCards msg;
	msg.m_pos = nCurPos;
	std::vector<Lint> vec;
	for (size_t i = 0; i < m_handCard[nCurPos].size(); ++i)
	{
		msg.m_MyCard.push_back(m_handCard[nCurPos][i]->GetIndex());
		vec.push_back(m_handCard[nCurPos][i]->GetIndex());
	}
	m_pDesk->m_user[nCurPos]->Send(msg);
	// ¼��;
	m_pDesk->m_video.AddOper(VIDEO_OPER_SHOWCARDS, nCurPos, vec);
}

///////////////////////////////////////////// ����desk����; //////////////////////////////////////////////////