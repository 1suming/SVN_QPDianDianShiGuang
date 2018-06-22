#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "LBase.h"

#define CARD_COUNT 144
#define CARD_COUNT_DDZ 54

#define DESK_USER_COUNT 4	//一桌玩家数量;
#define DEFAULT_INVAILD_POS		4  //无效的位置

#define INVAILD_POS		4  //无效的位置;
#define INVAILD_VAL     (-1)

#define GAME_APPID 15088 //游戏id定义

enum CARD_COLOR
{
	CARD_COLOR_WAN = 1,
	CARD_COLOR_TUO = 2,
	CARD_COLOR_SUO = 3,
	CARD_COLOR_FENG = 4,
	CARD_COLOR_JIAN = 5,
	CARD_COLOR_FLOWER = 6,
};

enum DESK_STATE
{
	DESK_FREE =		0,	//桌子空闲状态;
	DESK_WAIT,			//桌子等待状态;
	DESK_PLAY,		    //出牌;
	QIANG_DIZHU,        //抢地主阶段;
};

enum GOLDCHANGETYPE
{
	SYSTEMFIRSTGIVE          = 0, //首次赠送金币;
	GIVEPERDAY               = 1, //每日赠送金币;
	GAMERESULT               = 2, //消耗金币(输负赢正);
	BETUSED                  = 3, //转盘活动消耗;
	BETWIN                   = 4, //转盘活动奖励;
	GOLDCOSUME               = 5, //每局金币场金币消费;
	CARDCHANGE2GOLD          = 6, //房卡换金币;
};

enum 
{
	play_card               = 0,//出;
	not_play_card,              //不出;
};
enum 
{
	play_card_need_cheek = 0, //需要清空桌面;
	play_card_not_check,      //不需要清空桌面;
};
enum 
{
	play_card_success       = 0, //出牌操作成功;
	play_card_failed,            //出牌操作失败;
};
enum RESET_ROOM_TYPE
{
	RESET_ROOM_TYPE_WAIT,
	RESET_ROOM_TYPE_AGREE,
	RESET_ROOM_TYPE_REFLUSE,
};

enum OPER_QIANGDIZHU
{
	OPER_ERROE = 0,
	OPER_MENZHUA = 1,
	OPER_MINGZHUA = 2,
	OPER_DAO = 3,
	OPER_LA = 4,
	OPER_SHAIZI_HUASE = 5,
	OPER_DIZHU_SHAIZI = 6, //告诉地主投色子;
	OPER_LAIZI = 7,
};

enum GAME_USER_OPER
{
	OPER_NO = 0,
	OPER_YES = 1,
};

enum GR_CALL_STATE
{
	call_state_error= -1,
	call_state_calling,
	call_state_rushing,
	call_state_nothing,
	call_state_notcall,
	call_state_notrush,
	call_state_call,
	call_state_rush,
};

enum PK_GAME_ZONE
{
	game_zone_roomcard = 0,  //房卡进行游戏;
	game_zone_gold = 1,      //金币进行游戏;
};

// 扑克游戏具体游戏类型;
enum PK_GAME_STYLE
{
	game_style_error = -1,
	//paodekuai_3players_16= 0,
	//paodekuai_3players_15= 1,

	game_ddz_classical = 0,
	game_ddz_sichuan = 1,
	game_ddz_sichuan_pz = 2,	//四川四人痞子
	game_ddz_2players = 3,
	game_ddz_3lai = 4,
	game_ddz_sichuan3_pz = 5,	//四川3人痞子
	game_wakeng_shanxi = 7,	//和陕西相同定义

	game_Arena_2V1_PVE = 21, //2打一项目,人机战;
	PK_GAME_STYLE_MAX_DOUDIZHU = 50,
};

//麻将类型
enum GameType
{
	ZhuanZhuanMajiang = 0 + PK_GAME_STYLE_MAX_DOUDIZHU,
	TwoPeopleZhuanZhuan = 1 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ThreePeopleZhuanZhuan = 2 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ChangShaMaJiang = 3 + PK_GAME_STYLE_MAX_DOUDIZHU,
	TwoPeopleChangSha = 4 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ThreePeopleChangSha = 5 + PK_GAME_STYLE_MAX_DOUDIZHU,
	HongZhongMajiang = 6 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ShaoYangMajiang = 9 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ChenZhouMajiang = 12 + PK_GAME_STYLE_MAX_DOUDIZHU,
	ChangDeMajiang = 15 + PK_GAME_STYLE_MAX_DOUDIZHU,
	YongZhouMajiang = 18 + PK_GAME_STYLE_MAX_DOUDIZHU,  //永州
	XiangTanMajiang = 21 + PK_GAME_STYLE_MAX_DOUDIZHU,  //湘潭
	YiYangMajiang = 24 + PK_GAME_STYLE_MAX_DOUDIZHU,    //益阳
	ChaoGuMajiang = 27 + PK_GAME_STYLE_MAX_DOUDIZHU,    //岳阳炒股
	ZhuoXiaZiMajiang = 30 + PK_GAME_STYLE_MAX_DOUDIZHU,   //岳阳捉虾子
	HengYangMajiang = 31 + PK_GAME_STYLE_MAX_DOUDIZHU, //衡阳麻将
	JingMenMajiang = 32 + PK_GAME_STYLE_MAX_DOUDIZHU,  //荆门晃晃
	XiaoYiMajiang = 33 + PK_GAME_STYLE_MAX_DOUDIZHU,  //孝义麻将
	ShuangKaiMajiang = 34 + PK_GAME_STYLE_MAX_DOUDIZHU,  //双开麻将

	PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG = 100,

