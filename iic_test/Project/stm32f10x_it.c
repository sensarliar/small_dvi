/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include "usart.h"
#include "led.h"
#include "rtc.h"
#include "timer.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  实时时钟RTC秒中断服务程序
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);//清除RTC秒中断标志位
    LED=!LED;							//LED翻转
    TimeDisplay = 1;			//时间更新标志置1
    RTC_WaitForLastTask();//等待RTC寄存器操作完成 
  }
}

/**
  * @brief  EXTI0中断服务程序
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		LED=!LED;
    EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI line0的中断标志
  }
}

/**
  * @brief  USART1串口中断
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint8_t data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//接收中断
	{
		data = USART_ReceiveData(USART1);			//读取接收到的数据
		if((COM_RX_STA & 0x8000) == 0)				//接收未完成
		{
			if(COM_RX_STA & 0x4000)							//接收到了回车键
			{
				if(data != '\n') COM_RX_STA = 0;	//没接收到换行键，接收错误,重新开始
				else COM_RX_STA	|=	0x8000;				//接收到换行键，接收完成了 
			}
			else //还没收到0x0d
			{	
				if(data == '\r')	COM_RX_STA	|=	0x4000;//接收到回车键
				else
				{
					COM_RX_BUF[COM_RX_STA & 0x3fff]	=	data ;
					COM_RX_STA++;
					if(COM_RX_STA	>	(COM_RX_LEN - 1))	COM_RX_STA = 0;//缓冲溢出
				}		 
			}
		}
	}
}

/**
  * @brief  USART2串口中断
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	uint8_t Res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//接收中断
	{
		Res = USART_ReceiveData(USART2);	//读取接收到的数据
		USART_SendData(USART2, Res);			//将接收到的数据发送出去  		 
	}
}

/**
  * @brief  USART3串口中断
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	uint8_t Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//接收中断
	{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		USART_SendData(USART3, Res);			//将接收到的数据发送出去  		 
	}
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //发生了更新中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除更新中断标志
		if(CaptureStatus&0X4000)//捕获到上升沿
		{
			if((CaptureStatus&0X0FFF)>=0X0FFF)//溢出，捕获的时间太长了
			{
				CaptureStatus|=0X2000;//设置溢出标志
			}
			else
			{
				CaptureStatus++;//更新事件次数累加
			}
		}
	}
	
  if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);//清除捕获比较中断标志
		
		if(CaptureStatus&0X8000)//上次捕获值未被使用，丢弃掉上次捕获的值
		{
			CaptureStatus = 0;
			CaptureValue1 = 0;
			CaptureValue2 = 0;
		}
		
    if((CaptureStatus&0X4000) == 0)//捕捉到上升沿
    {
      CaptureValue1 = TIM_GetCapture1(TIM2);//记录捕获值
      CaptureStatus |= 0X4000;//标志捕捉到上升沿
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);//设置下降沿捕获
    }
    else if(CaptureStatus&0X4000)//捕获到下降沿
    {
      CaptureValue2 = TIM_GetCapture1(TIM2); //记录捕获值
      CaptureStatus &= ~0X4000;	//清除上升沿标志
			CaptureStatus |= 0X8000;	//设置捕获完成标志
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);//设置为上升沿捕获
    }
  }
}

/**
  * @brief  定时器3中断服务程序
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); //清除TIM3更新中断标志 
		LED=!LED;
	}
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
