#ifndef _LVIP_LOG_H
#define _LVIP_LOG_H

#include "LTool.h"
#include "LBuff.h"
#include "GameDefine.h"

struct VipDeskLog
{
	Lstring	 m_videoId;//¼��id
	Ldouble	 m_gold[DESK_USER_COUNT];
	Ldouble	 m_cal_gold[DESK_USER_COUNT];
	Lint	 m_win[DESK_USER_COUNT];
	Lint	 m_zhuangPos;//��ׯ��λ��
	Lint	 m_time;//����ʱ��
	Lint	 m_angang[DESK_USER_COUNT];//����
	Lint	 m_mgang[DESK_USER_COUNT];//����
	Lint	 m_bomb[DESK_USER_COUNT];	//�����ߵķ�����Ϣ

	////////////////�㶫///////////////////

	Lint	 m_fanshu[DEFAULT_DESK_USER_COUNT];//����
	Lint	 m_baohu[DEFAULT_DESK_USER_COUNT];	//��������
	Lint	m_maNum[DEFAULT_DESK_USER_COUNT];	//��������
	Lint	m_ghostZimo[DEFAULT_DESK_USER_COUNT];	//�й�����
	Lint	m_noghostZimo[DEFAULT_DESK_USER_COUNT];	//�޹�����
	Lint	m_normalhu[DEFAULT_DESK_USER_COUNT];		//��ͨ��
	Lint    m_singleflow[DEFAULT_DESK_USER_COUNT];	//������

	/////////////////�㶫end//////////////
	VipDeskLog()
	{
		memset(m_gold, 0, sizeof(m_gold));
		memset(m_cal_gold, 0, sizeof(m_cal_gold));
		memset(m_win, 0, sizeof(m_win));
		memset(m_angang, 0, sizeof(m_angang));
		memset(m_mgang, 0, sizeof(m_mgang));
		memset(m_bomb, 0, sizeof(m_bomb));
		memset(m_fanshu, 0, sizeof(m_fanshu));
		memset(m_baohu, 0, sizeof(m_baohu));
		memset(m_maNum, 0, sizeof(m_maNum));
		memset(m_ghostZimo, 0, sizeof(m_ghostZimo));
		memset(m_noghostZimo, 0, sizeof(m_noghostZimo));
		memset(m_normalhu, 0, sizeof(m_normalhu));
		memset(m_singleflow, 0, sizeof(m_singleflow));
		m_zhuangPos = INVAILD_POS;
		m_time = 0;
		m_videoId = "";
	}
};

struct LVipLogItem
{
	Lstring			m_id;		//id
	Lint			m_time;		//ʱ��
	Lint			m_flag;		//�����淨
	Lint			m_subFlag;	//�������淨
	Lint			m_deskId;	//����id
	Lstring			m_secret;	//����
	Lint			m_maxCircle;//��Ȧ��
	Lint			m_curCircle;//��ǰȦ��
	Lint			m_posUserId[DESK_USER_COUNT];//0-3����λ���ϵ����id
	Lint			m_curZhuangPos;//��ǰׯ��
	Ldouble			m_score[4];// ����λ������Ļ���
	Ldouble			m_calScore[4];// ����λ������Ľ��
	Ldouble			m_iniScore[4];// ����λ������ĳ�ʼ����
	Lint			m_reset;//�Ƿ����
	std::vector<Lint> m_playtype;

	Lint			m_curMatchId = 0;	//����ID
	Lstring			m_MatchName;		//����������
	Lint			m_nCurStage = 1;	//�ڼ���
	Lint            m_nMaxMember = 0;   //����������
	int				m_nGroupID = 0;		//С��ID
	Lint            m_nForceFinishTime = 0;  //ǿ�ƽ���������ʱ��
	int             m_IntoTuoGuanRemainTime = 15000;    //δ�йܽ����й�״̬ʣ��ʱ��
	int             m_InTuoGuanCanChuPaiTime = 3000;   //�й�״̬�¿ɳ���ʱ��
	Lint            m_MatchStartTime = 0;   //������ʼʱ��
	int		        m_CreaterID = 0;			//�����˵�ID
	Lint            m_MatchType = 0;           //ѭ���� ר�� ������ �㶫������
	int             m_nInitualDifen = 500;               //��ʼ�׷ֱ���
	int             m_nDifenPerMinIncStep = 500;         //�׷�ÿ������������
	int             m_nDingJuInitualDifen = 1;      //���ֻ��ֽ׶εĵ׷�
	int             m_nCurDifen = 1;                     //Ĭ��Ϊ1

	std::vector<VipDeskLog*> m_log;//ÿһ�ѵļ�¼
	bool			m_free;

	Lint			m_iPlayerCapacity;				//��������������ܳ���DEFAULT_DESK_USER_COUNT
	Lint			m_playerNum;

	LVipLogItem();
	virtual  ~LVipLogItem();
	virtual  Lstring		ToString();
	virtual  Lstring        PlayTypeToStrint();

	virtual  void			FromString(const Lstring& str);
	virtual  Lstring        PlayTypeToString4RLog();

	int				GetMatchID()const { return m_curMatchId; }
	void			SetMatchID(int nid) { m_curMatchId = nid; }
};
#endif