	/////////////////////////////广东麻将/////////////////////////////////
	HuNanMaJing = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 0,
	GuangDongJiPingHuMaJing = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 9,	//鸡平胡
	GuangDongTuiDaoHuMaJing = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 10,	//推到胡
	GuangDongOneHundred = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 11,		//一百张
	HeYuanJokerChickenHu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 12,		//河源百搭鸡胡
	GuangDongCustomTuiDaoHu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 13,	//做牌推到胡
	GuangDongChaoZhou = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 14,			//潮汕麻将
	ThreePlayerTuiDaoHu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 15,		//三人推到胡
	HuizhouzhuangGhost = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 16,		//惠州庄-花做鬼
	RedDragon = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 17,		//红中王
	Hongkong = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 18,		//香港麻将
	HuizhouzhuangFlower = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 19,	//惠州庄-花牌
	JiHuFlower = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 20,			//鸡胡补花
	GuangDongHongZhongMajiang = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 21,  //广东红中麻将
	TwoPlayerTuiDaoHu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 22,		//两人推到胡
	GuangDongJihu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 23,			//广东鸡胡
	MeizhouHongzhonbao = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 24,	//梅州红中宝
	GuangdongJieyang = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 25,		//揭阳麻将
	Shaoguan = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 26,				//韶关麻将
	Nanxiaong = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 27,				//南雄麻将
	Qujiang = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 28,				//曲江麻将
	Maba = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 29,					//马坝麻将
	GuangdongJieXiMajiang = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 30,	//揭西麻将
	ChaoShanBiHu = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 31,			//潮汕逼胡
	GuangDongRaoping = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 32,				//饶平麻将
	LongMenZiyouzhuang = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 33,		//龙门自由庄
	HuiYangWanFa = PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG + 34,			//惠阳玩法


	PK_GAME_STYLE_MAX_GUANGDONG_MAJIANG = 150,
};

//竞技场项目,支持动态，默认20个，需要在改名字
enum SportItemType
{
	EM_SIYJ_BEGIN = 99,
	EM_SIYJ_IPHONE_ZHUANZHUAN = 100, //赢奖转转 Iphone6s 手机赛
	EM_SIYJ_FREE_MATCH = 101,		 //免费场
	EM_SIYJ_FEE_MATCH = 102,		 //收费场
	EM_SIYJ_CHRISTMAS_EVE = 103,          //
	EM_SIYJ_CHRISTMAS_DAY = 104,		     //
	EM_SIYJ_EXTERN_5 = 105,          //
	EM_SIYJ_EXTERN_6 = 106,          //
	EM_SIYJ_EXTERN_7 = 107,          //
	EM_SIYJ_EXTERN_8 = 108,         //
	EM_SIYJ_EXTERN_9 = 109,         //
	EM_SIYJ_EXTERN_10 = 110,        //
	EM_SIYJ_EXTERN_11 = 111,        //
	EM_SIYJ_EXTERN_12 = 112,        //
	EM_SIYJ_EXTERN_13 = 113,        //
	EM_SIYJ_EXTERN_14 = 114,        //
	EM_SIYJ_EXTERN_15 = 115,        //
	EM_SIYJ_EXTERN_16 = 116,        //
	EM_SIYJ_EXTERN_17 = 117,        //
	EM_SIYJ_EXTERN_18 = 118,        //
	EM_SIYJ_EXTERN_19 = 119,        //
	EM_SIYJ_EXTERN_20 = 120,        //
	EM_SIYJ_MAX = 121,
};


#define game_style_sum     (6)   //当前游戏玩法的总数，注意修改;

#define	PLAY_CARD_TIME		10  //出牌时间;
#define WAIT_READY_TIME		15  //准备时间;

enum card_color_type
{
	card_color_type_null = 0,
	card_color_type_squal,//方块;
	card_color_type_floor,//梅花;
	card_color_type_red,//红桃;
	card_color_type_black,//黑桃;
};

enum card_number_type
{
	card_number_type_null = 0,
	card_number_type_3,
	card_number_type_4,
	card_number_type_5,
	card_number_type_6,
	card_number_type_7,
	card_number_type_8,
	card_number_type_9,
	card_number_type_10,
	card_number_type_J,
	card_number_type_Q,
	card_number_type_K,
	card_number_type_A,
	card_number_type_2,
	card_number_type_SJ,
	card_number_type_BJ,
	card_number_type_LZ,
};

namespace Shanxi_Wakeng
{
	enum card_number_type
	{
		card_number_type_3 = card_number_type_LZ + 1,
	};
}

enum card_mode_style
{
	card_style_error = 0,
	card_style_single = 1,				//单张;
	card_style_double = 2,				//一对;
	card_style_three  = 3,				//三个;
	card_style_three_single = 4,		//三带单;
	card_style_three_double = 5,		//三带对;
	card_style_three_list = 6,			//飞机;
	card_style_three_list_single = 7,	//飞机带单;
	card_style_three_list_double = 8,	//飞机带对;
	card_style_bomb_and_single = 9,		//四代2单;
	card_style_bomb_and_double = 10,		//四代2对;
	card_style_single_list = 11,			//单顺;
	card_style_double_list = 12,			//双顺;
	card_style_bomb3 = 13,				//带癞子的炸弹;
	card_style_bomb2 = 14,				//纯粹硬炸弹;
	card_style_bomb1 = 15,				//纯粹软炸弹;
	card_style_rocket = 16,				//火箭（王炸）;
	card_style_four2 = 17,              //四带二;
	card_style_laizi = 30,				//癞子;
	card_style_super_bomb = 31,			//重炸;
	card_style_supper_rocket = 32,		//重王炸;
	card_style_bomb_double	= 33,		//连炸;
};

enum PK_GAME_PARA
{
	game_para_error= -1,
	game_para_maxfan = 0,
	game_para_diOrFan = 1,	//底1还是翻2
	game_para_classic3_subType = 2,	//经典3人子类型
};

Lstring getGameTypeStr(Lint n);
Lint getGameUserCounts(Lint nGameType);

#define DEFAULT_DESK_USER_COUNT 4	//一桌玩家数量

////////////////////长沙定义/////////////////////////////

#define BASE_CARD_COUNT 36
#define LAI_ZI_COUNT 4
#define MAX_HANDCARD_COUNT 13
#define WIND_CARD_COUNT 28

