#include "User.h"
#include "Work.h"
#include "RoomVip.h"
#include "UserManager.h"
#include "Config.h"
#include "ActiveManager.h"

User::User(LUser data, Lint gateId, Lint gateUserId)
:m_userData(data), m_bLeaveGoldDesk(false), m_userState(0), m_nWaitTime(0)
{
	m_gateId = gateId;
	m_gateUserId = gateUserId;
	m_online = true;
	m_desk = NULL;
	m_goldDesk = NULL;
	m_Active.clear();
	m_userRoomState = EInvalid;
	m_bashu = 0;
	m_bTuoGuan = false;
	memset(&m_isDone, 0, sizeof(m_isDone));
}

User::~User()
{
}

void User::Tick()
{
	if (NULL == m_desk || game_zone_gold != m_desk->m_gameZone)
	{
		return;
	}
	if (true == m_bTuoGuan)
	{
		Lint pos = m_desk->GetUserPos(this);

		if (pos != INVAILD_POS)
		{
			switch (m_desk->m_deskState)
			{
			case DESK_FREE:
				{

				}
			case DESK_WAIT:
				{
					unSetTuoguan();  //準備之後取消託管狀態;(等同系統幫玩家準備，但是並不進入託管)
					LMsgC2SUserReady msg;
					msg.m_pos = pos;
					m_desk->HanderUserReady(this, &msg);

					//unSetTuoguan();  //準備之後取消託管狀態;(等同系統幫玩家準備，但是並不進入託管)
					//必須放在前邊，不然會有多線程錯誤;
				}
				break;
			case DESK_PLAY:
				{
					m_desk->m_GameHandler->ProcessRobot(pos, this);
				}
				break;
			case QIANG_DIZHU:
				m_desk->m_GameHandler->ProcessRobot(pos, this);
				break;
			}			
		}
	}
	else
	{
		if (m_nWaitTime > 0)
		{
			--m_nWaitTime;
		}
		else if (0 == m_nWaitTime)
		{
			setTuoguan();
			if (DESK_WAIT != m_desk->m_deskState)  //等待準備時自動狀態不發送託管通知，會在遊戲開始時候取消自動狀態;
			{
				m_desk->BoadCastGoldDeskEvent(3, m_desk->GetUserPos(this));
			}		
		}
		else
		{
			//负值表示不再等待该玩家，初始值为-1;
		}
	}
}
//获取玩家数据库里id;
Lint User::GetUserDataId()
{
	return m_userData.m_id;
}
//获取玩家唯一表示;
Llong	User::GetUserUnique()
{
	Llong id = ((Llong)GetUserGateId() << 32 | (Llong)GetUserGateUserId());
	return id;
}

void User::SetUserGateId(Lint gateId)
{
	m_gateId = gateId;
}

Lint User::GetUserGateId()
{
	return m_gateId;
}

void User::SetUserGateUserId(Lint gateUserId)
{
	m_gateUserId = gateUserId;
}

Lint User::GetUserGateUserId()
{
	return m_gateUserId;
}

bool User::GetOnline()
{
	return m_online;
}

bool User::Login(Lint gameZone)
{
	//这个方法要改，这样调用太LOW了;
	bool b_login = false;
	m_userData.m_lastLoginTime = gWork.GetCurTime().Secs();
	m_online = true;

	unsetLeaveGoldDesk();
	if (game_zone_gold == gameZone)
	{
		if(GetGoldDesk())
		{
			LLOG_DEBUG("getGoldDesk success");
			b_login = GetGoldDesk()->OnUserReconnect(this);
		}
	}
	else if(GetDesk())
	{
		b_login = GetDesk()->OnUserReconnect(this);
	}
	return b_login;
}

void User::Logout()
{
	m_online = false;
	//m_gateId = 0;
	//m_gateUserId = 0;
	//Save();

	SavePlayCount("system");
}

void User::Send(LMsg& msg)
{
	if (m_desk && game_zone_gold == m_desk->m_gameZone &&  true == getLeaveGoldDesk())  //玩家离开金币场后不让收到金币场消息，防止烦扰其他牌局;
		//消息中没有金币场消息标志，会拦截房卡场消息，当前Logic金币场独立，可以这么设计;
	{
		LLOG_DEBUG("user has not in logic userid:%d", m_userData.m_id);
		return;
	}
	GateInfo* info = gWork.GetGateInfoById(m_gateId);
	if (info)
	{
		LMsgL2GUserMsg send;
		send.m_id = m_gateUserId;
		send.m_dataBuff = msg.GetSendBuff();
		info->m_sp->Send(send.GetSendBuff());
	}
}

