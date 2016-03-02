/*******************************************************************************
 * @name    : 2.4G����ģ��NRF24L01����
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
 
#include "nrf24l01.h"
#include "spi.h"
#include "delay.h"

//��ַ�Ĵ����Ǵӵ��ֽڿ�ʼд�룬���д���ֽڵ�ʱ�򣬸��ֽڱ���ԭ�����ݲ���
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x12, 0x34, 0x56, 0x78, 0x9A}; //���͵�ַ
const uint8_t RX_ADDRESS0[RX_ADR_WIDTH]={0x12, 0x34, 0x56, 0x78, 0x9A}; //����ͨ��0��ַ
const uint8_t RX_ADDRESS1[RX_ADR_WIDTH]={0x11, 0x22, 0x33, 0x44, 0x55}; //����ͨ��1��ַ
const uint8_t RX_ADDRESS2[1]={0x22};//����ͨ��2��ַ
const uint8_t RX_ADDRESS3[1]={0x33};//����ͨ��3��ַ
const uint8_t RX_ADDRESS4[1]={0x44};//����ͨ��4��ַ
const uint8_t RX_ADDRESS5[1]={0x55};//����ͨ��5��ַ

/**
  * @brief NRF24L01����ģ���ʼ��
  * @param None
  * @retval 0 - ��ʼ���ɹ�; 1 - ��ʼ��ʧ��
  * @note
	* 	NRF24L01����ģ�����ӵ�SPI1��
	*		NRF_CE: 	PA2
	*		NRF_CSN:	PA3
	*		NRF_IRQ:	PA4
  */
uint8_t NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����NRF24L01Ƭѡ�ܽź�SPIѡͨ�ܽ�,Ƭѡ: PA2; SPIѡͨ: PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��������������Ϣ��ʼ��
	NRF_CSN_DESELECT();//��ʼ��SPI֮ǰ����ʹNRF24L01���ڷ�ѡͨ״̬
	
	//����NRF24L01�ж�����Ϊ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI1_Init();	//SPI1��ʼ��
	
	NRF_CE = 0;	//NRF24L01Ƭѡʹ��

	if(NRF24L01_Check())return 1;
	
	NRF_CE = 1;
	return 0;
}

/**
  * @brief д�Ĵ�������
  * @param reg : �Ĵ�����ַ
	* @param val : ���õļĴ���ֵ
  * @retval NRF24L01����״̬
  * @note
	* 	
  */
NRF_StatusTypedef NRF24L01_Write_Reg(uint8_t reg, uint8_t val)
{
	uint8_t status = 0;
	uint32_t timeout = 0;
	
	SPI_Set_BaudRate(SPI1, NRF_SPI_SPEED);	//����SPI1������
	NRF_CSN_SELECT();	//��ȡSPI����
	timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
	status = SPI_RW(NRF_SPI,reg,&timeout);//д�Ĵ�����ַ
	if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
	{
		NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
		return NRF_TIMEOUT;		//���ش���
	}
	
	timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
	status = SPI_RW(NRF_SPI,	val, &timeout);//д����
	if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
	{
		NRF_CSN_DESELECT();//�ͷ�SPI��ѡ
		return NRF_TIMEOUT;//���ش���
	}
	
	NRF_CSN_DESELECT();//�ͷ�SPI��ѡ
	
	return NRF_SUCCESS;//���ز����ɹ�
}

/**
  * @brief ���Ĵ���
  * @param reg : �Ĵ�����ַ
	* @param *val : ��ȡ���ļĴ���ֵ��ŵ�ַ
  * @retval NRF24L01����״̬
  * @note
	* 	
  */
NRF_StatusTypedef NRF24L01_Read_Reg(uint8_t reg, uint8_t* val)
{
	uint8_t data = 0;
	uint32_t timeout = 0;
	
	SPI_Set_BaudRate(SPI1, NRF_SPI_SPEED);	//����SPI1������
	NRF_CSN_SELECT();	//��ȡSPI����
	timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
	data = SPI_RW(NRF_SPI,0x00,&timeout);//д�Ĵ�����ַ
	if(timeout == 0x00 && data == 0xff)//�ж��Ƿ�ʱ
	{
		NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
		return NRF_TIMEOUT;		//���ش���
	}
	
	*val = data;				//�����ȡ�ļĴ���ֵ
	NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
	
	return NRF_SUCCESS;	//���ز����ɹ�
}

