#ifndef _CARD_MANAGER_H_
#define _CARD_MANAGER_H_

#include "gr_card_wakeng.h"
#include "LVideo.h"

#define CARD_COUNT_WAKENG	(52)

//////////////////////////////////////////////////////////////////////////
class CardStyle_Shanxi_Wakeng
{
public:
	CardStyle_Shanxi_Wakeng();
	~CardStyle_Shanxi_Wakeng();

	void        SortCard(CardVec_Wakeng& vce);//���ư���С����;
	void        SortCardLaiziFront(CardVec_Wakeng& vce);//��ӷ�ǰ��;
	void        SortCardLaiziInsert(CardVec_Wakeng& vce);//����滻��;

	void        EarseCard(CardVec_Wakeng& des,CardVec_Wakeng& src);//ɾ��des������src����;
	void		CheckCardStyle(CardVec_Wakeng const& src,CardStyleNumberVec& style,bool last);//������з�������;
	bool		CheckCardStyle(CardVec_Wakeng const& src,BYTE style,BYTE& cardNumber);//��ĳ����������鵱ǰ����;
	int			CanBig(DWORD dsCount,BYTE dsStyle,BYTE dsNumber,CardStyleNumberVec& style, bool isUseBomb);
	bool        CheckHasBomb(CardVec_Wakeng const& Src, Lint* nBombCard = NULL);//�鿴�������Ƿ���ը��;

	void		GetSingleDoubleThreeFour(CardVec_Wakeng const& vec,CardVec_Wakeng& single,CardVec_Wakeng& duizi,CardVec_Wakeng& three,CardVec_Wakeng& four);
	void		GetJokeNumber2(CardVec_Wakeng const& vec,CardVec_Wakeng& joke,CardVec_Wakeng& number2,CardVec_Wakeng& other);
	void		GetLaizi(CardVec_Wakeng const& vec,CardVec_Wakeng& laizi,CardVec_Wakeng& other);
	void		GetBombLaiziJokeNumber2(CardVec_Wakeng const& vec,CardVec_Wakeng& bomb,CardVec_Wakeng& laizi,CardVec_Wakeng& joke,CardVec_Wakeng& number2,CardVec_Wakeng& other);
	bool		CheckContinuous(std::vector<BYTE>& vec);
	//bool        HasRocket(CardVec_Wakeng const& vec);

	bool        IsBiZhua(CardVec_Wakeng const& vec);
	bool        IsBiDao(CardVec_Wakeng const& vec);

	int			GetCardScore(CardVec_Wakeng& vec);
private:

	//bool        IsRocket(CardVec_Wakeng const& vec,BYTE& cardType);	//�Ƿ���˫��;
	bool        IsDouble(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//����;
	//bool        IsBomb1(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//����ը��;
	bool        IsBomb2(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//��Ӳը��;
	//bool        IsBomb3(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//��ը��;
	//bool        IsBombToSingle(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);	//�Ĵ���;
	//bool        IsBombToDouble(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);	//�Ĵ���;
	bool        IsThree(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//����;
	//bool        IsThreeToSingle(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//������;
	//bool		IsThreeToDouble(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//������;
	//bool        IsThreeList(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);		//�ɻ�;
	//bool        IsThreeListToSingle(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//�ɻ�����;
	//bool		IsThreeListToDouble(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//�ɻ�����;
	bool        IsSingleList(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//�Ƿ��ǵ�˳;
	bool        IsDoubleList(CardVec_Wakeng const& vec,CardVec_Wakeng const& laizi,BYTE& cardType);//�Ƿ���˫˳;
public:
	static CardStyle_Shanxi_Wakeng* Instance();
};

///////////////////////////////////////////////////////////////////////////
//���Ƶ���;
class CardDeal_Shanxi_Wakeng
{
public:
	CardDeal_Shanxi_Wakeng();
	~CardDeal_Shanxi_Wakeng();

	void		ClearLaizi();
	void		SetLaizi(BYTE bCardNumber);
	void		DealCard(CardVec_Wakeng& vec1, CardVec_Wakeng& vec2, CardVec_Wakeng& vec3, Lint cardCount, CardValue* tiaopai = NULL, Lint nLenth = 0);
	void        GetLeftCard(CardVec_Wakeng& leftCard);
	void        SS(Lint nCardPos, Lint nColor, Lint nNumber);         // ���Ƹ�������;
	void        TiaoPai(CardValue tiaopai[], Lint nLenth);                                            // ���ƺ���;

private:
	void		RandCard();

private:
	Card_Wakeng		m_oCard[CARD_COUNT_WAKENG];
	CardVec_Wakeng		m_pCard;
};


////////////////////////////////////////////////////////////////////////////////////////////
class CardHelp_Shanxi_Wakeng
{
public:
	CardHelp_Shanxi_Wakeng();
	~CardHelp_Shanxi_Wakeng();

	//��ʾ;
	std::vector<std::vector<Lint>> HelpMe(Lint cardMode, Lint cardType, Lint cardCount, CardVec_Wakeng& handcard, std::vector<Lint>& m_curHelpUnusable, bool baojing, bool isUseBomb);


	void	CheckOne(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,std::vector<std::vector<Lint>>& ret,bool baojing);
	void	CheckTwo(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f, std::vector<std::vector<Lint>>& ret);
	//void	CheckThreeToTwo(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f, std::vector<std::vector<Lint>>& ret);
	//void	CheckThreeFlyToTwo(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,CardVec_Wakeng& handcard,Lint count, std::vector<std::vector<Lint>>& ret);
	void	CheckOneList(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,Lint count, std::vector<std::vector<Lint>>& ret);
	void	CheckTwoList(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,Lint count, std::vector<std::vector<Lint>>& ret);
	//void    CheckThreeToOne(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f, std::vector<std::vector<Lint>>& ret);
	//void    CheckFourToTwo(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f, std::vector<std::vector<Lint>>& ret);
	void    CheckThree(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f, std::vector<std::vector<Lint>>& ret);
	//void    CheckThreeList(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,CardVec_Wakeng& handcard,Lint count, std::vector<std::vector<Lint>>& ret);
	//void    CheckThreeFlyToSingle(Lint cardType,CardVec_Wakeng& o,CardVec_Wakeng& t,CardVec_Wakeng& th, CardVec_Wakeng& f,CardVec_Wakeng& handcard,Lint count, std::vector<std::vector<Lint>>& ret);
};
#endif