/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      CommFun.c
 * \author    yifeng
 * \version   1.0
 * \date      2023��6��30��
 * \brief     ͨ�ú���
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
#include "CommFun.h"

#include "NetDataDeal.h"

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


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/


/*!
 * \fn     DataCheckAccumulateXOR
 * \brief  ���ɻ���У�飱λ�ۼӺͼල��
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
 * \brief  ���ɻ���У�飱λ�ල���ѭ������У����
 *          
 * \param  [in] void *pData          #
 * \param  [in] unsigned short Len   #
 * 
 * \retval unsigned char
 */
unsigned char  DataCheckCrcByte (unsigned char *pData, unsigned short Len)   
{
  unsigned char i;       // ��ʱ�Ĵ�������
  unsigned char check;                        // У�����������е����ݱ���
  unsigned char seed;                         // ��ʽR(X)���ãң�У���ֽڣ�
  unsigned char *pDat;

  pDat  =  pData;
  seed  =  *pDat++;                // �ʣ������ݸ�ֵ����ʽR(X)
  Len   =  Len - 1;                // У���������г��ȼ�ȥ��λ�ãң��볤��

  while (Len-- != 0)
  {
    check         = *(pDat++);               // ȡ��һ��У�����������е�����
    for (i=0; i<8; i++)
    {
      if((seed & 0x80) == 0x80) // �ж���ʽR(X)�ĵĸ�λ�Ƿ�Ϊ����
      {
        seed      =  seed << 1;               // ��ʽR(X)���ƣ�λ
        // ��У�����ݵĸ�λ��λ7���ƽ���ʽR(X)�ĵ�λ��λ0��
        if((check & 0x80) == 0x80)// �ж�У�����ݵĸ�λ�Ƿ�Ϊ����
        {
          seed   |=  0x01;              // ��ʽR(X)�ĵ�λ�ɣ��䣱
        }
        check     =  check << 1;              // У���������ƣ�λ
        seed     ^=  0x8C;          // ��ʽR(X)���������ʽ���
      }
      // ����ж���ʽR(X)�ĸ�λ��Ϊ������ôֻ������λ���������������ʽ���
      else
      {
        seed      =  seed << 1;               // ��ʽR(X)���ƣ�λ
        // ��У�����ݵĸ�λ��λ7���ƽ���ʽR(X)�ĵ�λ��λ0��
        if((check & 0x80) == 0x80)// �ж�У�����ݵĸ�λ�Ƿ�Ϊ����
        {
          seed   |=  0x01;              // ��ʽR(X)�ĵ�λ�ɣ��䣱
        }
        check     =  check << 1;              // У���������ƣ�λ
      }
    }
  }
  
  return(seed);                               // ����һ���ֽڵļල�루�ãң��룩
}



/*!
 * \fn     MemCopySramToSram
 * \brief  �����ֽ����ݴ� SRAM �� SRAM ��
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
 * \brief  ��� SRAM �е��ֽ�����
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
 * \brief  �����ֽ����ݽ�ѹΪ�����ֽ�����
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
 * \brief  �����ֽ�����ѹ��Ϊ�����ֽ�����
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
 * \brief  ʮ��������ת��Ϊ���ӣâ� �ַ�����ַ�����
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
 * \brief  ���ӣâ� �ַ�����ַ���ת��Ϊʮ����������
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
 * \brief  ���ݻ��������г�ʼ��
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
 * \brief  ��ѯ�����ڴ����Ƿ�������������
 *          
 * \param  [in] unsigned char    *pCirc   # ָ�����ݻ��������е��׸���ַָ��  
 * 
 * \retval bool char
 */
