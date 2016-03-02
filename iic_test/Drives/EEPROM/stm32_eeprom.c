/*******************************************************************************
 * @name    : STM32 Flash����EEPROM����
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
#include "stm32_eeprom.h" 

//�����ַ�������ַ������Ψһ�ģ��Ҳ�����0XFFFF
//���ﶨ����100�������ַ�������ֵַ��VirtAddrTab�±���ͬ
uint16_t VirtAddVarTab[NumbOfVar] = 
{
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,
	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,
	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,
	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,
	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,
	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,
	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,
	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,
};

//��ģ��EEPROM�ж�ȡ���������ַ��Ӧ����ֵ
uint16_t DataVar = 0;

static FLASH_Status EE_Format(void);
static uint16_t EE_FindValidPage(uint8_t Operation);
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data);

/**
  * @brief  ϵͳ�ϵ�󣬸���ҳ״̬��ʼ��EEPROM
  * @param  None.
  * @retval - FLASH����������: д��������
  *         - FLASH_COMPLETE: �����ɹ�
	* @note ��Ϊϵͳ�����FLASH�����ݲ��ᶪʧ�����Գ�ʼ�������Ǹ�����һ��PAGE״̬���е�
  */
uint16_t EE_Init(void)
{
  uint16_t PageStatus0 = 6, PageStatus1 = 6;//Page״̬��16bitλ��
  uint16_t VarIdx = 0;
  uint16_t EepromStatus = 0, ReadStatus = 0;
  int16_t x = -1;
  uint16_t  FlashStatus;

  //��ȡPAGE0״̬
  PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);
  //��ȡPAGE1״̬
  PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

  //����PAGE״̬����EEPROM
  switch (PageStatus0)
  {
    case ERASED:
      if (PageStatus1 == VALID_PAGE) // Page0 ERASED, Page1 VALID_PAGE
      {
        //����Page0����
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);//����
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
      else if (PageStatus1 == RECEIVE_DATA) //Page0 ERASED, Page1 RECEIVE_DATA
      {
        //����Page0
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
				
        //��Page1����Ϊ��ЧPage
        FlashStatus = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
      else //EEPROM�״η���(Page0&1����ERRASED״̬)����Ч״̬ -> ��ʽ��EEPROM
      {
				//��ʽ��EEPROM: ����Page0��Page1������Page0Ϊ��ЧPage
        FlashStatus = EE_Format();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
      break;

    case RECEIVE_DATA:
      if (PageStatus1 == VALID_PAGE) // Page0 RECEIVE_DATA, Page1 VALID_PAGE 
      {
        //˵���ϴ�����Page1ת�����ݵ�Page0ʱ��ϵͳ�����ˣ���Ҫ�����������ת�ơ�
        for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++)
        {
					//PAGE0�ж����������ַ��Ч����������ַ�����ݲ���Ҫת��
          if (( *(__IO uint16_t*)(PAGE0_BASE_ADDRESS + 6 + 4*VarIdx)) == VirtAddVarTab[VarIdx])
          {
            x = VarIdx;
          }
          if (VarIdx != x)//�����������ַ����Ч�ģ�ת������
          {
            //��PAGE1�����������ַ���һ�δ洢����ֵ
            ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
            //�����PAGE1���ҵ��˸������ַ���Ҷ�������Ӧ������
            if (ReadStatus != 0x1)
            {
              //������������ת�Ƶ�Page0��
              EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
              if (EepromStatus != FLASH_COMPLETE) return EepromStatus;//����ʧ�ܣ����ش�����
            }
          }
        }			
        //��Page0��־Ϊ��Чҳ
				FLASH_Unlock();
        FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
				
        //����Page1
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
			
      else if (PageStatus1 == ERASED) // Page0 RECEIVE_DATA, Page1 ERASED 
      {
        //����Page1
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
				
        //��Page0��ʶΪ��Чҳ
				FLASH_Unlock();
        FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
			
      else//����״̬���Ǵ���ģ���ʽ��EEPROM��
      {
        //��ʽ��EEPROM: ����Page0��Page1������Page0Ϊ��ЧPage
        FlashStatus = EE_Format();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
      break;

    case VALID_PAGE:
      if (PageStatus1 == VALID_PAGE) //�����״̬
      {
        //��ʽ��EEPROM: ����Page0��Page1������Page0Ϊ��ЧPage
        FlashStatus = EE_Format();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
			
      else if (PageStatus1 == ERASED) /* Page0 VALID_PAGE, Page1 ERASED */
      {
        //����Page1
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      }
			
      else /* Page0 VALID_PAGE, Page1 RECEIVE_DATA */
      {
        //˵����Page0�������ݵ�Page1��ʱ��ϵͳ�����ˣ���Ҫ����������ݴ��䣡
        for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++)
        {
					//��Page1��ȡ���������ַ����Ч�ģ�˵����������ַ�������ϴ��Ѿ������ˣ�����Ҫ�ٴ��䡣
          if ((*(__IO uint16_t*)(PAGE1_BASE_ADDRESS + 6 + 4*VarIdx)) == VirtAddVarTab[VarIdx])
          {
            x = VarIdx;
          }
					//��Page1��ȡ���������ַ����Ч�ģ���Ҫ���´��䣡
          if (VarIdx != x)
          {
            //��Page0��ȡ�������ַ���һ�εĴ洢ֵ
            ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
            //����ɹ��Ķ�ȡ���˸������ַ�Ĵ洢ֵ
            if (ReadStatus != 0x1)
            {
              //���������ַ�ʹ洢ֵ���䵽Page1
              EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
              if (EepromStatus != FLASH_COMPLETE) return EepromStatus;//����ʧ�ܣ����ش�����
            }
          }
        }
        //��Page1����Ϊ��Чҳ
				FLASH_Unlock();
        FlashStatus = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����	
        //����Page0
				FLASH_Unlock();
        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
				FLASH_Lock();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����		
      }
      break;

    default:	//�����κ����
      //��ʽ��EEPROM: ����Page0��Page1������Page0Ϊ��ЧPage
        FlashStatus = EE_Format();
        if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//����ʧ�ܣ����ش�����
      break;
  }
	
  return FLASH_COMPLETE;
}

/**
  * @brief  ��EEPROM�ж�ȡ�����ַVirtAddress��Ӧ�Ĵ洢ֵ
  * @param  VirtAddress: ָ���������ַ
  * @param  Data: ��ȡ����ֵ
  * @retval
  *		- 0: �ɹ��ض�ȡ������Ӧ�Ĵ洢ֵ
  *		- 1: ָ���������ַδ�ҵ�
  *		- NO_VALID_PAGE: δ�ҵ���Чҳ
  */
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data)
{
  uint16_t ValidPage = PAGE0;
  uint16_t AddressValue = 0, ReadStatus = 1;
  uint32_t Address = PAGE0_END_ADDRESS, PageStartAddress = PAGE0_BASE_ADDRESS;

  //��ȡ��������Чҳ
  ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

  //���δ��ȡ����Чҳ
  if (ValidPage == NO_VALID_PAGE)
  {
    return  NO_VALID_PAGE;
  }

  //��Чҳ�׵�ַ
  PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  //��Чҳβ��ַ
  Address = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

  //��β��ַ��ʼ���Ҷ�Ӧ�������ַ
  while (Address > (PageStartAddress + 2))
  {
    //��ȡ�����ַ
    AddressValue = (*(__IO uint16_t*)Address);

    //�����ȡ���������ַ����ָ���������ַ
    if (AddressValue == VirtAddress)
    {
      //��ȡ�����ַ��Ӧ�Ĵ洢ֵ
      *Data = (*(__IO uint16_t*)(Address - 2));

      //���óɹ���ȡ���洢ֵ
      ReadStatus = 0;

      break;
    }
    else
    {
      //������һ�������ַ
      Address = Address - 4;
    }
  }
	
  return ReadStatus;
}

/**
  * @brief  д��һ�����ݵ�EEPROM
  * @param  VirtAddress: ָ���������ַ
  * @param  Data: ��Ҫд���16bit����
  * @retval 
  *		- FLASH_COMPLETE: д��ɹ�
  *		- PAGE_FULL: ��Чҳ�����Ѿ����ˣ���ʾ��Ч������Ҫת�ƣ�
  *		- NO_VALID_PAGE: û�ҵ���Чҳ
  *		- ����FLASH����������
  */
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data)
{
  uint16_t Status = 0;

  //д�����ݺ������ַ��EEPROM
  Status = EE_VerifyPageFullWriteVariable(VirtAddress, Data);

  //�����Чҳ��������
  if (Status == PAGE_FULL)
  {
    //����Ҳ���䣺������Ч����ת�Ƶ���һҳ��������ҳ״̬
    Status = EE_PageTransfer(VirtAddress, Data);
  }
	
  return Status;
}

/**
  * @brief  ��ʽ��EEPROM������Page0��Page1����Page0��Ϊ��Чҳ
  * @param  None
  * @retval Flash������
  */
static FLASH_Status EE_Format(void)
{
  FLASH_Status FlashStatus = FLASH_COMPLETE;

  //����Page0
	FLASH_Unlock();
  FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
	FLASH_Lock();
  //�������ʧ�ܣ����ش�����
  if (FlashStatus != FLASH_COMPLETE) return FlashStatus;

  //��Page0��Ϊ��Чҳ
	FLASH_Unlock();
  FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
	FLASH_Lock();
  //�������ʧ�ܣ����ش�����
  if (FlashStatus != FLASH_COMPLETE) return FlashStatus;

  //����Page1
	FLASH_Unlock();
  FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
	FLASH_Lock();
  return FlashStatus;
}

/**
  * @brief  ���Ҷ�EEPROM��дEEPROM��������Чҳ
  * @param  Operation: ��������д����
  *     @arg READ_FROM_VALID_PAGE: ������
  *     @arg WRITE_IN_VALID_PAGE: д����
  * @retval ��Чҳ(PAGE0 or PAGE1)�����û������Чҳ���򷵻� NO_VALID_PAGE
	*/
static uint16_t EE_FindValidPage(uint8_t Operation)
{
  uint16_t PageStatus0 = 6, PageStatus1 = 6;

  //��ȡPage0״̬
  PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);

  //��ȡPage1״̬
  PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

	//���ݶ�������д����ɢת
  switch (Operation)
  {
    case WRITE_IN_VALID_PAGE:	//д����
      if (PageStatus1 == VALID_PAGE)
      {
        //����Page1�������ݵ�Page0
        if (PageStatus0 == RECEIVE_DATA)
        {
          return PAGE0;	//Page0Ϊд������Чҳ
        }
        else
        {
          return PAGE1;	//Page1Ϊд������Чҳ
        }
      }
      else if (PageStatus0 == VALID_PAGE)
      {
        //����Page0�������ݵ�Page1
        if (PageStatus1 == RECEIVE_DATA)
        {
          return PAGE1;	//Page1Ϊд������Чҳ
        }
        else
        {
          return PAGE0;	//Page0Ϊд������Чҳ
        }
      }
      else
      {
        return NO_VALID_PAGE;//δ�ҵ���Чҳ
      }

    case READ_FROM_VALID_PAGE:	//������
      if (PageStatus0 == VALID_PAGE)
      {
        return PAGE0;	//Page0Ϊ����Чҳ
      }
      else if (PageStatus1 == VALID_PAGE)
      {
        return PAGE1;	//Page1Ϊ����Чҳ
      }
      else
      {
        return NO_VALID_PAGE ;	//δ�ҵ���Чҳ
      }

    default:
      return PAGE0;	//����Page0����������£���Ӧ��ִ�е����
  }
}

