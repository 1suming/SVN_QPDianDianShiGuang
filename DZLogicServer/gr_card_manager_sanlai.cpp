
#include "gr_card_manager_sanlai.h"
//#include "gr_card_manager.h"
//#include "Tool.h"
#include "LTool.h"
#include "LLog.h"
#include "Config.h"

extern bool sort_fun(Card* p1,Card* p2);

extern bool sort_fun_by_laizi(Card* p1,Card* p2);

extern bool sort_byte(BYTE b1,BYTE b2);


//////////////////////////////////////////////////////////////////////////
CardDeal_3Lai::CardDeal_3Lai()
{
	m_nSize = CARD_COUNT_DDZ;
	m_pCard.resize(m_nSize);
	for(int i = 0 ; i < CARD_COUNT_DDZ -2; ++i)
	{
		m_oCard[i].SetColorAndNumber(i%4+1,i/4+1);
	}
	m_oCard[52].SetColorAndNumber(card_color_type_null,card_number_type_SJ);
	m_oCard[53].SetColorAndNumber(card_color_type_null,card_number_type_BJ);
}

CardDeal_3Lai::~CardDeal_3Lai()
{

}

void CardDeal_3Lai::SS(Lint nCardPos, Lint nColor, Lint nNumber)
{
	Lint nLenth = m_pCard.size();
	for (int iii = nCardPos; iii < nLenth; iii++)
	{
		if (m_pCard[iii]->GetNumberType() == nNumber && m_pCard[iii]->GetColorType() == nColor)
		{
			std::swap(m_pCard[nCardPos], m_pCard[iii]);
			break;
		}
	}
}

void CardDeal_3Lai::TiaoPai(CardValue tiaopai[], Lint nLenth)
{
//	card_color_type_squal,//方块		card_color_type_floor,//梅花		card_color_type_red,//红桃		card_color_type_black,//黑桃;
//  card_number_type_null = 0,		card_number_type_3,		card_number_type_4,		card_number_type_5,		card_number_type_6,		card_number_type_7,		card_number_type_8,		card_number_type_9,		
//	card_number_type_10,		card_number_type_J,		card_number_type_Q,		card_number_type_K,		card_number_type_A,		card_number_type_2,		card_number_type_SJ,		card_number_type_BJ,
	// 第1个人;
	Lint i = 0;
	for (i = 0; i < nLenth; ++i)
	{
		Lint num = (tiaopai[i].m_number - 2) > 0 ? (tiaopai[i].m_number - 2) : (tiaopai[i].m_number + 11);
		SS(i, tiaopai[i].m_color, num);
	}
	//SS(++i, card_color_type_black, card_number_type_3);
	//SS(++i, card_color_type_floor, card_number_type_5); SS(++i, card_color_type_red, card_number_type_5); SS(++i, card_color_type_squal, card_number_type_5);
	//SS(++i, card_color_type_floor, card_number_type_8); SS(++i, card_color_type_red, card_number_type_8); SS(++i, card_color_type_squal, card_number_type_8);
	//SS(++i, card_color_type_floor, card_number_type_9); SS(++i, card_color_type_red, card_number_type_9); SS(++i, card_color_type_squal, card_number_type_9);
	//SS(++i, card_color_type_black, card_number_type_K); SS(++i, card_color_type_floor, card_number_type_K); SS(++i, card_color_type_red, card_number_type_A); SS(++i, card_color_type_floor, card_number_type_A);
	// 第3个人;
}

void CardDeal_3Lai::DealCard(CardVec * receivers, Lint receiversLength, CardValue* changeCard, Lint nChangeLength)
{
#if USEASSERT
	assert(receivers != NULL);
#endif

	CardVec& vec1 = *receivers; 
	CardVec& vec2 = *(receivers+1); 
	CardVec& vec3 = *(receivers+2);
  
	CardValue* tiaopai = changeCard; 

	RandCard();

	// 是否需要调牌;
	if (nChangeLength >0 && nChangeLength <= 54)
	{
		TiaoPai(tiaopai, nChangeLength);
	}

	int n = 0;
	int nSize = 17;
	vec1.insert(vec1.end(), m_pCard.begin() + n, m_pCard.begin() + n + nSize);
	n += nSize;
	vec2.insert(vec2.end(), m_pCard.begin() + n, m_pCard.begin() + n + nSize);
	n += nSize;
	vec3.insert(vec3.end(), m_pCard.begin() + n, m_pCard.begin() + n + nSize);
	n += nSize;
}

void CardDeal_3Lai::GetLeftCard(CardVec& leftCard)
{
	leftCard.insert(leftCard.end(), m_pCard.begin() + 51, m_pCard.end());
}

void CardDeal_3Lai::SetLaizi(BYTE bCardNumber)
{
	for(int i = 0 ; i < m_nSize; ++i)
	{
		if(m_oCard[i].GetNumberType() == bCardNumber)
		{
			m_oCard[i].SetLaizi(true);
		}
	}
}

void CardDeal_3Lai::ClearLaizi()
{
	for(int i = 0 ; i < m_nSize; ++i)
	{
		if(m_oCard[i].GetLaizi())
		{
			m_oCard[i].SetLaizi(false);
		}
	}
}

void CardDeal_3Lai::RandCard_15cards()
{
	m_pCard.clear();
	for(int i = 0 ; i < CARD_COUNT_DDZ-14; ++i)
	{
		m_pCard.push_back(&m_oCard[i]);
	}

	m_pCard.push_back(&m_oCard[CARD_COUNT_DDZ-11]);
	m_pCard.push_back(&m_oCard[CARD_COUNT_DDZ-12]);
	m_pCard.push_back(&m_oCard[CARD_COUNT_DDZ-13]);
	m_pCard.push_back(&m_oCard[CARD_COUNT_DDZ-3]);
	m_pCard.push_back(&m_oCard[CARD_COUNT_DDZ-7]);

	int seed = L_Rand(100,120);
	for(int i = 0 ; i < seed; ++i)
	{
		int seed1 = L_Rand(0,m_pCard.size()-1);
		int seed2 = L_Rand(0,m_pCard.size()-1);
		Card* tmp = m_pCard[seed1];
		m_pCard[seed1] = m_pCard[seed2];
		m_pCard[seed2] = tmp;
	}

	seed = L_Rand(0,m_pCard.size()-1);
	CardVec tmp(m_pCard.begin(),m_pCard.begin()+seed);
	tmp.insert(tmp.end(),m_pCard.begin()+seed,m_pCard.end());
	m_pCard = tmp;
}

void CardDeal_3Lai::RandCard()
{
	m_pCard.clear();
	for(int i = 0 ; i < m_nSize; ++i)
	{
		m_pCard.push_back(&m_oCard[i]);
	}

	int seed = L_Rand(100,120);
	for(int i = 0 ; i < seed; ++i)
	{
		int seed1 = L_Rand(0,m_pCard.size()-1);
		int seed2 = L_Rand(0,m_pCard.size()-1);
		Card* tmp = m_pCard[seed1];
		m_pCard[seed1] = m_pCard[seed2];
		m_pCard[seed2] = tmp;
	}
	seed = L_Rand(0,m_pCard.size()-1);
	CardVec tmp(m_pCard.begin(),m_pCard.begin()+seed);
	tmp.insert(tmp.end(),m_pCard.begin()+seed,m_pCard.end());

	m_pCard = tmp;
}

//////////////////////////////////////////////////////////////////////////
CardStyle_3Lai::CardStyle_3Lai()
{
}

CardStyle_3Lai::~CardStyle_3Lai()
{

}

void CardStyle_3Lai::SortCard(CardVec& vec)
{
	std::sort(vec.begin(),vec.end(),sort_fun);
}

void CardStyle_3Lai::SortCardLaiziFront(CardVec& vec)
{
	std::sort(vec.begin(),vec.end(),sort_fun);

	CardVec Normal,Laizi;
	for(DWORD i = 0 ; i < vec.size(); ++i)
	{
		if(vec[i]->GetLaizi())
		{
			Laizi.push_back(vec[i]);
		}
		else
		{
			Normal.push_back(vec[i]);
		}
	}
	Laizi.insert(Laizi.end(),Normal.begin(),Normal.end());
	vec.swap(Laizi);
}

void CardStyle_3Lai::SortCardLaiziInsert(CardVec& vec)
{
	std::sort(vec.begin(),vec.end(),sort_fun_by_laizi);
}

void CardStyle_3Lai::EarseCard(CardVec& des,CardVec& src)
{
	for(CardVec::iterator sit = src.begin();sit != src.end();++sit)
	{
		for(CardVec::iterator dit = des.begin();dit != des.end();)
		{
			if(*sit == *dit)
			{
				dit = des.erase(dit);
				break;
			}
			else
			{
				++dit;
			}
		}
	}
}

int CardStyle_3Lai::CanBig(DWORD dsCount,BYTE bsStyle ,BYTE bsNumber,CardStyleNumberVec& style)
{
	for(Lint i = style.size()-1 ; i >= 0; --i)
	{
		BYTE ddStyle = style[i].m_bCardStyle;
		BYTE ddNumber = style[i].m_bCardNumber;
		BYTE ddCount = style[i].m_bCardCount;
		if(bsStyle == card_style_error && ddStyle != card_style_error)
		{
			return i;
		}

		if (ddStyle >= card_style_bomb3 && ddStyle <= card_style_rocket )
		{
			if ( !(bsStyle >= card_style_bomb3 && bsStyle <= card_style_rocket ) )
			{
				return i;
			}
			else if (ddStyle > bsStyle)
			{
				return i;
			}
		}
		if (bsStyle == ddStyle && ddNumber > bsNumber && ddCount == dsCount)
		{
			return i;
		}
	}

	return -1;
}

bool CardStyle_3Lai::CheckHasBomb(CardVec const& Src, Lint* pBombCard)
{
	if (Src.size() < 4)
	{
		return false;
	}
	CardVec one,duizi,three,four;
	GetSingleDoubleThreeFour(Src,one,duizi,three,four);

	if (four.size() > 0)
	{
		if (pBombCard != NULL)
		{
			*pBombCard = four[0]->GetNumberType();
		}
		return true;
	}
	return false;   //不允许癞子炸弹;
}

