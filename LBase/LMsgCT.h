#ifndef _L_MSG_CT_H_
#define _L_MSG_CT_H_

#include "LMsg.h"
#include "GameDefine.h"
#include <vector>

typedef std::map<Lint, Lint>		tIntIntMap;
typedef std::list<Lint>				tIntList;
typedef std::map<Lint, tIntList>	tIntListMap;
typedef std::map<Lstring, Lint>		tStringIntMap;


struct STeamPlayConfigMsg
{
	Lint				m_roomType = 0;				//��������  8��  16��
	Lint				m_playEnum = -1;				//�淨ö��  
	Lint				m_teamID = 0;				//����ID
	std::vector<Lint>	m_playtype;					//�淨
	Llong				m_nCreateTime = 0;			// �淨������ʱ��� 
	Lint                m_circleGoldLimit = 0;
	Lint                m_playtype_count;
	MSGPACK_DEFINE_MAP(m_roomType, m_playEnum, m_teamID, m_playtype, m_nCreateTime, m_circleGoldLimit);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_roomType);
		buff.Read(m_playEnum);
		buff.Read(m_teamID);
		buff.Read(m_playtype_count);
		m_playtype.resize(m_playtype_count);
		for (Lint i = 0; i < m_playtype_count; ++i)
		{
			buff.Read(m_playtype[i]);
		}
		buff.Read(m_nCreateTime);
		buff.Read(m_circleGoldLimit);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_roomType);
		buff.Write(m_playEnum);
		buff.Write(m_teamID);
		m_playtype_count = m_playtype.size();
		buff.Write(m_playtype_count);
		for (Lint i = 0; i < m_playtype_count; ++i)
		{
			buff.Write(m_playtype[i]);
		}
		buff.Write(m_nCreateTime);
		buff.Write(m_circleGoldLimit);
		return true;
	}
};


// �ͻ��˲�ѯ������Ϣ [2017-7-17 willing]
struct LMsgC2SQuerTeam : public LMsgSC
{
	int   nTeamID = 0;		// Ҫ��ѯ�Ĺ���ID [2017-7-17 willing]

	LMsgC2SQuerTeam() :LMsgSC(MSG_C_2_S_QueryTeam) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SQuerTeam(); }
};

// ������Ϣ�õ��Ĺ�����Ϣ�ṹ�� [2017-7-21 willing]
struct STeamDescInfoMsg
{
	Lint		nTeamID = 0;
	Lstring		strTeamName;
	Lint		nCreaterID = 0;
	Lstring		strCreaterName;
	Lint		nMemCount = 0;
	Lint		nOnLienCount = 0;
	Lstring		m_CreaterHeadImageURL;
	Lint		nReqJoinTime = 0;		// ���ᴴ��ʱ��
	Lint		m_isMaster = 0;			//�Ƿ���Ⱥ�� 0������ 1����
	Lstring		m_boardInfo;
	Lint		m_nCards = 0;

	//�¼�����
	Lint	nManageId = 0;              //�ϼ����id
	Lint	nCountyId = 0;              //����id
	Lint    nDeskCntLimit = 0;          //����������
	std::vector<Lint> vPlayTypes;   // �淨����
	Lint    nDeskProportionate = 0; //���ӵĳ�ɱ���

	STeamPlayConfigMsg	m_playConfig;
	MSGPACK_DEFINE_MAP(nTeamID, strTeamName, nCreaterID, strCreaterName, nMemCount, nOnLienCount, nReqJoinTime, m_CreaterHeadImageURL,
		m_isMaster, m_playConfig, m_boardInfo, m_nCards, nManageId, nCountyId, nDeskCntLimit, vPlayTypes, nDeskProportionate);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual bool Read(LBuff& buff)
	{
		buff.Read(nTeamID);
		buff.Read(strTeamName);
		buff.Read(nCreaterID);
		buff.Read(strCreaterName);
		buff.Read(nMemCount);
		buff.Read(nOnLienCount);
		buff.Read(m_CreaterHeadImageURL);
		buff.Read(nReqJoinTime);
		buff.Read(m_isMaster);
		buff.Read(m_boardInfo);
		buff.Read(m_nCards);
		buff.Read(nManageId);
		buff.Read(nCountyId);
		buff.Read(nDeskCntLimit);
		Lint vPlayTypes_count;
		buff.Read(vPlayTypes_count);
		vPlayTypes.resize(vPlayTypes_count);
		for (Lint i = 0; i < vPlayTypes_count; ++i)
		{
			buff.Read(vPlayTypes[i]);
		}
		buff.Read(nDeskProportionate);
		m_playConfig.Read(buff);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(nTeamID);
		buff.Write(strTeamName);
		buff.Write(nCreaterID);
		buff.Write(strCreaterName);
		buff.Write(nMemCount);
		buff.Write(nOnLienCount);
		buff.Write(m_CreaterHeadImageURL);
		buff.Write(nReqJoinTime);
		buff.Write(m_isMaster);
		buff.Write(m_boardInfo);
		buff.Write(m_nCards);
		buff.Write(nManageId);
		buff.Write(nCountyId);
		buff.Write(nDeskCntLimit);
		Lint vPlayTypes_count = vPlayTypes.size();
		buff.Write(vPlayTypes_count);
		for (Lint i = 0; i < vPlayTypes_count; ++i)
		{
			buff.Write(vPlayTypes[i]);
		}
		buff.Write(nDeskProportionate);
		m_playConfig.Write(buff);
		return true;
	}
};

