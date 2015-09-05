/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/


#include "SysTick.h"
// #include "core_cm3.h"
static __IO u32 TimingDelay;
#define SYSCLK_FREQ  72000000
/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ���� 
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0
  { 
    /* Capture error */ 
    while (1);
  }
  //disable sysick
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/*
 * ��������Delay_us
 * ����  ��us��ʱ����,10usΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
 *       : Delay_us( 50000 ) ��ʵ�ֵ���ʱΪ 500ms
 *       ���ⲿ���� 
 */

void Delay_us(__IO u32 nTime)
{ 
  	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0
  { 
    /* Capture error */ 
    while (1);
  }
  TimingDelay = nTime;
  SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;//enable  systick
  while(TimingDelay != 0);
  //disable sysick
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
void Delay_ms(__IO u32 nTime)
{ 
  	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0
  { 
    /* Capture error */ 
    while (1);
  }
  TimingDelay = nTime;
  SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;//enable  systick
  while(TimingDelay != 0);
  //disable sysick
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
