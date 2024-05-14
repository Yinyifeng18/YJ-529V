/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      NetDataDeal.h
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月3日
 * \brief     协议数据处理头文件
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年7月3日
      author: yifeng
      change: create file

*****************************************************************************************/


/*=======================================================================================
下面两条指令是本头文件条件编译的预处理指令，用于防止本头文件被其他文件重复引用。
=======================================================================================*/
#ifndef __NetDataDeal_H__
#define __NetDataDeal_H__



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
#define cNetWaitLoginAckTick     5             // 等待终端登录请求应答超时10秒
#define cNetWaitHeartAckTick     5             // 等待终端心跳应答超时10秒
#define cNetWaitDataAckTick      5             // 等待终端控制数据请求应答超时10秒
#define cNetWaitAlarmAckTick     5             // 等待终端报警数据应答超时10秒

#define cNetWaitLoginAckMax      5             // 等待终端登录请求应答次数
#define cNetWaitHeartAckMax      2             // 等待终端心跳应答次数
#define cNetWaitDataAckMax       3             // 等待终端控制数据请求应答次数
#define cNetWaitAlarmAckMax      3             // 等待终端报警数据应答次数




#define cNetCrirBufSize               0x600     
#define cNetRxBufSize                 0x300            // 扩展模块接收数据缓冲区大小  
#define cNetTxBufSize                 0x300            // 扩展模块发送数据缓冲区大小
#define cNetDataSize                  0x300            // 传输数据报文的数据有效字节大小  

#define cNetDataNum                   15               // 传输数据报文内容缓冲区大小 


#define cNetByEscapeChar              0x7E            // 被转义字符  
#define cNetEscapeChar                0x7D            // 转义字符

// 网络状态 
#define   cNetStateWifi                0x01
#define   cNetStategLan                0x02
#define   cNetStategGprs               0x04



#define cNetTxGeneralRespone          0x7fff          // 终端通用应答 
#define cNetTxLoginRequest            0x0001          // 终端登录请求 
#define cNetTxlogout                  0x0002          // 终端注销 
#define cNetTxheartbeat               0x0003          // 终端心跳 
#define cNetTxSysParaRespone          0x0004          // 系统参数应答
#define cNetTxFWUpdateRespone         0x0005          // 终端固件升级请求应答
#define cNetTxFWMessRequest           0x0006          // 终端固件信息请求
#define cNetTxFWDownRequest           0x0007          // 终端固件下载
#define cNetTxTimeRequest             0x0008          // 请求服务器校时


#define cNetTxProgParaRespone         0x1000          // 查询终端编程参数设置应答
#define cNetTxOtherParaRespone        0x1001          // 查询终端其它参数设置应答
#define cNetTxCtrlRespone             0x1002          // 终端控制应答
#define cNetTxQueryRespone            0x1003          // 终端查询应答
#define cNetTxAlarmEvent              0x1004          // 终端警情
#define cNetTxServerOperate           0x1005          // 终端对服务器操作
#define cNetTxUpdateState             0x1006          // 设备状态变化主动通知服务器
#define cNetTxAddrProgRespone         0x1007          // 查询终端指令编程参数应答
#define cNetTxTogtherAlarm            0x100B          // 主机分发联防报警到服务器


#define cNetHeartTickOver             60               // 终端心跳90秒


/*---------------------------------------------------------------------------------------
的字符化常数定义：
----------------------------------------------------------------------------------------*/
typedef enum
{
  EnTxEmpty              =  0x00         , // 发送空
  EnTxLoginRequest                       , // 终端登录请求
  EnTxHeartBeat                          , // 终端心跳
  EnTxCtrlData                           , // 发送控制信息
  EnTxAlarmData                            // 发送控制报警信息
} NetTxEnum;

/*--------------------------------------------------------------------------------------
终端通用应答的全局枚举类型定义：
--------------------------------------------------------------------------------------*/
typedef enum
{
  EnGrSuccessPart            =  0x00             , // 成功/确认
  EnGrFalsePart                                  , // 失败
  EnGrCrcErrorPart                               , // 消息有误（校验错误）
  EnGrNoSupportPart                              , // 不支持
  EnGrAlarmSurePart                                // 报警处理确认
} NetGenResEnum;


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
  unsigned char WaitLoginAck:    1;              // 等待终端登录请求应答
  unsigned char WaitHeartAck:    1;              // 等待终端心跳应答
  unsigned char WaitDataAck:     1;              // 等待终端控制数据请求应答
  unsigned char WaitAlarmAck:    1;              // 等待终端报警数据应答

  unsigned char LoginSuccuess:   1;              // 登陆成功

  unsigned char TxFlg:           1;
}NetDealBits;  


typedef struct  
{  
  unsigned short  Front                     ;  // 环形内存的队头指针 （处理操作）  
  unsigned short  Rear                      ;  // 环形内存的队尾指针 （添加内容）  
  unsigned short  Size                      ;  // 环形内存数据缓冲区的个数大小  
  unsigned char   Escape                    ;  // 环形内存的转义字符  
  unsigned char   ByEscape ;                     // 环形内存的被转义字符 
  unsigned char   Buf[cNetCrirBufSize]     ;  // 数据报文内容  
} NetDataCrir;  



