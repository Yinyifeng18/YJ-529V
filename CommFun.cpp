/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      CommFun.c
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     通用函数
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
#include "CommFun.h"

#include "NetDataDeal.h"

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


/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
*****************************************************************************************
*/


/*!
 * \fn     DataCheckAccumulateXOR
 * \brief  生成或者校验１位累加和监督码
 *          
 * \param  [in] void  *pSrcoue     #
 * \param  [in] unsigned char Length   #
 * 
 * \retval unsigned char
 */
unsigned char DataCheckAccumulateXOR(unsigned char *pSrcoue, unsigned char Length)
{
  unsigned char *pData;
  unsigned char i;
  unsigned char sum;
  
  pData = pSrcoue;
  
  sum = *pData;
  pData++;
  
  for (i=0; i<Length-1; i++)
  {
    sum  ^= *pData;
    pData++;
  }
  
  return(sum);
}      



/*!
 * \fn     DataCheckCrcByte
 * \brief  生成或者校验１位监督码的循环冗余校验码
 *          
 * \param  [in] void *pData          #
 * \param  [in] unsigned short Len   #
 * 
 * \retval unsigned char
 */
unsigned char  DataCheckCrcByte (unsigned char *pData, unsigned short Len)   
{
  unsigned char i;       // 临时寄存器变量
  unsigned char check;                        // 校验数据序列中的数据变量
  unsigned char seed;                         // 余式R(X)（ＣＲＣ校验字节）
  unsigned char *pDat;

  pDat  =  pData;
  seed  =  *pDat++;                // 笫１个数据赋值于余式R(X)
  Len   =  Len - 1;                // 校验数据序列长度减去１位ＣＲＣ码长度

  while (Len-- != 0)
  {
    check         = *(pDat++);               // 取下一个校验数据序列中的数据
    for (i=0; i<8; i++)
    {
      if((seed & 0x80) == 0x80) // 判断余式R(X)的的高位是否为１？
      {
        seed      =  seed << 1;               // 余式R(X)左移１位
        // 将校验数据的高位（位7）移进余式R(X)的低位（位0）
        if((check & 0x80) == 0x80)// 判断校验数据的高位是否为１？
        {
          seed   |=  0x01;              // 余式R(X)的低位由０变１
        }
        check     =  check << 1;              // 校验数据左移１位
        seed     ^=  0x8C;          // 余式R(X)与除数多项式异或
      }
      // 如果判断余式R(X)的高位不为１，那么只进行移位而不做与除数多项式异或
      else
      {
        seed      =  seed << 1;               // 余式R(X)左移１位
        // 将校验数据的高位（位7）移进余式R(X)的低位（位0）
        if((check & 0x80) == 0x80)// 判断校验数据的高位是否为１？
        {
          seed   |=  0x01;              // 余式R(X)的低位由０变１
        }
        check     =  check << 1;              // 校验数据左移１位
      }
    }
  }
  
  return(seed);                               // 返回一个字节的监督码（ＣＲＣ码）
}



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
                         unsigned char ByteLen)
{
  unsigned char       i;
  unsigned char       temp;
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}

void  MemCopySramToSram2(unsigned char         *pSource,
                          unsigned char *pObject,
                          unsigned int  ByteLen)
{
  unsigned short       ii;
  unsigned char       temp;
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (ii=0; ii<ByteLen; ii++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}


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
                       unsigned char ByteLen)
{
  unsigned char  i;
  unsigned char  *ptr;

  ptr     =  pSection;
  for (i=0; i<ByteLen; i++)
  {
    *ptr  =  0x00;
    ptr++;
  }
}


