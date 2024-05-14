/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      KeyDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月10日
 * \brief     按键处理源程序
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年7月10日
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "KeyDeal.h"

#include "EventDeal.h"
#include "PlayPcm.h"
#include "TimerDeal.h"


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
static void *KeyDeal_Thread(void *arg);



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
KeyDealBits gbitKey;

KeyDealStru gstvKey;

FILE  *key_file;

/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
*****************************************************************************************
*/




/*!
 * \fn     Start_KeyDeal_Monitor_Thread
 * \brief  按键处理线程
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_KeyDeal_Monitor_Thread(void)
{
  pthread_t ntp_tidp;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pthread_create(&ntp_tidp, NULL, KeyDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: KeyDeal_Thread");
  }
}


/*!
 * \fn     KeyDeal_Thread
 * \brief  按键处理任务
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *KeyDeal_Thread(void *arg)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());

  memset(&gstvKey, 0, sizeof(KeyDealStru));
  memset(&gbitKey, 0, sizeof(KeyDealBits));

  
  key_file = fopen(IMP_KEY_FILE, "rb");
  if(key_file == NULL)
  {
    printf("[ERROR] %s: fopen %s failed\n", __func__, IMP_KEY_FILE);
  }

  while(!gstvSysRun.quit)
  {   
    key_file = fopen(IMP_KEY_FILE, "r");
    if(key_file == NULL)
    {
      continue;
    }

    gstvKey.size = fread(gstvKey.KeyVal, 1, 1, key_file);
    if(gstvKey.size == 1)
    {
      printf("buf[0] is 0x%x\n", gstvKey.KeyVal[0]);
      if(gstvKey.KeyVal[0] == 0x30)
      {
        gstvKey.KeyLowCnt++;
        gstvKey.KeyHightCnt = 0;
      }
      else
      {
        printf("buf[0] is 0x%x\n", gstvKey.KeyVal[0]);
        gstvKey.KeyHightCnt++;
        gstvKey.KeyLowCnt = 0;
      }
      
      if(gstvKey.KeyLowCnt >= 10)
      {
        gstvKey.KeyLowCnt = 0;
        
        if(gbitKey.KeyRstFlg == 0x00)
        {
          gbitKey.KeyRstFlg = 0x01;

          /* 开始/结束 */
          if(gstvSysRun.Alarming == 0x00)  
          {
            gstvSysRun.Alarming = 0x01;    // 报警

            printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

            EventCodeWriteAlarmToEvent(EnAcSosAlarm, 1);  

            PlayPcmInputRunMode(EnPmOpenFile);
          }
          else if(gstvSysRun.Alarming == 0x01)
          {
            gstvSysRun.Alarming = 0x00;    // 取消

            printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

            EventCodeWriteAlarmToEvent(EnAcUserCancel, 1);  

            /* 删除任务 */
            StopP2pTaskThread();
          }
        }
      }
      else if(gstvKey.KeyHightCnt >= 10)
      {
        gstvKey.KeyHightCnt = 0;
        gbitKey.KeyRstFlg = 0x00;
      }
    }
    
    fseek(key_file, 0, SEEK_SET); 
    
    usleep(10*1000);
  }

  pthread_exit(0);
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