/**
  * @brief ����д���ݵ�������(NRF24L01 FIFO)
  * @param reg : �Ĵ�����ַ
	* @param *pbuf : ���ݰ��׵�ַ
	* @param num : ���͵����ݸ���
  * @retval NRF24L01����״̬
  * @note  �ú�����Ҫ���ڽ�һ�����ݷŵ�NRF24L01�ķ���FIFO������
	* 	
  */
NRF_StatusTypedef NRF24L01_Write_Buf(uint8_t reg, uint8_t* pbuf, uint8_t num)
{
	uint8_t status = 0;
	uint32_t timeout = 0;
	uint8_t i = 0;
	
	SPI_Set_BaudRate(SPI1, NRF_SPI_SPEED);	//����SPI1������
	NRF_CSN_SELECT();//��ȡSPI����
	timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
	status = SPI_RW(NRF_SPI,reg,&timeout);	//ѡ��Ҫд��ļĴ���
	if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
	{
		NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
		return NRF_TIMEOUT;	//���ش���
	}
	
	for(i = 0; i < num; i++)
	{
		timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
		status = SPI_RW(NRF_SPI,*pbuf,&timeout);
		if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
		{
			NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
			return NRF_TIMEOUT;	//���ش���
		}
		pbuf++;
	}
	NRF_CSN_DESELECT();//�ͷ�SPI����
	return NRF_SUCCESS;//���ز����ɹ�
}

/**
  * @brief ����д���ݵ�������(NRF24L01 FIFO)
  * @param reg : �Ĵ�����ַ
	* @param *pbuf : ���ݰ��׵�ַ
	* @param num : ���͵����ݸ���
  * @retval NRF24L01����״̬
  * @note  �ú�����Ҫ���ڽ�һ�����ݷŵ�NRF24L01�ķ���FIFO������
	* 	
  */
NRF_StatusTypedef NRF24L01_Read_Buf(uint8_t reg, uint8_t* pbuf, uint8_t num)
{
	uint8_t status = 0;
	uint32_t timeout = 0;
	uint8_t i = 0;
	
	SPI_Set_BaudRate(SPI1, NRF_SPI_SPEED);	//����SPI1������
	NRF_CSN_SELECT();//��ȡSPI����
	timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
	status = SPI_RW(NRF_SPI,reg,&timeout);	//ѡ��Ҫд��ļĴ���
	if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
	{
		NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
		return NRF_TIMEOUT;	//���ش���
	}
	
	for(i = 0; i < num; i++)
	{
		timeout = NRF_TIMEOUT_VAL;//���÷��ʳ�ʱʱ��
		status = SPI_RW(NRF_SPI,0x00,&timeout);
		if(timeout == 0x00 && status == 0xff)//�ж��Ƿ�ʱ
		{
			NRF_CSN_DESELECT();	//�ͷ�SPI��ѡ
			return NRF_TIMEOUT;	//���ش���
		}
		*pbuf = status;//�������
		pbuf++;
	}
	
	NRF_CSN_DESELECT();//�ͷ�SPI����
	return NRF_SUCCESS;//���ز����ɹ�
}

/**
  * @brief ����д���ݵ�������(NRF24L01 FIFO)
  * @param reg : �Ĵ�����ַ
	* @param *pbuf : ���ݰ��׵�ַ
	* @param num : ���͵����ݸ���
  * @retval NRF24L01����״̬
  * @note  �ú�����Ҫ���ڽ�һ�����ݷŵ�NRF24L01�ķ���FIFO������
	* 	
  */
