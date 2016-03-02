/*******************************************************************************
 * @name    : 12864����ģ����ʾ�ײ�����
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ͨ�����ڻ��п�����12864
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
 * ����ģʽ:
 * LCM_RS  -> PB9  : ����/����ѡ��
 * LCM_RW  -> PB10 : ��/дѡ��
 * LCM_E   -> PB11 : ʹ���ź�
 * LCM_D0  -> PB0  : ����
 * LCM_D1  -> PB1  : ����
 * LCM_D2  -> PB2  : ����
 * LCM_D3  -> PB3  : ����
 * LCM_D4  -> PB4  : ����
 * LCM_D5  -> PB5  : ����
 * LCM_D6  -> PB6  : ����
 * LCM_D7  -> PB7  : ����
 * LCM_BK  -> PC13 : ������ƣ��ߵ�ƽ��
 * LCM_RST -> PB12 : ��λ
 *
 * ���п�ģʽ:
 * LCM_CS  -> PB9  : ѡ��
 * LCM_SIF -> PB10 : ����
 * LCM_CLK -> PB11 : ʱ��
 * LCM_BK  -> PC13 : ������ƣ��ߵ�ƽ��
 * LCM_RST -> PB12 : ��λ
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "stdlib.h"
#include "delay.h"
#include "lcm12864.h"

const uint8_t AC_TABLE[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,      //��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,      //�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,      //�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f      //�����к���λ��
};

/**
  * @brief  12864 LCMģ���ʼ��
  * @param  None
  * @retval None
	* @note 
  */
void LCM12864_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	/* ʹ��GPIOB,GPIOC��AFIOʱ�� */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);

  /* ����STM32ΪSWD����ģʽ��ע��Ҫ�ȿ���AFIOʱ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWDģʽ
	
	/* PC13����Ϊ������������ڿ���LCM������ƣ�ע��PC13��֧��5V��ѹ���ٶ�ֻ�����õ�2MHz������ο�����飩 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��
	GPIO_SetBits(GPIOC, GPIO_Pin_13);//�򿪱���
#if LCM_MODE	
	/* ����PB0..7,PB9,PB10,PB11������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
	                              GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                              GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��ʼ��
  LCM_E=0;
	
#else
	/* ����PB0..7,PB9,PB10,PB11������� */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LCM_CLK=0;