// ���������� ������Ϣ [2017-7-18 willing]
struct LMsgS2CQueryTeamInfo : public LMsgSC
{
	int		nTeamID = 0;				// Ҫ��ѯ�Ĺ���ID,�����ѯ���� ��ֵΪ0 [2017-7-17 willing]
	Lint	nCreaterID = 0;				// ������ID [2017-7-14 willing]
	Lstring	strCreaterName = "";		// �������� [2017-7-14 willing]
	Lstring	strTeamName = "";			// �������� [2017-7-14 willing]
	Lint	nMemCount = 0;
	Lint	nOnLineCount = 0;
	Lstring	m_CreaterHeadImageURL = "";
	Lint	nState = 0;					// ��ѯ����ڸù����е�״̬ [2017-7-29 willing]
	Lint	m_isInDesk = 0;		//�Ƿ�����˷���

	STeamDescInfoMsg teamInfo;//���᱾�����Ϣ

	LMsgS2CQueryTeamInfo() :LMsgSC(MSG_S_2_C_TeamInfo) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nCreaterID, strCreaterName, strTeamName, nMemCount, nOnLineCount,nState, m_CreaterHeadImageURL, m_isInDesk, teamInfo);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CQueryTeamInfo(); }
};


// ���������� ������Ϣ [2017-7-18 willing]
struct QueryTeamInfo
{
	Lint	nTeamID = 0;				// Ҫ��ѯ�Ĺ���ID,�����ѯ���� ��ֵΪ0 [2017-7-17 willing]
	Lint	nCreaterID = 0;				// ������ID [2017-7-14 willing]
	Lstring	strCreaterName = "";		// �������� [2017-7-14 willing]
	Lstring	strTeamName = "";			// �������� [2017-7-14 willing]
	Lint	nMemCount = 0;
	Lint	nOnLineCount = 0;
	Lstring	m_CreaterHeadImageURL = "";
	Lint	nState = 0;					// ��ѯ����ڸù����е�״̬ [2017-7-29 willing]
	Lint	m_isInDesk = 0;		//�Ƿ�����˷���

	STeamDescInfoMsg teamInfo;//���᱾�����Ϣ

	MSGPACK_DEFINE_MAP(nTeamID, nCreaterID, strCreaterName, strTeamName, nMemCount, nOnLineCount, nState, m_CreaterHeadImageURL, m_isInDesk, teamInfo);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual bool Read(LBuff& buff)
	{
		buff.Read(nTeamID);
		buff.Read(nCreaterID);
		buff.Read(strCreaterName);
		buff.Read(strTeamName);
		buff.Read(nMemCount);
		buff.Read(nOnLineCount);
		buff.Read(m_CreaterHeadImageURL);
		buff.Read(nState);
		buff.Read(m_isInDesk);

		teamInfo.Read(buff);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(nTeamID);
		buff.Write(nCreaterID);
		buff.Write(strCreaterName);
		buff.Write(strTeamName);
		buff.Write(nMemCount);
		buff.Write(nOnLineCount);
		buff.Write(m_CreaterHeadImageURL);
		buff.Write(nState);
		buff.Write(m_isInDesk);

		teamInfo.Write(buff);
		return true;
	}
};


//////////////////////////////////////////////////////////////////////////
//������ҵ�½�����Ϣ
struct LMsgS2CLogin :public LMsgSC
{
	Lint		m_errorCode;//��¼�����룬0-��¼�ɹ�,1-�ʺ�δע�ᣬ2-�ʺ��ѵ�¼��3-δ֪���� 17-���˺��������̻��ڷ��� ��ʹ��֮ǰ�̻���
	Lint		m_id;
	Lstring		m_unionId;
	Lstring		m_nike;
	Lint		m_exp;
	Lint		m_sex;
	Lstring		m_face;//
	Lstring		m_provice;
	Lstring		m_city;
	Lstring		m_ip;
	Lstring		m_playerType;
	Lint		m_new;//0-���ǣ�1-��
	Lint		m_card1;//���ֽ�������1
	Ldouble		m_card2;//���ֽ�������2
	Lint		m_card3;//���ֽ�������3
	Lint        m_gold; //��ҽ����;
	Lint		m_state;//0-���������1-��������
	Lstring		m_buyInfo;//������Ϣ
	Lint		m_hide;//��������0-���� 1- ������
	Lint		m_gm;//0-���ǣ�1-��GM
	Lint		m_curHeadIconId; //���ͷ���ID
	Lstring     m_phone;
	std::vector<Lint>	m_gainHeadIconId; //����ѻ�ȡ��ͷ���ID
	Lint		m_nCurTime = 0; //��������ǰʱ���

	Lint        m_nTeamId = 0; //��ǰʹ�õľ��ֲ�id
	Lstring     m_sTeamName; //��ǰʹ�õľ��ֲ����� �д����ʱ�����ʾ

