#include "ad7705.h"
u8 SPIx_ReadWriteByte(u8 TxData);       
/********TM7705��ʼ������***********/
void TM7705_Init(void)
{         
        unsigned char i ;
        for(i = 0; i < 100; i++)
    {
        SPIx_ReadWriteByte(0xFF);      //����DIN�ߵ�ƽд�������ָ�AD7705�ӿ�
    }
        SPIx_ReadWriteByte(0x20) ;                         //ͨ��1 ,��һ��дʱ�ӼĴ���
        Delay_ms(1);
        SPIx_ReadWriteByte(0x0c) ;                         //дʱ�ӼĴ������ø�������Ϊ200Hz
        Delay_ms(1);
        SPIx_ReadWriteByte(0x10) ;                         //ͨ��1 ,��һ��д���üĴ���
        Delay_ms(1);
        SPIx_ReadWriteByte(0x44) ;                         //д���üĴ��� ,���ó�˫���ԡ��޻��塢����Ϊ0���˲�����������У׼
        Delay_ms(500);
        Delay_ms(500);
}

/********��16λ����************/
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

/********��ȡAD7705��һͨ������************/
u16 ReadTM7705(void)
{
    unsigned long Ret = 0;
        SPIx_ReadWriteByte(0x38) ;       //���ö���ǰͨ������
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        /* Configure I/O for Flash Chip select */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //SPI CS
        GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  //�����������
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
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                //����SPI����ģʽ:����Ϊ��SPI
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //���ݲ����ڵڶ���ʱ����
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
        SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;// SPI_BaudRatePrescaler_64;                //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
        SPI_InitStructure.SPI_CRCPolynomial = 7;            //CRCֵ����Ķ���ʽ
        SPI_Init(SPI2, &SPI_InitStructure);             //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
        
        //SPI1->CR1|=1<<6; //SPI�豸ʹ��
        /* Enable SPI1  */
        SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
        
//         while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//        /* Send byte through the SPI1 peripheral */
//        SPI_I2S_SendData(SPI2, 0x55); //ͨ������SPIx����һ������
        
        SPIx_ReadWriteByte(0xff);//��������                 
}  
         

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{               
        GPIO_ResetBits(GPIOB,GPIO_Pin_11);
        while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
        /* Send byte through the SPI1 peripheral */
        SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
        /* Wait to receive a byte */
        while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־
        /* Return the byte read from the SPI bus */
        GPIO_SetBits(GPIOB,GPIO_Pin_11);
        return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����                                    
}
