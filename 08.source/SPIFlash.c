#include "spiflash.h"
#include "includes.h"

#define SPICS_PORT 		GPIOA
#define SPICS_Pin 		GPIO_Pin_4

/* Deselect sFLASH: Chip Select pin high */
#define FLASH_CS_LOW()       GPIO_ResetBits(SPICS_PORT, SPICS_Pin)
#define FLASH_CS_HIGH()      GPIO_SetBits(SPICS_PORT, SPICS_Pin)

static uint8 FLASH_Buffer[4096];

/*******************************************************************************
 函数名称：SPI_Gpio_Config
 功能描述：SPI GPIO 初始化
 输   入：
 返 回 值：
 *******************************************************************************/
static void SPI_Gpio_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*!< Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*!< SPI pins configuration *************************************************/

    /*!< Connect SPI pins to AF5 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*!< Configure sFLASH Card CS pin in output PushPull Mode ********************/
    GPIO_InitStructure.GPIO_Pin = SPICS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SPICS_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 函数名称：sFLASH_SendByte
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
static uint8 sFLASH_SendByte(uint8 byte)
{
    /*!< Loop while DR register in not emplty */
    int timeout = 0xfffff;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET && timeout--);
    if(timeout == 0)
    {
        return 0;
    }
    /*!< Send byte through the SPI1 peripheral */
    SPI1->DR = byte;
    /*!< Wait to receive a byte */
    timeout = 0xfffff;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET && timeout--);
    if(timeout == 0)
    {
        return 0;
    }
    /*!< Return the byte read from the SPI bus */
    return SPI1->DR;;
}