	Lint	    nState = 0;	// ��ѯ����ڸù����е�״̬ [2017-7-29 willing]
	Lint	    m_isInDesk = 0;	//�Ƿ�����˷���
	Lint		m_serverTime;

	STeamDescInfoMsg teamInfo; //���ֲ��Լ�����Ϣ

	LMsgS2CLogin() :LMsgSC(MSG_S_2_C_LOGIN), m_errorCode(0), m_id(0), m_exp(0), m_sex(0)
		, m_new(0), m_card1(0), m_card2(0.0f), m_card3(0), m_state(0), m_hide(0), m_gm(0), m_curHeadIconId(0), m_serverTime(0)
	{
	}

	MSGPACK_DEFINE_MAP(m_msgId, m_errorCode, m_id, m_unionId, m_nike, m_exp, m_sex, m_face, m_provice,
		m_city, m_ip, m_playerType, m_new, m_card1, m_card2, m_card3, m_gold, m_state, m_buyInfo, m_hide, m_gm,
		m_curHeadIconId, m_phone, m_gainHeadIconId, m_nCurTime, m_nTeamId, m_sTeamName, 
		nState, m_isInDesk, m_serverTime, teamInfo);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CLogin(); }
};


// ������빤�� [2017-7-19 willing]
struct LMsgC2SReqJoin : public LMsgSC
{
	int   nTeamID = 0;		// Ҫ�������Ĺ���ID [2017-7-17 willing]

	LMsgC2SReqJoin() :LMsgSC(MSG_C_2_S_REQ_JOIN) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SReqJoin(); }
};

// ������빤��Ľ�� [2017-7-19 willing]
struct LMsgS2CJoinRst : public LMsgSC
{
	Lint  nTeamID = 0;
	int   nJoinRst = 0;		// 0 ����ɹ���>0 ʧ��ԭ�� [2017-7-17 willing]

	LMsgS2CJoinRst() :LMsgSC(MSG_S_2_C_JOIN_RST) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID,nJoinRst);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CJoinRst(); }
};

// ������֪ͨ�᳤��ĳ����ҵĲ��� [2017-7-20 willing]
struct LMsgS2CNoticeEvent : public LMsgSC
{
	Lint nTeamID = 0;
	Lstring strCreaterName = "";
	Lstring strTeamName = "";
	Lint nOpt = 0;				// 0 ���������ύ�ɹ���1 �᳤��׼���룬2 �ܾ����룬 3 �ܾ��뿪 4 �᳤ͬ���뿪��5 ���᳤�Ƴ� 6 �᳤��������Ҽ��빤�� 15 �᳤���Ĺ�����Ϣ
	STeamDescInfoMsg m_teamInfo;
	LMsgS2CNoticeEvent() :LMsgSC(MSG_S_2_C_NOTICE_EVENT) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID,strCreaterName, strTeamName, nOpt, m_teamInfo);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CNoticeEvent(); }
};

// ��������Լ����ڵĹ����б� [2017-7-20 willing]
struct LMsgC2SReqTeamList : public LMsgSC
{
	int   nTeamState = 0;		// 1 �Ѿ�����ģ�2 �Ѿ�����Ĺ��� [2017-7-20 willing]

	LMsgC2SReqTeamList() :LMsgSC(MSG_C_2_S_REQ_TEAM_LIST) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamState);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SReqTeamList(); }
};


struct LMsgS2CTeamPlayConfig : public LMsgSC
{
	STeamPlayConfigMsg	m_playConfig;
	MSGPACK_DEFINE_MAP(m_msgId, m_playConfig);
	LMsgS2CTeamPlayConfig() :LMsgSC(MSG_S_2_C_PLAY_CONFIG) {}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CTeamPlayConfig(); }
};




// ��ͻ��˷��ع����б� [2017-7-20 willing]
struct LMsgS2CTeamList : public LMsgSC
{
	Lint nReqState = 0;		// ��������״ֵ̬ [2017-7-21 willing]
	Lint	m_isInDesk = 0;		//�Ƿ�����˷���
	std::vector<STeamDescInfoMsg> teamInfoList;

	LMsgS2CTeamList() :LMsgSC(MSG_S_2_C_TEAM_LIST) {}
	MSGPACK_DEFINE_MAP(m_msgId, nReqState, teamInfoList, m_isInDesk);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CTeamList(); }
};

// ��ȡ����������Ϣ [2017-7-21 willing]
struct LMsgC2SGetTeamInfo : public LMsgSC
{
	int   nTeamID = 0;

	LMsgC2SGetTeamInfo() :LMsgSC(MSG_C_2_S_TEAM_INFO) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetTeamInfo(); }
};

