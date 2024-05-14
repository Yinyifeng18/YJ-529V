/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      EventDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月6日
 * \brief     事件处理的源程序 
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年7月6日
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "EventDeal.h"
#include "CommFun.h"

/*=======================================================================================
下面这三条指令是用于区分编译系统为 C++ 或 C 的预处理指令。                               
"__cplusplus" 为 C++ 预处理器名字，这三条指令表明接在其下面的那些指令为 C 程序特征.      
=======================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=======================================================================================
                            本源程序文件内部使用的字符化常数定义                         
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的字符化常数定义：
----------------------------------------------------------------------------------------*/
const AlarmHYStru cMainHYTable[3] =   
{
  EnAcNoAlarm,     0x0000,    EnAtSystemAlarm,   EnAlSystem,  // 空   
  EnAcSosAlarm,    0x1103,    EnAtSystemAlarm,   EnAlSystem,  // 空  
  EnAcUserCancel,  0x1406,    EnAtSystemAlarm,   EnAlSystem,  // 空  
};

/*=======================================================================================
                             本源程序文件内部使用的函数原型声明                          
=======================================================================================*/


/*=======================================================================================
                       本源程序文件内部使用的局部常量、字符化常数定义                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的局部常量、字符化常数定义：
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         全局常量定义                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局常量定义：
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         全局变量定义                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局变量定义：
---------------------------------------------------------------------------------------*/
EventPtrStru gstvEvent;


/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
*****************************************************************************************
*/


/*!
 * \fn     EventDeal_Init
 * \brief  事件初始化
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
 *  函数名称：GetDateTime
 *  功能描述：取当前系统日期和时间
 *
 *  输出参数：char * psDateTime -- 系统日期时间,格式为yyymmddHHMMSS
 *  返回结果：0 -> 成功
 */
int EventGetDateTime(char *psDateTime) 
{
  time_t nSeconds;
  struct tm * pTM;
  
  time(&nSeconds);
  pTM = localtime(&nSeconds);

  /* 系统日期和时间,格式: yyyymmddHHMMSS */
  sprintf(psDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
          pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
          pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
          
  return 0;
}


/*!
 * \fn     EventDateTimeConvToBCD
 * \brief  把时间转成BCD编码
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
        
    // 转成BCD编码
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
 * \brief  写事件
 *          
 * \param  [in] unsigned char          Code   # 警情类型  
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
  
  pTab      = cMainHYTable;           // 警情代码表格  
  AlarmCode = Code;  
  ZoneNum   = ConvBin8ToBcd3(ZoneUser);
  
  buf.AlarmEnum = Code;  

  // 获取时间
  EventDateTimeConvToBCD();
  MemCopySramToSram(&gstvEvent.DateTime.YearL, &buf.YearL, 6);  
  
  buf.Aear       = 0x00;
  buf.Zone       = ZoneNum;
  
  for (i=0; i<sizeof(cMainHYTable)/sizeof(AlarmHYStru); i++)   
  {  
    if (AlarmCode  == pTab->Code )     // 如果防区报警警情代码 ＝已定义的警情类型码  
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
    EventWriteDataToEeprom(&buf);     // 写事件到历史事件缓存处理 
  } 
} 



/*!
 * \fn     EventWriteDataToEeprom
 * \brief  写事件到历史事件缓存处理
 *          
 * \param  [in] void       *pSrouce   #
 * 
 * \retval void
 */
void EventWriteDataToEeprom(EventFormatStru *pSrouce)
{  
  EventPtrStru   *pEvent;  
  unsigned char  *pSrc;  
  unsigned short  CurrAddr;        // 当前序号  
  unsigned short  Ecnt;             // 事件个数    
  
  pEvent      = &gstvEvent;   
  CurrAddr    = pEvent->CurrentPtr;  // 当前序号  
  Ecnt        = pEvent->EventCnt;     // 事件个数  

  printf("pEvent->CurrentPtr: %d\n", pEvent->CurrentPtr);
  printf("pEvent->Rear: %d\n", pEvent->Rear);
  printf("pEvent->EventCnt: %d\n", pEvent->EventCnt);
  
  // 写事件指针到 EEPROM  
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
  
  pEvent->CurrentPtr  = CurrAddr;  // 当前序号  
  pEvent->EventCnt   = Ecnt;       // 事件个数  
}  
 
 
/*!
 * \fn     EventReadDataToEeprom
 * \brief  读取历史事件处理。
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
下面这三条指令是用于与上面三条区分编译系统为 C++ 或 C 的预处理指令相对应。用于指定
 extern "C" 链接指示符作用域，此处的"}"与上面的"{"相对应，为链接指示符作用结束符。
=======================================================================================*/
#ifdef __cplusplus
}
#endif


/*
*****************************************************************************************
                                 本Ｃ语言源程序文件到此结束
*****************************************************************************************
*/
