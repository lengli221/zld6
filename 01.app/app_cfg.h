#ifndef OS_APP_CFG_H_
#define OS_APP_CFG_H_

/*
** ������Ϣ
*/
#define Debug_Info							1
#define Debug_SinglePoint					0/*�������*/

/*
** ���Ϻ��� �����̸�,����ˮλ
*/
#define HengSheng_MaskSmokeWater	1

/*
** Test Fun Module
*/
#define Test_FunModule_ZY					0/*������Դ�����ģ��*/

#define SysSoftCtrVersion					206/*��������汾����100Ϊ��ʼ-- ������200��ʼ*/

#define MT622V204							0//CPU--����--Pin��
#define MT_Card_M1							1//M1��--���ش�Ƽ�
#define MT_CardVersion						MT_Card_M1

#define DeviceAddr_MAX						(uint8)0x07

#define ALLDOORNUM							(uint8)0x0C

#define EEPROMDATA_REPAIRDAT_EN   	1//����ʹ��

#define Modify_PW_190822					1//�޸�����
#define MainPageVer_190823				1//��ҳ������汾��
#define Modify_Key_190826					1//�޸���Կ

/*�����������ȼ�*/
#define D_LogicCtr_TASK_PRO				(uint8)0x04
#define D_ZY_Charger_Task_PRO			(uint8)0x05
#define D_CARD_TASK_PRO					(uint8)0x07        	
#define D_LCD_TASK_PRO					(uint8)0x08
#define D_HMI_TASK_PRO					(uint8)0x0C

#endif

