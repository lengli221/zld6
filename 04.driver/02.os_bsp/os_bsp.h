#ifndef __OS_BSP_H_
#define __OS_BSP_H_

#define BSP_OS_SUCCESS		1			//�ɹ�
#define BSP_OS_FAILURE		0			//ʧ��

int8 Thread_create(void (*fun)(void *p_arg),void *p_arg,int32 *stk,int8 prio);
void Thread_start(void);
void os_init(void);

#endif 

