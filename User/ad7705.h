#ifndef __AD7705_H__
#define __AD7705_H__
#include "stm32f10x.h"
#include "SysTick.h"
void TM7705_Init(void);
u16 ReadTM7705_16BitValue(void);
u16 ReadTM7705(void);
void SPIx_Init(void);


#endif
