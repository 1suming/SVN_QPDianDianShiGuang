#include "GameHandler.h"
#include "Desk.h"
#include "Card.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"

class GameHandler_XiaoYi : public GameHandler
{
	//�������������Լ�������Ӧ���з��ף����������
	class LastCard
	{
	public:
		Lint  m_nCardNC;    //������������

		Lint  m_nType;       //0 ƽ��   1 �߶�    2 ʮ����     

		Lint  m_nHuColor;   //������ɫ

		LastCard() :m_nCardNC(0),  m_nType(0), m_nHuColor(0) {}

		LastCard(Lint nCardNC, Lint nHuColor, Lint nZhong, Lint nHei) :m_nCardNC(nCardNC), m_nType(0), m_nHuColor(nHuColor) {}

		bool operator<(const LastCard& other) const
		{
			if (m_nCardNC < other.m_nCardNC)
			{
				return true;
			}
			else if (m_nCardNC > other.m_nCardNC)
			{
				return false;
			}
			else if (m_nType  < other.m_nType)
			{
				return true;
			}
			else if (m_nType  > other.m_nType)
			{
				return false;
			}
			else if (m_nHuColor  < other.m_nHuColor)
			{
				return true;
			}
			else if (m_nHuColor  > other.m_nHuColor)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	};

	//����״̬�µĿɺ�����
	typedef  std::set<LastCard>   LastCardSet;
public:
	bool startup(Desk *desk)
	{
		if (desk)
		{
			shutdown();
			m_desk = desk;
			m_playtype.clear();
		}
		return true;
	}

	void shutdown(void)
	{
		m_desk = NULL;
		m_curOutCard = NULL;
		m_curGetCard = NULL;
		for (Lint i = 0; i< DESK_USER_COUNT; i++)
		{
			m_thinkInfo[i].Reset();
			m_thinkRet[i].Clear();
			m_handCard[i].clear();
			m_outCard[i].clear();
			m_pengCard[i].clear();
			m_minggangCard[i].clear();
			m_angangCard[i].clear();
			m_eatCard[i].clear();
			m_angang[i] = 0;
			m_minggang[i] = 0;
			m_diangang[i] = 0;
			m_adiangang[i] = 0;
			m_gangPos[i] = 0;
			m_playerHuInfo[i] = 0;
			m_playerBombInfo[i] = 0;
			m_playerIsTing[i] = 0;
			m_isAutoCard[i] = false;
			m_tingCard[i].clear();
			m_setNowTingCard[i].clear();
			m_isTingHu[i] = false;
		}
		memset(m_diangangRelation, 0, sizeof(m_diangangRelation));

		m_beforePos = 0;
		m_beforeType = 0;
		m_gold = 0;
		m_zhuangpos = 0;
		m_curPos = 0;
		m_needGetCard = false;
		m_isThinkingQiangGang = false;
		m_deskCard.clear();
		m_curOutCards.clear();
		m_first_turn = false;
		mGameInfo.m_gangcard.clear();
	}

	void DeakCard()
	{
		if (!m_desk || !m_desk->m_vip)
		{
			return;
		}
		for (Lint i = 0; i < DESK_USER_COUNT; ++i)
		{
			m_handCard[i].clear();
			m_outCard[i].clear();
			m_pengCard[i].clear();
			m_minggangCard[i].clear();
			m_angangCard[i].clear();
			m_eatCard[i].clear();
			m_thinkInfo[i].Reset();
			m_thinkRet[i].Clear();
			m_playerIsTing[i] = 0;
			m_isAutoCard[i] = false;
			m_tingCard[i].clear();
			m_setNowTingCard[i].clear();
			m_isTingHu[i] = false;
		}
		m_deskCard.clear();
		m_curOutCard = NULL;//��ǰ����������
		m_curGetCard = NULL;
		m_needGetCard = false;
		m_isThinkingQiangGang = false;
		m_haidi_zhuangpos = INVAILD_POS;
		m_curPos = m_zhuangpos;
		m_desk->UpdateLastOptTimeAndPos(m_curPos);
		m_beforePos = INVAILD_POS;
		mGameInfo.m_gangcard.clear();
		m_first_turn = true;
		
		memset(m_diangangRelation, 0, sizeof(m_diangangRelation));
		memset(m_angang, 0, sizeof(m_angang));//��������
		memset(m_minggang, 0, sizeof(m_minggang));
		memset(m_diangang, 0, sizeof(m_diangang));
		memset(m_adiangang, 0, sizeof(m_adiangang));
		memset(m_gangPos, 0, sizeof(m_gangPos));
		memset(m_playerHuInfo, 0, sizeof(m_playerHuInfo));
		memset(m_playerBombInfo, 0, sizeof(m_playerBombInfo));

		//����   
		if (gConfig.GetDebugModel() && (m_desk->m_specialCard[0].m_color > 0 && m_desk->m_specialCard[0].m_number > 0))   //���ָ������ �ƾ�
		{
			gCardMgr.DealCard2(m_handCard, m_desk->m_desk_user_count, m_deskCard, m_desk->m_specialCard, m_desk->getGameType(), m_playtype);
		}
		else                //����������� �ƾ�
		{
			gCardMgr.DealCard(m_handCard, m_desk->m_desk_user_count, m_deskCard, m_desk->getGameType(), m_playtype);
		}

		////ׯ�Ҷ෢һ����
		//Card* newCard = m_deskCard.back();
		//m_handCard[m_curPos].push_back(newCard);
		//m_deskCard.pop_back();
		gCardMgr.SortCard(m_handCard[m_curPos]);

		//������Ϣ���ͻ���
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			if (m_desk->m_user[i] != NULL)
			{
				LMsgS2CPlayStart msg;
				msg.m_zhuang = m_curPos;
				msg.m_pos = i;
				for (int x = 0; x<m_desk->m_desk_user_count; x++)
				{
					msg.m_score.push_back(m_desk->m_vip->m_score[x]);
				}
				for (Lsize j = 0; j < m_handCard[i].size(); ++j)
				{
					msg.m_cardValue[j].m_number = m_handCard[i][j]->m_number;
					msg.m_cardValue[j].m_color = m_handCard[i][j]->m_color;
				}

				for (Lint j = 0; j < m_desk->m_desk_user_count; ++j)
				{
					msg.m_cardCount[j] = m_handCard[j].size();
				}
				msg.m_dCount = (Lint)m_deskCard.size();
				m_desk->m_user[i]->Send(msg);
			}
		}

