#ifndef _L_CHARGE_H_
#define _L_CHARGE_H_

#include "LBuff.h"

//��������
enum CARD_TYPE
{
	CARD_TYPE_4 = 1,//4�ֿ�
	CARD_TYPE_8 = 2,//8�ֿ�
	CARD_TYPE_16 = 3,//16�ֿ�
	CARD_TYPE_32 = 4, //1�ֿ�
	CARD_TYPE_64 = 5, //2�ֿ�
};

enum FRIEND_ROOM_TYPE //���ѷ�����
{
	FRIEND_ROOM_TYPE_4 = 1,//4��
	FRIEND_ROOM_TYPE_8 = 2,//8��
	FRIEND_ROOM_TYPE_16 = 3,//16�ֿ�
};

enum CARDS_OPER_TYPE
{
	CARDS_OPER_TYPE_INIT = 0,//��������
	CARDS_OPER_TYPE_CHARGE = 1,//��ֵ�
	CARDS_OPER_TYPE_FREE_GIVE = 2,//�������
	CARDS_OPER_TYPE_CREATE_ROOM = 3,//��������۳�
	CARDS_OPER_TYPE_ACTIVE = 4,		//��ͷ���
	CARDS_OPER_TYPE_PURCHASE_GIVE = 5,	//�����������
	CARDS_OPER_TYPE_VIP_GIVE = 6,	//�������VIP��������
	CARDS_OPER_TYPE_INVITING = 7,		//��������ͷ���
	CARDS_OPER_TYPE_PLAYING = 8,	//����ƾ�����
	CARDS_OPER_TYPE_CHANGE2GOLD = 9,	//�һ����;

	CARDS_OPER_TYPE_ROOM_WIN_CARD = 104,        // ����Ӯ�Ľ��
	CARDS_OPER_TYPE_ROOM_FAIL_CARD = 105,        // ������Ľ��
	CARDS_OPER_TYPE_ROOM_COST_CARD = 106,        // ����۳�Ӯ�Ľ�� ���

	CARDS_OPER_TYPE_REQ_EXCHAGEGOLD = 107,      // ���ս�ҿۿ�
	CARDS_OPER_TYPE_CANNCEL_EXCHAGEGOLD = 108,      // ȡ�����ս�һ�ÿ�
	CARDS_OPER_TYPE_HTTP_REQ_EXCHAGEGOLD = 109,      // ��̨���ս�һ�ÿ�
	CARDS_OPER_TYPE_HTTP_CANNCEL_EXCHAGEGOLD = 110,      // ��̨ȡ�����ս�һ�ÿ�

	CARDS_OPER_TYPE_ROOM_WIN_CARD_EXADD = 111,        // ����Ӯ�Ľ�� ʣ���ۼ�
};

class LCharge
{
public:
	LCharge();
	~LCharge();

	void	Read(LBuff& buff);
	void	Write(LBuff& buff);
public:
	Lint	m_time;
	Lint	m_cardType;
	Ldouble	m_cardNum;
	Lint	m_type;
	Lint	m_userId;
	Lstring	m_admin;
	Ldouble	m_iTotalCardNum;
	Lint    m_teamId;
};
#endif