/**
  * @brief  �ж���Чҳ�Ƿ����ˣ����û�У��������ַ����ֵд��EEPROM
  * @param  VirtAddress: ָ���������ַ
  * @param  Data: ��Ҫд�����ֵ
  * @retval
  *		- FLASH_COMPLETE: �����ɹ�
  *		- PAGE_FULL: ��Чҳ������Ҫת������
  *		- NO_VALID_PAGE: δ�ҵ���Чҳ
  *		- Flash error code: ����FLASH����������
  */
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data)
{
  FLASH_Status FlashStatus = FLASH_COMPLETE;
  uint16_t ValidPage = PAGE0;
  uint32_t Address = PAGE0_BASE_ADDRESS, PageEndAddress = PAGE0_END_ADDRESS;

  //��ȡд������Чҳ
  ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

  //δ�ҵ���Чҳ
  if (ValidPage == NO_VALID_PAGE)
  {
    return  NO_VALID_PAGE;
  }

  //������Чҳ�׵�ַ
  Address = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  //������Чҳβ��ַ
  PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));
	
  //��ҳ�׿�ʼ���ҿ���д��Ŀյ�ַ
  while (Address < PageEndAddress)
  {
    // ���Address �� Address+2 ������ 0XFFFFFFFF��˵�������ַ�ǿյ�
    if ((*(__IO uint32_t*)Address) == 0xFFFFFFFF)
    {
      //д������
			FLASH_Unlock();
      FlashStatus = FLASH_ProgramHalfWord(Address, Data);
			FLASH_Lock();
      //�������ʧ�ܣ����ش�����
			if (FlashStatus != FLASH_COMPLETE) return FlashStatus;
      //д�������ַ
			FLASH_Unlock();
      FlashStatus = FLASH_ProgramHalfWord(Address + 2, VirtAddress);
			FLASH_Lock();
      return FlashStatus;
    }
    else
    {
      Address = Address + 4;//������һ��λ��
    }
  }

  //û�ҵ��յ�ַ��������Чҳ����
  return PAGE_FULL;
}

