#ifndef __MAX7219_H__
#define __MAX7219_H__
#include "stm32f10x.h"
 //寄存器宏定义

#define DECODE_MODE  0x09   //译码控制寄存器
#define INTENSITY    0x0A   //亮度控制寄存器
#define SCAN_LIMIT   0x0B   //扫描界限寄存器
#define SHUT_DOWN    0x0C   //关断模式寄存器
#define DISPLAY_TEST 0x0F   //测试控制寄存器 

//管脚设置

#define MAX7219_LOAD1_PORT GPIOB 
#define MAX7219_LOAD1_PIN GPIO_Pin_10//数码管

#define MAX7219_LOAD2_PORT GPIOB 
#define MAX7219_LOAD2_PIN GPIO_Pin_11//数码管

#define MAX7219_LOAD3_PORT GPIOB 
#define MAX7219_LOAD3_PIN GPIO_Pin_12//LED

#define MAX7219_DI_PORT GPIOA
#define MAX7219_DI_PIN GPIO_Pin_7

#define MAX7219_CLK_PORT GPIOA
#define MAX7219_CLK_PIN GPIO_Pin_5
//LED 定义
//E2    LedBlk3
#define LEDRun              (1<<6)
#define LEDCommunication    (1<<5)
#define LEDLocalControl     (1<<4)
#define LEDRemoteControl    (1<<3)
#define LEDVoltageStable    (1<<2)
#define LEDCurrentStable    (1<<1)
#define LEDAlarm            (1<<0)

//E1    LedBlk2
#define LEDTotalTime        (1<<6)
#define LEDForwardTime      (1<<5) 
#define LEDReverseTime      (1<<4)
//E0    LedBlk1
#define LEDReverseFreq      (1<<7)
#define LEDForwardCurrent   (1<<6)
#define LEDReverseCurrent   (1<<5)
#define LEDForwardVoltage   (1<<4)
#define LEDReverseVoltage   (1<<3)
#define LEDForwardDuty      (1<<2)
#define LEDReverseDuty      (1<<1)
#define LEDForwardFreq      (1<<0)
     
 
#define OnOffLed(block,Led,state) do{ state>0 ? ((block)|=(led)) : ((block)&=~(led)) }while(0)
//控制一组中的某个LED亮灭                                    
                                     
//函数声明

//void Write7219(unsigned char address,unsigned char dat);
void Write7219(unsigned char address,unsigned char dat,unsigned char select);
void MAX7219_Init(void); 

typedef struct
    {
        uint16_t Now;
        uint16_t FirstSegment;
        uint16_t LastSegment;
        uint16_t DpLocation;
        uint16_t SelectSegment;
        uint16_t Next;
        uint8_t* Disp1[4];
    }LedTube;


#endif
