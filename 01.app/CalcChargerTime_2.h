#ifndef __CALCCHARGERTIME_H
#define __CALCCHARGERTIME_H

typedef struct
{
	int32 min[ALLDOORNUM];
	int32 lastMin[ALLDOORNUM];
	uint32 itick[ALLDOORNUM];
}ChargerTime;

ChargerTime *GetChargerTime(void);
void CalcChargerIntervalTime(void);
void chargerTimeWriteBackupDat(void);
void chargerRecordReadBackupData(void);
void ChargerRecordClearBackupDat(void);

#endif