// ���ع���������Ϣ [2017-7-21 willing]
struct LMsgS2CTeamInfo : public LMsgSC
{
	Lint	m_error = 0;				//0:����
	Lint	nTeamID = 0;				// ����ID [2017-7-14 willing]
	Lstring strCreaterName;			// ���������� [2017-7-19 willing]
	Lstring	strTeamName;			// �������� [2017-7-14 willing]
	Lint	nCards = 0;					// ���᷿�� [2017-7-14 willing]
	Llong	nCreateTime = 0;			// ���ᴴ��ʱ��� [2017-7-14 willing]
	Lint	nMemCardLimitPerDay = 0;	// �����Աÿ������ʹ�ù��Ῠ������ [2017-7-14 willing]
	Lint	nState = 0;					// ��ǰ����״̬���Ƿ�����ʹ�ù��᷿���������� �� [2017-7-14 willing]
	Lint	nOnLineCount = 0;			// ��ǰ�������� [2017-7-18 willing]
	Lint	nMemCount = 0;				// ֻ����״̬�����ģ������������е���� [2017-7-19 willing]

	Lint	nTodayUserCard = 0;			// ��ҵ�����Ѿ�ʹ�õĹ��᷿�� [2017-7-26 willing]
	Lstring	m_boardInfo;				//���ṫ��
	LMsgS2CTeamInfo() :LMsgSC(MSG_S_2_C_Team_Info) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, strCreaterName, strTeamName, nCards, nCreateTime,
		nMemCardLimitPerDay, nState, nOnLineCount, nMemCount , nTodayUserCard, m_boardInfo, m_error);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CTeamInfo(); }
};

// ���󹤻��Ա�б� [2017-7-21 willing]
struct LMsgC2SGetTeamMemberList : public LMsgSC
{
	int   nTeamID = 0;

	LMsgC2SGetTeamMemberList() :LMsgSC(MSG_C_2_S_Member_List) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetTeamMemberList(); }
};

// ������Ϣ�õ��Ĺ����Ա��Ϣ�ṹ [2017-7-21 willing]
struct STeamMemberMsg
{
	Lint	nUserID = 0;				// ���UserID [2017-7-14 willing]
	Lstring strUserName;			// ������� [2017-7-21 willing]
	Lstring strHeadImageUrl;		// ���ͷ���URL [2017-7-14 willing]
	Lint	nGameState = 0;				// �������Ϸ�е�״̬ [2017-7-14 willing]
	Lint	nTeamState = 0;				//����ڹ����е�״̬ 1:�������  2��������Ա  3�������뿪
	Lint	m_reqTime = 0;				//�����������뿪��ʱ���
	Lint	m_joinTime = 0;
	MSGPACK_DEFINE_MAP(nUserID, strUserName, strHeadImageUrl, nGameState, nTeamState, m_reqTime, m_joinTime);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
};

// ���ع����Ա�б� [2017-7-21 willing]
struct LMsgS2CMemberList : public LMsgSC
{
	Lint nPackageIndex = 0;			// 0 ��һ������>0 �����İ���-1 ���һ���� [2017-8-11 willing]
	std::vector<STeamMemberMsg> memberList;

	LMsgS2CMemberList() :LMsgSC(MSG_S_2_C_Member_List) {}
	MSGPACK_DEFINE_MAP(m_msgId, nPackageIndex, memberList);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CMemberList(); }
};

// ȡ��������� [2017-7-21 willing]
struct LMsgC2SCancelJoin : public LMsgSC
{
	int   nTeamID = 0;

	LMsgC2SCancelJoin() :LMsgSC(MSG_C_2_S_Cancel_Join) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SCancelJoin(); }
};

// ȡ���������Ľ�� [2017-7-21 willing]
struct LMsgS2CCancelJoin : public LMsgSC
{
	Lint nTeamID = 0;			// ����ID [2017-7-22 willing]
	Lint nResult = 0;			// 0 �ɹ� [2017-7-21 willing]
	Lstring strTeamName = "";	// �������� [2017-7-21 willing]

	LMsgS2CCancelJoin() :LMsgSC(MSG_S_2_C_Cancel_Join) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID,nResult,strTeamName);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CCancelJoin(); }
};

// ֪ͨ������ң����ڵĹ��ᱻ�᳤��ɢ�� [2017-7-22 willing]
struct LMsgS2CNoticeTeamDismiss : public LMsgSC
{
	Lstring strCreaterName = "";	// ���������� [2017-7-22 willing]
	Lstring strTeamName = "";		// �������� [2017-7-21 willing]
	Lint    nTeamId = 0;			//����ID
	LMsgS2CNoticeTeamDismiss() :LMsgSC(MSG_S_2_C_Notice_Dismiss) {}
	MSGPACK_DEFINE_MAP(m_msgId, strCreaterName, strTeamName, nTeamId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CNoticeTeamDismiss(); }
};
// �����뿪���� [2017-7-22 willing]
struct LMsgC2SLeaveTeam : public LMsgSC
{
	int   nTeamID = 0;

	LMsgC2SLeaveTeam() :LMsgSC(MSG_C_2_S_Leave_Team) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SLeaveTeam(); }
};

// �����뿪����Ľ�� [2017-7-22 willing]		
struct LMsgS2CLeaveTeamRst : public LMsgSC
{
	Lint nTeamID = 0;			// ����ID [2017-7-22 willing]
	Lint nResult = 0;			// 0 �ɹ� [2017-7-21 willing]
	Lstring strTeamName = "";	// �������� [2017-7-21 willing]