		//¼����
		m_video.Clear();
		Lint id[4];
		Ldouble score[4];
		memset(id, 0, sizeof(id));
		memset(score, 0, sizeof(score));
		std::vector<CardValue> vec[4];
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			id[i] = m_desk->m_user[i]->GetUserDataId();
			score[i] = m_desk->m_vip->GetUserScore(m_desk->m_user[i]);
			for (Lint j = 0; j < m_handCard[i].size(); ++j)
			{
				CardValue v;
				v.m_color = m_handCard[i][j]->m_color;
				v.m_number = m_handCard[i][j]->m_number;
				vec[i].push_back(v);
			}
		}
		m_video.DealCard(id, vec, gWork.GetCurTime().Secs(), m_zhuangpos, score, m_desk->GetDeskId(), m_desk->m_vip->m_curCircle, m_desk->m_vip->m_maxCircle, m_desk->m_flag, &m_desk->getPlayType());
	}

	void SetDeskPlay()
	{
		if (!m_desk || !m_desk->m_vip)
		{
			return;
		}
		if (m_desk->m_vip)
			m_desk->m_vip->SendInfo();

		m_desk->setDeskState(DESK_PLAY);
		DeakCard();
		CheckStartPlayCard();
	}

	void ProcessRobot(Lint pos, User * pUser)
	{
		if (pos < 0 || pos > 3)
		{
			return;
		}
		switch (m_desk->getDeskPlayState())
		{
		case DESK_PLAY_GET_CARD:
		{
			//�����ȥ
			if (m_desk->getDeskPlayState() == DESK_PLAY_GET_CARD && m_curPos == pos)
			{
				LMsgC2SUserPlay msg;
				msg.m_thinkInfo.m_type = THINK_OPERATOR_OUT;

				CardValue card;
				card.m_color = m_handCard[pos][0]->m_color;
				card.m_number = m_handCard[pos][0]->m_number;
				for (Lint i = 0; i < m_handCard[pos].size(); ++i)
				{
					if ((m_handCard[pos][i]->m_color == 4 && m_handCard[pos][i]->m_number == 4)
						|| (m_handCard[pos][i]->m_color == 3 && m_handCard[pos][i]->m_number == 1)
						|| (m_handCard[pos][i]->m_color == 2 && m_handCard[pos][i]->m_number == 6)
						|| (m_handCard[pos][i]->m_color == 2 && m_handCard[pos][i]->m_number == 9))
					{
						card.m_color = m_handCard[pos][i]->m_color;
						card.m_number = m_handCard[pos][i]->m_number;
						break;
					}
				}

				msg.m_thinkInfo.m_card.push_back(card);
				m_desk->HanderUserPlayCard(pUser, &msg);
			}
		}
		break;
		case DESK_PLAY_THINK_CARD:
		{
			if (m_thinkInfo[pos].NeedThink())
			{
				for (int i = 0; i < m_thinkInfo[pos].m_thinkData.size(); i++)
				{
					if (m_thinkInfo[pos].m_thinkData[i].m_type == THINK_OPERATOR_BOMB)
					{
						LMsgC2SUserOper msg;
						msg.m_think.m_type = THINK_OPERATOR_NULL;		//
						std::vector<Card*>& mCard = m_thinkInfo[pos].m_thinkData[i].m_card;
						for (int j = 0; j < mCard.size(); j++)
						{
							CardValue card;
							card.m_color = mCard[j]->m_color;
							card.m_number = mCard[j]->m_number;
							msg.m_think.m_card.push_back(card);
						}
						m_desk->HanderUserOperCard(pUser, &msg);
						return;
					}
					else
					{
						LMsgC2SUserOper msg;
						msg.m_think.m_type = m_thinkInfo[pos].m_thinkData[i].m_type;
						std::vector<Card*>& mCard = m_thinkInfo[pos].m_thinkData[i].m_card;
						for (int j = 0; j < mCard.size(); j++)
						{
							CardValue card;
							card.m_color = mCard[j]->m_color;
							card.m_number = mCard[j]->m_number;
							msg.m_think.m_card.push_back(card);
						}
						m_desk->HanderUserOperCard(pUser, &msg);
					}
				}
			}
		}
		break;
		default:
		{

		}
		}
	}

	//ͬ��λ��
	void _S_BrocastTingInfo(Lint pos)
	{
		LMsgS2CUserOperTing sendTing;
		sendTing.m_pos = pos;
		if (m_playtype.H_ANTING)
		{
			sendTing.m_type = 1;
		}
		else
		{
			sendTing.m_card.m_color = m_tingCard[pos].GetNCIndex();
		}
		sendTing.m_card.m_number = m_tingCard[pos].GetOutIndex();
		m_desk->BoadCast(sendTing);
	}

	void HanderUserPlayCard(User* pUser, LMsgC2SUserPlay* msg)
	{
		if (m_desk == NULL || pUser == NULL || msg == NULL)
		{
			LLOG_DEBUG("HanderUserEndSelect NULL ERROR ");
			return;
		}
		LMsgS2CUserPlay sendMsg;
		sendMsg.m_errorCode = 0;
		sendMsg.m_pos = m_curPos;
		sendMsg.m_cs_card = msg->m_thinkInfo;

		Lint pos = m_desk->GetUserPos(pUser);
		if (pos == INVAILD_POS)
		{
			//pUser->Send(sendMsg);
			LLOG_DEBUG("HanderUserPlayCard pos error %s", pUser->m_userData.m_nike.c_str());
			return;
		}

		if (pos != m_curPos)
		{
			sendMsg.m_errorCode = 1;
			pUser->Send(sendMsg);
			LLOG_ERROR("HanderUserPlayCard not my pos %d:%d", pos, m_curPos);
			return;
		}

		if (msg->m_thinkInfo.m_type == THINK_OPERATOR_OUT)
		{
			if (m_thinkInfo[pos].NeedThink())
			{
				VideoDoing(99, pos, 0, 0);
			}
			if (msg->m_thinkInfo.m_card.size())
			{
				for (Lsize i = 0; i < m_handCard[pos].size(); ++i)
				{
					if (2 == m_playerIsTing[pos])
					{
						//��������״ֻ̬�ܳ���������
						if (NULL == m_curGetCard)
						{
							//���ƴ���
							LLOG_ERROR("ERROR HanderUserPlayCard pos %s is ting = 2 but out card c = %d n = %d", pUser->m_userData.m_nike.c_str(), msg->m_thinkInfo.m_card[0].m_color, msg->m_thinkInfo.m_card[0].m_number);
							return;
						}
						if (m_curGetCard->GetNCIndex() != msg->m_thinkInfo.m_card[0].GetNCIndex())
						{
							LLOG_DEBUG("XiaoYiHandler HanderUserPlayCard THINK_OPERATOR_OUT outcard error!");
							msg->m_thinkInfo.m_card[0].m_color = m_curGetCard->m_color;
							msg->m_thinkInfo.m_card[0].m_number = m_curGetCard->m_number;
						}
					}

					if (m_handCard[pos][i]->m_color == msg->m_thinkInfo.m_card[0].m_color && m_handCard[pos][i]->m_number == msg->m_thinkInfo.m_card[0].m_number)
					{
						if (m_beforeType == THINK_OPERATOR_TING && 1 == m_playerIsTing[pos])
						{
							Lint tmp_ncindex = msg->m_thinkInfo.m_card[0].GetNCIndex();
							if (m_setNowTingCard[pos].find(tmp_ncindex) == m_setNowTingCard[pos].end())
							{
								LLOG_DEBUG("XiaoYiHandler  THINK_OPERATOR_TING outcard error! %d", tmp_ncindex);
								return;
							}
						}

						Lint tmpbeforetype = m_beforeType;
						{
							//�����߼�
							m_curOutCard = m_handCard[pos][i];
							gCardMgr.EraseCard(m_handCard[pos], m_curOutCard);
							m_beforePos = pos;
							m_beforeType = THINK_OPERATOR_OUT;

							//¼��
							std::vector<CardValue> cards;
							CardValue card;
							card.m_color = m_curOutCard->m_color;
							card.m_number = m_curOutCard->m_number;
							cards.push_back(card);
							m_video.AddOper(VIDEO_OPER_OUT_CARD, pos, cards);

							//�޸�һ���߼� �ڳ���ʱ�������Ѿ�������ҵ��ѳ��б���
							m_outCard[pos].push_back(m_curOutCard);
							m_first_turn = false;
						}

						m_isTingHu[pos] = false;
						if (tmpbeforetype == THINK_OPERATOR_TING && 1 == m_playerIsTing[pos])
						{
							Lint tmp_ncindex = msg->m_thinkInfo.m_card[0].GetNCIndex();

							//���ƽ׶�Ϊ2
							m_playerIsTing[pos] = 2;
							m_isAutoCard[pos] = true;
							m_isTingHu[pos] = true;

							m_thinkRet[pos].Clear();
							m_thinkInfo[pos].Reset();
							m_tingCard[pos].setLiangPai(tmp_ncindex, m_outCard[pos].size());

							if (!m_playtype.H_ANTING)
							{
								m_desk->BoadCast(sendMsg);
							}

							//�㲥������������״̬
							LMsgS2CUserPlay sendMsg2;
							sendMsg2.m_errorCode = 0;
							sendMsg2.m_pos = pos;
							sendMsg2.m_cs_card.m_type = THINK_OPERATOR_TING;

							//�����㲥����
							if (!m_playtype.H_ANTING)
							{
								sendMsg2.m_cs_card.m_card.push_back(msg->m_thinkInfo.m_card[0]);
							}
							m_desk->BoadCast(sendMsg2);

							//¼��;                         ���Ʒ��������У���ɫλ�÷��Ƶ�NCֵ����ֵλ�÷�����λ�ã����ơ����ƻ���ƣ�
							VideoDoing(THINK_OPERATOR_TING, pos, msg->m_thinkInfo.m_card[0].m_color, msg->m_thinkInfo.m_card[0].m_number);
							if (!m_playtype.H_ANTING)
							{
								SetThinkIng();
							}
							else
							{
								ThinkEnd();
							}
							break;
						}

						//�������˼��
						m_desk->BoadCast(sendMsg);
						SetThinkIng();
						break;
					}
				}
			}
			return;
		}

		ThinkUnit* unit = NULL;
		for (Lint i = 0; i < m_thinkInfo[pos].m_thinkData.size(); ++i)
		{
			if (msg->m_thinkInfo.m_type == m_thinkInfo[pos].m_thinkData[i].m_type)
			{
				if (msg->m_thinkInfo.m_card.size() == m_thinkInfo[pos].m_thinkData[i].m_card.size())
				{
					bool find = true;
					for (Lsize j = 0; j < msg->m_thinkInfo.m_card.size(); ++j)
					{
						if (msg->m_thinkInfo.m_card[j].m_color != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_color ||
							msg->m_thinkInfo.m_card[j].m_number != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_number)
						{
							find = false;
							break;
						}
					}

					if (find)
					{
						unit = &m_thinkInfo[pos].m_thinkData[i];
						break;
					}
				}
			}
		}

		if (unit)
		{
			if (m_first_turn && (unit->m_type != THINK_OPERATOR_AGANG&&unit->m_type != THINK_OPERATOR_ABU))
			{
				m_first_turn = false;
			}
			if (unit->m_type == THINK_OPERATOR_BOMB)
			{
				//¼��;
				VideoDoing(unit->m_type, pos, 0, 0);

				if (m_curGetCard)
				{
					gCardMgr.EraseCard(m_handCard[pos], m_curGetCard);
					sendMsg.m_huCard.m_color = m_curGetCard->m_color;
					sendMsg.m_huCard.m_number = m_curGetCard->m_number;
				}
				sendMsg.m_hu = unit->m_hu;
				sendMsg.m_huCard.m_color = m_curGetCard->m_color;
				sendMsg.m_cardCount = m_handCard[pos].size();
				for (Lint i = 0; i < sendMsg.m_cardCount; i++)
				{
					CardValue mCard;
					mCard.m_color = m_handCard[pos][i]->m_color;
					mCard.m_number = m_handCard[pos][i]->m_number;
					sendMsg.m_cardValue.push_back(mCard);
				}
				m_desk->BoadCast(sendMsg);

				m_thinkRet[m_curPos] = m_thinkInfo[m_curPos].m_thinkData[0];
				m_playerHuInfo[m_curPos] = WIN_SUB_ZIMO;
				CardVector winCards[DESK_USER_COUNT];
				winCards[m_curPos] = unit->m_card;
				OnGameOver(WIN_ZIMO, m_playerHuInfo, INVAILD_POS, winCards);
			}
			else if (unit->m_type == THINK_OPERATOR_AGANG)
			{
				if (m_playerIsTing[pos] == 2)
				{
					if (unit->m_card[0]->GetNCIndex() == m_tingCard[pos].m_ncCard)
					{
						m_tingCard[pos].setType(2);
						m_tingCard[pos].resetOutIndex();
						_S_BrocastTingInfo(pos);
					}
				}
				//¼��;
				VideoDoing(unit->m_type, pos, unit->m_card[0]->m_color, unit->m_card[0]->m_number);
				//
				gCardMgr.EraseCard(m_handCard[pos], unit->m_card[0], 4);
				m_desk->BoadCast(sendMsg);
				m_angang[pos] += 1;

				//¼��
				std::vector<CardValue> cards;
				for (int i = 0; i < 4; i++)
				{
					CardValue card;
					card.m_color = unit->m_card[0]->m_color;
					card.m_number = unit->m_card[0]->m_number;
					cards.push_back(card);
					m_angangCard[pos].push_back(unit->m_card[0]);
				}
				m_curOutCard = unit->m_card[0];
				m_video.AddOper(VIDEO_OPER_AN_GANG, pos, cards);
				//�������˼��
				m_beforePos = pos;
				m_beforeType = THINK_OPERATOR_AGANG;
				if (m_first_turn)
				{
					SetPlayIng(pos, true, true, true, true, false, true);
					m_first_turn = false;
				}
				else
					SetPlayIng(pos, true, true, true, true);
			}
			else if (unit->m_type == THINK_OPERATOR_MGANG)
			{
				//����ܵ����ƣ���ô���Ƶ�����Ӧ�ø�Ϊ��
				if (m_playerIsTing[pos] == 2)
				{
					if (unit->m_card[0]->GetNCIndex() == m_tingCard[pos].m_ncCard)
					{
						m_tingCard[pos].setType(2);
						m_tingCard[pos].resetOutIndex();
						_S_BrocastTingInfo(pos);
					}
				}
				//¼��;
				VideoDoing(unit->m_type, pos, unit->m_card[0]->m_color, unit->m_card[0]->m_number);
				//m_desk->BoadCast(sendMsg);
				m_curOutCard = unit->m_card[0];
				gCardMgr.EraseCard(m_handCard[pos], unit->m_card[0], 1);

				//�������˼��
				m_beforePos = pos;
				m_beforeType = THINK_OPERATOR_MGANG;
				SetThinkIng(true);
			}
			else if (unit->m_type == THINK_OPERATOR_ABU)
			{
				//����ܵ����ƣ���ô���Ƶ�����Ӧ�ø�Ϊ��
				if (m_playerIsTing[pos] == 2)
				{
					if (unit->m_card[0]->GetNCIndex() == m_tingCard[pos].m_ncCard)
					{
						m_tingCard[pos].setType(2);
						m_tingCard[pos].resetOutIndex();
						_S_BrocastTingInfo(pos);
					}
				}
				//¼��;
				VideoDoing(unit->m_type, pos, unit->m_card[0]->m_color, unit->m_card[0]->m_number);
				//
				gCardMgr.EraseCard(m_handCard[pos], unit->m_card[0], 4);
				m_desk->BoadCast(sendMsg);
				m_angang[pos] += 1;
				//¼��
				std::vector<CardValue> cards;
				for (int i = 0; i < 4; i++)
				{
					CardValue card;
					card.m_color = unit->m_card[0]->m_color;
					card.m_number = unit->m_card[0]->m_number;
					cards.push_back(card);
					m_angangCard[pos].push_back(unit->m_card[0]);
				}
				m_video.AddOper(VIDEO_OPER_AN_BU, pos, cards);
				//�������˼��
				m_beforePos = pos;
				m_beforeType = THINK_OPERATOR_ABU;
				if (m_first_turn)
				{
					SetPlayIng(pos, true, true, true, true, false, true);
					m_first_turn = false;
				}
				else
					SetPlayIng(pos, true, true, true, true);
			}
			else if (unit->m_type == THINK_OPERATOR_MBU)
			{
				//����ܵ����ƣ���ô���Ƶ�����Ӧ�ø�Ϊ��
				if (m_playerIsTing[pos] == 2)
				{
					if (unit->m_card[0]->GetNCIndex() == m_tingCard[pos].m_ncCard)
					{
						m_tingCard[pos].setType(2);
						m_tingCard[pos].resetOutIndex();
						_S_BrocastTingInfo(pos);
					}
				}
				//¼��;
				VideoDoing(unit->m_type, pos, unit->m_card[0]->m_color, unit->m_card[0]->m_number);
				//
				gCardMgr.EraseCard(m_handCard[pos], unit->m_card[0], 4);
				m_curOutCard = unit->m_card[0];
				//�������˼��
				m_beforePos = pos;
				m_beforeType = THINK_OPERATOR_MBU;
				SetThinkIng(true);
			}//����
			else if (unit->m_type == THINK_OPERATOR_TING)
			{
				//¼��;
				VideoDoing(unit->m_type, pos, 0, 0);

				//�������Ϊ����״̬
				m_playerIsTing[pos] = 1;
				/*m_thinkRet[pos].Clear();
				m_thinkInfo[pos].Reset();*/
				m_beforeType = THINK_OPERATOR_TING;

				LMsgS2CCanTing send;
				std::map<Lint, LastCardSet>::iterator IT = m_setNowTingCard[pos].begin();
				while (IT != m_setNowTingCard[pos].end())
				{
					send.m_tingCards.push_back(IT->first);
					++IT;
				}
				pUser->Send(send);
			}
			else
			{
				LLOG_DEBUG("HandlerObject::HanderUserPlayCard think type %d", unit->m_type);
			}
		}
		else
		{  //ȡ�����ƻ��ߵ�����
			if (m_playerIsTing[pos] == 1)
			{
				m_playerIsTing[pos] = 0;
			}
			LLOG_DEBUG("Desk::HanderUserPlayCard %s,%d", pUser->m_userData.m_nike.c_str(), msg->m_thinkInfo.m_type);
		}
	}

	void HanderUserOperCard(User* pUser, LMsgC2SUserOper* msg)
	{
		LMsgS2CUserOper sendMsg;
		sendMsg.m_pos = m_curPos;
		sendMsg.m_think = msg->m_think;

		Lint pos = m_desk->GetUserPos(pUser);
		if (pos == INVAILD_POS || !m_thinkInfo[pos].NeedThink())
		{
			sendMsg.m_errorCode = 1;
			pUser->Send(sendMsg);
			return;
		}

		bool find = false;
		for (Lsize i = 0; i < m_thinkInfo[pos].m_thinkData.size(); ++i)
		{
			if (m_thinkInfo[pos].m_thinkData[i].m_type == msg->m_think.m_type)
			{
				bool check = true;
				if (m_thinkInfo[pos].m_thinkData[i].m_type == THINK_OPERATOR_CHI)
				{
					for (Lsize j = 0; j < msg->m_think.m_card.size() && j<2; ++j)
					{
						if (msg->m_think.m_card[j].m_color != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_color ||
							msg->m_think.m_card[j].m_number != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_number)
						{
							check = false;
							break;
						}
					}

					if (check)
					{
						m_thinkRet[pos] = m_thinkInfo[pos].m_thinkData[i];
						find = true;
						break;
					}
				}
				else if (m_thinkInfo[pos].m_thinkData[i].m_card.size() == msg->m_think.m_card.size())
				{
					for (Lsize j = 0; j < msg->m_think.m_card.size(); ++j)
					{
						if (msg->m_think.m_card[j].m_color != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_color ||
							msg->m_think.m_card[j].m_number != m_thinkInfo[pos].m_thinkData[i].m_card[j]->m_number)
						{
							check = false;
							break;
						}
					}

					if (check)
					{
						m_thinkRet[pos] = m_thinkInfo[pos].m_thinkData[i];
						find = true;
						break;
					}
				}
			}
		}
		
		if (!find)
		{
			m_thinkRet[pos].m_type = THINK_OPERATOR_NULL;
		}

		//¼��;
		VideoDoing(msg->m_think.m_type, pos, (msg->m_think.m_card.size()>0) ? msg->m_think.m_card[0].m_color : 0, (msg->m_think.m_card.size()>0) ? msg->m_think.m_card[0].m_number : 0);

		if (msg->m_think.m_type == THINK_OPERATOR_BOMB)
		{
			LMsgS2CUserOper send;
			send.m_pos = pos;
			send.m_errorCode = 0;
			send.m_think.m_type = THINK_OPERATOR_BOMB;
			for (int x = 0; x<m_thinkRet[pos].m_card.size(); x++)
			{
				if (m_thinkRet[pos].m_card[x])
				{
					CardValue card;
					card.m_color = m_thinkRet[pos].m_card[x]->m_color;
					card.m_number = m_thinkRet[pos].m_card[x]->m_number;
					send.m_think.m_card.push_back(card);
				}
			}
			if (!m_thinkRet[pos].m_card.empty())
			{
				for (int x = 0; x<m_thinkRet[pos].m_card.size(); x++)
				{
					if (x == 0 && m_thinkRet[pos].m_card[x])
					{
						send.m_card.m_color = m_thinkRet[pos].m_card[x]->m_color;
						send.m_card.m_number = m_thinkRet[pos].m_card[x]->m_number;
					}
					else if (x == 1 && m_thinkRet[pos].m_card[x])
					{
						send.m_hucard2.m_color = m_thinkRet[pos].m_card[x]->m_color;
						send.m_hucard2.m_number = m_thinkRet[pos].m_card[x]->m_number;
					}
				}
			}

			//��������
			send.m_hu = m_thinkRet[pos].m_hu;
			send.m_cardCount = m_handCard[pos].size();
			for (Lint i = 0; i < send.m_cardCount; i++)
			{
				CardValue mCard;
				if (m_handCard[pos][i])
				{
					mCard.m_color = m_handCard[pos][i]->m_color;
					mCard.m_number = m_handCard[pos][i]->m_number;
					send.m_cardValue.push_back(mCard);
				}
			}
			if (m_desk)
				m_desk->BoadCast(send);
		}
		else
		{
			m_isTingHu[pos] = false;
		}
		
		//�����Լ�˼������
		m_thinkInfo[pos].Reset();

		if (!continueThink())
		{
			CheckThink();
		}
	}


	//�Ƿ���Ҫ����˼��
	bool continueThink()
	{
		bool needThink = false;

		bool hasBomb = false;	//�Ѿ����ڵ��ں�����

								//�ɾ���������������ҿ�ʼ���ң�������������¼ҿ��Ժ���
		for (Lint index = 1; index < DESK_USER_COUNT; ++index)
		{
			Lint correctPos = (m_curPos + index) % DESK_USER_COUNT;

			if (hasBomb)	//�Ѿ����ڵ��ں�����
			{
				if (m_thinkRet[correctPos].m_type == THINK_OPERATOR_BOMB)
				{
					m_thinkRet[correctPos].Clear();	//�ֶ���Ϊ�գ����ɺ��ƣ�
				}
			}
			else	//��û���ҵ�������
			{
				if (m_thinkRet[correctPos].m_type == THINK_OPERATOR_BOMB)	//�Ѿ�����
				{
					hasBomb = true;
				}
				else if (m_thinkInfo[correctPos].NeedThink())	//��Ҫ˼��
				{
					if (m_thinkInfo[correctPos].HasHu())
					{
						needThink = true;
						break;
					}
				}
			}
		}

		//���ھ�������ĺ����ߣ�����Ҫ��˼��
		if (hasBomb)
		{
			needThink = false;
		}

		return needThink;
	}

	void OnUserReconnect(User* pUser)
	{
		if (pUser == NULL || m_desk == NULL)
		{
			return;
		}
		LLOG_DEBUG("Desk::OnUserReconnect pos1 %d", pUser->GetUserDataId());
		//���͵�ǰȦ����Ϣ
		if (m_desk->m_vip)
			m_desk->m_vip->SendInfo();
		Lint pos = m_desk->GetUserPos(pUser);
		if (pos == INVAILD_POS)
		{
			LLOG_ERROR("Desk::OnUserReconnect pos error %d", pUser->GetUserDataId());
			return;
		}
		Lint nCurPos = m_curPos;
		Lint nDeskPlayType = m_desk->getDeskPlayState();
		//	VideoDoing(VIDEO_PLAYER_RECONNECT,pos,0,0);
		LMsgS2CDeskState reconn;
		reconn.m_state = m_desk->getDeskState();
		reconn.m_pos = nCurPos;
		reconn.m_time = 15;
		reconn.m_zhuang = m_zhuangpos;
		reconn.m_myPos = pos;
		reconn.m_user_count = m_desk->m_desk_user_count;
		reconn.m_isBaoTing = (m_playerIsTing[pos] == 2);
		if (nCurPos != pos)
		{
			reconn.m_flag = 0;
		}
		else
		{
			reconn.m_flag = 1;			//��֪���Բ���
		}
		reconn.m_dCount = m_deskCard.size();
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			reconn.m_cardCount[i] = m_handCard[i].size();
			reconn.m_oCount[i] = m_outCard[i].size();
			reconn.m_aCount[i] = m_angangCard[i].size();
			reconn.m_mCount[i] = m_minggangCard[i].size();
			reconn.m_pCount[i] = m_pengCard[i].size();
			reconn.m_eCount[i] = m_eatCard[i].size();
			reconn.m_score[i] = m_desk->m_vip->m_score[i];

			for (Lsize j = 0; j < m_outCard[i].size(); ++j)
			{
				if (m_playtype.H_ANTING && pos != i)
				{
					Lint ncindex = m_tingCard[i].GetNCIndex();
					if (m_playerIsTing[i] == 2 && ncindex && j == (ncindex - 1))
					{
						reconn.m_oCard[i][j].m_color = 0;
						reconn.m_oCard[i][j].m_number = 0;
					}
					else
					{
						reconn.m_oCard[i][j].m_color = m_outCard[i][j]->m_color;
						reconn.m_oCard[i][j].m_number = m_outCard[i][j]->m_number;
					}
				}
				else
				{
					reconn.m_oCard[i][j].m_color = m_outCard[i][j]->m_color;
					reconn.m_oCard[i][j].m_number = m_outCard[i][j]->m_number;
				}
			}

			for (Lsize j = 0; j < m_angangCard[i].size(); ++j)
			{
				reconn.m_aCard[i][j].m_color = m_angangCard[i][j]->m_color;
				reconn.m_aCard[i][j].m_number = m_angangCard[i][j]->m_number;
			}

			for (Lsize j = 0; j < m_minggangCard[i].size(); ++j)
			{
				reconn.m_mCard[i][j].m_color = m_minggangCard[i][j]->m_color;
				reconn.m_mCard[i][j].m_number = m_minggangCard[i][j]->m_number;
			}

			for (Lsize j = 0; j < m_pengCard[i].size(); ++j)
			{
				reconn.m_pCard[i][j].m_color = m_pengCard[i][j]->m_color;
				reconn.m_pCard[i][j].m_number = m_pengCard[i][j]->m_number;
			}

			for (Lsize j = 0; j < m_eatCard[i].size(); ++j)
			{
				reconn.m_eCard[i][j].m_color = m_eatCard[i][j]->m_color;
				reconn.m_eCard[i][j].m_number = m_eatCard[i][j]->m_number;
			}

		}

		//�ҵ���,�ͻ���������֮ǰĪ���Ƶ������ó���������
		if (nDeskPlayType == DESK_PLAY_GET_CARD && m_needGetCard && pos == nCurPos)
		{
			CardVector tmp = m_handCard[pos];
			if (m_curGetCard)
			{
				reconn.m_cardCount[pos] -= 1;
				gCardMgr.EraseCard(tmp, m_curGetCard);
			}
			for (Lsize j = 0; j < tmp.size(); ++j)
			{
				reconn.m_cardValue[j].m_color = tmp[j]->m_color;
				reconn.m_cardValue[j].m_number = tmp[j]->m_number;
			}
		}
		else
		{
			for (Lsize j = 0; j < m_handCard[pos].size(); ++j)
			{
				reconn.m_cardValue[j].m_color = m_handCard[pos][j]->m_color;
				reconn.m_cardValue[j].m_number = m_handCard[pos][j]->m_number;
			}
		}

		//�ó��Ƶ���ң��෢һ���ƣ����ڴ��ȥ��
		if (m_needGetCard && nDeskPlayType == DESK_PLAY_THINK_CARD)
		{
			if (m_curOutCard && pos != nCurPos)
			{
				reconn.m_cardCount[nCurPos] ++;
			}
			else if (m_curOutCard&&pos == m_beforePos&& m_beforeType == THINK_OPERATOR_OUT)
			{
				reconn.m_cardCount[pos]++;
				//reconn.m_oCount[pos]--;
				CardValue xx;
				xx.m_color = m_curOutCard->m_color;
				xx.m_number = m_curOutCard->m_number;
				reconn.m_cardValue[reconn.m_cardCount[pos] - 1] = xx;
			}
		}

		//������Ϣ
		if (m_playtype.H_ANTING)
		{
			reconn.m_tingType = 1;
		}

		LLOG_DEBUG("Desk::OnUserReconnect pos2 %d", pUser->GetUserDataId());
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			if (m_playerIsTing[i] == 2)
			{
				reconn.m_tingPos[i] = i;
				if (i == pos || !m_playtype.H_ANTING)
				{
					reconn.m_tingCard[i].m_color = m_tingCard[i].GetNCIndex();
				}
				reconn.m_tingCard[i].m_number = m_tingCard[i].GetOutIndex();
			}
		}

		pUser->Send(reconn);
		LLOG_DEBUG("Desk::OnUserReconnect pos3 %d", pUser->GetUserDataId());

		//��˼��
		if (nDeskPlayType == DESK_PLAY_THINK_CARD)
		{
			if (m_thinkInfo[pos].NeedThink())
			{
				LMsgS2CThink think;
				think.m_time = 15;
				think.m_flag = 1;
				m_desk->UpdateLastOptTimeAndPos(pUser);
				think.m_card.m_color = (m_curOutCard == NULL) ? 1 : m_curOutCard->m_color;		//��ʱ�� �д��� m_curOutCard->m_color;
				think.m_card.m_number = (m_curOutCard == NULL) ? 1 : m_curOutCard->m_number;		//��ʱ�� �д���m_curOutCard->m_number;
				for (Lsize j = 0; j < m_thinkInfo[pos].m_thinkData.size(); ++j)
				{
					ThinkData info;
					info.m_type = m_thinkInfo[pos].m_thinkData[j].m_type;

					for (Lsize n = 0; n < m_thinkInfo[pos].m_thinkData[j].m_card.size(); ++n)
					{
						CardValue v;
						v.m_color = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_color;
						v.m_number = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_number;
						info.m_card.push_back(v);
					}
					think.m_think.push_back(info);
				}
				pUser->Send(think);
			}
		}
		LLOG_DEBUG("Desk::OnUserReconnect pos4 %d", pUser->GetUserDataId());
		//�ҳ���
		if (nDeskPlayType == DESK_PLAY_GET_CARD && m_needGetCard && pos == nCurPos)
		{
			LMsgS2COutCard msg;
			msg.m_time = 15;
			msg.m_pos = pos;
			m_desk->UpdateLastOptTimeAndPos(pos);
			msg.m_deskCard = (Lint)m_deskCard.size();
			msg.m_flag = (m_curGetCard&&m_needGetCard) ? 0 : 1;
			msg.m_gang = m_gangPos[pos];
			msg.m_end = m_deskCard.size() == 1 ? 1 : 0;
			if (m_needGetCard && m_curGetCard)
			{
				msg.m_curCard.m_color = m_curGetCard->m_color;
				msg.m_curCard.m_number = m_curGetCard->m_number;
			}

			if (m_playerIsTing[pos] != 1)
			{
				for (Lsize j = 0; j < m_thinkInfo[pos].m_thinkData.size(); ++j)
				{
					ThinkData info;
					info.m_type = m_thinkInfo[pos].m_thinkData[j].m_type;
					for (Lsize n = 0; n < m_thinkInfo[pos].m_thinkData[j].m_card.size(); ++n)
					{
						CardValue v;
						v.m_color = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_color;
						v.m_number = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_number;
						info.m_card.push_back(v);
					}
					msg.m_think.push_back(info);
				}
			}
			
			pUser->Send(msg);
		}
		LLOG_DEBUG("Desk::OnUserReconnect pos5 %d", pUser->GetUserDataId());
		//�ҳ���
		if (m_playerIsTing[pos] == 1)
		{
			LMsgS2CCanTing send;
			std::map<Lint, LastCardSet>::iterator IT = m_setNowTingCard[pos].begin();
			while (IT != m_setNowTingCard[pos].end())
			{
				send.m_tingCards.push_back(IT->first);
				++IT;
			}
			pUser->Send(send);
		} 
		LLOG_DEBUG("Desk::OnUserReconnect pos6 %d", pUser->GetUserDataId());
		//�����ϵ������·�����ҵ�����
		if (m_needGetCard && nDeskPlayType == DESK_PLAY_THINK_CARD && !m_isThinkingQiangGang)
		{
			if (m_curOutCard)
			{
				LMsgS2CUserPlay sendMsg;
				sendMsg.m_errorCode = 0;
				sendMsg.m_pos = nCurPos;
				sendMsg.m_cs_card.m_type = THINK_OPERATOR_OUT;
				CardValue card;
				card.m_color = m_curOutCard->m_color;
				card.m_number = m_curOutCard->m_number;
				sendMsg.m_cs_card.m_card.push_back(card);
				pUser->Send(sendMsg);
			}
		}
		LLOG_DEBUG("Desk::OnUserReconnect pos8== %d", pUser->GetUserDataId());
	}

	void CheckStartPlayCard()
	{
		if (!m_desk)
		{
			return;
		}
		//����ץ���ˣ�ֱ��˼��		//��BUG ˼��ʱ���������
		m_desk->setDeskPlayState(DESK_PLAY_GET_CARD);
		//SetPlayIng(m_curPos, false, false, true, true, false, true);
		//���ƾͼ���Ƿ������
		SetPlayIng(m_curPos, true, true, true, true, false, true);
		//m_curGetCard = m_handCard[m_curPos].back();
		m_needGetCard = true;
	}

	//����
	void SetPlayIng(Lint pos, bool needGetCard, bool gang, bool needThink, bool canhu, bool bu = false, bool first_think = false)
	{
		if (m_desk == NULL)
		{
			LLOG_DEBUG("HanderUserEndSelect NULL ERROR ");
			return;
		}
		//��ׯ
		if (!gang && m_deskCard.size() <= 16 && needGetCard)
		{
			LLOG_DEBUG("Desk::SetPlayIng huangzhuang game over");
			OnGameOver(WIN_NONE, m_playerHuInfo, INVAILD_POS, NULL);
			return;
		}
		m_curPos = pos;
		m_desk->UpdateLastOptTimeAndPos(m_curPos);

		//������˼����Ϣ
		m_thinkInfo[pos].m_thinkData.clear();
		if (m_desk->getDeskPlayState() != DESK_PLAY_END_CARD)
		{
			m_desk->setDeskPlayState(DESK_PLAY_GET_CARD);
		}
		m_needGetCard = false;
		if (needGetCard)
		{
			m_needGetCard = true;
			m_curGetCard = m_deskCard.back();
			m_deskCard.pop_back();

			//¼��
			std::vector<CardValue> cards;
			CardValue card;
			card.m_color = m_curGetCard->m_color;
			card.m_number = m_curGetCard->m_number;
			cards.push_back(card);
			m_video.AddOper(VIDEO_OPER_GET_CARD, pos, cards);
		}

		if (needThink) //|| m_desk->getDeskPlayState() == DESK_PLAY_END_CARD
		{
			if (m_playerIsTing[m_curPos] != 2)
			{
				canhu = false;
			}
				
			mGameInfo.m_GameType = m_desk->getGameType();	// 0 ���ϣ� 3�� ��ɳ
			mGameInfo.b_canEat = false;		// �Ƿ���Գ�
			mGameInfo.b_canHu = canhu;		// �Ƿ���Ժ�
			mGameInfo.b_onlyHu = (m_desk->getDeskPlayState() == DESK_PLAY_END_CARD || (m_playerIsTing[m_curPos] == 2));
			mGameInfo.m_thinkGang = gang;	// ���������ǲ��Ǹܵ���
			mGameInfo.m_deskState = m_desk->getDeskPlayState();	// ��ǰ����״̬
			mGameInfo.m_playerPos = m_curPos;	// ��ǰһ������λ��
			mGameInfo.m_cardState = THINK_OPERATOR_NULL;	// ��ǰһ������״̬
			mGameInfo.m_MePos = pos;		// ��ҵ�λ��
			mGameInfo.m_QiXiaoDui = false;
			mGameInfo.m_pt_laizi = false;
			mGameInfo.m_tianhu = first_think;
			mGameInfo.m_dihu = false;
			mGameInfo.b_menqing = m_playtype.H_MENQING;
			mGameInfo.b_menqingjiangjianghu = m_playtype.H_MENQINGPENGPENGHU;

			if (canhu && m_curGetCard)
			{
				if (m_curGetCard->m_color <= 3)
				{
					mGameInfo.b_canHu = (m_curGetCard->m_number >= 4);
				}
			}
			m_thinkInfo[pos].m_thinkData = gCardMgr.CheckGetCardOperator(m_handCard[pos], m_pengCard[pos], m_angangCard[pos], m_minggangCard[pos], m_eatCard[pos], m_curGetCard, mGameInfo);
			VideoThink(pos);
		}

		if (m_needGetCard)
		{
			m_handCard[pos].push_back(m_curGetCard);
			gCardMgr.SortCard(m_handCard[pos]);
		}

		if (!m_playerIsTing[m_curPos])
		{
			CheckTing(pos, m_setNowTingCard[pos]);
		}

		for (Lint i = 0; i < m_desk->m_desk_user_count;  ++i)
		{
			if (m_desk->m_user[i] != NULL)
			{
				LMsgS2COutCard msg;
				msg.m_time = 15;
				msg.m_pos = pos;
				m_desk->UpdateLastOptTimeAndPos(pos);
				msg.m_deskCard = (Lint)m_deskCard.size();
				msg.m_gang = m_gangPos[pos];
				msg.m_end = m_desk->getDeskPlayState() == DESK_PLAY_END_CARD ? 1 : 0;
				msg.m_flag = 1;
				if (m_needGetCard)
				{
					msg.m_flag = 0;
				}

				if (pos == i)
				{
					if (m_needGetCard)
					{
						msg.m_curCard.m_number = m_curGetCard->m_number;
						msg.m_curCard.m_color = m_curGetCard->m_color;
					}

					for (Lsize j = 0; j < m_thinkInfo[pos].m_thinkData.size(); ++j)
					{
						ThinkData info;
						info.m_type = m_thinkInfo[pos].m_thinkData[j].m_type;
						if (first_think&&info.m_type == THINK_OPERATOR_BOMB && !(m_handCard[pos].empty()))
						{
							if (m_handCard[pos].back())
							{
								m_thinkInfo[pos].m_thinkData[j].m_card.push_back(m_handCard[pos].back());
								msg.m_curCard.m_number = m_handCard[pos].back()->m_number;
								msg.m_curCard.m_color = m_handCard[pos].back()->m_color;
							}
						}
						for (Lsize n = 0; n < m_thinkInfo[pos].m_thinkData[j].m_card.size(); ++n)
						{
							CardValue v;
							v.m_color = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_color;
							v.m_number = m_thinkInfo[pos].m_thinkData[j].m_card[n]->m_number;
							info.m_card.push_back(v);
						}
						msg.m_think.push_back(info);
					}
				}

				m_desk->m_user[i]->Send(msg);
			}
		}
	}

	bool calcScore(Lint result, Lint winpos[], Lint bombpos, Lint gold[], Lint& bombCount, std::vector<Lint>& winPos, LMsgS2CGameOver& over)
	{
		if (!m_desk)
		{
			return false;
		}
		if (result == WIN_BOMB && (bombpos<0 || bombpos>m_desk->m_desk_user_count - 1))
		{
			return false;
		}
		Lint baseScore[4] = { 0 };
		Lint tmppos = 0;
		if (m_curPos != INVAILD_POS)
		{
			tmppos = m_curPos;
		}

		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			if (result == WIN_ZIMO)
			{
				if (winpos[tmppos] == WIN_SUB_ZIMO)
				{
					winPos.push_back(tmppos);
					break;
				}
			}
			else if (result == WIN_BOMB)
			{
				if (winpos[tmppos] == WIN_SUB_BOMB)
				{
					winPos.push_back(tmppos);
					break;
				}
			}
			tmppos = m_desk->GetNextPos(tmppos);
		}

		if (winPos.size() == 0)
		{
			LLOG_ERROR("calcScore winPos.size() = %d", winPos.size());
			return false;
		}

		Lint finalWinPos = winPos.front();
		for (Lint i = 0; i < m_desk->GetUserCount(); ++i)
		{
			if (i != finalWinPos)
			{
				winpos[i] = 0;
				m_thinkRet[i].m_hu.clear();
			}
		}

		if (result == WIN_ZIMO)
		{
			if (!m_curGetCard)
			{
				LLOG_ERROR("calcScore m_curGetCard null");
				return false;
			}
			if (m_curGetCard)
			{
				LLOG_DEBUG("calcScore2 m_curGetCard m_color  %d m_number %d", m_curGetCard->m_color, m_curGetCard->m_number);
				for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
				{
					if (i != finalWinPos)
					{
						if (m_curGetCard->m_color > 3)
						{
							gold[finalWinPos] += 10 * 2;
							gold[i] += -(10 * 2);
						}
						else
						{
							gold[finalWinPos] += m_curGetCard->m_number * 2;
							gold[i] += -(m_curGetCard->m_number * 2);
						}
					}
				}
			}

			//�����
			for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
			{
				gold[i] += (m_minggangCard[i].size()/4 * 5) * 3;
				gold[i] += (m_angang[i] * 10) * 3;
				LLOG_DEBUG("calcScore m_diangang %d m_angang %d m_minggang %d", m_diangang[i], m_angang[i], m_minggang[i]);
				for (Lint j = 0; j < m_desk->m_desk_user_count; ++j)
				{
					if (i != j)
					{
						gold[j] -= (m_minggangCard[i].size()/4 * 5);
						gold[j] -= (m_angang[i] * 10);
					}
				}
			}
		}

		if(result == WIN_BOMB)
		{
			if (!m_curOutCard)
			{
				LLOG_ERROR("calcScore m_curOutCard null");
				return false;
			}
			if (m_isAutoCard[bombpos] && !m_isTingHu[bombpos]) //����Ѿ��Զ����ƣ�������һ����������ǵ��ڶ��ٷ־Ͷ��ٷ�
			{
				LLOG_DEBUG("calcScore2 m_curOutCard m_color  %d m_number %d", m_curOutCard->m_color, m_curOutCard->m_number);
				for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
				{
					if (i != finalWinPos)
					{
						if (m_curOutCard->m_color > 3)
						{
							gold[finalWinPos] += 10;
							gold[i] += -(10);
						}
						else
						{
							gold[finalWinPos] += m_curOutCard->m_number;
							gold[i] += -(m_curOutCard->m_number);
						}
					}
				}

				//�����
				for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
				{
					gold[i] += (m_minggangCard[i].size() / 4 * 5) * 3;
					gold[i] += (m_angang[i] * 10) * 3;
					LLOG_DEBUG("calcScore m_diangang %d m_angang %d m_minggang %d", m_diangang[i], m_angang[i], m_minggang[i]);
					for (Lint j = 0; j < m_desk->m_desk_user_count; ++j)
					{
						if (i != j)
						{
							gold[j] -= (m_minggangCard[i].size() / 4 * 5);
							gold[j] -= (m_angang[i] * 10);
						}
					}
				}
			}
			else
			{
				{
					LLOG_DEBUG("calcScore2 m_curOutCard m_color  %d m_number %d", m_curOutCard->m_color, m_curOutCard->m_number);
					if (m_curOutCard->m_color > 3)
					{
						gold[finalWinPos] += 10 * 4;
						gold[bombpos] += -(10 * 4);
					}
					else
					{
						gold[finalWinPos] += m_curOutCard->m_number * 4;
						gold[bombpos] += -(m_curOutCard->m_number * 4);
					}
				}

				//�����
				for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
				{
					if (i != bombpos)
					{
						gold[i] += (m_minggangCard[i].size() / 4 * 5) * 4;
						gold[i] += (m_angang[i] * 10) * 4;
						gold[bombpos] -= (m_minggangCard[i].size() / 4 * 5) * 4;
						gold[bombpos] -= (m_angang[i] * 10) * 4;
						LLOG_DEBUG("calcScore2 m_diangang %d m_angang %d m_minggang %d", m_diangang[i], m_angang[i], m_minggang[i]);
					}
				}
			}
		}

		LLOG_DEBUG("HUPai: (1: %d) (2: %d) (3: %d) (4: %d)", gold[0], gold[1], gold[2], gold[3]);
		return true;
	}

	//���һ�ſ����鱨��
	bool CheckTing(Lint pos, std::map<Lint, LastCardSet>& lastCardMap)
	{
		CardVector handcard = m_handCard[pos];
		lastCardMap.clear();

		Lint nPreCardIdx = 0;       //ǰһ���ƵĻ�ɫ��ֵ
		bool bRet = false;   //����ֵ

		bool getCardFirst = true; //��һ��������������ͬ����Ҫ����

		gCardMgr.SortCard(handcard);

		for (Lsize nHandCardIdx = 0; nHandCardIdx < handcard.size(); ++nHandCardIdx)
		{
			Lint nDelCardIdx = handcard[nHandCardIdx]->GetNCIndex();   //�õ���Ҫ�޳���������
			Card * checkCd = handcard[nHandCardIdx];
			if (nPreCardIdx == nDelCardIdx)
			{
				continue;  //��ǰһ������ͬ������Ҫ�ټ������
			}

			nPreCardIdx = nDelCardIdx;
			CardVector tmpHandCard(handcard);   //������ʱ��һ����
												//�޳�һ���ƣ��ٿ���ʣ�µ����Ƿ��������
			CardVector::iterator IT = tmpHandCard.begin();
			while (IT != tmpHandCard.end())
			{
				if (*IT == tmpHandCard[nHandCardIdx])
				{
					tmpHandCard.erase(IT);
					break;
				}
				++IT;
			}

			//�������ƽ��
			std::vector<Card> hu_cards;
			if (gCardMgr.XiaoYi_CheckTing_GetHuCards(hu_cards, tmpHandCard, m_pengCard[pos], m_angangCard[pos], m_minggangCard[pos], m_eatCard[pos]))
			{
				LastCardSet tmpset;
				auto iter = hu_cards.begin();
				while (iter != hu_cards.end())
				{
					LastCard lastcard;
					lastcard.m_nCardNC = (*iter).GetNCIndex();
					tmpset.insert(lastcard);
					++iter;
				}
				lastCardMap[nDelCardIdx] = tmpset;
				bRet = true;
			}
		}

		if (!bRet)
		{
			return false;
		}

		ThinkUnit unit;
		unit.m_type = THINK_OPERATOR_TING;
		m_thinkInfo[pos].m_thinkData.push_back(unit);
		VideoThink(pos);
		return true;
	}

	void SetThinkIng(bool is_think_qianggang = false, bool first_think = false)
	{
		if (!m_desk)
			return;
		bool think = false;
		if (is_think_qianggang && m_curOutCard)
		{
			LLOG_DEBUG("YiYangMajiang setthinking isqinaggang = true, outcard:%d", m_curOutCard->m_color * 10 + m_curOutCard->m_number);
		}
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			m_thinkRet[i].Clear();
			m_thinkInfo[i].Reset();
			if (i != m_curPos)
			{
				mGameInfo.m_GameType = m_desk->getGameType();	// 0 ���ϣ� 3�� ��ɳ
				mGameInfo.b_canEat = false;		// �Ƿ���Գ�
				mGameInfo.b_canHu = (m_playerIsTing[i] == 2);
				mGameInfo.b_onlyHu = (m_beforeType == THINK_OPERATOR_MGANG || m_beforeType == THINK_OPERATOR_MBU || (m_playerIsTing[i] == 2));
				mGameInfo.b_guoGang = false;
				mGameInfo.m_thinkGang = false;	// ���������ǲ��Ǹܳ�������
				mGameInfo.m_deskState = m_desk->getDeskPlayState();	// ��ǰ����״̬
				mGameInfo.m_playerPos = m_curPos;	// ��ǰһ������λ��
				mGameInfo.m_cardState = m_beforeType;	// ��ǰһ������״̬
				mGameInfo.m_MePos = i;		// ��ҵ�λ��
				mGameInfo.m_QiXiaoDui = false;
				mGameInfo.m_pt_laizi = false;
				mGameInfo.m_tianhu = false;
				mGameInfo.m_dihu = first_think;
				mGameInfo.b_menqing = m_playtype.H_MENQING;
				mGameInfo.b_menqingjiangjianghu = m_playtype.H_MENQINGPENGPENGHU;

				if (mGameInfo.b_canHu && m_curOutCard)
				{
					if (m_curOutCard->m_color <= 3)
					{
						mGameInfo.b_canHu = (m_curOutCard->m_number >= 5);
					}
				}

				m_thinkInfo[i].m_thinkData = gCardMgr.CheckOutCardOperator(m_handCard[i], m_pengCard[i], m_angangCard[i], m_minggangCard[i], m_eatCard[i], m_curOutCard, mGameInfo);
				if (m_thinkInfo[i].NeedThink())
				{
					think = true;
					VideoThink(i);
				}
			}
		}

		if (think)
		{
			if (is_think_qianggang)
			{
				m_isThinkingQiangGang = true;
			}
			m_desk->setDeskPlayState(DESK_PLAY_THINK_CARD);
			for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
			{
				LMsgS2CThink think;
				think.m_time = 15;
				think.m_card.m_color = m_curOutCard->m_color;
				think.m_card.m_number = m_curOutCard->m_number;
				if (m_thinkInfo[i].NeedThink())
				{
					think.m_flag = 1;
					m_desk->UpdateLastOptTimeAndPos(m_desk->m_user[i]);
					for (Lsize j = 0; j < m_thinkInfo[i].m_thinkData.size(); ++j)
					{
						ThinkData info;
						info.m_type = m_thinkInfo[i].m_thinkData[j].m_type;
						for (Lsize n = 0; n < m_thinkInfo[i].m_thinkData[j].m_card.size(); ++n)
						{
							CardValue v;
							v.m_color = m_thinkInfo[i].m_thinkData[j].m_card[n]->m_color;
							v.m_number = m_thinkInfo[i].m_thinkData[j].m_card[n]->m_number;
							info.m_card.push_back(v);
						}
						think.m_think.push_back(info);
					}
				}
				else
				{
					think.m_flag = 0;
				}
				m_desk->m_user[i]->Send(think);
			}
		}
		else
		{
			ThinkEnd();
		}
	}

	Lint CheckPositionPrior(Lint CurPos, Lint first_pos, Lint second_pos)
	{
		if (!m_desk)
			return first_pos;
		Lint dif1 = first_pos - CurPos >= 0 ? first_pos - CurPos : first_pos - CurPos + m_desk->m_desk_user_count;
		Lint dif2 = second_pos - CurPos >= 0 ? second_pos - CurPos : second_pos - CurPos + m_desk->m_desk_user_count;
		if (dif1<dif2)
			return first_pos;
		else
			return second_pos;
	}

	void CheckThink()
	{
		if (!m_desk)
			return;
		{
			Lint hu = -1;
			Lint Peng = -1;
			Lint Chi = -1;
			Lint Gang = -1;
			Lint Bu = -1;
			Lint hu_New = -1;
			Lint Peng_New = -1;
			Lint Chi_New = -1;
			Lint Gang_New = -1;
			Lint Bu_New = -1;
			for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
			{
				int pos = i;
				if (m_beforePos != INVAILD_POS)
				{
					pos = (m_beforePos - i) < 0 ? (m_beforePos - i + m_desk->m_desk_user_count) : m_beforePos - i;
				}

				if (m_thinkRet[pos].m_type == THINK_OPERATOR_BOMB)		hu = pos;
				if (m_thinkRet[pos].m_type == THINK_OPERATOR_PENG)		Peng = pos;
				if (m_thinkRet[pos].m_type == THINK_OPERATOR_CHI)		Chi = pos;
				if (m_thinkRet[pos].m_type == THINK_OPERATOR_MGANG)	    Gang = pos;
				if (m_thinkRet[pos].m_type == THINK_OPERATOR_MBU)		Bu = pos;

				if (m_thinkInfo[pos].NeedThink())
				{
					if (m_thinkInfo[pos].HasHu())			hu_New = pos;
					if (m_thinkInfo[pos].HasMGang())		Gang_New = pos;
					if (m_thinkInfo[pos].HasMBu())			Bu_New = pos;
					if (m_thinkInfo[pos].HasPeng())			Peng_New = pos;
					if (m_thinkInfo[pos].HasChi())			Chi_New = pos;
				}
			}

			bool think = false;

			if (hu_New != -1)
				think = true;
			else
			{
				if (hu == -1)
				{
					if (Gang != -1)
					{
						if (Gang_New != -1)
						{
							if (Gang_New == CheckPositionPrior(m_beforePos, Gang, Gang_New))
							{
								think = true;
							}
						}
					}
					else if (Bu != -1)
					{
						if (Gang_New != -1)
						{
							think = true;
						}
					}
					else if (Peng != -1)
					{
						if (Gang_New != -1 || Bu_New != -1)
						{
							think = true;
						}
						else if (Peng_New != -1)
						{
							if (Peng_New == CheckPositionPrior(m_beforePos, Peng, Peng_New))
							{
								think = true;
							}
						}
					}
					else if (Chi != -1)
					{
						if (Gang_New != -1 || Bu_New != -1 || Peng_New != -1)
						{
							think = true;
						}
						else if (Chi_New != -1)
							think = true;
					}
					else {
						if (Gang_New != -1 || Bu_New != -1 || Peng_New != -1 || Chi_New != -1)
						{
							think = true;
						}
					}
				}
			}

			if (!think)
				ThinkEnd();
		}
	}

	void ThinkEnd()
	{
		if (!m_desk)
			return;
		for (int i = 0; i < m_desk->m_desk_user_count; i++)
		{
			if (m_thinkInfo[i].NeedThink())
			{
				VideoDoing(99, i, 0, 0);
			}
			m_thinkInfo[i].Reset();
		}

		Lint huCount = 0;

		Lint pengPos = INVAILD_POS;
		Lint gangPos = INVAILD_POS;
		Lint buPos = INVAILD_POS;
		Lint chiPos = INVAILD_POS;
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			int pos = i;
			if (m_beforePos != INVAILD_POS)
			{
				pos = (m_beforePos - i)<0 ? (m_beforePos - i + m_desk->m_desk_user_count) : m_beforePos - i;
			}

			if (m_thinkRet[pos].m_type == THINK_OPERATOR_BOMB)
			{
				m_playerHuInfo[pos] = WIN_SUB_BOMB;
				m_playerBombInfo[pos] = WIN_SUB_ABOMB;
				huCount++;
			}
			else if (m_thinkRet[pos].m_type == THINK_OPERATOR_MGANG)
				gangPos = pos;
			else if (m_thinkRet[pos].m_type == THINK_OPERATOR_MBU)
				buPos = pos;
			else if (m_thinkRet[pos].m_type == THINK_OPERATOR_PENG)
				pengPos = pos;
			else if (m_thinkRet[pos].m_type == THINK_OPERATOR_CHI)
				chiPos = pos;
		}

		if (huCount != 0)
		{
			CardVector winCards[DESK_USER_COUNT];
			for (int i = 0; i < m_desk->m_desk_user_count; i++)
			{
				if (m_thinkRet[i].m_type == THINK_OPERATOR_BOMB)
				{
					winCards[i] = m_thinkRet[i].m_card;
				}
			}
			OnGameOver(WIN_BOMB, m_playerHuInfo, m_beforePos, winCards);
			return;
		}

		//��
		if (gangPos != INVAILD_POS)
		{
			if (m_playerIsTing[m_beforePos] == 2)
			{
				if (m_curOutCard && m_curOutCard->GetNCIndex() == m_tingCard[m_beforePos].m_ncCard)
				{
					m_tingCard[m_beforePos].setType(2);
					m_tingCard[m_beforePos].resetOutIndex();
					_S_BrocastTingInfo(m_beforePos);
				}
			}

			LMsgS2CUserOper send;
			send.m_pos = gangPos;
			send.m_errorCode = 0;
			send.m_think.m_type = m_thinkRet[gangPos].m_type;
			for (Lsize i = 0; i < m_thinkRet[gangPos].m_card.size(); ++i)
			{
				CardValue v;
				if (m_thinkRet[gangPos].m_card[i])
				{
					v.m_color = m_thinkRet[gangPos].m_card[i]->m_color;
					v.m_number = m_thinkRet[gangPos].m_card[i]->m_number;
					send.m_think.m_card.push_back(v);
				}
			}
			if (m_curOutCard)
			{
				send.m_card.m_color = m_curOutCard->m_color;
				send.m_card.m_number = m_curOutCard->m_number;

				m_desk->BoadCast(send);
				gCardMgr.EraseCard(m_handCard[gangPos], m_thinkRet[gangPos].m_card[0], 3);

				//¼��
				std::vector<CardValue> cards;
				for (int i = 0; i < 4; i++)
				{
					CardValue card;
					card.m_color = m_curOutCard->m_color;
					card.m_number = m_curOutCard->m_number;
					cards.push_back(card);
					m_minggangCard[gangPos].push_back(m_curOutCard);
				}
				m_video.AddOper(VIDEO_OPER_GANG, gangPos, cards);

				m_diangang[gangPos] += 1;
				m_adiangang[m_beforePos] += 1;

				for (int i = 0; i < m_desk->m_desk_user_count; i++)
				{
					m_thinkRet[i].Clear();
				}
				m_beforePos = gangPos;
				m_beforeType = THINK_OPERATOR_MGANG;

				//�������һ����
				SetPlayIng(gangPos, true, true, true, true);
				//SetThinkIng(true);
				return;
			}
		}

		//��
		if (buPos != INVAILD_POS)
		{
			if (m_playerIsTing[m_beforePos] == 2)
			{
				if (m_curOutCard && m_curOutCard->GetNCIndex() == m_tingCard[m_beforePos].m_ncCard)
				{
					m_tingCard[m_beforePos].setType(2);
					m_tingCard[m_beforePos].resetOutIndex();
					_S_BrocastTingInfo(m_beforePos);
				}
			}

			LMsgS2CUserOper send;
			send.m_pos = buPos;
			send.m_errorCode = 0;
			send.m_think.m_type = m_thinkRet[buPos].m_type;
			for (Lsize i = 0; i < m_thinkRet[buPos].m_card.size(); ++i)
			{
				CardValue v;
				v.m_color = m_thinkRet[buPos].m_card[i]->m_color;
				v.m_number = m_thinkRet[buPos].m_card[i]->m_number;
				send.m_think.m_card.push_back(v);
			}

			if (m_curOutCard)
			{
				send.m_card.m_color = m_curOutCard->m_color;
				send.m_card.m_number = m_curOutCard->m_number;

				m_desk->BoadCast(send);
				gCardMgr.EraseCard(m_handCard[buPos], m_thinkRet[buPos].m_card[0], 3);

				//�ѶԷ����ϵ����һ�ų�����ȥ��
				gCardMgr.EraseCard(m_outCard[m_beforePos], m_curOutCard);

				//¼��
				std::vector<CardValue> cards;
				for (int i = 0; i < 4; i++)
				{
					CardValue card;
					card.m_color = m_curOutCard->m_color;
					card.m_number = m_curOutCard->m_number;
					cards.push_back(card);
					m_minggangCard[buPos].push_back(m_curOutCard);
				}
				m_video.AddOper(VIDEO_OPER_OTHER_BU, buPos, cards);


				m_diangang[buPos] += 1;
				m_adiangang[m_beforePos] += 1;

				for (int i = 0; i < m_desk->m_desk_user_count; i++)
				{
					m_thinkRet[i].Clear();
				}
				m_beforePos = buPos;
				m_beforeType = THINK_OPERATOR_MBU;
				//�������һ����
				SetPlayIng(buPos, true, true, true, true, true);
				//SetThinkIng(true);
				return;
			}
		}

		if (pengPos != INVAILD_POS)
		{
			if (m_playerIsTing[m_beforePos] == 2)
			{
				if (m_curOutCard && m_curOutCard->GetNCIndex() == m_tingCard[m_beforePos].m_ncCard && m_outCard[m_beforePos].size() == m_tingCard[m_beforePos].GetOutIndex())
				{
					m_tingCard[m_beforePos].setType(1);
					m_tingCard[m_beforePos].resetOutIndex();
					_S_BrocastTingInfo(m_beforePos);
				}
			}

			LMsgS2CUserOper send;
			send.m_pos = pengPos;
			send.m_errorCode = 0;
			send.m_think.m_type = m_thinkRet[pengPos].m_type;
			for (Lsize i = 0; i < m_thinkRet[pengPos].m_card.size(); ++i)
			{
				CardValue v;
				v.m_color = m_thinkRet[pengPos].m_card[i]->m_color;
				v.m_number = m_thinkRet[pengPos].m_card[i]->m_number;
				send.m_think.m_card.push_back(v);
			}

			if (m_curOutCard)
			{
				send.m_card.m_color = m_curOutCard->m_color;
				send.m_card.m_number = m_curOutCard->m_number;

				m_desk->BoadCast(send);
				gCardMgr.EraseCard(m_handCard[pengPos], m_curOutCard, 2);

				//�ѶԷ����ϵ����һ�ų�����ȥ��
				gCardMgr.EraseCard(m_outCard[m_beforePos], m_curOutCard);

				//¼��
				std::vector<CardValue> cards;
				for (int i = 0; i < 3; i++)
				{
					CardValue card;
					card.m_color = m_curOutCard->m_color;
					card.m_number = m_curOutCard->m_number;
					cards.push_back(card);
					m_pengCard[pengPos].push_back(m_curOutCard);
				}
				m_video.AddOper(VIDEO_OPER_PENG_CARD, pengPos, cards);

				for (int i = 0; i < m_desk->m_desk_user_count; i++)
				{
					m_thinkRet[i].Clear();
				}
				//�����һ����
				m_curGetCard = NULL;
				SetPlayIng(pengPos, false, false, true, false);
				m_needGetCard = true;
				return;
			}
		}

		//��
		if (chiPos != INVAILD_POS)
		{
			LMsgS2CUserOper send;
			send.m_pos = chiPos;
			send.m_errorCode = 0;
			send.m_think.m_type = m_thinkRet[chiPos].m_type;
			for (Lsize i = 0; i < m_thinkRet[chiPos].m_card.size(); ++i)
			{
				CardValue v;
				v.m_color = m_thinkRet[chiPos].m_card[i]->m_color;
				v.m_number = m_thinkRet[chiPos].m_card[i]->m_number;
				send.m_think.m_card.push_back(v);
			}

			if (m_curOutCard)
			{
				send.m_card.m_color = m_curOutCard->m_color;
				send.m_card.m_number = m_curOutCard->m_number;

				m_desk->BoadCast(send);
				if (m_thinkRet[chiPos].m_card.size() > 2 && m_thinkRet[chiPos].m_card[2] && m_thinkRet[chiPos].m_card[0] && m_thinkRet[chiPos].m_card[1])
				{
					gCardMgr.EraseCard(m_handCard[chiPos], m_thinkRet[chiPos].m_card[0]);
					gCardMgr.EraseCard(m_handCard[chiPos], m_thinkRet[chiPos].m_card[1]);

					//�ѶԷ����ϵ����һ�ų�����ȥ��
					gCardMgr.EraseCard(m_outCard[m_beforePos], m_curOutCard);

					//¼��
					std::vector<CardValue> cards;
					//����
					CardValue card;
					card.m_color = m_thinkRet[chiPos].m_card[0]->m_color;
					card.m_number = m_thinkRet[chiPos].m_card[0]->m_number;
					cards.push_back(card);
					//�Ե��Ʒ��м�
					card.m_color = m_curOutCard->m_color;
					card.m_number = m_curOutCard->m_number;
					cards.push_back(card);
					//����
					card.m_color = m_thinkRet[chiPos].m_card[1]->m_color;
					card.m_number = m_thinkRet[chiPos].m_card[1]->m_number;
					cards.push_back(card);

					m_video.AddOper(VIDEO_OPER_EAT, chiPos, cards);

					m_eatCard[chiPos].push_back(m_thinkRet[chiPos].m_card[0]);
					m_eatCard[chiPos].push_back(m_curOutCard);
					m_eatCard[chiPos].push_back(m_thinkRet[chiPos].m_card[1]);

					for (int i = 0; i < m_desk->m_desk_user_count; i++)
					{
						m_thinkRet[i].Clear();
					}
					//�������һ����
					m_curGetCard = NULL;
					SetPlayIng(chiPos, false, false, true, false);
					m_needGetCard = true;
					return;
				}
			}
		}

		//����û���˲���
		if (m_beforeType == THINK_OPERATOR_MGANG || m_beforeType == THINK_OPERATOR_MBU)
		{
			if (m_isThinkingQiangGang)
			{
				m_isThinkingQiangGang = false;
			}
			//¼��
			std::vector<CardValue> cards;
			for (int i = 0; i < 4; i++)
			{
				CardValue card;
				card.m_color = m_curOutCard->m_color;
				card.m_number = m_curOutCard->m_number;
				cards.push_back(card);
			}
			if (m_beforeType == THINK_OPERATOR_MGANG)
			{
				m_video.AddOper(VIDEO_OPER_MING_GANG, m_beforePos, cards);
			}
			else
			{
				m_video.AddOper(VIDEO_OPER_SELF_BU, m_beforePos, cards);
			}
			m_minggangCard[m_beforePos].push_back(m_curOutCard);
			m_minggang[m_beforePos]++;
			m_adiangang[m_beforePos] += 1;
			m_diangangRelation[m_beforePos][gangPos]++;
			CardVector::iterator it = m_pengCard[m_beforePos].begin();
			for (; it != m_pengCard[m_beforePos].end(); it += 3)
			{
				if (gCardMgr.IsSame(m_curOutCard, *it))
				{
					m_minggangCard[m_beforePos].insert(m_minggangCard[m_beforePos].end(), it, it + 3);
					m_pengCard[m_beforePos].erase(it, it + 3);
					break;
				}
			}
			//���ﴦ�� ���� ��������Ϣ
			LMsgS2CUserPlay sendMsg;
			sendMsg.m_errorCode = 0;
			sendMsg.m_pos = m_beforePos;
			sendMsg.m_cs_card.m_type = m_beforeType;
			CardValue card;
			card.m_color = m_curOutCard->m_color;
			card.m_number = m_curOutCard->m_number;
			sendMsg.m_cs_card.m_card.push_back(card);
			m_desk->BoadCast(sendMsg);

			SetPlayIng(m_beforePos, true, true, true, true);
		}
		else
		{
			/*if (DESK_PLAY_BAO_TING != m_desk->getDeskPlayState())
			{
				if (m_curOutCard != NULL)
					m_outCard[m_beforePos].push_back(m_curOutCard);
			}*/
			SetPlayIng(m_desk->GetNextPos(m_beforePos), true, false, true, true);
		}
	}

	void OnGameOver(Lint result, Lint winpos[], Lint bombpos, CardVector winCards[])
	{
		if (m_desk == NULL || m_desk->m_vip == NULL)
		{
			LLOG_DEBUG("OnGameOver NULL ERROR ");
			return;
		}

		//������Ӯ���
		Lint gold[4] = { 0 };
		Lint bombCount = 0;
		std::vector<Lint> winPos;
		//�㲥���
		LMsgS2CGameOver over;
		over.m_user_count = m_desk->m_desk_user_count;

		//�������
		bool checkScore = calcScore(result, winpos, bombpos, gold, bombCount, winPos, over);

		Lint zhuangPos = m_zhuangpos;
		//����ׯ
		if (result == WIN_ZIMO)
		{
			if (winCards)
			{
				if (!winPos.empty())
				{
					if (winPos.front()>-1 && winPos.front()< m_desk->m_desk_user_count)
					{
						m_zhuangpos = winPos.front();
						//¼��
						std::vector<CardValue> cards;
						for (Lint i = 0; i < winCards[m_zhuangpos].size(); i++)
						{
							CardValue curGetCard;
							curGetCard.m_color = winCards[m_zhuangpos][i]->m_color;
							curGetCard.m_number = winCards[m_zhuangpos][i]->m_number;
							cards.push_back(curGetCard);
						}
						m_video.AddOper(VIDEO_OPER_ZIMO, m_zhuangpos, cards);
					}
				}
			}
		}
		else if (result == WIN_BOMB)
		{
			winpos[m_beforePos] = WIN_SUB_ABOMB;		//�õ���λ
			if (!winPos.empty())
			{
				m_zhuangpos = winPos.front();
			}

			//¼��
			std::vector<CardValue> cards;
			if (winCards)
			{
				for (auto x = winPos.begin(); x != winPos.end(); x++)
				{
					if (*x>-1 && *x< m_desk->m_desk_user_count)
					{
						for (Lint i = 0; i < winCards[*x].size(); i++)
						{
							if (winCards[*x][i])
							{
								CardValue curOutCard;
								curOutCard.m_color = winCards[*x][i]->m_color;
								curOutCard.m_number = winCards[*x][i]->m_number;
								cards.push_back(curOutCard);
							}
						}
						m_video.AddOper(VIDEO_OPER_SHOUPAO, *x, cards);
					}
				}

			}

			//������ڣ��Ѿ��Զ����ƣ���ô���������ʾ����
			if (m_isAutoCard[m_beforePos] && !m_isTingHu[m_beforePos])
			{
				for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
				{
					winpos[i] = 0;
					m_playerBombInfo[i] = 0;
				}
				winpos[m_zhuangpos] = WIN_SUB_ABOMB2;		//�õ���λ
			}
		}
		else
		{
			m_zhuangpos = m_curPos;
			//¼��
			std::vector<CardValue> cards;
			m_video.AddOper(VIDEO_OPER_HUANGZHUANG, m_curPos, cards);
		}

		//����¼��
		m_video.m_Id = gVipLogMgr.GetVideoId();
		m_video.m_playType = m_desk->getPlayType();

		over.m_result = result;
		memcpy(over.m_cs_win, winpos, sizeof(over.m_cs_win));
		memcpy(over.m_score, gold, sizeof(over.m_score));
		memcpy(over.m_agang, m_angang, sizeof(over.m_agang));
		memcpy(over.m_mgang, m_minggang, sizeof(over.m_mgang));
		memcpy(over.m_dmgang, m_diangang, sizeof(over.m_dmgang));
		memcpy(over.m_dgang, m_adiangang, sizeof(over.m_dgang));

		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			Lsize oversz = m_handCard[i].size();
			over.m_count[i] = oversz;

			//����
			Lsize cardPos = 0;
			for (Lint j = 0; j < oversz; ++j)
			{
				over.m_card[i][cardPos].m_color = m_handCard[i][j]->m_color;
				over.m_card[i][cardPos++].m_number = m_handCard[i][j]->m_number;
			}

			//��������
			if (m_thinkRet[i].m_type == THINK_OPERATOR_BOMB)
			{
				over.m_hu[i] = m_thinkRet[i].m_hu;
			}
		}
		if (winCards)
		{
			for (auto x = winPos.begin(); x != winPos.end(); x++)
			{
				if (*x>-1 && *x<m_desk->m_desk_user_count)
				{
					for (Lint i = 0; i < winCards[*x].size(); i++)
					{
						CardValue curGetCard;
						curGetCard.m_color = winCards[*x][i]->m_color;
						curGetCard.m_number = winCards[*x][i]->m_number;
						over.m_hucards[*x].push_back(curGetCard);
						LLOG_DEBUG("GameHandler_HengYang onGameOver pos:%d, card:%d", *x, winCards[*x][i]->m_color * 10 + winCards[*x][i]->m_number);
					}
				}
			}
		}
		m_desk->SetDeskWait();
		Lint mgang[4] = { 0 };
		for (Lint i = 0; i < m_desk->m_desk_user_count; ++i)
		{
			mgang[i] += m_minggang[i];
			mgang[i] += m_diangang[i];
		}

		//������	
		Ldouble cal_gold[4] = { 0,0,0,0 };

		//������	
		m_desk->m_vip->AddLog(m_desk->m_user, gold, cal_gold, winpos, zhuangPos, m_angang, mgang, m_playerBombInfo, m_video.m_Id, m_video.m_time);

		LMsgL2LDBSaveVideo video;
		video.m_type = 0;
		m_video.SetEveryResult(4, gold[0], gold[1], gold[2], gold[3]);
		m_video.SetEveryResult2(4, cal_gold[0], cal_gold[1], cal_gold[2], cal_gold[3]);
		video.m_sql = m_video.GetInsertSql();
		gWork.SendMsgToDb(video);

		//�Ƿ����һ��
		over.m_end = m_desk->m_vip->isEnd() ? 1 : 0;
		m_desk->BoadCast(over);

		m_desk->HanderGameOver();

		//////////////////////////////////////////////////////////////////////////
	}

	void VideoThink(Lint pos)
	{
		if (m_thinkInfo[pos].m_thinkData.size() >0)
		{
			std::vector<CardValue> cards;
			for (auto itr = m_thinkInfo[pos].m_thinkData.begin(); itr != m_thinkInfo[pos].m_thinkData.end(); itr++)
			{
				CardValue card;
				card.m_number = itr->m_type;
				if (itr->m_card.size()>0)
					card.m_color = itr->m_card[0]->m_color * 10 + itr->m_card[0]->m_number;
				if (itr->m_card.size()>1)
					card.m_color = card.m_color * 1000 + itr->m_card[1]->m_color * 10 + itr->m_card[1]->m_number;
				cards.push_back(card);
			}
			m_video.AddOper(VIDEO_OPEN_THINK, pos, cards);
		}
	}

	void VideoDoing(Lint op, Lint pos, Lint card_color, Lint card_number)
	{
		std::vector<CardValue> cards;
		CardValue card;
		card.m_number = op;
		card.m_color = card_color * 10 + card_number;
		cards.push_back(card);
		m_video.AddOper(VIDEO_OPEN_DOING, pos, cards);

	}

	void	SetPlayType(std::vector<Lint>& l_playtype) {
		m_playtype.init_playtype_info(l_playtype);
		m_desk->m_desk_user_count = 4;
		//m_playtype.H_ANTING = true;
	}
	//////////////////////////////////////////////////////////////////////////
