/*******************************************************************************
 * @name    : ������PWM��������
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.1
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ��PB8����ΪPWM��������ڿ��Ʒ�����
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
 * TIM4_CH3 -> PB8 ���Ʒ�����
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.1
 * �������ݣ���Ϊλ������ģʽ
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "beep.h"
#include "timer.h"

//��ʱ��ʱ��ԴFck_cnt = 72000000/(1+psc)=1000000Hz
//��ʱ��Ƶ��(freq) = 72000000/((1+psc)*(arr+1)) = 1000000/2000=500Hz
//ռ�ձ�(duty) = compare/(arr+1) = 1000/2000 = 50%
uint16_t beep_arr = 1999;//������pwm������ֵ
uint16_t beep_psc = 71;//������pwm��Ԥ��Ƶֵ��
uint16_t beep_compare = 1000;//������pwm�ıȽ�ֵ������ռ�ձȣ�

/**
  * @brief ��ʼ��������
  * @param none
  * @retval none
  * @note
  *    ��TIM4_CH3��Ϊ������PWM����
  *    Ƶ�ʷ�Χ��: 1MHz~15Hz����ʼ����Ƶ����500Hz
	*		 ��ʼ����ռ�ձ���: 0%����˷�����������
  */
void BEEP_Init(void)
{
	TIM4_PWM_Init(beep_arr,beep_psc);
	BEEP_Off();
}

/**
  * @brief �رշ�����
  * @param none
  * @retval none
  * @note
  *    ��PWMռ�ձ�����Ϊ0%
  */
void BEEP_Off(void)
{
	BEEP_SetDuty(0);
}

/**
  * @brief �򿪷�����
  * @param none
  * @retval none
  * @note
  *   ��PWMƵ������Ϊ500Hz
	*		��ռ�ձ�����Ϊ50%
  */
void BEEP_On(void)
{
	BEEP_SetFreq(500);
	BEEP_SetDuty(50);
}

/**
  * @brief ���÷�����PWMƵ��
  * @param freq : ���õ�Ƶ�ʣ���λHz����Χ(15 ~ 65535)
  * @retval none
  * @note
  *    ��ʱ��ʱ��ԴƵ�� Fck_cnt = PCLK/(psc+1) = 1MHz
  *    Ƶ�� Freq = PCLK/((arr+1)*(psc+1))
  */
void BEEP_SetFreq(uint16_t freq)
{
	if(freq<15)return;//�����õ����Ƶ�ʱ������15
	
	beep_arr = 72000000/((1+beep_psc)*freq)-1;//����Ƶ�ʼ�������ֵARR
	beep_compare = (beep_arr+1)/2;	//��ռ�ձ�����Ϊ50%

	TIM4->ARR = beep_arr;//��������ֵ	
	TIM4->CCR3 = beep_compare;//���ñȽ�ֵ
}

/**
  * @brief ��ȡ��ǰ������PWMƵ��
  * @param none
  * @retval ��ǰPWM��Ƶ��
  * @note
  *    ��ʱ������Ƶ�� = PCLK / ( psc + 1 )
  *    ��ʱ���ж����� = ( arr + 1 )*( psc + 1) / PCLK
  */
uint16_t BEEP_GetFreq(void)
{
	uint16_t freq;
	freq = 72000000/((beep_arr+1)*(beep_psc+1));
	return freq;
}

/**
  * @brief ���÷�����PWMռ�ձ�
  * @param duty : ռ�ձȣ���50%�������Ϊ50
  * @retval none
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
void BEEP_SetDuty(uint8_t duty)
{
	beep_compare = duty*(beep_arr+1)/100;//����Ƚ�ֵ
	TIM4->CCR3 = beep_compare;//���±Ƚ�ֵ
}

/**
  * @brief ��ȡ��ǰ������PWMռ�ձ�
  * @param none
  * @retval ��ǰPWM��ռ�ձ�
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
uint8_t BEEP_GetDuty(void)
{
	uint16_t duty;
	duty = 100*beep_compare/(beep_arr+1);
	return duty;
}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