NRF_StatusTypedef NRF24L01_Check(void)
{
	NRF_StatusTypedef status = NRF_ERROR;
	uint8_t test[5] = {0x12, 0x34, 0x56, 0x78, 0x9A};
	uint8_t result[5] = {0x00,0x00,0x00,0x00,0x00};
	uint8_t i = 0;
	
	//����������д�뷢�ͻ���
	status = NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR, test, 5);
	if(status != NRF_SUCCESS)return status;//NRF24L01ͨ�Ŵ���
	
	//�ӷ��ͻ����ȡ����
	status = NRF24L01_Read_Buf(NRF_READ_REG+TX_ADDR, result, 5);
	if(status != NRF_SUCCESS)return status;//NRF24L01ͨ�Ŵ���
	
	//�ж϶�ȡ���������Ƿ�ͷ��͵�������ͬ
	for(i = 0; i < 5; i++)
	{
		if(result[i] != test[i])
			return NRF_ERROR;//���ݴ���
	}
	
	return NRF_SUCCESS;	//NRF24L01ͨ������
}

/**
  * @brief ��ʼ��NRF24L01ΪRXģʽ
  * @param none
  * @retval none
  * @note ����RX����ģʽ��������NRF24L01��6������ͨ��(channel0~channel5)����������Ӧ��ͨ����ַ��
	*				�ڱ������У��������õĽ���ͨ����ַΪ:
	*				RX_ADDR_P0: 0x12, 0x34, 0x56, 0x78, 0x9A
	*				RX_ADDR_P1:	0x11, 0x22, 0x33, 0x44, 0x55
	*				RX_ADDR_P2:	0x22, 0x22, 0x33, 0x44, 0x55
	*				RX_ADDR_P3:	0x33, 0x22, 0x33, 0x44, 0x55
	*				RX_ADDR_P4:	0x44, 0x22, 0x33, 0x44, 0x55
	*				RX_ADDR_P5:	0x55, 0x22, 0x33, 0x44, 0x55
	*				�������Ҫ���͵����ն˵�ĳһ��ͨ������ֻ��Ҫ���÷���˵ķ����ַ(TX_ADDR)������ͨ��0
	*       ��ַ(RX_ADDR_P0)�Ͷ�Ӧͨ���ĵ�ַ��ȼ��ɡ�
	*				ͨ�����ַ�ʽ������һ������ģ��������ͬʱ����6������ģ������ݣ����������κγ�ͻ��
	*/
void NRF24L01_RX_Mode(void)
{
	NRF_CE = 0;
	
	//���ý���ͨ����ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0, (uint8_t*)RX_ADDRESS0, RX_ADR_WIDTH);//д����ͨ��0��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P1, (uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);//д����ͨ��1��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P2, (uint8_t*)RX_ADDRESS2, 1);//д����ͨ��2��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P3, (uint8_t*)RX_ADDRESS3, 1);//д����ͨ��3��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P4, (uint8_t*)RX_ADDRESS4, 1);//д����ͨ��4��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P5, (uint8_t*)RX_ADDRESS5, 1);//д����ͨ��5��ַ

	//���ý���ͨ����Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//����ͨ��0��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);//����ͨ��1��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P2,RX_PLOAD_WIDTH);//����ͨ��2��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P3,RX_PLOAD_WIDTH);//����ͨ��3��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P4,RX_PLOAD_WIDTH);//����ͨ��4��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P5,RX_PLOAD_WIDTH);//����ͨ��5��Ч���ݿ��
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA, 0x3F);		//ʹ������ͨ���Զ�ACKӦ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR, 0x3F);//ʹ�����н���ͨ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);			//���ù���Ƶ����������ն˺ͷ����һ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP, 0x0F);	//��������2Mbps�����书��0dBm����������Ŵ���LNA
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0F);		//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
	
	NRF_CE = 1; //CEΪ��,�������ģʽ 
}						 

/**
  * @brief ��ʼ��NRF24L01ΪRXģʽ
  * @param CHx : ���÷���˶�Ӧ�Ľ���ͨ��
  * @retval none
  * @note
	*		- ���Ƚ������ַ��RX_ADDR_P0��ַ����Ϊ��Ӧ����ͨ���ĵ�ַ���������÷���ģʽ��ʱ�򣬻�Ҫ
	*			���ý��յ�ַ������Ϊ��Ҫͨ������ͨ��0������ACKӦ���źš�
	*		- ��CE��ߺ�,������TXģʽ,�����Խ���������
	*		- CEΪ�ߴ���10us,����������
  */