void User::SendLoginInfo(Lstring& buyInfo, Lint hide)
{
	LMsgS2CLogin send;
	send.m_id = m_userData.m_id;
	send.m_errorCode = 0;
	send.m_nike = m_userData.m_nike;
	send.m_exp = 0;
	send.m_sex = m_userData.m_sex;
	send.m_new = m_userData.m_new;
	send.m_state = GetDesk() ? 1 : 0;
	send.m_ip = m_ip;
	send.m_face = m_userData.m_headImageUrl;
	send.m_buyInfo = buyInfo;
	send.m_hide = hide;
	send.m_gm = m_userData.m_gm;
	if(m_userData.m_new == 0)
	{
		m_userData.m_new = 1; 
		send.m_card1 = 0;
		send.m_card2 = 0;
		send.m_card3 = 0;
		AddCardCount(CARD_TYPE_8, 0, CARDS_OPER_TYPE_INIT,"system");    //新手送卡，15张;
	}else{
		send.m_card1 = 0;
		send.m_card2 = 0;
		send.m_card3 = 0;
	}
	send.m_gold = m_userData.m_Gold;

	send.nState = LGU_STATE_DESK; // 查询玩家在该工会中的状态 [2017-7-29 willing]
	send.m_isInDesk = 1;		//是否加入了房间
	send.m_nTeamId = m_userData.m_teaminfo.nTeamID;
	send.m_sTeamName = m_userData.m_teaminfo.strTeamName;
	send.teamInfo = m_userData.m_teaminfo.teamInfo;//公会本身的信息
	Send(send);
}

void User::SendItemInfo()
{
	LMsgS2CItemInfo send;
	send.m_card1 = m_userData.m_numOfCard1s;
	send.m_card2 = m_userData.m_numOfCard2s;
	send.m_card3 = m_userData.m_numOfCard3s;
	send.m_gold = m_userData.m_Gold;
	send.m_diamondNum = m_userData.m_diamondNum;
	Send(send);
}

void User::SetDesk(Desk* desk)
{
	m_desk = desk;
}

void User::SetGoldDesk(Desk* desk)
{
	m_goldDesk = desk;
}

Desk* User::GetDesk()
{
	return m_desk;
}

Desk* User::GetGoldDesk()
{
	return m_goldDesk;
}

void User::HanderMsg(LMsg* msg)
{
	switch (msg->m_msgId)
	{
	case MSG_C_2_S_READY:
		HanderUserReady((LMsgC2SUserReady*) msg);
		break;
	case MSG_C_2_S_RESET_ROOM:
		HanderResutDesk((LMsgC2SResetDesk*) msg);
		break;
	case MSG_C_2_S_ASKGOLDDESK_STATE:
		HanderAskGoldDeskState((LMsgC2SAskGoldDeskState*) msg);
		break;
	case MSG_C_2_S_RESET_ROOM_SELECT:
		HanderSelectResutDesk((LMsgC2SSelectResetDesk*) msg);
		break;
	case MSG_C_2_S_LEAVE_ROOM:
		HanderUserLeaveDesk((LMsgC2SLeaveDesk*) msg);
		break;
	case MSG_C_2_S_PLAY_CARD:
		HanderUserPlayCard((LMsgC2SUserPlay*) msg);
		break;
	case MSG_C_2_S_QIANG_DIZHU:
		HanderUserQiangDiZhu((LMsgC2SQiangDiZhu*) msg);
		break;
	case MSG_C_2_S_QDZOPER_NEW:
		HanderUserQiangDiZhuOper((LMsgC2SQdzOper_Sichuan*) msg);
		break;
	case MSG_C_2_S_USER_SPEAK:
		HanderUserSpeak((LMsgC2SUserSpeak*) msg);
		break;
	case MSG_C_2_S_HEART:
		HanderUserHeart((LMsgC2SHeart*) msg);
		break;
	default:
		LLOG_ERROR("LogicServer Message Error: %d", msg->m_msgId);
		break;
	}
}

Lint User::HanderUserCreateDesk(LMsgLMG2LCreateDesk* msg)
{
	LLOG_DEBUG("User::HanderUserCreateDesk %d", msg->m_deskID);
	if (m_desk != NULL)
	{
		LLOG_ERROR("User::HanderUserCreateDesk %d", msg->m_deskID);
		return false;
	}

	return gRoomVip.CreateVipDesk(msg, this);
}