//家胡牌情况，0-没胡，1-自摸，2-收炮，3-点炮
enum WIN_TYPE
{
	WIN_ZIMO,		//自摸
	WIN_BOMB,		//点炮
	WIN_NONE,		//慌庄
};

//胡牌方式
enum WIN_TYPE_SUB
{
	WIN_SUB_NONE,
	WIN_SUB_ZIMO,		//自摸
	WIN_SUB_BOMB,		//收炮
	WIN_SUB_ABOMB,		//点炮

	WIN_SUB_DZIMO,		//大胡自摸
	WIN_SUB_DBOMB,		//大胡收炮
	WIN_SUB_DABOMB,		//大胡点炮  

	WIN_SUB_ABOMB2,		//2类点炮，听后点炮
};

//玩家思考类型
enum THINK_OPERATOR
{
	THINK_OPERATOR_NULL = 0,
	THINK_OPERATOR_OUT = 1,
	THINK_OPERATOR_BOMB = 2,//胡
	THINK_OPERATOR_AGANG = 3,//暗杠
	THINK_OPERATOR_MGANG = 4,//明杠
	THINK_OPERATOR_PENG = 5,//碰
	THINK_OPERATOR_CHI = 6,//吃
	THINK_OPERATOR_ABU = 7,//暗补
	THINK_OPERATOR_MBU = 8,//明补
	THINK_OPERATOR_TING = 9, //报听
	THINK_OPERATOR_XIABU = 10, //补虾牌

	THINK_OPERATOR_GHOSTPENG = 9,	//带鬼碰
	THINK_OPERATOR_GHOSTGANG1 = 10,	//带鬼补杠
	THINK_OPERATOR_GHOSTGANG2 = 11,	//对带鬼补杠再补杠
	THINK_OPERATOR_TRY_MGANG = 12, //尝试开明杠 给抢杠胡用的
};


//牌局当前状态
enum DESK_PLAY_STATE
{
	DESK_PLAY_SELECT_PIAO,	//选择飘 状态
	DESK_PLAY_START_HU,		//起手胡状态
	DESK_PLAY_GET_CARD,		//摸牌
	DESK_PLAY_THINK_CARD,	//思考
	DESK_PLAY_END_CARD,		//海底牌
	DESK_PLAY_BAO_TING,     //报听
	DESL_PLAY_XIA_BU,       //摸到虾子后补牌
	//DESK_PLAY_SEVEN_FLOWER = 4,	//七花胡牌 广东
	DESK_PLAY_SEVEN_FLOWER,	//七花胡牌
};

//起手胡牌类型
enum START_HU_INFO
{
	START_HU_NULL = 0,
	START_HU_QUEYISE = 1,	     //缺一色
	START_HU_BANBANHU = 2,	 //板板胡
	START_HU_DASIXI = 3,	     //大四喜
	START_HU_LIULIUSHUN = 4,   //六六顺
	START_HU_SANTONG = 5,      //三同
	START_HU_JIEMEIDUI = 6,    //姊妹对
	START_HU_YIZHIHUA = 7,	 //一枝花
};

//胡牌类型
enum HU_PAI_LEIXING
{
	HU_XIAOHU = 1,				//小胡
	HU_SPECIAL_XIAOQIDUI = 2,		//豪华小七对
	HU_XIAOQIDUI = 3,				//小七对
	HU_QINGYISE = 4,				//清一色
	HU_JIANGJIANGHU = 5,			//将将胡
	HU_PENGPENGHU = 6,			//碰碰胡
	HU_QUANQIUREN = 7,			//全球人
	HU_GANG1 = 8,					//杠上开花
	HU_GANG2 = 9,					//杠上炮
	HU_END1 = 10,					//海底捞
	HU_END2 = 11,					//海底炮
	HU_QIANGGANGHU = 12,			//抢扛胡
	HU_DOUBLESPECIAL_XIAOQIDUI = 14, //双豪华七小队
	HU_TianHu = 15,//天胡
	HU_DiHu = 16,//地胡
	HU_Qishou4Lai = 17, //起手四个癞子
	HU_WuHongZhong = 18,//没有红中
	HU_QiangGangHu2 = 19,//qiangganghu2


	HU_13Yao = 20,
	HU_FengYiSe = 21,
	HU_QingYiSeMenQing = 22,
	HU_QingYiSeQiXiaoDui = 23,
	HU_QingYiSeDaDuiPeng = 24,
	HU_MenQing = 25,        //门清
	HU_DaDuiPeng = 26,
	HU_ShaoYangQiQiaoDui = 27,		//七巧对
	HU_ShaoYangLongQiDui = 28,		//龙七对
	HU_ShiSanLan = 29,
	HU_QiFengDaoWei = 30,
	HU_QiQiaoDui = 31,		//七巧对
	HU_LongQiDui = 32,		//龙七对
	HU_LonngQiaoDui = 33,
	HU_YingQiaoDui = 34,
	HU_WangDiao = 35,
	HU_WangDiaoWang = 36,
	HU_WangChuang = 37,
	HU_WangChuangWang = 38,
	HU_WangGuiWei = 39,
	HU_ShuangWangGuiWei = 40,
	HU_SiDaTianWang = 41,
	HU_QiShou4WangBa = 42,
	//湘潭
	HU_MenQingPengpengHu = 43,
	HU_MenQingQingyisePengpengHu = 44,
	HU_QingyisePengpengHu = 45,

	//yiyang
	HU_THREESPECIAL_XIAOQIDUI = 46, //三豪华七小对

	//yueyang chaogu
	HU_XIAODAO = 47,   //小刀
	HU_DADAO = 48,     //大刀
	HU_SILIANGUAN = 49, //四连冠

	////荆门晃晃 软胡牌， 硬胡 点炮胡 杠上开花 海底胡 见字胡 甩牌胡
	//HU_RUANHU = 100, //软胡
	//HU_YINGHU = 101, //硬胡
	//HU_JIANZI = 102, //见字胡
	//HU_SHUAIPAI = 103, //甩牌胡

