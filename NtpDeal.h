/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      NtpDeal.h
 * \author    yifeng
 * \version   1.0
 * \date      2023年3月11日
 * \brief     NTP处理头文件
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年3月11日
      author: yifeng
      change: create file

*****************************************************************************************/


/*=======================================================================================
下面两条指令是本头文件条件编译的预处理指令，用于防止本头文件被其他文件重复引用。
=======================================================================================*/
#ifndef __NtpDeal_H__
#define __NtpDeal_H__

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
//授时服务器 端口默认 123
#define DEF_NTP_SERVER "ntp.neu.edu.cn"

#define DEF_NTP_SERVER_IP "202.112.31.197"
#define DEF_NTP_PORT 123


/*---------------------------------------------------------------------------------------
的字符化常数定义：
----------------------------------------------------------------------------------------*/
//默认请求数据包填充
#define LI          0   //协议头中的元素
#define VN          3   //版本
#define MODE        3 //模式 : 客户端请求
#define STRATUM     0
#define POLL        4  //连续信息间的最大间隔
#define PREC        -6 //本地时钟精度

//ntp时间从年开始，本地时间从年开始，这是两者之间的差值
#define JAN_1970 0x83aa7e80 //3600s*24h*(365days*70years+17days)
//x*10^(-6)*2^32 微妙数转 NtpTime 结构的 fraction 部分
#define NTPFRAC(x) (4294 * (x) + ((1981 * (x)) >> 11))
//NTPFRAC的逆运算
#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))


#define MKSEC(ntpt) ((ntpt).integer - JAN_1970)
#define MKUSEC(ntpt) (USEC((ntpt).fraction))
#define TTLUSEC(sec, usec) ((long long)(sec)*1000000 + (usec))
#define GETSEC(us) ((us) / 1000000)
#define GETUSEC(us) ((us) % 1000000)


#define DATA(i) ntohl(((unsigned int *)data)[i])

#define PDEBUG(fmt, args...) printf("[%s:%d]" fmt "\n", __func__, __LINE__, ##args)



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
//ntp时间戳结构
typedef struct
{
  unsigned int integer;
  unsigned int fraction;
} NtpTime;

typedef enum 
{
  EnNrInit      =0  ,  // 初始化
  EnNrConnect       ,  // 连接
  EnNrRun           ,  // 运行
  EnNrFail          ,  // 失败
  EnNrFailDelay     ,  // 延时
}NtpStateEnum;

typedef struct 
{
  NtpStateEnum   State;
  u16            Delay;
  u8             ErrorCnt;
  int            sock;

  unsigned short GetTimeDelay;
}NtpDealStru;

typedef struct 
{
  u8     HaveSocket:   1;
  u8     RefreshNtp:   1;
}NtpDealBits;


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
extern NtpDealStru gstvNtp;
extern NtpDealBits gbitNtp;

/*=======================================================================================
                                 源程序文件中的函数原型声明
=======================================================================================*/



/*!
 * \fn     Start_NtpDeal_Monitor_Thread
 * \brief  网络时间获取
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_NtpDeal_Monitor_Thread(void);



/*!
 * \fn     NtpDealRestartHandle
 * \brief  重启Ntp
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NtpDealRestartHandle(void);






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