void MemClearSram2(unsigned char      *pSection,
                       unsigned int  ByteLen)
{
  unsigned int   ii;
  unsigned char  *ptr;

  ptr     =  pSection;
  for (ii=0; ii<ByteLen; ii++)
  {
    *ptr  =  0x00;
    ptr++;
  }
}

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
                               unsigned char Len1)
{
  unsigned char *pSrc;
  unsigned char *pDst;
  unsigned char temp;

  pSrc =  pByte1;
  pDst =  pByte2;
  while (Len1)
  {
    temp   =  *pSrc;
    *pDst  =  (temp >> 4) & 0x0F;
    pDst++;
    *pDst  =  (temp & 0x0F);
    pDst++;
    pSrc++;
    Len1--;
  }
}

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
                             unsigned char Len2)
{
  unsigned char *pSrc;
  unsigned char *pDst;

  pSrc =  pByte2;
  pDst =  pByte1;
  Len2 >>=  1;
  
  while (Len2)
  {
    *pDst       =  (*pSrc << 4);
    pSrc++;
    *pDst      +=  (*pSrc & 0x0F);
    pSrc++;
    pDst++;
    Len2--;
  }
}

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
void AtHexConvAsc(unsigned char *pHex, unsigned char *pAsc, unsigned char HexLen)
{
  unsigned char   i;
  unsigned char   *pSrc;
  unsigned char   *pDst;
  unsigned char   temp;

  pSrc      =  pHex;
  pDst      =  pAsc;
  
  for(i=0; i<HexLen; i++)
  {
    temp    =  *pSrc;
    if (temp > 9)
    {
      temp   +=  'A' - 10;
    }
    else
    {
      temp   +=  '0';
    }
    *pDst   =  temp;
    pSrc++;
    pDst++;
  }
}


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
void AtAscConvHex(unsigned char *pAsc, unsigned char *pHex, unsigned char AscLen)
{
  unsigned int  i;
  unsigned char *pSrc;
  unsigned char *pDst;
  unsigned char temp;

  pSrc      =  pAsc;
  pDst      =  pHex;
  for(i=0; i<AscLen; i++)
  {
    temp    =  *pSrc;
    if (temp >= 'a')
    {
      temp   -=  'a'- 10;
    }
    else if (temp >= 'A')
    {
      temp   -=  'A'- 10;
    }
    else
    {
      temp   -=  '0';
    }
    *pDst   =  temp;
    pSrc++;
    pDst++;
  }
}



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
                   unsigned short Bytes)  
{
  NetDataQueue *pQue;   
  
  pQue = pQueue;  
  
  memset(pQue, 0, Len);
  
  pQue->Size = Size;                          

  pQue->Bytes = Bytes;                        
}




/*!
 * \fn     CircBusy
 * \brief  查询环形内存中是否有完整的数据
 *          
 * \param  [in] unsigned char    *pCirc   # 指向数据缓冲区队列的首个地址指针  
 * 
 * \retval bool char
 */
bool CircBusy(unsigned char *pCirc)    
{  
  unsigned short front;                         // 队头指针变量  
  unsigned short rear;                          // 队尾指针变量  
  unsigned short size;                          // 数据缓冲区队列的个数大小变量  
  unsigned char  escape;                        // 数据缓冲区队列的转义字符  
  unsigned char  *ptr;                          // 数据序列缓冲区指针变量  
  StruCric       *pCir;                         // 指向数据缓冲区队列的指针变量  
  unsigned char  flg;                           // 有数据标志  
  unsigned char  Headflg;                      // 头标志  
  unsigned short DstLen;                        // 目标文件数据的长度  
  unsigned char  *pTemp;  
  
  flg       = 0x00;  
  Headflg   = 0x00;  
  DstLen    = 0x00;  
  pCir      =  (StruCric *)pCirc;                           // 指针指向数据缓冲区队列的首个地址  
  
  front     = pCir->Front;                             // 为队头指针赋值  
  rear      = pCir->Rear;                            // 为队尾指针赋值  
  size      = pCir->Size;                            // 为数据缓冲区队列的个数大小赋值  
  escape    = pCir->Escape;  
  pTemp     = (unsigned char  *)&pCir->Buf;  
  ptr       = pTemp;  
  ptr      += front;  
  
  while(front != rear)  
  {  
    if(*ptr == escape)  
    {  
      // 头  
      if(Headflg == 0x00)  
      {  
        Headflg = 0x01;  
        
        ptr++;  
        front++;  
        if(front >= size)  
        {  
          front = 0x00;  
          
          pTemp = (unsigned char  *)&pCir->Buf;  
          ptr   = pTemp;  
        }  
      }  
      else  
      {  
        if(DstLen == 0x00)  
        {  
          ptr++;  
          front++;  
          if(front >= size)  
          {  
            front = 0x00;  
            pTemp = (unsigned char  *)&pCir->Buf;  
            ptr   = pTemp;  
          }  
        }  
        else  
        {  
          flg   = 0x01;  
          
          break;  
        }  
      }  
    }  
    else  
    {  
      if(Headflg == 0x01)  
      {  
        DstLen++;  
      }  
      
      ptr++;  
      front++;  
      if(front >= size)  
      {  
        front = 0x00;  
        pTemp = (unsigned char  *)&pCir->Buf;  
        ptr   = pTemp;  
      }  
    }  
  }  
  
  if(flg == 0x01)  
  {  
    return(true);  
  }  
  else  
  {  
    return(false);  
  }  
}  
 

