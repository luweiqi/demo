#include "ad7705.h"
u8 SPIx_ReadWriteByte(u8 TxData);       
/********TM7705初始化函数***********/
void TM7705_Init(void)
{         
        unsigned char i ;
        for(i = 0; i < 100; i++)
    {
        SPIx_ReadWriteByte(0xFF);      //持续DIN高电平写操作，恢复AD7705接口
    }
        SPIx_ReadWriteByte(0x20) ;                         //通道1 ,下一个写时钟寄存器
        Delay_ms(1);
        SPIx_ReadWriteByte(0x0c) ;                         //写时钟寄存器设置更新速率为200Hz
        Delay_ms(1);
        SPIx_ReadWriteByte(0x10) ;                         //通道1 ,下一个写设置寄存器
        Delay_ms(1);
        SPIx_ReadWriteByte(0x44) ;                         //写设置寄存器 ,设置成双极性、无缓冲、增益为0、滤波器工作、自校准
        Delay_ms(500);
        Delay_ms(500);
}

/********读16位数据************/
u16 ReadTM7705_16BitValue(void)
{
        unsigned long DataL = 0;
        unsigned long DataH = 0;
        unsigned long Ret = 0;

        DataH = SPIx_ReadWriteByte(0xFF);
    DataH = DataH << 8;
    DataL = SPIx_ReadWriteByte(0xFF);
        Ret          = DataH | DataL;

        return(Ret) ;                             
}

/********读取AD7705第一通道数据************/
u16 ReadTM7705(void)
{
    unsigned long Ret = 0;
        SPIx_ReadWriteByte(0x38) ;       //设置读当前通道数据
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10 )== 1)  //DRDY
        {
                ;
        }
    Ret = ReadTM7705_16BitValue();         
    return Ret;
}


void SPIx_Init(void)
        {         
        SPI_InitTypeDef  SPI_InitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2,ENABLE);
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE );
        /* Configure SPI1 pins: SCK, MISO and MOSI */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        /* Configure I/O for Flash Chip select */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //SPI CS
        GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  //复用推挽输出
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //DRDY
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//RST
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        
        /* Deselect the FLASH: Chip Select high */
        GPIO_SetBits(GPIOB,GPIO_Pin_11);
        GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//        Delay(1000000);
//        Delay(1000000);
        Delay_ms(500);
        Delay_ms(500);
        GPIO_SetBits(GPIOB,GPIO_Pin_12);
        /* SPI1 configuration */
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                //设置SPI工作模式:设置为主SPI
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                //设置SPI的数据大小:SPI发送接收8位帧结构
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                //选择了串行时钟的稳态:时钟悬空高
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //数据捕获于第二个时钟沿
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
        SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;// SPI_BaudRatePrescaler_64;                //定义波特率预分频的值:波特率预分频值为256
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
        SPI_InitStructure.SPI_CRCPolynomial = 7;            //CRC值计算的多项式
        SPI_Init(SPI2, &SPI_InitStructure);             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
        
        //SPI1->CR1|=1<<6; //SPI设备使能
        /* Enable SPI1  */
        SPI_Cmd(SPI2, ENABLE); //使能SPI外设
        
//         while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
//        /* Send byte through the SPI1 peripheral */
//        SPI_I2S_SendData(SPI2, 0x55); //通过外设SPIx发送一个数据
        
        SPIx_ReadWriteByte(0xff);//启动传输                 
}  
         

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{               
        GPIO_ResetBits(GPIOB,GPIO_Pin_11);
        while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
        /* Send byte through the SPI1 peripheral */
        SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
        /* Wait to receive a byte */
        while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志
        /* Return the byte read from the SPI bus */
        GPIO_SetBits(GPIOB,GPIO_Pin_11);
        return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据                                    
}
