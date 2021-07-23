#include "includes.h"

#if Test_FunModule_ZY == 1
static const uint8 chargerTestAddr = 0x02;/*充电器测试地址*/

extern uint16 modBus_CRC16(uint8* data,uint8 size);
void test_ZY_Charger_CRC(void){
	uint8 test_Crc[8] = {0x02,0x04,0x01,0xF4,0x00,0x06,0x00,0x00};
	volatile uint16 crc16 = 0;

	crc16 = modBus_CRC16((uint8*)&test_Crc[0],6);
}

/*
** test ZY Charger Flow
*/
void test_ZY_Charge_Flow(void){
	static uint8 step = 0;
	static uint32 itick = 0;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_VoltCur,0};
	ParamHS paramHS_Temp = get_HSVolCurLimit();

	switch(step){
		case 0:
			TickOut((uint32 *)&itick, 0);
			step++;
			break;
		case 1:/*设置模块电压电流*/
			if(TickOut((uint32 *)&itick, 3000) == true){
				TickOut((uint32 *)&itick, 0);
				set_ZY_Charger_VolCur(paramHS_Temp.volLimit, paramHS_Temp.curLimit, chargerTestAddr);
				iParam[1] = D_ZY_CMD_Set_VoltCur;
				iParam[2] = chargerTestAddr;
				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
				step++;
			}
			break;
		case 2:/*设置开机*/
			if(TickOut((uint32 *)&itick, 3000) == true){
				TickOut((uint32 *)&itick, 0);
				set_ZY_ChargerOnOff(true,chargerTestAddr);
				iParam[1] = D_ZY_CMD_Set_OnOff;
				iParam[2] = chargerTestAddr;
				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
				step++;
			}			
			break;
		case 3:/*设置关机*/
			if(TickOut((uint32 *)&itick, 30000) == true){
				TickOut((uint32 *)&itick, 0);
				set_ZY_ChargerOnOff(false,chargerTestAddr);
				iParam[1] = D_ZY_CMD_Set_OnOff;
				iParam[2] = chargerTestAddr;
				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
				step = 0;
			}
			break;
	}
}

/*
** Test ZY
*/
void test_ZY(void){
	test_ZY_Charger_CRC();
	/*
	** test ZY Charger Flow
	*/
	test_ZY_Charge_Flow();
}
#endif