void NRF24L01_TX_Mode(uint8_t CHx)
{
	uint8_t channel = 0xFF;
	
	channel = CHx;
	NRF_CE = 0;
	
	//Ĭ�Ϸ���˶�Ӧ���ն�ͨ��1
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);		//дTX��ַ����Ҫ�Ͷ�Ӧ�Ľ���ͨ����ַ��ͬ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);	//���ý���ͨ��0�ĵ�ַ�����ڽ���ACKӦ���ź�

	switch(channel)//����TX��Ӧ���ն˵Ľ���ͨ��(���ն˹���6��ͨ��)
	{
		case 0x00: //����ͨ��0
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS0, RX_ADR_WIDTH);		//дTX��ַ����Ҫ�Ͷ�Ӧ�Ľ���ͨ����ַ��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0, RX_ADR_WIDTH);	//���ý���ͨ��0�ĵ�ַ�����ڽ���ACKӦ���ź�
		break;
		
		case 0x01: //����ͨ��1
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);		//дTX��ַ����Ҫ�Ͷ�Ӧ�Ľ���ͨ����ַ��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);	//���ý���ͨ��0�ĵ�ַ�����ڽ���ACKӦ���ź�
		break;
		
		case 0x02: //����ͨ��2��ע��ͨ��2~5�ĵ�ַ��ֻ����������ֽڣ�����4�����ֽں�ͨ��1��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS2, 1);
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS2, 1);
		break;
		
		case 0x03: //����ͨ��2��ע��ͨ��2~5�ĵ�ַ��ֻ����������ֽڣ�����4�����ֽں�ͨ��1��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS3, 1);
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS3, 1);
		break;
		
		case 0x04: //����ͨ��2��ע��ͨ��2~5�ĵ�ַ��ֻ����������ֽڣ�����4�����ֽں�ͨ��1��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS4, 1);
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS4, 1);
		break;
		
		case 0x05: //����ͨ��2��ע��ͨ��2~5�ĵ�ַ��ֻ����������ֽڣ�����4�����ֽں�ͨ��1��ͬ
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS5, 1);
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS5, 1);
		break;
		
		default:
			break;
	}

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA, 0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);//����ͨ��0��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR, 0x1A);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH, 40);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP, 0x0F);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0E);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
	NRF_CE = 1;//CEΪ��,10us����������
}	

/**
  * @brief ����һ�����ݰ�
  * @param buf : �����������׵�ַ
  * @retval �������״̬
  * @note 
  */
uint8_t NRF24L01_TxPacket(uint8_t *buf)
{
	uint8_t status = 0;
	
	NRF_CE = 0;
	NRF24L01_Write_Buf(NRF_TX_PLOAD, buf, TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF_CE = 1;					//��������	   
	while(NRF_IRQ != 0);//�ȴ��������
	NRF24L01_Read_Reg(NRF_READ_REG+STATUS, &status);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, status); //���TX_DS��MAX_RT�жϱ�־
	if(status & MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(NRF_FLUSH_TX, 0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(status & TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

/**
  * @brief ����һ�����ݰ�
  * @param buf : ���յ������ݰ���ŵ�ַ
  * @retval 6~255 - δ���յ�, 0~5 - ���յ�ͨ���������ݰ�
  * @note 
  */
uint8_t NRF24L01_RxPacket(uint8_t *buf)
{
	uint8_t status = 0;
	uint8_t rx_channel = 0xFF;

	NRF24L01_Read_Reg(NRF_READ_REG+STATUS, &status);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, status); //���RX_DR�жϱ�־
	if(status & RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(NRF_RX_PLOAD, buf, RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(NRF_FLUSH_RX, 0xff);//���RX FIFO�Ĵ��� 
		rx_channel = (status>>1)&0x07;
		return rx_channel;
	}
	return 255;//û�յ�����
}


/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