Lint User::HanderUserIntoGoldDesk(LMsgLMG2LIntoGoldDesk* msg)
{
	LLOG_DEBUG("User::HanderUserCreateDesk %d", msg->m_deskID);
	if (m_goldDesk != NULL)
	{
		LLOG_ERROR("User::HanderUserIntoGoldDesk error %d", msg->m_deskID);
		return -1;
	}
	return gRoomVip.IntoVipGoldDesk(msg, this);
}

Lint User::HanderUserAddDesk(Lint nDeskID)
{
	LLOG_DEBUG("User::HanderUserAddDesk %d", nDeskID);
	if (m_desk != NULL)
	{
		LLOG_ERROR("User::HanderUserAddDesk %d", nDeskID);
		return false;
	}

	return gRoomVip.AddToVipDesk(this, nDeskID);
}

void User::HanderUserReady(LMsgC2SUserReady*msg)
{
	if (m_desk)
	{
		m_desk->HanderUserReady(this, msg);
	}
}

void User::HanderResutDesk(LMsgC2SResetDesk* msg)
{
	if(m_desk)
	{
		m_desk->HanderResutDesk(this,msg);
	}
}

void  User::HanderAskGoldDeskState(LMsgC2SAskGoldDeskState* msg)
{
	if(m_desk)
	{
		m_desk->HanderAskGoldDeskState(this,msg);
	}
}

void User::HanderSelectResutDesk(LMsgC2SSelectResetDesk* msg)
{
	if(m_desk)
	{
		m_desk->HanderSelectResutDesk(this,msg);
	}
}

void User::HanderUserLeaveDesk(LMsgC2SLeaveDesk* msg)
{
	gRoomVip.LeaveToVipDesk(msg,this);
}

void User::HanderUserQiangDiZhu(LMsgC2SQiangDiZhu* msg)
{
	if(m_desk)
	{
		m_desk->HanderUserQiangDiZhu(this,msg);
	}
}

void User::HanderUserQiangDiZhuOper(LMsgC2SQdzOper_Sichuan* msg)
{
	if(m_desk)
	{
		m_desk->HanderUserQiangDiZhuOper(this,msg);
	}
}

void User::HanderUserPlayCard(LMsgC2SUserPlay* msg)
{
	if(m_desk)
	{
		m_desk->HanderUserPlayCard(this,msg);
	}
}

void User::HanderUserSpeak(LMsgC2SUserSpeak* msg)
{
	if (m_desk)
	{
		m_desk->HanderUserSpeak(this, msg);
	}
}

void User::HanderUserHeart(LMsgC2SHeart* msg)
{
	LMsgS2CHeart send;
	send.m_time = gWork.GetCurTime().Secs();
	Send(send);
}

Lstring User::GetIp()
{
	return m_ip;
}

void User::SetIp(Lstring& ip)
{
	m_ip = ip;
}

void User::Save()
{
	LMsgL2CeSaveUser save;
	save.m_ID = gConfig.GetServerID();
	save.m_type = m_online?1:0;
	save.m_user = m_userData;
	gWork.SendToLogicManager(save);

	//保存把数;
	//SavePlayCount("system");
}

bool User::IfCardEnough(Lint cardType, Ldouble num)
{
	bool bEnough = false;
	switch (cardType)
	{
	case CARD_TYPE_4:
		{
			if( m_userData.m_numOfCard2s >= num/2)
				bEnough = true;
		}
		break;
	case CARD_TYPE_8:
		{
			if( m_userData.m_numOfCard2s >= num)
				bEnough = true;
		}
		break;
	case CARD_TYPE_16:
		{
			if( m_userData.m_numOfCard2s >= 2 * num)
				bEnough = true;
		}
		break;
	default:
		break;
	}

	return bEnough;
}

void User::AddCardCount(Lint cardType, Ldouble count, Lint operType,const Lstring& admin, bool bNeedSave)
{
	LLOG_INFO("User::AddCardCount type=%d,count=%d,operType=%d", cardType, count, operType);
	Ldouble addCount = 0;
	switch (cardType)
	{
	case CARD_TYPE_4:
		m_userData.m_numOfCard2s += count/2;
		m_userData.m_totalbuynum += count/2;
		addCount = count/2;
		//m_userData.m_numOfCard1s += count;
		break;
	case CARD_TYPE_8:
		m_userData.m_numOfCard2s += count;
		m_userData.m_totalbuynum += count;
		addCount = count;
		break;
	case CARD_TYPE_16:
		m_userData.m_numOfCard2s += count * 2;
		m_userData.m_totalbuynum += count * 2;
		addCount = count*2;
		//m_userData.m_numOfCard3s += count;
		break;
	default:
		break;
	}

	LMsgL2LMGModifyCard msg;
	msg.admin = admin;
	msg.cardType = cardType;
	msg.cardNum = count;
	msg.isAddCard = 1;
	msg.operType = operType;
	msg.m_userid = m_userData.m_id;
	gWork.SendToLogicManager(msg);
}