/*!
 * \fn     CircGain
 * \brief  从环形内存中获取数据
 *          
 * \param  [in] unsigned char *pCirc   #
 * \param  [in] unsigned char *pDst    #
 * 
 * \retval unsigned short
 */
unsigned short CircGain(unsigned char *pCirc, unsigned char *pDst)     
{  
  
  unsigned short front;                         // 队头指针变量  
  unsigned short rear;                          // 队尾指针变量  
  unsigned short size;                          // 数据缓冲区队列的个数大小变量  
  unsigned char  escape;                        // 数据缓冲区队列的转义字符  
  unsigned short DstLen;                        // 目标文件数据的长度  
  unsigned char  *ptr;                          // 数据序列缓冲区指针变量  
  StruCric       *pCir;                         // 指向数据缓冲区队列的指针变量  
  unsigned char  *pSrc;  
  unsigned char  Headflg;                      // 头标志  
  unsigned char  *pTemp;  
    

  Headflg   = 0x00;  
  pCir      =  (StruCric *)pCirc;                        

  ptr        =  pDst;  
  DstLen    =  0x00;  
 
  front     = pCir->Front;                           // 为队头指针赋值  
  rear      = pCir->Rear;                            // 为队尾指针赋值  
  size      = pCir->Size;                            // 为数据缓冲区队列的个数大小赋值  
  escape    = pCir->Escape;  
  pTemp     = (unsigned char *)&pCir->Buf;  
  pSrc      = pTemp;  
  pSrc     += front;       // 定位到队头指针对应的缓存地址
 
  while(front != rear)  
  {
    if(*pSrc == escape)      // 找到转义符，判断是否是前/后标识符
    {  
      if(Headflg == 0x00)  // 前标识符
      {  
        Headflg = 0x01;  // 已经找到前标识符
 
        pSrc++;    // 继续往下寻找转义符
        front++;   // 环形内存头往下加1
          
        if(front >= size)  // 超过尾部，重新返回到队头0
        {  
          front = 0x00;  
          pTemp     = (unsigned char *)&pCir->Buf;  //返回到首个缓存器
          pSrc      = pTemp;  
        }   
      }  
      else                  // 找到后标识符
      {  
        if(DstLen == 0x00)  // 目标长度为0，表示前面数据出错
        {  
          pSrc++;  
          front++;  
          if(front >= size)  
          {  
            front = 0x00;  
            pTemp     = (unsigned char *)&pCir->Buf;  
            pSrc      = pTemp;  
          }  
        }  
        else               // 数据正常，可以输出目标数据
        {  
          front++;  
          if(front >= size)  
          {  
            front = 0x00;  
            pTemp     = (unsigned char *)&pCir->Buf;  
            pSrc      = pTemp;  
          }  
          break;  
        }  
      }  
    }  
    else                      // 第1个字节不是转义符
    {  
      if(Headflg == 0x01)  
      {  
        *ptr    = *pSrc;  // 把数据拷到目标缓存区中
        ptr++;  
        DstLen++;  
      }  
        
      pSrc++;    // 继续往下寻找转义符
      front++;   // 环形内存头往下加1
        
      if(front >= size)  // 超过尾部，重新返回到队头0
      {  
        front = 0x00;  
        pTemp     = (unsigned char *)&pCir->Buf;  //返回到首个缓存器
        pSrc      = pTemp;  
      }  
    }  
  }  
 
  if(DstLen != 0x00)  
  {  
    pCir->Front=  front;  
  }  
  
  return(DstLen);  
}  

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
                      )  
{  
  unsigned short temp;                          // 临时变量  
  unsigned short front;                         // 队头指针变量  
  unsigned short rear;                          // 队尾指针变量  
  unsigned short size;                          // 数据缓冲区队列的个数大小变量  
  StruCric  *pCir;  
  
  pCir      =  pCirc;                          // 指针指向数据缓冲区队列的首个地址  
  front     = pCir->Front;  
  rear      = pCir->Rear;  
  size      = pCir->Size;  
   
   /* 判断数据空间还有多少 */   
  if (rear >= front)                           // 队尾指针 ≥队头指针，队列为正序  
  {  
    temp    =  (size - rear) + front;  
  }  
  else                                         // 队尾指针 ＜队头指针，队列为逆序  
  {  
    temp    =  front - rear;  
  }  
 
  if(Len >= temp)                              // 检查队要插入的队列个数是否大于等于空队列的个数 ？  
  {  
    return(0);                              // 是，返回 “FILL”（完全填满）值  
  }  
  else  
  {  
    return(1);                             // 否，返回 “EMPTY”（未填满）值  
  }  
 
    
}  


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
                 unsigned char Escape)    
{  
  StruCric  *pCir;  
  
  pCir = (StruCric *)pCirc;  
  
  memset(pCir, 0, Len);  
  
  pCir->Size   = Size;  
  pCir->Escape = Escape;  
}  


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
                        unsigned short Len)     
{  
  unsigned short rear;                         // 队尾指针变量  
  unsigned short size;                         // 数据缓冲区队列的个数大小变量  
  unsigned char *ptr;                          // 数据序列缓冲区指针变量  
  StruCric      *pCir;                         // 指向数据缓冲区队列的指针变量  
  unsigned short i;  
  unsigned char *pBuf;  
  unsigned char *pTemp;  
  
  pCir      =  (StruCric *)pCirc;                         // 指针指向数据缓冲区队列的首个地址  

  ptr      =  pSrc;  
    
  if (CircStatus(pCir, Len) == 1)              // 检查数据缓冲区队列是否有存储空间 ？  
  { 
    rear   = pCir->Rear;                       // 为队尾指针赋值  
    size   = pCir->Size;                       // 为数据缓冲区队列的个数大小赋值  
    pTemp  = (unsigned char *)&pCir->Buf;  
    pBuf   = pTemp;                            
    pBuf  += rear;                             // 数据缓冲区的数据头指针  
    
    for(i=0; i<Len; i++)  
    {  
      *pBuf = *ptr;  
      pBuf++;  
      ptr++;  
      rear++;  
      if(rear >= size)  
      {  
        rear  = 0x00;  
        pTemp = (unsigned char *)&pCir->Buf;  
        pBuf  = pTemp;  
      }  
    }  
    
    pCir->Rear =  rear;  
    return(1);  
  }  
  
  return(0);  
} 




