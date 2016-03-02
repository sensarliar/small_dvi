/*******************************************************************************
 * @name    : STM32 Flash����EEPROM����ͷ�ļ�
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.2
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����ͨ��STM32��FLASH�����EEPROM���ǳ����䣬ֵ���о���
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
 * ��������ļ����Ǽ򵥵Ķ�STM32��FLASH����ҳ������д�����Ȳ��������ǿ��ǵ�FLASH
 * ���������ԣ�����������ҳ(Page)��FLASH���в����������������ַ������ͨ�������
 * ����������������ҳ�����������Ӷ��ɱ����ӳ�FLASH����������������FLASH����ʱ�䡣
 * ��ȻҲ���Զ�������Page�����FLASH����Ҫ��ǳ����̵ĳ��ϡ���������ST�ٷ��ṩ
 * �ķ����Ͻ������Ż����������е�Bug�����������������ַ��Ķ���Ҳʹ��EEPROM����
 * ��÷ǳ��ı�ݡ�
 * �����EEPROM��ʼ��ַΪFLASH��62KByte(0X08000000+62*1024)��ʼ��2��page������
 * EEPROM��СΪ100��16bit����ֵ�������Թ���255��16bit��ֵ����
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/

#ifndef __STM32_EEPROM_H
#define __STM32_EEPROM_H
#include "stm32f10x.h"


#define FLASH_SIZE		(64*1024)//����STM32 FLASH��С������ʵ���������

//����STM32ҳ��С��FLASH < 256KB
#if (FLASH_SIZE < (256*1024))
  #define PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */
#else
  #define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif

//����ģ��EEPROM��ʼ��ַ�������Լ���ʵ��������壬ע����Ҫҳ���룡
#define EEPROM_START_ADDRESS    ((uint32_t)(0x08000000+62*PAGE_SIZE))

//����ģ��EEPROM PAGE0��PAGE1���׵�ַ��β��ַ��ע���ַ����16bit�������Ͷ���
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

//����ģ��EEPROMҳ�����ﶨ����ʹ��2ҳģ��EEPROM�����
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

//û����Чҳ���壬���ڲ�����Чҳ�ķ���
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

//PAGE״̬����
#define ERASED                  ((uint16_t)0xFFFF)	// PAGE ��
#define RECEIVE_DATA            ((uint16_t)0xEEEE)	// PAGE ���ڽ�������
#define VALID_PAGE              ((uint16_t)0x0000)	// PAGE ��Ч

//����Чҳ��д��Чҳ����
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

//ҳ���ˣ���Ҫת����Ч����
#define PAGE_FULL               ((uint8_t)0x80)
#define	PAGE_OVER								((uint8_t)0x40)

//���彫�����ģ��EEPROM�����ݸ���������ʵ���������
//��Ϊ���ӣ����Ƕ���128��16bit���ݣ��൱��һ��EEPROM: AT24C02
#define NumbOfVar               ((uint8_t)100)

//�û�����������ַ�������ַ������Ψһ�ģ�������0XFFFF
extern uint16_t VirtAddVarTab[NumbOfVar];

uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);
uint16_t EE_ReadBuf(uint16_t VirtAddress, uint16_t* Data, uint8_t NbrOfData);
uint16_t EE_WriteBuf(uint16_t VirtAddress, uint16_t* Data, uint8_t NbrOfData);

#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
