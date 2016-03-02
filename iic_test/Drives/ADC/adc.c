/*******************************************************************************
 * @name    : ADC�ײ�����
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.2
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����PA0Ϊģ������˿ڣ�ʹ���ڲ�STM32�ڲ��¶ȴ�����
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
 * PA0 -> ADC1_CH0
 * TEMP-> ADC1_CH16
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.1
 * �������ݣ���ADC_Initialize������˿���STM32�ڲ��¶ȴ�������
 * ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����������Ҫ����ʱ��ע�͵�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-07    �����ˣ�������
 * �汾��¼��V1.2
 * �������ݣ������ADC DMA������ʽ��void ADC1_DMAInitialize(void)
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "adc.h"
#include "delay.h"
#if ADC_DMA_TRANSFER
#include "dma.h"
__IO uint16_t ADC_Buffer[2];
#endif

/**
  * @brief ADC��ʼ��
	* @param none
  * @retval none
  * @note  ��ʼ��PA.00ΪADC1_CH1������ת�����������ADCת��
  */ 
void ADC_Initialize(void)
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
	
	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����������Ҫ����ʱ��ע�͵���һ��
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);	//��λADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼���� 
}

/**
  * @brief ���ADCֵ
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:ͨ��ֵ ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADCת��ֵ
  * @note 
  */
uint16_t Get_Adc(ADC_TypeDef* ADCx, uint8_t ch)   
{
	/* ADC�Ĺ���ͨ�������ã�һ�����У�����ʱ��239.5����	*/
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//��ʼת��
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));	//�ȴ�ת������
	return ADC_GetConversionValue(ADCx);						//����ADC1ת�����
}

/**
  * @brief ��ȡ����10��ADC������ƽ��ֵ
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:ͨ��ֵ ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADCת����ƽ��ֵ
  * @note 
  */
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx, uint8_t ch)
{
	uint32_t adc_val = 0;
	uint8_t i;
	
	for(i = 0; i < 10; i++)
	{
		adc_val += Get_Adc(ADCx, ch);
		delay_ms(5);
	}
	return adc_val/10;
}

#if ADC_DMA_TRANSFER
/**
  * @brief ADC1 DMA��ʼ��
	* @param none
  * @retval none
  * @note  ��ʼ��PA.00(ADC1_CH1),	STM32�ڲ��¶ȴ�����(ADC1_CH16)������ת����DMA����
  */ 
void ADC1_DMAInitialize(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��GPIOA��ADC1��DMA1ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

  /* ADC1_IN0 GPIO (PA.00) ���� --------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//���� PA.00 (ADC1_IN0) ��Ϊģ����������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* ADC1 ���� ------------------------------------------------------*/
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADCCLK��Ƶ���ӣ�72MHz/6 = 12MHz 
	
	ADC_DeInit(ADC1);  //��ADC1��Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;				//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������ADCת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//����ת��ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ʼ��ADC

  ADC_TempSensorVrefintCmd(ENABLE);//����ADC�����¶ȴ�����CH16
	
	//����ת������1��ADC1_CH1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	//����ת������2��ADC1_CH16���ڲ��¶ȴ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ADC1

	ADC_ResetCalibration(ADC1);	//��λADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)){}	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1)){}//�ȴ�У׼���� 
		
  /*    DMA������Ϣ
	      -------------
				- DMAͨ��: DMA1_Channel1
        - �������ݼĴ�����ַ: ADC1_DR_Address
        - ��������ַ: ADC_Buffer
				- �����С: 2
        - ��������: 1 -> 16bit
        - DMA����: ADC���ݼĴ���->ADC_Buffer
        - DMA����ģʽ: ѭ������
  */
		
	DMA_Polling_Init(DMA1, DMA1_Channel1,
									(uint32_t)ADC1_DR_Address,(uint32_t)ADC_Buffer,
									2,1,
									DMA_DIR_PeripheralSRC,DMA_Mode_Circular);
	
	ADC_DMACmd(ADC1, ENABLE);//����ADC��DMA֧��
	DMA_Cmd(DMA1_Channel1,ENABLE);//����DMA Channel1
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ADC�Զ�����ת��������Զ�������ADC_Value
}

#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