	LMsgS2CLeaveTeamRst() :LMsgSC(MSG_S_2_C_Leave_Team_Rst) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nResult, strTeamName);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CLeaveTeamRst(); }
};

// ͨ�����ᴴ������ʧ�� [2017-7-23 willing]
struct LMsgC2SCreateTeamDeskError : public LMsgSC
{
	int   nErrorCode = 0;

	LMsgC2SCreateTeamDeskError() :LMsgSC(MSG_S_2_C_CreateDeskError) {}
	MSGPACK_DEFINE_MAP(m_msgId, nErrorCode);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SCreateTeamDeskError(); }
};

// ������Ϣ�ı� [2017-7-23 willing]
struct LMsgS2CTeamInfoChange : public LMsgSC
{
	Lint nTeamID = 0;			// ����ID [2017-7-23 willing]
	Lint nCards = 0;			// ���᷿������ [2017-7-23 willing]

	LMsgS2CTeamInfoChange() :LMsgSC(MSG_S_2_C_TeamInfoChange) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID,nCards);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CTeamInfoChange(); }
};

// �鿴��Ա��ս�� [2017-7-24 willing]
struct LMsgC2SGetVideoLog : public LMsgSC
{
	int		nTeamID = 0;
	int		nUserID = 0;
	int		nYear = 0;
	int		nMonth = 0;
	int		nDay = 0;

	LMsgC2SGetVideoLog() :LMsgSC(MSG_C_2_S_GET_VIDEO_LOG) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nUserID, nYear, nMonth, nDay);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetVideoLog(); }
};

// �ͻ��˲�ѯ����ս���õ��Ľṹ�� [2017-7-24 willing]
struct STeamVideoLog
{
	Lint nTime = 0;
	Lint nDeskID = 0;
	std::vector<Lint> nUserID;
	std::vector<Lstring> strUserName;
	std::vector<Lint> nScore;
	Lint nOptState = 0;
	Lint m_flag = 0;
	Lint m_maxCircle = 0;
	MSGPACK_DEFINE_MAP(nTime, nDeskID, nUserID, strUserName, nScore, nOptState, m_flag, m_maxCircle);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
};

// �������������ս�� [2017-7-24 willing]
struct LMsgS2CVideoLog : public LMsgSC
{
	Lint nPackageIndex = 0;			//-2:��ѯս��ʧ�� -1�����һ���� 0����һ��
	std::vector<STeamVideoLog>	teamVideoLog;

	LMsgS2CVideoLog() :LMsgSC(MSG_S_2_C_VIDEO_LOG) {}
	MSGPACK_DEFINE_MAP(m_msgId, nPackageIndex, teamVideoLog);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CVideoLog(); }
};

// ��Ա��Ϣ�ı� [2017-7-25 willing]
struct LMsgS2CMemberInfoChange : public LMsgSC
{
	Lint nTeamID = 0;
	Lint nTodayUseCardCount = 0;

	LMsgS2CMemberInfoChange() :LMsgSC(MSG_S_2_C_MemberInfoChange) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nTodayUseCardCount);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CMemberInfoChange(); }
};

// ֪ͨ�����Ա�´�����һ������ [2017-8-3 willing]
struct LMsgS2CNewTeamDesk : public LMsgSC
{
	Lint nTeamID = 0;
	Lint nDeskID = 0;
	std::vector<Lint> nUserID;
	std::vector<Lstring> strHeadImageUrl;			// ��ҵ�ͷ���б� [2017-8-5 willing]
	Lint nMajongType = 0;							// �淨���� [2017-8-2 willing]
	Lint nMaxCircle = 0;							// ������ [2017-8-3 willing]
	Lint nMaxUserCount = 0;							// ���ӵ����������� [2017-8-4 willing]
	Lint nUpDownJi = 0;								// ���¼�
	Lint nAllJi = 0;								// ���ü�
	Lint nLianZhuang = 0;							// ��ׯ�Ƿ���� [2016-9-12 willing]
	Lint nPlayTypeCount = 0;						// С�淨���� [2017-8-3 willing]
	std::vector<Lint>	tPlayType;					// С�淨ѡ�� [2017-8-2 willing]
	Llong nCreateTime;								// ���Ӵ�����ʱ��� [2017-8-5 willing]

	LMsgS2CNewTeamDesk() :LMsgSC(MSG_S_2_C_NEW_TEAM_DESK),nCreateTime(0) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nDeskID,nUserID, strHeadImageUrl,nMajongType, nMaxCircle, nMaxUserCount,
						nUpDownJi, nAllJi, nLianZhuang, nPlayTypeCount, tPlayType, nCreateTime);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CNewTeamDesk(); }
};

// ���������е���Ա�仯�� [2017-8-3 willing]
struct LMsgS2CTeamDeskUserChg : public LMsgSC
{
	Lint nTeamID = 0;
	Lint nDeskID = 0;
	Lint nUserID = 0;
	Lint nIsJoin = 0;		// 1 �������ӣ�0 �뿪���� [2017-8-3 willing]
	Lstring strHeadImageUrl;

