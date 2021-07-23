#ifndef OS_APP_CFG_H_
#define OS_APP_CFG_H_

/*
** 调试信息
*/
#define Debug_Info							1
#define Debug_SinglePoint					0/*单点调试*/

/*
** 海南恒晟 屏蔽烟感,屏蔽水位
*/
#define HengSheng_MaskSmokeWater	1

/*
** Test Fun Module
*/
#define Test_FunModule_ZY					0/*测试振源充电器模块*/

#define SysSoftCtrVersion					206/*恒晟软件版本号以100为起始-- 比耐以200起始*/

#define MT622V204							0//CPU--铭特--Pin卡
#define MT_Card_M1							1//M1卡--量必达科技
#define MT_CardVersion						MT_Card_M1

#define DeviceAddr_MAX						(uint8)0x07

#define ALLDOORNUM							(uint8)0x0C

#define EEPROMDATA_REPAIRDAT_EN   	1//备份使能

#define Modify_PW_190822					1//修改密码
#define MainPageVer_190823				1//主页面软件版本号
#define Modify_Key_190826					1//修改密钥

/*任务运行优先级*/
#define D_LogicCtr_TASK_PRO				(uint8)0x04
#define D_ZY_Charger_Task_PRO			(uint8)0x05
#define D_CARD_TASK_PRO					(uint8)0x07        	
#define D_LCD_TASK_PRO					(uint8)0x08
#define D_HMI_TASK_PRO					(uint8)0x0C

#endif

