#include "includes.h"

extern LCD_CTRLFRAME LCD_CtrlAddrSheets[];
extern uint8 const  LCD_CtrlAddrSheetsNum;
extern LCD_DATAFRAME LCD_DataAddrSheets[];
extern uint16 const LCD_DataAddrSheetsNum;

bool LCD_SetData(void *pData,void *temp)
{
	uint16 i;
	uint8 j;
	uint16 Vual=0 ;
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if (st_FramePara == null) 
		return false;
	if((st_FramePara->ucFunCode == D_DP_CtrlRead)||(st_FramePara->ucFunCode == D_DP_CtrlWrite))
	{
		for (i = 0; i < LCD_CtrlAddrSheetsNum; i++)
		{
			if (st_FramePara->iAddress == LCD_CtrlAddrSheets[i].addr)
			{
				if ((LCD_CtrlAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for (j = 0; j < (st_FramePara->ucRegLen); j++)
				{
					*(LCD_CtrlAddrSheets[i + j].pData) = st_FramePara->ucDataSpace[j];
				}
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < LCD_DataAddrSheetsNum; i++)
		{
			if (st_FramePara->iAddress == LCD_DataAddrSheets[i].addr)
			{
				if ((LCD_DataAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for (j = 0; j < (st_FramePara->ucRegLen); j++)
				{
					Vual=st_FramePara->ucDataSpace[j * 2];
					Vual=Vual<<8;
					Vual=Vual+st_FramePara->ucDataSpace[j *2+1];
					*(LCD_DataAddrSheets[i + j].pData)=Vual;
				}
				break;
			}
		}
	}
	return true;
}

bool LCD_GetData(void *pData)
{
	uint16 i; 
	uint8 j;
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if (pData == null) 
		return false;
	
	if((st_FramePara->ucFunCode == D_DP_CtrlRead)||(st_FramePara->ucFunCode == D_DP_CtrlWrite))	
	{
		for (i = 0; i < LCD_CtrlAddrSheetsNum; i++)
		{
			if (st_FramePara->iAddress == LCD_CtrlAddrSheets[i].addr)
			{
				if ((LCD_CtrlAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for (j = 0; j < (st_FramePara->ucRegLen); j++)
				{
					st_FramePara->ucDataSpace[j] = *(LCD_CtrlAddrSheets[i + j].pData) & 0xff;
				}
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < LCD_DataAddrSheetsNum; i++)
		{
			if (st_FramePara->iAddress == LCD_DataAddrSheets[i].addr)
			{	
				if ((LCD_DataAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for (j = 0; j < (st_FramePara->ucRegLen); j++)
				{
					st_FramePara->ucDataSpace[j * 2] = *(LCD_DataAddrSheets[i + j].pData) >> 8;
					st_FramePara->ucDataSpace[j * 2 + 1] = *(LCD_DataAddrSheets[i + j].pData) & 0xff;
				}
				break;
			}
		}
	}
    return true;
}

