/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UsartDeal.c
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     串口操作处理
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年6月30日
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "UsartDeal.h"
#include "CommFun.h"
#include "UartDevice.h"


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


/*=======================================================================================
                             本源程序文件内部使用的函数原型声明                          
=======================================================================================*/
static void UsartDeal_Init(void);
static void *UsartDeal_Receive_Thread(void *arg);
static void *UsartDeal_Send_Thread(void *arg);


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
UsartDealStru  gstvUsart;
UsartDealBits  gbitUsart;

typedef struct termios termios_t;

/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
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

  /* 初始化，清除所有内存数据 */
  memset(&gbitUsart, 0, sizeof(UsartDealBits));
  memset(&gstvUsart, 0, sizeof(UsartDealStru));

  /* 串口初始化 */
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
 * \brief  串口初始化
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

  // 不成为控制终端程序，不受其他程序输出输出影响
  gstvUsart.UsartFd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if(gstvUsart.UsartFd < 0)
  {
    printf("/dev/ttyUSB0 open faild\r\n");
    return ;
  }
  printf("open /dev/ttyUSB0 succeed\n");
   
  bzero(ter_s,sizeof(*ter_s));
   
  ter_s->c_cflag |= CLOCAL | CREAD; //激活本地连接与接受使能
   
  ter_s->c_cflag &= ~CSIZE;//失能数据位屏蔽
  ter_s->c_cflag |= CS8;//8位数据位
   
  ter_s->c_cflag &= ~CSTOPB;//1位停止位
   
  ter_s->c_cflag &= ~PARENB;//无校验位
   
  ter_s->c_cc[VTIME] = 0;
  ter_s->c_cc[VMIN] = 0;
   
  /* 1 VMIN> 0 && VTIME> 0
     VMIN为最少读取的字符数，当读取到一个字符后，会启动一个定时器，在定时器超时事前，如果已经读取到了VMIN个字符，则read返回VMIN个字符。如果在接收到VMIN个字符之前，定时器已经超时，则read返回已读取到的字符，注意这个定时器会在每次读取到一个字符后重新启用，即重新开始计时，而且是读取到第一个字节后才启用，也就是说超时的情况下，至少读取到一个字节数据。
     2 VMIN > 0 && VTIME== 0
     在只有读取到VMIN个字符时，read才返回，可能造成read被永久阻塞。
     3 VMIN == 0 && VTIME> 0
     和第一种情况稍有不同，在接收到一个字节时或者定时器超时时，read返回。如果是超时这种情况，read返回值是0。
     4 VMIN == 0 && VTIME== 0
     这种情况下read总是立即就返回，即不会被阻塞。----by 解释粘贴自博客园
  */
  cfsetispeed(ter_s, B115200);//设置输入波特率
  cfsetospeed(ter_s, B115200);//设置输出波特率
  printf("==> cfsetospeed\r\n");
  tcflush(gstvUsart.UsartFd, TCIFLUSH);//刷清未处理的输入和/或输出
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
 * \brief  串口接收数据处理线程
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
    /* 循环等待接收串口数据 */
    ret = read(gstvUsart.UsartFd, DataBuf, 1024);
    if(ret > 0)
    {
      printf("Usart Receive: ");
      for(i = 0; i < ret; i++)
      {
        printf("0x%x ", DataBuf[i]);
      }
      printf("\r\n");

      /* 判断数据并转其他任务处理数据 */
      DeviceRxDatacIsr((unsigned char *)&DataBuf[0], ret);
    }
    
    usleep(5000);
  }

  pthread_exit(0);
}


/*!
 * \fn     UsartDeal_Send_Thread
 * \brief  串口发送数据处理线程
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

      
      /* 发送数据 */
      ret = write(gstvUsart.UsartFd, &gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Data[0], gstvUsart.TxDataBuf[gstvUsart.TxDataOutPtr].Len);
      tcdrain(gstvUsart.UsartFd);

      /* 清除缓存数据 */
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
 * \brief  把数据放入缓存
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