bool CardStyle_3Lai::CheckHasBomb_laizi(CardVec const& Src,CardVec const& laizi, Lint* pBombCard)
{
	size_t laiziSize = laizi.size();
	size_t normalSize = Src.size();
	//只处理六张牌的情况
	if(laiziSize + normalSize != 6) return false;

	CardVec one,duizi,three,four;
	GetSingleDoubleThreeFour(Src,one,duizi,three,four);

	if (four.size() > 0) //四张
	{
		*pBombCard = four[0]->GetNumberType();
		return true;
	}

	if(three.size() > 0) //三张所有情况
	{
		if(laiziSize > 0)
		{
			*pBombCard = three[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(three[0]->GetNumberType());
			return true;
		}
		else
		{
			return false;
		}
	}

	if(duizi.size() > 0)	//对子
	{
		Lint twoType = duizi[0]->GetNumberType();
		if(laiziSize < 2)
		{
			return false;
		}
		else if(laiziSize == 2)	//只能对子找
		{
			*pBombCard = twoType;
			laizi[0]->SetLaiziNumber(twoType);
			laizi[1]->SetLaiziNumber(twoType);
			return true;
		}
		else if(laiziSize > 2) //癞子3~4个
		{
			if(one.size() > 0)
			{
				Lint oneType = card_number_type_null;
				for(size_t i = 0; i<one.size(); ++i)
				{
					if(one[i]->GetNumberType() <= card_number_type_2)
					{
						oneType = one[i]->GetNumberType();
						break;
					}
					
				}

				if(oneType > twoType)
				{
					*pBombCard = oneType;
					laizi[0]->SetLaiziNumber(oneType);
					laizi[1]->SetLaiziNumber(oneType);
					laizi[2]->SetLaiziNumber(oneType);
					return true;
				}
				else
				{
					*pBombCard = twoType;
					laizi[0]->SetLaiziNumber(twoType);
					laizi[1]->SetLaiziNumber(twoType);
					return true;
				}
			}
			else
			{
				
				*pBombCard = twoType;
				laizi[0]->SetLaiziNumber(twoType);
				laizi[1]->SetLaiziNumber(twoType);
				return true;
			}
		}
	}

	if(one.size() > 0)
	{
		if(laiziSize < 3)
		{
			return false;
		}

		Lint oneType = card_number_type_null;
		for(size_t i = 0; i<one.size(); ++i)
		{
			if(one[i]->GetNumberType() <= card_number_type_2)
			{
				oneType = one[i]->GetNumberType();
				break;
			}
		}

		if(oneType == card_number_type_null)
		{
			return false;
		}

		*pBombCard = oneType;
		laizi[0]->SetLaiziNumber(oneType);
		laizi[1]->SetLaiziNumber(oneType);
		laizi[2]->SetLaiziNumber(oneType);
		return true;
	}
	
	assert(false);
	return false;
}

void CardStyle_3Lai::CheckCardStyle(CardVec const& Src, CardStyleNumberVec& style,bool last, Lint nObjCardStyle)
{
	CardVec Normal,Laizi;
	GetLaizi(Src,Laizi,Normal);

	CardStyleNumber node;
	node.m_bCardStyle = card_style_error;
	node.m_bCardNumber = card_number_type_null;
	node.m_bCardCount = BYTE(Src.size());
	switch(Src.size())
	{
	case 1:
		node.m_bCardNumber = Src[0]->GetNumberType();
		node.m_bCardStyle = card_style_single;
		style.push_back(node);
		break;
	case 2:
		if (IsRocket(Normal,Laizi, node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_rocket;
			style.push_back(node);
			break;
		}
		if( (card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double) && IsDouble(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double;
			style.push_back(node);
			break;
		}
		break;
	case 3:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three) && IsThree(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three;
			style.push_back(node);
		}
		break;
	case 4:
		if(IsBomb1(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_bomb1;
			style.push_back(node);
			break;
		}
		if(IsBomb2(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_bomb2;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_single) && IsThreeToSingle(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_single;
			style.push_back(node);
		}
		if(IsBomb3(Normal,Laizi,node.m_bCardNumber))
		{
		 	node.m_bCardStyle = card_style_bomb3;
		 	style.push_back(node);
		}
		//连子判断;
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_single_list;
		//	style.push_back(node);
		//	break;
		//}
		//双联;
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_double_list;
		//	style.push_back(node);
		//	//break;
		//}
		break;
	case 5:
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_super_bomb) && IsSuperBomb(Normal, Laizi, node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_super_bomb;
		//	style.push_back(node);
		//	break;
		//}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_double) && IsThreeToDouble(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_double;
			style.push_back(node);
			break;
		}
		break;
	case 6:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			//break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
		}
		Lint nBombCard;
		if ((card_style_error == nObjCardStyle || nObjCardStyle == card_style_four2) && CheckHasBomb_laizi(Normal, Laizi, &nBombCard) && !HasRocket(Normal)) // 检查四带二;
		{
			node.m_bCardStyle = card_style_four2;
			node.m_bCardNumber = nBombCard;
			style.push_back(node);
			break;
		}
		break;
	case 7:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		break;
	case 8:
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_bomb_double) && IsDoubleBomb(Normal, Laizi, node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_bomb_double;
		//	style.push_back(node);
		//	break;
		//}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_single) && IsThreeListToSingle(Normal,Laizi,node.m_bCardNumber))  //8张飞机三带单;
		{
			node.m_bCardStyle = card_style_three_list_single;
			style.push_back(node);
			break;
		}
		//if(IsSuperRocket(Normal,Laizi,node.m_bCardNumber))
		//{
		//	node.m_bCardNumber = card_style_supper_rocket;
		//	style.push_back(node);
		//	break;
		//}
		break;
	case 9:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
			break;
		}
		break;
	case 10:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_double) && IsThreeListToDouble(Normal,Laizi,node.m_bCardNumber))  //10张的三带一定是三带对;
		{
			node.m_bCardStyle = card_style_three_list_double;
			style.push_back(node);
			break;
		}
		break;
	case 11:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		break;
	case 12:
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_bomb_double) && IsDoubleBomb(Normal, Laizi, node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_bomb_double;
		//	style.push_back(node);
		//	break;
		//}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_single_list) && IsSingleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_single_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))  //12张牌可以同时是四飞机或者三飞机带单,以四飞机优先;
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_single) && IsThreeListToSingle(Normal,Laizi,node.m_bCardNumber))  //12张飞机三代单;
		{
			node.m_bCardStyle = card_style_three_list_single;
			style.push_back(node);
		}
		break;
	case 13:
		break;
	case 14:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		break;
	case 15:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_double) && IsThreeListToDouble(Normal,Laizi,node.m_bCardNumber))  //15张三带一定三带对;
		{
			node.m_bCardStyle = card_style_three_list_double;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
			break;
		}
		break;
	case 16:
		//if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_bomb_double) && IsDoubleBomb(Normal, Laizi, node.m_bCardNumber))
		//{
		//	node.m_bCardStyle = card_style_bomb_double;
		//	style.push_back(node);
		//	break;
		//}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_single) && IsThreeListToSingle(Normal,Laizi,node.m_bCardNumber))  //16张三带一定三带单;
		{
			node.m_bCardStyle = card_style_three_list_single;
			style.push_back(node);
			break;
		}
		break;
	case 17:
		break;
	case 18:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
			break;
		}
		break;
	case 19:
		break;
	case 20:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_double_list) && IsDoubleList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_double_list;
			style.push_back(node);
			break;
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_double) && IsThreeListToDouble(Normal,Laizi,node.m_bCardNumber))   //20张飞机可以4连飞机三代对，或者5连飞机三带单;
		{
			node.m_bCardStyle = card_style_three_list_double;
			style.push_back(node);
		}
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list_single) && IsThreeListToSingle(Normal,Laizi,node.m_bCardNumber))  //20张三带定三带单;
		{
			node.m_bCardStyle = card_style_three_list_single;
			style.push_back(node);
			break;
		}
		break;
	case 21:
		if((card_style_error == nObjCardStyle ||nObjCardStyle == card_style_three_list) && IsThreeList(Normal,Laizi,node.m_bCardNumber))
		{
			node.m_bCardStyle = card_style_three_list;
			style.push_back(node);
			break;
		}
		break;
	}
}