bool CircBusy(unsigned char *pCirc)    
{  
  unsigned short front;                         // ��ͷָ�����  
  unsigned short rear;                          // ��βָ�����  
  unsigned short size;                          // ���ݻ��������еĸ�����С����  
  unsigned char  escape;                        // ���ݻ��������е�ת���ַ�  
  unsigned char  *ptr;                          // �������л�����ָ�����  
  StruCric       *pCir;                         // ָ�����ݻ��������е�ָ�����  
  unsigned char  flg;                           // �����ݱ�־  
  unsigned char  Headflg;                      // ͷ��־  
  unsigned short DstLen;                        // Ŀ���ļ����ݵĳ���  
  unsigned char  *pTemp;  
  
  flg       = 0x00;  
  Headflg   = 0x00;  
  DstLen    = 0x00;  
  pCir      =  (StruCric *)pCirc;                           // ָ��ָ�����ݻ��������е��׸���ַ  
  
  front     = pCir->Front;                             // Ϊ��ͷָ�븳ֵ  
  rear      = pCir->Rear;                            // Ϊ��βָ�븳ֵ  
  size      = pCir->Size;                            // Ϊ���ݻ��������еĸ�����С��ֵ  
  escape    = pCir->Escape;  
  pTemp     = (unsigned char  *)&pCir->Buf;  
  ptr       = pTemp;  
  ptr      += front;  
  
  while(front != rear)  
  {  
    if(*ptr == escape)  
    {  
      // ͷ  
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
 * \brief  �ӻ����ڴ��л�ȡ����
 *          
 * \param  [in] unsigned char *pCirc   #
 * \param  [in] unsigned char *pDst    #
 * 
 * \retval unsigned short
 */
unsigned short CircGain(unsigned char *pCirc, unsigned char *pDst)     
{  
  
  unsigned short front;                         // ��ͷָ�����  
  unsigned short rear;                          // ��βָ�����  
  unsigned short size;                          // ���ݻ��������еĸ�����С����  
  unsigned char  escape;                        // ���ݻ��������е�ת���ַ�  
  unsigned short DstLen;                        // Ŀ���ļ����ݵĳ���  
  unsigned char  *ptr;                          // �������л�����ָ�����  
  StruCric       *pCir;                         // ָ�����ݻ��������е�ָ�����  
  unsigned char  *pSrc;  
  unsigned char  Headflg;                      // ͷ��־  
  unsigned char  *pTemp;  
    

  Headflg   = 0x00;  
  pCir      =  (StruCric *)pCirc;                        

  ptr        =  pDst;  
  DstLen    =  0x00;  
 
  front     = pCir->Front;                           // Ϊ��ͷָ�븳ֵ  
  rear      = pCir->Rear;                            // Ϊ��βָ�븳ֵ  
  size      = pCir->Size;                            // Ϊ���ݻ��������еĸ�����С��ֵ  
  escape    = pCir->Escape;  
  pTemp     = (unsigned char *)&pCir->Buf;  
  pSrc      = pTemp;  
  pSrc     += front;       // ��λ����ͷָ���Ӧ�Ļ����ַ
 
  while(front != rear)  
  {
    if(*pSrc == escape)      // �ҵ�ת������ж��Ƿ���ǰ/���ʶ��
    {  
      if(Headflg == 0x00)  // ǰ��ʶ��
      {  
        Headflg = 0x01;  // �Ѿ��ҵ�ǰ��ʶ��
 
        pSrc++;    // ��������Ѱ��ת���
        front++;   // �����ڴ�ͷ���¼�1
          
        if(front >= size)  // ����β�������·��ص���ͷ0
        {  
          front = 0x00;  
          pTemp     = (unsigned char *)&pCir->Buf;  //���ص��׸�������
          pSrc      = pTemp;  
        }   
      }  
      else                  // �ҵ����ʶ��
      {  
        if(DstLen == 0x00)  // Ŀ�곤��Ϊ0����ʾǰ�����ݳ���
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
        else               // �����������������Ŀ������
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
    else                      // ��1���ֽڲ���ת���
    {  
      if(Headflg == 0x01)  
      {  
        *ptr    = *pSrc;  // �����ݿ���Ŀ�껺������
        ptr++;  
        DstLen++;  
      }  
        
      pSrc++;    // ��������Ѱ��ת���
      front++;   // �����ڴ�ͷ���¼�1
        
      if(front >= size)  // ����β�������·��ص���ͷ0
      {  
        front = 0x00;  
        pTemp     = (unsigned char *)&pCir->Buf;  //���ص��׸�������
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
 * \brief  ��ѯ�����ڴ�״̬
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
  unsigned short temp;                          // ��ʱ����  
  unsigned short front;                         // ��ͷָ�����  
  unsigned short rear;                          // ��βָ�����  
  unsigned short size;                          // ���ݻ��������еĸ�����С����  
  StruCric  *pCir;  
  
  pCir      =  pCirc;                          // ָ��ָ�����ݻ��������е��׸���ַ  
  front     = pCir->Front;  
  rear      = pCir->Rear;  
  size      = pCir->Size;  
   
   /* �ж����ݿռ仹�ж��� */   
  if (rear >= front)                           // ��βָ�� �ݶ�ͷָ�룬����Ϊ����  
  {  
    temp    =  (size - rear) + front;  
  }  
  else                                         // ��βָ�� ����ͷָ�룬����Ϊ����  
  {  
    temp    =  front - rear;  
  }  
 
  if(Len >= temp)                              // ����Ҫ����Ķ��и����Ƿ���ڵ��ڿն��еĸ��� ��  
  {  
    return(0);                              // �ǣ����� ��FILL������ȫ������ֵ  
  }  
  else  
  {  
    return(1);                             // �񣬷��� ��EMPTY����δ������ֵ  
  }  
 
    
}  


/*!
 * \fn     CircInit
 * \brief  �����ڴ��ʼ�� 
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
 * \brief  ���뻷���ڴ�
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
  unsigned short rear;                         // ��βָ�����  
  unsigned short size;                         // ���ݻ��������еĸ�����С����  
  unsigned char *ptr;                          // �������л�����ָ�����  
  StruCric      *pCir;                         // ָ�����ݻ��������е�ָ�����  
  unsigned short i;  
  unsigned char *pBuf;  
  unsigned char *pTemp;  
  
  pCir      =  (StruCric *)pCirc;                         // ָ��ָ�����ݻ��������е��׸���ַ  

  ptr      =  pSrc;  
    
  if (CircStatus(pCir, Len) == 1)              // ������ݻ����������Ƿ��д洢�ռ� ��  
  { 
    rear   = pCir->Rear;                       // Ϊ��βָ�븳ֵ  
    size   = pCir->Size;                       // Ϊ���ݻ��������еĸ�����С��ֵ  
    pTemp  = (unsigned char *)&pCir->Buf;  
    pBuf   = pTemp;                            
    pBuf  += rear;                             // ���ݻ�����������ͷָ��  
    
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
 * \brief  ��ѯ���ݻ���������״̬
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueStatus(NetDataQueue *pQueue)    
{
  unsigned short temp;                         // ��ʱ����
  unsigned short front;                        // ��ͷָ�����
  unsigned short rear;                         // ��βָ�����
  unsigned short size;                         // ���ݻ��������еĸ�����С����
  NetDataQueue *pQue;                        // ָ�����ݻ��������е�ָ�����

  pQue      =  pQueue;                        // ָ��ָ�����ݻ��������е��׸���ַ
                                              // ע�⣺��ͷָ�������ڶ��нṹ���׸���ַ��
  front     = pQue->Front;                    // Ϊ��ͷָ�븳ֵ

  rear      = pQue->Rear;                     // Ϊ��βָ�븳ֵ

  size      = pQue->Size;                     // Ϊ���ݻ��������еĸ�����С��ֵ

  temp      =  rear + 1;                      // �ö�βָ��Ϊ��һ��ָ��
  if (temp >= size)
  {
    temp    =  0;
  }

  if(temp == front)                           // ����β��һ��ָ���Ƿ���ڶ�ͷָ�룿
  {
    return(0);                             // �ǣ����ء�FILL������ȫ������ֵ
  }
  else
  {
    return(1);                            // �񣬷��ء�EMPTY����δ������ֵ
  }
}


/*!
 * \fn     QueueInsert
 * \brief  �������ݻ���������
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
  unsigned short rear;                         // ��βָ�����
  unsigned short size;                         // ���ݻ��������еĸ�����С����
  unsigned short byte;                         // ÿ�����ݻ�����ռ�õ��ֽڳ��ȱ���
  unsigned char *ptr;                         // �������л�����ָ�����
  NetDataQueue *pQue;                        // ָ�����ݻ��������е�ָ�����

  pQue      =  (NetDataQueue *)pQueue;                        // ָ��ָ�����ݻ��������е��׸���ַ
  if (QueueStatus(pQue) == 1)             // ������ݻ����������Ƿ��д洢�ռ䣿
  {
    rear      = pQue->Rear;                   // Ϊ��βָ�븳ֵ
    size      = pQue->Size;                   // Ϊ���ݻ��������еĸ�����С��ֵ
    byte      = pQue->Bytes;                  // Ϊÿ�����ݻ�����ռ�õ��ֽڳ��ȸ�ֵ
    ptr       = pQueue;
    ptr      += 8;
    ptr      +=  rear * byte;                   // ָ��ָ���βָ����ʾ�����ݻ�������ַ��
    MemCopySramToSram2(pSrc, ptr, Len);

    rear   +=  1;                             // ���ݻ��������еĶ�βָ��ӣ�
    if (rear >= size)                         // ����βָ���Ƿ���ڵ������ֵ��
    {
      rear  =  0x00;                          // ���ݻ��������еĶ�βָ��飰
    }

    pQue->Rear   =  rear;                     // �����µ����ݻ��������еĶ�βָ��ֵ

    return(1);
  }

  return(0);
}



/*!
 * \fn     QueueBusy
 * \brief  ��ѯ���ݻ��������е�״̬
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueBusy(NetDataQueue *pQueue)  
{
  unsigned short front;                        // ��ͷָ�����
  unsigned short rear;                         // ��βָ�����
  NetDataQueue *pQue;                        // ָ�����ݻ��������е�ָ�����

  pQue      =  pQueue;                        // ָ��ָ�����ݻ��������е��׸���ַ
                                              // ע�⣺��ͷָ�������ڶ��нṹ���׸���ַ��
  front     = pQue->Front;                    // Ϊ��ͷָ�븳ֵ

  rear      = pQue->Rear;                     // Ϊ��βָ�븳ֵ
  
  if (front != rear)
  {
    return(TRUE);
  }
  return(FALSE);
} 


/*!
 * \fn     QueueDelete
 * \brief  ɾ�����ݻ���������
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
 * \brief  ���ɻ���У��2λ�ۼӺͼල��
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
 * \brief  �Ƚ� SRAM �� SRAM �е����ݺ���
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
 * \brief  ���ڣ�λ���������������£ã���ת������
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
