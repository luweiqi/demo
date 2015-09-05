#include "max7219.h"
#include "SysTick.h"

LedTube  LedTube1[4]=
{
    {0,1,4,1,1,1,0},//��ǰ���,��һ�������λ��,���һ�������λ��,С����λ��,��˸��������,�¸�����λ��
    {1,5,8,5,5,2,0},
    {2,1,4,1,1,3,0},
    {3,5,8,5,5,0,0}
    
};
//int32_t ForwardTime,ReverseTime,ReverseFreq,ForwardCurrent,ReverseCurrent,ForwardVoltage,LEDReverseVoltage,ForwardDuty,ReverseDuty,ForwardFreq;
uint8_t Disp1[4]={0,0,0,0};//��һ~��λ,С����λ��
uint8_t Disp2[4]={0,0,0,0};
uint8_t Disp3[4]={0,0,0,0};
uint8_t Disp4[4]={0,0,0,0};
uint8_t LedBlk1,LedBlk2,LedBlk3;


void DispRefresh(uint8_t *dispa,uint8_t *dispb,uint8_t *dispc,uint8_t *dispd,uint8_t area) //area:1-4  5->all
{
    uint8_t i;
    //��һƬ7219
    if(area==1||area==5)
    {
       // if((*dispa)[4]!=0)  (*dispa)[((*dispa)[4] - 1)] |= 0x80 //������һλ�ϼ�С����
         for(i = 1; i <= 4; i++)
        { 
            Write7219(i, (*dispa),1);      //�������ʾ1~4λ  
            Delay_ms(2);
            dispa++;
        }
       // if((*dispa)[4]!=0)  (*dispa)[((*dispa)[4] - 1)] &= ~0x80 //������һλ��ȥ��С����
    }
    if(area==2||area==5)
    {
         for(i = 5; i <=8; i++)
        {
            Write7219(i, (*dispb),1);      //�������ʾ5~8λ
            Delay_ms(2);
            dispb++;
        }
    }
    //�ڶ�Ƭ7219
    if(area==3||area==5)
    {
         for(i = 1; i <=4; i++)
        {
            Write7219(i, (*dispc),2);      //�������ʾ1~4λ
            Delay_ms(2);
            dispc++;
        }
    }
    if(area==4||area==5)
    {
         for(i = 5; i <=8; i++)
        {
            Write7219(i, (*dispd),2);      //�������ʾ5~8λ
            Delay_ms(2);
            dispd++;
        }
    }
}
void LedRefresh(uint8_t LedBlk1,uint8_t LedBlk2,uint8_t LedBlk3)
{
    //д����Ƭ7219 ����Led
    Write7219(1,LedBlk1,3);
    Write7219(2,LedBlk2,3);
    Write7219(3,LedBlk3,3);
    
}




void Uload_7219(uint8_t sel)
{
    switch(sel)
    {
        case 1 : GPIO_ResetBits (MAX7219_LOAD1_PORT,MAX7219_LOAD1_PIN); break;
        case 2 : GPIO_ResetBits (MAX7219_LOAD2_PORT,MAX7219_LOAD2_PIN); break;
        case 3 : GPIO_ResetBits (MAX7219_LOAD3_PORT,MAX7219_LOAD3_PIN); break;
        case 4 : GPIO_ResetBits (MAX7219_LOAD3_PORT,MAX7219_LOAD1_PIN|MAX7219_LOAD2_PIN|MAX7219_LOAD3_PIN); break;//Unload all
    }
}

