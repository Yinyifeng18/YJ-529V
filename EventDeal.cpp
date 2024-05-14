/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      EventDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023��7��6��
 * \brief     �¼������Դ���� 
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��7��6��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "EventDeal.h"
#include "CommFun.h"

/*=======================================================================================
����������ָ�����������ֱ���ϵͳΪ C++ �� C ��Ԥ����ָ�                               
"__cplusplus" Ϊ C++ Ԥ���������֣�������ָ������������������Щָ��Ϊ C ��������.      
=======================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=======================================================================================
                            ��Դ�����ļ��ڲ�ʹ�õ��ַ�����������                         
=======================================================================================*/
/*---------------------------------------------------------------------------------------
���ַ����������壺
----------------------------------------------------------------------------------------*/
const AlarmHYStru cMainHYTable[3] =   
{
  EnAcNoAlarm,     0x0000,    EnAtSystemAlarm,   EnAlSystem,  // ��   
  EnAcSosAlarm,    0x1103,    EnAtSystemAlarm,   EnAlSystem,  // ��  
  EnAcUserCancel,  0x1406,    EnAtSystemAlarm,   EnAlSystem,  // ��  
};

/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õĺ���ԭ������                          
=======================================================================================*/


/*=======================================================================================
                       ��Դ�����ļ��ڲ�ʹ�õľֲ��������ַ�����������                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
�ľֲ��������ַ����������壺
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         ȫ�ֳ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֳ������壺
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         ȫ�ֱ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֱ������壺
---------------------------------------------------------------------------------------*/
EventPtrStru gstvEvent;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/


/*!
 * \fn     EventDeal_Init
 * \brief  �¼���ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void EventDeal_Init(void)
{
  memset(&gstvEvent, 0, sizeof(EventPtrStru));

  
}


/**
 *  �������ƣ�GetDateTime
 *  ����������ȡ��ǰϵͳ���ں�ʱ��
 *
 *  ���������char * psDateTime -- ϵͳ����ʱ��,��ʽΪyyymmddHHMMSS
 *  ���ؽ����0 -> �ɹ�
 */
int EventGetDateTime(char *psDateTime) 
{
  time_t nSeconds;
  struct tm * pTM;
  
  time(&nSeconds);
  pTM = localtime(&nSeconds);

  /* ϵͳ���ں�ʱ��,��ʽ: yyyymmddHHMMSS */
  sprintf(psDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
          pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
          pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
          
  return 0;
}


/*!
 * \fn     EventDateTimeConvToBCD
 * \brief  ��ʱ��ת��BCD����
 *          
 * \param  [in] void   #
 * 
 * \retval int
 */
int EventDateTimeConvToBCD(void)
{
  int ret;
  int i, j;
  char DateTime[20] =  {0};
  char BCD_DateTime[6] =  {0};
  char BCD_Buffer[20] =  {0};

  
  EventGetDateTime(DateTime);
  if(ret == 0)
  {
    printf("The Local date and time is %s\n", DateTime);

    j = 0;
    for(i = 0; i <  20; i++)
    {
      if(DateTime[i] == 0x2d || DateTime[i] == 0x20 
      || DateTime[i] == 0x3a || DateTime[i] == 0x00)
      {
      }
      else
      {
        BCD_Buffer[j] = DateTime[i]-0x30;
        j++;
      }
    }
        
    // ת��BCD����
    CompressByte2To1((unsigned char *)&BCD_Buffer[2], (unsigned char *)&BCD_DateTime[0], 12);
    MemCopySramToSram((unsigned char *)&BCD_DateTime[0], (unsigned char *)&gstvEvent.DateTime.YearL, sizeof(EventDateTimeStru));
    printf("BCD: ");
    for(i = 0; i < 6; i++)
    {
      printf("0x%x ", BCD_DateTime[i]);
    }    
    printf("\r\n"); 

    printf("DateTime: ");
    printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", gstvEvent.DateTime.YearL, gstvEvent.DateTime.Month,gstvEvent.DateTime.Day,
                                                gstvEvent.DateTime.Hour, gstvEvent.DateTime.Minute,gstvEvent.DateTime.Second);
    
  }
}

/*!
 * \fn     EventCodeWriteAlarmToEvent
 * \brief  д�¼�
 *          
 * \param  [in] unsigned char          Code   # ��������  
 * \param  [in] u8                 ZoneUser   #
 * 
 * \retval void
 */