#endif

	LCM_RST=0;	//��λ12864
  delay_ms(50);
	LCM_RST=1;
	delay_ms(30);

	LCM_WriteCmd(0x30);//�������ã�һ����8λ���ݣ�����ָ�
	LCM_WriteCmd(0x0C);//0000,1100  ������ʾ���α�off���α�λ��off
	LCM_WriteCmd(0x01);//0000,0001 ��DDRAM
	LCM_WriteCmd(0x02);//0000,0010 DDRAM��ַ��λ
	LCM_WriteCmd(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC//     
	LCM_WriteCmd(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ�� һ//   
	LCM_WriteCmd(0x0C);//��ʾ�趨������ʾ����ʾ��꣬��ǰ��ʾλ��������
}


#if LCM_MODE
/**
  * @brief  12864 LCMЩ����
  * @param  data : д�������
  * @retval None
	* @note 
  */
void LCM_WriteData(uint8_t data)
{
	SET_DATA_OUT();
	DATAOUT(data);
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 1;
	delay_us(5);
	LCM_E = 0;
}

/**
  * @brief  12864 LCMģ��дָ��
  * @param  cmd : д���ָ��
  * @retval None
	* @note 
  */
void LCM_WriteCmd(uint8_t cmd) 
{
	SET_DATA_OUT();
	DATAOUT(cmd);
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_E = 1;
	delay_us(5);
	LCM_E = 0;
}

/**
  * @brief  12864 LCM������
  * @param  None
  * @retval ����������
	* @note 
  */
uint8_t LCM_ReadData(void)
{
	uint8_t data;
	
	SET_DATA_IN();
	LCM_RS = 1;
	LCM_RW = 1;
	LCM_E = 1;
	delay_us(10);
	data=DATAIN;
	LCM_E = 0;
	return data;
}

#else
/**
  * @brief  ͨ������ģʽ��12864����1Byte����
  * @param  data : д�������
  * @retval None
	* @note 
  */
void LCM_SendByte(uint8_t data)
{
	uint8_t i=0;
	LCM_CS=1;
	for(i=0;i<8;i++)
	{
		if(data&0x80)LCM_SIF=1;//��λ�ȴ�
		else LCM_SIF=0;
		data<<=1;//����1λ
		LCM_CLK=1;
		LCM_CLK=0;
	}
	LCM_CS=0;
}

/**
  * @brief  ͨ������ģʽ��12864����1Byte����
  * @param  data : д�������
  * @retval None
	* @note 
  */
 void LCM_SPIWrite(uint8_t data,uint8_t mode)
 {
   LCM_SendByte(0XF8+(mode<<1));
   LCM_SendByte(data&0XF0);
   LCM_SendByte((data<<4)&0XF0);
 }

 /**
  * @brief  12864 LCMЩ����
  * @param  data : д�������
  * @retval None
	* @note 
  */
void LCM_WriteData(uint8_t data)
{
	LCM_SPIWrite(data,1);
}

/**
  * @brief  12864 LCMģ��дָ��
  * @param  cmd : д���ָ��
  * @retval None
	* @note 
  */
void LCM_WriteCmd(uint8_t cmd) 
{
  LCM_SPIWrite(cmd,0);
	delay_us(100);
}

#endif

/**
  * @brief  �ַ���ʼλ���趨
  * @param  row - �ڼ��� (0~3)
  * @param  col - �ڼ��� (0~7)
  * @retval None
	* @note 
  */
void LCM_SetCursor(uint8_t row, uint8_t col)
{ 
	if(row>3 || col>7)return;
	
	LCM_WriteCmd(0x30);	 //8BitMCU,����ָ���
	LCM_WriteCmd(AC_TABLE[8*row+col]);	//��ʼλ��
}

/**
  * @brief  ��ָ��λ����ʾһ���ַ�
  * @param  row - �ڼ��� (0~3)
  * @param  col - �ڼ��� (0~7)
  * @retval None
	* @note 
  */
void LCM_ShowChar(uint8_t row, uint8_t col, uint8_t ch)
{
	if(row>3 || col>7)return;
  LCM_SetCursor(row, col);
  LCM_WriteData(ch);
}

/**
  * @brief  ��ָ��λ�ÿ�ʼ��ʾ�ַ���
  * @param  row - �ڼ��� (0~3)
  * @param  col - �ڼ��� (0~7)
  * @param *str - �ַ���ָ��
  * @retval None
	* @note 
  */
void LCM_ShowString(uint8_t row, uint8_t col, uint8_t *str) 
{ 
	LCM_WriteCmd(0x30);	//8BitMCU,����ָ���
	LCM_WriteCmd(AC_TABLE[8*row+col]);      //��ʼλ��
	while(*str != '\0')	//�ж��ַ����Ƿ���ʾ���
	{
		if(col==8)				//�жϻ���
		{									//�����ж�,���Զ��ӵ�һ�е�������
			col=0;
			row++;
		}
		if(row==4) row=0;		//һ����ʾ��,�ص������Ͻ�
		LCM_WriteCmd(AC_TABLE[8*row+col]);
		LCM_WriteData(*str);	//һ������Ҫд����
		str++;
		LCM_WriteData(*str);
		str++;
		col++;
	}
}

/*----------------------------------------��ͼ-------------------------------------------*/
/**
  * @brief  ����16bit������
  * @param  data - �����͵�����
  * @retval None
	* @note 
  */
void LCM_SendInt(uint16_t data)
{
	LCM_WriteData(data>>8);
	LCM_WriteData(data);
}

/**
  * @brief  ��ָ��λ�û���
  * @param  X - �ڼ��� (0~127)
  * @param  Y - �ڼ��� (0~63)
  * @retval None
	* @note 
  */
void LCM_Gra(uint8_t X, uint8_t Y)
{
	uint8_t XX;
	uint8_t YY;
	LCM_WriteCmd(0x34);
	LCM_WriteCmd(0x36);
	XX=X/16;
	YY=63-Y;
	if(YY>=32){XX=XX+8;YY-=32;}
	LCM_WriteCmd(0x80+YY);
	LCM_WriteCmd(0x80+XX);
	LCM_SendInt(0x8000>>(X%16));
}

void LCM_ShowBMP(uint8_t *bmp)
{
	uint16_t x=0;
	uint8_t i,j;
	LCM_WriteCmd(0x34);	//8Bit����ָ�,��ʹ��36HҲҪд����
	LCM_WriteCmd(0x36);	//��ͼON,����ָ�����36H���ܿ���ͼ
	for(i=0;i<32;i++)		//12864ʵ��Ϊ256x32
	{
		LCM_WriteCmd(0x80|i);	//��λ��
		LCM_WriteCmd(0x80);		//��λ��
		for(j=0;j<32;j++)			//256/8=32 byte, ��λ��ÿ���Զ�����
		{
			LCM_WriteData(bmp[x]);
			x++;
		}
	}
}

void LCM_ClrBMP(void)
{
	uint8_t  i,j,x,y;
	
	LCM_WriteCmd(0x3e);// RE=1  ��չָ��ѡ��  G=1  ��ͼ����ʾ 
	x=0x80;
	y=0x80;
	for(j=0;j<64;j++)
	{
		y=0x80;
		LCM_WriteCmd(x);
		LCM_WriteCmd(y);
		for(i=0;i<32;i++)
		{
			LCM_WriteData(0x00);
      
		}
		x=x+1;
	}    
}

/*----------------------------------------����-------------------------------------------*/

/**
  * @brief  ����
  * @param  none
  * @retval none
	* @note 
  */
void LCM_ClrScreen(void)
{
	LCM_WriteCmd(0x30);	//8BitMCU,����ָ���
	LCM_WriteCmd(0x01);	//����
	delay_us(10);
}

/**
  * @brief  �ر���ʾ�ͱ���
  * @param  None
  * @retval None
	* @note 
  */
void LCM_Off(void)
{
	LCM_BK=0;//�ر���
	LCM_WriteCmd(0x08);//����ʾ
}

/**
  * @brief  ����ʾ�ͱ���
  * @param  None
  * @retval None
	* @note 
  */
void LCM_On(void)
{
	LCM_WriteCmd(0x0C);//����ʾ
	LCM_BK=1;//�ر���
}


/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

