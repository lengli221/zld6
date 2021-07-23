#ifndef __SMCOMMANAGER_H
#define __SMCOMMANAGER_H

#define D_SM_TASKID_LCD       						0//��ʾ��
#define D_SM_TASKID_CARD			  			1//������
#define D_SM_ZY_Charger							2//��Դ���ģ��
#define D_SM_TaskNum            						3//ͨ�������������  

#define D_SM_NewCmd             						0x0001//����ָ����Ϣ       
#define D_SM_NoCmd              							0x0000//û����ָ����Ϣ    

#define D_SM_CycleCmd           						0x0000//ѭ��ָ����Ϣ      
#define D_SM_InitiativeCmd      						0x0001//����ָ����Ϣ    

#define D_ComType_COM            				 		0x0001//�������ͺ� 
#define D_ComType_CAN             						0x0002 //CAN�����ͺ�   

#define D_RCV_CNT               							100

/*�뿨ͨ������*/
/*MT_CardVersion == MT622V204*/
#define D_MT318_CMD_FindCar					0x0001//Ѱ��ָ����Ϣ	    		   
#define D_MT318_CMD_Pretereatment    			0x0002//Ԥ����ָ����Ϣ			   
#define D_MT318_CMD_StartCharge				0x0004//��ʼ�ӵ�ָ����Ϣ			    
#define D_MT318_CMD_EndCharge				0x0008//�����ӵ�ָ����Ϣ			    
#define D_MT318_CMD_AddDeal              			0x0010//����ӵ�ָ����Ϣ	
/*MT_CardVersion == MT_Card_M1*/
#define D_M1_CMD_FindCar							0x0020
#define D_M1_CMD_VerifyKeyA						0x0040
#define D_M1_CMD_VerifyKeyB						0x0080
#define D_M1_CMD_ReadData						0x0100
#define D_M1_CMD_WriteData						0x0200
#define D_M1_CMD_Impairment					0x0400
#if Modify_Key_190826 == 1
#define D_M1_CMD_Modify_key					0x0800//�޸���Կ
#endif


/*����ʾ��ͨ������*/
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

#define D_LCD_CMD_WriteVoiceInfo			0x2F/*������Ϣ*/

/*ͨ���߼�����Ԫ*/
typedef struct __SM_LogicUnit__
{
	uint8   ucCmdType;//�������� �Ƿ�ѭ�� 
	uint32  ucCmdCode;//������
	void    *pFrame;//����֡  
	bool   (*PackageFrame)( void *,  void *,uint16 *);//����ص�
	bool   (*ParseFrame)(void *,  void *,uint16);//����ص�
	bool   (*SetDataCmd)( void *,void *);//�������ݻص�(֡����)������(*ParseFrame)��������
	bool   (*GetDataCmd)( void *,uint8);//��ȡ���ݻص���֡���� �豸��)������(*PackageFrame)�������
	void    *pDevAddrs;//�ӻ���ַ
	uint8    *ucDevNum;
	uint32  *ucComState;//ͨ��״̬ (֧��32�����豸) 	ͨ���쳣��1
	uint16    *ComFailCnt;//ͨѶʧ����
	uint32  *ucComFlag;//ͨѶ����״̬	ͨ���쳣��1
}SM_LogicUnit;

/*ͨ���������*/
typedef struct __SM_TaskPara__
{
	uint8 ucComType;//ͨ�Ŷ˿�����
	uint8 ucComID;//ͨ�Ŷ˿ں�
	SM_LogicUnit *pst_LogicTable;//ͨ���߼���
	uint16 ucTableLen;//ͨ���߼���Ԫ��
	uint32 iParam[3];//���� 1Ϊ�Ƿ���ָ�� 2Ϊָ������ 3Ϊ���豸��
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

