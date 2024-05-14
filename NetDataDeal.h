/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      NetDataDeal.h
 * \author    yifeng
 * \version   1.0
 * \date      2023��7��3��
 * \brief     Э�����ݴ���ͷ�ļ�
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��7��3��
      author: yifeng
      change: create file

*****************************************************************************************/


/*=======================================================================================
��������ָ���Ǳ�ͷ�ļ����������Ԥ����ָ����ڷ�ֹ��ͷ�ļ��������ļ��ظ����á�
=======================================================================================*/
#ifndef __NetDataDeal_H__
#define __NetDataDeal_H__



/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "Main.h"


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
#define cNetWaitLoginAckTick     5             // �ȴ��ն˵�¼����Ӧ��ʱ10��
#define cNetWaitHeartAckTick     5             // �ȴ��ն�����Ӧ��ʱ10��
#define cNetWaitDataAckTick      5             // �ȴ��ն˿�����������Ӧ��ʱ10��
#define cNetWaitAlarmAckTick     5             // �ȴ��ն˱�������Ӧ��ʱ10��

#define cNetWaitLoginAckMax      5             // �ȴ��ն˵�¼����Ӧ�����
#define cNetWaitHeartAckMax      2             // �ȴ��ն�����Ӧ�����
#define cNetWaitDataAckMax       3             // �ȴ��ն˿�����������Ӧ�����
#define cNetWaitAlarmAckMax      3             // �ȴ��ն˱�������Ӧ�����




#define cNetCrirBufSize               0x600     
#define cNetRxBufSize                 0x300            // ��չģ��������ݻ�������С  
#define cNetTxBufSize                 0x300            // ��չģ�鷢�����ݻ�������С
#define cNetDataSize                  0x300            // �������ݱ��ĵ�������Ч�ֽڴ�С  

#define cNetDataNum                   15               // �������ݱ������ݻ�������С 


#define cNetByEscapeChar              0x7E            // ��ת���ַ�  
#define cNetEscapeChar                0x7D            // ת���ַ�

// ����״̬ 
#define   cNetStateWifi                0x01
#define   cNetStategLan                0x02
#define   cNetStategGprs               0x04



#define cNetTxGeneralRespone          0x7fff          // �ն�ͨ��Ӧ�� 
#define cNetTxLoginRequest            0x0001          // �ն˵�¼���� 
#define cNetTxlogout                  0x0002          // �ն�ע�� 
#define cNetTxheartbeat               0x0003          // �ն����� 
#define cNetTxSysParaRespone          0x0004          // ϵͳ����Ӧ��
#define cNetTxFWUpdateRespone         0x0005          // �ն˹̼���������Ӧ��
#define cNetTxFWMessRequest           0x0006          // �ն˹̼���Ϣ����
#define cNetTxFWDownRequest           0x0007          // �ն˹̼�����
#define cNetTxTimeRequest             0x0008          // ���������Уʱ


#define cNetTxProgParaRespone         0x1000          // ��ѯ�ն˱�̲�������Ӧ��
#define cNetTxOtherParaRespone        0x1001          // ��ѯ�ն�������������Ӧ��
#define cNetTxCtrlRespone             0x1002          // �ն˿���Ӧ��
#define cNetTxQueryRespone            0x1003          // �ն˲�ѯӦ��
#define cNetTxAlarmEvent              0x1004          // �ն˾���
#define cNetTxServerOperate           0x1005          // �ն˶Է���������
#define cNetTxUpdateState             0x1006          // �豸״̬�仯����֪ͨ������
#define cNetTxAddrProgRespone         0x1007          // ��ѯ�ն�ָ���̲���Ӧ��
#define cNetTxTogtherAlarm            0x100B          // �����ַ�����������������


#define cNetHeartTickOver             60               // �ն�����90��


