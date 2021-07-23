#ifndef __COMMON_H_
#define __COMMON_H_

#define Byte_Mode_Small						0x00
#define Byte_Mode_Big							0x01
#define Byte_Mode									Byte_Mode_Small

#define LimitCtr(a,min,max) (a>=min&&a<=max)?a:(a>max)?max:min

uint16 CRC16(uint8 *puchMsg, uint16 usDataLen);
bool TickOut(uint32 *tick,uint32 timeout);
void Sleep(uint32 milliSec);
void uint16Tou16Ascii(uint16 tmp,uint16 *u16Ascii);
/*
void Systime_Up_DIWEN_Show(uint16 *ptr_time);
*/
bool Compare_Data(uint8*Src,uint8*Dst,uint16 len);
void  Uint16Reverse(uint16 *buf, uint8 len);
uint8 CalcBcc(uint8 *buf,uint16 len);
uint8 calcSum(uint8 *buf,uint16 len);
void U32ToBytesBE(uint8 *pu8Buf, uint32 u32Dat);
void A_FConvertToAscii(uint8 *tmp);
void uint32ToAscii(uint32 inint,uint8 point, uint16 *outasc);
void ByteConversionAscii(uint8*src,uint8*dst,uint16 len);
void AsciiConversionByte(uint8*src,uint8*dst,uint16 len);
/*
** ½»»»¸ßµÍ×Ö½Ú
*/
uint16 little_Big_Endian__Exchange(uint16 data);
#endif

