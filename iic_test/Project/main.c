/*******************************************************************************
 * @name    : I2Cͨ��ʵ��
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.2
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ʵ����ͨ��STM32ͨ��IO��ģ��I2Cͨ�ŵĵײ㺯��
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
 * ���̽��ܣ�
 * 1.1 ϵͳ�ϵ�󣬳�ʼ��I2C�˿�PB10(SCL)��PB11(SDA)��
 * 1.2 JoyStick����KEY_UPʵ�ֲ���������(��ߵ�400Kbps)��KEY_DOWNʵ�ֲ����ʵļ��٣�
 *     KEY_CENTERʵ�ֶ�I2C���豸MPU6050��һ�ζ�д���ʡ�
 * 1.3 OLED��ʾ��ǰI2C���ٶ�(Ĭ����100Kbps)��I2C��ȡ������ֵ���Լ�I2C�����Ƿ�ɹ���
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "oled.h"
#include "exti.h"
#include "adc.h"
#include "joystick.h"
#include "timer.h"
#include "rtc.h"
#include "standby.h"
#include "dma.h"
#include "beep.h"
#include "stm32_eeprom.h"
#include "spi.h"
#include "nrf24l01.h"
#include "i2c.h"
#include "mpu6050.h"

#define DVI_REG_ADDR 0x00
//#define DVI_DevAddr 0x08
#define DVI_Dev2Addr 0x00
#define DVI_DS16Addr 0xAC


void modify_buff(uint8_t* buffer)
{
	buffer[0] = 0x0;
	//buffer[1] = 0x80;
	buffer[1] = 0xC0;
		//A
//	buffer[2] = 0x0C;
//	buffer[2] = 0x04;
	buffer[2] = 0x0C;
//	buffer[3] = 0x0D;
	buffer[3] = 0x80;//0f
//	buffer[4] = 0x00;
	buffer[4] = 0x10;
//	buffer[4] = 0x11;
	buffer[5] = 0x00;
	//B
	buffer[6] = 0x0C;
	buffer[7] = 0x80;
	buffer[8] = 0x10;
	buffer[9] = 0x00;
	//C
	buffer[10] = 0xF5;
	buffer[11] = 0x00;
	buffer[12] = 0x10;
	buffer[13] = 0x00;

	buffer[14] = 0x00;
	buffer[15] = 0x01;
	buffer[16] = 0x00;
	buffer[17] = 0x00;
	
	buffer[18] = 0x0F;
	
}


void modify_ds_buffer(uint8_t* buffer_ds)
{
	buffer_ds[3]=0x47;
	buffer_ds[4]=0x44;
	buffer_ds[7]=0x00;
	buffer_ds[8]=0x70;
	
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t t=0;
	uint8_t data = 0;
	uint8_t key = 0;
	uint16_t speed = 100;
	uint8_t i = 0;
	uint8_t flag =0;
	
	uint8_t buffer[19];
		uint8_t ds16_buffer[9];

	LED_Init();				//LED IO��ʼ��
	Delay_Init();			//��ʱ��ʼ��
	COM_Init(COM1, 115200);//����1��ʼ��
	/*
	OLED_Init();			//��ʼ��OLED
	ADC_Initialize();	//ADC��ʼ��
	JoyStick_Init();	//ҡ�˰���(JoyStick)��ʼ��
	BEEP_Init();			//��������ʼ��
  EE_Init();				//FLASH ģ��EEPROM��ʼ��
	*/
	I2C_Init();				//��ʼ��I2C�ӿ�
/*	
	//��ʾ��ʾ��Ϣ
	OLED_ShowString(0,0,"WWW.UCORTEX.COM");	
	OLED_ShowString(0,16,"SPEED: 100 Kbps");
	OLED_ShowString(0,32,"DATA = 0x  ");
	OLED_ShowString(0,48,"RESULT: ");
	OLED_Refresh_Gram();
	*/

	for(i = 0; i < 19; i++)
	{
			I2C_ReadOneByte(DVI_Dev2Addr, DVI_REG_ADDR+i, &buffer[i]);
	}
	//		I2C_ReadOneByte(DVI_DevAddr, DVI_REG_ADDR+0x12, &buffer[11]);
		//	I2C_ReadOneByte(DVI_DevAddr, DVI_REG_ADDR+0x6, &buffer[12]);
			modify_buff(buffer);
	
	for(i = 0; i < 19; i++)
	{
			I2C_WriteOneByte(DVI_Dev2Addr, DVI_REG_ADDR+i, buffer[i]);
	}
	
	
		CS_high();
	
		for(i = 0; i < 9; i++)
	{
			I2C_ReadOneByte(DVI_DS16Addr, DVI_REG_ADDR+i, &ds16_buffer[i]);
	}
			CS_low();	
	modify_ds_buffer(ds16_buffer);
	
			CS_high();
			for(i = 3; i < 9; i++)
	{
			I2C_WriteOneByte(DVI_DS16Addr, DVI_REG_ADDR+i, ds16_buffer[i]);
	}
	
			CS_low();
	
			CS_high();
	
		for(i = 0; i < 9; i++)
	{
			I2C_ReadOneByte(DVI_DS16Addr, DVI_REG_ADDR+i, &ds16_buffer[i]);
	}
//			CS_low();	
	
 	while(1){}
	
/*
		while(0)
	{
		key = JoyStick_Scan(0);
		if(key == KEY_CENTER)//����һ��I2C��д
		{

	//		I2C_ReadBurst(DVI_DevAddr, DVI_REG_ADDR+0x2, buffer, 5);
//			data = MPU6050_getDeviceID();

	if(flag == 1)
	{
		buffer[1] = 0x80;
		flag = 0;
	}
	else
	{
		buffer[1] = 0x8F;
		flag = 1;
	}
		
	I2C_WriteOneByte(DVI_DevAddr, DVI_REG_ADDR+0x1, buffer[1]);
	
	
			I2C_WriteBurst(DVI_DevAddr, DVI_REG_ADDR, buffer, 5);
			I2C_WriteBurst(DVI_DevAddr, DVI_REG_ADDR+0x06, buffer, 3);
			I2C_WriteBurst(DVI_DevAddr, DVI_REG_ADDR+0x0A, buffer, 3);
			I2C_WriteBurst(DVI_DevAddr, DVI_REG_ADDR+0x10, buffer, 3);
			
			OLED_ShowxNum(72,32,buffer[3],2,16,0x13);
		//	OLED_ShowxNum(72,32,data,2,16,0x13);
			if(data == 0x68)
				OLED_ShowString(64,48,"SUCCESS");
			else
				OLED_ShowString(64,48,"ERROR  ");
		}

		
		OLED_Refresh_Gram();
		delay_ms(50);
		if(t++>5)
		{
			t=0;
			LED=!LED;//����״ָ̬ʾ
		}
	}
	*/
	
}


/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
