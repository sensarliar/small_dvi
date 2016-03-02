/*******************************************************************************
 * @name    : SPI����
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : SPI�����ļ�
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX��Ȩ���У�Υ�߱ؾ�������Դ�������Ҳο���ּ�ڽ���ѧϰ�ͽ�ʡ����ʱ�䣬
 * ������Ϊ�ο����ļ����ݣ��������Ĳ�Ʒֱ�ӻ����ܵ��ƻ������漰���������⣬��
 * �߲��е��κ����Ρ�����ʹ�ù����з��ֵ����⣬���������WWW.UCORTEX.COM��վ��
 * �������ǣ����ǻ�ǳ���л�����������⼰ʱ�о����������ơ����̵İ汾���£�����
 * ���ر�֪ͨ���������е�WWW.UCORTEX.COM�������°汾��лл��
 * ��������������UCORTEX������һ�����͵�Ȩ����
 * ----------------------------------------------------------------------------
 * @description
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
 
#include "spi.h" 


/**
  * @brief  ��ʼ��SPI1 GPIO
  * @param  None
  * @retval None
	* @nete		
	*		SPI1_CLK  : PA5
	*		SPI1_MISO : PA6
	*		SPI1_MOSI : PA7
  */
void SPI1_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��SPI1 GPIOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	//���� SPI1_CLK(PA5), SPI1_MOSI(PA7)Ϊ���������������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//���������������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��������������Ϣ��ʼ��

  //���� SPI1_MISO(PA6)Ϊ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��������������Ϣ��ʼ��
}

/**
  * @brief SPI1��ʼ��
  * @param None
  * @retval None
  * @note 
	*		SPI1�ҽ���APB2�������ߣ������������Ƶ��Ϊ72MHz����ʼ�������У���������SPI
	*		������Ԥ��Ƶ��8����˲����� = 72MHz/16 = 4MHz���ر�ע��SPI2�ǹҽ���APB1����
	*		���ߵģ�APB1������������36MHz�����ͬ����Ԥ��Ƶ�õ��Ĳ����ʱ�Ϊ2MHz��
  */
void SPI1_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //ʹ��SPI1����ʼ��
	
  SPI1_GPIO_Init();// SPI1 GPIO��ʼ��

  //SPI1��ʼ��
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��ģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//����ΪSPI���豸
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//����Ϊ8bitλ������֡
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//SPIʱ���źſ���״̬Ϊ�ߵ�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			//SPIʱ�ӵڶ�������(������/�½���)��������
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;					//�������ѡͨ�ܽ�(Ӳ����������SPI��Ӧ��NSS�ܽ�)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//������Ԥ��Ƶ,PCLK2/8=9MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ�MSB��ʼ����
  SPI_InitStructure.SPI_CRCPolynomial = 7;					//����CRCֵ�����õĶ���ʽ
  SPI_Init(SPI1, &SPI_InitStructure);								//�������ϲ�����ʼ��SPI1

  SPI_Cmd(SPI1, ENABLE);	//����SPI1
}

/**
  * @brief  SPI��д������д����
  * @param  SPIx: ѡ��SPI�˿ڣ�SPI1��SPI2...
	* @param	data: �����͵����ݣ�������������ǣ�����һ����Ч����(dummy data)
	* @param	timeout: �ȴ���ʱʱ��
  * @retval ���յ�������
  */
uint8_t SPI_RW(SPI_TypeDef* SPIx, uint8_t data, uint32_t* timeout)
{
  //�ȴ����ݼĴ���Ϊ��
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		//ֻҪ����timeout=0���ҷ���ֵΪ0XFF����˵�����䳬ʱ
		*timeout -= 1;
		if(*timeout == 0) return 0XFF;
	}
  SPI_I2S_SendData(SPIx, data);//��SPIдһ������
	

  //�ȴ���SPI��һ������
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		//ֻҪ����timeout=0���ҷ���ֵΪ0XFF����˵�����䳬ʱ
		*timeout -= 1;
		if(*timeout == 0) return 0XFF;
	}
  return SPI_I2S_ReceiveData(SPIx);//���ض���������
}


/**
  * @brief  ����SPI������
  * @param  SPIx: ѡ��SPI�˿ڣ�SPI1��SPI2...
	* @param	SPI_BaudRatePrescaler: ������Ԥ��Ƶ
  *		@arg - SPI_BaudRatePrescaler_2   2��Ƶ   
  *		@arg - SPI_BaudRatePrescaler_8   8��Ƶ   
  *		@arg - SPI_BaudRatePrescaler_16  16��Ƶ  
  *		@arg - SPI_BaudRatePrescaler_256 256��Ƶ 
  * @retval none
	* @note: ������ =  PCLK / SPI_BaudRatePrescaler
  */
void SPI_Set_BaudRate(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler)
{
	uint16_t tmpreg;
	
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	
	tmpreg = SPIx->CR1;
	tmpreg &= 0XFFC7;	//���BRλ
	tmpreg |= (uint16_t)SPI_BaudRatePrescaler; //���ò�����
	SPIx->CR1 = tmpreg;
	
//	SPI_Cmd(SPIx,ENABLE); 

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