typedef struct  
{  
  NetDataCrir    RxCrir;
  unsigned short RxSize;                // 每次接收数据个数大小  
  unsigned char  RxBuf[cNetRxBufSize];  // 每次接收数据缓冲区  
  
  unsigned short RxSizeBak;                // 每次接收数据个数大小  
  unsigned char  RxBufBak[cNetRxBufSize];  // 每次接收数据缓冲区  

  unsigned short TxSize;                   // 每次发送数据个数大小
  unsigned char  TxBuf[cNetTxBufSize];     // 每次发送数据缓冲区
  
  unsigned short HeartTick;             // 心跳时间
  unsigned short RxTxTick;              // 接收和发送时间

  unsigned short UpdateStateTick;       // 更新状态
  unsigned char  LoginCnt;              // 登录次数

  unsigned short WaitLoginAckDelay;     // 等待终端登录请求应答时间
  unsigned char  WaitLoginAckCnt;       // 等待终端登录请求应答次数

  unsigned short WaitHeartAckDelay;     // 等待终端心跳应答时间
  unsigned char  WaitHeartAckCnt;       // 等待终端心跳应答次数

  unsigned short WaitDataAckDelay;      // 等待终端控制数据应答时间
  unsigned char  WaitDataAckCnt;        // 等待终端控制数据应答次数

  unsigned short WaitAlarmAckDelay;     // 等待终端报警数应答时间
  unsigned char  WaitAlarmAckCnt;       // 等待终端报警数应答次数

  WORD_BYTES     TxSerial;              // 发送消息流水号(服务器应当回复的消息ID)
  WORD_BYTES     TxKeyWord;             // 发送终端消息的ID(服务器应当回复的消息流水号)
  NetTxEnum      TxMKey;                // 发送关键字
  NetTxEnum      TxMKeyBak;             // 发送关键字

  WORD_BYTES     RxKeyWord;             // 对应的平台消息的ID
  WORD_BYTES     RxSerial;              // 对应的平台消息的流水号

  WORD_BYTES     StateFlags;            // 设备状态
  WORD_BYTES     AlarmFlags;            // 报警标志
  
  unsigned char  CurrNetData;           // 当前网络数据
  unsigned char  TxAlarmCnt                  ;   // 当前发送的报警记录的条数           

  unsigned short RxDelay;               // 接收超时

  unsigned short TestDelay; 
}NetDealStru;  


typedef struct  
{  
  unsigned short  Key;  
  void           (*KeySubProgram)(void);  
} NetKeySubStru;  


/*--------------------------------------------------------------------------------------  
网络数据报文内容编码格式的全局数据结构类型定义 ：  
--------------------------------------------------------------------------------------*/  
typedef struct  
{  
  unsigned short Key                         ;  // 主关键字 （即命令字）  
  unsigned short Len                         ;  // 数据长度 （字节数）  
  unsigned char  Data[cNetDataSize]          ;  // 数据报文内容  
} NetDataStru;  


typedef struct  
{  
  unsigned short   Front                     ;  // 数据报文缓冲区队列的队头指针 （处理操作）  
  unsigned short   Rear                      ;  // 数据报文缓冲区队列的队尾指针 （添加内容）  
  unsigned short   Size                      ;  // 数据报文缓冲区队列的个数大小  
  unsigned short   Bytes                     ;  // 每个数据报文内容占用的字节长度  
  NetDataStru      Buf[cNetDataNum]          ;  // 数据报文内容 （Size×Bytes）  
} NetDataQueue;  


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
extern NetDealBits  gbitNet;
extern NetDealStru  gstvNet;

/*=======================================================================================
                                 源程序文件中的函数原型声明
=======================================================================================*/




/*!
 * \fn     Start_NetDataDeal_Monitor_Thread
 * \brief  有线网络处理操作
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_NetDataDeal_Monitor_Thread(void);


/*!
 * \fn     NetEscapeDecodingHandle
 * \brief  数据转义解码处理。
 *          
 * \param  [in] void      *pSrouce   #
 * \param  [in] void      *pDestin   #
 * \param  [in] u16        DataLen   #
 * 
 * \retval u16
 */
unsigned short NetEscapeDecodingHandle(unsigned char          *pSrouce,   
                                                 unsigned char *pDestin,  
                                                 unsigned short DataLen    
                                                 );        

/*!
 * \fn     NetEscapeCodingHandle
 * \brief  数据转义编码处理。
 *          
 * \param  [in] unsigned char         *pSrouce   #
 * \param  [in] unsigned char  *pDestin          #
 * \param  [in] unsigned short  DataLen          #
 * 
 * \retval unsigned short
 */
unsigned short NetEscapeCodingHandle(unsigned char         *pSrouce,  // 源数据指针  
                           unsigned char  *pDestin,  // 目标数据指针  
                           unsigned short  DataLen    // 源数据长度  
                           );    

/*!
 * \fn     NetDataLoginServerInit
 * \brief  重新登录服务器初始化
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataLoginServerInit(void);

/*!
 * \fn     NetDataRxCrirInit
 * \brief  数据接收队列初始化
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataRxCrirInit(void);



/*------------------------------------------------------------------------------------  
 数据〖终端通用应答〗  
-------------------------------------------------------------------------------------*/  
void  NetTxGeneralRespone(WORD_BYTES* SerialNum,
                          WORD_BYTES* KeyWord,      
                          NetGenResEnum  Results);    

/*------------------------------------------------------------------------------------  
 数据〖终端控制应答〗  
-------------------------------------------------------------------------------------*/ 
void  NetTxCtrlRespone(WORD_BYTES* SerialNum,     // 应答流水号
                       unsigned char Command,       // 命令字
                       unsigned char  Result         // 结果
                       );


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


