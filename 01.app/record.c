#include "includes.h"

uint8 FlashWrite_lock;
uint8 FlashRead_lock;
static uint16 recordNo;

Charge_Record charge_Record[ALLDOORNUM] = {0};

uint16 ChargeGetRecordNo(void)
{
	if(recordNo > RECORD_NO_MAX)
	{
		return RECORD_NO_MAX;
	}
	return recordNo;
}

void chargeClearRecordNo(void)
{
	recordNo = 0;
}

 void ChargeRecordNoWrite(uint16 no)
{
	uint8 buf[3];
	buf[0] = no >> 8;
	buf[1] = no & 0xff;
	buf[2] = calcSum(buf,ELEN_RECORDNO-1);
	eeprom_write(EADDR_RECORDNO,buf,ELEN_RECORDNO);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_RECORDNO, buf, ELEN_RECORDNO);
	#endif
}

void ChargeRecordNoRead(void)
{
	uint8 buf[3],buf_bkp[3];
	eeprom_read(EADDR_RECORDNO,buf,ELEN_RECORDNO);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_RECORDNO, buf_bkp, ELEN_RECORDNO);
	#endif
	if(calcSum(buf,ELEN_RECORDNO-1) == buf[2])
	{
		recordNo = (((uint16)buf[0])<<8) + buf[1];
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_RECORDNO, buf, ELEN_RECORDNO);
		#endif
	}
	else if(calcSum(buf_bkp,ELEN_RECORDNO-1) == buf_bkp[2])
	{
		recordNo = (((uint16)buf_bkp[0])<<8) + buf_bkp[1];
		eeprom_write(EADDR_RECORDNO,buf_bkp,ELEN_RECORDNO);
	}
	else
	{
		recordNo = 0;
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = calcSum(buf,ELEN_RECORDNO-1);
		eeprom_write(EADDR_RECORDNO,buf,ELEN_RECORDNO);
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_RECORDNO, buf, ELEN_RECORDNO);
		#endif
	}
}

void ChargeRecordWritePreData(uint8 proc,uint8 state,uint16 consumeWay)
{
	charge_Record[proc].flashFlag = RECORD_FLASH_FLAG;
	charge_Record[proc].state = state;
	GetCurrentTime((Common_Time *)&charge_Record[proc].startTime);
	charge_Record[proc].consumeWay = consumeWay;
}

void ChargeRecordWrite(uint8 proc,uint8 state,uint8 *cardNo,uint8 doorId)
{
	charge_Record[proc].state = state;
	charge_Record[proc].doorId = doorId;
	GetCurrentTime((Common_Time *)&charge_Record[proc].stopTime);
	memcpy((uint8*)&charge_Record[proc].cardNo[0],(uint8*)cardNo,sizeof(charge_Record[proc].cardNo));
	charge_Record[proc].sum = calcSum((uint8 *)&charge_Record[proc].flashFlag, &charge_Record[proc].sum - &charge_Record[proc].flashFlag);

	while(FlashWrite_lock==1) 
	{
   		Sleep(3);
  	}
	FlashWrite_lock=1; 
	SPI_flash_writeData(FADDR_RECORD_BASE+((recordNo%RECORD_NO_MAX)*FLEN_RECORD), (uint8*)&charge_Record[proc].flashFlag, sizeof(Charge_Record));
	FlashWrite_lock=0; 
	recordNo++;
	if(recordNo >= (RECORD_NO_MAX<<1))
	{
		recordNo -= RECORD_NO_MAX;
	}
	ChargeRecordNoWrite(recordNo);

	memset((uint8*)&charge_Record[proc].flashFlag,0x00,sizeof(Charge_Record));
}

bool ChargeRecordRead(uint16 no,Charge_Record *record)
{
	if((no < recordNo)&&(no < RECORD_NO_MAX))
	{
		no = recordNo - no - 1;	
		while(FlashRead_lock==1)
		{
		 	Sleep(3);
		}
		FlashRead_lock=1;
		SPI_flash_read(FADDR_RECORD_BASE + ((no%RECORD_NO_MAX)*FLEN_RECORD), (uint8 *)&record->flashFlag, sizeof(Charge_Record));
		FlashRead_lock=0;
		if(record->flashFlag == RECORD_FLASH_FLAG && record->sum == calcSum((uint8 *)&record->flashFlag, &record->sum-&record->flashFlag/*sizeof(Charge_Record)-1*/))
		{
			return(true);
		}
	}
	memset((uint8*)&record->flashFlag,0x00,sizeof(Charge_Record));
	return(false);
}