	LMsgS2CTeamDeskUserChg() :LMsgSC(MSG_S_2_C_TeamDeskUserChg) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nDeskID, nUserID, nIsJoin, strHeadImageUrl);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CTeamDeskUserChg(); }
};

// ɾ��һ���������ӣ�����ɢ�����Ѿ����� [2017-8-3 willing]
struct LMsgS2CDelTeamDesk: public LMsgSC
{
	Lint nTeamID = 0;
	Lint nDeskID = 0;

	LMsgS2CDelTeamDesk() :LMsgSC(MSG_S_2_C_DelTeamDesk) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nDeskID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CDelTeamDesk(); }
};

// �ͻ�������ǰ���еĹ������� [2017-8-3 willing]
struct LMsgC2SGetAllTeamDesk : public LMsgSC
{
	int		nTeamID = 0;
	int		nUserID = 0;

	LMsgC2SGetAllTeamDesk() :LMsgSC(MSG_C_2_S_GetAllTeamDesk) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nUserID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetAllTeamDesk(); }
};

// ���й����е����� [2017-8-3 willing]
struct STeamDeskInfoMsg
{
	Lint nTeamID = 0;
	Lint nDeskID = 0; //�����
	std::vector<Lint> nUserID;
	std::vector<Lstring> strHeadImageUrl;			// ���ͷ�� [2017-8-5 willing]
	std::vector<Lstring> m_nike;					// ����ǳ�
	Lint nMajongType = 0;							// �淨���� [2017-8-2 willing]
	Lint nMaxCircle = 0;							// ������ [2017-8-3 willing]
	Lint nMaxUserCount = 0;
	//Lint nPlayTypeCount = 0;						// С�淨���� [2017-8-3 willing]
	std::vector<Lint>	tPlayType;					// С�淨ѡ�� [2017-8-2 willing]
	Llong nCreateTime = 0;							// ���Ӵ�����ʱ��� [2017-8-5 willing]
	Lint nStake = 0;							    //Ѻע����
	Lint nCircleGoldLimit = 0;                      //ÿһ���ӵ����ƽ���Ľ����
	Lint nMaxFanshu = 0;                            //�ͻ���ը��
	MSGPACK_DEFINE_MAP(nTeamID, nDeskID, nUserID, strHeadImageUrl,nMajongType, nMaxCircle, 
						nMaxUserCount, tPlayType,nCreateTime, nStake, m_nike, nCircleGoldLimit, nMaxFanshu);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
};

struct LMsgS2CAllTeamDesk : public LMsgSC
{
	Lint nTeamID = 0;
	std::vector<STeamDeskInfoMsg> tAllTeamDesk;

	LMsgS2CAllTeamDesk() :LMsgSC(MSG_S_2_C_ALLTeamDesk) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, tAllTeamDesk);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CAllTeamDesk(); }
};

// �������ӱ仯��Ϣ [2017-8-3 willing]
struct LMsgC2SSubscribeTeamDesk : public LMsgSC
{
	int		nTeamID = 0;
	int		nCareFor = 0;		// 0 ȡ�����ģ�1 ���� [2017-8-3 willing]

	LMsgC2SSubscribeTeamDesk() :LMsgSC(MSG_C_2_S_SubscribeTeamDesk) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nCareFor);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SSubscribeTeamDesk(); }
};

// ��Ҳ�ѯ�Լ��Թ���Ĳ�����־ [2017-8-8 willing]
struct LMsgC2SGetTeamOptLog : public LMsgSC
{
	LMsgC2SGetTeamOptLog() :LMsgSC(MSG_C_2_S_GetTeamOptLog) {}
	MSGPACK_DEFINE_MAP(m_msgId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetTeamOptLog(); }
};

// ������������ҶԹ���Ĳ�����־ [2017-8-8 willing]
struct STeamMemOptLog
{
	int nTeamID = 0;
	Lstring strTeamName = "";
	int nOpt = 0;
	Llong nTimestamp = 0;		// ʱ��� [2017-8-8 willing]

	MSGPACK_DEFINE_MAP(nTeamID, strTeamName, nOpt, nTimestamp);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
};

struct LMsgS2CMemTeamOptLog : public LMsgSC
{
	std::vector<STeamMemOptLog> tAllTeamMemOptLog;

	LMsgS2CMemTeamOptLog() :LMsgSC(MSG_S_2_C_MemTeamOptLog) {}
	MSGPACK_DEFINE_MAP(m_msgId, tAllTeamMemOptLog);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CMemTeamOptLog(); }
};

// ��ȡָ��ʱ���ڵĶԾ���־ [2017-8-10 willing]
struct LMsgC2SGetRangeVideoLog : public LMsgSC
{
	int		nTeamID = 0;
	int		nYear = 0;
	int		nMonth = 0;
	int		nDay = 0;
	int		nEndYear = 0;
	int		nEndMonth = 0;
	int		nEndDay = 0;
	LMsgC2SGetRangeVideoLog() :LMsgSC(MSG_C_2_S_GET_RANGE_VIDEO_LOG) {}
	MSGPACK_DEFINE_MAP(m_msgId, nTeamID, nYear, nMonth, nDay, nEndYear, nEndMonth, nEndDay);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SGetRangeVideoLog(); }
};

