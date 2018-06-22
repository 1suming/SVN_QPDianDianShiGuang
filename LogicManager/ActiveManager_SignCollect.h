#pragma once
#include "LBase.h"
#include "LMsgS2S.h"
//2016��ʮһ�

extern void split(Lstring& s, Lstring& delim,std::vector< Lstring >* ret);

struct ActiveOutInfo
{
	Lint	m_activeType;
	Lint	m_drawChanceCount;	//ÿ���ܳ鼸��
	Lint	m_date;
	std::vector<Lint>	m_words;	//1~8
	Lint	m_word;
	Lstring	m_nickName;	//
	Lint	m_isUpdate;
	bool	m_isNewWord;
	bool	m_hadGift;

	ActiveOutInfo()
	{
		m_activeType = INVAILD_VAL;
		m_drawChanceCount = 0;
		m_date = 0;
		m_word = 0;
		m_isUpdate = false;
		m_isNewWord = false;
		m_hadGift = false;
	}
};

class ActiveMgr_SignCollect
{
public:
	enum ActiveType	//��Ҫָ��ֵ
	{
		ESign,
		ECollectCard,
		EMax,
	};

	enum ActiveCount
	{
		ECollectMax = 99,
	};

	ActiveMgr_SignCollect(){ ReSet();};
	~ActiveMgr_SignCollect(){};

	void ReSet();

	void SaveToDb(Lint userId);

	bool IsActiveOpen(Lint date);
	bool IsActiveOpen(ActiveType type, Lint date);

	bool IsActiveLoaded(ActiveType type);

	bool IsSignByDate(Lint date);	//@1@2���Ժϲ�

	bool IsLeftDrawByDate(Lint date);

	bool run(ActiveType type, Lint date, ActiveOutInfo & out);

	bool runSign(Lint date, ActiveOutInfo & out);

	bool runCollect(Lint date, ActiveOutInfo & out);

	void AddChance(Lint count);

	void DelChance(Lint count);

	//void AddWinInfo(Lint word);

	bool CheckFinalGift(ActiveType type);

	bool CheckFinalSignGift();

	bool CheckFinalCollectGift();

	std::vector<ActiveType> GetActiveTypes();

	void LoadData(ActiveType type, std::vector<LActiveSignCollectSS> &actives);

	std::vector<LActiveSignCollectSS> GetActive(ActiveType type);

	void SetLoaded(ActiveType type, bool isLoaded);
	bool GetLoaded(ActiveType type);
	//�������״̬
	void SetCollectUpdate(bool isUpdate);
	bool GetCollectUpdate();


private:
	std::vector<LActiveSignCollectSS> m_actives[EMax];		//�
	bool m_isLoaded[EMax];
};
 