void User::DelCardCount(Lint cardType, Ldouble count, Lint operType, Lint nGameType, const Lstring& admin)
{
	LLOG_INFO("User::DelCardCount type=%d,count=%d,operType=%d", cardType, count, operType);
	Ldouble delCount = 0;
	switch (cardType)
	{
	case CARD_TYPE_4:
		delCount = count / 2;
		if (m_userData.m_numOfCard2s >= count / 2)
			m_userData.m_numOfCard2s -= count / 2;
		else
			m_userData.m_numOfCard2s = 0;
		break;
	case CARD_TYPE_8:
		delCount = count;
		if (m_userData.m_numOfCard2s >= count)
			m_userData.m_numOfCard2s -= count;
		else
			m_userData.m_numOfCard2s = 0;
		break;
	case CARD_TYPE_16:
		delCount = count * 2;
		if (m_userData.m_numOfCard2s >= count * 2)
			m_userData.m_numOfCard2s -= count * 2;
		else
			m_userData.m_numOfCard2s = 0;
	default:
		break;
	}

	LMsgL2LMGModifyCard msg;
	msg.admin = admin;
	msg.cardType = cardType;
	msg.cardNum = count;
	msg.isAddCard = 0;
	msg.operType = operType;
	msg.m_userid = m_userData.m_id;
	msg.m_nGameType = nGameType;
	gWork.SendToLogicManager(msg);
}

void User::send2LMGGoldChange(Lint changeNum, Lint changeReason, Lint goldNum)
{
	LMsgL2LMGGoldChange msg;
	msg.m_userid = m_userData.m_id;
	msg.m_oper = changeReason;
	msg.m_GoldNum = changeNum;
	gWork.SendToLogicManager(msg);
}

void User::ModifyUserState(bool bLeaveServer)
{
	LMsgL2LMGModifyUserState msg_logic;
	msg_logic.m_userid = m_userData.m_id;
	msg_logic.m_logicID = bLeaveServer ? 0 : gConfig.GetServerID();
	msg_logic.m_userstate = getUserState();
	gWork.SendToLogicManager(msg_logic);

	LMsgL2GModifyUserState msg_gate;
	msg_gate.m_gateid = GetUserGateUserId();
	msg_gate.m_logicID = bLeaveServer ? 0 : gConfig.GetServerID();
	msg_gate.m_userstate = getUserState();
	GateInfo* gateInfo = gWork.GetGateInfoById(GetUserGateId());
	if (gateInfo)
	{
		gateInfo->m_sp->Send(msg_gate.GetSendBuff());
	}
}

bool User::IsRobot()
{
	return false;
}

//十一活动对局数的统计
void User::SavePlayCount(const Lstring& admin)
{
	LMsgL2LMGAddUserBashu msg;
	msg.admin = admin;
	msg.m_addBashu = m_bashu;
	msg.m_userid = m_userData.m_id;
	gWork.SendToLogicManager(msg);

	//把数清零;
	m_bashu = 0;
}

void User::addBashu(Lint baishu)
{ 
	if(baishu >= 0)
	{
		m_bashu += baishu;
	}
}

void  User::setTuoguan()
{
	m_bTuoGuan = true;
	m_nWaitTime = -1;
}

void  User::unSetTuoguan()
{
	m_bTuoGuan = false;
	m_nWaitTime = -1;
}

void User::setUserState(Lint nValue, Lint maYouQuan /*= 0*/)
{
	m_userState = nValue;
	Lint nPos = DEFAULT_DESK_USER_COUNT;
	if (m_desk)
	{
		nPos = m_desk->GetUserPos(this);
	}
	if (maYouQuan > 0)
	{
		// 把状态同步给 工会服务器 [2017-7-25 willing]
		LMsgLS2TS_GameState gameState;
		gameState.nIsInGame = (nValue == LGU_STATE_CENTER) ? 0 : 1;
		gameState.nUserID = GetUserDataId();
		gameState.nPos = nPos;
		gWork.SendMsgToTeamServer(gameState);
	}
}
