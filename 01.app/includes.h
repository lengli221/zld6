#ifndef __INLUDES_H_
#define __INLUDES_H_

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include "ucos_ii.h"

#include "stm32f2xx.h"
#include "stm32f2xx_conf.h"
#include "stm32f2xx_spi.h"


#include "type.h"
#include "queue.h"
#include "common.h"

#include "DGUSVPro.h"
#include "MT318Protocol.h"
#include "zy_Charger_Pro.h"

#include "usart.h"
#include "stm32f207bsp.h"
#include "os_bsp.h"
#include "SPIFlash.h"
#include "FM24CL16.h"
#include "gpio.h"
#include "rtc.h"
#include "timer.h"
#include "temp.h"
#include "adc.h"
#include "backup.h"
#include "watchdog.h"

#include "main.h"
#include "app_cfg.h"
#include "SMComManager.h"
#include "HMI_DW_DATA.h"
#include "record.h"
#include "paramConfig.h"
#include "lcdData_Voice.h"
#include "LCDData_DW.h"
#if MT_CardVersion == MT622V204
#include "CardReaderData.h"
#elif MT_CardVersion == MT_Card_M1
#include "CardM1ReaderData.h"
#endif

#include "zy_Charger.h"
#include "zy_Charger_Test.h"

#include "chargerstate.h"
#include "door_sys_logicCtr_2.h"
#include "CalcChargerTime_2.h"


#endif