struct LMsgC2SCreatePlayConfig : public LMsgSC
{
	Lint				m_roomType;				//��������  8��  16��
	Lint				m_playEnum;				//�淨ö��  
	Lint				m_teamID;				//����ID
	std::vector<Lint>	m_playtype;				//�淨
	Lint				m_opt;					//�������� 0��
	Lint                m_circleGoldLimit = 0;
	LMsgC2SCreatePlayConfig() :LMsgSC(MSG_C_2_S_CREATE_KUAISU_JU),m_roomType(0),m_playEnum(0),m_teamID(0),m_opt(0) {}
	MSGPACK_DEFINE_MAP(m_msgId,m_roomType, m_playEnum,m_playtype,m_teamID,m_opt, m_circleGoldLimit);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SCreatePlayConfig(); }
};



struct LMsgC2SFastPlay : public LMsgSC
{
	Lint				m_teamID;				//����ID
	LMsgC2SFastPlay() :LMsgSC(MSG_C_2_S_FAST_PLAY), m_teamID(0){}
	MSGPACK_DEFINE_MAP(m_msgId, m_teamID);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SFastPlay(); }
};
struct LMsgS2CFastPlayRet : public LMsgSC
{
	Lint				m_error;
	LMsgS2CFastPlayRet() :LMsgSC(MSG_S_2_C_FAST_PLAY), m_error(0) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_error);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CFastPlayRet(); }
};

struct LMsgC2SOptMember : public LMsgSC
{
	int		m_teamId = 0;	//����ID
	int		m_opt = 0;		// 1:��׼���� 2:�ܾ����� 3:�ܾ��뿪  4:ͬ���뿪 5:ɾ����� 
	int		m_userId = 0;	//���������id

	LMsgC2SOptMember() :LMsgSC(MSG_C_2_S_OPT_MEM) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_teamId, m_opt, m_userId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SOptMember(); }
};

struct LMsgS2COptMemRet : public LMsgSC
{
	int		m_error = 0;		//0:�ɹ� 3:���᲻���� 4:����Ĵ�����ID����ȷ  5:��Ա����Ĳ����벻��ʶ��
	int		m_teamId = 0;
	int		m_opt = 0;			// ��������    1:��׼���� 2:�ܾ����� 3:�ܾ��뿪  4:ͬ���뿪 5:ɾ����� 
	int		m_userId = 0;
	LMsgS2COptMemRet() :LMsgSC(MSG_S_2_C_OPT_MEM_RET) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_error, m_teamId, m_opt, m_userId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2COptMemRet(); }
};

struct LMsgC2SOptTeamInfo : public LMsgSC
{
	int		m_teamId = 0;	//����ID
	Lstring	m_teamName;
	Lstring m_boardInfo;

	LMsgC2SOptTeamInfo() :LMsgSC(MSG_C_2_S_OPT_TEAMINFO),m_teamId(0) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_teamId, m_teamName, m_boardInfo);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SOptTeamInfo(); }
};
struct LMsgS2CAdminInviteOtherRet : public LMsgSC
{
	Lint m_error;
	Lint m_teamID;
	Lstring m_strTeamName;

	LMsgS2CAdminInviteOtherRet() :LMsgSC(MSG_S_2_C_ADMIN_IVITE_RET), m_error(0),m_teamID(0) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_error);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CAdminInviteOtherRet(); }
};

struct LMsgC2SAdminInviteOther : public LMsgSC
{
	int		m_teamId;	//����ID
	Lint	m_userid;
	Lint	m_adminId;
	LMsgC2SAdminInviteOther() :LMsgSC(MSG_C_2_S_ADMIN_IVITE), m_teamId(0),m_userid(0),m_adminId(0) {}
	MSGPACK_DEFINE_MAP(m_msgId, m_teamId, m_userid, m_adminId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SAdminInviteOther(); }
};

////////////////////////////////////////////////////////////////////////////////////////
// ����һ����
//MSG_C_2_S_REQ_EXCHAGEGOLD = 344, // ����һ����
struct LMsgC2SReqExchageGold : public LMsgSC
{
	Lint		nToUserId = 0;
	Lint		nGoldNum = 0;
	LMsgC2SReqExchageGold() :LMsgSC(MSG_C_2_S_REQ_EXCHAGEGOLD) {}
	MSGPACK_DEFINE_MAP(m_msgId, nToUserId, nGoldNum);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SReqExchageGold(); }
};

// ����һ���� ���
//MSG_S_2_C_REQ_EXCHAGEGOLDD = 345, // ����һ���� ���
struct LMsgS2CReqExchageGold : public LMsgSC
{
	Lint nResCode = 0; //0 �ɹ��� 1 �һ���Ǯ������ 2 �Ҳ����ˣ� 3 ��������

	LMsgS2CReqExchageGold() :LMsgSC(MSG_S_2_C_REQ_EXCHAGEGOLDD) {}
	MSGPACK_DEFINE_MAP(m_msgId, nResCode);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CReqExchageGold(); }
};

