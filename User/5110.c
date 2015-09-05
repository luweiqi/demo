#include "5110.h"
#include "stm32f10x.h"
#include "math.h"
#include "SysTick.h"
         //1100   
#define VC_ON 0x2C       //1010   
#define VR_ON 0x2A   
#define VF_ON 0x29      //1001   
#define NOP _nop_();_nop_();_nop_();_nop_()    
#define delay_ms(ms) do{Delay_ms(ms);}while(0) 
#define delay_us(us) do{Delay_us(us);}while(0) 
//ASCII����ֿ�
uchar ASCII_6_8[][6] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20 (�ո�)
,{0x00, 0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x00, 0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x00, 0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x00, 0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x00, 0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x00, 0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x00, 0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x00, 0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x00, 0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x00, 0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x00, 0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x00, 0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x00, 0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x00, 0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x00, 0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x00, 0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x00, 0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x00, 0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x00, 0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x00, 0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x00, 0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x00, 0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x00, 0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x00, 0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x00, 0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x00, 0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x00, 0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x00, 0x02, 0x04, 0x08, 0x10, 0x20} // 5c \ //
,{0x00, 0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x00, 0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x00, 0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
//,{0x00, 0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x00, 0x00, 0x00, 0x06, 0x06, 0x00} // 60 `
,{0x00, 0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x00, 0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x00, 0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x00, 0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x00, 0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x00, 0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x00, 0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x00, 0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x00, 0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x00, 0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x00, 0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x00, 0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x00, 0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x00, 0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x00, 0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x00, 0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x00, 0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x00, 0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x00, 0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
,{0x00, 0x78, 0x46, 0x41, 0x46, 0x78} // 7f (delete)
};  
  
    
unsigned char  deep[504] = {        
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};    


/******************************delay time***********************************/  

  
/****************************reset IC for start****************************/  
void LCD_Reset()  
{  
// IE  =  0x00;  
 //RES =  0;  
  GPIO_ResetBits (LED_RES_PORT,LED_RES_PORT_PIN);
  

 delay_ms(50);  
  GPIO_SetBits (LED_RES_PORT,LED_RES_PORT_PIN);
 //RES =  1;  
 delay_ms(50);  
}   
 
void LCD_write_byte(unsigned char dat, unsigned char dc)
{
  GPIO_ResetBits (LED_CS_PORT,LED_CS_PORT_PIN);
  
  if(dc==1) 
  {
    
    GPIO_SetBits (LED_RS_PORT,LED_RS_PORT_PIN);
  }
  else
  {
    GPIO_ResetBits (LED_RS_PORT,LED_RS_PORT_PIN);
   
  }
  
  
    SPI_I2S_SendData(SPI1, dat);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
    GPIO_SetBits (LED_CS_PORT,LED_CS_PORT_PIN);    //��ֹLCD
}

void LCD_set_XY(unsigned char X, unsigned char Y)
  {
      LCD_write_byte(0x80 | X, 0);        // X     ��(������)
	  LCD_write_byte(0x40 | Y, 0);		  // column	 ��(������)
  }
 
void clear_wave()
{
 unsigned char t;
	unsigned char k;
	
	for(t=0;t<6;t++)
	{   
      //LCD_set_XY(0,t);
		for(k=0;k<84;k++)
		{ 
			deep[t*84+k]=0;
				
		} 
	}
    LCD_Draw_Line_x( 0, 83,47, 1 );
LCD_Draw_Line_Y( 0, 0, 47, 1 );
}
/*------------------------------------------
LCD_clear: LCD�������� 
--------------------------------------------*/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);   //00
				
		} 
	}
}
/*********************************************************** 
�������ƣ�LCD_write_char
�������ܣ���ʾӢ���ַ�
��ڲ�����c	:  ��ʾ���ַ�
���ڲ������� 
�� ע�� 
***********************************************************/
void LCD_write_char(unsigned char c)
{
    unsigned char i; 
	 c-=0x20;                //ASCII���ȥ 0x20

    for (i=0; i<6; i++)
        LCD_write_byte(ASCII_6_8[c][i], 1);
}