	HU_RUANHU = HU_WangDiao, //软胡
	HU_YINGHU = HU_WangDiaoWang, //硬胡
	HU_JIANZI = HU_WangDiao, //见字胡
	HU_SHUAIPAI = HU_WangDiao, //甩牌胡
	HU_QUANQIUREN2 = HU_WangChuangWang,			//全球人2

	/////////////////////////广东部分//////////////
	HU_ZIMO = 7,					//自摸
	HU_HAIDIPAO = 11,				//海底炮
	HU_MENQING = 14,				//门清
	HU_GANGSHANGKAIHUA = 8,		//杠上开花
	HU_GANGSHANGPAO = 9,			//杠上炮
	HU_HAIDILAO = 10,				//海底捞
	HU_SANYUANPAIONE = 15,		//一副中发白刻
	HU_SANYUANPAITWO = 16,		//二副中发白刻
	HU_FENGPAIKEQUAN = 17,		//风牌刻与圈相同
	HU_FENGPAIKEWEI = 18,			//风牌刻与位相同

	HU_JIHU = 101,				//鸡胡
	HU_PING = 102,				//平胡
	//HU_PENGPENGHU = 103,		//碰碰胡
	HU_HUNYISE = 104,			//混一色
	HU_BAOHU = 104,
	HU_HUNPENG = 105,			//混碰
	//HU_QINGYISE = 106,			//清一色
	HU_QINGPENG = 107,			//清碰
	HU_HUNYAOJIU = 108,			//混幺九
	HU_XIAOSANYUAN = 109,		//小三元
	HU_XIAOSIXI = 110,			//小四喜
	HU_ZIYISE = 111,			//字一色
	HU_QINGYAOJIU = 112,		//清幺九
	HU_DASANYUAN = 113,			//大三元
	HU_DASIXI = 114,			//大四喜
	HU_SHISANYAO = 115,			//十三幺
	HU_LUXURYQIDUI = 116,		//豪华七对
	HU_QIDUI = 117,				//小七对
	HU_RENHU = 118,				//人胡
	HU_DIHU = 119,				//地胡
	HU_TIANHU = 120,			//天胡
	HU_JIULIANBAODENG = 121,			//九莲宝灯

	HU_FOURGHOST = 122,			//四鬼胡
	HU_YAOJIU = 123,			//幺九
	HU_NOGHOST2TIMES = 124,		//无鬼加番

	HU_FLOWERMATCHSINGLE = 125,	//花吊单
	HU_FLOWERMATCHFLOWER = 126, //花吊花
	HU_GENGZHUANGSUCC = 127, //跟庄成功
	HU_QIANGMINGGANGHU = 128,		//抢明扛胡

	//潮汕新增胡牌类型
	HU_DLUXURYQIDUI = 129,		//双豪华七对
	HU_TLUXURYQIDUI = 130,		//三豪华
	HU_EIGHTEENARHATS = 131,	//十八罗汉

	//惠州庄
	HU_QINGQIDUI = 132,			//清七对
	HU_QINGLUXURYQIDUI = 133,	//清豪华七对
	HU_QINGDLUXURYQIDUI = 134,	//清双豪华七对
	HU_QINGTLUXURYQIDUI = 135,	//清三豪华七对
	HU_TWELVECARDSOUT = 136,	//十二张落地
	HU_ZIMOMENQING = 137,		//自摸+门清
	HU_ZHUANGJIA = 138,		//庄家
	HU_SEVENFLOWER = 139,		//七花胡牌
	HU_EIGHTFLOWER = 140,		//八花胡牌

	HU_MENQINGQINGYAOJIU = 141, //门清清幺九
	HU_MENQINGQINGPENG = 142,	//门清清对
	HU_MENQINGHUNYAOJIU = 143,	//门清混幺九
	HU_MENQINGQINGYISE = 144,	//门清清一色
	HU_MENQINGHUNPENG = 145,	//门清混碰
	HU_WITHOUTHONGZHONG = 146,	//无红中

	//揭阳麻将
	HU_HUNQIDUI = 147,		//混一色七对
	HU_XIAOYAO = 148,		//花幺九
	HU_QINGEIGHTEENARHATS = 151,		//清十八罗汉

	//饶平
	HU_DANDIAO = 152,			//单吊

	//龙门自由庄
	HU_DAGEDA = 153,		//大哥大
	HU_DAJI = 154,		//大鸡
	HU_12LUODIQUANBAO = 155,	//十二张落地全包

};


enum  PLAYTYPE
{
	PT_UNDEFINE = 0,
	PT_ZIMOHU = 1,
	PT_DIANPAOHU = 2,
	PT_KEQIANGGANG = 3,
	PT_NEEDWIND = 4,
	PT_QIXIAODUI = 5,
	PT_ZHUANGXIAN = 6,//庄闲
	PT_ZHANIAO2 = 7,//扎2鸟
	PT_ZHANIAO4 = 8,//扎4鸟
	PT_ZHANIAO6 = 9,//扎6鸟
	PT_TWOPLAYER = 10,//两人
	PT_THREEPLAYER = 11,//三人
	PT_HAVEWIND = 12,//带风
	PT_ZHUANGCANEAT = 13,//可吃
	PT_ZhaNiao_2 = 14,//抓2鸟翻倍
	PT_ZhuangXian_2 = 15,//庄闲翻倍
	PT_XueZhan = 16,//血战玩法
	PT_XueZhan2fan = 17,//血战 2番
	PT_XueZhan4fan = 18,//血战 4番
	PT_QiShouHuCountNiao = 19, //起手胡算鸟
	PT_QiShouHuCountZhuang = 20,//起手胡算庄闲
	PT_EachDaHuCountNiao = 21, //每个大胡单算鸟
	PT_EachDaHuCountZhuang = 22,//每个大胡单算庄闲
	PT_HongZhong2Ma = 23, //红中2码
	PT_HongZhong3Ma = 24,//红中3码
	PT_HongZhong4Ma = 25,//红中4码