/*******************************************************************************
 函数名称：sFLASH_WaitForWriteEnd
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
static void sFLASH_WaitForWriteEnd(void)
{
    uint8 flashstatus = 0;

    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Read Status Register" instruction */
    sFLASH_SendByte(sFLASH_CMD_RDSR);

    /*!< Loop as long as the memory is busy with a write cycle */
    do
    {
        /*!< Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

    }
    while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 函数名称：sFLASH_WriteEnable
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
static void sFLASH_WriteEnable(void)
{
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Write Enable" instruction */
    sFLASH_SendByte(sFLASH_CMD_WREN);

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 函数名称：sFLASH_EraseSector
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
void sFLASH_EraseSector(uint32 SectorAddr)
{
    /*!< Send write enable instruction */
    sFLASH_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /*!< Send Sector Erase instruction */
    sFLASH_SendByte(sFLASH_CMD_SE);
    /*!< Send SectorAddr high nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /*!< Send SectorAddr medium nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /*!< Send SectorAddr low nibble address byte */
    sFLASH_SendByte(SectorAddr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    sFLASH_WaitForWriteEnd();
}

/*******************************************************************************
 函数名称：sFLASH_WritePage
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
static  void sFLASH_WritePage(uint8 *pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
    /*!< Enable the write access to the FLASH */
    sFLASH_WriteEnable();
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /*!< Send "Write to Memory " instruction */
    sFLASH_SendByte(sFLASH_CMD_WRITE);	
    /*!< Send WriteAddr high nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*!< Send WriteAddr medium nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*!< Send WriteAddr low nibble address byte to write to */
    sFLASH_SendByte(WriteAddr & 0xFF);

    /*!< while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /*!< Send the current byte */
        sFLASH_SendByte(*pBuffer);
        /*!< Point on the next byte to be written */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    sFLASH_WaitForWriteEnd();
}


/*******************************************************************************
 函数名称：sFLASH_WriteBuffer
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
void sFLASH_WriteBuffer(uint8 *pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
    uint8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
    count = sFLASH_SPI_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
    NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

    if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
        {
            sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /*!< NumByteToWrite > sFLASH_PAGESIZE */
        {
            while (NumOfPage--)
            {
                sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
                WriteAddr +=  sFLASH_SPI_PAGESIZE;
                pBuffer += sFLASH_SPI_PAGESIZE;
            }

            sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
        {
            if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
            {
                temp = NumOfSingle - count;

                sFLASH_WritePage(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                sFLASH_WritePage(pBuffer, WriteAddr, temp);
            }
            else
            {
                sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /*!< NumByteToWrite > sFLASH_PAGESIZE */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
            NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

            sFLASH_WritePage(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
                WriteAddr +=  sFLASH_SPI_PAGESIZE;
                pBuffer += sFLASH_SPI_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*******************************************************************************
 函数名称：sFLASH_ReadBuffer
 功能描述：
 输   入：
 返 回 值：
 *******************************************************************************/
void sFLASH_ReadBuffer(uint8 *pBuffer, uint32 ReadAddr, uint16 NumByteToRead)
{
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Read from Memory " instruction */
    sFLASH_SendByte(sFLASH_CMD_READ);

    /*!< Send ReadAddr high nibble address byte to read from */
    sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /*!< Send ReadAddr medium nibble address byte to read from */
    sFLASH_SendByte((ReadAddr & 0xFF00) >> 8);
    /*!< Send ReadAddr low nibble address byte to read from */
    sFLASH_SendByte(ReadAddr & 0xFF);

    while (NumByteToRead--) /*!< while there is data to be read */
    {
        /*!< Read a byte from the FLASH */
        *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}


void sFLASH_WriteData(uint8 *pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
	uint32 sectorAddr = WriteAddr & 0xFFFFF000;
	uint32 lastCntNum;
	uint8 writeCnt;
	writeCnt = (WriteAddr+NumByteToWrite-sectorAddr)/sFLASH_SPI_SECTORSIZE;
	lastCntNum = ((WriteAddr+NumByteToWrite-sectorAddr)%sFLASH_SPI_SECTORSIZE);
	if(lastCntNum)
	{
		writeCnt++;
	}

	if((writeCnt > 0)&&(WriteAddr != sectorAddr))
	{
		#if 0
		sFLASH_ReadBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sFLASH_EraseSector(sectorAddr);
		if((WriteAddr+NumByteToWrite) < (sectorAddr + sFLASH_SPI_SECTORSIZE))
		{
			memcpy(FLASH_Buffer+WriteAddr-sectorAddr,pBuffer,NumByteToWrite);
			pBuffer += NumByteToWrite;
		}
		else
		{
			memcpy(FLASH_Buffer+WriteAddr-sectorAddr,pBuffer,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
			pBuffer += sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr);
		}
		sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sectorAddr += sFLASH_SPI_SECTORSIZE;
		writeCnt--;
		#else
		if((WriteAddr+NumByteToWrite) < (sectorAddr + sFLASH_SPI_SECTORSIZE))
		{
			memcpy(FLASH_Buffer,pBuffer,NumByteToWrite);
			pBuffer += NumByteToWrite;
			sFLASH_WriteBuffer(FLASH_Buffer,WriteAddr,NumByteToWrite);
		Sleep(1);
			writeCnt = 0;
		}
		else
		{
			memcpy(FLASH_Buffer,pBuffer,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
			pBuffer += sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr);
			sFLASH_WriteBuffer(FLASH_Buffer,WriteAddr,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
		Sleep(1);

			sectorAddr += sFLASH_SPI_SECTORSIZE;
			writeCnt--;
		}
		#endif
	}
	while(writeCnt)
	{
		sFLASH_ReadBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sFLASH_EraseSector(sectorAddr);
		Sleep(3);
		if(writeCnt == 1)
		{
			memcpy(FLASH_Buffer,pBuffer,lastCntNum);
			sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,lastCntNum);
		}
		else
		{
			memcpy(FLASH_Buffer,pBuffer,sFLASH_SPI_SECTORSIZE);
			sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		}
		Sleep(1);
		sectorAddr += sFLASH_SPI_SECTORSIZE;
		pBuffer += sFLASH_SPI_SECTORSIZE;
		writeCnt--;
	}
}

void sFLASH_WriteDataWithClr(uint8 *pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
	uint32 sectorAddr = WriteAddr & 0xFFFFF000;
	uint32 lastCntNum;
	uint8 writeCnt;
	writeCnt = (WriteAddr+NumByteToWrite-sectorAddr)/sFLASH_SPI_SECTORSIZE;
	lastCntNum = ((WriteAddr+NumByteToWrite-sectorAddr)%sFLASH_SPI_SECTORSIZE);
	if(lastCntNum)
	{
		writeCnt++;
	}

	if((writeCnt > 0)&&(WriteAddr != sectorAddr))
	{
		#if 1
		sFLASH_ReadBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sFLASH_EraseSector(sectorAddr);
		if((WriteAddr+NumByteToWrite) < (sectorAddr + sFLASH_SPI_SECTORSIZE))
		{
			memcpy(FLASH_Buffer+WriteAddr-sectorAddr,pBuffer,NumByteToWrite);
			pBuffer += NumByteToWrite;
		}
		else
		{
			memcpy(FLASH_Buffer+WriteAddr-sectorAddr,pBuffer,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
			pBuffer += sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr);
		}
		sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sectorAddr += sFLASH_SPI_SECTORSIZE;
		writeCnt--;
		#else
		if((WriteAddr+NumByteToWrite) < (sectorAddr + sFLASH_SPI_SECTORSIZE))
		{
			memcpy(FLASH_Buffer,pBuffer,NumByteToWrite);
			pBuffer += NumByteToWrite;
			sFLASH_WriteBuffer(FLASH_Buffer,WriteAddr,NumByteToWrite);
		Sleep(1);
			writeCnt = 0;
		}
		else
		{
			memcpy(FLASH_Buffer,pBuffer,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
			pBuffer += sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr);
			sFLASH_WriteBuffer(FLASH_Buffer,WriteAddr,sFLASH_SPI_SECTORSIZE-(WriteAddr-sectorAddr));
		Sleep(1);

			sectorAddr += sFLASH_SPI_SECTORSIZE;
			writeCnt--;
		}
		#endif
	}
	while(writeCnt)
	{
		sFLASH_ReadBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		sFLASH_EraseSector(sectorAddr);
		Sleep(3);
		if(writeCnt == 1)
		{
			memcpy(FLASH_Buffer,pBuffer,lastCntNum);
			sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,lastCntNum);
		}
		else
		{
			memcpy(FLASH_Buffer,pBuffer,sFLASH_SPI_SECTORSIZE);
			sFLASH_WriteBuffer(FLASH_Buffer,sectorAddr,sFLASH_SPI_SECTORSIZE);
		}
		Sleep(1);
		sectorAddr += sFLASH_SPI_SECTORSIZE;
		pBuffer += sFLASH_SPI_SECTORSIZE;
		writeCnt--;
	}
}

unsigned int SPI_flash_ReadID(void)
{
    unsigned int Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "RDID " instruction */
    sFLASH_SendByte(0x9F);


    /*!< Read a byte from the FLASH */
    Temp0 = sFLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp1 = sFLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp2 = sFLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();


    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;

}


uint8 SPI_flash_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    SPI_Gpio_Config();

    FLASH_CS_HIGH();																	//Deselect the FLASH: Chip Select high

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//SPI configuration
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);															//Enable the sFLASH_SPI

    if(SPI_flash_ReadID() == SPI_FLASH_ID)
        return 1;
    else
        return 0;
}

uint8 spi_flash_init(void)
{
	return SPI_flash_Init();
}