/*-------------------------------------------
;����: Ӣ���ַ�����ʾ����
;������*s��Ӣ���ַ���ָ��
---------------------------------------------*/
void LCD_write_String(unsigned char X,unsigned char Y,unsigned char *s)
{
	LCD_set_XY(X,Y);
	while (*s) 	                 //��Ч*s!='\0'
	{
		LCD_write_char(*s);
		s++;
	}
} 
	

											


/*----------------------------------------------------
LCD_WRITE_ZH: ��ʾ16����*16���ߣ������к��ֵȰ����
���������zh����ʾ�ĺ������飻index ���ֶ�Ӧ���
---------------------------------------------------*/
void LCD_WRITE_ZH(unsigned char X, unsigned char Y,unsigned char  ZH[][32],unsigned char index) 
{
	unsigned char i;  	
	
	LCD_set_XY(X,Y);     //  �����ϲ���
	for(i=0; i<16;i++) 
	{ 
		LCD_write_byte(ZH[index][i],1); 
	}
	
    LCD_set_XY(X, Y+1);  // �����°벿��
	for(i=16; i<32;i++) 
	{
		LCD_write_byte(ZH[index][i],1);
	}	
}		

/*-----------------------------------------------------------------------
LCD_WRITE_ZH_STRING: ��LCD����ʾһ������

���������X��Y    ����ʾ���ֵ���ʼX��Y���ꣻ
           width �����ֵ���ı߳� 
          index   �����ֵ��������п�ʼ�ַ���������
		    num   ����index��ʼҪ��ʾ���ֵĸ����� 
          space   ��������ʾ���м��

���ԣ�			       X����  Y����	    ���ִ�	 ���ֱ߿�  ��������   ����    �м��       
	LCD_WRITE_ZH_STRING(0,      0,      hangzi,    16,        0,       5,      0);
-----------------------------------------------------------------------*/                        
void LCD_WRITE_ZH_STRING(uchar X,uchar Y,          // ��ʼ����
                              uchar ZHS[][32],     // �����ַ���
							  uchar width,         // ÿ���ַ��߳�   
							  uchar index,         // ��ʼ������
							  uchar num,		   // ��ʾ����
                   			  uchar space)         // �м��
  {
    unsigned char i,n;
    
    LCD_set_XY(X,Y);                          //���ó�ʼλ��
    
    for (i=0; i<num; i++)                      //��ʾnum������
      {
      	for (n=0; n<width; n++)                //�����ϰ벿��
      	{
		   LCD_write_byte(ZHS[index+i][n],1);
		}                    
	   
	    LCD_set_XY((X+(width+space)*i),Y+1);   //д���ֵ��°벿��
	    for (n=width; n<width*2; n++)             
      	{
		   LCD_write_byte(ZHS[index+i][n],1);
		}    
      	      
      	LCD_set_XY((X+(width+space)*(i+1)),Y); //��һ������λ��
      }
  }												


  