void EventCodeWriteAlarmToEvent(AlarmCodeEnum Code, unsigned char ZoneUser)  
{  
  AlarmCodeEnum      AlarmCode;  
  unsigned short     ZoneNum;  
  EventFormatStru    buf;  
  const AlarmHYStru *pTab; 
  
  u8               i;  

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pTab      = cMainHYTable;           // ���������  
  AlarmCode = Code;  
  ZoneNum   = ConvBin8ToBcd3(ZoneUser);
  
  buf.AlarmEnum = Code;  

  // ��ȡʱ��
  EventDateTimeConvToBCD();
  MemCopySramToSram(&gstvEvent.DateTime.YearL, &buf.YearL, 6);  
  
  buf.Aear       = 0x00;
  buf.Zone       = ZoneNum;
  
  for (i=0; i<sizeof(cMainHYTable)/sizeof(AlarmHYStru); i++)   
  {  
    if (AlarmCode  == pTab->Code )     // ������������������ ���Ѷ���ľ���������  
    {  
      buf.List      = pTab->List;  
      buf.Type      = pTab->Type; 
      buf.AlarmCode = pTab->Event; 
      break;  
    }  
    pTab++;  
  }  
  
  if (i < sizeof(cMainHYTable)/sizeof(AlarmHYStru))  
  {  
    EventWriteDataToEeprom(&buf);     // д�¼�����ʷ�¼����洦�� 
  } 
} 



/*!
 * \fn     EventWriteDataToEeprom
 * \brief  д�¼�����ʷ�¼����洦��
 *          
 * \param  [in] void       *pSrouce   #
 * 
 * \retval void
 */
void EventWriteDataToEeprom(EventFormatStru *pSrouce)
{  
  EventPtrStru   *pEvent;  
  unsigned char  *pSrc;  
  unsigned short  CurrAddr;        // ��ǰ���  
  unsigned short  Ecnt;             // �¼�����    
  
  pEvent      = &gstvEvent;   
  CurrAddr    = pEvent->CurrentPtr;  // ��ǰ���  
  Ecnt        = pEvent->EventCnt;     // �¼�����  

  printf("pEvent->CurrentPtr: %d\n", pEvent->CurrentPtr);
  printf("pEvent->Rear: %d\n", pEvent->Rear);
  printf("pEvent->EventCnt: %d\n", pEvent->EventCnt);
  
  // д�¼�ָ�뵽 EEPROM  
  MemCopySramToSram((unsigned char *)&pSrouce->AlarmEnum, (unsigned char *)&gstvEvent.EventCode[pEvent->Rear].AlarmEnum, sizeof(EventFormatStru));

  printf("EventCode: \n");
  printf("gstvEvent.EventCode[%d].AlarmEnum: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].AlarmEnum);
  printf("gstvEvent.EventCode[%d].List: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].List);
  printf("gstvEvent.EventCode[%d].YearL: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].YearL);
  printf("gstvEvent.EventCode[%d].Month: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Month);
  printf("gstvEvent.EventCode[%d].Day: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Day);
  printf("gstvEvent.EventCode[%d].Hour: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Hour);
  printf("gstvEvent.EventCode[%d].Minute: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Minute);
  printf("gstvEvent.EventCode[%d].Second: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Second);
  printf("gstvEvent.EventCode[%d].Type: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Type);
  printf("gstvEvent.EventCode[%d].AlarmCode: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].AlarmCode);
  printf("gstvEvent.EventCode[%d].Aear: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Aear);
  printf("gstvEvent.EventCode[%d].Zone: 0x%x\n", pEvent->Rear, gstvEvent.EventCode[pEvent->Rear].Zone);

  pEvent->Rear++;
  if(pEvent->Rear >= cEventCodeNum)  
  {  
    pEvent->Rear  = 0;  
  }
  
  CurrAddr++;  
  Ecnt++;  
  if(Ecnt >= cEventCodeNum)  
  {  
    Ecnt  = cEventCodeNum;  
  }  
  if(CurrAddr >= cEventCodeNum)  
  {  
    CurrAddr = 0;  
  }  
  
  pEvent->CurrentPtr  = CurrAddr;  // ��ǰ���  
  pEvent->EventCnt   = Ecnt;       // �¼�����  
}  
 
 
/*!
 * \fn     EventReadDataToEeprom
 * \brief  ��ȡ��ʷ�¼�����
 *          
 * \param  [in] void           *pObject   #
 * \param  [in] u16              Front    #
 * 
 * \retval void
 */
void EventReadDataToEeprom(EventFormatStru          *pObject,        
                                     unsigned short Front      
                                    )
{  
  unsigned char *pDst;    
  
  EventFormatStru buf;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pDst  =  (unsigned char *)&pObject->AlarmEnum;  

  MemCopySramToSram((unsigned char *)&gstvEvent.EventCode[Front].AlarmEnum, pDst, sizeof(EventFormatStru));
}  
 



/*=======================================================================================
����������ָ���������������������ֱ���ϵͳΪ C++ �� C ��Ԥ����ָ�����Ӧ������ָ��
 extern "C" ����ָʾ�������򣬴˴���"}"�������"{"���Ӧ��Ϊ����ָʾ�����ý�������
=======================================================================================*/
#ifdef __cplusplus
}
#endif


/*
*****************************************************************************************
                                 ��������Դ�����ļ����˽���
*****************************************************************************************
*/
