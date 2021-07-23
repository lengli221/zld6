#ifndef __ZY_CHARGER_PRO_H_
#define __ZY_CHARGER_PRO_H_

#define get_ZY_Charger_FunCode									(uint8)0x04
#define set_ZY_Charger_FunCode									(uint8)0x10


/*
** zy PackageFrame
*/
bool zy_PackageFrame(void* pPara,void* pBuf1,uint16* dataLen);
/*
** zy ParseFrame
*/
bool zy_ParseFrame(void* pPara,void* pBuf1,uint16 dataLen);

#endif

