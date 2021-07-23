#include "includes.h"
extern ProcessSysCtr processSysCtr[ALLDOORNUM];
extern ProcessSysCtr thisProcessSysCtr;
extern void DoorLogicCtr(uint8 proc);

void DoorSysProcessCtrFun(void)
{
	uint8 proc = 0;

	for(proc=0;proc<ALLDOORNUM;proc++)
	{
		if(processSysCtr[proc].flag)
		{
			memcpy((uint8*)&thisProcessSysCtr.flag,(uint8*)&processSysCtr[proc].flag,sizeof(ProcessSysCtr));
			DoorLogicCtr(thisProcessSysCtr.process);
			memcpy((uint8*)&processSysCtr[proc].flag,(uint8*)&thisProcessSysCtr.flag,sizeof(ProcessSysCtr));
		}
	}
}

