/*******************************************************************************
 * @name    : 1602����ģ����ʾ�ײ�����ͷ�ļ�
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
#ifndef __LCM1602_H
#define __LCM1602_H

#include "stm32f10x.h"
#include "stdlib.h"
#include "bitband.h"

//OLEDģʽ����: 4�ߴ���ģʽ
		    						  
//-----------------OLED�˿ڶ���----------------
#define LCM_RS  PBout(9)	// ����/����ѡ��
#define LCM_RW  PBout(10)	// ��/дѡ��
#define LCM_E   PBout(11)	// ʹ���ź�
#define LCM_BK  PCout(13) // ������ƣ��ߵ�ƽ��

//PB0..7��Ϊ������
#define SET_DATA_OUT()	{GPIOB->CRL=0X33333333;}//����Ϊ�������
#define SET_DATA_IN()		{GPIOB->CRL=0X88888888;GPIOB->BSRR=0XFF;}//����Ϊ��������
#define DATAOUT(X) 			{GPIOB->ODR&=0xff00;GPIOB->ODR|=X;}
#define DATAIN					(uint8_t)(GPIOB->IDR&0x00ff)
 		     
#define LCM_CMD  0	//д����
#define LCM_DATA 1	//д����

void LCM1602_Init(void);
void LCM_WriteData(uint8_t data);
void LCM_WriteCmd(uint8_t cmd, uint8_t busy);
uint8_t LCM_ReadData(void);
void LCM_ReadStatus(void);
void LCM_SetXY(uint8_t x, uint8_t y);
void LCM_ShowChar(uint8_t X, uint8_t Y, uint8_t data);
void LCM_ShowString(uint8_t x, uint8_t y, uint8_t *str);
void LCM_ClrScreen(void);
void LCM_Off(void);
void LCM_On(void);

#endif  

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