	PT_ChenZhouPiao = 26,//郴州 飘
	PT_ChenZhouAuNiao = 27, //郴州 金鸟
	PT_ChangDeHuangZhuangHuangGang = 28,//常德荒庄荒杠
	PT_ChangDeHuangZhuangNoHuangGang = 29,//常德荒庄不荒杠

	PT_ChangDeHuangZhuangQiangGangHuCountMa = 30,//常德抢杠胡算码
	PT_ChangDeHuangZhuangQiangGangHuNoCountMa = 31,//常德抢杠胡算码
	PT_ChangDe159CountMa = 32,//常德159奖码
	PT_ChangDeHuCountMa = 33, //常德摸几奖几
	PT_GangFollowHu = 34,

	//湘潭玩法增加
	PT_ShangWang = 35,
	PT_XiaWang = 36,
	PT_XuanMei01 = 37,
	PT_XuanMei02 = 38,
	PT_XuanMei03 = 39,
	PT_XuanMei04 = 40,
	PT_XuanMei05 = 41,
	PT_XuanMei06 = 42,
	PT_XuanMei07 = 43,
	PT_XuanMei08 = 44,

	//yiyang
	PT_FEINIAO1 = 46,//飞1鸟
	PT_FEINIAO2 = 47,//飞2鸟
	PT_FEINIAO3 = 48,//飞3鸟
	PT_FEINIAO4 = 49,//飞4鸟
	PT_FEINIAO5 = 50,//飞5鸟
	PT_FEINIAO6 = 51,//飞6鸟
	PT_FEINIAO7 = 52,//飞7鸟
	PT_FEINIAO8 = 53,//飞8鸟

	PT_BAOTING = 56,
	PT_MENQING = 57,
	PT_MENQINGPENPENGHU = 58,  //门清将将胡

	PT_SHANGXIAN48 = 60,
	PT_SHANGXIAN96 = 61,

	PT_SILIANGUAN = 65, //四连冠
	
	PT_MINGTING = 100, //明听
	PT_ANTING = 101, //暗听

	////////////////////////////广东选项//////////////////
	PT_ZHANIAO0 = 0,
	//PT_QIXIAODUI = 5,		//可胡七对
	//PT_ZHANIAO2 = 2,
	//PT_ZHANIAO4 = 4,
	//PT_ZHANIAO6 = 6,

	//鬼牌
	PT_GHOSTCARDNO = 7,
	PT_GHOSTCARDBAIBAN = 8,
	PT_GHOSTCARDFLOP = 9,
	PT_REMOVEWINDCARDS = 10,

	//胡牌
	//PT_ZIMOHU = 11,			//自摸
	//PT_DIANPAOHU = 12,		//点炮
	//PT_KEQIANGGANG = 13,	//可抢杠胡
	PT_HONGZHONG = 14,		//红中癞子
	//PT_ZHUANGXIAN = 15,		//庄闲

	PT_QIXIAOFUIDOUBLESCORE = 16,	//七对双倍
	PT_QIANGGANGQUANBAOSCORE = 17,	//抢杠全包
	PT_QIANGGANGHU = 18,			//可抢杠

	PT_NOGHOST2TIMESSCORE = 19,		//无鬼加倍
	PT_GANGSHANGKAIHUAQUANBAO = 20, //杠上开花全包
	PT_FOURGHOSTHU = 21,			//四鬼胡
	PT_FOURGHOSTHU2TIMESSCORE = 22, //四鬼加倍
	PT_DUIDUIHUTYPE = 23,	//对对胡
	PT_QINGYISETYPE = 24,	//清一色
	PT_YAOJIUTYPE = 25,		//幺九
	PT_ALLWINDTYPE = 26,			//全风
	PT_SHISANYAOTYPE = 27,		//十三幺

	PT_GHOSTCARDFLOPTWO = 28,		//翻双鬼
	PT_GHOSTCARDFLOWER = 29,		//花牌做鬼
	PT_MINGGANGKEQIANG = 30,		//明杠可抢杠胡
	PT_YAOJIU1OR9 = 31,				//幺九只含有幺九即可
	PT_LUXURYQIDUI = 32,	//七对四倍
	PT_GENGZHUANGONE = 33,			//跟庄(一分)
	//PT_JIANGMA = 34,				//奖马
	PT_MAGENGDIFEN = 35,			//马跟底分
	PT_HONGZHONGZUOMA = 36,			//红中做马
	PT_HONGZHONGJIAFEN = 37,		//红中加分

	//潮汕
	PT_QIANGGANGHU2TIME = 38,		//抢杠胡两倍
	PT_GANGSHANGKAIHUA2TIME = 39,	//杠上开花两倍
	PT_HUNYISETYPE = 40,			//混一色
	PT_TIDITYPE = 41,				//天胡地胡
	PT_CATCHHORSE = 42,				//抓马
	PT_STEADILYHIGH = 43,			//节节高

	PT_DLUXURYQIDUI = 44,		//双豪华七对
	PT_TLUXURYQIDUI = 45,		//三豪华
	PT_EIGHTEENARHATS = 46,	//十八罗汉
	PT_LITTLEHU = 47,	//鸡胡

	PT_NOWAN = 48,		//不带万
	PT_REDDRAGONASHORSE = 49, //红中当马
	PT_ADDTWOHORSE = 50, //无红中多2马
	PT_GHOSTREDDRAGON = 51, //红中做鬼
	PT_HaidiLao2TimeType = 52, //海底捞月双倍

	PT_HORSEFOLLOWGANG = 53,//马跟杠
	PT_TWELVECARDSOUT = 54,	//十二张落地
	PT_NOFLOWER = 55,		//无花
	PT_TWOFLOWER = 56,		//2花
	PT_FOURFLOWER = 57,		//4花
	PT_SIXFLOWER = 58,		//6花
	PT_EIGHTFLOWER = 59,	//8花

	PT_LITTLESANYUAN = 60,	//小三元
	PT_LITTLESIXI = 61,		//小四喜
	PT_BIGSANYUAN = 62,		//大三元
	PT_BIGSIXI = 63,		//大四喜