private:
	Desk			*m_desk;
	Card*			m_curOutCard;//��ǰ����������
	Card*			m_curGetCard;//��ǰ��ȡ����

	Lint			m_playerIsTing[DESK_USER_COUNT];	    //��¼����Ƿ��Ѿ���  0 = ���� 1 = ��������״̬δ���� 2 = ����״̬֮��(ֻ�ܳ���������)
	std::map<Lint, LastCardSet>	m_setNowTingCard[DESK_USER_COUNT];
	bool            m_isAutoCard[DESK_USER_COUNT];//�Զ�����

	ThinkTool		m_thinkInfo[DESK_USER_COUNT];//��ǰ����˼��״̬
	ThinkUnit		m_thinkRet[DESK_USER_COUNT];//��ҷ���˼�����

	CardVector		m_handCard[DESK_USER_COUNT];//������ϵ���

	CardVector		m_outCard[DESK_USER_COUNT];	//��ҳ�����

	CardVector		m_pengCard[DESK_USER_COUNT];//��������ƣ�

	CardVector		m_minggangCard[DESK_USER_COUNT];//������ܵ���

	CardVector		m_angangCard[DESK_USER_COUNT];//��Ұ��ܵ���
	CardVector		m_eatCard[DESK_USER_COUNT];//��ҳԵ���
	LiangPai        m_tingCard[DESK_USER_COUNT];//���������

	Lint			m_angang[DESK_USER_COUNT];//��������
	Lint			m_minggang[DESK_USER_COUNT];//��������
	Lint			m_diangang[DESK_USER_COUNT];//�������
	Lint			m_adiangang[DESK_USER_COUNT];//���������
	Lint			m_diangangRelation[DESK_USER_COUNT][DESK_USER_COUNT];//A��B��ܵĴ���

	Lint			m_gangPos[DESK_USER_COUNT];	//ĳλ����й���

	Lint			m_playerHuInfo[DESK_USER_COUNT];		//��Һ�����Ϣ�� ��Ϊ���Զ�� ���ڵ���ֻ������λ��
	Lint			m_playerBombInfo[DESK_USER_COUNT];	//��ҷ�����Ϣ��[���λ�ö�Ӧ���ڵ�����]


	Lint			m_beforePos;//֮ǰ������λ��
	Lint			m_beforeType;//֮ǰ����������

	Lint			m_gold;

	Lint			m_zhuangpos;//ׯ��λ��
	Lint			m_curPos;						//��ǰ�������

	bool			m_needGetCard;

	bool			m_isThinkingQiangGang;				//��ǰ�Ƿ���˼������
	CardVector		m_deskCard;		//������ʣ�����
	CardVector		m_curOutCards;	//��ǰ����������		(Ϊ��д��)
	Lint			m_haidi_zhuangpos;
	VideoLog		m_video;						//¼��
	bool			m_first_turn;
	OperateState	mGameInfo;
	PlayTypeInfo    m_playtype;
	bool            m_isTingHu[DESK_USER_COUNT];

};

DECLARE_GAME_HANDLER_CREATOR(XiaoYiMajiang, GameHandler_XiaoYi)