/**
  * @brief  ������޸ĵ���Чֵ��������Page������һPage
  * @param  VirtAddress: 16bit�����ַ
  * @param  Data: 16bit����
  * @retval
  *		- FLASH_COMPLETE: �����ɹ�
  *		- PAGE_FULL: ҳ����
  *		- NO_VALID_PAGE: δ�ҵ���Чҳ
  *		- Flash error code: ����FLASH����������
  */
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data)
{
  FLASH_Status FlashStatus = FLASH_COMPLETE;
  uint32_t NewPageAddress = 0x080103FF, OldPageAddress = 0x08010000;
  uint16_t ValidPage = PAGE0, VarIdx = 0;
  uint16_t EepromStatus = 0, ReadStatus = 0;

  //��ȡ��������Чҳ
  ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

  if (ValidPage == PAGE1)	//��ǰPage1��Ч
  {
    //�����µ�Page��ַΪPage0�׵�ַ������ת�Ƶ�Ŀ��Page
    NewPageAddress = PAGE0_BASE_ADDRESS;

    //�����ϵ�PageΪPage1�׵�ַ�����ݴ���Pageת�Ƶ���Page
    OldPageAddress = PAGE1_BASE_ADDRESS;
  }
  else if (ValidPage == PAGE0)//��ǰPage0��Ч
  {
    ///�����µ�Page��ַΪPage1�׵�ַ������ת�Ƶ�Ŀ��Page
    NewPageAddress = PAGE1_BASE_ADDRESS;

    //�����ϵ�PageΪPage0�׵�ַ�����ݴ���Pageת�Ƶ���Page
    OldPageAddress = PAGE0_BASE_ADDRESS;
  }
  else
  {
    return NO_VALID_PAGE; //δ�ҵ���Чҳ
  }

  //������Page��ҳ״̬ΪRECEIVE_DATA
	FLASH_Unlock();
  FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, RECEIVE_DATA);
	FLASH_Lock();
  if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//Flash����ʧ�ܣ����ش����룡

  //�������ַ����Ӧ����ֵд��RCEIVE_DATAҳ
  EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddress, Data);
  if (EepromStatus != FLASH_COMPLETE) return EepromStatus;//EEPROM����ʧ�ܣ����ش����룡

  //��ʼת������: ������������µ���Ч��ַ�Ͷ�Ӧ����ֵ���䵽��Page��
	//�������ַ��˳�����δ��䣬�������������ַ����һ��
  for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++)
  {
    if (VirtAddVarTab[VarIdx] != VirtAddress)//�ǸղŴ���������ַ����Ϊ�ղ��Ѿ���������µ�ֵ�ˣ�
    {
      ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);//��ȡ��Ӧ�����ַ�����ֵ
      if (ReadStatus != 0x1)//�ɹ���ȡ���˶�Ӧ�����ַ����ֵ
      {
        //�������ַ����ֵ���䵽��Page��
        EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
        if (EepromStatus != FLASH_COMPLETE) return EepromStatus;//EEPROM����ʧ�ܣ����ش����룡
      }
    }
  }

  //������Page
	FLASH_Unlock();
  FlashStatus = FLASH_ErasePage(OldPageAddress);
	FLASH_Lock();
  if (FlashStatus != FLASH_COMPLETE) return FlashStatus;//Flash����ʧ�ܣ����ش����룡

  //����Page����Ϊ��ǰ��ЧPage
	FLASH_Unlock();
  FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, VALID_PAGE);
	FLASH_Lock();
  return FlashStatus;
}

