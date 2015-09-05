/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "SysTick.h"
#include "stdio.h"
#include "max7219.h"
/* 
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
extern uint8_t Disp1[4];//第一~四位，小数点位置
extern uint8_t Disp2[4];
extern uint8_t Disp3[4];
extern uint8_t Disp4[4];
extern LedTube  LedTube1[4];
extern uint8_t LedBlk1,LedBlk2,LedBlk3;
 void (*KeyFuncPtr)(); //按键功能指针
uint8_t FRIFlag=1,FRVFlag=1,FRTFlag=1,FRFFlag=1,FRDutyFlag=1,CCCVFlag=1;//1正向 0反向
uint8_t SingDualDcFlag=2,OnOffFlag=0,KeyOnoff=1,ConfirmChange=0;//1直流2单脉冲3双脉冲
uint16_t FTMinSet,FTSecSet,RTMinSet,RTSecSet,FFSet,RFSet,FDutySet,RDUtySet,TotalTimeMin,TotalTimeHour;
float FVSet,RVSet,FISet,RISet;
uint16_t FVMeasure,RVMeasure,FIMeasure,RIMeasure;

typedef struct{
    void (*InitOperate)();      //初始化状态应该执行的功能操作
    void (*CurrentOperate)(); //当前状态应该执行的功能操作
    //KbdTabStruct (*LastLevelMenu);//上次菜单指针
    void (*UpOperate)();      //初始化状态应该执行的功能操作
    void (*DownOperate)();      //初始化状态应该执行的功能操作
    void (*RightOperate)();      //初始化状态应该执行的功能操作
    void (*SetOperate)();      //初始化状态应该执行的功能操作
    void (*OnoffOperate)();      //初始化状态应该执行的功能操作
    void (*CCCVOperate)();      //初始化状态应该执行的功能操作
    void (*RemoteOperate)();      //初始化状态应该执行的功能操作
    KbdTabStruct (*NextLevelMenu);//下次菜单指针
    LedTube* DispTube;
} KbdTabStruct;
KbdTabStruct IdleMenu,Select,SetMenu,CalMenu,RunMenu;

IdleMenu.InitOperate =IdleMenuInit;
IdleMenu.CurrentOperate =DispIdle;
void menuinit()
{
    IdleMenu.InitOperate =IdleMenuInit;
    IdleMenu.CurrentOperate =DispIdle;
   
}

void ShowFRV(float FRV)
{
    FRV*=10;
    Disp1[0]=FRV%10000/1000;//正向电压  0.1-999.9  小于100显示小数
    Disp1[1]=FRV%1000/100;
    Disp1[2]=FRV%100/10;
    Disp1[3]=FRV%10;
    if(FRV>1000)
    {        
        Disp[3]=Hide;
    }
    else
    {
        Disp1[2]|=0x80;
    }
    DispRefresh(Disp1,Disp2,Disp3,Disp4,5);//全刷新
}
void ShowFRI(float FRI)
{   
    Disp2[0]=FRI%10000/1000;//正向电流 无小数 1-2000
    Disp2[1]=FRI%1000/100;
    Disp2[2]=FRI%100/10;
    Disp2[3]=FRI%10;
}
void ShowFRF(uint16_t FRF)
{   
    Disp3[0]=FRF%10000/1000;//正向频率 10-9999
    Disp3[1]=FRF%1000/100;
    Disp3[2]=FRF%100/10;
    Disp3[3]=FRF%10;
}
void HideV()
{
    Disp1[0]=Hide;//正向电压  0.1-999.9  小于100显示小数
    Disp1[1]=Hide;
    Disp1[2]=Hide;
    Disp1[3]=Hide;
}
void HideI()
{
    Disp2[0]=Hide;//正向电流 无小数 1-2000
    Disp2[1]=Hide;
    Disp2[2]=Hide;
    Disp2[3]=Hide;
}
void TimeShow(uint16_t TimeHigh,uint16_t TimeLow)
{
    Disp4[0]=TimeHigh%100/10;//正向时间  分：秒
    Disp4[1]=TimeHigh%10;

    Disp4[2]=TimeLow%100/10;
    Disp4[3]=TimeLow%10;
}
void ShowFRDuty(uint16_t FRDuty)
void IdleUp()
{
}
void IdleDown()
{
}
void IdleRight()
{
}
void IdleSet()
{
    Select=NextLevelMenu;
    Select.InitOperate();
    Select.CurrentOperate();
}
void IdleMenuInit()
{
    CCCVFlag=1;//恒压
    SingDualDcFlag=3;//双脉冲


}
void IdleOnOff()
{
    OnOffFlag=1;
    Select=IdleMenu;
    Select.InitOperate();
    Select.CurrentOperate();


}
void IdleCCCV()
{
    //恒压恒流切换
    if(CCCVFlag==1)
    {
        CCCVFlag=0;//恒流
        OnOffLed(LedBlk3,LEDVoltageStable,1);
        OnOffLed(LedBlk3,LEDCurrentStable,0);
    }
    else
    {
        CCCVFlag=1;//恒压
        OnOffLed(LedBlk3,LEDVoltageStable,0);
        OnOffLed(LedBlk3,LEDCurrentStable,1);
    }

}
void IdleRemote()
{
    if(KeyOnoff==1)
    {
        KeyOnoff = 0;
        OnOffLed(LedBlk3,LEDRemoteControl,1);
        OnOffLed(LedBlk3,LEDLocalControl,0);
    }
    else
    {
        KeyOnoff = 1;
        OnOffLed(LedBlk3,LEDRemoteControl,0);
        OnOffLed(LedBlk3,LEDLocalControl,1);
    }
    
}

{
    Disp3[0]=15;//不显示前两位
    Disp3[1]=15;
    Disp3[2]=FRDuty%100/10;
    Disp3[3]=FRDuty%10;
}
void DispIdle()
{

    uint8_t t=0;
    //显示区域1 显示区域2    
    //正向
    if((CCCVFlag==1)&&(t%2==0))
    {
        ShowFRV(FVSet);
        HideI();
    }
    if((CCCVFlag==0)&&(t%2==0))
    {
        ShowFRI(FISet);
        HideV();
    }    
    //反向
    if((CCCVFlag==1)&&(t%2==1))
    {
        ShowFRV(RVSet);
        HideI();
    }
     if((CCCVFlag==0)&&(t%2==1))
    { 
        ShowFRI(RISet);   
        HideV(); 
    }
    //显示区域3
    if(t%4==0)
    {
        ShowFRF(FFSet);//显示正向频率
    }
    if(t%4==1)
    {
        ShowFRF(RFSet);//显示负向频率
    }
    if(t%4==2)
    {
        ShowFRDuty(FDutySet)//正向占空 5-95
    }
    if(t%4==3)
    {
        ShowFRDuty(RDutySet)//负向占空 5-95
    }
    //显示区域4
    if(t%3==0)
    {
        TimeShow(TotalTimeHour,TotalTimeMin);//总时间  时：分      
    }
    if(t%3==1)
    {
        TimeShow(FTMinSet,FTSecSet);//正向时间  分：秒
        
    }
    if(t%3==2)
    {
        TimeShow(RTMinSet,RTSecSet);//反向时间  分：秒
    }
    DispRefresh(Disp1,Disp2,Disp3,Disp4,5);//数码管全刷新

    t++;

    if(t>=SingDualDcFlag)//1直流2单脉冲3双脉冲
    {
            t=0;
    }   
}





 /*
   Prescaler = (TIM3CLK / TIM3 counter clock) - 1
   The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
   = 24 MHz / 666 = 36 KHz
     TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
     TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
     TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
     TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

*/
 void TIM2_init()
 {   
   uint32_t PrescalerValue;
   GPIO_InitTypeDef GPIO_Initstructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef TIM_OCInitStructure;
    /* Compute the prescaler value */
   PrescalerValue = (uint16_t) (SystemCoreClock /72000000) - 1;
 
  /* Time base configuration */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
  TIM_TimeBaseInitStructure .TIM_ClockDivision = 0;
  TIM_TimeBaseInitStructure .TIM_CounterMode =TIM_CounterMode_Up;
   TIM_TimeBaseInitStructure .TIM_Period= 30;//24M/10=2.4M  设置下一个更新事件装入活动的自动重装载寄存器周期的值
   TIM_TimeBaseInitStructure .TIM_Prescaler=PrescalerValue ;
   TIM_TimeBaseInitStructure .TIM_RepetitionCounter=0;
   TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
   
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure .TIM_OCIdleState=TIM_OCIdleState_Reset ;
   TIM_OCInitStructure .TIM_OCNIdleState =TIM_OCNIdleState_Set ;
   TIM_OCInitStructure .TIM_OCMode=TIM_OCMode_PWM1 ;     //poler向上计数时，当TIMx_CNT < TIMx_CCR*时，输出电平有效，否则为无效向下计数时，当TIMx_CNT > TIMx_CCR*时，输出电平无效，否则为有效
   TIM_OCInitStructure .TIM_OCNPolarity =TIM_OCNPolarity_Low ;
   TIM_OCInitStructure .TIM_OCPolarity= TIM_OCPolarity_High ;                 
   TIM_OCInitStructure .TIM_OutputNState=TIM_OutputNState_Enable ;// 互补输出使能
   TIM_OCInitStructure .TIM_OutputState =TIM_OutputState_Enable ; //开启OC*输出到对应引脚
   TIM_OCInitStructure .TIM_Pulse =15;                     //比较/PWM 通道的值
   
   
   TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable );
   
