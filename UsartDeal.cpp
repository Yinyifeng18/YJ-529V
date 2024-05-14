/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UsartDeal.c
 * \author    yifeng
 * \version   1.0
 * \date      2023��6��30��
 * \brief     ���ڲ�������
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��6��30��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "UsartDeal.h"
#include "CommFun.h"
#include "UartDevice.h"


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
static void UsartDeal_Init(void);
static void *UsartDeal_Receive_Thread(void *arg);
static void *UsartDeal_Send_Thread(void *arg);


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
UsartDealStru  gstvUsart;
UsartDealBits  gbitUsart;

typedef struct termios termios_t;

/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/




/*!
 * \fn     Start_UsartDeal_Monitor_Thread
 * \brief  
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_UsartDeal_Monitor_Thread(void)
{
  pthread_t recv_tid;
  pthread_t send_tid;
  
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  /* ��ʼ������������ڴ����� */
  memset(&gbitUsart, 0, sizeof(UsartDealBits));
  memset(&gstvUsart, 0, sizeof(UsartDealStru));

  /* ���ڳ�ʼ�� */
  UsartDeal_Init();

  if(gbitUsart.InitOk == SET)
  {
    return ;
  }
  
  if(pthread_create(&recv_tid, NULL, UsartDeal_Receive_Thread, NULL) < 0)
  {
    perror("pthread_create: UsartDeal_Receive_Thread");
  }
  
  
  if(pthread_create(&send_tid, NULL, UsartDeal_Send_Thread, NULL) < 0)
  {
    perror("pthread_create: UsartDeal_Send_Thread");
  }

  close(gstvUsart.UsartFd);
}



/*!
 * \fn     UsartDeal_Init
 * \brief  ���ڳ�ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void UsartDeal_Init(void)
{
  termios_t      *ter_s;
  
  ter_s = (termios_t *)malloc(sizeof(*ter_s));

  gbitUsart.InitOk = SET;

  // ����Ϊ�����ն˳��򣬲�����������������Ӱ��
  gstvUsart.UsartFd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if(gstvUsart.UsartFd < 0)
  {
    printf("/dev/ttyUSB0 open faild\r\n");
    return ;
  }
  printf("open /dev/ttyUSB0 succeed\n");
   
  bzero(ter_s,sizeof(*ter_s));
   
  ter_s->c_cflag |= CLOCAL | CREAD; //��������������ʹ��
   
  ter_s->c_cflag &= ~CSIZE;//ʧ������λ����
  ter_s->c_cflag |= CS8;//8λ����λ
   
  ter_s->c_cflag &= ~CSTOPB;//1λֹͣλ
   
  ter_s->c_cflag &= ~PARENB;//��У��λ
   
  ter_s->c_cc[VTIME] = 0;
  ter_s->c_cc[VMIN] = 0;
   
  /* 1 VMIN> 0 && VTIME> 0
     VMINΪ���ٶ�ȡ���ַ���������ȡ��һ���ַ��󣬻�����һ����ʱ�����ڶ�ʱ����ʱ��ǰ������Ѿ���ȡ����VMIN���ַ�����read����VMIN���ַ�������ڽ��յ�VMIN���ַ�֮ǰ����ʱ���Ѿ���ʱ����read�����Ѷ�ȡ�����ַ���ע�������ʱ������ÿ�ζ�ȡ��һ���ַ����������ã������¿�ʼ��ʱ�������Ƕ�ȡ����һ���ֽں�����ã�Ҳ����˵��ʱ������£����ٶ�ȡ��һ���ֽ����ݡ�
     2 VMIN > 0 && VTIME== 0
     ��ֻ�ж�ȡ��VMIN���ַ�ʱ��read�ŷ��أ��������read������������
     3 VMIN == 0 && VTIME> 0
     �͵�һ��������в�ͬ���ڽ��յ�һ���ֽ�ʱ���߶�ʱ����ʱʱ��read���ء�����ǳ�ʱ���������read����ֵ��0��
     4 VMIN == 0 && VTIME== 0
     ���������read���������ͷ��أ������ᱻ������----by ����ճ���Բ���԰
  */
  cfsetispeed(ter_s, B115200);//�������벨����
  cfsetospeed(ter_s, B115200);//�������������
  printf("==> cfsetospeed\r\n");
  tcflush(gstvUsart.UsartFd, TCIFLUSH);//ˢ��δ����������/�����
  printf("==> tcflush\r\n");
  if(tcsetattr(gstvUsart.UsartFd, TCSANOW, ter_s) != 0)
  {
    printf("com set error!\r\n");    
    return ;
  }

  gbitUsart.InitOk = CLEAR;
  
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
}




/*!
 * \fn     UsartDeal_Receive_Thread
 * \brief  ���ڽ������ݴ����߳�
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *UsartDeal_Receive_Thread(void *arg)
{
  int i = 0;
  int ret = 0;
  unsigned char DataBuf[1024];
  
  pthread_detach(pthread_self());
   
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  while(!gstvSysRun.quit)
  {
    /* ѭ���ȴ����մ������� */
    ret = read(gstvUsart.UsartFd, DataBuf, 1024);
    if(ret > 0)
    {
      printf("Usart Receive: ");
      for(i = 0; i < ret; i++)
      {
        printf("0x%x ", DataBuf[i]);
      }
      printf("\r\n");

      /* �ж����ݲ�ת�������������� */
      DeviceRxDatacIsr((unsigned char *)&DataBuf[0], ret);
    }
    
    usleep(5000);
  }

  pthread_exit(0);
}


/*!
 * \fn     UsartDeal_Send_Thread
 * \brief  ���ڷ������ݴ����߳�
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *UsartDeal_Send_Thread(void *arg)
{
  int ret = 0;
  int TimeTick = 0;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());

  while(!gstvSysRun.quit)
  {
    if(gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len != 0)
    {
      printf("TxDataBuf=>%d: ", gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len);
      for(int i = 0; i < gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len; i++)
      {
        printf("0x%x ", gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Data[i]);
      }
      printf("\r\n");

      
      /* �������� */
      ret = write(gstvUsart.UsartFd, &gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Data[0], gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len);
      tcdrain(gstvUsart.UsartFd);

      /* ����������� */
      memset((unsigned char *)&gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Data[0], 0, cUsartTxBufferSize);
      gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len = 0;
    }

    gstvUsart.TxDataOutPtr++;
    if(gstvUsart.TxDataOutPtr >= cUsartDataNum)
    {
      gstvUsart.TxDataOutPtr = 0;    
    }
    
    usleep(1000*10);
  }

  pthread_exit(0);
}


/*!
 * \fn     UsartDeal_InputData
 * \brief  �����ݷ��뻺��
 *          
 * \param  [in] unsigned char *Data   #
 * \param  [in] unsigned short len    #
 * 
 * \retval void
 */
void UsartDeal_InputData(unsigned char *Data, unsigned short len)
{
  unsigned char Index = 0;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  Index = gstvUsart.TxDataInPtr;

  gstvUsart.TxDataBuf[Index].Len = len;
  MemCopySramToSram2(Data, (unsigned char *)&gstvUsart.TxDataBuf[Index].Data[0], len);

  gstvUsart.TxDataInPtr++;
  if(gstvUsart.TxDataInPtr >= cUsartDataNum)
  {
    gstvUsart.TxDataInPtr= 0;
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
