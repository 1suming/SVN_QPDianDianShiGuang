#pragma once

// �����붨�� [2017-7-22 willing]
enum E_TEAM_ERROR_CODE
{
	ETEC_SUCCESS = 0,				// �ɹ� [2017-7-22 willing]
	ETEC_TEAM_NOT_EXIST = 1,		// ���᲻���� [2017-7-22 willing]
	ETEC_TEAM_MEM_NOT_EXIST = 2,	// �����Ա������ [2017-7-22 willing]
	ETEC_CANOT_CANCEL_JOIN = 3,		// ����ڹ����е�״̬�����������״̬�����ܳ������� [2017-7-22 willing]
	ETEC_CANOT_LEAVE_TEAM = 4,		// ����ڹ����е�״̬�����Ѿ�����״̬�����������뿪 [2017-7-22 willing]
	ETEC_JUSHU_ERROR = 5,			// ��������ʱ���ݵľ�������ȷ [2017-7-23 willing]
	ETEC_CARD_NOT_ENOUGH = 6,		// ���᷿������ [2017-7-23 willing]
	ETEC_TEAM_NOT_SAME = 7,			// ����ͬһ�����ᣬ���ܲ�ѯս�� [2017-7-24 willing]
	ETEC_QUERY_VIDEO_LOG_ERR = 8,	// ��ѯս��ʧ��[2017-7-24 willing]
	ETEC_PERDAY_CARD_LIMIT = 9,		// ÿ��ʹ�ù��᷿���������� [2017-7-24 willing]
	ETEC_JOIN_MAX_TEAM = 10,			// ���빤�������������� [2017-7-29 willing]
	ETEC_TEAM_MAX_COUNT = 11,		// �������Ĺ����Ա�Ѿ��ﵽ������ [2017-7-29 willing]
	ETEC_ALREADY_JOIN = 12,			// �Ѿ�����������˹��� [2017-7-29 willing]
	ETEC_TEAM_NORMAL = 13,			// �Ѿ��ǹ�����ʽ��Ա�ˣ������������ [2017-7-29 willing]
	ETEC_TEAM_LEAVE = 14,			// �Ѿ��������뿪������������� [2017-7-29 willing]
	ETEC_LEAVE_CREATE_DESK = 15,	// ���ǹ�����ʽ��Ա������ʹ�ù��᷿���������� [2017-7-29 willing]
	ETEC_IN_DESK_LEAVE = 16,		// �ڹ��������У������뿪���� [2017-8-5 willing]
	ETEC_IN_DESK = 17,				//��������ߣ������ٴ���������
	ETEC_TEAM_PLAY_CONFIG = 18,		//��������淨û������
	ETEC_TEAM_ADMIN_ERR = 19,		//���ǹ���Ĺ�����
	ETEC_TEAM_ADMIN_OPT_ERR = 20,	//��������
	ETEC_MEM_STATE_ERROR = 21,		// ��Ա״̬������
	ETEC_MEM_IN_MATCH = 22,			// ����Ѽ��������
	ETEC_DESK_OVER_LIMIT = 23,      // ��������������
	ETEC_NO_DESK = 24,              // û������
	ETEC_GOLD_NOT_LIMIT = 101,      // Ǯ������������
};

// ��̨HTTP��������붨�� [2017-7-25 willing]
enum E_HTTP_ERQUEST_ERROR_CODE
{
	EHEEC_SUCCESS,				// �ɹ� [2017-7-25 willing]
	EHEEC_PARAM_ERROR,			// �������� [2017-7-25 willing]
	EHEEC_TEAM_NAME,			// ���������ظ� [2017-7-25 willing]
	EHEEC_TEAM_NOT_EXIST,		// ���᲻���� [2017-7-25 willing]
	EHEEC_TEAM_CREATEID_ERR,	// ����Ĵ�����ID����ȷ [2017-7-25 willing]
	EHEEC_MEM_OPT_ERR,			// ��Ա����Ĳ����벻��ʶ�� [2017-7-25 willing]
	EHEEC_MEM_OPT_NOTALL_SUC,	// ��Ա����Ĳ���û��ȫ���ɹ� [2017-7-27 willing]
	EHEEC_CERATE_TEAM_MAX_COUNT,// �����Ĺ����Ѿ��ﵽ��������� [2017-7-29 willing]
	EHEEC_TEAM_MEM_COUNT,		// Ŀ�깤���Ѿ���Ա [2017-7-30 willing]
	EHEEC_IS_IN_TEAM,			// ������Ѿ��ǹ����Ա [2017-8-5 willing]
	EHEEC_NOT_FOUND_USER,		// Ŀ����Ҳ����� [2017-8-5 willing]
	EHEEC_JOIN_TEAM_MAX,		// ������ֲ������ﵽ����
	EHEEC_NOT_HAS_MANAGER,		// ��������ʱû��managerID
	EHEEC_NOT_HAS_COUNTY,       // ��������ʱû��countyId
	EHEEC_NOT_HAS_DESK_CNT_LIMIT, //��������ʱû��deskCntLimit
	EHEEC_NOT_HAS_COUNTY_CIRCLE_GOLD_LIMIT, //��������ʱû��circleGoldLimit
};

// �����Ա�Թ���Ĳ������� [2017-8-9 willing]
enum E_TEAM_MEM_OPT_TYPE
{
	ETMOT_REQ_JOIN_TEAM,		// ������빤�� [2017-8-9 willing]
	ETMOT_REQ_LEAVE_TEAM,		// �����뿪���� [2017-8-9 willing]
	ETMOT_ALLOW_JOIN_TEAM,		// �᳤ͬ���˼������� [2017-8-9 willing]
	ETMOT_REJECT_JOIN_TEAM,		// �᳤�ܾ��˳�Ա�ļ������� [2017-8-9 willing]
	ETMOT_REJECT_LEAVE_TEAM,	// �᳤�ܾ��������뿪���� [2017-8-9 willing]
	ETMOT_ALLOW_LEAVE_TEAM,		// �᳤ͬ���������뿪���� [2017-8-9 willing]
	ETMOT_RMV_FROM_TEAM,		// �᳤���������˹��� [2017-8-9 willing]
	ETMOT_TEAM_ADD_MEM,			// �᳤���������� [2017-8-10 willing]
	ETMOT_CANCEL_JOIN_TEAM,		// ��ҳ����� ���빤������ [2017-8-10 willing]
};

//��̨�޸ľ�����ֵ
enum E_TEAM_INFO_OPT_TYPE
{
	ETMOT_all,		// ȫ���޸�
	ETMOT_playTypes,		// �޸ľ�����ֵ
	ETMOT_deskCntLimit,		// �޸ľ�����ֵ
	ETMOT_boardInfo,	// �޸ľ�����ֵ
	ETMOT_teamName,		// �޸ľ�����ֵ
	ETMOT_playTypes_deskCntLimit,		// �޸ľ�����ֵ
	ETMOT_deskProportionate,// �޸ľ�����ֵ
};