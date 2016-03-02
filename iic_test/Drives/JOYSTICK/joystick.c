 /*******************************************************************************
 * @name    : JoyStickҡ�˰�������
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����һ��ͨ��ģ���ѹ����ⰴ���ķ���
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
 * ��ʵ��ʵ�ֹ��ܣ�
 * ����JoyStick�Ĳ�ͬ���������������У������ݵ����ѹԭ�����ڹ����ˣ����ӵ�STM32��
 * PA.00��������ͬ�ĵ�ѹ��ͨ�����PA.00�������ĵ�ѹֵ�����ǾͿ����ж����ĸ��������£�
 * �Ӷ�ʵ�ְ���ɨ�蹦�ܡ����ַ�ʽʹ�����ǿ���ͨ��һ��GPIO��ʵ��5������ֵ��ɨ�衣
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
 
#include "joystick.h"
#include "delay.h"

/**
  * @brief ҡ�˰�����JoyStick����ʼ��
	* @param none
  * @retval none
  * @note  ��ʼ��PA.00ΪADC1_CH1������ת�����������ADCת��
  */
void JoyStick_Init(void)
{ 
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��GPIOA��ADC1,AFIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	
	/* ����ADCCLK��Ƶ���� ADCCLK = PCLK2/6���� 72MHz/6 = 12MHz */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  /* ���� PA.00 (ADC1_IN0) ��Ϊģ���������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //��ADC1��Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������ADCת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//����ת��ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ʼ��ADC

	/* ADC�Ĺ���ͨ�������ã�һ�����У�����ʱ��239.5����	*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);	//��λADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

/**
  * @brief ҡ�˰�����JoyStick��������
	* @param none
  * @retval ����ֵ: KEY_NONE,KEY_CENTER,KEY_UP,KEY_LEFT,KEY_DOWN,KEY_RIGHT
  * @note
  *    �˺�������Ӧ���ȼ�,KEY_CENTER>KEY_RIGHT>KEY_DOWN>KEY_LEFT>KEY_UP!!
  */
uint8_t JoyStick_Scan(uint8_t polling)
{
	uint16_t AdcValue;
	static uint8_t key_release = 1;//�������ɿ���־

  /* ����ADCת�� */	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//��ʼת��
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������
	AdcValue = ADC_GetConversionValue(ADC1);				//����ADC1ת�����
	
	if(polling)key_release = 1;	//֧������
	
	if(key_release && (AdcValue >= 0xb4))//��⵽��������
	{
		key_release = 0;
		if(AdcValue>=0xcba)return KEY_CENTER;
		else if(AdcValue>=0x973)return KEY_RIGHT;
		else if(AdcValue>=0x686)return KEY_DOWN;
		else if(AdcValue>=0x354)return KEY_LEFT;
		else return KEY_UP;
	}
	
	if(AdcValue < 0xb4) key_release = 1;//δ��⵽��������
	
 	return 0;// �ް�������
}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
