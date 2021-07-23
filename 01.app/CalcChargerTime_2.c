#include "includes.h"

ChargerTime chargerTime;

ChargerTime *GetChargerTime(void)
{
	return(&chargerTime);
}

void CalcChargerIntervalTime(void)
{
	uint8 i=0;
	static uint32 ctick = 0;
	Common_Time curTime = {0};
	DoorSysParam *cDoorSysParam = GetDoorSysParam();

	if(TickOut((uint32 *)&ctick, 1000))
	{
		TickOut((uint32 *)&ctick, 0x00);
		for(i=0;i<ALLDOORNUM;i++)
		{
			if(cDoorSysParam->doorState[i] == BatCharging || cDoorSysParam->doorState[i] == BatFull)
			{
				GetCurrentTime((Common_Time *)&curTime);
				chargerTime.min[i] = (TimeToMins(curTime) - TimeToMins(cDoorSysParam->time[i])+chargerTime.lastMin[i]);
			}
		}
	}
}

void chargerTimeBackupDatWrite(uint8 * chgTim)
{
	memcpy((uint8*)&chgTim[0],(uint8*)&chargerTime.min[0],sizeof(ChargerTime));
}

void ChargerTimeRepairWrite(uint8 * chgTim)
{
	memcpy((uint8*)&chargerTime.lastMin[0],(uint8*)&chgTim[0],sizeof(ChargerTime));
}

void chargerTimeWriteBackupDat(void)
{
	static uint32 ctick = 0;
	uint8 chargeBackupDat[Backup_ChargerTimeAddr_Len];
	if(DoorSysGetUsingBatNum()>=1)
	{
		if(TickOut((uint32 *)&ctick, 3000))
		{
			TickOut((uint32 *)&ctick, 0x00);
			chargerTimeBackupDatWrite((uint8 *)&chargeBackupDat[2]);
			chargeBackupDat[0] = 0xA5;
			chargeBackupDat[1] = 0xAA;
			BKP_WriteBackupDat(Backup_ChargerTimeAddr, (uint8_t *)&chargeBackupDat[0], Backup_ChargerTimeAddr_Len);
		}
	}
}

static void delay(uint16 dly) {while(dly > 0) { dly--;} };
static void delayMS(uint16 dly) {while(dly > 0) { dly--;delay(5000);watchdogUpdate();} };
void chargerRecordReadBackupData(void)
{
	uint8 chargeRBackupDat[Backup_ChargerTimeAddr_Len];
	delayMS(25);
	BKP_ReadBackupRegDat(Backup_ChargerTimeAddr, (uint8_t *)&chargeRBackupDat[0], Backup_ChargerTimeAddr_Len);
	if(chargeRBackupDat[1] == 0xAA)
	{
		delayMS(25);
		ChargerTimeRepairWrite((uint8 *)&chargeRBackupDat[2]);
		delayMS(25);
	}
}

void ChargerRecordClearBackupDat(void)
{
	uint8 chargeBackupDat[10];

	chargeBackupDat[0] = 0x00;
	chargeBackupDat[1] = 0x00;
	BKP_WriteBackupDat(Backup_ChargerTimeAddr,(uint8 *)&chargeBackupDat[0],2);	
}

