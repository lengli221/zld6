#ifndef __SMCOMMANAGER_H
#define __SMCOMMANAGER_H

#define D_SM_TASKID_LCD       						0//显示屏
#define D_SM_TASKID_CARD			  			1//读卡器
#define D_SM_ZY_Charger							2//振源充电模块
#define D_SM_TaskNum            						3//通信任务最大数量  

#define D_SM_NewCmd             						0x0001//有新指令消息       
#define D_SM_NoCmd              							0x0000//没有新指令消息    

#define D_SM_CycleCmd           						0x0000//循环指令信息      
#define D_SM_InitiativeCmd      						0x0001//主动指令信息    

#define D_ComType_COM            				 		0x0001//串口类型号 
#define D_ComType_CAN             						0x0002 //CAN口类型号   

#define D_RCV_CNT               							100

/*与卡通信命令*/
/*MT_CardVersion == MT622V204*/
#define D_MT318_CMD_FindCar					0x0001//寻卡指令信息	    		   
#define D_MT318_CMD_Pretereatment    			0x0002//预处理指令信息			   
#define D_MT318_CMD_StartCharge				0x0004//开始加电指令信息			    
#define D_MT318_CMD_EndCharge				0x0008//结束加电指令信息			    
#define D_MT318_CMD_AddDeal              			0x0010//补充加电指令信息	
/*MT_CardVersion == MT_Card_M1*/
#define D_M1_CMD_FindCar							0x0020
#define D_M1_CMD_VerifyKeyA						0x0040
#define D_M1_CMD_VerifyKeyB						0x0080
#define D_M1_CMD_ReadData						0x0100
#define D_M1_CMD_WriteData						0x0200
#define D_M1_CMD_Impairment					0x0400
#if Modify_Key_190826 == 1
#define D_M1_CMD_Modify_key					0x0800//修改密钥
#endif


/*与显示屏通信命令*/
#define D_LCD_CMD_JumpScreen					0x00
#define D_LCD_CMD_ReadScreenId				0x01
#define D_LCD_CMD_WriteTime					0x02
#define D_LCD_CMD_WriteStateIcon				0x03
#define D_LCD_CMD_WriteStateNum				0x04
#define D_LCD_CMD_WriteAdminPwShow		0x05
#define D_LCD_CMD_WriteAdminPwAscii			0x06
#define D_LCD_CMD_ReadAdminPwAscii			0x07
#define D_LCD_CMD_WriteAdminPwIcon			0x08
#define D_LCD_CMD_WriteSysParamTime		0x09
#define D_LCD_CMD_ReadSysParamTime		0x0A
#define D_LCD_CMD_WriteConsumeWay			0x0B
#define D_LCD_CMD_ReadConsumeWay			0x0C
#define D_LCD_CMD_WriteConsumeIcon			0x0D
#define D_LCD_CMD_WriteSingleConsume		0x0E
#define D_LCD_CMD_ReadSingleConsume		0x0F
#define D_LCD_CMD_WriteFullSetTime			0x10
#define D_LCD_CMD_ReadFullSetTime			0x11
#define D_LCD_CMD_WriteRecord					0x12
#define D_LCD_CMD_WriteRecordState			0x13
#define D_LCD_CMD_ReadRecordState			0x14
#define D_LCD_CMD_WriteClearRecordBtn		0x15
#define D_LCD_CMD_ReadClearRecordBtn		0x16
#define D_LCD_CMD_ReadOneBtnOpenDoor		0x17
#define D_LCD_CMD_WriteOneBtnOpenDoor	0x18
#define D_LCD_CMD_ReadAppointDoorId		0x19
#define D_LCD_CMD_WriteAppointDoorId		0x1A
#define D_LCD_CMD_WriteSoftVersion			0x1B

#if Modify_PW_190822 == 1
#define D_LCD_CMD_ReadModifyPWAscii			0x26
#define D_LCD_CMD_WriteModifyPWAscii			0x27	
#define D_LCD_CMD_WriteModifyPWShow		0x28
#endif

#if Modify_Key_190826 == 1
#define D_LCD_CMD_ReadModifyKey 				0x29
#define D_LCD_CMD_WriteModifyKey				0x2A
#define D_LCD_CMD_ReadClearStepBtn			0x2B
#define D_LCD_CMD_WriteClearStepBtn			0x2C
#define D_LCD_CMD_WriteKeyModeChoice		0x2D
#endif

#define D_LCD_CMD_WriteVoiceInfo			0x2F/*语音信息*/

/*通信逻辑处理单元*/
typedef struct __SM_LogicUnit__
{
	uint8   ucCmdType;//命令类型 是否循环 
	uint32  ucCmdCode;//命令编号
	void    *pFrame;//组解包帧  
	bool   (*PackageFrame)( void *,  void *,uint16 *);//组包回调
	bool   (*ParseFrame)(void *,  void *,uint16);//解包回调
	bool   (*SetDataCmd)( void *,void *);//设置数据回调(帧数据)服务与(*ParseFrame)解析函数
	bool   (*GetDataCmd)( void *,uint8);//获取数据回调（帧数据 设备号)服务与(*PackageFrame)打包函数
	void    *pDevAddrs;//从机地址
	uint8    *ucDevNum;
	uint32  *ucComState;//通信状态 (支持32个从设备) 	通信异常置1
	uint16    *ComFailCnt;//通讯失败数
	uint32  *ucComFlag;//通讯最终状态	通信异常置1
}SM_LogicUnit;

/*通信任务参数*/
typedef struct __SM_TaskPara__
{
	uint8 ucComType;//通信端口类型
	uint8 ucComID;//通信端口号
	SM_LogicUnit *pst_LogicTable;//通信逻辑表
	uint16 ucTableLen;//通信逻辑表单元数
	uint32 iParam[3];//参数 1为是否新指令 2为指令内容 3为从设备号
	int16 cmdNum;
} SM_TaskPara;

bool SM_SetCmd(uint8 ucSmNo,uint32* piParam);
bool SM_GetCmd(uint8 ucSmNo,uint32* piParam);
void SM_DisposeSmCom(void *pPara);
void SM_InitSmComManagement(void);
uint32 SM_GetCardComState(void);
/*
** get Charger Com State 
*/
uint32 get_ChargerComState(void);
#endif

