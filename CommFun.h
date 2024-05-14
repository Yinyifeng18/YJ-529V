/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      CommFun.h
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     通用函数头文件
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
#ifndef __CommFun_H__
#define __CommFun_H__


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "Main.h"

#include "NetDataDeal.h"


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
  unsigned short Front;                         // 环形内存的队头指针 （处理操作）  
  unsigned short Rear;                          // 环形内存的队尾指针 （添加内容）  
  unsigned short Size;                          // 环形内存的个数大小  
  unsigned char  Escape;                        // 环形内存的转义字符  
  unsigned char  ByEscape;                      // 环形内存的被转义字符 
  unsigned char  *Buf;                          // 数据缓冲区的首个地址指针  
} StruCric;  


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


/*=======================================================================================
                                 源程序文件中的函数原型声明
=======================================================================================*/

/*!
 * \fn     DataCheckAccumulateXOR
 * \brief  生成或者校验１位累加和监督码
 *          
 * \param  [in] void  *pSrcoue     #
 * \param  [in] unsigned char Length   #
 * 
 * \retval unsigned char
 */
unsigned char DataCheckAccumulateXOR(unsigned char *pSrcoue, unsigned char Length);


/*!
 * \fn     DataCheckCrcByte
 * \brief  生成或者校验１位监督码的循环冗余校验码
 *          
 * \param  [in] void *pData          #
 * \param  [in] unsigned short Len   #
 * 
 * \retval unsigned char
 */
unsigned char DataCheckCrcByte (unsigned char *pData, unsigned short Len);   

/*!
 * \fn     MemCopySramToSram
 * \brief  复制字节数据从 SRAM 到 SRAM 中
 *          
 * \param  [in] const void          *pSource   #
 * \param  [in] void                *pObject   #
 * \param  [in] const unsigned char ByteLen    #
 * 
 * \retval void
 */
void  MemCopySramToSram(unsigned char        *pSource,
                         unsigned char *pObject,
                         unsigned char ByteLen);

void  MemCopySramToSram2(unsigned char         *pSource,
                          unsigned char *pObject,
                          unsigned int  ByteLen);

/*!
 * \fn     MemClearSram
 * \brief  清除 SRAM 中的字节数据
 *          
 * \param  [in] void                *pSection   #
 * \param  [in] const unsigned char ByteLen     #
 * 
 * \retval void
 */
void MemClearSram(unsigned char      *pSection,
                       unsigned char ByteLen);

void MemClearSram2(unsigned char      *pSection,
                       unsigned int  ByteLen);
                       


/*!
 * \fn     DecompressByte1To2
 * \brief  将１字节数据解压为２个字节数据
 *          
 * \param  [in] unsigned char *pByte1   #
 * \param  [in] unsigned char *pByte2   #
 * \param  [in] unsigned char Len1      #
 * 
 * \retval void
 */
void DecompressByte1To2(unsigned char *pByte1,
                               unsigned char *pByte2,
                               unsigned char Len1);
                               

/*!
 * \fn     CompressByte2To1
 * \brief  将２字节数据压缩为１个字节数据
 *          
 * \param  [in] unsigned char *pByte2   #
 * \param  [in] unsigned char *pByte1   #
 * \param  [in] unsigned char Len2      #
 * 
 * \retval void
 */
void CompressByte2To1(unsigned char *pByte2,
                             unsigned char *pByte1,
                             unsigned char Len2);
                             

/*!
 * \fn     AtHexConvAsc
 * \brief  十六进制数转换为ＡＳＣⅡ 字符码的字符串。
 *          
 * \param  [in] const void *pHex             #
 * \param  [in] void *pAsc                   #
 * \param  [in] const unsigned char HexLen   #
 * 
 * \retval void
 */
void AtHexConvAsc(unsigned char *pHex, unsigned char *pAsc, unsigned char HexLen);

/*!
 * \fn     AtAscConvHex
 * \brief  ＡＳＣⅡ 字符码的字符串转换为十六进制数。
 *          
 * \param  [in] unsigned char *pAsc    #
 * \param  [in] unsigned char *pHex    #
 * \param  [in] unsigned char AscLen   #
 * 
 * \retval void
 */
void AtAscConvHex(unsigned char *pAsc, unsigned char *pHex, unsigned char AscLen);