void Load_7219(uint8_t sel)
{
    switch(sel)
    {
        case 1 : GPIO_SetBits (MAX7219_LOAD1_PORT,MAX7219_LOAD1_PIN); break;
        case 2 : GPIO_SetBits (MAX7219_LOAD2_PORT,MAX7219_LOAD2_PIN); break;
        case 3 : GPIO_SetBits (MAX7219_LOAD3_PORT,MAX7219_LOAD3_PIN); break;
        case 4 : GPIO_SetBits (MAX7219_LOAD3_PORT,MAX7219_LOAD1_PIN|MAX7219_LOAD2_PIN|MAX7219_LOAD3_PIN); break;//Load all
    }
}
void Write7219(unsigned char address,unsigned char dat,unsigned char select)
{  
    Unload_7219(select);
   // GPIO_ResetBits (MAX7219_LOAD1_PORT,MAX7219_LOAD1_PIN);    //����Ƭѡ��,ѡ������
    Delay_ms(5);
    //���͵�ַ  
    SPI_I2S_SendData(SPI1, address);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
     Delay_ms(5);
    //��������
    SPI_I2S_SendData(SPI1, dat);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
    Load_7219(select);
   // GPIO_SetBits (MAX7219_LOAD1_PORT,MAX7219_LOAD1_PIN);    //���ͽ���,�������������� 
    Delay_ms(5);  
}

//MAX7219��ʼ��,����MAX7219�ڲ��Ŀ��ƼĴ���


void MAX7219_Init()  
{  
        SPI_InitTypeDef SPI_InitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE );
        GPIO_InitStructure .GPIO_Pin=MAX7219_CLK_PIN|MAX7219_DI_PIN ;
        GPIO_InitStructure .GPIO_Mode =GPIO_Mode_AF_PP ;
        GPIO_InitStructure .GPIO_Speed =GPIO_Speed_10MHz;        //SPI IO PIN INIT
        GPIO_Init(GPIOA,&GPIO_InitStructure );

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );
        SPI_InitStructure .SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128 ;   //SPI INIT
        SPI_InitStructure .SPI_DataSize=SPI_DataSize_8b;
        SPI_InitStructure .SPI_CPHA=SPI_CPHA_1Edge;
        SPI_InitStructure .SPI_CPOL=SPI_CPOL_Low;
        SPI_InitStructure .SPI_CRCPolynomial=SPI_CRCPR_CRCPOLY;
        SPI_InitStructure .SPI_Direction=SPI_Direction_1Line_Tx;//8BIT �����ش����� 
        SPI_InitStructure .SPI_FirstBit=SPI_FirstBit_MSB;
        SPI_InitStructure .SPI_Mode=SPI_Mode_Master;
        SPI_InitStructure .SPI_NSS=SPI_NSS_Soft;
        SPI_Init(SPI1, &SPI_InitStructure);
        SPI_Cmd(SPI1, ENABLE );

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE );
        GPIO_InitStructure .GPIO_Pin=MAX7219_LOAD1_PIN|MAX7219_LOAD2_PIN|MAX7219_LOAD3_PIN;
        GPIO_InitStructure .GPIO_Mode =GPIO_Mode_Out_PP ;
        GPIO_InitStructure .GPIO_Speed =GPIO_Speed_10MHz;        //LOAD PIN INIT
        GPIO_Init(GPIOB,&GPIO_InitStructure );
 //       GPIO_ResetBits(MAX7219_LOAD1_PORT,MAX7219_LOAD1_PIN); //LOAD_L
        Unload_7219(4);//Unload All
        Write7219(SHUT_DOWN,0x01,5);         //������������ģʽ��0xX1��
        Delay_ms(2);
        Write7219(DISPLAY_TEST,0x01,5);      //ѡ����ģʽ��0xX0��
        Delay_ms(100);
        Write7219(DISPLAY_TEST,0x00,5);      //ѡ����ģʽ��0xX0��
        Delay_ms(2);
        Write7219(DECODE_MODE,0xff,5);       //ѡ��ȫ����ģʽ
        Delay_ms(2);
        Write7219(SCAN_LIMIT,0x07,5);        //8ֻLEDȫ��
        Delay_ms(2);
        Write7219(INTENSITY,0x0f,5);          //���ó�ʼ���� 
        Delay_ms(2);
}


