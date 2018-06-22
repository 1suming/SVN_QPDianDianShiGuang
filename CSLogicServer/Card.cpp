#include "Card.h"
#include "LTool.h"
#include "LLog.h"
#include "LTime.h"

static bool CardSortFun(Card* c1,Card* c2)
{
	if(c2->m_color > c1->m_color)
	{
		return true;
	}
	else if(c2->m_color == c1->m_color)
	{
		return c2->m_number > c1->m_number;
	}
	
	return false;
}

bool CardManager::Init()
{
	//万
	Lint index = 0;
	for (Lint k = 0; k < 9; k ++)  //9张牌
	{
		for(Lint i = 0; i < 4 ;++i)		//循环加四次
		{
				
			wan_base_card[index].m_color = 1;
			wan_base_card[index].m_number = k + 1;
			index ++ ;
		}
	}
	
	//饼
	index = 0;
	for (Lint k = 0; k < 9; k ++)  //9张牌
	{
		for(Lint i = 0; i < 4 ;++i)		//循环加四次
		{

			bing_base_card[index].m_color = 2;
			bing_base_card[index].m_number = k + 1;
			index ++ ;
		}
	}

	//条
	index = 0;
	for (Lint k = 0; k < 9; k ++)  //9张牌
	{
		for(Lint i = 0; i < 4 ;++i)		//循环加四次
		{

			tiao_base_card[index].m_color = 3;
			tiao_base_card[index].m_number = k + 1;
			index ++ ;
		}
	}

	index = 0;
	//风+字
	for (Lint j = 0; j < 7; j ++)	
	{
		for(Lint i = 0; i < 4 ;++i)		//循环加四次
		{
			wind_card[index].m_color = 4;
			wind_card[index].m_number = j+1;
			index ++ ;
		}	
	}

	index = 0;
	for(Lint i = 0; i < 4 ;++i)		//循环加四次
	{
		Lai_zi_card[index].m_color = 4;
		Lai_zi_card[index].m_number = 5;
		index ++ ;
	}	

	for(Lint i = 0 ; i < BASE_CARD_COUNT; ++i)
	{
		m_3_feng.push_back(&wan_base_card[i]);
		m_3_feng.push_back(&bing_base_card[i]);
		m_3_feng.push_back(&tiao_base_card[i]);

		m_3_laizi.push_back(&wan_base_card[i]);
		m_3_laizi.push_back(&bing_base_card[i]);
		m_3_laizi.push_back(&tiao_base_card[i]);

		if (i < 4 || i>31)
		{
			m_ZhuoXiaZi.push_back(&wan_base_card[i]);
		}
		m_ZhuoXiaZi.push_back(&bing_base_card[i]);
		m_ZhuoXiaZi.push_back(&tiao_base_card[i]);

		m_wan_feng.push_back(&wan_base_card[i]);

		m_3.push_back(&wan_base_card[i]);
		m_3.push_back(&bing_base_card[i]);
		m_3.push_back(&tiao_base_card[i]);
	}

	for(Lint i = 0 ; i < WIND_CARD_COUNT; ++i)
	{
		m_3_feng.push_back(&wind_card[i]);
		m_wan_feng.push_back(&wind_card[i]);
	}
	for(Lint i = 0 ; i < LAI_ZI_COUNT; ++i)
	{
		m_3_laizi.push_back(&Lai_zi_card[i]);
	}

	Lint temp = 0;
	for (Lint i = 1; i < 4; i++)
	{
		yao_13[temp].m_color = i;
		yao_13[temp].m_number = 1;
		temp++;
		yao_13[temp].m_color = i;
		yao_13[temp].m_number = 9;
		temp++;
	}
	for (Lint i = 1; i < 8; i++)
	{
		yao_13[temp].m_color = 4;
		yao_13[temp].m_number = i;
		temp++;
	}
	for (Lint i = 0; i < 13; i++)
	{
		m_13yao.push_back(&yao_13[i]);
	}


	SortCard(m_13yao);
	SortCard(m_3_feng);
	SortCard(m_3_laizi);
	SortCard(m_wan_feng);
	SortCard(m_3);
	SortCard(m_ZhuoXiaZi);
	return true;
}

bool CardManager::Final()
{
	return true;
}

void CardManager::SwapCardBySpecial(CardVector& cvIn, const Card specialCard[CARD_COUNT])
{
	Lint nCardCount = cvIn.size();
	for (Lint i = 0; i < nCardCount; i ++)
	{
		if (specialCard[i].m_color == 0 || specialCard[i].m_number == 0)
		{
			break;
		}
		for (Lint j = i + 1; j < nCardCount; j ++)
		{
			if (cvIn[j]->m_color == specialCard[i].m_color && cvIn[j]->m_number == specialCard[i].m_number)
			{
				Card* tmp = cvIn[j];
				cvIn[j] = cvIn[i];
				cvIn[i] = tmp;	
				break;
			}
		}
	}
}

void CardManager::DealCard2(CardVector hand_card[],Lint user_count,CardVector& rest_card,const Card specialCard[CARD_COUNT],Lint game_type, PlayTypeInfo& playtype)
{

	CardVector mCards;
	Lint choose=0;
	if (game_type == ShuangKaiMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == ZhuanZhuanMajiang || game_type == ChangShaMaJiang)
	{
		if (playtype.H_TwoPlayer)
		{
			mCards = m_wan_feng;
		}
		else {
			if (playtype.H_HaveWind)
			{
				mCards = m_3_feng;
				choose = 1;
			}
			else if (playtype.H_HongZhongLaiZi)
			{
				mCards = m_3_laizi;
				choose = 2;
			}
			else {
				mCards = m_3;
				choose = 3;
			}
		}
	}
	else if (game_type == HongZhongMajiang)
	{
		mCards = m_3_laizi;
		choose = 2;
	}
	else if (game_type == ShaoYangMajiang)
	{
		if (playtype.H_HaveWind)
		{
			mCards = m_3_feng;
			choose = 1;
		}
		else {
			mCards = m_3;
			choose = 3;
		}
	}
	else if (game_type == ChenZhouMajiang|| game_type == ChangDeMajiang || game_type == HengYangMajiang)
	{
		if (playtype.H_HongZhongLaiZi)
		{
			mCards = m_3_laizi;
			choose = 2;
		}
		else {
			mCards = m_3;
			choose = 3;
		}
	}
	else if (game_type == JingMenMajiang)
	{
		mCards = m_3_laizi;
		choose = 2;
	}
	else if (game_type == XiaoYiMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == YongZhouMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == XiangTanMajiang || game_type == YiYangMajiang || game_type == ChaoGuMajiang)
	{
		mCards = m_3;
		choose = 3;
	}
	else if (game_type == ZhuoXiaZiMajiang)
	{
		mCards = m_ZhuoXiaZi;
		choose = 4;
	}

	CardVector cardtmp;
	Lint nSize = mCards.size();
	while (nSize > 0)
	{
		Lint seed1 = L_Rand(0, nSize - 1);
		cardtmp.push_back(mCards[seed1]);
		mCards.erase(mCards.begin() + seed1, mCards.begin() + seed1 + 1);
		nSize = mCards.size();
	}
	
	SwapCardBySpecial(cardtmp, specialCard);

	for(int x=0;x<user_count;x++)
	{
		hand_card[x].insert(hand_card[x].end(),cardtmp.begin()+13*x,cardtmp.begin()+13*(x+1));
		SortCard(hand_card[x]);
	}
	rest_card.insert(rest_card.end(),cardtmp.begin()+13*user_count,cardtmp.end());
	std::reverse(rest_card.begin(),rest_card.end());		//逆序桌上牌
}

void CardManager::DealCard(CardVector hand_card[],Lint user_count,CardVector& rest_card, Lint game_type, PlayTypeInfo& playtype)
{
	CardVector mCards;
	Lint choose=0;
	if (game_type == ShuangKaiMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == ZhuanZhuanMajiang || game_type == ChangShaMaJiang)
	{
		if (playtype.H_TwoPlayer)
		{
			mCards = m_wan_feng;
		}
		else {
			if (playtype.H_HaveWind)
			{
				mCards = m_3_feng;
				choose = 1;
			}
			else if (playtype.H_HongZhongLaiZi)
			{
				mCards = m_3_laizi;
				choose = 2;
			}
			else {
				mCards = m_3;
				choose = 3;
			}
		}
	}
	else if (game_type == HongZhongMajiang)
	{
		mCards = m_3_laizi;
		choose = 2;
	}
	else if (game_type == ShaoYangMajiang)
	{
		if (playtype.H_HaveWind)
		{
			mCards = m_3_feng;
			choose = 1;
		}
		else {
			mCards = m_3;
			choose = 3;
		}
	}
	else if (game_type == ChenZhouMajiang || game_type == ChangDeMajiang || game_type == HengYangMajiang)
	{
		if (playtype.H_HongZhongLaiZi)
		{
			mCards = m_3_laizi;
			choose = 2;
		}
		else {
			mCards = m_3;
			choose = 3;
		}
	}
	else if (game_type == JingMenMajiang)
	{
		mCards = m_3_laizi;
		choose = 2;
	}
	else if (game_type == XiaoYiMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == YongZhouMajiang)
	{
		mCards = m_3_feng;
		choose = 1;
	}
	else if (game_type == XiangTanMajiang || game_type == YiYangMajiang || game_type == ChaoGuMajiang)
	{
		mCards = m_3;
		choose = 3;
	}
	else if (game_type == ZhuoXiaZiMajiang)
	{
		mCards = m_ZhuoXiaZi;
		choose = 4;
	}

	CardVector cardtmp;
	Lint nSize = mCards.size();
	//设置随机因子
	LTime timeNow;
	srand(timeNow.Secs());
	while (nSize > 0)
	{
		Lint seed1 = L_Rand(0, nSize - 1);
		cardtmp.push_back(mCards[seed1]);
		mCards[seed1] = mCards[nSize - 1];
		nSize --;
	}

	switch(choose)
	{
	case 0:
		m_wan_feng=cardtmp;
		break;
	case 1:
		m_3_feng=cardtmp;
		break;
	case 2:
		m_3_laizi=cardtmp;
		break;
	case 3:
		m_3 = cardtmp;
		break;
	case 4:
		m_ZhuoXiaZi = cardtmp;
		break;
	}

	for(int x=0;x<user_count;x++)
	{
		hand_card[x].insert(hand_card[x].end(),cardtmp.begin()+13*x,cardtmp.begin()+13*(x+1));
		SortCard(hand_card[x]);
	}
	rest_card.insert(rest_card.end(),cardtmp.begin()+13*user_count,cardtmp.end());
	LLOG_DEBUG("1cardtmp:%d,handcard:%d,rest_card:%d\n", cardtmp.size(), hand_card[0].size(), rest_card.size());
}

void CardManager::SortCard(CardVector& vec)
{
	std::sort(vec.begin(),vec.end(),CardSortFun);
}

void CardManager::EraseCard(CardVector& src,CardVector& des)
{
	//这里直接比较地址是否相等
	CardVector::iterator it2 = des.begin();
	for(; it2 != des.end(); ++it2)
	{
		CardVector::iterator it1 = src.begin();
		for(; it1 != src.end();)
		{
			if((*it1) == (*it2))
			{
				it1 = src.erase(it1);
			}
			else
			{
				++it1;
			}
		}
	}
}

void CardManager::EraseCard(CardVector& src,Card* pCard)
{
	//这里直接比较地址是否相等
	for(Lsize i = 0 ; i  < src.size(); ++i)
	{
		if(src[i] == pCard)
		{
			src.erase(src.begin()+i);
			break;
		}
	}
}

//删除src 与pCard 数据一样的牌 N张
void CardManager::EraseCard(CardVector& src,Card* pCard,Lint n)
{
	Lint cnt = 0;
	CardVector::iterator it2 = src.begin();
	for(; it2 != src.end();)
	{
		if(cnt >= n)
			break;

		if(IsSame(pCard,*it2))
		{
			cnt ++;
			it2 = src.erase(it2);
		}
		else
		{
			++it2;
		}
	}
}

void CardManager::EarseSameCard(CardVector& src)
{
	if(src.empty())
	{
		return;
	}

	Lint color = src.front()->m_color;
	Lint number = src.front()->m_number;
	CardVector::iterator it = src.begin()+1;
	for(; it != src.end();)
	{
		if(color == (*it)->m_color && number == (*it)->m_number)
		{
			it = src.erase(it);
		}
		else
		{
			color = (*it)->m_color;
			number= (*it)->m_number;
			++it;
		}
	}
}

void _S_GGetCard(Card & card, Lint index)
{
	card.m_color = index / 10 + 1;
	card.m_number = (index % 10) / 1;
}

void _S_GGetShunKeCard(std::vector<Lint>& card_vector, Card & card, Lint index, Lint shunke)
{
	_S_GGetCard(card, index);
}

bool _S_G_HaveCard(std::vector<Card>& check_cards, Card* card)
{
	Lint sz = check_cards.size();
	for (Lint i = 0; i < sz; ++i)
	{
		if (gCardMgr.IsSame(&check_cards[i], card))
		{
			return true;
		}
	}
	return false;
}

bool CardManager::XiaoYi_CheckTing_GetHuCards(std::vector<Card>& hu_cards, CardVector handcard, CardVector pengCard, CardVector agangCard, CardVector mgangCard, CardVector eatCard)
{
	std::vector<Lint> huIndexs;
	std::vector<Lint> huShunKes;
	std::vector<Lint> card_vector(40, 0);

	for (int x = 0; x<handcard.size(); x++)
	{
		card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
	}

	if (XiaoYi_CheckTing(card_vector, huIndexs, huShunKes, handcard, pengCard, agangCard, mgangCard, eatCard))
	{
		Lint sz = huShunKes.size();

		Card card;
		for (Lint i = 0; i < sz; ++i)
		{
			_S_GGetShunKeCard(card_vector, card, huIndexs[i], huShunKes[i]);
			if (card.m_color <= 3) //摸到4点以上才能胡
			{
				if (card.m_number >= 4 && !_S_G_HaveCard(hu_cards, &card))
				{
					hu_cards.push_back(card);
				}
			}
			else
			{
				hu_cards.push_back(card);
			} 
		}
	}

	return hu_cards.size() >= 1;
}

ThinkVec CardManager::CheckBaoTing(CardVector& handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, OperateState& gameInfo)
{
	ThinkVec result;
	ThinkUnit unit;
	if (gameInfo.m_GameType == XiaoYiMajiang)
	{
		std::vector<Card> hu_cards;
		if (XiaoYi_CheckTing_GetHuCards(hu_cards, handcard, pengCard, agangCard, mgangCard, eatCard))
		{
			unit.m_type = THINK_OPERATOR_TING;
			result.push_back(unit);
		}
	}
	else if (gameInfo.m_GameType == YiYangMajiang)
	{
		if (YiYang_CheckTing(handcard, pengCard, agangCard, mgangCard, eatCard))
		{
			unit.m_type = THINK_OPERATOR_TING;
			result.push_back(unit);
		}
	}
	return result;
}