bool CardStyle_3Lai::CheckCardStyle(CardVec const& Src,BYTE style,BYTE& cardNumber)
{
	CardVec Normal,Laizi;
	GetLaizi(Src,Laizi,Normal);

	std::size_t len = Src.size();
	cardNumber = card_number_type_null;
	switch(style)
	{
	case card_style_single:
		if(len == 1)
		{
			cardNumber = Src[0]->GetNumberType();
			return true;
		}
		return false;
	case card_style_rocket:
		if(len == 2 && IsRocket(Src,Laizi, cardNumber))
		{
			return true;
		}
		return false;
	case card_style_double:
		if(len == 2 && IsDouble(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_three:
		if(len == 3 && IsThree(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_bomb1:
		if(len == 4 && IsBomb1(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_bomb2:
		if(len == 4 && IsBomb2(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_bomb3:
		if(len == 4 && IsBomb3(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_three_single:
		if(len == 4 && IsThreeToSingle(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_three_double:
		if(len == 5 && IsThreeToDouble(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_single_list:
		if(len >= 5 && len <= 12 && IsSingleList(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_double_list:
		if(len/2 >= 3 && len%2 == 0 && IsDoubleList(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_three_list_single:
		if(len%4 == 0 && len/4 >= 2 && IsThreeListToSingle(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_three_list_double:
		if(len%5 == 0 && len/5 >= 2 && IsThreeListToDouble(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_bomb_and_single:
		if(len == 6 && IsBombToSingle(Normal,Laizi,cardNumber))
		{
			return true;
		}
		return false;
	case card_style_bomb_and_double:
		if(len == 8 && IsBombToDouble(Normal,Laizi,cardNumber))
		{
			return true;
		}
	}
	return false;
}

//OK;
bool CardStyle_3Lai::IsRocket(CardVec const& vec,CardVec const& laizi, BYTE& cardNumber)	//是否是火箭;
{
	if(vec.size() == 2 && vec[0]->GetNumberType() == card_number_type_BJ && vec[1]->GetNumberType() == card_number_type_SJ)
	{
		cardNumber = card_number_type_BJ;
		return true;
	}

	return false;
}

//OK;
bool CardStyle_3Lai::IsSuperRocket(CardVec const& vec, CardVec const& laizi, BYTE& cardNumber)	//是否是火箭;
{
#if USEASSERT
	assert(vec.size() >= 2);
#endif

	if(laizi.size() == 1 && vec[0]->GetNumberType() == card_number_type_BJ && vec[1]->GetNumberType() == card_number_type_SJ)
	{
		cardNumber = card_number_type_BJ;
		laizi[0]->SetLaiziNumber(cardNumber);
		return true;
	}

	return false;
}

//OK;
bool CardStyle_3Lai::IsDouble(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//是否是对子;
{
	if(laizi.size() == 0)
	{
		if(vec[0]->GetNumberType() == vec[1]->GetNumberType())
		{
			cardNumber = vec[0]->GetNumberType(); 
			return true;
		}
	}
	else if(laizi.size() == 1 && vec[0]->GetNumberType() <= card_number_type_2)
	{
		cardNumber = vec[0]->GetNumberType(); 
		laizi[0]->SetLaiziNumber(cardNumber);
		return true;
	}
	else if(laizi.size() == 2)
	{
		cardNumber = laizi[0]->GetNumberType(); 
		return true;
	}
	return false;
}

//OK
bool CardStyle_3Lai::IsBomb1(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//纯软炸弹;
{
	if(laizi.size() == 4)
	{
		cardNumber = laizi[0]->GetNumberType();
		return true;
	}
	return false;
}

//OK
bool CardStyle_3Lai::IsBomb2(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//纯硬炸弹;
{
	if(vec.size() == 4 && vec[0]->GetNumberType() == vec[3]->GetNumberType())
	{
		cardNumber = vec[0]->GetNumberType();
		return true;
	}
	return false;
}

//OK
bool CardStyle_3Lai::IsBomb3(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//软炸弹;
{
	if (vec.size()>0 && (vec[0]->GetNumberType() == card_number_type_BJ || vec[0]->GetNumberType() == card_number_type_SJ ))
	{
		return false;
	}
	if(laizi.size() == 1)
	{
		if(vec[0]->GetNumberType() == vec[2]->GetNumberType())
		{
			cardNumber = vec[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(cardNumber);
			return true;
		}
	}
	else if(laizi.size() == 2)
	{
		if(vec[0]->GetNumberType() == vec[1]->GetNumberType())
		{
			cardNumber = vec[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(cardNumber);
			laizi[1]->SetLaiziNumber(cardNumber);
			return true;
		}
	}
	else if(laizi.size() == 3)
	{
		cardNumber = vec[0]->GetNumberType();
		laizi[0]->SetLaiziNumber(cardNumber);
		laizi[1]->SetLaiziNumber(cardNumber);
		laizi[2]->SetLaiziNumber(cardNumber);
		return true;
	}
	return false;
}



bool CardStyle_3Lai::IsSuperBomb(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//软炸弹;
{
	if(laizi.size() == 1)
	{
		if(vec.size() == 4 && vec[0]->GetNumberType() == vec[3]->GetNumberType())
		{
			cardNumber = vec[0]->GetNumberType();
			return true;
		}
	}

	return false;
}


bool CardStyle_3Lai::IsDoubleBomb(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)
{
#if USEASSERT
	assert(vec.size() > 0);
#endif

	if(vec[0]->GetNumberType() > card_number_type_A)
	{
		return false;
	}

	if(laizi.size() == 1)
	{
		if(vec.size() == 7)
		{
			if(vec[6]->GetNumberType() == vec[3]->GetNumberType() && 
				vec[2]->GetNumberType() == vec[0]->GetNumberType() &&
				vec[3]->GetNumberType() + 1 == vec[2]->GetNumberType())
			{
				laizi[0]->SetLaiziNumber(vec[0]->GetNumberType());

				cardNumber = vec[0]->GetNumberType();
				return true;
			}

			if(vec[6]->GetNumberType() == vec[4]->GetNumberType() &&
				vec[3]->GetNumberType() == vec[0]->GetNumberType() &&
				vec[4]->GetNumberType() + 1 == vec[3]->GetNumberType())
			{
				laizi[0]->SetLaiziNumber(vec[6]->GetNumberType());

				cardNumber = vec[0]->GetNumberType();
				return true;
			}
		}
		else if(vec.size() == 11)	//3连炸
		{
			CardVec one, two, three, four;
			GetSingleDoubleThreeFour(vec, one, two, three, four);
			if(four.size()/4 < 2) return false;	//2
			if(four.size()/4 + three.size()/3 < 3) return false;	//3
			CardVec normal, lai;
			normal.push_back(four[3]);
			normal.push_back(four[4]);
			normal.push_back(three[0]);
			SortCard(normal);
			bool is = IsSingleList(normal, lai, cardNumber);
			if(is)
			{
				laizi[0]->SetLaiziNumber(three[0]->GetNumberType());
				return true;
			}
		}
		else if(vec.size() == 15)	//4连炸
		{
			CardVec one, two, three, four;
			GetSingleDoubleThreeFour(vec, one, two, three, four);
			if(four.size()/4 < 3) return false;	//3
			if(four.size()/4 + three.size()/3 < 4) return false;	//4
			CardVec normal, lai;
			normal.push_back(four[3]);
			normal.push_back(four[4]);
			normal.push_back(four[7]);
			normal.push_back(three[0]);
			SortCard(normal);
			bool is = IsSingleList(normal, lai, cardNumber);
			if(is)
			{
				laizi[0]->SetLaiziNumber(three[0]->GetNumberType());
				return true;
			}
		}


		
	}
	else if(laizi.size() == 0)
	{
		if(vec.size() == 8)
		{
			if(vec[7]->GetNumberType() == vec[4]->GetNumberType() && 
				vec[3]->GetNumberType() == vec[0]->GetNumberType() &&
				vec[4]->GetNumberType() + 1 == vec[3]->GetNumberType())
			{

				cardNumber = vec[0]->GetNumberType();
				return true;
			}
		}
		else if( vec.size() == 12)
		{
			CardVec one, two, three, four;
			GetSingleDoubleThreeFour(vec, one, two, three, four);
			if(four.size()/4 < 3) return false;	//3
			CardVec normal, lai;
			normal.push_back(four[3]);
			normal.push_back(four[7]);
			normal.push_back(four[11]);
			SortCard(normal);
			bool is = IsSingleList(normal, lai, cardNumber);
			if(is)
			{
				return true;
			}
		}
		else if(vec.size() == 16)
		{
			CardVec one, two, three, four;
			GetSingleDoubleThreeFour(vec, one, two, three, four);
			if(four.size()/4 < 4) return false;	//4
			CardVec normal, lai;
			normal.push_back(four[3]);
			normal.push_back(four[7]);
			normal.push_back(four[11]);
			normal.push_back(four[15]);
			SortCard(normal);
			bool is = IsSingleList(normal, lai, cardNumber);
			if(is)
			{
				return true;
			}
		}
	}

	return false;
}

//OK
bool CardStyle_3Lai::IsBombToSingle(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)//四带2单
{
	std::size_t sLaizi = laizi.size();
	CardVec single,duizi,three,four,numnber2,joke,other;
	GetJokeNumber2(vec,joke,numnber2,other);
	GetSingleDoubleThreeFour(other,single,duizi,three,four);

	switch(numnber2.size())
	{
	case 1:
		single.insert(single.begin(),numnber2.begin(),numnber2.end());
		break;
	case 2:
		duizi.insert(duizi.begin(),numnber2.begin(),numnber2.end());
		break;
	case 3:
		three.insert(three.begin(),numnber2.begin(),numnber2.end());
		break;
	case 4:
		four.insert(four.begin(),numnber2.begin(),numnber2.end());
		break;
	}

	if(four.size())
	{
		cardNumber = four[0]->GetNumberType();
		return true;
	}
	else if(three.size())
	{
		if(laizi.size() >= 1)
		{
			cardNumber = three[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(cardNumber);
			return true;
		}
	}
	else if(duizi.size())
	{
		if(laizi.size() >= 2)
		{
			cardNumber = duizi[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(cardNumber);
			laizi[1]->SetLaiziNumber(cardNumber);
			return true;
		}
	}
	else if(single.size())
	{
		if(laizi.size() >= 3)
		{
			cardNumber = single[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(cardNumber);
			laizi[1]->SetLaiziNumber(cardNumber);
			laizi[2]->SetLaiziNumber(cardNumber);
			return true;
		}
	}

	return false;
}

//OK
bool CardStyle_3Lai::IsBombToDouble(CardVec const& vec,CardVec const& laizi,BYTE& cardType)	//四带2对
{
	std::size_t sLaizi = laizi.size();
	CardVec joke,number2,single,duizi,three,four,other;
	GetJokeNumber2(vec,joke,number2,other);
	GetSingleDoubleThreeFour(other,single,duizi,three,four);
	if(joke.size())
	{
		return false;
	}

	if(number2.size() == 1)
	{
		single.push_back(number2[0]);
	}
	else if(number2.size() == 2)
	{
		duizi.insert(duizi.end(),number2.begin(),number2.end());
	}
	else if(number2.size() == 3)
	{
		three.insert(three.end(),number2.begin(),number2.end());
	}
	else if(number2.size() == 4)
	{
		four.insert(four.end(),number2.begin(),number2.end());
	}

	if(sLaizi < (single.size()+three.size()/3))
	{
		return false;
	}
	sLaizi -= (single.size()+three.size()/3);

	std::vector<BYTE> change,check2,check4;
	for(std::size_t i = 0 ;i < single.size(); i += 1)
	{
		change.push_back(single[i]->GetNumberType());
		check2.push_back(single[i]->GetNumberType());
	}
	for(std::size_t i = 0 ;i < duizi.size(); i += 2)
	{
		check2.push_back(duizi[i]->GetNumberType());
	}
	for(std::size_t i = 0 ;i < three.size(); i += 3)
	{
		change.push_back(three[i]->GetNumberType());
		check4.push_back(three[i]->GetNumberType());
	}
	for(std::size_t i = 0 ;i < four.size(); i += 4)
	{
		check4.push_back(four[i]->GetNumberType());
	}
	std::sort(check2.begin(),check2.end(),sort_byte);

	bool ret = false;
	if(check4.size())
	{
		cardType = check4.front();
		ret = true;
	}
	else if(check2.size())
	{
		if(sLaizi == 2)
		{
			change.push_back(check2.front());
			change.push_back(check2.front());
			cardType = check2.front();
			ret = true;
		}
	}

	int index = 0;
	while(change.size() && ret)
	{
		laizi[index++]->SetLaiziNumber(change.back());
		change.pop_back();
	}

	return ret;
}

//OK
bool CardStyle_3Lai::IsThree(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)	//是否是三张;
{
	if(vec.size() && vec[0]->GetNumberType() > card_number_type_2)
	{
		return false;
	}

	if(laizi.size() < 3)
	{
		BYTE count = 0;
		BYTE type = vec[0]->GetNumberType();
		CardVec::const_iterator it = vec.begin();
		for(;it != vec.end();++it)
		{
			if((*it)->GetNumberType() == type)
			{
				count++;
			}
		}

		if(count + laizi.size() == 3)
		{
			cardNumber = type;
			for(std::size_t i = 0; i < laizi.size(); ++i)
			{
				laizi[i]->SetLaiziNumber(cardNumber);
			}
			return true;
		}
		return false;
	}

	cardNumber = laizi[0]->GetNumberType();
	return true;
}

bool CardStyle_3Lai::IsThreeList(CardVec const& vec,CardVec const& laizi,BYTE& cardType)	//飞机;
{
	std::vector<BYTE> change,check;
	std::size_t sLaizi = laizi.size();
	CardVec joke,number2,single,duizi,three,four,other;
	GetJokeNumber2(vec,joke,number2,other);
	GetSingleDoubleThreeFour(other,single,duizi,three,four);

	if(joke.size() || number2.size() || four.size())
	{
		return false;
	}

	for(size_t i = 0 ; i < three.size(); i += 3)
	{
		check.push_back(three[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < duizi.size(); i += 2)
	{
		change.push_back(duizi[i]->GetNumberType());
		check.push_back(duizi[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < single.size(); i += 1)
	{
		check.push_back(single[i]->GetNumberType());
		change.push_back(single[i]->GetNumberType());
		change.push_back(single[i]->GetNumberType());
	}
	if (check.empty())
	{
		return false;
	}
	std::sort(check.begin(),check.end(),sort_byte);
	BYTE type = check[0];
	if(sLaizi < change.size() || type > card_number_type_A)
	{
		return false;
	}
	sLaizi -= change.size();

	BYTE type1 = check[0];
	for(std::size_t i = 1 ; i < check.size(); ++i)
	{
		if (type1 - check[i] > 2 || (type1 - check[i] == 2 && sLaizi < 3))
		{
			return false;
		}
		if(type1 - check[i] == 2 && sLaizi >= 3)
		{
			change.push_back(type1-1);
			change.push_back(type1-1);
			change.push_back(type1-1);
			sLaizi -= 3;
		}
		type1 = check[i];
	}

	if(sLaizi == 3)
	{
		if(type == card_number_type_A)
		{
			change.push_back(type1-1);
			change.push_back(type1-1);
			change.push_back(type1-1);
		}
		else
		{
			change.push_back(++type);
			change.push_back(type);
			change.push_back(type);
		}
	}

	BYTE index = 0;
	while(change.size())
	{
		laizi[index++]->SetLaiziNumber(change.back());
		change.pop_back();
	}

	cardType = type;
	return true;
}

//这个函数写的太长了，得想办法优化;
bool CardStyle_3Lai::IsThreeListToSingle(CardVec const& vec,CardVec const& laizi,BYTE& cardType)//飞机带单
{
	std::size_t sThreeCount = (vec.size()+laizi.size())/4;    //8（2）， 12（3）  16（4）   20（4）
	std::vector<BYTE> change,check1,check2,check3;
	std::size_t sLaizi = laizi.size();
	CardVec joke,number2,single,duizi,three,four,other;
	GetJokeNumber2(vec,joke,number2,other);
	GetSingleDoubleThreeFour(other,single,duizi,three,four);
	if (joke.size() == 2 || four.size())
	{
		return false;
	}

	CardVec tmp;
	for(Lsize i = 0 ; i+2 < three.size(); i += 3)
	{
		if (three[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(three[i]);
			tmp.push_back(three[i+1]);
			tmp.push_back(three[i+2]);
		}
	}

	for (size_t i = 0; i < four.size(); ++i)
	{
		if (four[i]->GetNumberType() <= card_number_type_A)
		{
			if ( i%4 == 0 )
			{
				single.push_back(four[i]);
			}
			else
			{
				tmp.push_back(four[i]);
			}
		}
	}
	for(size_t i = 0 ; i < tmp.size(); i += 3)
	{
		check3.push_back(tmp[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < duizi.size(); i += 2)
	{
		check2.push_back(duizi[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < single.size(); i += 1)
	{
		check1.push_back(single[i]->GetNumberType());
	}
	if (check3.size() + sLaizi < sThreeCount)
	{
		return false;
	}
	if (static_cast<int>(check3.size() - sThreeCount) >= static_cast<int>(1) && ( check3[check3.size()-2] - check3[check3.size()-1] > 1 ) )
	{
		check1.push_back(check3[check3.size()-1]);
		check2.push_back(check3[check3.size()-1]);  //不能连上的三张分放到单和对中，这里不求三张牌分别的索引值，不影响判断;
		check3.pop_back(); 	
	}
	if (static_cast<int>(check3.size() - sThreeCount) >= static_cast<int>(1) && (check3[0] - check3[1] > 1))
	{
		check1.push_back(check3[0]);
		check2.push_back(check3[0]);
		std::vector<BYTE>::iterator k = check3.begin(); 
		check3.erase(k);
	}
	std::sort(check3.begin(),check3.end(),sort_byte);
	std::sort(check2.begin(),check2.end(),sort_byte);
	std::sort(check1.begin(),check1.end(),sort_byte);

	if(CheckContinuous(check3) && check3.size() >= sThreeCount)//这里不缺三条,不管有没癞子;
	{
			cardType = check3[0];
			return true;
	}
	if(sLaizi == 1)
	{
		if((check3.size()+1) == sThreeCount)//缺一个三条;
		{
			for(std::size_t i = 0 ; i < check2.size(); ++i)//1癞子替换一个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					cardType = tmp.front();
					return true;
				}
			}
		}
	}
	else if(sLaizi == 2)
	{
		if((check3.size()+1) == sThreeCount)//缺一个三条;
		{
			for(std::size_t i = 0 ; i < check1.size(); ++i)//2癞子替换一个单张;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check1[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check1[i]);
					laizi[1]->SetLaiziNumber(check1[i]);
					cardType = tmp.front();
					return true;
				}
			}
			for(std::size_t i = 0 ; i < check2.size(); ++i)//1癞子替换一个对子，另外一张癞子没用;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					cardType = tmp.front();
					return true;
				}
			}
		}	
		if((check3.size()+2) == sThreeCount)//缺2个三条;
		{
			for(std::size_t i = 0 ; i+1 < check2.size(); ++i)//2癞子替换2个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i+1]);
					cardType = tmp.front();
					return true;
				}
			}
		}
	}
	else if(sLaizi == 3)
	{
		if((check3.size()+3) == sThreeCount)//缺3个三条;
		{
			for(std::size_t i = 0 ; i+2 < check2.size(); ++i)//3癞子替换3个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				tmp.push_back(check2[i+2]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i+1]);
					laizi[2]->SetLaiziNumber(check2[i+2]);
					cardType = tmp.front();
					return true;
				}
			}
		}
		if((check3.size()+2) == sThreeCount)//缺2个三条;
		{
			for(std::size_t i = 0 ; i < check2.size(); ++i)//1癞子替换1个对子,2癞子替换一个单张;
			{
				for(std::size_t j = 0 ; j < check1.size(); ++j)
				{
					std::vector<BYTE> tmp = check3;
					tmp.push_back(check2[i]);
					tmp.push_back(check1[j]);
					std::sort(tmp.begin(),tmp.end(),sort_byte);
					if(CheckContinuous(tmp))
					{
						laizi[0]->SetLaiziNumber(check2[i]);
						laizi[1]->SetLaiziNumber(check1[j]);
						laizi[2]->SetLaiziNumber(check1[j]);
						cardType = tmp.front();
						return true;
					}
				}
			}
			for(std::size_t i = 0 ; i+1 < check2.size(); ++i)//2癞子替换2个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i+1]);
					cardType = tmp.front();
					return true;
				}
			}
		}
		else if((check3.size()+1) == sThreeCount)//缺1个三条;
		{
			for(std::size_t i = 0 ; i < check1.size(); ++i)//2癞子替换一个单张;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check1[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check1[i]);
					laizi[1]->SetLaiziNumber(check1[i]);
					cardType = tmp.front();
					return true;
				}
			}
			for(std::size_t i = 0 ; i < check2.size(); ++i)//1癞子替换一个对子，另外2张癞子没用;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					cardType = tmp.front();
					return true;
				}
			}
			if(CheckContinuous(check3))//三癞子代替一个三条;
			{
				if(check3.front() < card_number_type_A)
				{
					cardType = check3.front()+1;
				}
				else
				{
					cardType = check3.back()-1;
				}
				laizi[0]->SetLaiziNumber(cardType);
				laizi[1]->SetLaiziNumber(cardType);
				laizi[2]->SetLaiziNumber(cardType);
				cardType = card_number_type_A;
				return true;
			}
			{
				Lint cardnum1 = check3[0];
				for (size_t i = 1; i < check3.size(); ++i)
				{
					if (cardnum1 - check3[i+1] > 1 && sLaizi >= 3 )
					{
						sLaizi -= 3;
						cardnum1 +=1;
						change.push_back(cardnum1) ;
					}
					if (cardnum1 - check3[i+1] > 1 && sLaizi < 3 )
					{
						return false;
					}
					cardnum1 = check3[i];
				}
				cardType = cardnum1;
				return true;
			}
		}
	}
	else if(sLaizi == 4)
	{
		//能到这里的概率,估计跟中彩票的概率差不多，先不考虑;
		if ((check3.size() + 4) == sThreeCount)//缺3个三条;
		{
			for (std::size_t i = 0; i + 3 < check2.size(); ++i)
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i + 1]);
				tmp.push_back(check2[i + 2]);
				tmp.push_back(check2[i + 3]);
				std::sort(tmp.begin(), tmp.end(), sort_byte);
				if (CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i + 1]);
					laizi[2]->SetLaiziNumber(check2[i + 2]);
					laizi[3]->SetLaiziNumber(check2[i + 2]);
					cardType = tmp.front();
					return true;
				}
			}
		}

		if ((check3.size() + 3) == sThreeCount)//缺3个三条;
		{
			for (std::size_t i = 0; i + 2 < check2.size(); ++i)//3癞子替换3个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i + 1]);
				tmp.push_back(check2[i + 2]);
				std::sort(tmp.begin(), tmp.end(), sort_byte);
				if (CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i + 1]);
					laizi[2]->SetLaiziNumber(check2[i + 2]);
					cardType = tmp.front();
					return true;
				}
			}
		}
		if ((check3.size() + 2) == sThreeCount)//缺2个三条;
		{
			for (std::size_t i = 0; i < check2.size(); ++i)//1癞子替换1个对子,2癞子替换一个单张;
			{
				for (std::size_t j = 0; j < check1.size(); ++j)
				{
					std::vector<BYTE> tmp = check3;
					tmp.push_back(check2[i]);
					tmp.push_back(check1[j]);
					std::sort(tmp.begin(), tmp.end(), sort_byte);
					if (CheckContinuous(tmp))
					{
						laizi[0]->SetLaiziNumber(check2[i]);
						laizi[1]->SetLaiziNumber(check1[j]);
						laizi[2]->SetLaiziNumber(check1[j]);
						cardType = tmp.front();
						return true;
					}
				}
			}
			for (std::size_t i = 0; i + 1 < check2.size(); ++i)//2癞子替换2个对子;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i + 1]);
				std::sort(tmp.begin(), tmp.end(), sort_byte);
				if (CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					laizi[1]->SetLaiziNumber(check2[i + 1]);
					cardType = tmp.front();
					return true;
				}
			}
		}
		else if ((check3.size() + 1) == sThreeCount)//缺1个三条;
		{
			for (std::size_t i = 0; i < check1.size(); ++i)//2癞子替换一个单张;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check1[i]);
				std::sort(tmp.begin(), tmp.end(), sort_byte);
				if (CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check1[i]);
					laizi[1]->SetLaiziNumber(check1[i]);
					cardType = tmp.front();
					return true;
				}
			}
			for (std::size_t i = 0; i < check2.size(); ++i)//1癞子替换一个对子，另外2张癞子没用;
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				std::sort(tmp.begin(), tmp.end(), sort_byte);
				if (CheckContinuous(tmp))
				{
					laizi[0]->SetLaiziNumber(check2[i]);
					cardType = tmp.front();
					return true;
				}
			}
			if (CheckContinuous(check3))//三癞子代替一个三条;
			{
				if (check3.front() < card_number_type_A)
				{
					cardType = check3.front() + 1;
				}
				else
				{
					cardType = check3.back() - 1;
				}
				laizi[0]->SetLaiziNumber(cardType);
				laizi[1]->SetLaiziNumber(cardType);
				laizi[2]->SetLaiziNumber(cardType);
				return true;
			}
		}
	}

	return false;
}

//这个函数最麻烦;
bool CardStyle_3Lai::IsThreeListToDouble(CardVec const& vec,CardVec const& laizi,BYTE& cardType)//飞机带对;
{
	bool ret = false;
	std::size_t sThreeCount = (vec.size()+laizi.size())/5;
	std::vector<BYTE> change,/*check1,*/check2,check3;
	std::size_t sLaizi = laizi.size();
	CardVec joke,number2,single,duizi,three,four,other;
	GetJokeNumber2(vec,joke,number2,other);
	GetSingleDoubleThreeFour(other,single,duizi,three,four);
	if (joke.size())
	{
		return false;
	}
	if (four.size())
	{
		return false;
	}
	if(number2.size()%2)
	{
		if (sLaizi > 0)
		{
			--sLaizi;   //如果不判断,sLaizi可能变负值;
			change.push_back(number2[0]->GetNumberType());
		}
		else
			return false;
	}
	for (Lsize i = 0; i < single.size(); ++i)
	{
		if (sLaizi > 0)
		{
			--sLaizi;
			change.push_back(single[i]->GetNumberType());
			duizi.push_back(single[i]);
		}
		else
			return false;
	}
	CardVec tmp;
	for(Lsize i = 0 ; i+2 < three.size(); i += 3)
	{
		if (three[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(three[i]);
			tmp.push_back(three[i+1]);
			tmp.push_back(three[i+2]);
		}
	}
	//至此牌型均应为对子或者三张，之后三张不足依靠癞子补充;
	if (tmp.size()/3 + sLaizi < sThreeCount)
	{
		return false;
	}

	for(size_t i = 0 ; i < tmp.size(); i += 3)
	{
		check3.push_back(tmp[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < duizi.size(); i += 2)
	{
		check2.push_back(duizi[i]->GetNumberType());
	}

	std::sort(check2.begin(),check2.end(),sort_byte);
	std::sort(check3.begin(),check3.end(),sort_byte);

	if(check3.size() >= sThreeCount)//这里不缺三条;
	{
		for(Lsize i = 0 ; i <= check3.size()-sThreeCount; ++i)
		{
			std::vector<BYTE> tmpCheck(check3.begin()+i,check3.begin()+i+sThreeCount);
			if(CheckContinuous(tmpCheck))
			{
				cardType = tmpCheck.front();
				ret = true;
			}
		}
	}
	
	else if(check3.size()+1 == sThreeCount)//缺一个三条;
	{
		if(sLaizi == 1)
		{
			for(std::size_t i = 0 ; i < check2.size(); ++i)
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					change.push_back(check2[i]);
					cardType = tmp.front();
					ret = true;
					break;
				}
			}
		}
		else if(sLaizi == 3)
		{
			if(CheckContinuous(check3))
			{
				if(check3.front() < card_number_type_A)
				{
					cardType = check3.front()+1;
				}
				else
				{
					cardType = check3.back()-1;
				}
				change.push_back(cardType);
				change.push_back(cardType);
				change.push_back(cardType);
				ret = true;
			}
		}
	}
	else if(check3.size()+2 == sThreeCount)//缺2个三条
	{
		if(sLaizi == 2 || sLaizi == 4)
		{
			for(std::size_t i = 0 ; i+1 < check2.size(); ++i)
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					change.push_back(check2[i]);
					change.push_back(check2[i+1]);
					cardType = tmp.front();
					ret = true;
					break;
				}
			}
		}
	}
	else if(check3.size()+3 == sThreeCount)//缺3个三条
	{
		if(sLaizi == 3)
		{
			for(std::size_t i = 0 ; i+2 < check2.size(); ++i)
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				tmp.push_back(check2[i+2]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					change.push_back(check2[i]);
					change.push_back(check2[i+1]);
					change.push_back(check2[i+2]);
					cardType = tmp.front();
					ret = true;
					break;
				}
			}
		}
	}
	else if(check3.size()+4 == sThreeCount)//缺4个三条
	{
		if(sLaizi == 4)
		{
			for(std::size_t i = 0 ; i+3 < check2.size(); ++i)
			{
				std::vector<BYTE> tmp = check3;
				tmp.push_back(check2[i]);
				tmp.push_back(check2[i+1]);
				tmp.push_back(check2[i+2]);
				tmp.push_back(check2[i+3]);
				std::sort(tmp.begin(),tmp.end(),sort_byte);
				if(CheckContinuous(tmp))
				{
					change.push_back(check2[i]);
					change.push_back(check2[i+1]);
					change.push_back(check2[i+2]);
					change.push_back(check2[i+3]);
					cardType = tmp.front();
					ret = true;
					break;
				}
			}
		}
	}

	if(ret)
	{
		for(std::size_t i = 0 ; i < change.size() && i < laizi.size(); ++i)
		{
			laizi[i]->SetLaiziNumber(change[i]);
		}
	}
	return ret;
}

//OK
bool CardStyle_3Lai::IsThreeToSingle(CardVec const& vec,CardVec const& laizi,BYTE& cardType)//三带1
{
	std::size_t sLaizi = laizi.size();
	CardVec single,duizi,three,four;
	GetSingleDoubleThreeFour(vec,single,duizi,three,four);
	if(sLaizi == 0)
	{
		if(three.size() && single.size())
		{
			cardType = three[0]->GetNumberType();
			return true;
		}
	}
	else if(sLaizi == 1)
	{
		if(duizi.size() && single.size())
		{
			cardType = duizi[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(duizi[0]->GetNumberType());
			return true;
		}
		if(three.size())
		{
			cardType = three[0]->GetNumberType();
			return true;
		}
	}
	else if(sLaizi == 2)
	{
		if(single.size() == 2)
		{
			cardType = single[0]->GetNumberType();
			if (cardType >= card_number_type_SJ )
			{
				cardType = single[1]->GetNumberType();
			}
			if (cardType >= card_number_type_SJ )
			{
				return false;
			}
			laizi[0]->SetLaiziNumber(cardType);
			laizi[1]->SetLaiziNumber(cardType);
			return true;
		}
		//一对加两癞子暂时不允许三带处理;
	}
	else if (sLaizi == 3 && vec.size() > 0 && (vec[0]->GetNumberType() == card_number_type_BJ || vec[0]->GetNumberType() == card_number_type_SJ) )  //三张癞子和大小王共存时为三代单，其他为炸弹;
	{
		cardType = laizi[0]->GetNumberType();
		return true;
	}
	return false;
}

//OK;
bool CardStyle_3Lai::IsThreeToDouble(CardVec const& vec,CardVec const& laizi,BYTE& cardType)//三带对;
{
	std::size_t sLaizi = laizi.size();
	CardVec single,duizi,three,four;
	GetSingleDoubleThreeFour(vec,single,duizi,three,four);

	if (single.size() > 0) //三带对不能出现王;
	{
		if (single[0]->GetNumberType() == card_number_type_BJ || single[0]->GetNumberType() == card_number_type_SJ)
		{
			return false;
		}
	}

	if (four.size() > 0)
	{
		return false;
	}

	if(sLaizi == 0)
	{
		if( three.size() && duizi.size())
		{
			cardType = three[0]->GetNumberType();
			return true;
		}
	}
	else if(sLaizi == 1)
	{
		if(three.size() && single.size())
		{
			cardType = three[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(single[0]->GetNumberType());
			return true;
		}
		if(duizi.size() == 4)
		{
			cardType = duizi[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(duizi[0]->GetNumberType());
			return true;
		}
	}
	else if(sLaizi == 2)
	{
		if(three.size())
		{
			cardType = three[0]->GetNumberType();
			return true;
		}
		if(duizi.size() && single.size())
		{
			if(duizi[0]->GetNumberType() > single[0]->GetNumberType())
			{
				cardType = duizi[0]->GetNumberType();
				laizi[0]->SetLaiziNumber(duizi[0]->GetNumberType());
				laizi[1]->SetLaiziNumber(single[0]->GetNumberType());
			}
			else
			{
				cardType = single[0]->GetNumberType();
				laizi[0]->SetLaiziNumber(single[0]->GetNumberType());
				laizi[1]->SetLaiziNumber(single[0]->GetNumberType());
			}
			return true;
		}
	}
	else if(sLaizi == 3)
	{
		if(duizi.size())
		{
			if(duizi[0]->GetNumberType() > laizi[0]->GetNumberType())
			{
				cardType = duizi[0]->GetNumberType();
				laizi[0]->SetLaiziNumber(duizi[0]->GetNumberType());
			}
			else
			{
				cardType = laizi[0]->GetNumberType();
			}
			return true;
		}
		if(single.size() == 2)
		{
			cardType = single[0]->GetNumberType();
			laizi[0]->SetLaiziNumber(single[0]->GetNumberType());
			laizi[1]->SetLaiziNumber(single[0]->GetNumberType());
			laizi[2]->SetLaiziNumber(single[1]->GetNumberType());
			return true;
		}
	}
	else if(sLaizi == 4)
	{
		if(single.size())
		{
			if(single[0]->GetNumberType() > laizi[0]->GetNumberType())
			{
				cardType = single[0]->GetNumberType();
				laizi[0]->SetLaiziNumber(cardType);
				laizi[1]->SetLaiziNumber(cardType);
			}
			else
			{
				cardType = laizi[0]->GetNumberType();
				laizi[0]->SetLaiziNumber(single[0]->GetNumberType());
			}
			return true;
		}
	}
	return false;
}

//OK
bool CardStyle_3Lai::IsSingleList(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)//是否是单顺;
{
	BYTE type = vec[0]->GetNumberType();
	if(type > card_number_type_A)
	{
		return false;
	}

	std::size_t dLaizi = laizi.size();
	BYTE bStepType = type;
	std::vector<BYTE> change;
	for(std::size_t i = 1 ; i < vec.size(); ++i)
	{
		BYTE bNextType = vec[i]->GetNumberType();
		if(bStepType <= bNextType)
		{
			return false;
		}
		while(--bStepType > bNextType)
		{
			if(dLaizi == 0)
			{
				return false;
			}
			else
			{
				dLaizi -= 1;
				change.push_back(bStepType);
			}
		}
	}

	std::size_t index = 0;
	while(change.size())
	{
		laizi[index++]->SetLaiziNumber(change.back());
		change.pop_back();
	}
	while(++type < card_number_type_2 && index < laizi.size())
	{
		laizi[index++]->SetLaiziNumber(type);
	}
	while(index < laizi.size())
	{
		laizi[index++]->SetLaiziNumber(--bStepType);
	}

	cardNumber = type-1;
	return true;
}

//OK
bool CardStyle_3Lai::IsDoubleList(CardVec const& vec,CardVec const& laizi,BYTE& cardNumber)//是否是双顺;
{
	BYTE type = vec[0]->GetNumberType();
	if(type > card_number_type_A )
	{
		return false;
	}

	std::size_t sLaizi = laizi.size();
	std::vector<BYTE> change,check;
	CardVec single,duizi,three,four;
	GetSingleDoubleThreeFour(vec,single,duizi,three,four);
	if(sLaizi < single.size() || three.size() || four.size())
	{
		return false;
	}

	for(size_t i = 0 ; i < duizi.size(); i += 2)
	{
		check.push_back(duizi[i]->GetNumberType());
	}
	for(size_t i = 0 ; i < single.size(); i += 1)
	{
		check.push_back(single[i]->GetNumberType());
		change.push_back(single[i]->GetNumberType());
	}
	sLaizi -= single.size();

	std::sort(check.begin(),check.end(),sort_byte);
	BYTE type1 = check.front();
	BYTE type2 = check.back();
	for(std::size_t i = 1 ; i < check.size(); ++i)
	{   
		BYTE len = (type1-check[i]-1)*2;
		if(len == 0)
		{
			type1 = check[i];
			continue;
		}

		if(len > sLaizi)
		{
			return false;
		}
		else
		{
			sLaizi -= len;
			for(BYTE j = 1 ; j < len; j+=2)
			{
				change.push_back(--type1);
				change.push_back(type1);
			}
			type1 = check[i];
		}
	}

	std::size_t index = 0;
	while(change.size())
	{
		laizi[index++]->SetLaiziNumber(change.back());
		change.pop_back();
	}
	while(++type < card_number_type_2 && index < laizi.size())
	{
		laizi[index++]->SetLaiziNumber(type);
		laizi[index++]->SetLaiziNumber(type);
	}
	while(index < laizi.size())
	{
		laizi[index++]->SetLaiziNumber(--type2);
		laizi[index++]->SetLaiziNumber(type2);
	}

	cardNumber = type-1;
	return true;
}

void CardStyle_3Lai::GetSingleDoubleThreeFour(CardVec const& vec,CardVec& single,CardVec& duizi,CardVec& three,CardVec& four)
{
	single.clear();
	duizi.clear();
	three.clear();
	four.clear();
	if(vec.empty())
	{
		return;
	}

	BYTE type = vec[0]->GetNumberType();
	BYTE len = 1;
	std::size_t i = 1;
	for(; i < vec.size(); ++i)
	{
		if(vec[i]->GetNumberType() == type)
		{
			len++;
		}
		else
		{
			if(len == 1)
			{
				single.push_back(vec[i-1]);
			}
			else if(len == 2)
			{
				duizi.push_back(vec[i-1]);
				duizi.push_back(vec[i-2]);
			}
			else if(len == 3)
			{
				three.push_back(vec[i-1]);
				three.push_back(vec[i-2]);
				three.push_back(vec[i-3]);
			}
			else if(len == 4)
			{
				four.push_back(vec[i-1]);
				four.push_back(vec[i-2]);
				four.push_back(vec[i-3]);
				four.push_back(vec[i-4]);
			}
			type = vec[i]->GetNumberType();
			len = 1;
		}
	}

	if(len == 1)
	{
		single.push_back(vec[i-1]);
	}
	else if(len == 2)
	{
		duizi.push_back(vec[i-1]);
		duizi.push_back(vec[i-2]);
	}
	else if(len == 3)
	{
		three.push_back(vec[i-1]);
		three.push_back(vec[i-2]);
		three.push_back(vec[i-3]);
	}
	else if(len == 4)
	{
		four.push_back(vec[i-1]);
		four.push_back(vec[i-2]);
		four.push_back(vec[i-3]);
		four.push_back(vec[i-4]);
	}
}

void CardStyle_3Lai::GetJokeNumber2(CardVec const& vec,CardVec& joke,CardVec& number2,CardVec& other)
{
	joke.clear();
	number2.clear();
	other.clear();
	std::size_t i = 0;
	for( ; i < vec.size(); ++i)
	{
		if(vec[i]->GetNumberType() < card_number_type_2)
		{
			other.push_back(vec[i]);
		}
		else if(vec[i]->GetNumberType() == card_number_type_2)
		{
			number2.push_back(vec[i]);
		}
		else
		{
			joke.push_back(vec[i]);
		}
	}
}

void CardStyle_3Lai::GetLaizi(CardVec const& vec,CardVec& laizi,CardVec& other)
{
	laizi.clear();
	other.clear();
	for(std::size_t i = 0 ; i < vec.size(); ++i)
	{
		if(vec[i]->GetLaizi())
		{
			laizi.push_back(vec[i]);
		}
		else
		{
			other.push_back(vec[i]);
		}
	}
}

void CardStyle_3Lai::GetBombLaiziJokeNumber2(CardVec const& vec,CardVec& bomb,CardVec& laizi,CardVec& joke,CardVec& number2,CardVec& other)
{
	bomb.clear();
	laizi.clear();
	joke.clear();
	number2.clear();
	other.clear();
	CardVec tmp;
	GetLaizi(vec,laizi,tmp);
	GetJokeNumber2(tmp,joke,number2,other);
	if(number2.size() == 4)
	{
		bomb = number2;
		number2.clear();
	}

	if(other.size() < 4)
	{
		return ;
	}

	BYTE type = other[0]->GetNumberType();
	BYTE len = 1;
	std::size_t i = 1;
	for(; i < other.size(); ++i)
	{
		if(other[i]->GetNumberType() == type)
		{
			len++;
		}
		else
		{
			if(len == 4)
			{
				bomb.push_back(other[i-1]);
				bomb.push_back(other[i-2]);
				bomb.push_back(other[i-3]);
				bomb.push_back(other[i-4]);
			}
			type = other[i]->GetNumberType();
			len = 1;
		}
	}

	if(len == 4)
	{
		bomb.push_back(other[i-1]);
		bomb.push_back(other[i-2]);
		bomb.push_back(other[i-3]);
		bomb.push_back(other[i-4]);
	}

	EarseCard(other,bomb);
}

bool CardStyle_3Lai::CheckContinuous(std::vector<BYTE>& vec)
{
	if(vec.empty())
	{
		return false;
	}
	BYTE type = vec[0];
	for(std::size_t i = 1 ; i < vec.size(); ++i)
	{
		if(type - vec[i] > 1)
		{
			return false;
		}
		type = vec[i];
	}

	return true;
}

bool CardStyle_3Lai::HasRocket(CardVec const& vec)
{
	Lint nHasBKing = 0;
	Lint nHasSKing = 0;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i]->GetNumberType() == card_number_type_BJ)
		{
			nHasBKing = 1;
		}
		else if (vec[i]->GetNumberType() == card_number_type_SJ)
		{
			nHasSKing = 1;
		}
		else {} //do noting;
	}
	if (1 == nHasBKing && 1 == nHasSKing)
	{
		return true;
	}
	else
		return false;
}

bool CardStyle_3Lai::Has2Count4Bomb(CardVec const& vec)
{
	Lint nHas2 = 0;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i]->GetNumberType() == card_number_type_2)
		{
			nHas2 += 1;
		}
		else {} //do noting;
	}
	if (4 == nHas2)
	{
		return true;
	}
	else
		return false;
}

//这里主要是给一个全面的评分;
int CardStyle_3Lai::GetCardScore(CardVec& vec)
{
	int score = 0;

	for(int i = 0 ; i < int(vec.size()); ++i)
	{
		if(vec[i]->GetLaizi())
		{
			score += card_number_type_2;
		}
		else
		{
			score += vec[i]->GetNumberType();
		}
	}
	if(vec.size() != 0)
	{
		score = score/vec.size();
	}

	return score;
}

CardStyle_3Lai* CardStyle_3Lai::Instance()
{
	static CardStyle_3Lai inc;
	return &inc;
}

//////////////////////////////////////////////////////////////////////////
CardHelp_3Lai::CardHelp_3Lai()
{

}

CardHelp_3Lai::~CardHelp_3Lai()
{

}

std::vector<std::vector<Lint>> CardHelp_3Lai::HelpMe(Lint cardMode, Lint cardType, Lint cardCount, CardVec& handcard, std::vector<Lint>& m_curHelpUnusable, bool baojing)
{
	//只对普通牌进行判牌;
	std::vector<std::vector<Lint>> vec;
	CardVec normal, laizi;
	CardStyle_3Lai::Instance()->GetLaizi(handcard, laizi, normal);

	//
	CardVec one,tow,three,four;
	CardStyle_3Lai::Instance()->GetSingleDoubleThreeFour(normal,one,tow,three,four);

	Lint nLen = normal.size();
	if (nLen >= 2 && card_number_type_SJ == normal[1]->GetNumberType() && card_number_type_BJ == normal[0]->GetNumberType() && cardMode < card_style_super_bomb)
	{
		std::vector<Lint> ret;
		ret.push_back(normal[1]->GetIndex());
		ret.push_back(normal[0]->GetIndex());
		vec.push_back(ret);
	}

	if (card_style_four2 == cardMode) // 四带二的提示放在vec的最底，让炸弹优先在客户端展示;
	{
		CheckFourToTwo(cardType,one,tow,three,four,vec);
	}

	//纯粹软炸弹最大;
	if (cardMode != card_style_rocket && laizi.size() >= 4)
	{
		std::vector<Lint> ret;
		ret.push_back(laizi[0]->GetIndex());
		ret.push_back(laizi[1]->GetIndex());
		ret.push_back(laizi[2]->GetIndex());
		ret.push_back(laizi[3]->GetIndex());
		vec.push_back(ret);
	}

	for(Lsize i = 0 ; i < four.size(); i+=4)
	{
		if((card_style_bomb2 != cardMode && card_style_bomb1 != cardMode && card_style_rocket != cardMode && cardMode < card_style_super_bomb) ||
			(cardMode == card_style_bomb2 && cardType < four[i]->GetNumberType()))
		{
			std::vector<Lint> ret;
			ret.push_back(four[i]->GetIndex());
			ret.push_back(four[i+1]->GetIndex());
			ret.push_back(four[i+2]->GetIndex());
			ret.push_back(four[i+3]->GetIndex());
			vec.push_back(ret);
		}
	}
	switch(cardMode)
	{
	case card_style_single:
		CheckOne(cardType,one,tow,three,four,vec,baojing);
		break;
	case card_style_double:
		CheckTwo(cardType,one,tow,three,four,vec);
		break;
	case card_style_three_double:
		CheckThreeToTwo(cardType,one,tow,three,four,vec);
		break;
	case card_style_three_single:
		CheckThreeToOne(cardType,one,tow,three,four,vec);
		break;
	case card_style_three_list_double:
		CheckThreeFlyToTwo(cardType,one,tow,three,four,normal,cardCount,vec);
		break;
	case card_style_three_list_single:
		CheckThreeFlyToSingle(cardType,one,tow,three,four,normal,cardCount,vec);
		break;
	case card_style_three_list:
		CheckThreeList(cardType,one,tow,three,four,normal,cardCount,vec);
		break;
	case card_style_single_list:
		CheckOneList(cardType,one,tow,three,four,cardCount,vec);
		break;
	case card_style_double_list:
		CheckTwoList(cardType,one,tow,three,four,cardCount,vec);
		break;
	case card_style_three:
		CheckThree(cardType,one,tow,three,four,vec);
		break;
	default:
		break;
	}
	m_curHelpUnusable.clear();
	Lint vecFlag[CARD_COUNT_DDZ+20] = {0};   // 不在玩家手中牌为0，在手中可出为1，在手中不可出为2;
	for (size_t kk = 0; kk < normal.size(); ++kk)
	{
		vecFlag[normal[kk]->GetIndex()] = 2;
	}
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		for (size_t jj = 0; jj < vec[ii].size(); ++jj)
		{
	//		vecFlag[vec[ii][jj]] = 1;
			Card cardTmp;
			cardTmp.SetIndex(vec[ii][jj]);
			if (cardTmp.GetNumberType() == card_number_type_SJ)
			{
				vecFlag[52] = 1;
			}
			else if (cardTmp.GetNumberType() == card_number_type_BJ)
			{
				vecFlag[53] = 1;
			}
			else
			{
				vecFlag[0 + (cardTmp.GetNumberType() - 1) * 4] = 1;
				vecFlag[1 + (cardTmp.GetNumberType() - 1) * 4] = 1;
				vecFlag[2 + (cardTmp.GetNumberType() - 1) * 4] = 1;
				vecFlag[3 + (cardTmp.GetNumberType() - 1) * 4] = 1;
			}
		}
	}
	for (Lint ii = 0; ii < CARD_COUNT_DDZ; ++ii)
	{
		if (2 == vecFlag[ii])
		{
			m_curHelpUnusable.push_back(ii);
		}
	}

	return vec;
}

void CardHelp_3Lai::CheckOne(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret,bool baojing)
{
	std::vector<Card*> vec;

	for(Lsize i = 0 ; i < th.size(); i+=3)
	{
		if(th[i]->GetNumberType() > cardType)
		{
			vec.push_back(th[i]);
		}
	}
	for(Lsize i = 0 ; i < t.size(); i+=2)
	{
		if(t[i]->GetNumberType() > cardType)
		{
			vec.push_back(t[i]);
		}
	}
	for(Lsize i = 0 ; i < o.size(); ++i)
	{
		if(o[i]->GetNumberType() > cardType)
		{
			vec.push_back(o[i]);
		}
	}

	for(Lsize i = 0 ; i < vec.size(); ++i)
	{
		std::vector<Lint> v;
		v.push_back(vec[i]->GetIndex());
		ret.push_back(v);
	}
}

void CardHelp_3Lai::CheckTwo(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret)
{
	std::vector<Card*> vec;

	//炸弹拆分打对子不作为提示;
	//for(Lsize i = 0 ; i < f.size(); i+=4)
	//{
	//	if(f[i]->GetNumberType() > cardType)
	//	{
	//		vec.push_back(f[i]);
	//		vec.push_back(f[i+1]);
	//	}
	//}
	for(Lsize i = 0 ; i < th.size(); i+=3)
	{
		if(th[i]->GetNumberType() > cardType)
		{
			vec.push_back(th[i]);
			vec.push_back(th[i+1]);
		}
	}
	for(Lsize i = 0 ; i < t.size(); i+=2)
	{
		if(t[i]->GetNumberType() > cardType)
		{
			vec.push_back(t[i]);
			vec.push_back(t[i+1]);
		}
	}

	for(Lsize i = 0 ; i < vec.size(); i += 2)
	{
		std::vector<Lint> v;
		v.push_back(vec[i]->GetIndex());
		v.push_back(vec[i+1]->GetIndex());
		ret.push_back(v);
	}
}

void CardHelp_3Lai::CheckThreeToOne(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret)
{
	CardVec tmp = o;
	tmp.insert(tmp.end(),t.begin(),t.end());
	tmp.insert(tmp.end(),th.begin(),th.end());
	tmp.insert(tmp.end(),f.begin(),f.end());
	CardStyle_3Lai::Instance()->SortCard(tmp);
	if(tmp.size() < 4)
		return;
    CardVec th_new;
	for (size_t i = 0; i+3 < f.size(); i += 4)  //炸弹拆分打过三带;
	{
		th_new.push_back(f[i]);
		th_new.push_back(f[i+1]);
		th_new.push_back(f[i+2]);
	}
	for (size_t i = 0; i+2 < th.size(); i += 3)  //炸弹拆分打过三带;
	{
		th_new.push_back(th[i]);
		th_new.push_back(th[i+1]);
		th_new.push_back(th[i+2]);
	}

	for(Lsize i = 0 ; i < th_new.size(); i+=3)
	{
		if(th_new[i]->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			v.push_back(th_new[i]->GetIndex());
			v.push_back(th_new[i+1]->GetIndex());
			v.push_back(th_new[i+2]->GetIndex());

			//按照提示优先程度逆序加入提示组;
			for(Lsize j = 0 ; j < f.size(); j+=4)
			{
				if (th_new[i]->GetNumberType() != f[j]->GetNumberType())
				{
					std::vector<Lint> vv = v;
					vv.push_back(f[j]->GetIndex());
					ret.push_back(vv);
				}
			}
			for(Lsize j = 0 ; j < th.size(); j+=3)
			{
				if (th_new[i]->GetNumberType() != th[j]->GetNumberType())
				{
					std::vector<Lint> vv = v;
					vv.push_back(th[j]->GetIndex());
					ret.push_back(vv);
				}
			}
			for (Lsize j = 0; j < t.size(); j+=2)
			{
				std::vector<Lint> vv = v;
				vv.push_back(t[j]->GetIndex());
				ret.push_back(vv);
			}
			for (Lsize j = 0; j < o.size(); ++j)
			{
				std::vector<Lint> vv = v;
				vv.push_back(o[j]->GetIndex());
				ret.push_back(vv);
			}
		}
	}
}

void CardHelp_3Lai::CheckThreeToTwo(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret)
{
	CardVec tmp = o;
	tmp.insert(tmp.end(),t.begin(),t.end());
	tmp.insert(tmp.end(),th.begin(),th.end());
	tmp.insert(tmp.end(),f.begin(),f.end());
	CardStyle_3Lai::Instance()->SortCard(tmp);
	if(tmp.size() < 5)
		return;
	CardVec th_new;
	for (size_t i = 0; i+3 < f.size(); i += 4)  //炸弹拆分打过三带;
	{
		th_new.push_back(f[i]);
		th_new.push_back(f[i+1]);
		th_new.push_back(f[i+2]);
	}
	for (size_t i = 0; i+2 < th.size(); i += 3)  //炸弹拆分打过三带;
	{
		th_new.push_back(th[i]);
		th_new.push_back(th[i+1]);
		th_new.push_back(th[i+2]);
	}

	for(Lsize i = 0 ; i < th_new.size(); i+=3)
	{
		if(th_new[i]->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			v.push_back(th_new[i]->GetIndex());
			v.push_back(th_new[i+1]->GetIndex());
			v.push_back(th_new[i+2]->GetIndex());

			for(Lsize j = 0 ; j + 2 < f.size(); j+=4)
			{
				if (th_new[i]->GetNumberType() != f[j]->GetNumberType()) 
				{
					std::vector<Lint> vv = v;
					vv.push_back(f[j]->GetIndex());
					vv.push_back(f[j+1]->GetIndex());
					ret.push_back(vv);
				}
			}
			for(Lsize j = 0 ; j + 1 < th.size(); j+=3)
			{
				if (th_new[i]->GetNumberType() != th[j]->GetNumberType()) 
				{
					std::vector<Lint> vv = v;
					vv.push_back(th[j]->GetIndex());
					vv.push_back(th[j+1]->GetIndex());
					ret.push_back(vv);
				}
			}
			for(Lsize j = 0 ; j + 1 < t.size(); j+=2)
			{
				std::vector<Lint> vv = v;
				vv.push_back(t[j]->GetIndex());
				vv.push_back(t[j+1]->GetIndex());
				ret.push_back(vv);
			}
		}
	}
}

void CardHelp_3Lai::CheckThreeFlyToTwo(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f,CardVec& handcard,Lint count, std::vector<std::vector<Lint>>& ret)
{
	if(handcard.size() < (size_t)count)
		return;

	//首先提取3张
	CardVec tmp;
	for(Lsize i = 0 ; i+2 < th.size(); i += 3)
	{
		if (th[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(th[i]);
			tmp.push_back(th[i+1]);
			tmp.push_back(th[i+2]);
		}
	}
	for(Lsize i = 0 ; i+3 < f.size(); i += 4)
	{
		if (f[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(f[i]);
			tmp.push_back(f[i+1]);
			tmp.push_back(f[i+2]);
		}
	}
	CardStyle_3Lai::Instance()->SortCard(tmp);

	Lint step = count/5; //飞机带双中三张个数;
	
	for(Lsize i = 0 ; i+step*3 <= tmp.size(); i += 3)
	{
		CardVec vec;
		CardVec check(tmp.begin()+i,tmp.end());
		Lint type = check.front()->GetNumberType();
		vec.push_back(check.front());
		vec.push_back(*(check.begin()+1));
		vec.push_back(*(check.begin()+2));
		for(Lsize j = 3 ; j+2 < check.size(); j += 3)
		{
			if(check[j]->GetNumberType()+1 == type)
			{
				type -= 1;
				vec.push_back(check[j]);
				vec.push_back(check[j+1]);
				vec.push_back(check[j+2]);
				if(vec.size() == step*3)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if(vec.size() == step*3 && vec.front()->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			for(Lsize j = 0; j < vec.size(); ++j)
			{
				v.push_back(vec[j]->GetIndex());
			}

			CardVec h = handcard;

			// 提取除了飞机之外剩下的对子,如果够与飞机搭配则认为可以组成合适牌型;
			CardStyle_3Lai::Instance()->EarseCard(h,vec);
            CardVec one,two,three,four;
			CardStyle_3Lai::Instance()->GetSingleDoubleThreeFour(h,one,two,three,four);
			CardVec two_new;
			//优先级低的先放入提示数组;
			for(Lsize i = 0 ; i+3 < four.size(); i += 4)
			{
				two_new.push_back(four[i]);
				two_new.push_back(four[i+1]);
				//two_new.push_back(four[i+2]);    //飞机不允许带有炸弹，但是允许炸弹中的两张;
				//two_new.push_back(four[i+3]);
			}
			for(Lsize i = 0 ; i+1 < three.size(); i += 3)
			{
				two_new.push_back(three[i]);
				two_new.push_back(three[i+1]);
			}
			for(Lsize i = 0 ; i+1 < two.size(); i += 2)
			{
				two_new.push_back(two[i]);
				two_new.push_back(two[i+1]);
			}
			for(Lsize j = 0; j + step*2 <= two_new.size(); j += 2)
			{
				std::vector<Lint> vv = v;
				for(Lint k=0;k<step*2; ++k)
				{
					vv.push_back(two_new[j+k]->GetIndex());
				}
				ret.push_back(vv);
			}
		}
	}
}

void CardHelp_3Lai::CheckThreeFlyToSingle(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f,CardVec& handcard,Lint count, std::vector<std::vector<Lint>>& ret)
{
	if(handcard.size() < (size_t)count)
		return;

	//首先提取3张
	CardVec tmp;
	for(Lsize i = 0 ; i+2 < th.size(); i += 3)
	{
		if (th[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(th[i]);
			tmp.push_back(th[i+1]);
			tmp.push_back(th[i+2]);
		}
	}
	for(Lsize i = 0 ; i+3 < f.size(); i += 4)
	{
		if (f[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(f[i]);
			tmp.push_back(f[i+1]);
			tmp.push_back(f[i+2]);
		}
	}
	CardStyle_3Lai::Instance()->SortCard(tmp);

	Lint step = count/4; //飞机带单中三张个数;

	for(Lsize i = 0 ; i+step*3 <= tmp.size(); i += 3)
	{
		CardVec vec;
		CardVec check(tmp.begin()+i,tmp.end());
		Lint type = check.front()->GetNumberType();
		vec.push_back(check.front());
		vec.push_back(*(check.begin()+1));
		vec.push_back(*(check.begin()+2));
		for(Lsize j = 3 ; j+2 < check.size(); j += 3)
		{
			if(check[j]->GetNumberType()+1 == type)
			{
				type -= 1;
				vec.push_back(check[j]);
				vec.push_back(check[j+1]);
				vec.push_back(check[j+2]);
				if(vec.size() == step*3)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if(vec.size() == step*3 && vec.front()->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			for(Lsize j = 0; j < vec.size(); ++j)
			{
				v.push_back(vec[j]->GetIndex());
			}

			CardVec h = handcard;

			// 提取除了飞机之外单牌;
			CardStyle_3Lai::Instance()->EarseCard(h,vec);
			for(Lsize j = 0; j + step <= h.size(); ++j)
			{
				std::vector<Lint> vv = v;
				for(Lint k=0;k<step; ++k)
				{
					vv.push_back(h[j+k]->GetIndex());
				}
				ret.push_back(vv);
			}
		}
	}
}

void CardHelp_3Lai::CheckOneList(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f,Lint count, std::vector<std::vector<Lint>>& ret)
{
	//首先提取单张;
	CardVec tmp = o;
	for(Lsize i = 0 ; i < t.size(); i += 2)
	{
		tmp.push_back(t[i]);
	}
	for(Lsize i = 0 ; i < th.size(); i += 3)
	{
		tmp.push_back(th[i]);
	}
	for(Lsize i = 0 ; i < f.size(); i += 4)
	{
		tmp.push_back(f[i]);
	}
	CardStyle_3Lai::Instance()->SortCard(tmp);

	if(tmp.size() < (size_t)count)
		return;

	//把2和大小王去掉排除掉;
	while(tmp.size() > 0 && (tmp.front()->GetNumberType() == card_number_type_2 || tmp.front()->GetNumberType() == card_number_type_SJ ||tmp.front()->GetNumberType() == card_number_type_BJ))
	{
		tmp.erase(tmp.begin());
	}

	for(Lsize i = 0 ; i+count <= tmp.size(); ++i)
	{
		CardVec vec;
		CardVec check(tmp.begin()+i,tmp.end());
		Lint type = check.front()->GetNumberType();
		vec.push_back(check.front());
		for(Lsize j = 1 ; j < check.size(); ++j)
		{
			if(check[j]->GetNumberType()+1 == type)
			{
				type -= 1;
				vec.push_back(check[j]);
				if(vec.size() == count)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if(vec.size() == count && vec.front()->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			for(Lsize j = 0; j < vec.size(); ++j)
			{
				v.push_back(vec[j]->GetIndex());
			}
			ret.push_back(v);
		}
	}
}

void CardHelp_3Lai::CheckTwoList(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f,Lint count, std::vector<std::vector<Lint>>& ret)
{
	CardVec tmp;
	for(Lsize i = 0 ; i+1 < t.size(); i += 2)
	{
		if (t[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(t[i]);
			tmp.push_back(t[i+1]);
		}
	}
	for(Lsize i = 0 ; i+1 < th.size(); i += 3)
	{
		if (th[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(th[i]);
			tmp.push_back(th[i+1]);
		}
	}
	for(Lsize i = 0 ; i+1 < f.size(); i += 4)
	{
		if (f[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(f[i]);
			tmp.push_back(f[i+1]);
		}
	}
	CardStyle_3Lai::Instance()->SortCard(tmp);

	for(Lsize i = 0 ; i+count <= tmp.size(); i += 2)
	{
		CardVec vec;
		CardVec check(tmp.begin()+i,tmp.end());
		Lint type = check.front()->GetNumberType();
		vec.push_back(check.front());
		vec.push_back(*(check.begin()+1));
		for(Lsize j = 2 ; j < check.size(); j += 2)
		{
			if(check[j]->GetNumberType()+1 == type)
			{
				type -= 1;
				vec.push_back(check[j]);
				vec.push_back(check[j+1]);
				if(vec.size() == count)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if(vec.size() == count && vec.front()->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			for(Lsize j = 0; j < vec.size(); ++j)
			{
				v.push_back(vec[j]->GetIndex());
			}
			ret.push_back(v);
		}
	}
}

void CardHelp_3Lai::CheckFourToTwo(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret)
{
	CardVec tmp = o;
	tmp.insert(tmp.end(),t.begin(),t.end());
	tmp.insert(tmp.end(),th.begin(),th.end());
	tmp.insert(tmp.end(),f.begin(),f.end());
	CardStyle_3Lai::Instance()->SortCard(tmp);
	if(tmp.size() < 6)
		return;

	for(Lsize i = 0 ; i+3 < f.size(); i+=4)
	{
		if(f[i]->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			v.push_back(f[i]->GetIndex());
			v.push_back(f[i+1]->GetIndex());
			v.push_back(f[i+2]->GetIndex());
			v.push_back(f[i+3]->GetIndex());

			CardVec m = tmp;
			CardVec mm(f.begin()+i,f.begin()+i+4);
			CardStyle_3Lai::Instance()->EarseCard(m,mm);

			for(Lsize j = 0 ; j + 1 < m.size(); ++j)
			{
				std::vector<Lint> vv = v;
				if ( (m[j]->GetNumberType() == card_number_type_BJ && m[j+1]->GetNumberType() == card_number_type_SJ ) ||
					(m[j]->GetNumberType() == card_number_type_SJ && m[j+1]->GetNumberType() == card_number_type_BJ ) )
				{
					//双王不能参与四带二;
				}
				else
				{
					vv.push_back(m[j]->GetIndex());
					vv.push_back(m[j+1]->GetIndex());
					ret.push_back(vv);
				}
			}
		}
	}
}

void CardHelp_3Lai::CheckThree(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f, std::vector<std::vector<Lint>>& ret)
{
	for(Lsize i = 0 ; i+2 < th.size(); i+=3)
	{
		if(th[i]->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			v.push_back(th[i]->GetIndex());
			v.push_back(th[i+1]->GetIndex());
			v.push_back(th[i+2]->GetIndex());
			ret.push_back(v);
		}
	}
}

void CardHelp_3Lai::CheckThreeList(Lint cardType,CardVec& o,CardVec& t,CardVec& th, CardVec& f,CardVec& handcard,Lint count, std::vector<std::vector<Lint>>& ret)
{
	CardVec tmp;
	for(Lsize i = 0 ; i+2 < th.size(); i += 3)
	{
		if (th[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(th[i]);
			tmp.push_back(th[i+1]);
			tmp.push_back(th[i+2]);
		}
		else
			continue;
	}
	for(Lsize i = 0 ; i+2 < f.size(); i += 4)
	{
		if (f[i]->GetNumberType() <= card_number_type_A)
		{
			tmp.push_back(f[i]);
			tmp.push_back(f[i+1]);
			tmp.push_back(f[i+2]);
		}
		else
			continue;
	}
	CardStyle_3Lai::Instance()->SortCard(tmp);

	for(Lsize i = 0 ; i+count <= tmp.size(); i += 3)
	{
		CardVec vec;
		CardVec check(tmp.begin()+i,tmp.end());
		Lint type = check.front()->GetNumberType();
		vec.push_back(check.front());
		vec.push_back(*(check.begin()+1));
		vec.push_back(*(check.begin()+2));
		for(Lsize j = 3 ; j < check.size(); j += 3)
		{
			if(check[j]->GetNumberType()+1 == type)
			{
				type -= 1;
				vec.push_back(check[j]);
				vec.push_back(check[j+1]);
				vec.push_back(check[j+2]);
				if(vec.size() == count)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if(vec.size() == count && vec.front()->GetNumberType() > cardType)
		{
			std::vector<Lint> v;
			for(Lsize j = 0; j < vec.size(); ++j)
			{
				v.push_back(vec[j]->GetIndex());
			}
			ret.push_back(v);
		}
	}
}