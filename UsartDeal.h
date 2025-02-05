/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UsartDeal.h
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     串口操作处理头文件
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


/*=======================================================================================
下面两条指令是本头文件条件编译的预处理指令，用于防止本头文件被其他文件重复引用。
=======================================================================================*/
#ifndef __UsartDeal_H__
#define __UsartDeal_H__



/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "Main.h"

/*=======================================================================================
下面这三条指令是用于区分编译系统为 C++ 或 C 的预处理指令。                               
"__cplusplus" 为 C++ 预处理器名字，这三条指令表明接在其下面的那些指令为 C 程序特征.      
=======================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=======================================================================================
                               系统运行的各类字符化常数定义                             
=======================================================================================*/
/*---------------------------------------------------------------------------------------
为了做到较好的软件可移植性，推荐用户在编程时使用下面定义的数据类型大小
---------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------
的字符化常数定义：
----------------------------------------------------------------------------------------*/
#define cUsartRxBufferSize            512
#define cUsartTxBufferSize            512


#define cUsartDataNum                 20

/*=======================================================================================
                      各类字符化常数和硬件设计相关的输入／输出引脚定义
=======================================================================================*/
/*--------------------------------------------------------------------------------------
的输入／输出引脚定义：
--------------------------------------------------------------------------------------*/


/*======================================================================================
                             全局变量数据结构类型定义
======================================================================================*/
/*--------------------------------------------------------------------------------------
的全局数据结构类型定义 ：  
--------------------------------------------------------------------------------------*/
// 待发送的数据
typedef struct  
{  
  unsigned short Len ;                      // 数据长度 （字节数）  
  unsigned char  Data[cUsartTxBufferSize];  // 数据报文内容  
} UsartTxDataStru; 



typedef struct 
{
  int   UsartFd;                            // 串口文件描述符
  
  char  TxSize;                             // 每次发送数据个数大小
  char  TxCnt;                              // 每次发送数据个数计数器
  char  TxBuf[cUsartTxBufferSize];          // 每次发送数据缓冲区
  
  char  RxSize;                             // 每次接收数据个数大小
  char  RxCnt;                              // 每次接收数据个数计数器
  char  RxBuf[cUsartRxBufferSize];          // 每次接收数据缓冲区


  unsigned char   TxDataInPtr;              // 指向缓冲区存放的位置
  unsigned char   TxDataOutPtr;             // 指向缓冲区存放数据的位置
  UsartTxDataStru TxDataBuf[cUsartDataNum];    // 数据帧的发送缓冲区
}UsartDealStru;



typedef struct 
{
  unsigned char InitOk:      1;    // 1:初始化失败       0:初始化成功
  unsigned char TxOk:        1;
  unsigned char RxOk:        1;
}UsartDealBits;



/*=======================================================================================
                                         全局常量声明                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局常量声明：
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         全局变量声明                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局变量声明：
---------------------------------------------------------------------------------------*/
extern UsartDealStru  gstvUsart;
extern UsartDealBits  gbitUsart;



/*=======================================================================================
                                 源程序文件中的函数原型声明
=======================================================================================*/




/*!
 * \fn     Start_UsartDeal_Monitor_Thread
 * \brief  
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_UsartDeal_Monitor_Thread(void);



/*!
 * \fn     UsartDeal_InputData
 * \brief  把数据放入缓存
 *          
 * \param  [in] unsigned char *Data   #
 * \param  [in] unsigned short len    #
 * 
 * \retval void
 */
void UsartDeal_InputData(unsigned char *Data, unsigned short len);





/*=======================================================================================
下面这三条指令是用于与上面三条区分编译系统为 C++ 或 C 的预处理指令相对应。用于指定
 extern "C" 链接指示符作用域，此处的"}"与上面的"{"相对应，为链接指示符作用结束符。
=======================================================================================*/
#ifdef __cplusplus
}
#endif

#endif
/*
*****************************************************************************************
                                 本Ｃ语言源程序文件到此结束
*****************************************************************************************
*/