void LCD_Init()  
{  
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE );
  
  GPIO_InitStructure .GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7 ;
  GPIO_InitStructure .GPIO_Mode =GPIO_Mode_AF_PP ;
  GPIO_InitStructure .GPIO_Speed =GPIO_Speed_10MHz;        //SPI IO PIN INIT
  GPIO_Init(GPIOA,&GPIO_InitStructure );
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );
  SPI_InitStructure .SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;   //SPI INIT
  SPI_InitStructure .SPI_DataSize=SPI_DataSize_8b;
  SPI_InitStructure .SPI_CPHA=SPI_CPHA_2Edge;
  SPI_InitStructure .SPI_CPOL=SPI_CPOL_High;
  SPI_InitStructure .SPI_CRCPolynomial=SPI_CRCPR_CRCPOLY;
  SPI_InitStructure .SPI_Direction=SPI_Direction_1Line_Tx;
  SPI_InitStructure .SPI_FirstBit=SPI_FirstBit_MSB;
  SPI_InitStructure .SPI_Mode=SPI_Mode_Master;
  SPI_InitStructure .SPI_NSS=SPI_NSS_Soft;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE );
  

      GPIO_InitStructure .GPIO_Pin=LED_RS_PORT_PIN |LED_RES_PORT_PIN|LED_CS_PORT_PIN ;
      GPIO_InitStructure .GPIO_Mode =GPIO_Mode_Out_PP ;
      GPIO_InitStructure .GPIO_Speed =GPIO_Speed_10MHz;        //LCD PIN INIT
      GPIO_Init(GPIOA,&GPIO_InitStructure );
      GPIO_SetBits(LED_RS_PORT, LED_RS_PORT_PIN); //LCD_RS
      GPIO_ResetBits(LED_RES_PORT,LED_RES_PORT_PIN ); //LCD_RES
      GPIO_SetBits(LED_CS_PORT,LED_CS_PORT_PIN  ); //LCD_CS
      
   
   
    
    LCD_Reset();  
    GPIO_ResetBits (LED_CS_PORT,LED_CS_PORT_PIN);
 
    LCD_write_byte(0x21, 0);	// LCDģʽ����:оƬ���ˮƽѰַ��ʹ����չָ��
    LCD_write_byte(0xc8, 0);	// ����Һ��ƫ�õ�ѹ
    LCD_write_byte(0x06, 0);	// �¶�У��
    LCD_write_byte(0x13, 0);	// 1:48
    LCD_write_byte(0x20, 0);	// ʹ�û������V=0��ˮƽѰַ
    LCD_clear();	           // ����
    LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
    GPIO_SetBits (LED_CS_PORT,LED_CS_PORT_PIN);   //��ֹLCD
           
   
}

void Display()
{
  unsigned char t;
	unsigned char k;
	
	for(t=0;t<6;t++)
	{   
      LCD_set_XY(0,t);
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(deep[t*84+k],1);
				
		} 
	}
}

void GUI_Point2(unsigned char x,unsigned char y,unsigned char color)
{
      unsigned int y_Dyte,y_t;//�����е�ַ���ֽ�λ�������ֽ��е���1λ��0-127��
      unsigned int Dyte=0;//����Ϊ����������(ȡֵΪ0-63)���е�ַ(ȡֵΪ0~31)
      /***X,Y ���껥��������ͨ��X,Y ����***/
      
     // x_Dyte=y/8*128+x; //calc the byte in arry
     // y_Dyte=y%8; //bit in a byte

        y_t=y%8;
      LCD_set_XY(x,(y)/8);                          //���ó�ʼλ��
      if(color==1)
      Dyte|=(0x01<<y_t); //��λGDRAM ����8 λ��������Ӧ�ĵ�
      if(color==0)
      Dyte&=~(0x01<<y_t);//���GDRAM����8λ��������Ӧ�ĵ�
      if(color==2)
      Dyte^=(0x01<<y_t);//ȡ��GDRAM����8λ��������Ӧ�ĵ�
      LCD_write_byte(Dyte,1);
}
/*Function :Draw Point at X Y*/
void GUI_Point(unsigned char x,unsigned char y,unsigned char color)
{
      unsigned int y_Dyte,y_t;//�����е�ַ���ֽ�λ�������ֽ��е���1λ��0-127��
      unsigned int Dyte=0;//����Ϊ����������(ȡֵΪ0-63)���е�ַ(ȡֵΪ0~31)
      /***X,Y ���껥��������ͨ��X,Y ����***/
      
     // x_Dyte=y/8*128+x; //calc the byte in arry
     // y_Dyte=y%8; //bit in a byte
      
        y_Dyte=y/8;
        y_t=y%8;
      //LCD_set_XY(x,y/8);                          //���ó�ʼλ��
      if(color==1)
      deep[y/8*84+x]|=(0x01<<y_t); //��λGDRAM ����8 λ��������Ӧ�ĵ�
      if(color==0)
      deep[y/8*84+x]&=~(0x01<<y_t);//���GDRAM����8λ��������Ӧ�ĵ�
      if(color==2)
      deep[y/8*84+x]^=(0x01<<y_t);//ȡ��GDRAM����8λ��������Ӧ�ĵ�
      //LCD_write_byte(Dyte,1);
}

