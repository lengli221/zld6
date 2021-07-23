#include "includes.h"
uint8 uart4TxBuffer[UART_BUFLEN];
uint8 uart4RxBuffer[UART_BUFLEN];
static uint8 uart4DMABuffer[DMA_BUFLEN];

DECLARE_QUEUE(uart4TxQueue,uart4TxBuffer, sizeof(uart4TxBuffer));
DECLARE_QUEUE(uart4RxQueue,uart4RxBuffer, sizeof(uart4RxBuffer));

bool uart4Busy = false;
const UartInfo uart4Info={
    &uart4Busy,
    &uart4TxQueue,
    &uart4RxQueue
};


void Usart4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	USART4_485_RX;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Usart4_DmaConfig(void) //DMA
{
	DMA_InitTypeDef DMA_InitStructure;
	/*
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART4Tx_DR_ADDRESS;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32)uart4DMABuffer;
	DMA_InitStructure.DMA_BufferSize = DMA_BUFLEN;
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);  
	DMA_ITConfig(DMA1_Stream4, DMA_IT_HT, DISABLE); 
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);   
}

void Usart4_DmaSend(void) //DMA发送
{
	uint32 bufferCount = 0;
	while((bufferCount<DMA_BUFLEN )&&(!QueueIsEmpty(uart4Info.TxQueue)))
	{
		Dequeue(uart4Info.TxQueue,&uart4DMABuffer[bufferCount++]);//出列
	}
	DMA1_Stream4->NDTR = bufferCount;//缓冲大小
	DMA1_Stream4->M0AR = (uint32)uart4DMABuffer;//buffer地址 
	//*uart4Info.Busy = true;
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);//开启DMA中断 
	USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);//关闭接收中断
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);//开启UART DMA请求
	DMA_Cmd(DMA1_Stream4,ENABLE);//打开DMA
}

bool Usart4_DmaWrite(uint8 * txBuf,uint32 txNum)
{
	uint32 timeOutCount = 4000;
	
	if((txNum == 1) || (txNum > 512)|| (txNum == 0))
	{
		return false;
	}
	else
	{
		while(timeOutCount--)
		{
			if(!(*uart4Info.Busy))//端口
			//if(QueueIsEmpty(uart4Info.TxQueue))
			{
				*uart4Info.Busy = true;
				USART4_485_TX;
				FlushQueue(uart4Info.TxQueue);
				FlushQueue(uart4Info.RxQueue);
				while(txNum>0)//全部入列
				{
					if(Enqueue(uart4Info.TxQueue, *txBuf++))
						txNum--;
					else
						break;
				}
				Usart4_DmaSend();
				while(*uart4Info.Busy)
				{
					Sleep(1);
				}
				Sleep(1);
				USART4_485_RX;
				return true;
			}
			else
			{
				timeOutCount--;
				Sleep(50);//休眠10ms
				if(timeOutCount == 0)//连续等待80次
				{
					*uart4Info.Busy = false;
					FlushQueue(uart4Info.TxQueue);
					return false;
				}
			}
		}
	}
	return true;
}

void DMA1_Stream4_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_FEIF4|DMA_FLAG_DMEIF4|DMA_FLAG_TEIF4|DMA_FLAG_HTIF4);
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4))
	{
		DMA_Cmd(DMA1_Stream4,DISABLE);	//关闭DMA
		USART_ITConfig(UART4,USART_IT_TC,ENABLE);//开发送完成中断
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,DISABLE);
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
	}
}

void Usart4_BufferInit(void)
{//说明:初始化缓冲区
	FlushQueue(uart4Info.TxQueue);
	FlushQueue(uart4Info.RxQueue);
	*uart4Info.Busy = false;
	memset(uart4TxBuffer, 0, sizeof(uart4TxBuffer));  
	memset(uart4RxBuffer, 0, sizeof(uart4RxBuffer));
	memset(uart4DMABuffer, 0, sizeof(uart4DMABuffer)); 
}

int Usart4_Rcv(uint8 *rxBuf, int rxNum) ///从缓冲区里面读
{
	int32 iTempRxNum = 0;
	
	if(rxNum == 0)
		return -1;
	if(QueueIsEmpty(uart4Info.RxQueue))
		return -1;
	while(rxNum >0 && (!QueueIsEmpty(uart4Info.RxQueue)))
	{
		USART_ITConfig(UART4, USART_IT_TC, DISABLE);
		Dequeue(uart4Info.RxQueue, rxBuf++);
		USART_ITConfig(UART4, USART_IT_TC, ENABLE);
		rxNum--;
		iTempRxNum++;
	};	
	return iTempRxNum;
}

void UART4_IRQHandler(void)
{
	uint8 dat;
	if(USART_GetFlagStatus(UART4,USART_FLAG_FE|USART_FLAG_NE|USART_FLAG_ORE))
	{
		USART_ReceiveData(UART4);//将错误数据读取出来
	}
	
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//接收到数据
	{
		USART_ITConfig(UART4,USART_IT_RXNE,DISABLE); 
		dat = USART_ReceiveData(UART4);
		Enqueue(uart4Info.RxQueue,dat);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	}
	
	if(USART_GetITStatus(UART4,USART_IT_TC)!=RESET)
	{
		USART_ITConfig(UART4,USART_IT_TC,DISABLE); 
		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		*uart4Info.Busy = false;
	}
}

void Usart4_Stop(void)
{
	USART_ITConfig(UART4, USART_IT_TC, DISABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
	USART_Cmd(UART4, DISABLE);   
}