	PT_BOMBHORSE = 64,		//爆炸马
	PT_BOMBHORSE_EXTRA_ADD = 65, //爆炸马加分
	PT_BOMBHORSE_EXTRA_MULTIPLE = 66,	//爆炸马多倍
	PT_ZHANIAO8 = 67,	//买8马
	PT_QUANCHONG = 68,	//全冲

	PT_NOFANCANHU = 69, //无番起胡
	PT_ONEFANCANHU = 70,	//一番起胡
	PT_TWOFANCANHU = 71,    //二番起胡
	PT_THREEFANCANHU = 72,	//三番起胡
	PT_FIVEFANCANHU = 73,	//五番起胡

	PT_TOPTOFIVEFAN = 74,	//五番封顶
	PT_TOPTOSEVENFAN = 75,	//七番封顶
	PT_TOPTOTENFAN = 76,	//十番封顶
	PT_TOPTOTHIETEENFAN = 77,//十三番封顶
	PT_GENGZHUANGTWO = 78,	 //跟庄(二分)
	PT_GENGZHUANGTHREE = 79, //跟庄(三分)
	PT_ZHANIAO10 = 80,

	PT_BUYHU0 = 91,			//不买胡
	PT_BUYHU3 = 93,			//买三胡
	PT_BUYHU5 = 95,			//买五胡
	PT_MENGQINGADDSCORE = 100,
	PT_HUWITHOUTHONGZHONG = 101,
	PT_GANGSHANGKAIHUAADDHORSE = 102,
	PT_GANGBOQUANBAO = 103,

	PT_ZHANIAO20 = 105,		//买20马
	PT_FOURFANCANHU = 106,	//四番起胡
	PT_HUNPENG = 107,		//混碰
	PT_HUNQIDUI = 108,		//混一色七对
	PT_XIAOYAO = 109,		//花幺九
	PT_QINGQIDUI = 110,		//清七对
	PT_CUNYAOJIU = 111,		//纯幺九
	PT_QINGPENG = 112,		//清碰
	PT_TENFANTINGCANJIEPAO = 113,	//十倍听牌可接炮
	PT_TENFANTINGFREESCORE = 114,	//十倍听牌免分
	PT_LIUJUGANGSCOREVALID = 115,	//流局杠算分
	PT_MAXCAP = 116,				//无封顶
	PT_MULIT_DIANPAO = 117,		//一炮多响
	PT_JIHU_CANQIANGGANG = 118,	//鸡胡可抢杠
	PT_HAVE_ONENINE_WAN = 119,  //带19万
	PT_JIEXIJIDA = 120,		//揭西鸡大选项
	PT_JIEXIJIHU = 121,			//揭西鸡胡选项
	//潮汕优化
	PT_CHAOSHANBIHU = 122,				//潮汕逼胡选项
	PT_ZHANZHUANGJIAFEN = 123,			//站庄加分选项
	PT_GANGHOUPAO2TIMES = 124,			//杠后炮两倍
	PT_GANGSHANGKAIHUABUQUANBAO = 125,  //杠上开花不全包,潮汕默认是全包
	PT_RAOPINGBIHU = 126,			//饶平必胡

	//begin:饶平玩法特有选项仅用于饶平
	PT_CANNOT_JIHU = 119,	//不可以鸡胡
	PT_CANNOT_JIHU_DIANPAO = 120, //鸡胡不能吃胡
	PT_CATCH_TWO_HORSE = 121,		//抓2马
	PT_CATCH_FOUR_HORSE = 122,		//抓4马
	PT_CATCH_SIX_HORSE = 123,		//抓6马
	PT_GANGHOUPAO2TIMESCORE = 124, //杠后炮2倍
	PT_DANDIAO2TIMESCORE = 125,		//单吊2倍
	//end:饶平玩法特有选项仅用于饶平

	PT_2DIFEN = 127,	//2分底分
	PT_5DIFEN = 128,	//5分底分
	PT_10DIFEN = 129,	//10分底分
	PT_12LUODIQUANBAO = 130,	//十二张落地全包
	PT_NOGENFAN = 131,		//不跟番
	PT_GEN3FAN = 132,		//跟3番
	PT_GEN5FAN = 133,		//跟5番
	PT_GENJIN = 134,		//跟进
	PT_QIANGGANG_ZHONGMA2 = 135,		//抢杠胡中马数翻倍
	PT_HUANGZHUANG_HUAGNGANG = 136,		//荒庄荒杠

	//2-3人玩法，服务器私有类型
	PT_2P = 502,			//2人
	PT_3P = 503,			//3人


};


