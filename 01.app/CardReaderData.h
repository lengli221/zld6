#ifndef CARDREADERDATA_H
#define CARDREADERDATA_H
#if MT_CardVersion == MT622V204
#define S1		           					0x30					//寻卡返回值是有卡
#define S2		           					0x30					//寻卡返回值是系统卡

enum
{
	NoCardExiting = 0x00,
	CardExiting = 0x01
};

//卡数据 
typedef struct __MT_CardData__
{
	uint8 byCardNo[8];//卡号
	uint16 iCardState;//卡状态
	uint8 byBalance[4];//卡中余额
	uint8 ucCardType;//卡类型
	uint32 iDealCounts;	//购电次数
	uint8 byPseudoRandomNum[4];//伪随机数
	uint8 byOnlineSerial[2];//联机交易序号
	uint8 byLeftMoney[4];//电子钱包余额
	uint8 byDealMoney[2];//本次交易余额
	uint8 ucType;//交易类型标志
	uint8 byEterm[6];//桩体号
	Common_Time st_CommTime;//时间数据
	uint8 bytserial[2];//脱机交易序号
} MT_CardData;

//读卡器数据
typedef struct __MT_Data__
{
    uint16 iRcvResult;//解析卡回码的结果
    MT_CardData st_CardData;//卡数据
    uint8 ucReState;//状态字
    uint8 ucPiceChangeTime[5];//备用费率电价单价切换时间
    uint8 ucPresentDealPice1[3];//当前交易费率1电量单价
    uint8 ucPresentDealPice2[3];//当前交易费率2电量单价
    uint8 ucPresentDealPice3[3];//当前交易费率3电量单价
    uint8 ucPresentDealPice4[3];//当前交易费率4电量单价
    uint8 ucPresentDealPice5[3];//当前交易费率5电量单价
    uint8 ucPresentDealPice6[3];//当前交易费率6电量单价
    uint8 ucPresentDealPice7[3];//当前交易费率7电量单价
    uint8 ucPresentDealPice8[3];//当前交易费率8电量单价
    uint8 ucReserveDealPice1[3];//备用交易费率1电量单价
    uint8 ucReserveDealPice2[3];//备用交易费率2电量单价
    uint8 ucReserveDealPice3[3];//备用交易费率3电量单价
    uint8 ucReserveDealPice4[3];//备用交易费率4电量单价
    uint8 ucReserveDealPice5[3];//备用交易费率5电量单价
    uint8 ucReserveDealPice6[3];//备用交易费率6电量单价
    uint8 ucReserveDealPice7[3];//备用交易费率7电量单价
    uint8 ucReserveDealPice8[3];//备用交易费率8电量单价
    uint8  ucPraseErr;//解析失败
} MT_Data;

//设置读卡器数据
typedef struct __MT_GData__
{
	uint32 iConsume;//消费金额
	Common_Time st_CommTime;//时间数据
	uint32 iElectric ;//耗电量
} MT_GData;
bool MT_FindCardGetData(void * pData,uint8 dev);
bool MT_FindCardSetData(void *pData,void * temp);
bool MT_PreStartGetData(void *pData,uint8 dev);
bool MT_PreSetData(void *pData,void * temp);
bool MT_StartGetData(void * pData,uint8 dev);
bool MT_EndGetData(void *pData,uint8 dev);
bool MT_StartEndChargeSetData(void *pData,void * temp);

/**********************************************************************************/
void MT_SetCardExitingFlag(uint8 flag);
uint8 MT_GetCardExitingFlag(void);
bool MT_GetRecResult(void);
bool MT_ClearRecResult(void);
bool MT_GetCardLock(void);
bool MT_ReadCardRemain(uint32 *remain);
bool MT_SetCardConsume(uint32 consume);
#endif

#endif