/******************��ˮƽ��*********************/
void LCD_Draw_Line_x( unsigned char X0, unsigned char X1, unsigned char Y, unsigned char Color )
{
  unsigned char Temp;
  if( X0 > X1 )
  {
    Temp = X1;
    X1 = X0;
    X0 = Temp;
  }
  for(;X0 <= X1;X0++ )
  GUI_Point( X0, Y, Color );
}
//*************����ֱ��  ��x��  �ӵ�y0�л�����y1��  ��ɫ color **************************/
void LCD_Draw_Line_Y( unsigned char X, unsigned char Y0, unsigned char Y1, unsigned char Color )
{
  unsigned char Temp;
  if( Y0 > Y1 )
  {
    Temp = Y1;
    Y1 = Y0;
    Y0 = Temp;
  }
  for(; Y0 <= Y1;Y0++)
  GUI_Point( X, Y0, Color);
}

/******************************************************************************
                   ���������㣨x0,y0���ͣ�x1,y1��֮���ֱ��
(x0,y0)��㣬(x1,y1)�յ�
����˵����dx��ʾֱ��X��Ĳ�ֵ������dy��ʾֱ��y��Ĳ�ֵ����
dx_sym��ʾx����������-1Ϊ��ֵ����+1Ϊ��ֵ����
dy_sym��ʾy����������-1Ϊ��ֵ����+1Ϊ��ֵ����
dx_x2��ʾdx * 2��dy_x2��ʾdy * 2�����ڼӿ������ٶȣ�
di��ʾ���߱���
*******************************************************************************/
char DrawLine(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color)
{
    int  dx,dy,dx_sym,dy_sym,dx_x2,dy_x2,di;
    if(x0 == x1)
    {      LCD_Draw_Line_Y(x0,y0,y1,color);
      return 1;
    }
    if(y0 == y1)
    {    LCD_Draw_Line_x(x0,x1,y0,color);
      return 1;
    }
    dx = x1 - x0;
    dy = y1 - y0;
    if(dx > 0)
    {
        dx_sym = 1;
    }
    else
    {   if(dx < 0)
        {
            dx_sym = -1;
        }
        else
        {   LCD_Draw_Line_Y(x0,y0,y1,color);           
            return 1;
        }
    }
    if(dy > 0)
    {
        dy_sym = 1;
    }
    else
    {   if(dy < 0)
        {
            dy_sym = -1;
        }
        else
        {         LCD_Draw_Line_x(x0,x1,y0,color);
            return 1;
        }
    }
    dx = dx * dx_sym;
    dy = dy * dy_sym;
    dx_x2 = dx * 2;
    dy_x2 = dy * 2;
    if(dx >= dy)
    {
        di = dy_x2 - dx;
        while(x0 != x1)
        {
           GUI_Point(x0,y0,color);
           x0 = x0 + dx_sym;
           if(di < 0)
           {
                di += dy_x2;
           }
           else
           {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
           }
        }
       GUI_Point(x0,y0,color);
    }
    else
    {
        di = dx_x2 - dy;
        while(y0 != y1)
        {
           GUI_Point(x0,y0,color);
           y0 = y0 + dy_sym;
           if(di < 0)
           {
                di = di + dx_x2;
           }
           else
           {
                di = di + dx_x2 - dy_x2;
                x0 = x0 + dx_sym;
           }
        }
        GUI_Point(x0,y0,color);
    }
   return 1;
}

/*******************************************
//  sin����

********************************************/
void fsin2()
{
  float x,y;
  unsigned char x1,y1;
  for(x=0;x<(4*3.14159);x+=0.1)
  {
    y=1*sin(3*x);
    x1=(int)(10*x);
    y1=(int)((10*y+0.5)+20);    //��yֵ������������
    GUI_Point(x1,y1,1);
  }
}
/****************************************
//        ��Բ
*****************************************/
void GUI_Circle(unsigned char x0,unsigned char y0,unsigned char r,unsigned char color)
{
  char a,b;
signed char di;
  if(r>31 ||r==0)
    return; //�������ˣ���Һ����ʾ�����Բ�뾶Ϊ31
  a=0;
  b=r;
  di=3-2*r; //�ж��¸���λ�õı�־
  while(a<=b)
  {
  GUI_Point(x0-b,y0-a,color); //3
  GUI_Point(x0+b,y0-a,color); //0
  GUI_Point(x0-a,y0+b,color); //1
  GUI_Point(x0-b,y0-a,color); //7
  GUI_Point(x0-a,y0-b,color); //2
  GUI_Point(x0+b,y0+a,color); //4
  GUI_Point(x0+a,y0-b,color); //5
  GUI_Point(x0+a,y0+b,color); //6
  GUI_Point(x0-b,y0+a,color);
  a++;
  /***ʹ��Bresenham �㷨��Բ**/
    if(di<0)
    di +=4*a+6;
      else
     {
      di +=10+4*(a-b);
      b--;
      }
  GUI_Point(x0+a,y0+b,color);
  }
}

