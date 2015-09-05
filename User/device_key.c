/*
********************************************************************************
*                                嵌入式微系统
*                                   msOS
*
*                            硬件平台:msPLC DEMO
*                          主芯片:STM32F103R8T6/RBT6
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:Wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        淘宝店:http://52edk.taobao.com
*                      论坛:http://gongkong.eefocus.com/bbs/
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名     : device_key.c
*作用       : 按键设备
*原理       : 基于SystemTick 100/秒 10mS扫描获取按键值，按键值分为长按与短按两种
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
********************************************************************************
*/



#include "drive.h"
#include "system.h"

#define ScanPin00	PbOut(5)
#define ScanPin01	PbOut(4)

#define ScanPin10	PcIn(10)
#define ScanPin11	PcIn(11)
#define ScanPin12	PcIn(12)
#define ScanPin13	PdIn(2)

#define ShortInterval       2		//短按按键间隔，不得低于2
#define LongInterval        40		//长按按键间隔
#define DoubleHitInterval   20		//防连续双击误动间隔
#define KeyBeepInterval     20      //按键音长度

#define ContinueInterval 200    //连续增减

static byte ScanFirstData = invalid;   //第二次采集按键值
static byte ScanSecondData = invalid;    //第三次采集按键值

static byte ScanCounter = 0;
static byte DoubleHitCounter = 0;
static byte KeyBeepCounter = 0;



static byte RemapKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:
            return(0);
        case 0xDF:
            return(1);
        case 0xBF:
            return(2);
        case 0x7F:
            return(3);
        case 0xFE:
            return(4);
        case 0xFD:
            return(5);
        case 0xFB:
            return(6);
        case 0xF7:
            return(7);
        default:
            return(invalid);
    }
}

static byte RemapLongKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:
            return(0x30);
        case 0xDF:
            return(0x31);
        case 0xBF:
            return(0x32);
        case 0x7F:
            return(0x33);
        case 0xFE:
            return(0x34);
        case 0xFD:
            return(0x35);
        case 0xFB:
            return(0x36);
        case 0xF7:
            return(0x37);
        default:
            return(invalid);
    }	
}

static byte ScanPin(void)
{
    byte scan;
	
    scan = invalid;
    if(ScanPin13 == 0)
        scan &= 0x7F;

    if(ScanPin12 == 0)
        scan &= 0xBF;

    if(ScanPin11 == 0)
        scan &= 0xDF;

    if(ScanPin10 == 0)
        scan &= 0xEF;

    ScanPin00 = 0;
    ScanPin01 = 1;
	

    DelayUs(1);

    if(ScanPin13 == 0)
        scan &= 0xF7;

    if(ScanPin12 == 0)
        scan &= 0xFB;

    if(ScanPin11 == 0)
        scan &= 0xFD;

    if(ScanPin10 == 0)
        scan &= 0xFE;

    ScanPin01 = 0;
    ScanPin00 = 1;

    return(scan);
}


/*******************************************************************************
* 描述	    : 系统节拍100/S，即10mS一次扫描获取按键值，在多次检测确认后，
*           : 发送按键的映射消息到LogicTask的消息队列中。
*******************************************************************************/
void KeySystick100Routine(void) 
{
    byte scan;
    byte key;

    if (KeyBeepCounter == 1) 
        System.Device.IO.SetBeep(false);
    
    if (KeyBeepCounter > 0) KeyBeepCounter--;

    scan = ScanPin();
	
    if (scan != invalid) 
    {
        ScanCounter++;
		
        if (ScanCounter == 1) 
            ScanFirstData = scan;
        else if(ScanCounter == 2)
            ScanSecondData = scan;
        else if (ScanCounter > LongInterval) 
            ScanCounter = LongInterval;
    }
    else
	{
        if(DoubleHitCounter)
        {
            DoubleHitCounter--;
            ScanCounter = 0;
            return;
        }
        
        if (ScanCounter == 0)
            return;
        
        if ((ScanFirstData == invalid) || (ScanFirstData != ScanSecondData))
        {
            ScanCounter = 0;
            return;
        }

        if (ScanCounter == LongInterval)        
            key = RemapLongKey(ScanFirstData);
        else if (ScanCounter >= ShortInterval) 
            key = RemapKey(ScanFirstData);        
        
        else
            key = invalid;
        
        if (key != invalid)
        {
            PostMessage(MessageKeyType, key);  
            System.Device.IO.SetBeep(true);
            KeyBeepCounter = KeyBeepInterval;
            DoubleHitCounter = DoubleHitInterval;
        }
        ScanFirstData = invalid;
        ScanSecondData = invalid;
        ScanCounter = 0;
	} 
}

void InitKey(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ScanPin01 = 0;
	ScanPin00 = 1;
}