struct PlayTypeInfo
{
	bool H_Xuezhan;//血战
	bool H_ZhuangXian_2;//庄闲翻倍
	bool H_ZhaNiao_2;//扎鸟翻倍
	bool H_CanEat;//转转可吃
	bool H_HaveWind;//带风
	bool H_ThreePlayer;//3人玩法
	bool H_TwoPlayer;//2人玩法
	Lint I_ZhaNiao;//扎N鸟
	bool H_ZhuangXian;//庄闲
	bool H_Xiaoqidui;//小七对
	bool H_HongZhongLaiZi;//红中癞子
	bool H_DianPao;//点炮胡
	bool H_ZiMo;//自摸
	Lint H_XueZhanFan; //血战番数
	bool H_QiShouHuCountNiao;
	bool H_QiShouHuCountZhuang;
	bool H_EachDaHuCountNiao;
	bool H_EachDaHuCountZhuang;
	bool H_ChenZhouPiao;
	bool H_ChenZhouAuNiao;
	bool H_ChangDeHuangGang;//常德荒杠
	bool H_ChangDeQiangGangHuCountMa;//常德 抢杠胡奖码
	bool H_ChangDeHuCountMa;
	bool H_GangFollowHu;
	bool H_ShangWang;
	bool H_XiaWang;
	Lint I_XuanMei;         //选美数量
	Lint I_FeiNiao;
	bool H_BAOTING;
	bool H_MENQING;
	bool H_MENQINGPENGPENGHU;
	Lint I_ScoreLimit;
	bool H_SILIANGUAN;
	bool H_MINGTING; //明听
	bool H_ANTING; //暗听
	PlayTypeInfo() :H_QiShouHuCountNiao(false), H_QiShouHuCountZhuang(false), H_EachDaHuCountNiao(false), H_EachDaHuCountZhuang(false), H_Xuezhan(false), \
		H_ZhuangXian_2(false), H_ZhaNiao_2(false), H_CanEat(false), H_HaveWind(false), H_ThreePlayer(false), H_TwoPlayer(false), I_ZhaNiao(0), H_ZhuangXian(false), \
		H_Xiaoqidui(false), H_HongZhongLaiZi(false), H_DianPao(false), H_ZiMo(false), H_XueZhanFan(2), H_ChenZhouPiao(false), H_ChenZhouAuNiao(false), \
		H_ChangDeHuangGang(false), H_ChangDeQiangGangHuCountMa(false), H_ChangDeHuCountMa(false), H_GangFollowHu(false), H_ShangWang(false), H_XiaWang(false), \
		I_XuanMei(0), I_FeiNiao(0), H_BAOTING(false), H_MENQING(false), H_MENQINGPENGPENGHU(false), I_ScoreLimit(0), H_SILIANGUAN(false), H_MINGTING(false), H_ANTING(false)
	{
	}
	void clear() {
		H_QiShouHuCountNiao = false;
		H_QiShouHuCountZhuang = false;
		H_EachDaHuCountNiao = false;
		H_EachDaHuCountZhuang = false;
		H_Xuezhan = false;
		H_ZhuangXian_2 = false;
		H_ZhaNiao_2 = false;
		H_CanEat = false;
		H_HaveWind = false;
		H_ThreePlayer = false;
		H_TwoPlayer = false;
		I_ZhaNiao = 0;
		H_ZhuangXian = false;
		H_Xiaoqidui = false;
		H_HongZhongLaiZi = false;
		H_DianPao = false;
		H_ZiMo = false;
		H_XueZhanFan = 2;
		H_ChenZhouPiao = false;
		H_ChenZhouAuNiao = false;
		H_ChangDeHuangGang = false;  //常德 荒庄荒杠
		H_ChangDeQiangGangHuCountMa = false;
		H_ChangDeHuCountMa = false;
		H_GangFollowHu = false;
		H_ShangWang = false;
		H_XiaWang = false;
		I_XuanMei = 0;
		I_FeiNiao = 0;
		H_BAOTING = false;
		H_MENQING = false;
		H_MENQINGPENGPENGHU = false;
		I_ScoreLimit = 0;
		H_SILIANGUAN = false;
		H_MINGTING = false;
		H_ANTING = false;
	}
	void init_playtype_info(std::vector<Lint>& l_play_type)
	{
		for (int x = 0; x<l_play_type.size(); x++)
		{
			switch (l_play_type[x])
			{
			case PT_ZIMOHU:
				H_ZiMo = true;
				break;
			case PT_DIANPAOHU:
				H_DianPao = true;
				break;
			case PT_NEEDWIND:
				H_HongZhongLaiZi = true;
				break;
			case PT_QIXIAODUI:
				H_Xiaoqidui = true;
				break;
			case PT_ZHUANGXIAN:
				H_ZhuangXian = true;
				break;
			case PT_ZHANIAO2:
				I_ZhaNiao = 2;
				break;
			case PT_ZHANIAO4:
				I_ZhaNiao = 4;
				break;
			case PT_ZHANIAO6:
				I_ZhaNiao = 6;
				break;
			case PT_TWOPLAYER:
				H_TwoPlayer = true;
				break;
			case PT_THREEPLAYER:
				H_ThreePlayer = true;
				break;
			case PT_HAVEWIND:
				H_HaveWind = true;
				break;
			case PT_ZHUANGCANEAT:
				H_CanEat = true;
				break;
			case PT_ZhaNiao_2:
			{
				I_ZhaNiao = 2;
				H_ZhaNiao_2 = true;
				break;
			}
			case PT_ZhuangXian_2:
				H_ZhuangXian_2 = true;
				break;
			case PT_XueZhan:
				H_Xuezhan = true;
				break;
			case PT_XueZhan2fan:
				H_XueZhanFan = 2;
				break;
			case PT_XueZhan4fan:
				H_XueZhanFan = 4;
				break;
			case PT_QiShouHuCountNiao:
				H_QiShouHuCountNiao = true;
				break;
			case PT_QiShouHuCountZhuang:
				H_QiShouHuCountZhuang = true;
				break;
			case PT_EachDaHuCountNiao:
				H_EachDaHuCountNiao = true;
				break;
			case PT_EachDaHuCountZhuang:
				H_EachDaHuCountZhuang = true;
				break;
			case PT_HongZhong2Ma:
				I_ZhaNiao = 2;
				break;
			case PT_HongZhong3Ma:
				I_ZhaNiao = 3;
				break;
			case PT_HongZhong4Ma:
				I_ZhaNiao = 4;
				break;
			case PT_ChenZhouAuNiao:
				H_ChenZhouAuNiao = true;
				break;
			case PT_ChenZhouPiao:
				H_ChenZhouPiao = true;
				break;
			case PT_ChangDeHuangZhuangHuangGang:
				H_ChangDeHuangGang = true;
				break;
			case PT_ChangDeHuangZhuangNoHuangGang:
				H_ChangDeHuangGang = false;
				break;
			case PT_ChangDeHuangZhuangQiangGangHuCountMa:
				H_ChangDeQiangGangHuCountMa = true;
				break;
			case PT_ChangDeHuangZhuangQiangGangHuNoCountMa:
				H_ChangDeQiangGangHuCountMa = false;
				break;
			case PT_ChangDeHuCountMa:
				H_ChangDeHuCountMa = true;
				break;
			case PT_GangFollowHu:
				H_GangFollowHu = true;
				break;
			case PT_ShangWang:
				H_ShangWang = true;
				break;
			case PT_XiaWang:
				H_XiaWang = true;
				break;
			case PT_XuanMei01:
				I_XuanMei = 1;
				break;
			case PT_XuanMei02:
				I_XuanMei = 2;
				break;
			case PT_XuanMei03:
				I_XuanMei = 3;
				break;
			case PT_XuanMei04:
				I_XuanMei = 4;
				break;
			case PT_XuanMei05:
				I_XuanMei = 5;
				break;
			case PT_XuanMei06:
				I_XuanMei = 6;
				break;
			case PT_FEINIAO1:
				I_FeiNiao = 1;
				break;
			case PT_FEINIAO2:
				I_FeiNiao = 2;
				break;
			case PT_FEINIAO3:
				I_FeiNiao = 3;
				break;
			case PT_FEINIAO4:
				I_FeiNiao = 4;
				break;
			case PT_FEINIAO5:
				I_FeiNiao = 5;
				break;
			case PT_FEINIAO6:
				I_FeiNiao = 6;
				break;
			case PT_BAOTING:
				H_BAOTING = true;
				break;
			case PT_MENQING:
				H_MENQING = true;
				break;
			case PT_MENQINGPENPENGHU:
				H_MENQINGPENGPENGHU = true;
				break;
			case PT_SHANGXIAN48:
				I_ScoreLimit = 48;
				break;
			case PT_SHANGXIAN96:
				I_ScoreLimit = 96;
				break;
			case PT_SILIANGUAN:
				H_SILIANGUAN = true;
				break;
			case PT_MINGTING:
				H_MINGTING = true;
				break;
			case PT_ANTING:
				H_ANTING = true;
				break;
			}
		}
	}
};

