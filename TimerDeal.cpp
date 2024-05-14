/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      TimerDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023��3��11��
 * \brief     ��ʱ������ͷ�ļ�
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��3��11��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "TimerDeal.h"

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


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õĺ���ԭ������                          
=======================================================================================*/
static void *TimerDeal_Thread(void *arg);

static void microseconds_sleep(unsigned long uSec);


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
TimerIncStru gstvTimerInc;
TimerCntStru gstvTimerCnt;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/

/*!
 * \fn     Start_TimerDeal_Monitor_Thread
 * \brief  ��ʱ������
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_TimerDeal_Monitor_Thread(void)
{
  pthread_t timer_tidp;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pthread_create(&timer_tidp, NULL, TimerDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: TimerDeal_Thread");
  }
  
}



/*!
 * \fn     TimerDeal_Thread
 * \brief  ��ʱ���߳�
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *TimerDeal_Thread(void *arg)
{
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pthread_detach(pthread_self());

  
  /* ��ʼ������ */
  memset(&gstvTimerInc, 0, sizeof(TimerIncStru));
  memset(&gstvTimerCnt, 0, sizeof(TimerCntStru));
  
  while(!gstvSysRun.quit)
  {
    TimerUpdataIsr();

    microseconds_sleep(750);
  }

  pthread_exit(0);
}

/*!
 * \fn     microseconds_sleep
 * \brief  ΢���ʱ��
 *          
 * \param  [in] unsigned long mSec   #
 * 
 * \retval static void
 */
static void microseconds_sleep(unsigned long uSec)
{
  struct timeval tv;
  tv.tv_sec = uSec/1000000;
  tv.tv_usec = uSec%1000000;

  int err;

  do {
    err = select(0, NULL, NULL, NULL, &tv);
  }while(err < 0 && errno == EINTR);
}


/*!
 * \fn     TimerGet1msInc
 * \brief  ��ȡ1ms��ʱ����ֵ��
 *          
 * \param  [in] void   #
 * 
 * \retval u16
 */
u16 TimerGet1msInc(void)
{
  return(gstvTimerInc.Inc1msCnt);
}

/*!
 * \fn     TimerGet1sInc
 * \brief  ��ȡ1s��ʱ����ֵ��
 *          
 * \param  [in] void   #
 * 
 * \retval u16
 */
u16 TimerGet1sInc(void)
{
  return(gstvTimerInc.Inc1sCnt);
}

/*!
 * \fn     TimerGet1minInc
 * \brief  ��ȡ1min��ʱ����ֵ��
 *          
 * \param  [in] void   #
 * 
 * \retval u16
 */
u16 TimerGet1minInc(void)
{
  return(gstvTimerInc.Inc1minCnt);
}

/*!
 * \fn     TimerGet1msSubb
 * \brief  ������1ms��ʱ����ֵ��
 *          
 * \param  [in] u16 MsCnt     #
 * \param  [in] u16 SubbVal   #
 * 
 * \retval bool
 */
bool TimerGet1msSubb(u16 MsCnt, u16 SubbVal)
{
  u16 Count;
  u16 Value;
  u16 Subb;
 
  Count  = MsCnt;
  Value  = SubbVal;
  Subb   = gstvTimerInc.Inc1msCnt - Count;
  if(Subb >= Value)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}

/*!
 * \fn     TimerGet1sSubb
 * \brief  ������1s��ʱ����ֵ��
 *          
 * \param  [in] u16 SecCnt    #
 * \param  [in] u16 SubbVal   #
 * 
 * \retval bool
 */
bool TimerGet1sSubb(u16 SecCnt, u16 SubbVal)
{
  u16 Count;
  u16 Value;
  u16 Subb;
  
  Count  = SecCnt;
  Value  = SubbVal;
  Subb   = gstvTimerInc.Inc1sCnt - Count;
  if(Subb >= Value)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}


/*!
 * \fn     TimerGet1minSubb
 * \brief  ������1min��ʱ����ֵ��
 *          
 * \param  [in] u16 MinCnt    #
 * \param  [in] u16 SubbVal   #
 * 
 * \retval bool
 */
bool TimerGet1minSubb(u16 MinCnt, u16 SubbVal)
{
  u16 Count;
  u16 Value;
  u16 Subb;
 
  Count  = MinCnt;
  Value  = SubbVal;
  Subb   = gstvTimerInc.Inc1minCnt - Count;
  
  if(Subb >= Value)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}

/*!
 * \fn     TimerUpdataIsr
 * \brief  �жϳ���
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void TimerUpdataIsr(void)
{
  gstvTimerInc.Inc1msCnt++;
  gstvTimerCnt.Timing1sCnt++;
  if(gstvTimerCnt.Timing1sCnt >= 1000)
  {
    gstvTimerCnt.Timing1sCnt = 0;
    gstvTimerInc.Inc1sCnt++;
    gstvTimerCnt.Timing1minCnt++;

    printf("Timer second: %d\n", gstvTimerInc.Inc1sCnt);
    if(gstvTimerCnt.Timing1minCnt >= 60)
    {
      gstvTimerCnt.Timing1minCnt = 0;
      gstvTimerInc.Inc1minCnt++;
    }
  }
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
