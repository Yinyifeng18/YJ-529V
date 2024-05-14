/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UartDevice.h
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     烧录设备ID号等处理的头文件
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
#ifndef __UartDevice_H__
#define __UartDevice_H__



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
#define cSysSoftwareVer                      0x1105            // 软件版本V1.1.01
#define cSysHardwareVer                      0x10              // 固件版本V1.0
#define cSysPanelVer                         0x02

/*---------------------------------------------------------------------------------------
的字符化常数定义：
----------------------------------------------------------------------------------------*/
#define  cDeviceTxQueryMessAsk     0x01   // 数据〖查询设备信息应答〗
#define  cDeviceTxSetMessAsk       0x02   // 数据〖设置设备信息应答〗
#define  cDeviceTxWebrtcSetMessAsk 0x03   

#define cDeviceRxBufSize        0x40      // 扩展模块接收数据缓冲区大小
#define cDeviceTxBufSize        0x40      // 扩展模块发送数据缓冲区大小

#define cDeviceHead1            0xfc      // 协议头
#define cDeviceHead2            0xfd


#define cDeviceOverByte         10

#define cDeviceEuqiIDSize       0x04    // 设备ID的长度
#define cDeviceGsmImeiSize      15u     // 设备 GSM IMEI 长度
#define cDeviceMacSize          6       //  设备 WIFI MAC 长度
#define cDeviceWebrtcIdSize     32      // 

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
typedef struct  
{  
  unsigned char   Key;            // 主关键字 （即命令字）  
  unsigned short  Len;            // 数据长度 （字节数）  
  unsigned char   Data[64];       // 数据报文内容  
} DeviceDataStru; 


typedef struct
{
  unsigned char  Key;
  void           (*KeySubProgram)(void);
} DeviceMessageKeyStru;

typedef struct
{
  unsigned short TxSize;                            // 每次发送数据个数大小
  unsigned short TxCnt;                             // 每次发送数据个数计数器
  unsigned char  TxBuf[cDeviceTxBufSize];           // 每次发送数据缓冲区
  unsigned short RxSize;                            // 每次接收数据个数大小
  unsigned short RxCnt;                             // 每次接收数据个数计数器
  unsigned char  RxBuf[cDeviceRxBufSize];           // 每次接收数据缓冲区
  
  DeviceDataStru WaitTxData;                        // 等待发送的数据
  
  unsigned char  RxOver;                            // 接收数据的所用的时间计数  
  unsigned short RxByteOver;                        // 接收数据的所用的时间计数 

  unsigned short TestDelay;                         // 延时测试

  unsigned char EncryptDat[4];                      // 滚动数据
  unsigned char EuqiID[cDeviceEuqiIDSize];          // 设备ID号
  unsigned char GsmImei[cDeviceGsmImeiSize+1];      // IMEI  
  unsigned char MacAddr[cDeviceMacSize];            // MAC地址
  unsigned char WebrtcId[cDeviceWebrtcIdSize];
} DeviceUartStru;

typedef struct
{
  unsigned char RxByteOver:   1;               // 接收超时  
  unsigned char TranData:     1;               // 发送数据标志

  unsigned char NoGetID:      1;               // 没有写入ID号，网关不连入服务器
} DeviceUartBits;


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
extern  DeviceUartStru  gstvDevice;

extern  DeviceUartBits  gbitDevice;

/*=======================================================================================
                                 源程序文件中的函数原型声明
=======================================================================================*/



/*!
 * \fn     Start_DeviceUartDeal_Monitor_Thread
 * \brief  
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_DeviceUartDeal_Monitor_Thread(void);




/*!
 * \fn     DecodeDeviceIDHandle
 * \brief  解析设备ID
 *          
 * \param  [in] void      *pSrouce   #
 * \param  [in] void      *pDestin   #
 * 
 * \retval bool
 */
bool DecodeDeviceIDHandle(unsigned char *pSrouce, unsigned char *pDestin);     


/*!
 * \fn     Encrypt_HCS301
 * \brief  滚动码程序
 *          
 * \param  [in] unsigned char *keyin   #
 * \param  [in] unsigned char *datin   #
 * 
 * \retval void
 */
void Encrypt_HCS301(unsigned char *keyin, unsigned char *datin);


/*!
 * \fn     DeviceRxDatacIsr
 * \brief  设备接收数据中断处理
 *          
 * \param  [in] unsigned char *Data   #
 * \param  [in] unsigned short len    #
 * 
 * \retval void
 */
void DeviceRxDatacIsr(unsigned char *Data, unsigned short len);


/*!
 * \fn     DeviceTxWebrtcSetMessTest
 * \brief  测试数据
 *          
 * \param  [in] unsigned char Result   #
 * 
 * \retval static void
 */
void  DeviceTxWebrtcSetMessTest(void);



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