//    /* PWM1 Mode configuration: Channel2 */
//  
//   TIM_OCInitStructure.TIM_Pulse = 5;
// 
//   TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//   TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//   
//      /* PWM1 Mode configuration: Channel3 */
//  
//   TIM_OCInitStructure.TIM_Pulse = 7;
// 
//   TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//   TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
//       /* PWM1 Mode configuration: Channel4 */
//  
//   TIM_OCInitStructure.TIM_Pulse = 9;
// 
//   TIM_OC4Init(TIM2, &TIM_OCInitStructure);
//   TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

   
   TIM_ARRPreloadConfig( TIM2,ENABLE   );     
   /* TIM2 enable counter */

   TIM_Cmd(TIM2, ENABLE);
    //     TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState); ctrl TIM and TIM8
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE ); 
  GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_Initstructure);
 }

U8 CheckKey(void)
{
    U8 IsKey;
    Key Key1;
    IsKey = 1;
    asm("wdr");
    Key = KeyScan();
    switch(Key1){
        case Up:{ //向上键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case Down:{ //向下键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDnState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

           (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case Set:{ //向左键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case OnOff:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case Choice:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case CCCV,:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case Remote:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        default: //按键错误的处理
            IsKey = 0;
            break;
    }
    asm("wdr");
    if(Flash){//两个当中有任意一个不为零，那么执行一次，我们应该不会出现Flash和ReFlash都是1的情况
        KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
        (*KeyFuncPtr)();//执行当前按键的操作
        Flash = 0;
       // ReFlash = 0;
     }

    return IsKey;
}



 void main(void) 
 {

  /* 设置系统时钟为72M */
 
  SystemInit();
  SysTick_Init(); 
  MAX7219_Init(); 
  Write7219(0x01,0X01);      //数码管显示1~8
  Delay_ms(2);
  Write7219(0x02,0X02);      //数码管显示1~8
  Delay_ms(2);
  Write7219(0x03,0X03);      //数码管显示1~8
  Delay_ms(2);
  Write7219(0x04,0X8f);      //数码管显示1~8
  Delay_ms(2);

  
//  //Set PA8 clock output 4M
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );  
//  GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AF_PP;
//  GPIO_Initstructure.GPIO_Pin=GPIO_Pin_8;
//  GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_Initstructure);
//  RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);
//  //led toggle  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
//  GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;
//  GPIO_Initstructure.GPIO_Pin=GPIO_Pin_13;           
//  GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_Init(GPIOC, &GPIO_Initstructure);
//  
	//TIM2_init(); 
  


 // while(1);

  	  while(1)
      {
      
        
      }
	  // add your code here ^_^。
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


