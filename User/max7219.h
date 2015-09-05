#ifndef __MAX7219_H__
#define __MAX7219_H__
#include "stm32f10x.h"
 //�Ĵ����궨��

#define DECODE_MODE  0x09   //������ƼĴ���
#define INTENSITY    0x0A   //���ȿ��ƼĴ���
#define SCAN_LIMIT   0x0B   //ɨ����޼Ĵ���
#define SHUT_DOWN    0x0C   //�ض�ģʽ�Ĵ���
#define DISPLAY_TEST 0x0F   //���Կ��ƼĴ��� 

//�ܽ�����

#define MAX7219_LOAD1_PORT GPIOB 
#define MAX7219_LOAD1_PIN GPIO_Pin_10//�����

#define MAX7219_LOAD2_PORT GPIOB 
#define MAX7219_LOAD2_PIN GPIO_Pin_11//�����

#define MAX7219_LOAD3_PORT GPIOB 
#define MAX7219_LOAD3_PIN GPIO_Pin_12//LED

#define MAX7219_DI_PORT GPIOA
#define MAX7219_DI_PIN GPIO_Pin_7

#define MAX7219_CLK_PORT GPIOA
#define MAX7219_CLK_PIN GPIO_Pin_5
//LED ����
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
//����һ���е�ĳ��LED����                                    
                                     
//��������

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
