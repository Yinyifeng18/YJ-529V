/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      CommFun.h
 * \author    yifeng
 * \version   1.0
 * \date      2023��6��30��
 * \brief     ͨ�ú���ͷ�ļ�
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


/*=======================================================================================
��������ָ���Ǳ�ͷ�ļ����������Ԥ����ָ����ڷ�ֹ��ͷ�ļ��������ļ��ظ����á�
=======================================================================================*/
#ifndef __CommFun_H__
#define __CommFun_H__


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "Main.h"

#include "NetDataDeal.h"


/*=======================================================================================
����������ָ�����������ֱ���ϵͳΪ C++ �� C ��Ԥ����ָ�                               
"__cplusplus" Ϊ C++ Ԥ���������֣�������ָ������������������Щָ��Ϊ C ��������.      
=======================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=======================================================================================
                               ϵͳ���еĸ����ַ�����������                             
=======================================================================================*/
/*---------------------------------------------------------------------------------------
Ϊ�������Ϻõ��������ֲ�ԣ��Ƽ��û��ڱ��ʱʹ�����涨����������ʹ�С
---------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------
���ַ����������壺
----------------------------------------------------------------------------------------*/


/*=======================================================================================
                      �����ַ���������Ӳ�������ص����룯������Ŷ���
=======================================================================================*/
/*--------------------------------------------------------------------------------------
�����룯������Ŷ��壺
--------------------------------------------------------------------------------------*/


/*======================================================================================
                             ȫ�ֱ������ݽṹ���Ͷ���
======================================================================================*/
/*--------------------------------------------------------------------------------------
��ȫ�����ݽṹ���Ͷ��� ��  
--------------------------------------------------------------------------------------*/
typedef struct  
{  
  unsigned short Front;                         // �����ڴ�Ķ�ͷָ�� �����������  
  unsigned short Rear;                          // �����ڴ�Ķ�βָ�� ��������ݣ�  
  unsigned short Size;                          // �����ڴ�ĸ�����С  
  unsigned char  Escape;                        // �����ڴ��ת���ַ�  
  unsigned char  ByEscape;                      // �����ڴ�ı�ת���ַ� 
  unsigned char  *Buf;                          // ���ݻ��������׸���ַָ��  
} StruCric;  


/*=======================================================================================
                                         ȫ�ֳ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֳ���������
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                         ȫ�ֱ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֱ���������
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                                 Դ�����ļ��еĺ���ԭ������
=======================================================================================*/

/*!
 * \fn     DataCheckAccumulateXOR
 * \brief  ���ɻ���У�飱λ�ۼӺͼල��
 *          
 * \param  [in] void  *pSrcoue     #
 * \param  [in] unsigned char Length   #
 * 
 * \retval unsigned char
 */
unsigned char DataCheckAccumulateXOR(unsigned char *pSrcoue, unsigned char Length);


/*!
 * \fn     DataCheckCrcByte
 * \brief  ���ɻ���У�飱λ�ල���ѭ������У����
 *          
 * \param  [in] void *pData          #
 * \param  [in] unsigned short Len   #
 * 
 * \retval unsigned char
 */
unsigned char DataCheckCrcByte (unsigned char *pData, unsigned short Len);   

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
                         unsigned char ByteLen);

void  MemCopySramToSram2(unsigned char         *pSource,
                          unsigned char *pObject,
                          unsigned int  ByteLen);

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
                       unsigned char ByteLen);

void MemClearSram2(unsigned char      *pSection,
                       unsigned int  ByteLen);
                       


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
                               unsigned char Len1);
                               

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
                             unsigned char Len2);
                             

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
void AtHexConvAsc(unsigned char *pHex, unsigned char *pAsc, unsigned char HexLen);

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
void AtAscConvHex(unsigned char *pAsc, unsigned char *pHex, unsigned char AscLen);

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
                   unsigned short Bytes);


/*!
 * \fn     CircBusy
 * \brief  ��ѯ�����ڴ����Ƿ�������������
 *          
 * \param  [in] unsigned char    *pCirc   # ָ�����ݻ��������е��׸���ַָ��  
 * 
 * \retval bool char
 */
bool  CircBusy(unsigned char *pCirc);    
                   
/*!
 * \fn     CircGain
 * \brief  �ӻ����ڴ��л�ȡ����
 *          
 * \param  [in] unsigned char *pCirc   #
 * \param  [in] unsigned char *pDst    #
 * 
 * \retval unsigned short
 */
unsigned short CircGain(unsigned char *pCirc, unsigned char *pDst);


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
                      );

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
                 unsigned char Escape);  

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
                        unsigned short Len);


/*!
 * \fn     QueueStatus
 * \brief  ��ѯ���ݻ���������״̬
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueStatus(NetDataQueue *pQueue);


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
signed char QueueInsert(unsigned char     *pQueue,   
                          unsigned char *pSrc,     
                          unsigned short Len);

                          
/*!
 * \fn     QueueBusy
 * \brief  ��ѯ���ݻ��������е�״̬
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval signed char
 */
signed char QueueBusy(NetDataQueue *pQueue);



/*!
 * \fn     QueueDelete
 * \brief  ɾ�����ݻ���������
 *          
 * \param  [in] void *pQueue   #
 * 
 * \retval void
 */
void QueueDelete(NetDataQueue *pQueue);


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
                                         unsigned short Length);


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
                                       unsigned char ByteLen);

signed char MemCompSramAndSram2(unsigned char  *pSource,
                                        unsigned char *pObject,
                                        unsigned int  ByteLen);
                                        

/*!
 * \fn     ConvBin8ToBcd3
 * \brief  ���ڣ�λ���������������£ã���ת������
 *          
 * \param  [in] unsigned char binary_value   #
 * 
 * \retval unsigned int
 */
unsigned int ConvBin8ToBcd3 (unsigned char binary_value);



/*=======================================================================================
����������ָ���������������������ֱ���ϵͳΪ C++ �� C ��Ԥ����ָ�����Ӧ������ָ��
 extern "C" ����ָʾ�������򣬴˴���"}"�������"{"���Ӧ��Ϊ����ָʾ�����ý�������
=======================================================================================*/
#ifdef __cplusplus
}
#endif

#endif
/*
*****************************************************************************************
                                 ��������Դ�����ļ����˽���
*****************************************************************************************
*/


