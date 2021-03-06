/*******************************************************************************
 * @name    : I2C通信实验
 * @author  : 布谷鸟
 * @web     : WWW.UCORTEX.COM
 * @version : V1.2
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : 实现了通过STM32通用IO口模拟I2C通信的底层函数
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX版权所有，违者必究！例程源码仅供大家参考，旨在交流学习和节省开发时间，
 * 对于因为参考本文件内容，导致您的产品直接或间接受到破坏，或涉及到法律问题，作
 * 者不承担任何责任。对于使用过程中发现的问题，如果您能在WWW.UCORTEX.COM网站反
 * 馈给我们，我们会非常感谢，并将对问题及时研究并作出改善。例程的版本更新，将不
 * 做特别通知，请您自行到WWW.UCORTEX.COM下载最新版本，谢谢。
 * 对于以上声明，UCORTEX保留进一步解释的权利！
 * ----------------------------------------------------------------------------
 * @description
 * 例程介绍：
 * 1.1 系统上电后，初始化I2C端口PB10(SCL)和PB11(SDA)。
 * 1.2 JoyStick按键KEY_UP实现波特率增加(最高到400Kbps)，KEY_DOWN实现波特率的减少，
 *     KEY_CENTER实现对I2C从设备MPU6050的一次读写访问。
 * 1.3 OLED提示当前I2C的速度(默认是100Kbps)，I2C读取到的数值，以及I2C访问是否成功。
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * 更改时间：2014-04-06    更改人：布谷鸟
 * 版本记录：V1.0
 * 更改内容：新建
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
	buffer[4] = 0x33;
//	buffer[4] = 0x11;
	buffer[5] = 0x00;
	//B
	buffer[6] = 0x0C;
	buffer[7] = 0x80;
	buffer[8] = 0x33;
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
	buffer_ds[3]=0x07;
	buffer_ds[4]=0x00;
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

	LED_Init();				//LED IO初始化
	Delay_Init();			//延时初始化
	COM_Init(COM1, 115200);//串口1初始化
	/*
	OLED_Init();			//初始化OLED
	ADC_Initialize();	//ADC初始化
	JoyStick_Init();	//摇杆按键(JoyStick)初始化
	BEEP_Init();			//蜂鸣器初始化
  EE_Init();				//FLASH 模拟EEPROM初始化
	*/
	I2C_Init();				//初始化I2C接口
/*	
	//显示提示信息
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
			CS_low();	
	
 	while(1){}
	
/*
		while(0)
	{
		key = JoyStick_Scan(0);
		if(key == KEY_CENTER)//进行一次I2C读写
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
			LED=!LED;//工作状态指示
		}
	}
	*/
	
}


/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
