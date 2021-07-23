#include "includes.h"

extern HMI_DW_PAGE thispage;
extern HMI_DW_PAGE thislastpage;
extern const HMI_DW_PAGE hmi_DW_PageTable[];
extern uint8 hmi_DW_PageTableNum;
extern HMI_MultiDeepPage hmiMulitDeepPage;

extern void HMI_DW_SwipingPage(void);
extern void HMI_DW_TaskingPage(void);
extern void HMI_DW_NoSwOpt(void);

uint8 HMI_GetMultiDeepPageId(void)
{
	return(hmiMulitDeepPage.id[0]);
}

void HMI_DW_PageDataTableFun(void)
{
	uint8 pageId = 0;
	uint8 i=0;

	if(LCD_GetScreenID() != thispage.id)
	{
		pageId = LCD_GetScreenID();
		for(i=0;i<hmi_DW_PageTableNum;i++)
		{
			if((pageId == hmi_DW_PageTable[i].id)&&(pageId != 0xFF))
			{
				memcpy((uint8*)&thislastpage.id,(uint8*)&thispage.id,sizeof(thispage));
				memcpy((uint8*)&thispage.id,(uint8*)&hmi_DW_PageTable[i].id,sizeof(thispage));
				if(thispage.handle == HMI_DW_SwipingPage || thispage.handle == HMI_DW_TaskingPage || thispage.handle == HMI_DW_NoSwOpt
				)
				{
					hmiMulitDeepPage.id[hmiMulitDeepPage.deepCnt] = thislastpage.id;	
					hmiMulitDeepPage.handle[hmiMulitDeepPage.deepCnt] = thislastpage.handle;
					hmiMulitDeepPage.deepCnt++;
				}
				else
				{
					hmiMulitDeepPage.deepCnt = 0;
					hmiMulitDeepPage.id[0] = thislastpage.id;	
					hmiMulitDeepPage.handle[0] = thislastpage.handle;
				}
				break;
			}
		}
		thispage.id = pageId;
	}
	thispage.handle();

}

