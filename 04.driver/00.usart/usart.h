#ifndef __USART_H_
#define __USART_H_

#define UART_BUFLEN								512		               
#define DMA_BUFLEN         							512              

#define USART1_DR_ADDRESS                 			((uint32_t)USART1 + 0x04)
#define USART2_DR_ADDRESS                 			((uint32_t)USART2 + 0x04)
#define USART3_DR_ADDRESS               				((uint32_t)USART3 + 0x04)
#define UART4Tx_DR_ADDRESS                			((uint32_t)UART4 + 0x04)

#define BSP_ComType_USART             				0x0001

#define BSP_ComUsart1								1//card
#define BSP_ComUsart2								2//charger state
#define BSP_ComUsart3        							3//lcd
#define BSP_ComUsart4								4

#define BSP_COM_NO									0			
#define BSP_COM_EVEN								1			
#define BSP_COM_ODD								2			

#define BSP_COM_STOP1							1
#define BSP_COM_STOP2							2

#define USART4_485_RX       							(GPIOC->BSRRH = GPIO_Pin_8)
#define USART4_485_TX       							(GPIOC->BSRRL = GPIO_Pin_8)
#define USART2_485_RX       							(GPIOG->BSRRH = GPIO_Pin_8)
#define USART2_485_TX       							(GPIOG->BSRRL = GPIO_Pin_8)

typedef struct {
  bool  *  Busy;    
  Queue *  TxQueue;   
  Queue *  RxQueue;   
}UartInfo;

void Usart1_Init(void);
void Usart1_DmaConfig(void);
void Usart1_BufferInit(void);
int Usart1_Rcv(uint8 *rxBuf, int rxNum);
bool Usart1_DmaWrite(uint8 * txBuf,uint32 txNum);

void Usart2_Init(void);
void Usart2_DmaConfig(void);
void Usart2_BufferInit(void);
int Usart2_Rcv(uint8 *rxBuf, int rxNum);
bool Usart2_DmaWrite(uint8 * txBuf,uint32 txNum);

void Usart3_Init(void);
void Usart3_DmaConfig(void);
void Usart3_BufferInit(void);
int Usart3_Rcv(uint8 *rxBuf, int rxNum);
bool Usart3_DmaWrite(uint8 * txBuf,uint32 txNum);

void Usart4_Init(void);
void Usart4_DmaConfig(void);
void Usart4_BufferInit(void);
int Usart4_Rcv(uint8 *rxBuf, int rxNum);
bool Usart4_DmaWrite(uint8 * txBuf,uint32 txNum);

void Com_Init(void);
void BSP_SmSend(uint8 ucComType,uint8 ucSmNo,void *msgbuf, uint16 *uclen);
void BSP_SmRecv(uint8 ucComType,uint8 ucSmNo,void *msgbuf, uint16 *uclen);
#endif