/**
  * @brief  ��EEPROM�ж�ȡָ�����ȵ�����
  * @param  VirtAddrOffset: ָ���������ַƫ����
  * @param  Data: ��ֵ�����ʼ��ַ
	* @param	NbrOfData: ��ȡ�����ݸ���
  * @retval
  *		- 0: ��ȡ�ɹ�
  *		- 1: ָ���������ַδ�ҵ�
  *		- NO_VALID_PAGE: δ�ҵ���Чҳ
  */
uint16_t EE_ReadBuf(uint16_t VirtAddrOffset, uint16_t* Data, uint8_t NbrOfData)
{
	uint16_t ReadStatus = 1;
	uint8_t VarIdx = 0;
	
	if(NbrOfData > NumbOfVar) return 1;//�����ȡ�����ݸ���������Χ
	
	for (VarIdx = 0; VarIdx < NbrOfData; VarIdx++)
	{
		//�������ַ��ȡ��ֵ
		ReadStatus = EE_ReadVariable(VirtAddVarTab[VirtAddrOffset+VarIdx], Data+VarIdx);
	}
	return ReadStatus;
}

/**
  * @brief  ��EEPROMָ����ַ��ʼд��һ����ֵ
  * @param  VirtAddrOffset: ָ���������ַƫ����
  * @param  Data: ��Ҫд���16bit������ʼ��ַ
	* @param	NbrOfData: ��Ҫд������ݸ���
  * @retval 
  *		- FLASH_COMPLETE: д��ɹ�
  *		- PAGE_FULL: ��Чҳ�����Ѿ����ˣ���ʾ��Ч������Ҫת�ƣ�
	*		- PAGE_OVER: ��д������ݸ�����������Χ
  *		- NO_VALID_PAGE: û�ҵ���Чҳ
  *		- ����FLASH����������
  */
uint16_t EE_WriteBuf(uint16_t VirtAddrOffset, uint16_t* Data, uint8_t NbrOfData)
{
	uint16_t status = 0;
	uint8_t VarIdx = 0;
	
	if(NbrOfData > NumbOfVar) return	PAGE_OVER;//��д������ݸ�����������Χ
	
	for (VarIdx = 0; VarIdx < NbrOfData; VarIdx++)
	{
		//��EEPROM�����ַд������
		status = EE_WriteVariable(VirtAddVarTab[VirtAddrOffset+VarIdx], *(Data + VarIdx));
	}
	
	return status;
	
}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