inline bool gIsMatchType(Lint type)
{
	if (type > EM_SIYJ_BEGIN && type < EM_SIYJ_MAX)
	{
		return true;
	}
	return false;
}

//竞技场提示的定义
enum SportTipsType
{
	EM_STT_PRE_MACTH = 1, //预选赛匹配对手 预选赛开赛匹配 m_id 1 m_type 为 1 m_args1 为淘汰名次
	EM_STT_PRE_MACTH_NEXT = 2, //预选赛下一轮等待 m_id 2 m_type为 2 m_args1当前积分 m_args2当前排名 m_args3胜率 m_args4前几名 m_args5 前几名的积分
	EM_STT_PRE_WAIT_OVER = 3, //预选赛等待结束 预选赛最终等待 m_id 3 m_type为 3 m_args1能晋级多少人 m_args2剩余桌子数
	EM_STT_PRE_OUT = 4, //预选赛被淘汰 m_id 4 m_type 为 4 m_args1 为名次 m_args2奖励id1 
	EM_STT_PRE_OK = 5, //预选赛晋级成功 预选赛晋级成功 m_id 5 m_type 为 5 m_args1 为名次
	EM_STT_ADVANCE_MATCH = 6, //晋级比赛等待匹配 晋级赛开赛匹配 m_id 6 m_type为6 m_args1当前积分 m_args2当前排名 m_args3胜率 m_args4前几名 m_args5 前几名的积分
	EM_STT_ADVANCE_WAIT = 7, //晋级等待结果尚未定 晋级赛下一轮等待（等待尚未确定是否晋级） m_id 7 m_type为7 m_args1本桌排名 m_args2等待剩余桌数  m_args3 淘汰人数
	EM_STT_ADVANCE_OK = 8, //晋级成功 晋级赛下一轮匹配（晋级成功）m_id 8 m_type为8 m_args1当前积分 m_args2当前排名 m_args3胜率 m_args4前几名 m_args5 前几名的积分
	EM_STT_ADVANCE_FAIL = 9, //晋级失败 晋级赛被淘汰 m_id 9 m_type为9 m_args1比赛排名 m_args2积分  m_args3奖励id1 
	EM_STT_ADVANCE_WAIT_OK = 10, //晋级等待结果已经晋级  晋级赛最终等待 m_id 10 m_type为10 m_args1剩余桌子数  m_args2 淘汰人数
	EM_STT_ADVANCE_OVER = 11, //晋级结束 晋级赛最终结果 m_id 11 m_type为11 m_args1名次 m_args2奖励id1 m_args3 当前积分
	EM_STT_SPORT_NOT_OPEN = 12, //由于条件不满足，关闭赛场: m_id 12 m_type为 12 
	EM_STT_SPORT_RESET = 13, //时间过长解散房间

	EM_STT_PLAYER_REQ_OUT = 15, //玩家请求退出比赛
	EM_STT_PLAYER_GIVEUP_MATCH = 16, //玩家自动放弃比赛
	EM_STT_CONFUPDATE_GIVEUP = 17, //资源文件更新，报名返还，请重新报名
};

//日志类型的定义
enum EMGameLogicType
{
	EMGameLogicType_DouDiZhuQP = 0, //斗地主棋牌
	EMGameLogicType_ChangShaMJ = 1, //长沙麻将
	EMGameLogicType_GuangDongMJ = 2, //广东麻将
};

inline EMGameLogicType  GetGameLogicTypeByFlag(Lint flag)
{
	//玩法添加到前面
	if (flag >= PK_GAME_STYLE_MAX_CHANGSHA_MAJIANG)
	{
		return EMGameLogicType_GuangDongMJ;
	}
	if (flag >= PK_GAME_STYLE_MAX_DOUDIZHU)
	{
		return EMGameLogicType_ChangShaMJ;
	}
	return EMGameLogicType_DouDiZhuQP;
}



/////////////////////长沙定义 end////////////////////////


/////////////////////广东定义 ///////////////////////////
enum NUMBER
{
	EIGHTEEN = 18,
};

enum  FLOWERSHUNUM
{
	SEVENFLOWER = 7,
	EIGHTFLOWER = 8,
};
////////////////////////////////////////////////////////

#endif