/*---------------------------------------------------------------------------------------
���ַ����������壺
----------------------------------------------------------------------------------------*/
typedef enum
{
  EnTxEmpty              =  0x00         , // ���Ϳ�
  EnTxLoginRequest                       , // �ն˵�¼����
  EnTxHeartBeat                          , // �ն�����
  EnTxCtrlData                           , // ���Ϳ�����Ϣ
  EnTxAlarmData                            // ���Ϳ��Ʊ�����Ϣ
} NetTxEnum;

/*--------------------------------------------------------------------------------------
�ն�ͨ��Ӧ���ȫ��ö�����Ͷ��壺
--------------------------------------------------------------------------------------*/
typedef enum
{
  EnGrSuccessPart            =  0x00             , // �ɹ�/ȷ��
  EnGrFalsePart                                  , // ʧ��
  EnGrCrcErrorPart                               , // ��Ϣ����У�����
  EnGrNoSupportPart                              , // ��֧��
  EnGrAlarmSurePart                                // ��������ȷ��
} NetGenResEnum;


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
  unsigned char WaitLoginAck:    1;              // �ȴ��ն˵�¼����Ӧ��
  unsigned char WaitHeartAck:    1;              // �ȴ��ն�����Ӧ��
  unsigned char WaitDataAck:     1;              // �ȴ��ն˿�����������Ӧ��
  unsigned char WaitAlarmAck:    1;              // �ȴ��ն˱�������Ӧ��

  unsigned char LoginSuccuess:   1;              // ��½�ɹ�

  unsigned char TxFlg:           1;
}NetDealBits;  


typedef struct  
{  
  unsigned short  Front                     ;  // �����ڴ�Ķ�ͷָ�� �����������  
  unsigned short  Rear                      ;  // �����ڴ�Ķ�βָ�� ��������ݣ�  
  unsigned short  Size                      ;  // �����ڴ����ݻ������ĸ�����С  
  unsigned char   Escape                    ;  // �����ڴ��ת���ַ�  
  unsigned char   ByEscape ;                     // �����ڴ�ı�ת���ַ� 
  unsigned char   Buf[cNetCrirBufSize]     ;  // ���ݱ�������  
} NetDataCrir;  



typedef struct  
{  
  NetDataCrir    RxCrir;
  unsigned short RxSize;                // ÿ�ν������ݸ�����С  
  unsigned char  RxBuf[cNetRxBufSize];  // ÿ�ν������ݻ�����  
  
  unsigned short RxSizeBak;                // ÿ�ν������ݸ�����С  
  unsigned char  RxBufBak[cNetRxBufSize];  // ÿ�ν������ݻ�����  

  unsigned short TxSize;                   // ÿ�η������ݸ�����С
  unsigned char  TxBuf[cNetTxBufSize];     // ÿ�η������ݻ�����
  
  unsigned short HeartTick;             // ����ʱ��
  unsigned short RxTxTick;              // ���պͷ���ʱ��

  unsigned short UpdateStateTick;       // ����״̬
  unsigned char  LoginCnt;              // ��¼����

  unsigned short WaitLoginAckDelay;     // �ȴ��ն˵�¼����Ӧ��ʱ��
  unsigned char  WaitLoginAckCnt;       // �ȴ��ն˵�¼����Ӧ�����

  unsigned short WaitHeartAckDelay;     // �ȴ��ն�����Ӧ��ʱ��
  unsigned char  WaitHeartAckCnt;       // �ȴ��ն�����Ӧ�����

  unsigned short WaitDataAckDelay;      // �ȴ��ն˿�������Ӧ��ʱ��
  unsigned char  WaitDataAckCnt;        // �ȴ��ն˿�������Ӧ�����

  unsigned short WaitAlarmAckDelay;     // �ȴ��ն˱�����Ӧ��ʱ��
  unsigned char  WaitAlarmAckCnt;       // �ȴ��ն˱�����Ӧ�����

  WORD_BYTES     TxSerial;              // ������Ϣ��ˮ��(������Ӧ���ظ�����ϢID)
  WORD_BYTES     TxKeyWord;             // �����ն���Ϣ��ID(������Ӧ���ظ�����Ϣ��ˮ��)
  NetTxEnum      TxMKey;                // ���͹ؼ���
  NetTxEnum      TxMKeyBak;             // ���͹ؼ���

  WORD_BYTES     RxKeyWord;             // ��Ӧ��ƽ̨��Ϣ��ID
  WORD_BYTES     RxSerial;              // ��Ӧ��ƽ̨��Ϣ����ˮ��

  WORD_BYTES     StateFlags;            // �豸״̬
  WORD_BYTES     AlarmFlags;            // ������־
  
  unsigned char  CurrNetData;           // ��ǰ��������
  unsigned char  TxAlarmCnt                  ;   // ��ǰ���͵ı�����¼������           

  unsigned short RxDelay;               // ���ճ�ʱ

  unsigned short TestDelay; 
}NetDealStru;  