/*!
 * \fn     QueueStatus
 * \brief  查询数据缓冲区队列状态
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueStatus(NetDataQueue *pQueue)    
{
  unsigned short temp;                         // 临时变量
  unsigned short front;                        // 队头指针变量
  unsigned short rear;                         // 队尾指针变量
  unsigned short size;                         // 数据缓冲区队列的个数大小变量
  NetDataQueue *pQue;                        // 指向数据缓冲区队列的指针变量

  pQue      =  pQueue;                        // 指针指向数据缓冲区队列的首个地址
                                              // 注意：队头指针必须放在队列结构的首个地址中
  front     = pQue->Front;                    // 为队头指针赋值

  rear      = pQue->Rear;                     // 为队尾指针赋值

  size      = pQue->Size;                     // 为数据缓冲区队列的个数大小赋值

  temp      =  rear + 1;                      // 置队尾指针为下一个指针
  if (temp >= size)
  {
    temp    =  0;
  }

  if(temp == front)                           // 检查队尾下一个指针是否等于队头指针？
  {
    return(0);                             // 是，返回“FILL”（完全填满）值
  }
  else
  {
    return(1);                            // 否，返回“EMPTY”（未填满）值
  }
}


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
signed char QueueInsert(unsigned char *pQueue,   
                          unsigned char *pSrc,     
                          unsigned short Len)     

{
  unsigned short rear;                         // 队尾指针变量
  unsigned short size;                         // 数据缓冲区队列的个数大小变量
  unsigned short byte;                         // 每个数据缓冲区占用的字节长度变量
  unsigned char *ptr;                         // 数据序列缓冲区指针变量
  NetDataQueue *pQue;                        // 指向数据缓冲区队列的指针变量

  pQue      =  (NetDataQueue *)pQueue;                        // 指针指向数据缓冲区队列的首个地址
  if (QueueStatus(pQue) == 1)             // 检查数据缓冲区队列是否有存储空间？
  {
    rear      = pQue->Rear;                   // 为队尾指针赋值
    size      = pQue->Size;                   // 为数据缓冲区队列的个数大小赋值
    byte      = pQue->Bytes;                  // 为每个数据缓冲区占用的字节长度赋值
    ptr       = pQueue;
    ptr      += 8;
    ptr      +=  rear * byte;                   // 指针指向队尾指针所示的数据缓冲区地址处
    MemCopySramToSram2(pSrc, ptr, Len);

    rear   +=  1;                             // 数据缓冲区队列的队尾指针加１
    if (rear >= size)                         // 检查队尾指针是否大于等于最大值？
    {
      rear  =  0x00;                          // 数据缓冲区队列的队尾指针归０
    }

    pQue->Rear   =  rear;                     // 返回新的数据缓冲区队列的队尾指针值

    return(1);
  }

  return(0);
}



/*!
 * \fn     QueueBusy
 * \brief  查询数据缓冲区队列的状态
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueBusy(NetDataQueue *pQueue)  
{
  unsigned short front;                        // 队头指针变量
  unsigned short rear;                         // 队尾指针变量
  NetDataQueue *pQue;                        // 指向数据缓冲区队列的指针变量

  pQue      =  pQueue;                        // 指针指向数据缓冲区队列的首个地址
                                              // 注意：队头指针必须放在队列结构的首个地址中
  front     = pQue->Front;                    // 为队头指针赋值

  rear      = pQue->Rear;                     // 为队尾指针赋值
  
  if (front != rear)
  {
    return(TRUE);
  }
  return(FALSE);
} 


/*!
 * \fn     QueueDelete
 * \brief  删除数据缓冲区队列
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval void
 */
