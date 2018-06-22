#ifndef GUANGDONG_ONEHUNDREDHANDLER
#define GUANGDONG_ONEHUNDREDHANDLER

#include "GameHandler.h"
#include "Desk.h"
#include "Card.h"
#include "LTime.h"
#include "LVideo.h"
#include "LLog.h"
#include "Config.h"
#include "RoomVip.h"
#include "Work.h"


class GameHandler_GuangDongOneHundler : public GameHandler
{
public:
	virtual bool startup(Desk *desk);
	virtual void shutdown(void);
	virtual void SetDeskPlay();
	virtual void ProcessRobot(Lint pos, User * pUser);
	virtual void HanderUserPlayCard(User* pUser,LMsgC2SUserPlay* msg);
	virtual void HanderUserOperCard(User* pUser,LMsgC2SUserOper* msg);
	virtual void GenerateGhost();
	virtual void DeakCard();
	virtual void FillspecifichuType(OperateState& gameinfo);
	virtual bool CanFourGhostHu() const;
	virtual Lint GetSocore(const Lint pos);
	virtual Lint GetGhostType() const;
	virtual bool gametypeDianPao() const;
	virtual void SetDeskPlayNum();
	virtual void SetThinkIng();
	virtual void SetPlayIng(Lint pos, bool needGetCard, bool gang, bool needThink, bool canhu, bool bu = false);
	virtual void OnUserReconnect(User* pUser);
	virtual void OnGameOver(Lint result,Lint winpos[],Lint bombpos, Lint winner = 0);
	virtual void CheckStartPlayCard();
	
	virtual bool IsGangshangkaihuaScore() const;
	virtual bool IsPayQianggangHorseScore() const;
	virtual bool calcScore(Lint result, Lint winpos[],Lint bombpos, Ldouble gold[], Lint& bombCount, Lint& zhuangpos, Lint maNum[], LMsgS2CGameOver& over, Lint winner);
	virtual void calHorsrScore(Lint result,Lint winpos[],Lint bombpos, Ldouble gold[], Lint maNum[], LMsgS2CGameOver& over, Lint winner);
	virtual void calCardTypeScore(Lint result, Lint winpos[], Lint bombpos, Ldouble gold[]);
	virtual void calBuyHorseScore(Lint winpos[],Lint bombpos, Ldouble gold[], Lint maNum[], LMsgS2CGameOver& over);
	virtual void calBombHorseScore(Lint result, Lint winpos[], Lint bombpos, Ldouble gold[], Lint maNum[], LMsgS2CGameOver& over);
	virtual void calHorseGenGangScore(Lint winpos[], Lint bombpos, Ldouble gold[], const std::vector<CardValue>& horsecards, Lint winner);
	virtual void ThinkEnd();
	virtual void calHorseNum(LMsgS2CZhaBirdShow &maPaiStruct, Lint winpos[], Lint maNum[]);
	virtual void calRedDragonSceor(Lint winpos[], Lint bombpos, Ldouble gold[]);
	virtual bool CanQiangGanghu() const;
	virtual bool IsBompHorseAddScoreType() const;
	virtual bool IsBompHorseMultipleScoreType() const;
	virtual bool CanTianhu();
	virtual bool CanDihu();
	virtual bool CanQiangGangHuHorseJiaBei();
	virtual bool CanHuangZhuangHuangGang();

	virtual bool IsNeedWindCards() const;
	virtual bool IsHorseGenGang() const;
	virtual bool IsSteadilyHighType() const;
	virtual bool IsBombHorse() const;
	virtual bool IsGenZhangOnlyOnce() const;
	virtual Lint GetMaPaiBaseScore(const Lint pos);
	virtual void UpdateZhuangPos(Lint result, Lint winpos[], Lint bombpos, Lint& zhuangpos);
	virtual bool calcMaPai(Lint zhuangPos, Lint winPos, Lint maPaiNum);
	virtual void calGenZhuangScore(Ldouble gold[]);
	virtual bool IsLiuju();
	virtual void calGangScore(Lint result, Ldouble gold[]);