typedef struct  
{  
  unsigned short  Key;  
  void           (*KeySubProgram)(void);  
} NetKeySubStru;  


/*--------------------------------------------------------------------------------------  
�������ݱ������ݱ����ʽ��ȫ�����ݽṹ���Ͷ��� ��  
--------------------------------------------------------------------------------------*/  
typedef struct  
{  
  unsigned short Key                         ;  // ���ؼ��� ���������֣�  
  unsigned short Len                         ;  // ���ݳ��� ���ֽ�����  
  unsigned char  Data[cNetDataSize]          ;  // ���ݱ�������  
} NetDataStru;  


typedef struct  
{  
  unsigned short   Front                     ;  // ���ݱ��Ļ��������еĶ�ͷָ�� �����������  
  unsigned short   Rear                      ;  // ���ݱ��Ļ��������еĶ�βָ�� ��������ݣ�  
  unsigned short   Size                      ;  // ���ݱ��Ļ��������еĸ�����С  
  unsigned short   Bytes                     ;  // ÿ�����ݱ�������ռ�õ��ֽڳ���  
  NetDataStru      Buf[cNetDataNum]          ;  // ���ݱ������� ��Size��Bytes��  
} NetDataQueue;  


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
extern NetDealBits  gbitNet;
extern NetDealStru  gstvNet;

/*=======================================================================================
                                 Դ�����ļ��еĺ���ԭ������
=======================================================================================*/




/*!
 * \fn     Start_NetDataDeal_Monitor_Thread
 * \brief  �������紦�����
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_NetDataDeal_Monitor_Thread(void);


/*!
 * \fn     NetEscapeDecodingHandle
 * \brief  ����ת����봦��
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
 * \brief  ����ת����봦��
 *          
 * \param  [in] unsigned char         *pSrouce   #
 * \param  [in] unsigned char  *pDestin          #
 * \param  [in] unsigned short  DataLen          #
 * 
 * \retval unsigned short
 */
unsigned short NetEscapeCodingHandle(unsigned char         *pSrouce,  // Դ����ָ��  
                           unsigned char  *pDestin,  // Ŀ������ָ��  
                           unsigned short  DataLen    // Դ���ݳ���  
                           );    

/*!
 * \fn     NetDataLoginServerInit
 * \brief  ���µ�¼��������ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataLoginServerInit(void);

/*!
 * \fn     NetDataRxCrirInit
 * \brief  ���ݽ��ն��г�ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataRxCrirInit(void);



/*------------------------------------------------------------------------------------  
 ���ݡ��ն�ͨ��Ӧ��  
-------------------------------------------------------------------------------------*/  
void  NetTxGeneralRespone(WORD_BYTES* SerialNum,
                          WORD_BYTES* KeyWord,      
                          NetGenResEnum  Results);    

/*------------------------------------------------------------------------------------  
 ���ݡ��ն˿���Ӧ��  
-------------------------------------------------------------------------------------*/ 
void  NetTxCtrlRespone(WORD_BYTES* SerialNum,     // Ӧ����ˮ��
                       unsigned char Command,       // ������
                       unsigned char  Result         // ���
                       );


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


