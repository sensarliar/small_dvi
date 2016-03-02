/*******************************************************************************
 * @name    : 1602����ģ����ʾ�ײ�����
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ͨ����������
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
#include "lcm1602.h"

/**
  * @brief  1602 LCMģ���ʼ��
  * @param  None
  * @retval None
	* @note 
  */
void LCM1602_Init(void)
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
	
	/* ����PB0..7,PB9,PB10,PB11������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
	                              GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
	                              GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��ʼ��
 
  LCM_E=0;
  
  delay_ms(15);
	LCM_WriteCmd(0x38,0); //������ʾģʽ���ã������æ�ź�
	delay_ms(5);
	LCM_WriteCmd(0x38,0);
	delay_ms(5);
	LCM_WriteCmd(0x38,0);
	delay_ms(5);

	LCM_WriteCmd(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	LCM_WriteCmd(0x08,1); //�ر���ʾ,�򿪹����˸
	LCM_WriteCmd(0x01,1); //��ʾ����
	LCM_WriteCmd(0x06,1); // ��ʾ����ƶ�����
	LCM_WriteCmd(0x0C,1); // ��ʾ����������ã�����ʾ���޹��
}

/**
  * @brief  1602 LCMЩ����
  * @param  data : д�������
  * @retval None
	* @note 
  */
void LCM_WriteData(uint8_t data)
{
	LCM_ReadStatus();	//���æ
	SET_DATA_OUT();
	DATAOUT(data);
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 1;
	delay_us(100);
	LCM_E = 0;
}

/**
  * @brief  1602 LCMģ��дָ��
  * @param  cmd : д���ָ��
  * @param  cmd : 0 - ����æ���  1 - ��æ���
  * @retval None
	* @note 
  */
void LCM_WriteCmd(uint8_t cmd, uint8_t busy) 
{
  if (busy) LCM_ReadStatus(); //æ���
	SET_DATA_OUT();
	DATAOUT(cmd);
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_E = 1;
	delay_us(100);
	LCM_E = 0;
}

/**
  * @brief  1602 LCM������
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
	delay_us(100);
	data=DATAIN;
	LCM_E = 0;
	return data;
}

/**
  * @brief  1602 LCMģ��״̬���
  * @param  None
  * @retval None
	* @note 
  */
void LCM_ReadStatus(void)
{
	SET_DATA_IN();
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 1;
	delay_us(100);
	while (DATAIN & 0X80); //���æ�ź�
	LCM_E = 0;
}

/**
  * @brief  �ַ���ʼλ���趨
  * @param  x - �ڼ��� (0~15)
  * @param  y - �ڼ��� (0~1)
  * @retval None
	* @note 
  */
void LCM_SetXY(uint8_t x, uint8_t y)
{ 
  y &= 0X1;//yΪ�ڼ��У�y���ܴ���1
  x &= 0XF;//xΪ�ڼ��У�x���ܴ���15
  if(y==0)     x+=0X80; 
  else if(y==1)x+=0XC0; 
  LCM_WriteCmd(x,1);
}

/**
  * @brief  ��ָ��λ����ʾһ���ַ�
  * @param  x - �ڼ��� (0~15)
  * @param  y - �ڼ��� (0~1)
  * @retval None
	* @note 
  */
void LCM_ShowChar(uint8_t x, uint8_t y, uint8_t ch)
{
  LCM_SetXY(x, y);
  LCM_WriteData(ch);
}

/**
  * @brief  ��ָ��λ�ÿ�ʼ��ʾ�ַ���
  * @param  x - �ڼ��� (0~15)
  * @param  y - �ڼ��� (0~1)
  * @param *str - �ַ���ָ��
  * @retval None
	* @note 
  */
void LCM_ShowString(uint8_t x, uint8_t y, uint8_t *str) 
{ 
  LCM_SetXY(x,y); 
	while(*str) 
  { 
    LCM_WriteData(*str); 
    str++; 
  }
}

/**
  * @brief  ����
  * @param  none
  * @retval none
	* @note 
  */
void LCM_ClrScreen(void)
{
	LCM_WriteCmd(0x01,1); //��ʾ����
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
	LCM_WriteCmd(0x08,1);//����ʾ
}

/**
  * @brief  ����ʾ�ͱ���
  * @param  None
  * @retval None
	* @note 
  */
void LCM_On(void)
{
	LCM_WriteCmd(0x0C,1);//����ʾ
	LCM_BK=1;//�ر���
}


/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

