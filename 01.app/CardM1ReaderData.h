#ifndef __CARDM1READERDATA_H
#define __CARDM1READERDATA_H
#if MT_CardVersion == MT_Card_M1

#define P_M1_FIND							0x3430//寻射频卡
#define P_M1_KEYA							0x3432//验证Key_A密钥
#define P_M1_KEYB							0x3439//验证Key_B密钥
#define P_M1_READ							0x3433//读扇区块数据
#define P_M1_WRITE							0x3434//写扇区块数据
#define P_M1_IMPAIRMENT					0x3438//减值操作

#if Modify_Key_190826 == 1
#define P_M1_MODIFYKEY					0x3435//修改密钥
#endif

#define P_M1_ACK_OK						0x59//操作成功
#define P_M1_ACK_FAIL						0x4E//操作失败
#if Modify_Key_190826 == 1
#define P_M1_ACK_PWFail					0x33//密码错误
#endif

#define M1_COM_STATE_PWERR			0x01//密码错误
#define M1_COM_STATE_ACKERR			0x02//不可识别的响应码
#define M1_COM_STATE_STATER			0x03//操作状态与实际不符
#define M1_COM_STATE_DONE				0x04//读卡操作正常完成
#if Modify_Key_190826 == 1
#define M1_COM_ModifyCardKey_Ok		0x05//修改卡密钥
#endif

typedef struct
{
	uint8 sector;
	uint8 block;
	uint8 bdata[16];
}M1_BLOCKDATA;

typedef struct
{
	M1_BLOCKDATA block[3];
	uint32 u32balance;
	uint8 lock;
	uint32 consume;
}M1_CARDDATA;

typedef struct
{
	uint8 state;
	uint16 cmd;
	uint8 result;
	uint8 sector;//扇区号
	uint8 block;//块号
	uint8 bdata[16];
}M1_COMDATA;

bool M1_FindCard(void * pData,uint8 dev);
bool M1_FindCardResult(void *pData,void *temp);
bool M1_VerifyKeyA(void * pData,uint8 dev);
bool M1_VerifyKeyB(void * pData,uint8 dev);
bool M1_VerifyResult(void *pData,void *temp);
bool M1_ReadBlockData(void *pData,uint8 dev);
bool M1_ReadResult(void *pData, void *temp);
bool M1_ImpairmentOpt(void * pData,uint8 dev);
bool M1_ImpairmentOptResult(void *pData, void *temp);
/******************************************************************************************/
bool MT_GetRecResult(void);
void MT_ClearRecResult(void);
void M1_SetBlockReq(uint8 sector,uint8 block);
void M1_SetCardData(uint8 idx);
void M1_GetM1CardData(M1_CARDDATA* p_stM1Card);
void M1_ClearM1CardData(void);
uint8 M1_CardReadCom(uint8 state);
uint8 M1_CardImpairmentCom(uint8 state);
bool M1_ModifyKeyOpt(void * pData,uint8 dev);
#if Modify_Key_190826 == 1
void M1_UpdateKeyData(uint8* data,uint8 sector);
void M1_UpdateCardKeyData(uint8* data,uint8 sector);
M1_COMDATA Get_M1_ComData(void);
#endif
#endif
#endif