void QueueDelete(NetDataQueue *pQueue)
{
  unsigned short front;                      
  unsigned short rear;                         
  unsigned short size;                         
  unsigned short byte;                        
  NetDataQueue *pQue;                         

  pQue      =  pQueue;                        
                                               
  front     = pQue->Front;                     

  rear      = pQue->Rear;                     

  if(front != rear)
  {
    size      = pQue->Size;                          
    
    byte      = pQue->Bytes;                          
    MemClearSram2((unsigned char *)&pQue->Buf[front], byte);
    
    front    +=  1;                            
    if (front >= size)                          
    {
      front   =  0x00;                     
    }
    
    pQue->Front     =  front;                        
  }
}


 

const unsigned char cCrcCrc16Tab16High[]  =  
{
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
  0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
  0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
  0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
  0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
  0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
  0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
  0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
  0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
  0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
  0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
  0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
  0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
  0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
  0x80,0x41,0x00,0xC1,0x81,0x40
};
const unsigned char cCrcCrc16Tab16Low[]  =  
{
  0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
  0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
  0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
  0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
  0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
  0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
  0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
  0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
  0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
  0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
  0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
  0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
  0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
  0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
  0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
  0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
  0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
  0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
  0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
  0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
  0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
  0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
  0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
  0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
  0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
  0x43,0x83,0x41,0x81,0x80,0x40
};



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
                                         unsigned short Length) 
{
  unsigned char  *pData;
  unsigned char  chCRCHi = 0xff;
  unsigned char  chCRCLo = 0xff;
  unsigned short wIndex;
  
  pData = pSrcoue;
  
  while(Length--)
  {
    wIndex = chCRCLo ^ *pData++;
    chCRCLo= chCRCHi ^ cCrcCrc16Tab16High[wIndex];
    chCRCHi= cCrcCrc16Tab16Low[wIndex];
  }
  
  return ((chCRCHi << 8) | chCRCLo); 
}

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
                                       unsigned char ByteLen)
{
  unsigned char       i;
  unsigned char *pSrc;
  unsigned char *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    if (*pSrc != *pDst)
    {
      return(1);
    }
    pSrc++;
    pDst++;
  }
  return(0);
}

signed char MemCompSramAndSram2(unsigned char         *pSource,
                                        unsigned char *pObject,
                                        unsigned int  ByteLen)
{
  unsigned int       i;
  unsigned char *pSrc;
  unsigned char *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    if (*pSrc != *pDst)
    {
      return(1);
    }
    pSrc++;
    pDst++;
  }
  return(0);
}

/*!
 * \fn     ConvBin8ToBcd3
 * \brief  用于８位二进制数到３个ＢＣＤ码转换运算
 *          
 * \param  [in] unsigned char binary_value   #
 * 
 * \retval unsigned int
 */
unsigned int ConvBin8ToBcd3 (unsigned char binary_value)
{
  unsigned char   ByteTemp;
  unsigned short  BcdVal = 0;
  
  ByteTemp = binary_value;
  
  BcdVal    |= (ByteTemp/100)<<8;
  ByteTemp  %= 100;
  BcdVal    |= (ByteTemp/10)<<4;
  ByteTemp  %= 10;
  BcdVal    |= ByteTemp;
  
  return(BcdVal);
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