/******************************************************************************
                            ��������
(x0,y0)�Ǿ������ϽǶ˵㣬(x1,y1)�Ǿ������½Ƕ˵㣬
color = 1��ʾ��color = 0����ʾ
*******************************************************************************/
char DrawFillRectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color)
{
    unsigned char temp;

    if(x0 > x1)
    {
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if(y0 > y1)
    {
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    if(y0 == y1)
    {   LCD_Draw_Line_x(x0,x1,y0,1);
    	
       return 0;
    }
    if(x0 == x1)
    {
      LCD_Draw_Line_Y(x0,y0,y1,1);

        return 0;
    }
    while(y0 <= y1)
    {
      LCD_Draw_Line_x(x0,x1,y0,1);
        y0++;
    }
    return 0;
}
/******************************************************************************
                            �����������
����˵����
x0:���������Ͻ�x����ֵ
y0:���������Ͻ�y����ֵ
width:�����εĿ�
color = 1��ʾ��color = 0����ʾ
*******************************************************************************/
char DrawFillSquare(unsigned char x0,unsigned char y0,unsigned char width,unsigned char color)
{
	
	if(width == 0) return 0;
    if((x0 + width) > 127) return 0;
    if((y0 + width) > 63) return 0;
    DrawFillRectangle(x0,y0,x0 + width,y0 + width,1);
    return 0;
}

/******************************************************************************
                            ������
(x0,y0)�Ǿ������ϽǶ˵㣬(x1,y1)�Ǿ������½Ƕ˵㣬
color = 1��ʾ��color = 0����ʾ
*******************************************************************************/
char DrawRectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color)
{
    unsigned char temp;

    if(x0 > x1)
    {
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if(y0 > y1)
    {
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    if(y0 == y1)
    {
      LCD_Draw_Line_x(x0,x1,y0,1);
       return 0;
    }
    if(x0 == x1)
    {
      LCD_Draw_Line_Y(x0,y0,y1,1);
        return 0;
    }
    LCD_Draw_Line_x(x0,x1,y0,1);
    LCD_Draw_Line_x(x0,x1,y1,1);
    LCD_Draw_Line_Y(x0,y0,y1,1);
    LCD_Draw_Line_Y(x1,y0,y1,1);
    return 0;
}


/******************************************************************************
                            ��������
����˵����
x0:���������Ͻ�x����ֵ
y0:���������Ͻ�y����ֵ
width:�����εĿ�
color = 1��ʾ��color = 0����ʾ
*******************************************************************************/
char DrawSquare(unsigned char x0,unsigned char y0,unsigned char width,unsigned char color)
{
	
	if(width == 0) return 0;
    if((x0 + width) > 127) return 0;
    if((y0 + width) > 63) return 0;
    DrawRectangle(x0,y0,x0 + width,y0 + width,1);
    return 0;
}

void mai()  
{    
  LCD_Init();   

//  delay_ms(100);  
  
  delay_ms(100);  
//  Display_fill(0x00);  
//  delay_ms(5);   ;        


  delay_ms(100);  
  DrawSquare(20,20,10,1);  
  GUI_Circle(64,32,20,1);
  fsin2();
 DrawLine(0,0,127,63,1);
  DrawLine(0,0,0,63,1);
 DrawLine(0,0,127,0,1);
  GUI_Point(0,0,0);
  GUI_Point(0,63,0);
  GUI_Point(127,63,0);
  GUI_Point(127,0,0);
  GUI_Point(64,32,0);
  Display();
  delay_ms(100);     
}   