/*!
 * \fn     QueueInit
 * \brief  数据缓冲区队列初始化
 *          
 * \param  [in] NetDataQueue    *pQueue   #
 * \param  [in] unsigned short Len        #
 * \param  [in] unsigned short Size       #
 * \param  [in] unsigned short Bytes      #
 * 
 * \retval void
 */
void QueueInit(NetDataQueue      *pQueue,   
                   unsigned short Len,     
                   unsigned short Size,   
                   unsigned short Bytes);


/*!
 * \fn     CircBusy
 * \brief  查询环形内存中是否有完整的数据
 *          
 * \param  [in] unsigned char    *pCirc   # 指向数据缓冲区队列的首个地址指针  
 * 
 * \retval bool char
 */
bool  CircBusy(unsigned char *pCirc);    
                   
/*!
 * \fn     CircGain
 * \brief  从环形内存中获取数据
 *          
 * \param  [in] unsigned char *pCirc   #
 * \param  [in] unsigned char *pDst    #
 * 
 * \retval unsigned short
 */
unsigned short CircGain(unsigned char *pCirc, unsigned char *pDst);


/*!
 * \fn     CircStatus
 * \brief  查询环形内存状态
 *          
 * \param  [in] void                 *pCirc   #
 * \param  [in] const unsigned short Len      #
 * 
 * \retval signed char
 */
signed char CircStatus(StruCric *pCirc,     
                       unsigned short Len
                      );

/*!
 * \fn     CircInit
 * \brief  环形内存初始化 
 *          
 * \param  [in] void                  *pCirc   #
 * \param  [in] unsigned short       Len       #
 * \param  [in] const unsigned short Size      #
 * \param  [in] const unsigned char Escape     #
 * 
 * \retval void
 */
void CircInit(unsigned char    *pCirc,      
                 unsigned short Len,        
                 unsigned short Size,      
                 unsigned char Escape);  

/*!
 * \fn     CircInsert
 * \brief  插入环形内存
 *          
 * \param  [in] StruCric *pCirc       #
 * \param  [in] unsigned char *pSrc   #
 * \param  [in] unsigned short Len    #
 * 
 * \retval signed char
 */
signed char CircInsert(unsigned char *pCirc,     
                        unsigned char *pSrc,       
                        unsigned short Len);


/*!
 * \fn     QueueStatus
 * \brief  查询数据缓冲区队列状态
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueStatus(NetDataQueue *pQueue);


/*!
 * \fn     QueueInsert
 * \brief  插入数据缓冲区队列
 *          
 * \param  [in] void         *pQueue   #
 * \param  [in] void         *pSrc     #
 * \param  [in] unsigned short Len     #
 * 
 * \retval signed char
 */
signed char QueueInsert(unsigned char     *pQueue,   
                          unsigned char *pSrc,     
                          unsigned short Len);

                          
/*!
 * \fn     QueueBusy
 * \brief  查询数据缓冲区队列的状态
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueBusy(NetDataQueue *pQueue);



/*!
 * \fn     QueueDelete
 * \brief  删除数据缓冲区队列
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval void
 */
void QueueDelete(NetDataQueue *pQueue);


/*!
 * \fn     DataCheckAccumulateCRC16
 * \brief  生成或者校验2位累加和监督码
 *          
 * \param  [in] void					 *pSrcoue      #
 * \param  [in] unsigned short Length   #
 * 
 * \retval unsigned short
 */
unsigned short DataCheckAccumulateCRC16(unsigned char *pSrcoue,
                                         unsigned short Length);


/*!
 * \fn     MemCompSramAndSram
 * \brief  比较 SRAM 与 SRAM 中的数据函数
 *          
 * \param  [in] const void         *pSource   #
 * \param  [in] const void         *pObject   #
 * \param  [in] const unsigned char ByteLen   #
 * 
 * \retval signed char
 */
signed char  MemCompSramAndSram(unsigned char         *pSource,
                                       unsigned char *pObject,
                                       unsigned char ByteLen);

signed char MemCompSramAndSram2(unsigned char  *pSource,
                                        unsigned char *pObject,
                                        unsigned int  ByteLen);
                                        

/*!
 * \fn     ConvBin8ToBcd3
 * \brief  用于８位二进制数到３个ＢＣＤ码转换运算
 *          
 * \param  [in] unsigned char binary_value   #
 * 
 * \retval unsigned int
 */
unsigned int ConvBin8ToBcd3 (unsigned char binary_value);



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