// ��������һ���� ��¼
//MSG_C_2_S_REQ_EXCHAGEGOLD_LOG = 346, // ��������һ���� ��¼
struct LMsgC2SReqExchageGoldLog : public LMsgSC
{
	LMsgC2SReqExchageGoldLog() :LMsgSC(MSG_C_2_S_REQ_EXCHAGEGOLD_LOG) {}
	MSGPACK_DEFINE_MAP(m_msgId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SReqExchageGoldLog(); }
};

// ��ҵĲ�����־
struct SExchageGoldOptLog
{
	Llong lLogId = 0; //��־��id
	Lint nFromUserId = 0; //˭�����
	Lint nToUserId = 0; //��˭����
	Lint nGoldNum = 0; //���ն���
	Lstring sFromName = ""; //˭�����
	Lstring sToName = ""; //˭����
	Lint nOpt = 0; //0��δ���� 1�Ѿ����� 2 fromȡ������ 3 toȡ������
	Lint nReqTime = 0;		//�����ʱ�� 
	Lint nProTime = 0;		//�����ʱ�� 

	MSGPACK_DEFINE_MAP(lLogId, nFromUserId, nToUserId, nGoldNum, sFromName, sToName, nOpt, nReqTime, nProTime);
	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
};

//��Ҷһ���־ ֻ��10����¼
//MSG_S_2_C_EXCHAGEGOLD_OPT_LOG = 347, // ��Ҷһ���־
struct LMsgS2CExchageGoldOptLog : public LMsgSC
{
	std::vector<SExchageGoldOptLog> tAllExchageGoldOptLog;

	LMsgS2CExchageGoldOptLog() :LMsgSC(MSG_S_2_C_EXCHAGEGOLD_OPT_LOG) {}
	MSGPACK_DEFINE_MAP(m_msgId, tAllExchageGoldOptLog);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CExchageGoldOptLog(); }
};


// ȡ���һ�
//MSG_C_2_S_CANNCEL_EXCHAGEGOLD = 348, // ȡ���һ�
struct LMsgC2SCanncelExchageGold : public LMsgSC
{
	Llong		lLogId = 0;
	LMsgC2SCanncelExchageGold() :LMsgSC(MSG_C_2_S_CANNCEL_EXCHAGEGOLD) {}
	MSGPACK_DEFINE_MAP(m_msgId, lLogId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SCanncelExchageGold(); }
};

// ȡ���һ� ���
//MSG_S_2_C_CANNCEL_EXCHAGEGOLD = 349, // ȡ���һ� ���
struct LMsgS2CCanncelExchageGold : public LMsgSC
{
	Lint nResCode = 0; //0 �ɹ��� 1 ����id�� 2 ����ȡ���������Ѿ����һ��ȣ� 3 ��������

	LMsgS2CCanncelExchageGold() :LMsgSC(MSG_S_2_C_CANNCEL_EXCHAGEGOLD) {}
	MSGPACK_DEFINE_MAP(m_msgId, nResCode);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CCanncelExchageGold(); }
};


//MSG_S_2_C_EXCHAGEGOLD_OPTNOTICE = 352, // ���ս�Ҳ��� ֪ͨ
struct LMsgS2CExchageGoldOptNotice : public LMsgSC
{
	Lint nResCode = 0; //0 �Ѿ������գ� 1 ��������
	Lint nFromUserId = 0; //˭�����
	Lint nToUserId = 0; //��˭����
	Lint nGoldNum = 0; //���ն���
	Lstring sFromName = ""; //˭�����
	Lstring sToName = ""; //˭����

	LMsgS2CExchageGoldOptNotice() :LMsgSC(MSG_S_2_C_EXCHAGEGOLD_OPTNOTICE) {}
	MSGPACK_DEFINE_MAP(m_msgId, nResCode);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CExchageGoldOptNotice(); }
};

////////////////////////////////////////////////////////////////////////////////////////

// ������뷿��
//MSG_C_2_S_INVITE_INROOM = 350, // ������뷿��
struct LMsgC2SInviteInRoom : public LMsgSC
{
	Lint memberId = 0; //Ҫ�����˵�id
	LMsgC2SInviteInRoom() :LMsgSC(MSG_C_2_S_INVITE_INROOM) {}
	MSGPACK_DEFINE_MAP(m_msgId, memberId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgC2SInviteInRoom(); }
};

// �յ�  ������뷿��
//MSG_S_2_C_INVITE_INROOM = 351, // �յ� ������뷿��
struct LMsgS2CInviteInRoom : public LMsgSC
{
	Lint fromId = 0; //˭�����
	Lstring sFromName = ""; //˭���������
	Lint deskId = 0; //����id

	LMsgS2CInviteInRoom() :LMsgSC(MSG_S_2_C_INVITE_INROOM) {}
	MSGPACK_DEFINE_MAP(m_msgId, fromId, sFromName, deskId);

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack) { pack.pack(*this);	return true; }
	virtual bool Read(msgpack::object& obj) { obj.convert(*this); return true; }
	virtual LMsg* Clone() { return new LMsgS2CInviteInRoom(); }
};

//////////////////////////////////////////////////////////////////////////

#endif // _L_MSG_CT_H_