ThinkVec CardManager::CheckOutCardOperator(CardVector& handcard,CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard, Card* outCard,OperateState& gameInfo)
{
	ThinkVec result;
	ThinkUnit unit;
	if(gameInfo.b_canHu&&CheckCanHu(handcard,pengCard,agangCard,mgangCard,eatCard, outCard,gameInfo,unit.m_hu, false))
	{
		unit.m_type = THINK_OPERATOR_BOMB;
		unit.m_card.push_back(outCard);
		if (gameInfo.m_GameType == ShuangKaiMajiang)
		{
			Lint playerscore = gameInfo.m_playerScore;
			if (playerscore >= gameInfo.i_canHuScore)
			{
				result.push_back(unit);
			}
		}
		else if (gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
		{
			unit.count_score();
			if (unit.m_score>gameInfo.i_canHuScore)
				result.push_back(unit);
		}
		else if (gameInfo.m_GameType == ZhuanZhuanMajiang)
		{
			unit.count_score();
			if (unit.m_score>gameInfo.i_canHuScore)
				result.push_back(unit);
		}
		else if (gameInfo.m_GameType == HongZhongMajiang)
		{
			unit.count_score();
			if (unit.m_score>gameInfo.i_canHuScore)
				result.push_back(unit);
		}
		else if (gameInfo.m_GameType == JingMenMajiang || YongZhouMajiang == gameInfo.m_GameType)
		{
			Lint sz = unit.m_hu.size();
			Lint playerscore = gameInfo.m_playerScore;
			for (Lint i = 0; i < sz; ++i)
			{
				if (unit.m_hu[i] == HU_YINGHU)
				{
					++playerscore;
				}
			}

			if (playerscore >= gameInfo.i_canHuScore)
			{
				result.push_back(unit);
			}
		}
		else if(gameInfo.m_GameType == ShaoYangMajiang)
		{
			if (gameInfo.m_gangShangPao)
			{
				unit.m_hu_way = 4;
			}
			else if (gameInfo.m_qiangMingGang)
			{
				unit.m_hu_way = 2;
			}
			else if (gameInfo.m_qiangAnGang)
			{
				unit.m_hu_way = 3;
			}
			else if (gameInfo.m_qiangMing2Gang)
			{
				unit.m_hu_way = 2;
			}
			result.push_back(unit);
		}
		else if (gameInfo.m_GameType == ChenZhouMajiang)
		{
			if (gameInfo.m_qiangMingGang)
			{
				unit.m_hu_way = 2;
			}
			result.push_back(unit);
		}
		else if (gameInfo.m_GameType == ChangDeMajiang)
		{
			if (gameInfo.m_qiangMingGang)
			{
				unit.m_hu_way = 2;
			}
			result.push_back(unit);
		}
		else
		{
			result.push_back(unit);
		}
	}
	if (!gameInfo.b_canHu && gameInfo.m_GameType == YongZhouMajiang && gameInfo.m_first_turn &&CheckQishouWangBa(handcard, gameInfo))
	{
		unit.m_type = THINK_OPERATOR_BOMB;
		unit.m_card.push_back(outCard);
		unit.m_hu.push_back(HU_QiShou4WangBa);
	}
	if (gameInfo.m_GameType == XiaoYiMajiang && gameInfo.b_onlyHu)
	{
		if (gameInfo.b_guoGang)
		{
			return result;
		}
		if (CheckCanGang(handcard, outCard, gameInfo))
		{
			if(CheckXiaoYiCanGang(handcard, handcard, pengCard, agangCard, mgangCard, eatCard, outCard))
			{
				unit.Clear();
				unit.m_type = THINK_OPERATOR_MBU;
				unit.m_card.push_back(outCard);
	
				result.push_back(unit);
				gameInfo.m_gangcard.push_back(outCard);
			}
		}
		return result;
	}
	if (!gameInfo.b_onlyHu)
	{
		if(CheckCanGang(handcard,outCard,gameInfo))
		{
			unit.Clear();
			unit.m_type = THINK_OPERATOR_MBU;
			unit.m_card.push_back(outCard);
			if (gameInfo.m_GameType == YongZhouMajiang || gameInfo.m_GameType == JingMenMajiang || gameInfo.m_GameType == ShuangKaiMajiang)
			{
				if (!gCardMgr.HaveTheCard(pengCard, outCard))
				{
					result.push_back(unit);
				}
			}
			else if (gameInfo.m_GameType == XiangTanMajiang)
			{
				if (XiangTanCheckTing(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, false, gameInfo))
				{
					result.push_back(unit);
				}
			}
			else
			{
				result.push_back(unit);
			}
			gameInfo.m_gangcard.push_back(outCard);
			if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
			{
				CardVector tmp_handcard(handcard);
				CardVector tmp_mgangCard(mgangCard);

				EraseCard(tmp_handcard,outCard,3);
				for(int x=0;x<4;x++)
					tmp_mgangCard.push_back(outCard);
				if(CheckTing(tmp_handcard,pengCard,agangCard,tmp_mgangCard,eatCard,gameInfo))
				{
					unit.Clear();
					unit.m_type = THINK_OPERATOR_MGANG;
					unit.m_card.push_back(outCard);
					result.push_back(unit);
				}
			}
		}
		if(!gameInfo.b_guoGang &&CheckCanPeng(handcard,outCard,gameInfo))
		{
			unit.Clear();   
			unit.m_type = THINK_OPERATOR_PENG;
			unit.m_card.push_back(outCard);
			result.push_back(unit);
		}

		std::vector<Card*> vec;
		if(!gameInfo.b_guoGang &&gameInfo.b_canEat && CheckCanChi(handcard,outCard,vec,gameInfo))
		{
			for(Lsize i = 0 ; i < vec.size(); i += 3)
			{
				LLOG_DEBUG("canEat vec: %d,%d,,%d,%d,,%d,%d", vec[0]->m_color, vec[0]->m_number, vec[1]->m_color, vec[1]->m_number, vec[2]->m_color, vec[2]->m_number);
				unit.Clear();
				unit.m_type = THINK_OPERATOR_CHI;
				unit.m_card.push_back(vec[i]);
				unit.m_card.push_back(vec[i+1]);
				unit.m_card.push_back(vec[i+2]);
				result.push_back(unit);
			}
		}
	}

	return result;
}

ThinkVec CardManager::CheckOutCardOperator(CardVector& handcard,CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard, CardVector& outCards,OperateState& gameInfo,LMsgS2CUserGangThinkCard& send_message)
{
	ThinkVec result;
	ThinkUnit unit;
	Lint	hu_count=0;
	gang_think the_gank_think[4];
	for (int i = 0; i < outCards.size(); i ++)
	{
		if(!outCards[i])
		{
			return result;
		}
		
		the_gank_think[i].m_gang_card.m_color = outCards[i]->m_color;
		the_gank_think[i].m_gang_card.m_number = outCards[i]->m_number;
	
		unit.Clear();
		if(gameInfo.b_canHu && !gCardMgr.HaveTheCard(gameInfo.m_louhuCard, outCards[i]) && CheckCanHu(handcard,pengCard,agangCard,mgangCard,eatCard, outCards[i],gameInfo,unit.m_hu,false))
		{
			if (hu_count> 0)
			{
				result[0].m_card.push_back(outCards[i]);
				for (int i = 0; i < unit.m_hu.size(); i ++)
				{
					result[0].m_hu.push_back(unit.m_hu[i]);
				}
			}
			else
			{
				hu_count++;
				unit.m_type = THINK_OPERATOR_BOMB;
				unit.m_card.push_back(outCards[i]);
				result.push_back(unit);
			}
			ThinkData think;
			think.m_type = THINK_OPERATOR_BOMB;
			think.m_card.push_back(the_gank_think[i].m_gang_card);
			the_gank_think[i].m_think.push_back(think);
		}
	}

	if (gameInfo.m_GameType == XiangTanMajiang)
	{

	}
	else
	{
		if (!result.empty())
		{
			result[0].count_score();
			if (result[0].m_score > gameInfo.i_canHuScore)
			{

			}
			else {
				result.clear();
				for (int x = 0; x < send_message.m_card.size(); x++)
				{
					send_message.m_card[x].m_think.clear();
				}
			}
		}
	}

	for (int xx = 0; xx < outCards.size(); xx ++)
	{
		if (!gameInfo.b_onlyHu)
		{
			if(CheckCanGang(handcard,outCards[xx], gameInfo))
			{	
				unit.Clear();
				unit.m_type = THINK_OPERATOR_MBU;
				unit.m_card.push_back(outCards[xx]);

				if (gameInfo.m_GameType == XiangTanMajiang)
				{
					if (XiangTanCheckTing(handcard, pengCard, agangCard, mgangCard, eatCard, NULL, false, gameInfo))
					{
						result.push_back(unit);
					}
				}
				else
				{
					result.push_back(unit);
				}
				

				ThinkData think;
				think.m_type = THINK_OPERATOR_MBU;
				think.m_card.push_back(the_gank_think[xx].m_gang_card);
				the_gank_think[xx].m_think.push_back(think);
				if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
				{
					CardVector tmp_handcard(handcard);
					CardVector tmp_mgangCard(mgangCard);

					EraseCard(tmp_handcard,outCards[xx],3);
					for(int x=0;x<4;x++)
						tmp_mgangCard.push_back(outCards[xx]);

					if(CheckTing(tmp_handcard,pengCard,agangCard,tmp_mgangCard,eatCard,gameInfo))
					{
						unit.Clear();
						unit.m_type = THINK_OPERATOR_MGANG;
						unit.m_card.push_back(outCards[xx]);
						result.push_back(unit);

						ThinkData think;
						think.m_type = THINK_OPERATOR_MGANG;
						think.m_card.push_back(the_gank_think[xx].m_gang_card);
						the_gank_think[xx].m_think.push_back(think);
					}
				}
			}
			if(!gameInfo.b_guoGang && CheckCanPeng(handcard,outCards[xx], gameInfo))
			{
				unit.Clear();
				unit.m_type = THINK_OPERATOR_PENG;
				unit.m_card.push_back(outCards[xx]);
				result.push_back(unit);

				ThinkData think;
				think.m_type = THINK_OPERATOR_PENG;
				think.m_card.push_back(the_gank_think[xx].m_gang_card);
				the_gank_think[xx].m_think.push_back(think);
			}

			std::vector<Card*> vec;
			if(!gameInfo.b_guoGang && gameInfo.b_canEat && CheckCanChi(handcard,outCards[xx],vec, gameInfo))
			{
				for(Lsize i = 0 ; i+2 < vec.size(); i += 3)
				{
					unit.Clear();
					unit.m_type = THINK_OPERATOR_CHI;
					unit.m_card.push_back(vec[i]);
					unit.m_card.push_back(vec[i+1]);
					unit.m_card.push_back(vec[i+2]);
					result.push_back(unit);

					ThinkData think;
					think.m_type = THINK_OPERATOR_CHI;
					CardValue card;
					if(vec[i])
					{
						card.m_color = vec[i]->m_color;
						card.m_number = vec[i]->m_number;
						think.m_card.push_back(card);
					}
					if(vec[i+1])
					{
						card.m_color = vec[i+1]->m_color;
						card.m_number = vec[i+1]->m_number;
						think.m_card.push_back(card);
					}
					if(vec[i+2])
					{	
						card.m_color = vec[i+2]->m_color;
						card.m_number = vec[i+2]->m_number;
						think.m_card.push_back(card);
					}
					the_gank_think[xx].m_think.push_back(think);
				}
			}
		}
		send_message.m_card.push_back(the_gank_think[xx]);
	}
	
	return result;
}
bool CardManager::CheckXiaoYiCanGang(CardVector& handcard1, CardVector& handcard2, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* outCard)
{
	if (!outCard) return false;
	bool result = false;
	CardVector tmp_handcard1 = handcard1;
	CardVector tmp_handcard2 = handcard2;
	std::vector<Card> hu_cards;
	std::vector<Card> hu_cards2;
	if (!XiaoYi_CheckTing_GetHuCards(hu_cards, tmp_handcard1, pengCard, agangCard, mgangCard, eatCard))
	{
		return result;
	}
	EraseCard(tmp_handcard2, outCard, 4);
	if (!XiaoYi_CheckTing_GetHuCards(hu_cards2, tmp_handcard2, pengCard, agangCard, mgangCard, eatCard))
	{
		return result;
	}
	if (!hu_cards2.size())
	{
		return false;
	}
	return true;
}

ThinkVec CardManager::CheckGetCardOperator(CardVector& handcard, CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard,Card* getCard,OperateState& gameInfo)
{
	ThinkVec result;
	ThinkUnit unit;
	if(gameInfo.b_canHu&&CheckCanHu(handcard, pengCard,agangCard,mgangCard,eatCard,getCard,gameInfo,unit.m_hu,true))
	{
		unit.m_type = THINK_OPERATOR_BOMB;
		if (getCard)
		{
			unit.m_card.push_back(getCard);
		}
		if (gameInfo.m_GameType == ShuangKaiMajiang || gameInfo.m_GameType == JingMenMajiang)
		{
			Lint playerscore = gameInfo.m_playerScore;
			if (playerscore >= gameInfo.i_canHuScore)
			{
				if (gameInfo.m_gangShangHua)
				{
					unit.m_hu_way = 5;
					unit.m_score = gameInfo.m_playerPos;
				}
				result.push_back(unit);
			}
		}
		else
		{
			result.push_back(unit);
		}
	}

	if (gameInfo.m_GameType == XiaoYiMajiang && gameInfo.b_onlyHu)
	{
		CardVector tmp = handcard;
		if (getCard)
		{
			tmp.push_back(getCard);
		}
		SortCard(tmp);

		std::vector<Card*> vec;
		if (CheckAnGang(tmp, vec, gameInfo))
		{
			for (Lsize i = 0; i < vec.size(); ++i)
			{
				if (!CheckXiaoYiCanGang(handcard, tmp, pengCard, agangCard, mgangCard, eatCard, vec[i]))
				{
					continue;
				}
				unit.Clear();
				unit.m_type = THINK_OPERATOR_ABU;
				unit.m_card.push_back(vec[i]);
				result.push_back(unit);
			}
		}
		vec.clear();
		if (CheckMGang(tmp, pengCard, vec, gameInfo))
		{
			for (Lsize i = 0; i < vec.size(); ++i)
			{
				if (!CheckXiaoYiCanGang(handcard, tmp, pengCard, agangCard, mgangCard, eatCard, vec[i]))
				{
					continue;
				}
				unit.Clear();
				unit.m_type = THINK_OPERATOR_MBU;
				unit.m_card.push_back(vec[i]);
				result.push_back(unit);
			}
		}
	}
	if(!gameInfo.b_onlyHu)
	{
		CardVector tmp = handcard;
		if (getCard)
		{
			tmp.push_back(getCard);
		}
		SortCard(tmp);

		std::vector<Card*> vec;
		if(CheckAnGang(tmp, vec, gameInfo))
		{
			for(Lsize i = 0 ; i < vec.size(); ++i)
			{
				unit.Clear();
				unit.m_type = THINK_OPERATOR_ABU;
				unit.m_card.push_back(vec[i]);
				if (gameInfo.m_GameType == XiangTanMajiang)
				{
					if (XiangTanCheckTing(handcard, pengCard, agangCard, mgangCard, eatCard, getCard, true, gameInfo))
					{
						result.push_back(unit);
					}
				}
				else
				{
					result.push_back(unit);
				}
				if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
				{
					CardVector tmp_handcard=tmp;
					CardVector tmp_agangCard=agangCard;

					EraseCard(tmp_handcard,vec[i],4);
					for(int x=0;x<4;x++)
						tmp_agangCard.push_back(vec[i]);

					if(CheckTing(tmp_handcard,pengCard,tmp_agangCard,mgangCard,eatCard,gameInfo))
					{
						unit.m_type = THINK_OPERATOR_AGANG;
						result.push_back(unit);
					}
				}
			}
		}

		vec.clear();
		if(CheckMGang(tmp,pengCard,vec,gameInfo))
		{
			for(Lsize i = 0 ; i < vec.size(); ++i)
			{
				bool next=false;
				if (gameInfo.m_GameType == XiangTanMajiang)
				{
					if (!XiangTanCheckTing(handcard, pengCard, agangCard, mgangCard, eatCard, getCard, true, gameInfo))
					{
						next = true;
					}
				}
				//不允许先碰后杠
				if(gameInfo.m_GameType==ZhuanZhuanMajiang || gameInfo.m_GameType == YongZhouMajiang || gameInfo.m_GameType == JingMenMajiang ||gameInfo.m_GameType == ShaoYangMajiang || gameInfo.m_GameType == ChenZhouMajiang || gameInfo.m_GameType == ChangDeMajiang || gameInfo.m_GameType == YiYangMajiang)
				{
					for(int x=0;x<gameInfo.m_gangcard.size();x++)
					{
						if(gameInfo.m_gangcard[x]->m_color==vec[i]->m_color&&gameInfo.m_gangcard[x]->m_number==vec[i]->m_number)
						{
							next=true;
							break;
						}
					}
				}
				else if(gameInfo.m_GameType==HongZhongMajiang&&getCard)
				{
					if (getCard->m_color != vec[i]->m_color|| getCard->m_number != vec[i]->m_number)
					{
						continue;
					}	
				}

				if(!next)
				{
					unit.Clear();
					unit.m_type = THINK_OPERATOR_MBU;
					unit.m_card.push_back(vec[i]);
					result.push_back(unit);
					if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
					{
						CardVector tmp_handcard=tmp;
						EraseCard(tmp_handcard,vec[i],1);
						if(CheckTing(tmp_handcard,pengCard,mgangCard,mgangCard,eatCard,gameInfo))
						{
							unit.m_type = THINK_OPERATOR_MGANG;
							result.push_back(unit);
						}
					}
				}
			} 
		}
	}
	
	return result;
}

ThinkVec CardManager::CheckGetCardOperator(CardVector& handcard,CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard,CardVector& getCards, OperateState& gameInfo)
{
	ThinkVec result;
	ThinkUnit unit;
	for (int i = 0; i < getCards.size(); i ++)
	{
		unit.Clear();
		if(gameInfo.b_canHu && CheckCanHu(handcard, pengCard,agangCard,mgangCard,eatCard,getCards[i],gameInfo,unit.m_hu,true))
		{
			if (result.size() > 0)
			{
				result[0].m_card.push_back(getCards[i]);
				for (int i = 0; i < unit.m_hu.size(); i ++)
				{
					result[0].m_hu.push_back(unit.m_hu[i]);
				}
			}
			else
			{
				unit.m_type = THINK_OPERATOR_BOMB;
				unit.m_card.push_back(getCards[i]);
				result.push_back(unit);
			}
		}

		if(!gameInfo.b_onlyHu)
		{
			CardVector tmp = handcard;
			tmp.push_back(getCards[i]);
			SortCard(tmp);

			std::vector<Card*> vec;
			if(CheckAnGang(tmp,vec,gameInfo))
			{
				for(Lsize i = 0 ; i < vec.size(); ++i)
				{
					unit.Clear();
					unit.m_type = THINK_OPERATOR_ABU;
					unit.m_card.push_back(vec[i]);
					result.push_back(unit);
					if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
					{
						CardVector tmp_handcard(tmp);
						CardVector tmp_agangCard(agangCard);

						EraseCard(tmp_handcard,vec[i],4);
						for(int x=0;x<4;x++)
							tmp_agangCard.push_back(vec[i]);
						if(CheckTing(tmp_handcard,pengCard,tmp_agangCard,mgangCard,eatCard,gameInfo))
						{
							unit.m_type = THINK_OPERATOR_AGANG;
							result.push_back(unit);
						}
					}
				}
			}

			vec.clear();
			if(CheckMGang(tmp,pengCard,vec,gameInfo))
			{
				for(Lsize i = 0 ; i < vec.size(); ++i)
				{
					unit.Clear();
					unit.m_type = THINK_OPERATOR_MBU;
					unit.m_card.push_back(vec[i]);
					result.push_back(unit);

					if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
					{
						CardVector tmp_handcard(tmp);
						CardVector tmp_agangCard(agangCard);

						EraseCard(tmp_handcard,vec[i],4);
						for(int x=0;x<4;x++)
							tmp_agangCard.push_back(vec[i]);
						if(CheckTing(tmp_handcard,pengCard,tmp_agangCard,mgangCard,eatCard,gameInfo))
						{
							unit.m_type = THINK_OPERATOR_MGANG;
							result.push_back(unit);
						}
					}
				} 
			}
		}
	}
	return result;
}

bool CardManager::CheckStartHu(CardVector& handcard,std::vector<StartHuCard>& vec)
{
	vec.clear();

	//缺一色
	std::set<Lint> color_num;
	for(Lsize i = 0; i < handcard.size(); ++i)
		color_num.insert(handcard[i]->m_color);

	if(color_num.size() < 3)
	{
		StartHuCard info;
		info.m_type = START_HU_QUEYISE;
		vec.push_back(info);
	}

	//大四喜
	std::vector<Card*>angang;
	OperateState x;
	CheckAnGang(handcard,angang, x);
	if(angang.size())
	{
		StartHuCard info;
		info.m_type = START_HU_DASIXI;
		for(Lint i = 0 ; i < angang.size(); i += 1)
		{
			info.m_card.push_back(angang[i]);
		}
		vec.push_back(info);
	}

	//板板胡
	bool banban = true;
	for(Lsize i = 0 ; i < handcard.size(); ++i)
	{
		if(handcard[i]->m_number == 2 || handcard[i]->m_number == 5 ||
			handcard[i]->m_number == 8)
		{
			banban = false;
			break;
		}
	}

	if(banban)
	{
		StartHuCard info;
		info.m_type = START_HU_BANBANHU;
		vec.push_back(info);
	}

	//六六顺
	CardVector oneh, twoh, fourh, threeh;
	GetSpecialOneTwoThreeFour(handcard, oneh, twoh, threeh, fourh);
	if(threeh.size()/3 + fourh.size()/4 >= 2)
	{
		StartHuCard info;
		info.m_type = START_HU_LIULIUSHUN;
		for(Lint i = 0 ; i < threeh.size(); i += 3)
		{
			info.m_card.push_back(threeh[i]);
		}
		for(Lint i = 0 ; i < fourh.size(); i += 4)
		{
			info.m_card.push_back(fourh[i]);
		}
		vec.push_back(info);
	}

	return true;
}

bool CardManager::CheckCanHu_ShuangKai(CardVector& handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* outCard, OperateState& gameInfo, std::vector<Lint>& vec, bool isGetCard)
{
	if (gameInfo.m_GameType != ShuangKaiMajiang)
	{
		return false;
	}

	Lint laizi = 0;
	bool setquanqiurenkai = false;
	bool setYingHu = false;
	bool setFanShu = false;
	{
		Lsize sz = handcard.size();
		for (Lsize i = 0; i < sz; ++i)
		{
			Card * card = handcard[i];
			if (card->m_color == gameInfo.m_wangBa->m_color && card->m_number == gameInfo.m_wangBa->m_number)
			{
				++laizi;
			}
		}
		if (outCard && IsSame(outCard, gameInfo.m_wangBa))
		{
			++laizi;
		}
	}
	
	CardVector tmp_handcard = handcard;
	if (outCard)
	{
		tmp_handcard.push_back(outCard);
	}
	Lsize sz = tmp_handcard.size();
	CardVector removeHand;
	for (Lsize i = 0; i < sz; ++i)
	{
		Card * card = tmp_handcard[i];
		if (card->m_color == gameInfo.m_wangBa->m_color && card->m_number == gameInfo.m_wangBa->m_number)
		{
			removeHand.push_back(card);
		}
	}
	sz = removeHand.size();
	for (Lsize i = 0; i < sz; ++i)
	{
		EraseCard(tmp_handcard, removeHand[i]);
	}
	if (CheckLuanjianghu(tmp_handcard, eatCard, pengCard, agangCard, mgangCard, nullptr))
	{
		vec.push_back(HU_JIANGJIANGHU);
		if (!setFanShu) { gameInfo.m_playerScore += 2; setFanShu = true; }
		if (!isGetCard && gameInfo.m_dihu)
		{
			vec.push_back(HU_QingYiSeMenQing);
		}
		if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }

	}
	else if (CheckFengYiSe2(tmp_handcard, eatCard, pengCard, agangCard, mgangCard))
	{
		vec.push_back(HU_FengYiSe);
		if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
	}
	if (CheckQingyise2(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, vec, gameInfo.m_wangBa))
	{
		vec.push_back(HU_QINGYISE);
		if (!setFanShu) { gameInfo.m_playerScore += 2; setFanShu = true; }
		if (!isGetCard && gameInfo.m_dihu)
		{
			vec.push_back(HU_QingYiSeMenQing);
		}
		if (!laizi)
		{
			if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
		}
		else {
			if (CheckQingyise2(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, vec, nullptr))
			{
				if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
			}
		}
	}
	if (CheckPengpenghu(handcard, eatCard, outCard, gameInfo.m_wangBa))
	{
		vec.push_back(HU_PENGPENGHU);
		if (!setFanShu) { gameInfo.m_playerScore += 1; setFanShu = true; }
		if (!laizi)
		{
			if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
		}
		else {
			if (CheckPengpenghu(handcard, eatCard, outCard, nullptr))
			{
				if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
			}
		}
	}
	if (CheckQuanqiuren3(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo.m_wangBa))
	{
		CardVector check;
		check = handcard;
		if (outCard)
		{
			check.push_back(outCard);
		}
		bool haveset = false;

		if (eatCard.size())
		{
			if (check.size() == 2)
			{
				if (IsSame(check[0], gameInfo.m_wangBa) && IsSame(check[1], gameInfo.m_wangBa))
				{
					vec.push_back(HU_QUANQIUREN2);
					haveset = true;
				}
				else {
					if (IsSame(check[0], gameInfo.m_wangBa))
					{
						Card * ckcard = check[1];
						if (ckcard->m_color < 4 && (ckcard->m_number == 2 || ckcard->m_number == 5 || ckcard->m_number == 8))
						{
							vec.push_back(HU_QUANQIUREN2);
							haveset = true;
						}
					}
					else
					{
						Card * ckcard = check[0];
						if (ckcard->m_color < 4 && (ckcard->m_number == 2 || ckcard->m_number == 5 || ckcard->m_number == 8))
						{
							vec.push_back(HU_QUANQIUREN2);
							haveset = true;
						}
					}
				}
			}
		}
		else
		{
			vec.push_back(HU_QUANQIUREN);
			haveset = true;
		}
		if (haveset)
		{
			if (check.size() == 2)
			{
				if (!laizi || IsSame(check[0], check[1]))
				{
					if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
				}
			}
			setquanqiurenkai = true;
			if (CheckGangshangkaihua(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_GANG1);
				if (!setFanShu) { gameInfo.m_playerScore += 1; setFanShu = true; }
			}
		}
	}

	if (vec.size() <= 0)
	{
		//没有大胡才检测小胡
		if (laizi < 2 && xiaohu_CheckHu3(handcard, outCard, true, gameInfo.m_wangBa))
		{
			vec.push_back(HU_XIAOHU);
			if (!laizi)
			{
				if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; }
			}
			else {
				if (xiaohu_CheckHu3(handcard, outCard, true, nullptr))
				{
					if (!setYingHu) { vec.push_back(HU_YINGHU); setYingHu = true; ++gameInfo.m_playerScore; }
				}
			}
		}
	}

	if (vec.size())
	{
		if (CheckQiangganghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
		{
			vec.push_back(HU_QIANGGANGHU);
		}
		if (!setquanqiurenkai && CheckGangshangkaihua(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
		{
			vec.push_back(HU_GANG1);
			++gameInfo.m_playerScore;
		}
	}
	else
	{
		//单独检查抢杠胡
		if (xiaohu_CheckHu2(handcard, outCard, false, gameInfo.m_wangBa))
		{
			if (CheckQiangganghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_QIANGGANGHU);
			}
		}
	}

	return vec.size() != 0;
}

bool CardManager::CheckCanHu(CardVector& handcard,CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard, Card* outCard, OperateState& gameInfo,std::vector<Lint>& vec, bool isGetCard)
{ 
	//检测大胡
	if (gameInfo.m_GameType == ShuangKaiMajiang)
	{
		return CheckCanHu_ShuangKai(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, gameInfo, vec, isGetCard);
	}
	else if(gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
	{
		Lint isSpecial = 0;
		if (CheckXiaoqidui(handcard, isSpecial, outCard, isGetCard,gameInfo))
		{
			if(isSpecial == 2){
				vec.push_back(HU_DOUBLESPECIAL_XIAOQIDUI);
			}
			else if (isSpecial ==1)
			{
				vec.push_back(HU_SPECIAL_XIAOQIDUI);
			}
			else{
				vec.push_back(HU_XIAOQIDUI);
			}
		}
		if(CheckLuanjianghu(handcard,eatCard,pengCard,agangCard,mgangCard,outCard))
		{
			vec.push_back(HU_JIANGJIANGHU);
		}
		if (CheckPengpenghu(handcard, eatCard, outCard))
		{
			vec.push_back(HU_PENGPENGHU);
		}
		if (CheckQuanqiuren(handcard,eatCard,pengCard,agangCard,mgangCard,outCard))
		{
			vec.push_back(HU_QUANQIUREN);
		}
		if(CheckQingyise(handcard,eatCard,pengCard,agangCard,mgangCard,outCard, vec))
		{
			vec.push_back(HU_QINGYISE);
		}
		if (vec.size() <= 0)
		{
			//没有大胡才检测小胡
			if(xiaohu_CheckHu(handcard,outCard,true,!isGetCard))
				vec.push_back(HU_XIAOHU);
		}
		if (vec.size())
		{
			if(CheckQiangganghu(handcard,eatCard,pengCard,agangCard,mgangCard,outCard, gameInfo))
			{
				vec.push_back(HU_QIANGGANGHU);
			}
			if(CheckGangshangkaihua(handcard,eatCard,pengCard,agangCard,mgangCard,outCard,gameInfo))
			{
				vec.push_back(HU_GANG1);
			}
			if(CheckGangshangpao(handcard,eatCard,pengCard,agangCard,mgangCard,outCard,gameInfo))
			{
				vec.push_back(HU_GANG2);
			}
			if (CheckHaiDiLao(handcard,eatCard,pengCard,agangCard,mgangCard,outCard, gameInfo))
			{
				vec.push_back(HU_END1);
			}
			if (CheckHaiDiPao(handcard,eatCard,pengCard,agangCard,mgangCard,outCard, gameInfo))
			{
				vec.push_back(HU_END2);
			}
			if(gameInfo.m_tianhu)
			{
				vec.push_back(HU_TianHu);
			}
			if(gameInfo.m_dihu)
			{
				vec.push_back(HU_DiHu);
			}
		}

		if(vec.size() >= 2)
		{
			std::vector<Lint>::iterator it = vec.begin();
			for(; it !=  vec.end(); )
			{
				if((*it) == HU_XIAOHU)
				{
					vec.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}
		}
	}
	else if(gameInfo.m_GameType == ZhuanZhuanMajiang)
	{
		Lint isSpecial = 0;
		if(xiaohu_CheckHu(handcard,outCard,false,!isGetCard,gameInfo.m_pt_laizi))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (gameInfo.m_QiXiaoDui && CheckXiaoqidui(handcard, isSpecial, outCard, isGetCard,gameInfo))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if(isGetCard&&gameInfo.m_pt_laizi&&gameInfo.m_first_turn &&CheckQishouLaizi(handcard))
		{
			vec.push_back(HU_XIAOHU);
		}
	}
	else if(gameInfo.m_GameType == HongZhongMajiang)
	{
		Lint isSpecial = 0;
		if(xiaohu_CheckHu(handcard,outCard,false,!isGetCard,true))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (gameInfo.m_QiXiaoDui && CheckXiaoqidui(handcard, isSpecial, outCard, isGetCard,gameInfo))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if(isGetCard&&gameInfo.m_first_turn &&CheckQishouLaizi(handcard))
		{
			vec.push_back(HU_XIAOHU);
		}
	}
	else if(gameInfo.m_GameType == ShaoYangMajiang)
	{
		if (gameInfo.m_getCardThink||gameInfo.m_qiangMingGang||gameInfo.m_gangShangPao || gameInfo.m_qiangMing2Gang)
		{
			Lint isSpecial = 0;
			if (CheckShiSanYiao(handcard, outCard))
			{
				vec.push_back(HU_13Yao);
			}
			else if (CheckFengYiSe(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
			{
				vec.push_back(HU_FengYiSe);
			}
			else if (CheckMenQing(handcard, eatCard, pengCard, agangCard, mgangCard, outCard) && CheckQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
			{
				vec.push_back(HU_QingYiSeMenQing);
			}
			else if (CheckXiaoqidui(handcard, isSpecial, outCard, true, gameInfo) && CheckXiangTongSe(handcard, outCard))
			{
				vec.push_back(HU_QingYiSeQiXiaoDui);
			}
			else if (CheckQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, outCard) && CheckPengpenghu(handcard, eatCard, outCard))
			{
				vec.push_back(HU_QingYiSeDaDuiPeng);
			}
			else if (CheckXiaoqidui(handcard, isSpecial, outCard, true, gameInfo))
			{
				if (isSpecial > 0)
				{
					vec.push_back(HU_ShaoYangLongQiDui);
				}
				else
				{
					vec.push_back(HU_ShaoYangQiQiaoDui);
				}
			}
			else if (CheckQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
			{
				vec.push_back(HU_QINGYISE);
			}
			else if (CheckPengpenghu(handcard, eatCard, outCard))
			{
				vec.push_back(HU_DaDuiPeng);
			}
			else if (CheckMenQing(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
			{
				vec.push_back(HU_MenQing);
			}
			else
			{
				//没有大胡才检测小胡
				if (xiaohu_CheckHu(handcard, outCard, false, !isGetCard))
					vec.push_back(HU_XIAOHU);
			}
		}
		else if (gameInfo.m_qiangAnGang)
		{
			if (CheckShiSanYiao(handcard, outCard))
			{
				vec.push_back(HU_13Yao);
			}
		}
	}
	else if (gameInfo.m_GameType == ChenZhouMajiang)
	{
		Lint special;
		if (xiaohu_CheckHu(handcard, outCard, false, !isGetCard,gameInfo.m_pt_laizi))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (gameInfo.m_QiXiaoDui&&CheckXiaoqidui(handcard, special, outCard, isGetCard,gameInfo))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (isGetCard&&gameInfo.m_pt_laizi&&gameInfo.m_first_turn &&CheckQishouLaizi(handcard))
		{
			vec.push_back(HU_XIAOHU);
		}
	}
	else if(gameInfo.m_GameType == ChangDeMajiang) {
		Lint special;
		if (xiaohu_CheckHu(handcard, outCard, false, !isGetCard, gameInfo.m_pt_laizi))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (gameInfo.m_QiXiaoDui&&CheckXiaoqidui(handcard, special, outCard, isGetCard, gameInfo))
		{
			vec.push_back(HU_XIAOHU);
		}
		else if (isGetCard&&gameInfo.m_pt_laizi&&gameInfo.m_first_turn &&CheckQishouLaizi(handcard))
		{
			vec.push_back(HU_XIAOHU);
		}
	}
	else if (gameInfo.m_GameType == JingMenMajiang)
	{
		std::vector<HU_PAI_LEIXING> huTypes;
		if (Jingmen_CheckBaseHu(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, !isGetCard, gameInfo.m_wangBa, huTypes, gameInfo))
		{
			if (gameInfo.m_playerScore >= gameInfo.i_canHuScore)
			{
				vec.insert(vec.end(), huTypes.begin(), huTypes.end());
			}
		}
		if (vec.size())
		{
			if (CheckGangshangkaihua(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				if (isGetCard)
				{
					vec.push_back(HU_GANG1);
				}
			}
			if (CheckHaiDiLao(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				if (isGetCard)
				{
					vec.push_back(HU_END1);
				}
			}
		}
	}
	else if(gameInfo.m_GameType == YongZhouMajiang)
	{
		//十三烂胡牌
		bool qifengdaowei = false;
		HU_PAI_LEIXING huType;
		std::vector<HU_PAI_LEIXING> huTypes;
		Lint  special;
		//起手四个王霸
		if (gameInfo.m_first_turn &&CheckQishouWangBa(handcard, gameInfo))
		{
			vec.clear();
			vec.push_back(HU_QiShou4WangBa);
			if (gameInfo.m_tianhu)
			{
				vec.push_back(HU_TianHu);
			}
			return true;
		}
		if (CheckShiSanLan(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, qifengdaowei))
		{
			if (qifengdaowei)
			{
				vec.push_back(HU_QiFengDaoWei);
			}
			else {
				vec.push_back(HU_ShiSanLan);
			}
		}
		//十三幺胡牌
		if (CheckShiSanYiao(handcard, outCard))
		{
			vec.push_back(HU_13Yao);
		}

		//小七对胡牌
		if (CheckqiaoduiWithWangba(handcard, huType, outCard, isGetCard, gameInfo))
		{
			vec.push_back(huType);
		}
		//正常胡法
		if (Yongzhou_CheckBaseHu(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, !isGetCard, gameInfo.m_wangBa, huTypes))
		{
			vec.insert(vec.end(),huTypes.begin(),huTypes.end());
		}

		if (gameInfo.m_tianhu && vec.size())
		{
			vec.push_back(HU_TianHu);
		}
	}
	else if (gameInfo.m_GameType == XiangTanMajiang)
	{
		HU_PAI_LEIXING huType;
		std::vector<HU_PAI_LEIXING> huTypes;
		//起手四个王霸
		if (gameInfo.m_first_turn &&CheckQishouWangBa(handcard, gameInfo))
		{
			vec.push_back(HU_QiShou4WangBa);
		}
		//小七对胡牌
		if (CheckqiaoduiWithWangba(handcard, huType, outCard, isGetCard, gameInfo))
		{
			if (BaseQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo.m_wangBa, gameInfo.m_wangBa2))
			{
				vec.push_back(HU_QingYiSeQiXiaoDui);
			}
			else
			{
				vec.push_back(HU_XIAOQIDUI);
			}
		}
		//正常胡法
		if (XiangTan_CheckBaseHu(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, !isGetCard, gameInfo.m_wangBa, gameInfo.m_wangBa2, huTypes))
		{
			vec.insert(vec.end(), huTypes.begin(), huTypes.end());
		}
		if (vec.size() > 1)
		{
			Lint huleixing;
			Lint base_score = 0;
			for (std::vector<Lint>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				switch (*it)
				{
				case HU_XIAOHU:
					if (base_score < 1)
					{
						base_score = 1;
						huleixing = *it;
					}
					break;
				case HU_MenQing:
					if (base_score < 2)
					{
						base_score = 2;
						huleixing = *it;
					}
					break;
				case HU_QINGYISE:
				case HU_PENGPENGHU:
				case HU_XIAOQIDUI:
				case HU_QiShou4WangBa:
					if (base_score < 3)
					{
						base_score = 3;
						huleixing = *it;
					}
					break;
				case HU_QingYiSeMenQing:
				case HU_QingYiSeQiXiaoDui:
				case HU_MenQingPengpengHu:
				case HU_QingyisePengpengHu:
					if (base_score < 6)
					{
						base_score = 6;
						huleixing = *it;
					}
					break;
				case HU_MenQingQingyisePengpengHu:
					if (base_score < 12)
					{
						base_score = 12;
						huleixing = *it;
					}
					break;
				default:
					break;
				}
			}
			vec.clear();
			vec.push_back(huleixing);
		}
	}
	else if (gameInfo.m_GameType == XiaoYiMajiang)
	{
		bool xiaohu = xiaohu_CheckHu(handcard, outCard, false, false);
		if (xiaohu)
		{
			bool isCanhu = true;
			if (outCard && outCard->m_color <= 3)
			{
				if (outCard->m_number == 4)
				{
					if (!isGetCard)
					{
						isCanhu = false;
					}
				}
				else if(outCard->m_number < 4)
				{
					isCanhu = false;
				}
			}

			if (isCanhu)
			{
				vec.push_back(HU_XIAOHU);
				if (CheckQiangganghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
				{
					vec.push_back(HU_QIANGGANGHU);
				}
			}
		}
	}
	else if (gameInfo.m_GameType == YiYangMajiang)
	{
		Lint isSpecial = 0;
		if (pengCard.size() == 0 && agangCard.size() == 0 && mgangCard.size() == 0 && CheckXiaoqidui2(handcard, isSpecial, outCard, isGetCard, gameInfo))
		{
			vec.push_back(HU_XIAOQIDUI);
			if (isSpecial >= 2) 
			{
				vec.push_back(HU_SPECIAL_XIAOQIDUI);
			}
			if (isSpecial >= 3)
			{
				vec.push_back(HU_DOUBLESPECIAL_XIAOQIDUI);
			}
			if (isSpecial >= 4)
			{
				vec.push_back(HU_THREESPECIAL_XIAOQIDUI);
			}
		}
		bool xiaohu = xiaohu_CheckHu(handcard, outCard, false, false);
		bool menqing = !(eatCard.size() || pengCard.size() || mgangCard.size());
		if ((isGetCard || (menqing&& gameInfo.b_menqingjiangjianghu)) && CheckLuanjianghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
		{
			vec.push_back(HU_JIANGJIANGHU);
// 			if (menqing)
// 			{
// 				vec.push_back(HU_MenQing);
// 			}
		}
		if (xiaohu && CheckPengpenghu(handcard, eatCard, outCard))
		{
			vec.push_back(HU_PENGPENGHU);
		}
		if (CheckQuanqiuren2(handcard, eatCard, pengCard, agangCard, mgangCard, outCard))
		{
			vec.push_back(HU_QUANQIUREN);
		}
		if (xiaohu && BaseQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, NULL, NULL))
		{
			vec.push_back(HU_QINGYISE);
		}
		if (vec.size() || xiaohu)
		{
			if (CheckQiangganghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_QIANGGANGHU);
			}
			if (CheckGangshangkaihua(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_GANG1);
			}
			if (CheckHaiDiLao(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_END1);
			}
			if (gameInfo.m_tianhu)
			{
				vec.push_back(HU_TianHu);
			}
			if (gameInfo.m_dihu)
			{
				vec.push_back(HU_DiHu);
			}
		}
		if ((vec.size()>0 || isGetCard) && xiaohu && gameInfo.b_menqing && menqing)
		{
			vec.push_back(HU_MenQing);
		}
		else if (vec.size() <= 0 && isGetCard && xiaohu)
		{
			//没有大胡才检测小
			vec.push_back(HU_XIAOHU);
		}
	}
	else if (gameInfo.m_GameType == ChaoGuMajiang || gameInfo.m_GameType == ZhuoXiaZiMajiang)
	{
		bool xiaohu = xiaohu_CheckHu(handcard, outCard, false, false);
		if (xiaohu)
		{
			if (CheckQiangganghu(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_QIANGGANGHU);
			}
			if (CheckGangshangkaihua(handcard, eatCard, pengCard, agangCard, mgangCard, outCard, gameInfo))
			{
				vec.push_back(HU_GANG1);
			}
		}
		if (xiaohu && vec.size() == 0)
		{
			if (isGetCard)
			{
				bool menqing = !(eatCard.size() || pengCard.size() || mgangCard.size());
				if(menqing)
				{
					vec.push_back(HU_DADAO);
				}
				else
				{
					vec.push_back(HU_XIAODAO);
				}
			}
			else
			{
				vec.push_back(HU_XIAODAO);
			}
		}
	}
	else if (gameInfo.m_GameType == HengYangMajiang)
	{

	}
	return vec.size()!=0;
}


bool CardManager::CheckCanGang(CardVector& handcard,Card* outCard, OperateState& gameInfo)
{
	if (outCard)
	{
		if (gameInfo.m_pt_laizi&&outCard->m_color == 4 && outCard->m_number == 5)
		{
			return false;
		}
		if (gameInfo.m_wangBa && outCard->m_color == gameInfo.m_wangBa->m_color && outCard->m_number == gameInfo.m_wangBa->m_number) //永州麻将王霸不能吃碰杠
		{
			return false;
		}
		if (gameInfo.m_wangBa2 && outCard->m_color == gameInfo.m_wangBa2->m_color && outCard->m_number == gameInfo.m_wangBa2->m_number) 
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	CardVector tmpVec;
	for(Lsize i = 0 ;i < handcard.size(); ++i)
	{
		Card* pcard = handcard[i];
		if(pcard->m_color == outCard->m_color && 
			pcard->m_number == outCard->m_number)
		{
			tmpVec.push_back(pcard);
		}
	}
	
	if(tmpVec.size()>0)
	{
		if(gameInfo.m_pt_laizi&&tmpVec[0]->m_color==4&&tmpVec[0]->m_number==5)
		{
			return false;
		}
	}

	return tmpVec.size() >= 3;
}

bool CardManager::CheckCanPeng(CardVector& handcard,Card* outCard, OperateState& gameInfo)
{
	CardVector tmpVec;

	if(outCard)
	{
		if (gameInfo.m_pt_laizi&&outCard->m_color == 4 && outCard->m_number == 5)
		{
			return false;
		}
		if (gameInfo.m_wangBa && outCard->m_color == gameInfo.m_wangBa->m_color && outCard->m_number == gameInfo.m_wangBa->m_number) //永州麻将王霸不能吃碰杠
		{
			return false;
		}
		if (gameInfo.m_wangBa2 && outCard->m_color == gameInfo.m_wangBa2->m_color && outCard->m_number == gameInfo.m_wangBa2->m_number)
		{
			return false;
		}
	}
	else 
	{
		return false;
	}

	for(Lsize i = 0 ;i < handcard.size(); ++i)
	{
		Card* pcard = handcard[i];
		if(pcard->m_color == outCard->m_color && 
			pcard->m_number == outCard->m_number)
		{
			tmpVec.push_back(pcard);
		}
	}

	return (tmpVec.size() >= 2);
}

bool CardManager::CheckXiaoqidui2(CardVector& handcard, Lint& special, Card* outCard, bool isGetCard, OperateState& gameInfo)
{
	CardVector all = handcard;
	if (outCard)
	{
		all.push_back(outCard);
		gCardMgr.SortCard(all);
	}
	if (all.size() != 14)
	{
		return false;
	}
	CardVector one, tow, three, four;
	GetSpecialOneTwoThreeFour(all, one, tow, three, four);
	if (one.empty() && three.empty())
	{
		if (four.size() == 0)
		{
			special = 1;
		}
		if (four.size() == 4)
		{
			special = 2;
		}
		else if (four.size() == 8)
		{
			special = 3;
		}
		else if (four.size() == 12)
		{
			special = 4;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool CardManager::CheckXiaoqidui(CardVector& handcard,Lint& special,Card* outCard, bool isGetCard, OperateState& gameInfo)
{
	special = 0;
	CardVector all = handcard;
	Lint nLaiZiCount = 0;
	if (isGetCard)
	{
		if(outCard)
		{
			all.push_back(outCard);
		}
		for (Lint i = 0; i < all.size(); )
		{
			if (gameInfo.m_pt_laizi&&all[i]->m_color == 4&&all[i]->m_number == 5)
			{
				gCardMgr.EraseCard(all,all[i]);
				nLaiZiCount ++;
			}
			else
			{
				++i;
			}
		}
	}
	else
	{
		for (Lint i = 0; i < all.size(); )
		{
			if (gameInfo.m_pt_laizi&&all[i]->m_color == 4&&all[i]->m_number == 5)
			{
				gCardMgr.EraseCard(all,all[i]);
				nLaiZiCount ++;
			}
			else
			{
				++i;
			}
		}
		if(outCard)
		{
			all.push_back(outCard);
		}
	}
	gCardMgr.SortCard(all);
	if (nLaiZiCount > 0)
	{
		if (nLaiZiCount + all.size() != 14)
		{
			return false;
		}
		CardVector one,tow,three,four;
		GetSpecialOneTwoThreeFour(all,one,tow,three,four);
		if (one.size() + three.size()/3 <= nLaiZiCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (all.size() != 14)
		{
			return false;
		}
		CardVector one,tow,three,four;
		GetSpecialOneTwoThreeFour(all,one,tow,three,four);
		if(one.empty() && three.empty())
		{
			if(four.size()==4)
			{
				special = 1;
			}
			else if(four.size()>4)
			{
				special = 2;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CardManager::CheckqiaoduiWithWangba(CardVector& handcard, HU_PAI_LEIXING &leixin, Card* outCard, bool isGetCard, OperateState& gameInfo)
{
	CardVector all = handcard;
	Lint nWangbaCount = 0;
	if (outCard)
	{
		if (gameInfo.m_wangBa && !isGetCard && outCard->m_color == gameInfo.m_wangBa->m_color && outCard->m_number == gameInfo.m_wangBa->m_number)
		{
			return false;  //王霸不可点炮胡
		}
		if (gameInfo.m_wangBa2 && !isGetCard && outCard->m_color == gameInfo.m_wangBa2->m_color && outCard->m_number == gameInfo.m_wangBa2->m_number)
		{
			return false;  //王霸不可点炮胡
		}
		if (outCard)
		{
			all.push_back(outCard);
		}
	}

	for (Lint i = 0; i < all.size(); )
	{
		if (gameInfo.m_wangBa &&all[i]->m_color == gameInfo.m_wangBa->m_color && all[i]->m_number == gameInfo.m_wangBa->m_number)
		{
			gCardMgr.EraseCard(all, all[i]);
			nWangbaCount++;
		}
		else if (gameInfo.m_wangBa2 &&all[i]->m_color == gameInfo.m_wangBa2->m_color && all[i]->m_number == gameInfo.m_wangBa2->m_number)
		{
			gCardMgr.EraseCard(all, all[i]);
			nWangbaCount++;
		}
		else
		{
			++i;
		}
	}
	
	gCardMgr.SortCard(all);

	if (nWangbaCount + all.size() != 14)
	{
		return false;
	}
	CardVector one, two, three, four;
	GetSpecialOneTwoThreeFour(all, one, two, three, four);
	int restWangbaNum = nWangbaCount - one.size();
	int restWangbaNum2 = nWangbaCount - one.size() - three.size() / 3;
	if (nWangbaCount>0 && restWangbaNum>=0 && restWangbaNum%2 == 0 && three.size() / 3 == 0 && four.size() / 4 == 0)
	{
		leixin = HU_QiQiaoDui;
		return true;
	}
	else if (restWangbaNum2>=0 && restWangbaNum2%2 == 0 && four.size() / 4 >= 1)
	{
		leixin = HU_LongQiDui;
		return true;
	}
	else if (three.size() / 3 > 0 && four.size() / 4 == 0 && restWangbaNum2 >= 0 && restWangbaNum2 % 2 == 0)
	{
		leixin = HU_LongQiDui;
		return true;
	}
	else if (three.size() / 3 == 0 && four.size() / 4 == 0 && restWangbaNum2 >= 2 && restWangbaNum2 % 2 == 0)
	{
		leixin = HU_LongQiDui;
		return true;
	}
	else if (two.size() / 2 == 7 && nWangbaCount == 0)
	{
		leixin = HU_YingQiaoDui;
		return true;
	}
	return false;
}

//检测清一色
bool CardManager::CheckQingyise(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, std::vector<Lint>& vec)
{
	CardVector tmp = handcard;
	tmp.insert(tmp.end(),eatCard.begin(),eatCard.end());
	tmp.insert(tmp.end(),pengCard.begin(),pengCard.end());
	tmp.insert(tmp.end(),gangcard.begin(),gangcard.end());
	tmp.insert(tmp.end(),mgangcard.begin(),mgangcard.end());
	if(outCard)
	{
		tmp.push_back(outCard);
		SortCard(tmp);
	}

	if(tmp.empty())
	{
		return false;
	}

	Lint color = tmp.front()->m_color;
	for(Lsize i = 1 ; i < tmp.size(); ++i)
	{
		if(tmp[i]->m_color != color)
			return false;
	}

	if (xiaohu_CheckHu(handcard,outCard,false,false)|| vec.size() > 0)
	{
		return true;
	}
	return false;
}

bool CardManager::CheckQingyise2(CardVector& handcard, CardVector& eatCard, CardVector& pengCard, CardVector& gangcard, CardVector& mgangcard, Card* outCard, std::vector<Lint>& vec, Card* Wangba)
{
	if (!Wangba) return false;

	CardVector tmp = handcard;
	tmp.insert(tmp.end(), eatCard.begin(), eatCard.end());
	tmp.insert(tmp.end(), pengCard.begin(), pengCard.end());
	tmp.insert(tmp.end(), gangcard.begin(), gangcard.end());
	tmp.insert(tmp.end(), mgangcard.begin(), mgangcard.end());
	if (outCard)
	{
		tmp.push_back(outCard);
		SortCard(tmp);
	}

	if (tmp.empty())
	{
		return false;
	}

	Lint color = 0;
	for (Lsize i = 0; i < tmp.size(); ++i)
	{
		if (Wangba->m_color == tmp[i]->m_color && Wangba->m_number == tmp[i]->m_number)
		{
			continue;
		}
		if (color == 0)
		{
			color = tmp[i]->m_color;
		}
		if (tmp[i]->m_color != color)
			return false;
	}

	if (xiaohu_CheckHu2(handcard, outCard, false, Wangba))
	{
		return true;
	}

	return false;
}

//检测乱将胡
bool CardManager::CheckLuanjianghu(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard)
{
	CardVector tmp = handcard;
	if(outCard)
	{
		tmp.push_back(outCard);
	}
	tmp.insert(tmp.end(),eatCard.begin(),eatCard.end());
	tmp.insert(tmp.end(),pengCard.begin(),pengCard.end());
	tmp.insert(tmp.end(),gangcard.begin(),gangcard.end());
	tmp.insert(tmp.end(),mgangcard.begin(),mgangcard.end());
	if(tmp.empty())
	{
		return false;
	}

	for(Lsize i = 0 ; i < tmp.size(); ++i)
	{
		if(!(tmp[i]->m_number == 2 || tmp[i]->m_number == 5 ||
			tmp[i]->m_number == 8))
		{
			return false;
		}
	}
	return true;
}

//检测碰碰胡
bool CardManager::CheckPengpenghu(CardVector& handcard,CardVector& eatCard,Card* outCard, Card *WangBa1, Card *WangBa2)
{
	if(eatCard.size())
	{
		return false;
	}
	Lint WangBaNum = 0;
	CardVector check,one,tow,three,four;
	check = handcard;
	if(outCard)
	{
		check.push_back(outCard);
		SortCard(check);
	}
	if (WangBa1 != NULL || WangBa2 != NULL)
	{
		for (std::vector<Card *>::iterator it = check.begin(); it != check.end(); )
		{
			if (WangBa1 && gCardMgr.IsSame(*it, WangBa1))
			{
				it = check.erase(it);
				WangBaNum++;
			}
			else if (WangBa2 && gCardMgr.IsSame(*it, WangBa2))
			{
				it = check.erase(it);
				WangBaNum++;
			}
			else
			{
				++it;
			}
		}
	}

	GetSpecialOneTwoThreeFour(check,one,tow,three,four);

	if (WangBa1 == NULL && WangBa2 == NULL)
	{
		if (one.size() || four.size())
			return false;

		if (tow.size() != 2)
			return false;

		return true;
	}
	else
	{
		if (four.size())
		{
			Lint restWangBa = WangBaNum - 2 * one.size() - 2*four.size() / 4 + 1;
			if (restWangBa >= 0 && (restWangBa) % 3 == 0)
			{
				return true;
			}
			return false;
		}
		Lint restWangBa = WangBaNum - 2 * one.size() - tow.size() / 2 + 1;
		if (restWangBa >= 0 && (restWangBa)%3 == 0)
		{
			return true;
		}
		return false;
	}
}

//检测抢杠胡
bool CardManager::CheckQiangganghu(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, OperateState& gameInfo)
{
	if (gameInfo.m_GameType == ShuangKaiMajiang)
	{
		if (!gameInfo.m_thinkGang && (gameInfo.m_cardState == THINK_OPERATOR_MGANG && gameInfo.m_deskState != DESK_PLAY_END_CARD))
		{
			return true;
		}
	}
	if (gameInfo.m_GameType != ShuangKaiMajiang && gameInfo.m_GameType == ChangShaMaJiang)
	{
		if (!gameInfo.m_thinkGang && (gameInfo.m_cardState == THINK_OPERATOR_MGANG && gameInfo.m_deskState != DESK_PLAY_END_CARD))
		{
			return true;
		}
	}
	if (gameInfo.m_GameType == YiYangMajiang || gameInfo.m_GameType == ChaoGuMajiang || gameInfo.m_GameType == ZhuoXiaZiMajiang || gameInfo.m_GameType == XiaoYiMajiang || gameInfo.m_GameType == ShuangKaiMajiang)
	{
		if (!gameInfo.m_thinkGang && gameInfo.m_MePos != gameInfo.m_playerPos && (gameInfo.m_cardState == THINK_OPERATOR_MGANG || gameInfo.m_cardState == THINK_OPERATOR_MBU) && gameInfo.m_deskState != DESK_PLAY_END_CARD)
		{
			return true;
		}
	}
	return false;
}

//检测全球人2
bool CardManager::CheckQuanqiuren2(CardVector& handcard, CardVector& eatCard, CardVector& pengCard, CardVector& gangcard, CardVector& mgangcard, Card* outCard)
{
	CardVector check;
	check = handcard;
	if (outCard)
	{
		check.push_back(outCard);
	}
	if (check.size() != 2)
	{
		return false;
	}
	return (check[0]->m_color == check[1]->m_color && check[0]->m_number == check[1]->m_number);
}

bool  CardManager::CheckQuanqiuren3(CardVector& handcard, CardVector& eatCard, CardVector& pengCard, CardVector& gangcard, CardVector& mgangcard, Card* outCard, Card* Wangba)
{
	if (!Wangba)
	{
		return false;
	}

	CardVector check;
	check = handcard;
	if (outCard)
	{
		check.push_back(outCard);
	}

	if (check.size() != 2)
	{
		return false;
	}

	if (IsSame(check[0], Wangba) || IsSame(check[1], Wangba))
	{
		return true;
	}
	
	return IsSame(check[0], check[1]);
}

//检测全球人
bool CardManager::CheckQuanqiuren(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard)
{
	CardVector check;
	check = handcard;
	if(outCard)
	{
		check.push_back(outCard);
	}
	if (check.size() != 2)
	{
		return false;
	}
	return ((CheckLuanjianghu(handcard,eatCard,pengCard,gangcard,mgangcard,outCard) || 
		(check[0]->m_color == check[1]->m_color && check[0]->m_number == check[1]->m_number)));
}

//杠上开花
bool CardManager::CheckGangshangkaihua(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, OperateState& gameInfo)
{
	//判断是不是自已杠的
	if (gameInfo.m_MePos == gameInfo.m_playerPos && gameInfo.m_thinkGang)
	{
		return true;
	}

	return false;
}

//杠上炮
bool CardManager::CheckGangshangpao(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, OperateState& gameInfo)
{

	//杠上炮，判断是不是别人的杠
	if (gameInfo.m_MePos != gameInfo.m_playerPos && gameInfo.m_thinkGang)
	{
		return true;
	}

	return false;
}
//海底捞
bool CardManager::CheckHaiDiLao(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, OperateState& gameInfo)
{
	if (gameInfo.m_MePos == gameInfo.m_playerPos && gameInfo.m_deskState == DESK_PLAY_END_CARD)
	{
		return true;
	}
	return false;
}
//海底炮
bool CardManager::CheckHaiDiPao(CardVector& handcard,CardVector& eatCard,CardVector& pengCard,CardVector& gangcard,CardVector& mgangcard,Card* outCard, OperateState& gameInfo)
{
	if (gameInfo.m_MePos != gameInfo.m_playerPos && gameInfo.m_deskState == DESK_PLAY_END_CARD)
	{
		return true;
	}
	return false;
}

bool CardManager::CheckXiangTongSe(CardVector & handcard, Card * outCard)
{
	CardVector tmp = handcard;
	if (outCard)
	{
		tmp.push_back(outCard);
		SortCard(tmp);
	}

	Lint color = tmp.front()->m_color;
	for (Lsize i = 1; i < tmp.size(); ++i)
	{
		if (tmp[i]->m_color != color)
			return false;
	}
	return true;
}

bool CardManager::BaseQingyise(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard, Card* Wangba1, Card* Wangba2)
{
	CardVector tmp = handcard;
	tmp.insert(tmp.end(), eatCard.begin(), eatCard.end());
	tmp.insert(tmp.end(), pengCard.begin(), pengCard.end());
	tmp.insert(tmp.end(), gangcard.begin(), gangcard.end());
	tmp.insert(tmp.end(), mgangcard.begin(), mgangcard.end());
	if (outCard)
	{
		tmp.push_back(outCard);
		SortCard(tmp);
	}

	if (tmp.empty())
	{
		return false;
	}

	Lint color = 0;
	for (Lsize i = 0; i < tmp.size(); ++i)
	{
		if ((Wangba1 && Wangba1->m_color != 0 && Wangba1->m_color == tmp[i]->m_color && Wangba1->m_number == tmp[i]->m_number) ||
			(Wangba2 && Wangba2->m_color != 0 && Wangba2->m_color == tmp[i]->m_color && Wangba2->m_number == tmp[i]->m_number))
		{
			continue;
		}
		else if (0 == color)
		{
			color = tmp[i]->m_color;
		}
		else if (tmp[i]->m_color != color)
		{
			return false;
		}
	}
	return true;
}

bool CardManager::CheckQingyise(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard)
{
	if (BaseQingyise(handcard,eatCard,pengCard,gangcard,mgangcard,outCard, NULL, NULL) && xiaohu_CheckHu(handcard, outCard, false,true))
	{
		return true;
	}
	return false;
	
}

bool CardManager::CheckQingyiseDapengdui(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard)
{
	CardVector tmp = handcard;

	//peng  return false;
	if (!eatCard.empty())
	{
		return false;
	}

	if (tmp.size() > 1)
	{
		return false;
	}

	if (tmp[0]->m_color == outCard->m_color && tmp[0]->m_number == outCard->m_number)
	{
		return true;
	}
	else
		return false;
}

bool CardManager::CheckMenQing(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard)
{
	//if(eatCard.size() || pengCard.size() ||gangcard.size() || mgangcard.size())
	if (eatCard.size() || pengCard.size() || mgangcard.size())
	{
		return false;
	}
	if (xiaohu_CheckHu(handcard, outCard, false,true))
	{
		return true;
	}
	return false;
}

bool CardManager::CheckFengYiSe(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard)
{
	if (CheckPengpenghu(handcard, eatCard, outCard))
	{
		CardVector tmp = handcard;
		tmp.insert(tmp.end(), pengCard.begin(), pengCard.end());
		tmp.insert(tmp.end(), gangcard.begin(), gangcard.end());
		tmp.insert(tmp.end(), mgangcard.begin(), mgangcard.end());
		for (Lsize i = 0; i < tmp.size(); ++i)
		{
			if (tmp[i]->m_color != 4)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool CardManager::CheckFengYiSe2(CardVector& handcard, CardVector& eatCard, CardVector& pengCard, CardVector& gangcard, CardVector& mgangcard)
{
	CardVector tmp = handcard;
	tmp.insert(tmp.end(), pengCard.begin(), pengCard.end());
	tmp.insert(tmp.end(), gangcard.begin(), gangcard.end());
	tmp.insert(tmp.end(), mgangcard.begin(), mgangcard.end());
	for (Lsize i = 0; i < tmp.size(); ++i)
	{
		if (tmp[i]->m_color != 4)
		{
			return false;
		}
	}
	return true;
}

bool CardManager::CheckShiSanYiao(CardVector & handcard, Card * outCard)
{
	CardVector tmp = handcard;
	if (outCard)
	{
		tmp.push_back(outCard);
	}
	if (tmp.size() > 13)
	{
		Card* mo = tmp.back();
		tmp.pop_back();
		if (!outCard)
		{
			outCard = mo;
		}
	}
	SortCard(tmp);
	EarseSameCard(tmp);
	if (tmp.size() < 13)
	{
		return false;		//手牌不够十三张，肯定不是十三
	}
	for (Lint i = 0; i < 13; i++)
	{
		if (tmp[i]->m_color != m_13yao[i]->m_color || tmp[i]->m_number != m_13yao[i]->m_number)
		{
			return false;
		}
	}
	if (outCard && outCard->m_color < 4 && (outCard->m_number == 1 || outCard->m_number == 9))
	{
		return true;
	}
	else if (outCard && outCard->m_color == 4 && outCard->m_number > 0 && outCard->m_number <= 7)
	{
		return true;
	}
	return false;
}

bool CardManager::CheckShiSanLan(CardVector & handcard, CardVector & eatCard, CardVector & pengCard, CardVector & gangcard, CardVector & mgangcard, Card * outCard, bool & isqifeng)
{
	// 有吃碰杠肯定不是烂牌
	CardVector* cards[4] = { &eatCard, &pengCard, &gangcard, &mgangcard };
	for (int i = 0; i < 4; ++i)
	{
		if (cards[i]->size() > 0)
		{
			return false;
		}
	}
	int cardnum[4][9] = { 0 };
	for (CardVector::iterator it = handcard.begin(); it != handcard.end(); ++it)
	{
		if (!(*it))
		{
			return false;
		}
		if ((*it)->m_color <= 0 || (*it)->m_color > 4)
		{
			return false;
		}
		if ((*it)->m_number <= 0 || (*it)->m_number > 9)
		{
			return false;
		}
		cardnum[(*it)->m_color - 1][(*it)->m_number - 1]++;
		// 只能有一张
		if (cardnum[(*it)->m_color - 1][(*it)->m_number - 1] > 1)
		{
			return false;
		}
	}
	if (outCard)
	{
		if (outCard->m_number <= 0 || outCard->m_number > 9)
		{
			return false;
		}
		cardnum[outCard->m_color - 1][outCard->m_number - 1]++;
		// 只能有一张
		if (cardnum[outCard->m_color - 1][outCard->m_number - 1] > 1)
		{
			return false;
		}
	}
	// 检查数字牌的间隔是否都隔开两种
	for (int i = 0; i < 3; ++i)
	{
		Lint last_number = -3;
		for (int k = 0; k < 9; ++k)
		{
			if (cardnum[i][k] > 0)
			{
				if (k - last_number <= 2)
				{
					return false;
				}
				last_number = k;
			}
		}
	}
	// 检查是否东南西北是否全部存在
	isqifeng = true;
	for (int k = 0; k < 7; ++k)
	{
		if (cardnum[3][k] == 0)
		{
			isqifeng = false;
			break;
		}
	}
	return true;
}

bool CardManager::CheckCanChi(CardVector& handcard, Card* outCard, std::vector<Card*>& vec, OperateState& gameInfo)
{
	LLOG_DEBUG("CheckCanChi");
	bool ret = false;
	if(handcard.size() < 2)
		return false;
	if(!outCard)
	{
		return false;
	}
	if (gameInfo.m_wangBa || gameInfo.m_wangBa2)
	{
		if (gameInfo.m_wangBa && ((outCard->m_color == 4 && outCard->m_number != 5) || (outCard->m_color == 4 && outCard->m_number == 5 && gameInfo.m_wangBa->m_color == 4)))
		{
			return false;
		}
		else if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color == outCard->m_color && gameInfo.m_wangBa->m_number == outCard->m_number) //王霸不能吃
		{
			return false;
		}
		else if (gameInfo.m_wangBa2 && gameInfo.m_wangBa2->m_color == outCard->m_color && gameInfo.m_wangBa2->m_number == outCard->m_number) //王霸不能吃
		{
			return false;
		}
		else if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color == 4)
		{
			if (outCard->m_color == 4)
			{
				return false;
			}

			CardVector one = handcard;
			SortCard(one);
			EarseSameCard(one);
			EraseCard(one, outCard, 4);

			for (Lint i = 0; i + 1 < one.size(); ++i)
			{
				CardVector tmp;
				tmp.push_back(one[i]);
				tmp.push_back(one[i + 1]);
				tmp.push_back(outCard);
				SortCard(tmp);
				if (IsContinue(tmp))
				{
					vec.push_back(one[i]);
					vec.push_back(one[i + 1]);
					vec.push_back(outCard);
					ret = true;
				}
			}
			return ret;
		}
		else
		{  //红中代替王霸
			LLOG_DEBUG("chi replace hongzhong");
			if (outCard->m_color == 4 && outCard->m_number != 5)
			{
				return false;
			}
			Card* judge_chi = outCard;
			CardVector one = handcard;

			//这几个玩法，王牌和
			if (gameInfo.m_GameType == JingMenMajiang || gameInfo.m_GameType == ShuangKaiMajiang)
			{
				EraseCard(one, GetCard(4, 5), 4);
			}

			if (outCard->m_color == 4)//出红中
			{
				if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color != 0)
				{
					judge_chi = gameInfo.m_wangBa;
				}
				else if (gameInfo.m_wangBa2 && gameInfo.m_wangBa2->m_color != 0)
				{
					judge_chi = gameInfo.m_wangBa2;
				}
			}
			if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color != 0)
			{
				EraseCard(one, gameInfo.m_wangBa, 4);
			}
			if (gameInfo.m_wangBa2 && gameInfo.m_wangBa2->m_color != 0)
			{
				EraseCard(one, gameInfo.m_wangBa2, 4);
			}
			EraseCard(one, outCard, 4);
			
			if (HaveTheCard(one, GetCard(4,5)) && gameInfo.m_wangBa && gameInfo.m_wangBa->m_color != 0)//如果有红中
			{
				one.push_back(gameInfo.m_wangBa);
			}

			SortCard(one);
			EarseSameCard(one);
			for (Lsize i = 0; i < one.size(); i++)
			{
				LLOG_DEBUG("canEat one: %d,%d", one[i]->m_color, one[i]->m_number);
			}

			for (Lint i = 0; i + 1 < one.size(); ++i)
			{
				CardVector tmp;
				tmp.push_back(one[i]);
				tmp.push_back(one[i + 1]);
				tmp.push_back(judge_chi);
				SortCard(tmp);
				if (IsContinue(tmp))
				{
					if (gameInfo.m_wangBa && one[i]->m_color == gameInfo.m_wangBa->m_color && one[i]->m_number == gameInfo.m_wangBa->m_number)
					{
						vec.push_back(GetCard(4, 5));
					}
					else
					{
						vec.push_back(one[i]);
					}
					if (gameInfo.m_wangBa && one[i+1]->m_color == gameInfo.m_wangBa->m_color && one[i+1]->m_number == gameInfo.m_wangBa->m_number)
					{
						vec.push_back(GetCard(4, 5));
					}
					else
					{
						vec.push_back(one[i + 1]);
					}
					vec.push_back(outCard);
					ret = true;
				}
			}
			return ret;
		}
	}
	else
	{
		if (outCard->m_color == 4)
		{
			return false;
		}

		CardVector one = handcard;
		SortCard(one);
		EarseSameCard(one);
		EraseCard(one, outCard, 4);

		for (Lint i = 0; i + 1 < one.size(); ++i)
		{
			CardVector tmp;
			tmp.push_back(one[i]);
			tmp.push_back(one[i + 1]);
			tmp.push_back(outCard);
			SortCard(tmp);
			if (IsContinue(tmp))
			{
				vec.push_back(one[i]);
				vec.push_back(one[i + 1]);
				vec.push_back(outCard);
				ret = true;
			}
		}
		return ret;
	}
	
}
bool CardManager::CheckAnGang(CardVector& handcard,  std::vector<Card*>& vec, OperateState& gameInfo)
{
	if(handcard.size() < 4)
		return false;

	for(Lsize i = 0 ;i+3 < handcard.size(); ++i)
	{
		Card* pcarda = handcard[i];
		Card* pcardb = handcard[i+3];
		if(gameInfo.m_pt_laizi)
		{
			if(pcarda->m_color == pcardb->m_color && 
				pcarda->m_number == pcardb->m_number&& !(pcarda->m_color == 4&&pcarda->m_number ==5))
			{
				vec.push_back(pcarda);
			}
		}
		else if ((gameInfo.m_GameType==YongZhouMajiang || gameInfo.m_GameType == JingMenMajiang || gameInfo.m_GameType == ShuangKaiMajiang)&&gameInfo.m_wangBa)
		{
			if (pcarda->m_color == pcardb->m_color &&
				pcarda->m_number == pcardb->m_number && !(pcarda->m_color == gameInfo.m_wangBa->m_color && pcarda->m_number == gameInfo.m_wangBa->m_number))
			{
				vec.push_back(pcarda);
			}
		}
		else{
			if(pcarda->m_color == pcardb->m_color && pcarda->m_number == pcardb->m_number)
			{
				vec.push_back(pcarda);
			}
		}
	}
	return true;
}

bool CardManager::CheckMGang(CardVector& handcard,CardVector& pengCard,  std::vector<Card*>& vec, OperateState& gameInfo)
{
	for(Lsize i = 0 ; i < pengCard.size(); i+=3)
	{
		Card* pcard = pengCard[i];
		for(Lsize j = 0 ; j < handcard.size(); ++j)
		{
			if(gameInfo.m_pt_laizi)
			{
				if(pcard->m_number == handcard[j]->m_number &&
					pcard->m_color == handcard[j]->m_color&& !(pcard->m_color == 4&&pcard->m_number ==5))
				{
					vec.push_back(pcard);
					break;
				}
			}
			else if ((gameInfo.m_GameType == YongZhouMajiang || gameInfo.m_GameType == JingMenMajiang || gameInfo.m_GameType == ShuangKaiMajiang)&&gameInfo.m_wangBa)
			{
				if (pcard->m_color == handcard[j]->m_color &&
					pcard->m_number == handcard[j]->m_number && !(pcard->m_color == gameInfo.m_wangBa->m_color && pcard->m_number == gameInfo.m_wangBa->m_number))
				{
					vec.push_back(pcard);
					break;
				}
			}
			else{
				if(pcard->m_number == handcard[j]->m_number &&
					pcard->m_color == handcard[j]->m_color)
				{
					vec.push_back(pcard);
					break;
				}
			}
		}
	}
	return true;
}


bool CardManager::IsContinue(CardVector& result)
{
	if(result.empty())
	{
		return false;
	}

	Lint number = result.front()->m_number;
	Lint color = result.front()->m_color;
	for(Lsize i = 1; i < result.size(); ++i)
	{
		if(result[i]->m_number != number+i || color != result[i]->m_color)
			return false;
	}

	return true;
}

bool CardManager::IsSame(CardVector& result)
{
	if(result.empty())
	{
		return false;
	}

	Lint number = result.front()->m_number;
	Lint color = result.front()->m_color;
	for(Lsize i = 1; i < result.size(); ++i)
	{
		if(result[i]->m_number != number || color != result[i]->m_color)
			return false;
	}

	return true;
}
bool CardManager::IsSame(Card* c1,Card* c2)
{
	return c1->m_color==c2->m_color&&c1->m_number==c2->m_number;
}

bool CardManager::IsNineOne(Card* c)
{
	return c->m_number == 0 || c->m_number == 9;
}

bool CardManager::HaveTheCard(CardVector & result,Card* thecard)
{
	if (!thecard)
	{
		return false;
	}

	for (auto x = result.begin(); x != result.end(); ++x)
	{
		if ((*x)->m_color == thecard->m_color && (*x)->m_number == thecard->m_number)
		{
			return true;
		}
	}
	return false;
}

Card * CardManager::GetCard(Lint m_color, Lint m_number)
{
	for (auto x = m_3_feng.begin(); x != m_3_feng.end(); ++x)
	{
		if ((*x)->m_color == m_color && (*x)->m_number == m_number)
		{
			return *x;
		}
	}
	return nullptr;
}

bool CardManager::GetSpecialOneTwoThreeFour(CardVector& src,CardVector& one,CardVector& two,CardVector& three,CardVector& four)
{
	if(src.empty())
	{
		return false;
	}

	Lsize i = 0 ;
	Lint number = 0,color = 0 ,length = 0 ;
	for(; i < src.size(); ++i)
	{
		Card* pCard = src[i];
		if(number == pCard->m_number && color == pCard->m_color)
		{
			length += 1;
		}
		else
		{
			if(length == 1)
			{
				one.push_back(src[i-1]);
			}
			else if(length == 2)
			{
				two.push_back(src[i-2]);
				two.push_back(src[i-1]);
			}
			else if(length == 3)
			{
				three.push_back(src[i-3]);
				three.push_back(src[i-2]);
				three.push_back(src[i-1]);
			}
			else if(length == 4)
			{
				four.push_back(src[i-4]);
				four.push_back(src[i-3]);
				four.push_back(src[i-2]);
				four.push_back(src[i-1]);
			}
			length = 1;
			number = pCard->m_number;
			color = pCard->m_color;
		}
	}

	if(length == 1)
	{
		one.push_back(src[i-1]);
	}
	else if(length == 2)
	{
		two.push_back(src[i-2]);
		two.push_back(src[i-1]);
	}
	else if(length == 3)
	{
		three.push_back(src[i-3]);
		three.push_back(src[i-2]);
		three.push_back(src[i-1]);
	}
	else if(length == 4)
	{
		four.push_back(src[i-4]);
		four.push_back(src[i-3]);
		four.push_back(src[i-2]);
		four.push_back(src[i-1]);
	}

	return true;
}



bool CardManager::CheckQishouLaizi(CardVector& handcard)
{
	int lanzi_num=0;
	for(int x=0;x<handcard.size();x++)
	{
		if(handcard[x]!=NULL)
		{
			if(handcard[x]->m_color==4&&handcard[x]->m_number==5)
				lanzi_num++;
		}
	}
	if(lanzi_num==4)
	{
		return true;
	}
	else{
		return false;
	}
}

bool CardManager::CheckQishouWangBa(CardVector & handcard, OperateState & gameInfo)
{
	int Wangba_num = 0;

		for (int x = 0; x<handcard.size(); x++)
		{
			if (handcard[x] != NULL)
			{
				if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color != 0 && handcard[x]->m_color == gameInfo.m_wangBa->m_color && handcard[x]->m_number == gameInfo.m_wangBa->m_number)
				{
					Wangba_num++;
				}
				if (gameInfo.m_wangBa2 && gameInfo.m_wangBa2->m_color != 0 && handcard[x]->m_color == gameInfo.m_wangBa2->m_color && handcard[x]->m_number == gameInfo.m_wangBa2->m_number)
				{
					Wangba_num++;
				}
			}
		}

	if (Wangba_num == 4)
	{
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool CardManager::xiaohu_CheckHu(CardVector handcard,Card* curCard,bool needJiang,bool is_out_card,bool hava_laizi)
{
	if(curCard)
	{
		handcard.push_back(curCard);
	}

	std::vector<Lint> card_vector(40,0);
	Lint Lai_num=0;

	//第一步获取癞子,分数量统计
	for(int x=0;x<handcard.size();x++)
	{
		if(hava_laizi&&handcard[x]->m_color==4&&handcard[x]->m_number==5)
		{
			Lai_num++;
		}
		else
			card_vector[(handcard[x]->m_color-1)*10+(handcard[x]->m_number)*1]++;
	}

	if(curCard)
	{
		//玩家出了红中癞子
		if(hava_laizi&&is_out_card&&curCard->m_color==4&&curCard->m_number==5)
		{
			if(Lai_num>1)
			{
				std::vector<Lint> tmp_cards(card_vector);
				if (explorer_zheng_function(tmp_cards, 0, Lai_num - 2))
				{
					return true;
				}
				else {//癞子组成刻子
					if (Lai_num >= 3)
					{
						Lai_num -= 3;
					}
					else {
						return false;
					}
				}
				
			}
			else{
				return false;
			}
		}
	}
	int juge_lai_jiang = true;
	for(int x=0;x<card_vector.size();x++)
	{
		int index = x%10;
		if(x%10==0)
		{
			continue;
		}
		else if(x>37)
		{
			break;
		}

		if(needJiang&&(index!=2&&index!=5&&index!=8))
		{
			if(x==30)
				return false;
			continue;
		}
		
		if(card_vector[x]>1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x]-=2;
			if(explorer_zheng_function(tmp_cards,0,Lai_num))
			{
				return true;
			}
		}
		else if(card_vector[x]==1&&Lai_num>0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x]-=1;
			if(explorer_zheng_function(tmp_cards,0,Lai_num-1))
			{
				return true;
			}
		}
		else{
			if(Lai_num>1&&juge_lai_jiang)
			{
				juge_lai_jiang = false;
				if(explorer_zheng_function(card_vector,0,Lai_num-2))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CardManager::xiaohu_CheckHu3(CardVector handcard, Card* curCard, bool needJiang, Card *Wangba)
{
	if (curCard)
	{
		handcard.push_back(curCard);
	}

	std::vector<Lint> card_vector(40, 0);
	Lint Lai_num = 0;

	//第一步获取癞子,分数量统计
	for (int x = 0; x<handcard.size(); x++)
	{
		if (Wangba && handcard[x]->m_color == Wangba->m_color && handcard[x]->m_number == Wangba->m_number)
		{
			Lai_num++;
		}
		else
			card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
	}

	int juge_lai_jiang = true;
	for (int x = 0; x<card_vector.size(); x++)
	{
		//不检查中发白为将牌
		if (needJiang && x == 30)
			return false;

		int index = x % 10;
		if (x % 10 == 0)
		{
			continue;
		}
		else if (x>37)
		{
			break;
		}

		if (needJiang && (index != 2 && index != 5 && index != 8))
		{
			if (x == 30)
				return false;
			continue;
		}

		if (card_vector[x]>1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 2;
			if (explorer_zheng_function(tmp_cards, 0, Lai_num))
			{
				return true;
			}
		}
		else if (card_vector[x] == 1 && Lai_num>0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function(tmp_cards, 0, Lai_num - 1))
			{
				return true;
			}
		}
		else {
			if (Lai_num>1 && juge_lai_jiang)
			{
				juge_lai_jiang = false;
				if (explorer_zheng_function(card_vector, 0, Lai_num - 2))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CardManager::xiaohu_CheckHu2(CardVector handcard, Card* curCard, bool needJiang, Card *Wangba)
{
	if (curCard)
	{
		handcard.push_back(curCard);
	}

	std::vector<Lint> card_vector(40, 0);
	Lint Lai_num = 0;

	//第一步获取癞子,分数量统计
	for (int x = 0; x<handcard.size(); x++)
	{
		if (Wangba && handcard[x]->m_color == Wangba->m_color && handcard[x]->m_number == Wangba->m_number)
		{
			Lai_num++;
		}
		else
			card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
	}

	int juge_lai_jiang = true;
	for (int x = 0; x<card_vector.size(); x++)
	{
		int index = x % 10;
		if (x % 10 == 0)
		{
			continue;
		}
		else if (x>37)
		{
			break;
		}

		if (needJiang && (index != 2 && index != 5 && index != 8))
		{
			if (x == 30)
				return false;
			continue;
		}

		if (card_vector[x]>1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 2;
			if (explorer_zheng_function(tmp_cards, 0, Lai_num))
			{
				return true;
			}
		}
		else if (card_vector[x] == 1 && Lai_num>0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function(tmp_cards, 0, Lai_num - 1))
			{
				return true;
			}
		}
		else {
			if (Lai_num>1 && juge_lai_jiang)
			{
				juge_lai_jiang = false;
				if (explorer_zheng_function(card_vector, 0, Lai_num - 2))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CardManager::XiangTan_CheckBaseHu(CardVector handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* curCard, bool is_out_card, Card * wangbaCard, Card * wangbaCard2, std::vector<HU_PAI_LEIXING> &playtypes)
{
	if (curCard && wangbaCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number && is_out_card)
	{
		LLOG_DEBUG("XiangTan_CheckBaseHu wangba cannot dianpaohu");
		return false;
	}
	if (curCard && wangbaCard2 && curCard->m_color == wangbaCard2->m_color && curCard->m_number == wangbaCard2->m_number && is_out_card)
	{
		LLOG_DEBUG("XiangTan_CheckBaseHu wangba cannot dianpaohu");
		return false;
	}
	bool pengpeng = CheckPengpenghu(handcard, eatCard, curCard, wangbaCard, wangbaCard2);
	if (curCard)
	{
		LLOG_DEBUG("XiangTan_CheckBaseHu handcardsize:%d outcard:%d,%d  isout:%d", handcard.size(), curCard->m_color, curCard->m_number, is_out_card);
		handcard.push_back(curCard);
	}
	bool yise = BaseQingyise(handcard, eatCard, pengCard, agangCard, mgangCard, curCard, wangbaCard, wangbaCard2);

	std::vector<Lint> card_vector(40, 0);
	Lint wangbaNum = 0;
	bool ret = false, xiaohu = false, qingyise = false;
	//第一步获取癞子,分数量统计
	for (int x = 0; x < handcard.size(); x++)
	{
		if (wangbaCard && handcard[x]->m_color == wangbaCard->m_color && handcard[x]->m_number == wangbaCard->m_number)
		{
			wangbaNum++;
		}
		else if (wangbaCard2 && handcard[x]->m_color == wangbaCard2->m_color && handcard[x]->m_number == wangbaCard2->m_number)
		{
			wangbaNum++;
		}
		else
		{
			card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
		}
	}

	
	bool juge_lai_jiang = true;
	for (int x = 0; x < card_vector.size(); x++)
	{
		int index = x % 10;
		if (x % 10 == 0)
		{
			continue;
		}
		else if (x > 37)
		{
			break;
		}

		if (x == 30)
		{
			break;
		}

		if (card_vector[x] > 1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 2;
			if (explorer_zheng_function(tmp_cards, 0, wangbaNum))
			{
				if (index == 2 || index == 5 || index == 8) xiaohu = true;
				qingyise = yise;
			}
		}
		else if (card_vector[x] == 1 && wangbaNum > 0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 1))
			{
				if (index == 2 || index == 5 || index == 8) xiaohu = true;
				qingyise = yise;
			}
		}
		else {
			if (wangbaNum > 1 && juge_lai_jiang)
			{
				juge_lai_jiang = false;
				if (explorer_zheng_function(card_vector, 0, wangbaNum - 2))
				{
					xiaohu = true;
					qingyise = yise;
				}
			}
		}
	}
	bool menqing = (pengCard.size() == 0 && mgangCard.size() == 0 && eatCard.size() == 0);
	
	if (xiaohu && !qingyise)
	{
		playtypes.push_back(HU_XIAOHU);
		ret = true;
	}
	if (menqing)
	{
		if (qingyise && pengpeng)
		{
			playtypes.push_back(HU_MenQingQingyisePengpengHu);
			ret = true;
		}
		else if (qingyise)
		{
			playtypes.push_back(HU_QingYiSeMenQing);
			ret = true;
		}
		else if (xiaohu)
		{
			playtypes.push_back(HU_MenQing);
			ret = true;
		}
	}
	if (qingyise && pengpeng)
	{
		playtypes.push_back(HU_QingyisePengpengHu);
		ret = true;
	}
	else if (qingyise)
	{
		playtypes.push_back(HU_QINGYISE);
		ret = true;
	}
	if (pengpeng && menqing)
	{
		playtypes.push_back(HU_MenQingPengpengHu);
		ret = true;
	}
	else if (pengpeng)
	{
		playtypes.push_back(HU_PENGPENGHU);
		ret = true;
	}
	return ret;
}

bool CardManager::checkJingmenOtherHu(std::vector<Lint> &card_vector, Card* curCard, bool is_out_card, Card * wangbaCard,  Lint wangbaNum, std::vector<HU_PAI_LEIXING> &playtypes, OperateState& gameInfo)
{
	bool ret = false;
	int card_num = card_vector.size();
	for (int x = 0; x < card_num; ++x)
	{
		int index = x % 10;
		if (index == 0)
		{
			continue;
		}
		else if (x > 37)
		{
			break;
		}

		if (card_vector[x] > 1)  //将牌无王霸
		{
			if (wangbaNum == 0)
			{
				std::vector<Lint> tmp_cards(card_vector);
				tmp_cards[x] -= 2;
				if (explorer_zheng_function(tmp_cards, 0, wangbaNum))
				{
					playtypes.push_back(HU_YINGHU);
					++gameInfo.m_playerScore;
					ret = true;
				}
			}
			else //只有一张
			{
				Lint WangbaShunziNum = 0;
				//检查是否是甩牌胡
				if ((!is_out_card) && curCard && (x != ((curCard->m_color - 1) * 10 + (curCard->m_number) * 1)))
				{
					std::vector<Lint> tmp_cards2(card_vector);
					tmp_cards2[x] -= 2;
					if (explorer_zheng_function(tmp_cards2, 0, wangbaNum, &WangbaShunziNum, wangbaCard))
					{
						playtypes.push_back(HU_SHUAIPAI);
						ret = true;
					}
				}

				std::vector<Lint> tmp_cards(card_vector);
				tmp_cards[(wangbaCard->m_color - 1) * 10 + (wangbaCard->m_number) * 1]++;
				tmp_cards[x] -= 2;
				if (explorer_zheng_function(tmp_cards, 0, 0))
				{
					playtypes.push_back(HU_YINGHU);
					++gameInfo.m_playerScore;
					ret = true;
				}
				else
				{
					std::vector<Lint> tmp_cards2(card_vector);
					tmp_cards2[x] -= 2;
					if (explorer_zheng_function(tmp_cards2, 0, wangbaNum, &WangbaShunziNum, wangbaCard))
					{
						playtypes.push_back(HU_RUANHU);
						ret = true;
					}
				}
			}
		}

		if (card_vector[x] == 1 && wangbaNum > 0)  
		{
			//将牌一张王霸，一张普通牌
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 1))
			{
				playtypes.push_back(HU_RUANHU);
				ret = true;
			}
		}
	}

	return ret;
}

bool CardManager::Jingmen_CheckBaseHu(CardVector handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* curCard, bool is_out_card, Card * wangbaCard, std::vector<HU_PAI_LEIXING> &playtypes, OperateState& gameInfo)
{
	if (NULL == wangbaCard)
	{
		return false;
	}
	if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number && is_out_card)
	{
		LLOG_DEBUG("Yongzhou_CheckBaseHu wangba cannot dianpaohu");
		return false;
	}
	if (curCard)
	{
		LLOG_DEBUG("Yongzhou_CheckBaseHu handcardsize:%d outcard:%d,%d  isout:%d", handcard.size(), curCard->m_color, curCard->m_number, is_out_card);
		handcard.push_back(curCard);
	}

	std::vector<Lint> card_vector(40, 0);
	Lint wangbaNum = 0;
	bool ret = false;

	//第一步获取癞子,分数量统计
	for (int x = 0; x < handcard.size(); x++)
	{
		if (handcard[x]->m_color == wangbaCard->m_color && handcard[x]->m_number == wangbaCard->m_number)
		{
			++wangbaNum;
		}
		else if (handcard[x]->m_color == 4 && handcard[x]->m_number == 5) //有红中不能胡
		{
			return ret;
		}
		else
		{
			card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
		}
	}

	if (wangbaNum > 1) //最多只能有一张
	{
		return ret;
	}

	//先判断属于见字胡的直接不让胡
	if (wangbaNum)
	{
		//如果摸的是王牌，那么不会让他胡见字胡的
		if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number)
		{
		}
		else
		{
			if (curCard)
			{
				std::vector<Lint> tmp_cards(card_vector);
				tmp_cards[(curCard->m_color - 1) * 10 + (curCard->m_number) * 1]--;
				if (explorer_zheng_function(tmp_cards, 0, 0))
				{
					if (!is_out_card)
					{
						playtypes.push_back(HU_JIANZI);
						ret = true;
					}
					return ret;
				}
			}
		}
	}

	if (wangbaNum == 0)
	{
		if (handcard.size() == 2)
		{
			if (handcard[0] && handcard[1] &&(handcard[0]->m_color == handcard[1]->m_color) && (handcard[0]->m_number == handcard[1]->m_number))
			{
				playtypes.push_back(HU_YINGHU);
				ret = true;
				return ret;
			}
		}
	}

	//见字胡的判断
	if (wangbaNum && !is_out_card)
	{
		std::vector<Lint> tmp_cards(card_vector);
		if (handcard.size() == 2)
		{
			if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number)
			{
				playtypes.push_back(HU_RUANHU);
			}
			else
			{
				playtypes.push_back(HU_JIANZI);
			}
			ret = true;
			return ret;
		}
		else if (handcard.size() > 2)
		{
			//只要归位，就是硬胡
			tmp_cards[(wangbaCard->m_color - 1) * 10 + (wangbaCard->m_number) * 1]++;
			if (explorer_zheng_function(tmp_cards, 0, 0))
			{
				playtypes.push_back(HU_YINGHU);
				ret = true;
				return ret;
			}
		}
	}

	//判断其他
	if (checkJingmenOtherHu(card_vector, curCard, is_out_card, wangbaCard, wangbaNum, playtypes, gameInfo))
	{
		bool haveshuaipai = false;
		bool haveruanhu = false;
		bool haveyinghu = false;
		bool havajianzi = false;
		auto iter = playtypes.begin();
		while (iter != playtypes.end())
		{
			if (*iter == HU_SHUAIPAI)
			{
				if (haveshuaipai)
				{
					iter = playtypes.erase(iter);
					continue;
				}
				haveshuaipai = true;
			}

			if (*iter == HU_RUANHU)
			{
				if (haveruanhu)
				{
					iter = playtypes.erase(iter);
					continue;
				}
				haveruanhu = true;
			}

			if (*iter == HU_YINGHU)
			{
				if (haveyinghu)
				{
					iter = playtypes.erase(iter);
					continue;
				}
				haveyinghu = true;
			}

			if (*iter == HU_JIANZI)
			{
				if (havajianzi)
				{
					iter = playtypes.erase(iter);
					continue;
				}
				havajianzi = true;
			}
			++iter;
		}

		if (haveyinghu && haveruanhu)
		{
			iter = playtypes.begin();
			while (iter != playtypes.end())
			{
				if (*iter == HU_RUANHU)
				{
					iter = playtypes.erase(iter);
					continue;
				}
				++iter;
			}
		}

		ret = true;
	}

	return ret;
}

bool CardManager::Yongzhou_CheckBaseHu(CardVector handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* curCard, bool is_out_card, Card * wangbaCard, std::vector<HU_PAI_LEIXING> &playtypes)
{
	if (NULL == wangbaCard)
	{
		return false;
	}
	if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number && is_out_card)
	{
		LLOG_DEBUG("Yongzhou_CheckBaseHu wangba cannot dianpaohu");
		return false;
	}
	if (curCard)
	{
		LLOG_DEBUG("Yongzhou_CheckBaseHu handcardsize:%d outcard:%d,%d  isout:%d", handcard.size(), curCard->m_color, curCard->m_number, is_out_card);
		handcard.push_back(curCard);
	}
	std::vector<Lint> card_vector(40, 0);
	std::vector<Lint> WangdiaoCard_vector(40, 0);
	Lint WangdiaoWangbaNum = 0;
	Lint ColorCardNum[5] = {0};
	Lint TotalColorNum = 0;
	playtypes.push_back(HU_XIAOHU);
	Lint wangbaNum = 0;
	bool ret = false;
	//第一步获取癞子,分数量统计
	for (int x = 0; x < handcard.size(); x++)
	{
		if (handcard[x]->m_color == wangbaCard->m_color && handcard[x]->m_number == wangbaCard->m_number)
		{
			wangbaNum++;
		}
		else if (handcard[x]->m_color == 4 && handcard[x]->m_number == 5)
		{
			ColorCardNum[wangbaCard->m_color - 1]++;
			card_vector[(wangbaCard->m_color - 1) * 10 + (wangbaCard->m_number) * 1]++;
		}
		else
		{
			ColorCardNum[(handcard[x]->m_color - 1)]++;
			card_vector[(handcard[x]->m_color - 1) * 10 + (handcard[x]->m_number) * 1]++;
		}
		if (x == handcard.size() - 2)
		{
			WangdiaoCard_vector = card_vector;
			WangdiaoWangbaNum = wangbaNum;
		}
	}
	for (auto &card : pengCard)
	{
		ColorCardNum[card->m_color - 1]++;
	}
	for (auto &card : agangCard)
	{
		ColorCardNum[card->m_color - 1]++;
	}
	for (auto &card : mgangCard)
	{
		ColorCardNum[card->m_color - 1]++;
	}
	for (auto &card : eatCard)
	{
		ColorCardNum[card->m_color - 1]++;
	}
	//判断清一色
	if (ColorCardNum[3] == 0 && ColorCardNum[4] == 0)
	{
		for (Lint i = 0; i < 3; i++)
		{
			if (ColorCardNum[i] != 0)
			{
				TotalColorNum++;
			}
		}
		if (TotalColorNum == 1)
		{
			playtypes.push_back(HU_QINGYISE);
		}
	}

	//判断其他
	bool juge_wangba_jiang = true;
	for (int x = 0; x < card_vector.size(); x++)
	{
		int index = x % 10;
		if (index == 0)
		{
			continue;
		}
		else if (x > 37)
		{
			break;
		}

		if (card_vector[x] > 1)  //将牌无王霸
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 2;
			if (explorer_zheng_function(tmp_cards, 0, wangbaNum))
			{
				if (wangbaNum == 4)
				{
					playtypes.push_back(HU_SiDaTianWang);
				}
				ret = true;
			}

			//判断王闯，王闯王
			if (WangdiaoWangbaNum >= 2)
			{
				std::vector<Lint> tmp_cards(WangdiaoCard_vector);
				tmp_cards[x] -= 2;
				if (explorer_zheng_function(tmp_cards, 0, WangdiaoWangbaNum - 2))
				{
					if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number)
					{
						playtypes.push_back(HU_WangChuangWang);
						ret = true;
					}
					else if (curCard)
					{
						playtypes.push_back(HU_WangChuang);
						ret = true;
					}
				}
			}

			//判断王归位，双王归位
			if (wangbaNum >= 2)
			{
				std::vector<Lint> tmp_cards(card_vector);
				Lint WangbaShunziNum = 0;
				tmp_cards[x] -= 2;
				if (wangbaCard->m_color < 4)
				{
					tmp_cards[(wangbaCard->m_color - 1) * 10 + wangbaCard->m_number] += 2;
				}
				if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 2, &WangbaShunziNum, wangbaCard))
				{
					if (2 <= WangbaShunziNum)
					{
						playtypes.push_back(HU_ShuangWangGuiWei);
					}
					if (1 == WangbaShunziNum)
					{
						playtypes.push_back(HU_WangGuiWei);
					}
					ret = true;;
				}
			}
			if (wangbaNum >= 1)
			{
				std::vector<Lint> tmp_cards(card_vector);
				Lint WangbaShunziNum = 0;
				tmp_cards[x] -= 2;
				if (wangbaCard->m_color < 4)
				{
					tmp_cards[(wangbaCard->m_color - 1) * 10 + wangbaCard->m_number] += 1;
				}
				if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 1, &WangbaShunziNum, wangbaCard))
				{
					if (1 == WangbaShunziNum)
					{
						playtypes.push_back(HU_WangGuiWei);
					}
					ret = true;
				}
			}
		}
		else if (card_vector[x] == 1 && wangbaNum > 0)  //将牌一张王霸，一张普通牌
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 1))
			{
				if (wangbaNum == 4)
				{
					playtypes.push_back(HU_SiDaTianWang);
				}
				ret = true;;
			}
			//判断王闯，王闯王
			if (WangdiaoWangbaNum > 2)
			{
				std::vector<Lint> tmp_cards(WangdiaoCard_vector);
				tmp_cards[x] -= 1;
				if (explorer_zheng_function(tmp_cards, 0, WangdiaoWangbaNum - 3))
				{
					if (curCard && curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number)
					{
						playtypes.push_back(HU_WangChuangWang);
						ret = true;
					}
					else if(curCard)
					{
						playtypes.push_back(HU_WangChuang);
						ret = true;
					}
				}
			}

			//判断王归位，双王归位
			if (wangbaNum >= 3)
			{
				std::vector<Lint> tmp_cards(card_vector);
				Lint WangbaShunziNum = 0;
				tmp_cards[x] -= 1;
				if (wangbaCard->m_color < 4)
				{
					tmp_cards[(wangbaCard->m_color - 1) * 10 + wangbaCard->m_number] += 2;
				}
				if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 3, &WangbaShunziNum, wangbaCard))
				{
					if (2 <= WangbaShunziNum)
					{
						playtypes.push_back(HU_ShuangWangGuiWei);
					}
					if (1 == WangbaShunziNum)
					{
						playtypes.push_back(HU_WangGuiWei);
					}
					ret = true;;
				}
			}
			if (wangbaNum >= 2)
			{
				std::vector<Lint> tmp_cards(card_vector);
				Lint WangbaShunziNum = 0;
				tmp_cards[x] -= 1;
				if (wangbaCard->m_color < 4)
				{
					tmp_cards[(wangbaCard->m_color - 1) * 10 + wangbaCard->m_number] += 1;
				}
				if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 2, &WangbaShunziNum, wangbaCard))
				{
					if (1 == WangbaShunziNum)
					{
						playtypes.push_back(HU_WangGuiWei);
					}
					ret = true;
				}
			}
		}
		else {
			if (wangbaNum > 1 && juge_wangba_jiang)  ///将牌为两张王霸
			{
				juge_wangba_jiang = false;
				if (explorer_zheng_function(card_vector, 0, wangbaNum - 2))
				{
					if (wangbaNum == 4)
					{
						playtypes.push_back(HU_SiDaTianWang);
					}
					ret = true;;
				}
				//判断王归位，双王归位
				if (wangbaNum >= 3)
				{
					std::vector<Lint> tmp_cards(card_vector);
					Lint WangbaShunziNum = 0;
					if (wangbaCard->m_color < 4)
					{
						tmp_cards[(wangbaCard->m_color - 1) * 10 + wangbaCard->m_number] += 1;
					}
					if (explorer_zheng_function(tmp_cards, 0, wangbaNum - 3, &WangbaShunziNum, wangbaCard))
					{
						if (1 == WangbaShunziNum)
						{
							playtypes.push_back(HU_WangGuiWei);
						}
						ret = true;
					}
				}
			}
		}
	}
	//判断王调、王调王
	if (WangdiaoWangbaNum > 0)
	{
		std::vector<Lint> tmp_cards(WangdiaoCard_vector);
		Lint WangbaShunziNum = 0;
		if (explorer_zheng_function(tmp_cards, 0, WangdiaoWangbaNum - 1, &WangbaShunziNum))
		{
			if (curCard && (curCard->m_color == wangbaCard->m_color && curCard->m_number == wangbaCard->m_number))
			{
				playtypes.push_back(HU_WangDiaoWang);
			}
			else
			{
				playtypes.push_back(HU_WangDiao);
			}
			ret = true;
		}
	}
	LLOG_DEBUG("Yongzhou_CheckBaseHu ret:%d", ret);
	return ret;
}

bool CardManager::explorer_zheng_function2(Lint loopsize, std::vector<Lint> & huIndexs, std::vector<Lint> & huShunKes, std::vector<Lint> cards, Lint index, Lint Lai_num, Lint *wangbashunzi, Card *Wangba)
{
	bool inner_ret = false;
	for(;;)
	{
		++loopsize;
		if (loopsize >= 100000)
		{
			LLOG_ERROR("explorer_zheng_function2 loop out size %d", loopsize);
			break;
		}
		if (index<39)
		{
			if (index % 10 == 0)
			{
				index++;
			}

			if (cards[index]>0)
			{
				std::vector<Lint> tmp_cards(cards);
				Lint ke_dif = Lai_num - judge_ke(tmp_cards, index);
				if (ke_dif >= 0)
				{
					if (explorer_zheng_function2(loopsize, huIndexs, huShunKes, tmp_cards, index, ke_dif, wangbashunzi, Wangba))
					{
						if (ke_dif == 0 && Lai_num == 1)
						{
							huShunKes.push_back(2);
							huIndexs.push_back(index);
							index++;
							inner_ret = true;
							continue;
						}
						inner_ret = true;
						break;
					}
				}

				if (index % 10<8 && index<30)
				{
					std::vector<Lint> tmp_cards2(cards);
					std::vector<Lint> need_vector;
					Lint shun_dif = Lai_num - judge_shun2(tmp_cards2, index, need_vector);
					if (shun_dif >= 0)
					{
						if (explorer_zheng_function2(loopsize, huIndexs, huShunKes, tmp_cards2, index, shun_dif, wangbashunzi, Wangba))
						{
							if (shun_dif == Lai_num && wangbashunzi && Wangba && \
								Wangba->m_color == ((index / 10) + 1) && Wangba->m_number >= index % 10 && Wangba->m_number <= ((index % 10) + 3))
							{
								(*wangbashunzi)++;
							}
							if (shun_dif == 0 && Lai_num == 1 && need_vector.size())
							{
								huShunKes.push_back(1); //刻字肯定是刻子 单顺子未必是顺子，有可能去了两个的，顺子。
								huIndexs.push_back(need_vector[0]);
								index++;
								inner_ret = true;
								continue;
							}
							inner_ret = true;
							break;
						}
					}
				}
				break;
			}
			else {
				if (Lai_num>0 && index % 10<8 && index<30)
				{
					std::vector<Lint> tmp_cards(cards);
					std::vector<Lint> need_vector;
					Lint shun_dif = Lai_num - judge_shun2(tmp_cards, index, need_vector);
					if (shun_dif >= 0)
					{
						if (explorer_zheng_function2(loopsize, huIndexs, huShunKes, tmp_cards, index, shun_dif, wangbashunzi, Wangba))
						{
							if (shun_dif == Lai_num && wangbashunzi && Wangba && \
								Wangba->m_color == ((index / 10) + 1) && Wangba->m_number >= index % 10 && Wangba->m_number <= ((index % 10) + 3))
							{
								(*wangbashunzi)++;
							}
							if (shun_dif == 0 && Lai_num == 1 && need_vector.size())
							{
								huShunKes.push_back(1);
								huIndexs.push_back(need_vector[0]);
								index++;
								inner_ret = true;
								continue;
							}
							inner_ret = true;
							break;
						}
					}
				}
				index++;
				if (explorer_zheng_function2(loopsize, huIndexs, huShunKes, cards, index, Lai_num, wangbashunzi, Wangba))
				{
					return true;
				}
				break;
			}
		}
		else {
			return true;
		}
	}

	return inner_ret;
}

bool CardManager::explorer_zheng_function(std::vector<Lint> cards,Lint index,Lint Lai_num, Lint *wangbashunzi, Card *Wangba)
{
	if(index<39)
	{
		if(index%10==0)
		{
			index++;
		}

		if(cards[index]>0)
		{
			std::vector<Lint> tmp_cards(cards);
			Lint ke_dif = Lai_num-judge_ke(tmp_cards,index);
			if(ke_dif>=0)
			{
				if (explorer_zheng_function(tmp_cards, index, ke_dif, wangbashunzi, Wangba))
				{
					return true;
				}
			}

			if(index%10<8&&index<30)
			{
				std::vector<Lint> tmp_cards2(cards);
				Lint shun_dif = Lai_num-judge_shun(tmp_cards2,index);
				if(shun_dif>=0)
				{
					if (explorer_zheng_function(tmp_cards2, index, shun_dif, wangbashunzi, Wangba))
					{
						if (shun_dif == Lai_num && wangbashunzi && Wangba && \
							Wangba->m_color == ((index/10)+1) && Wangba->m_number>=index%10 && Wangba->m_number <= ((index % 10)+3))
						{
							(*wangbashunzi)++;
						}
						return true;
					}
				}
			}
			return false;
		}
		else{
			if(Lai_num>0&&index%10<8&&index<30)
			{
				std::vector<Lint> tmp_cards(cards);
				Lint shun_dif = Lai_num-judge_shun(tmp_cards,index);
				if(shun_dif>=0)
				{
					if(explorer_zheng_function(tmp_cards,index,shun_dif, wangbashunzi, Wangba))
					{
						if (shun_dif == Lai_num && wangbashunzi && Wangba && \
							Wangba->m_color == ((index / 10) + 1) && Wangba->m_number >= index % 10 && Wangba->m_number <= ((index % 10) + 3))
						{
							(*wangbashunzi)++;
						}
						return true;
					}
				}
			}
			index++;
			if(explorer_zheng_function(cards,index,Lai_num, wangbashunzi, Wangba))
			{
				return true;
			}
			return false;
		}
	}
	else{
		return true;
	}
}

Lint CardManager::judge_ke(std::vector<Lint>& targe_vector,Lint index)
{
	Lint Lai_num = 0;
	switch(targe_vector[index])
	{
	case 4:
	case 3:
		{
			targe_vector[index]-=3;
			Lai_num=0;
			break;
		}
	case 2:
		{
			targe_vector[index]-=2;
			Lai_num=1;
			break;
		}
	case 1:
		{
			targe_vector[index]-=1;
			Lai_num=2;
			break;
		}
	case 0:
		{
			Lai_num=3;
			break;
		}
	default:
		return 100;
		break;
	}
	return Lai_num;
}

Lint CardManager::judge_shun2(std::vector<Lint>& targe_vector, Lint index, std::vector<Lint>& need_vector)
{
	Lint Lai_num = 3;
	if (targe_vector[index]>0)
	{
		targe_vector[index]--;
		Lai_num--;
	}
	else
	{
		need_vector.push_back(index);
	}
	if (targe_vector[index + 1]>0)
	{
		targe_vector[index + 1]--;
		Lai_num--;
	}
	else
	{
		need_vector.push_back(index + 1);
	}
	if (targe_vector[index + 2]>0)
	{
		targe_vector[index + 2]--;
		Lai_num--;
	}
	else
	{
		need_vector.push_back(index + 2);
	}
	return Lai_num;
}

Lint CardManager::judge_shun(std::vector<Lint>& targe_vector,Lint index)
{
	Lint Lai_num =3;
	if(targe_vector[index]>0)
	{
		targe_vector[index]--;
		Lai_num--;
	}
	if(targe_vector[index+1]>0)
	{
		targe_vector[index+1]--;
		Lai_num--;
	}
	if(targe_vector[index+2]>0)
	{
		targe_vector[index+2]--;
		Lai_num--;
	}
	return Lai_num;
}

bool CardManager::have_play_type(Lint playtype,std::vector<Lint> playtypes)
{
	for(auto x=playtypes.begin();x!=playtypes.end();x++)
	{
		if(*x == playtype)
			return true;
	}
	return false;
}

bool CardManager::XiangTanCheckTing(CardVector handcard, CardVector& pengCard, CardVector& agangCard, CardVector& mgangCard, CardVector& eatCard, Card* curCard, bool isGetCard, OperateState& gameInfo)
{
	HU_PAI_LEIXING huType;
	std::vector<HU_PAI_LEIXING> huTypes;
	Card *outCard;
	if (gameInfo.m_wangBa && gameInfo.m_wangBa->m_color != 0 && gameInfo.m_wangBa->m_number != 0)
	{
		outCard = gameInfo.m_wangBa;
	}
	if (gameInfo.m_wangBa2 && gameInfo.m_wangBa2->m_color != 0 && gameInfo.m_wangBa2->m_number != 0)
	{
		outCard = gameInfo.m_wangBa2;
	}

	if (isGetCard && curCard)
	{
		handcard.push_back(curCard);
	}
	gCardMgr.SortCard(handcard);

	//小七对胡牌
	if (CheckqiaoduiWithWangba(handcard, huType, outCard, true, gameInfo))
	{
		LLOG_DEBUG("XiangTanCheckTing true");
		return true;
	}
	for (int i = 0; i < pengCard.size(); i += 3)
	{
		CardVector tempCards(handcard);
		gCardMgr.EraseCard(tempCards, pengCard[i], 1);
		if (XiangTan_CheckBaseHu(tempCards, pengCard, agangCard, mgangCard, eatCard, outCard, false, gameInfo.m_wangBa, gameInfo.m_wangBa2, huTypes))
		{
			LLOG_DEBUG("XiangTanCheckTing true");
			return true;
		}
	}

	CardVector one, two, three, four;
	GetSpecialOneTwoThreeFour(handcard, one, two, three, four);
	if (four.size() > 0)
	{
		for (int i = 0; i < four.size(); i += 4)
		{
			CardVector tempCards(handcard);
			gCardMgr.EraseCard(tempCards, four[i], 1);
			if (XiangTan_CheckBaseHu(tempCards, pengCard, agangCard, mgangCard, eatCard, outCard, false, gameInfo.m_wangBa, gameInfo.m_wangBa2, huTypes))
			{
				LLOG_DEBUG("XiangTanCheckTing true");
				return true;
			}
		}
	}
	//正常胡法
	if (XiangTan_CheckBaseHu(handcard, pengCard, agangCard, mgangCard, eatCard, outCard, false, gameInfo.m_wangBa, gameInfo.m_wangBa2, huTypes))
	{
		LLOG_DEBUG("XiangTanCheckTing true");
		return true;
	}
	LLOG_DEBUG("XiangTanCheckTing false");
	return false;

}

bool CardManager::CheckTing(CardVector& handcard,CardVector& pengCard,CardVector& agangCard,CardVector& mgangCard,CardVector& eatCard,OperateState& gameInfo)
{
	//杠后不可能胡小七对 先删掉
	if(CheckLuanjianghu(handcard,eatCard,pengCard,agangCard,mgangCard,NULL))
	{
		return true;
	}
	if(handcard.size()==1)//听全球人
	{
		return true;
	}
	if(xiaohu_CheckTing(handcard,true))
	{
		return true;
	}	
	
	if(xiaohu_CheckTing(handcard,false))
	{
		//清一色
		bool qingyise=true;
		CardVector tmp(handcard);
		tmp.insert(tmp.end(),pengCard.begin(),pengCard.end());
		tmp.insert(tmp.end(),agangCard.begin(),agangCard.end());
		tmp.insert(tmp.end(),mgangCard.begin(),mgangCard.end());
		tmp.insert(tmp.end(),eatCard.begin(),eatCard.end());
		Lint color = 0;
		for(int x=0;x<tmp.size();x++)
		{
			if(x==0)
			{
				color=tmp[0]->m_color;
			}
			else{
				if(color!=tmp[x]->m_color)
				{
					qingyise=false;
					break;
				}
			}
		}
		if(qingyise)
		{
			return true;
		}

		//碰碰胡
		bool pengpenghu=false;
		
		tmp.clear();
		tmp.insert(tmp.end(),handcard.begin(),handcard.end());
		CardVector one,two,three,four;
		SortCard(tmp);
		GetSpecialOneTwoThreeFour(tmp,one,two,three,four);

		if(one.size()==1&&two.empty()&&four.empty())
		{
			pengpenghu=true;
		}
		else if(one.empty()&&two.size()==4&&four.empty())
		{
			pengpenghu=true;
		}

		if(!eatCard.empty())
		{
			pengpenghu=false;
		}

		if(pengpenghu)
		{
			return true;
		}
	}
	return false;
}
bool CardManager::XiaoYi_CheckTing(std::vector<Lint>& card_vector,std::vector<Lint> & huIndexs, std::vector<Lint> & huShunKes, CardVector handcard, CardVector pengCard, CardVector agangCard, CardVector mgangCard, CardVector eatCard)
{
	return xiaohu_CheckTing2(card_vector, false, huIndexs, huShunKes);
}
bool CardManager::YiYang_CheckTing(CardVector handcard, CardVector pengCard, CardVector agangCard, CardVector mgangCard, CardVector eatCard)
{
	bool ret = false;
	if (xiaohu_CheckTing(handcard, false))
	{
		ret = true;
	}
	else if (pengCard.size() == 0 && agangCard.size() == 0 && mgangCard.size() == 0 && eatCard.size() == 0)
	{
		CardVector one, two, three, four;
		GetSpecialOneTwoThreeFour(handcard, one, two, three, four);
		if (one.size() + three.size() / 3 == 1)
		{
			ret = true;
		}
	}
	else if (CheckLuanjianghu(handcard, pengCard, agangCard, mgangCard, eatCard, NULL))
	{
		ret = true;
	}
	return ret;
}

bool CardManager::xiaohu_CheckTing2(std::vector<Lint> & card_vector, bool needJiang, std::vector<Lint> & huIndexs, std::vector<Lint> & huShunKes)
{
	Lint Lai_num = 1;
	int juge_lai_jiang = true;
	Lint card_vector_size = card_vector.size();

	Lint loopsize = 0;
	for (int x = 0; x < card_vector_size; x++)
	{
		++loopsize;
		if (loopsize >= 100000)
		{
			LLOG_ERROR("xiaohu_CheckTing2 LOOP out size %d", loopsize);
			break;
		}
		int index = x % 10;
		if (x % 10 == 0)
		{
			continue;
		}
		else if (x>37)
		{
			break;
		}

		if (needJiang && (index != 2 && index != 5 && index != 8))
		{
			if (x == 30)
				return false;
			continue;
		}

		if (card_vector[x]>1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 2;
			explorer_zheng_function2(loopsize, huIndexs, huShunKes, tmp_cards, 0, Lai_num);
		}
		else if (card_vector[x] == 1 && Lai_num>0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x] -= 1;
			if (explorer_zheng_function2(loopsize, huIndexs, huShunKes, tmp_cards, 0, Lai_num - 1))
			{
				huShunKes.push_back(2);
				huIndexs.push_back(x);
			}
		}
		else {
			if (Lai_num>1 && juge_lai_jiang)
			{
				juge_lai_jiang = false;
				explorer_zheng_function2(loopsize, huIndexs, huShunKes, card_vector, 0, Lai_num - 2);
			}
		}
	}
	return huIndexs.size() >= 1;
}

//目前只针对长沙
bool CardManager::xiaohu_CheckTing(CardVector handcard,bool needJiang)
{
	std::vector<Lint> card_vector(40,0);
	Lint Lai_num=1;

	//第一步获取癞子,分数量统计
	for(int x=0;x<handcard.size();x++)
	{
		card_vector[(handcard[x]->m_color-1)*10+(handcard[x]->m_number)*1]++;
	}

	int juge_lai_jiang = true;
	for(int x=0;x<card_vector.size();x++)
	{
		int index = x%10;
		if(x%10==0)
		{
			continue;
		}
		else if(x>37)
		{
			break;
		}

		if(needJiang&&(index!=2&&index!=5&&index!=8))
		{
			if(x==30)
				return false;
			continue;
		}

		if(card_vector[x]>1)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x]-=2;
			if(explorer_zheng_function(tmp_cards,0,Lai_num))
			{
				return true;
			}
		}
		else if(card_vector[x]==1&&Lai_num>0)
		{
			std::vector<Lint> tmp_cards(card_vector);
			tmp_cards[x]-=1;
			if(explorer_zheng_function(tmp_cards,0,Lai_num-1))
			{
				return true;
			}
		}
		else{
			if(Lai_num>1&&juge_lai_jiang)
			{
				juge_lai_jiang = false;
				if(explorer_zheng_function(card_vector,0,Lai_num-2))
				{
					return true;
				}
			}
		}
	}
	return false;
}