	bool IsNoGhostType();
	bool IsNoHorseType();
	void CheckThink();
	void VideoThink(Lint pos);
	void VideoDoing(Lint op, Lint pos, Lint card_color, Lint card_number);
	bool CheckIfBigHu(std::vector<Lint>& vec);
	bool IsXiaoQiDuiType();
	Lint GameBuyhorseNum();
	Lint GetGengzhuangBaseScore();
	void UpdateGenZhuangInfo(const Card* const outCard, const Lint pos, const std::vector<CardValue>& cards);
	
	bool IsZhuang(Lint pos);
	bool IsQiduiDoubleScore() const;
	bool IsGhostCardBaiban() const;
	bool CanGengZhuang() const;
	bool IsMaGenDifen() const;
	bool IsTiDihu10Time() const;
	//�޹�ӱ�
	bool IsNoGhost2TimesScore() const;
	//�Ĺ�ӱ�
	bool IsFourGhost2TimesScore() const;
	//�ԶԺ��ӱ�
	bool IsDuiduihuType() const;
	//��һɫ�ı�
	bool IsQingyiseType() const;
	//�۾�����
	bool IsYaojiuType() const;
	//�۾��ƺ�1����9����
	bool IsYaojiuHasOneOrNine() const;
	//��һɫ�˱�
	bool IsAllWindType() const;
	//ʮ���۰˱�
	bool IsShisanyaoType() const;
	//��һɫ˫��
	bool IsHunyise2TimesScore() const;

	//˫�����߶�
	bool IsDLuxuryqiduiType() const;
	//������
	bool IsTLluxuryqiduiType() const;
	//ʮ���޺�
	bool IsEighteenarhats() const;
	//С��Ԫ
	bool IsLittleSanyuanType() const;
	//����Ԫ
	bool IsBigSanyuanType() const;
	//С��ϲ
	bool IsLittleSixiType() const;
	//����ϲ
	bool IsBigSixiType() const;

	bool IsMingGangkeqiang() const;
	bool IsQiangGangHu(std::vector<Lint>& hu);
	bool IsGangshangKaihuaHu(std::vector<Lint>& hu);
	Lint GetGhostCardNum(Lint pos) const;
	bool IsValidPos(const Lint& pos) const;
	const std::vector<CardValue> GetGhostCard() const;
	void UpdateEndInfo(Lint winpos[], Lint maNum[], const Lint result);
	void UpdateSuccessiveInfo(Lint prezhuangPos, Lint curzhuangPos);

protected:
	const std::vector<CardValue> GetHorseFromBirdInfo(const std::vector<BirdInfo>& birdinfo);
	bool m_gengZhengOnceEnd;

protected:
	void calPlayerHitHorseNum(const std::vector<CardValue>& horsecards, Lint maNum[]);
	Lint GetHorseGenGangHuPos(Lint winpos[], Lint bombpos, Lint winner ) const;
	Lint GetQihuFanshu();

public:
	Lint			m_maNum[DEFAULT_DESK_USER_COUNT];
	Lint			m_successive[DEFAULT_DESK_USER_COUNT];		//��ׯ	
	Lint			m_ghostZimo[DEFAULT_DESK_USER_COUNT];		//����й���������
	Lint			m_noghostZimo[DEFAULT_DESK_USER_COUNT];		//����޹���������
	std::vector<CardValue>	m_ghostCardReal;			//����
	Card			m_ghostCardFlop;					//�����Ĺ��� ���͸��ͻ���
	bool			m_oper_gang;						//���˳���Ȼ���
	GengZhuangInfo  m_gengzhuangInfo;
	Lint			m_redDragonNum[DEFAULT_DESK_USER_COUNT];	//����������
	CardVector		m_flowerCard[DEFAULT_DESK_USER_COUNT]; //����ѿ�����
protected:
	Lint			m_playCardNum;		//÷�ݺ��б��ĵغ��ͱ���淨��һ�����ڶ����������ǵغ�
	Lint			m_operateNum;		//������ҳ����ܴ���
	bool			m_canTianHu;		//���
	bool			m_canDiHu;			//�غ�

	OLD_TICK_TUOGUAN_DECLARE
};

DECLARE_GAME_HANDLER_CREATOR(GuangDongOneHundred, GameHandler_GuangDongOneHundler)

#endif