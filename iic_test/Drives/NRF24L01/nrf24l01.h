/*******************************************************************************
 * @name    : 2.4G����ģ��NRF24L01����ͷ�ļ�
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.1
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : 2.4G����ģ������
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
 * �������������ȳ�ʼ����NRF24L01����ģ���Ӧ��SPI�ӿڣ�ͬʱ�ṩ������NRF24L01
 * ����ģʽ(RXģʽ��TXģʽ)�Ľӿں��������ý���ģʽ�ӿں���NRF24L01_TX_Mode�У�
 * ���ǽ�NRF24L01��6������ͨ��ȫ��ʹ�ܣ�������������Ӧ��ͨ����ַ��
 * ���÷���ģʽ�ӿں���NRF24L01_TX_Mode�У�����CHx����������˵�������������Ϊ
 * ��Ӧ�ĸ�����ͨ����
 * �ڱ������У��������õĽ���ͨ����ַΪ:
 *	RX_ADDR_P0: 0x12, 0x34, 0x56, 0x78, 0x9A
 *	RX_ADDR_P1:	0x11, 0x22, 0x33, 0x44, 0x55
 *	RX_ADDR_P2:	0x22, 0x22, 0x33, 0x44, 0x55
 *	RX_ADDR_P3:	0x33, 0x22, 0x33, 0x44, 0x55
 *	RX_ADDR_P4:	0x44, 0x22, 0x33, 0x44, 0x55
 *	RX_ADDR_P5:	0x55, 0x22, 0x33, 0x44, 0x55
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-05-02    �����ˣ�������
 * �汾��¼��V1.1
 * �������ݣ������6������ͨ�������ģ��Ӷ�ʵ��ͨ��6��ͨ���ֱ��������6�����Ͷ�
 * �����ݣ���ͨ�����ݰ����պ���NRF24L01_RxPacket���ؽ��յ����������ĸ�ͨ������
 * �ݡ�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/

#ifndef __NRF24L01_H
#define __NRF24L01_H
#include "stm32f10x.h"
#include "bitband.h"

//NRF24L01�Ĵ�����������
#define NRF_READ_REG		 0x00	//�����üĴ�������5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG		 0x20	//д���üĴ�������5λΪ�Ĵ�����ַ
#define NRF_RX_PLOAD		 0x61	//��RX��Ч���ݣ�1-32���ֽڣ�������ʼ�մ�0�ֽڿ�ʼ������ģʽ��ʹ��
#define NRF_TX_PLOAD		 0xA0	//дTX��Ч���ݣ�1-32���ֽڣ�д����ʼ�մ�0�ֽڿ�ʼ������ģʽ��ʹ��
#define NRF_FLUSH_TX		 0xE1	//���TX FIFO�Ĵ���������ģʽ��ʹ��
#define NRF_FLUSH_RX		 0xE2	//���RX FIFO�Ĵ���������ģʽ��ʹ��
#define NRF_REUSE_TX_PL	 0xE3	//�ظ�ʹ����һ�����͵���Ч���ݣ�Ӧ���뷢���
#define NRF_NOP					 0xFF	//�ղ�������������ȡ״̬�Ĵ���

//NRF24L01�Ĵ�����ַ����
#define CONFIG					 0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
													 		 //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA            0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR        0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW         0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR       0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH            0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP         0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS           0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                               //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX					 0x10  //�ﵽ����ʹ����ж�
#define TX_OK						 0x20  //TX��������ж�
#define RX_OK						 0x40  //���յ������ж�

#define OBSERVE_TX       0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD               0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0       0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1       0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2       0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3       0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4       0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5       0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR          0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0         0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1         0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2         0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3         0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4         0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5         0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS  0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                               //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;

//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  12  	//12�ֽڵ��û����ݿ��(1-32)
#define RX_PLOAD_WIDTH  12  	//12�ֽڵ��û����ݿ��(1-32)

#define NRF_TIMEOUT_VAL					(200)	//����SPIͨ�ų�ʱʱ��
#define NRF_SPI									SPI1
#define NRF_SPI_SPEED						SPI_BaudRatePrescaler_16

//����NRF24L01����GPIO
#define NRF_CE									GPIOout(GPIOA,2)
#define NRF_CSN									GPIOout(GPIOA,3)
#define NRF_IRQ									GPIOin(GPIOA, 4)

#define NRF_CSN_SELECT()				{NRF_CSN = 0;}
#define NRF_CSN_DESELECT()			{NRF_CSN = 1;}

typedef enum
{
	NRF_SUCCESS = 0,
	NRF_ERROR,
	NRF_BUSY,
	NRF_TIMEOUT,
}NRF_StatusTypedef;

uint8_t NRF24L01_Init(void);			//NRF24L01��ʼ��
NRF_StatusTypedef NRF24L01_Write_Reg(uint8_t reg, uint8_t val);
NRF_StatusTypedef NRF24L01_Read_Reg(uint8_t reg, uint8_t* val);
NRF_StatusTypedef NRF24L01_Write_Buf(uint8_t reg, uint8_t* pbuf, uint8_t num);
NRF_StatusTypedef NRF24L01_Read_Buf(uint8_t reg, uint8_t* pbuf, uint8_t num);
NRF_StatusTypedef NRF24L01_Check(void);	//���24L01�Ƿ����
void NRF24L01_RX_Mode(void);
void NRF24L01_TX_Mode(uint8_t CHx);
uint8_t NRF24L01_TxPacket(uint8_t *buf);
uint8_t NRF24L01_RxPacket(uint8_t *buf);
 
#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
