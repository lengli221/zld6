#include "includes.h"

int stk[1024] = {0}; 

void hard_init(void)
{
	BspCPUClkFreq();
	BspInit();
}

void Thread(void *p)
{	
	SM_InitSmComManagement();
	HMI_Init();
	DoorSysLogicCtr_Init();
	
	while(1)
	{
		Sleep(50);
	}
}

int main()
{
	#if OS_CRITICAL_METHOD == 3u           		
		OS_CPU_SR  cpu_sr = 0u;
	#endif
	OS_ENTER_CRITICAL();
	hard_init();

	/*读取备份区数据*/
	chargerRecordReadBackupData();
	os_init();

	Thread_create(Thread,0,&stk[1024 - 1],10);

	OS_EXIT_CRITICAL();

	Thread_start();

	while(1)
	{
		Sleep(10);
	}
}

void App_TaskIdleHook(void)
{

}

