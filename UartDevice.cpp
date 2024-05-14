/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UartDevice.c
 * \author    yifeng
 * \version   1.0
 * \date      2023��6��30��
 * \brief     ��¼�豸ID�ŵȴ���Դ����
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
#include "UartDevice.h"

#include "CommFun.h"
#include "UsartDeal.h"
#include "TimerDeal.h"
#include "EepromDeal.h"

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
static void *DeviceUart_Thread(void *arg);

static void DeviceInfoGeneration(void);

static void DeviceSendDataHandle(void);
static void DeviceReadDataHandle(void);

static void DeviceRxQueryMess(void);
static void DeviceTxQueryMessAsk(unsigned char ParaSum, unsigned char *pParaVal);
static void DeviceRxSetMess(void);
static void DeviceTxSetMessAsk(unsigned char charResult);

static void DeviceRxWebRtcIdMess(void);
static void DeviceTxWebrtcSetMessAsk(unsigned char Result);


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
const  DeviceMessageKeyStru cDeviceKeyTable[] =
{
  {  0x80,  DeviceRxQueryMess           },
  {  0x81,  DeviceRxSetMess             },

  {  0x83,  DeviceRxWebRtcIdMess        },
};


unsigned char cNetRollKey[8]={0X15,0XFB,0XC3,0X31,0X97,0X7C,0Xfe,0X65}; 

/*=======================================================================================
                                         ȫ�ֱ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֱ������壺
---------------------------------------------------------------------------------------*/
DeviceUartStru  gstvDevice;

DeviceUartBits  gbitDevice;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/




/*!
 * \fn     Start_DeviceUartDeal_Monitor_Thread
 * \brief  
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_DeviceUartDeal_Monitor_Thread(void)
{
  pthread_t Devicd_tid;

  
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);


  if(pthread_create(&Devicd_tid, NULL, DeviceUart_Thread, NULL) < 0)
  {
    perror("pthread_create: DeviceUart_Thread");
  }
}



/*!
 * \fn     DeviceUart_Thread
 * \brief  ��¼ID�ȴ����߳�
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *DeviceUart_Thread(void *arg)
{
  pthread_detach(pthread_self());
   
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  /* ��ʼ������������ڴ����� */
  memset(&gbitDevice, 0, sizeof(DeviceUartBits));
  memset(&gstvDevice, 0, sizeof(DeviceUartStru));

  DeviceInfoGeneration();


  while(!gstvSysRun.quit)
  {
    /* ���ݽ��մ��� */
    if(gbitDevice.RxByteOver == SET)
    {
      if(TimerGet1msSubb(gstvDevice.RxByteOver, 10))
      {
        gbitDevice.RxByteOver = CLEAR;
        gstvDevice.RxByteOver = TimerGet1msInc();
        
        DeviceReadDataHandle();
      }
    }
    
    /* ���ݷ��ʹ��� */
    DeviceSendDataHandle();

    if(TimerGet1sSubb(gstvDevice.TestDelay, 5))
    {
      gstvDevice.TestDelay = TimerGet1sInc();

      DeviceTxWebrtcSetMessTest();
    }

    usleep(5000);
  }

  pthread_exit(0);
}

/*!
 * \fn     DeviceInfoGeneration
 * \brief  �豸��Ϣ����
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceInfoGeneration(void)
{
  // {0xbf, 0xe6, 0x40, 0xf6, 0x2d, 0x15}
  unsigned char  EuqiID[8] = {0};
  
  
  /* ��ȡID ������MAC ��ַ */
  //EeWriteDeviceParamToIni(&EuqiID[0]);
  
  EeReadDeviceParamToSram((unsigned char *)&EuqiID[0]);
  EeReadWebrtcIDParamToSram((unsigned char *)&gstvDevice.WebrtcId[0]);
  printf("WebRtc ID: %s\n", gstvDevice.WebrtcId);
  
  
  printf("Sram EuqiID: ");
  for(int i = 0; i < cDeviceEuqiIDSize; i++)
  {
    printf("0x%x ", EuqiID[i]);
  }
  printf("\r\n");
   
  gbitDevice.NoGetID = CLEAR;
  if(DecodeDeviceIDHandle(EuqiID, gstvDevice.EuqiID) ==FALSE)
  {
    // �豸ID�洢����
    gstvDevice.EuqiID[0] = 0x00;
    gstvDevice.EuqiID[1] = 0x22;
    gstvDevice.EuqiID[2] = 0x00;
    gstvDevice.EuqiID[3] = 0x01;
    gbitDevice.NoGetID   = SET;
  }
  
  gstvDevice.MacAddr[0] = 0x4e;
  gstvDevice.MacAddr[1] = 0x56;
  gstvDevice.MacAddr[2] = 0xe0;
  gstvDevice.MacAddr[3] = gstvDevice.EuqiID[1];
  gstvDevice.MacAddr[4] = gstvDevice.EuqiID[2];
  gstvDevice.MacAddr[5] = gstvDevice.EuqiID[3];
  
  Encrypt_HCS301(cNetRollKey, &gstvDevice.MacAddr[2]);
  
  gstvDevice.MacAddr[2] = gstvDevice.EncryptDat[0];
  gstvDevice.MacAddr[3] = gstvDevice.EncryptDat[1];
  gstvDevice.MacAddr[4] = gstvDevice.EncryptDat[2];
  gstvDevice.MacAddr[5] = gstvDevice.EncryptDat[3];

  printf("EuqiID: ");
  for(int i = 0; i < cDeviceEuqiIDSize; i++)
  {
    printf("0x%x ", gstvDevice.EuqiID[i]);
  }
  printf("\r\n");
  

  printf("MacAddr: ");
  for(int i = 0; i < cDeviceMacSize; i++)
  {
    printf("0x%x ", gstvDevice.MacAddr[i]);
  }
  printf("\r\n");

}


/*!
 * \fn     DeviceSendDataHandle
 * \brief  �������ݴ���
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceSendDataHandle(void)
{ 
  DeviceUartStru     *pDevice;
  unsigned char      *ptr;
  unsigned char      len;

  
  pDevice   = &gstvDevice;

  if(gbitDevice.TranData  == CLEAR)  
  {  
    return ;
  }
  
  gbitDevice.TranData = CLEAR;      

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pDevice->WaitTxData.Len != 0x00)
  {
    ptr             =  pDevice->TxBuf;
    *ptr++          =  cDeviceHead1;
    *ptr++          =  cDeviceHead2;
    *ptr++          =  pDevice->WaitTxData.Key;
    *ptr++          =  pDevice->WaitTxData.Len;
    len             =  pDevice->WaitTxData.Len;
    MemCopySramToSram(pDevice->WaitTxData.Data, ptr, len);
                                    
    ptr            +=  len;                   // ָ��ָ�����һ���ֽڣ��ãң��룩
    *ptr            =  DataCheckAccumulateXOR(&pDevice->TxBuf[2], len+2);
    len            +=  5; 
    
    pDevice->TxSize   = len;                     // �ñ��η������ݵ��ܸ���ֵ  
    pDevice->TxCnt    = 0x00;  

    pDevice->WaitTxData.Len = 0x00;
    
    UsartDeal_InputData(pDevice->TxBuf, pDevice->TxSize);
  }
}

/*!
 * \fn     DeviceReadDataHandle
 * \brief  ���ݴ���
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceReadDataHandle(void)
{
  DeviceUartStru     *pDevice;
  const DeviceMessageKeyStru *pTab;
  unsigned char      i;
  unsigned char      key;
  
  pTab     =  cDeviceKeyTable;
  pDevice   = &gstvDevice;  
  
  key     = pDevice->RxBuf[2];    // �ؼ���

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  if((pDevice->RxBuf[0]  == cDeviceHead1)
  &&(pDevice->RxBuf[1]  == cDeviceHead2))
  {
    printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
    // ����յ�ȫ�����ĵģãң����Ƿ���ȷ��
    if(DataCheckAccumulateXOR(&pDevice->RxBuf[2], pDevice->RxBuf[3]+3) == 0x00)
    {
      printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
      printf("key = 0x%x\r\n", key);

      for(i=0; i<(sizeof(cDeviceKeyTable) / sizeof(DeviceMessageKeyStru)); i++)
      {                               
        if (key == pTab->Key)         
        {
          pTab->KeySubProgram( );   
          gbitDevice.TranData  = SET;
          break;
        }
        pTab++;
      }
    }
  }
}



/*!
 * \fn     DeviceRxQueryMess
 * \brief  ��ѯ�豸��Ϣ
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxQueryMess(void)
{
  DeviceUartStru     *pDevice;
  unsigned char        ParaSum;  // ��������
  
  pDevice    = &gstvDevice;
  ParaSum      = pDevice->RxBuf[4];

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  DeviceTxQueryMessAsk(ParaSum, &pDevice->RxBuf[5]);
}

/*!
 * \fn     DeviceTxQueryMessAsk
 * \brief  ��ѯ�豸��ϢӦ��
 *          
 * \param  [in] u8           ParaSum    #
 * \param  [in] void*        pParaVal   #
 * 
 * \retval void
 */
static void DeviceTxQueryMessAsk(unsigned char ParaSum, unsigned char *pParaVal)
{
  DeviceDataStru  *pbuf;
  unsigned char   len;
  unsigned char   i;
  unsigned char   *ptr;
  unsigned char   *pPara;
  unsigned char   temp;
  DeviceUartStru  *pDevice;
  unsigned char   j;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pDevice   = &gstvDevice;
  pbuf      = &pDevice->WaitTxData;
  
  len   = 1;
  ptr   = &pbuf->Data[1];
  pPara = pParaVal;
  
  pbuf->Key   = cDeviceTxQueryMessAsk;  // ��ѯ�豸��ϢӦ��
  
  pbuf->Data[0] = ParaSum;
  
  // ����ÿ���ؼ���
  for(i=0; i<ParaSum; i++)
  {
    *ptr++ = *pPara;   // ����ID
    if(*pPara == 0x01)  // �豸ID��
    {
      temp     = cDeviceEuqiIDSize;    // ��������
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.EuqiID[0], ptr, temp);
    }
    else if(*pPara == 0x02)  //IMEI��
    {
      temp     = cDeviceGsmImeiSize;    // ��������
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.GsmImei[0], ptr, temp);
    }
    else if(*pPara == 0x03)  //MAC��ַ
    {
      temp     = cDeviceMacSize;    // ��������
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.MacAddr[0], ptr, temp);
    }
    else if(*pPara == 0x04)  //Ӳ���汾��
    {
      temp     = 1;    // ��������
      *ptr++   =  temp;
      *ptr     = cSysHardwareVer;
    }
    else if(*pPara == 0x05)  //�̼��汾��
    {
      temp     = 2;    // ��������
      *ptr++   =  temp;
      *ptr++   = cSysSoftwareVer >>  8;
      *ptr     = cSysSoftwareVer & 0xff;
      ptr--;
    }
    else if(*pPara == 0x06)
    {
      temp = cDeviceWebrtcIdSize-6;
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.WebrtcId[0], ptr, temp);
    }
    else
    {
      temp = 0;
      *ptr++ = temp;
    }    
    
    pPara++;
    
    len    += temp+2;
    ptr    += temp;
  }
  
  pbuf->Len   = len ;                // ���ݳ���
}


/*!
 * \fn     DeviceRxSetMess
 * \brief  �����豸��Ϣ
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxSetMess(void)
{
  DeviceUartStru    *pDevice;
  unsigned char     ParaID;   // ����ID
  unsigned char     ParaLen;  // ��������
  unsigned char     ParaSum;  // ��������
  unsigned char     *ptr;
  unsigned char     i, j;
  unsigned char     EuqiID[cDeviceEuqiIDSize];
  unsigned char     buf[6];
  unsigned char     flg=0;    // ���������ʾ

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pDevice    = &gstvDevice;
  ParaSum    = pDevice->RxBuf[4];
  ptr        = &pDevice->RxBuf[5];
  
  // ����ÿ���ؼ���
  for(i=0; i<ParaSum; i++)
  {
    ParaID  = *ptr++;
    ParaLen = *ptr++;
    
    if(ParaID == 0x01)  // �豸ID��
    {
      if(ParaLen  == 0x06)
      {
        if(DecodeDeviceIDHandle(ptr, EuqiID) == TRUE)
        {
          DecodeDeviceIDHandle(ptr, gstvDevice.EuqiID);
          MemCopySramToSram(ptr, buf, 0x06);
          gstvDevice.MacAddr[0] = 0x4e;
          gstvDevice.MacAddr[1] = 0x56;
          gstvDevice.MacAddr[2] = 0xe0;
          gstvDevice.MacAddr[3] = gstvDevice.EuqiID[1];
          gstvDevice.MacAddr[4] = gstvDevice.EuqiID[2];
          gstvDevice.MacAddr[5] = gstvDevice.EuqiID[3];
          
          Encrypt_HCS301(cNetRollKey, &gstvDevice.MacAddr[2]);
        
          gstvDevice.MacAddr[2] = gstvDevice.EncryptDat[0];
          gstvDevice.MacAddr[3] = gstvDevice.EncryptDat[1];
          gstvDevice.MacAddr[4] = gstvDevice.EncryptDat[2];
          gstvDevice.MacAddr[5] = gstvDevice.EncryptDat[3];
          
          printf("EuqiID: ");
          for(int j = 0; j < cDeviceEuqiIDSize; j++)
          {
            printf("0x%x ", gstvDevice.EuqiID[j]);
          }
          printf("\r\n");
          printf("EuqiID buf: ");
          for(int j = 0; j < 6; j++)
          {
            printf("0x%x ", buf[j]);
          }
          printf("\r\n");

          /* ���豸IDд�� INI �ļ��� */
          EeWriteDeviceParamToIni(&buf[0]);          
          ptr   += 6;
        }
        else
        {
          flg =1;
          break;
        }
      }
      else  // ����
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x02)  // IMEI��
    {
      if(ParaLen  == 0x0f)
      {
        ptr   += 0x0f;
      }
      else  // ����
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x03)  // MAC��ַ
    {
      if(ParaLen  == 0x06)
      {
        ptr   += 6;
      }
      else  // ����
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x04)  // Ӳ���汾��
    {
      if(ParaLen  == 0x01)
      {
        ptr   += 1;
      }
      else  // ����
      {
        flg =1;
        break;
      }  
    }
    else if(ParaID == 0x05)  // �̼��汾��
    {
      if(ParaLen  == 0x02)
      {
        ptr   += 2;
      }
      else  // ����
      {
        flg =1;
        break;
      }
    }
    else
    {
      ptr   += ParaLen;
    }
  }
  
  // �ж������Ƿ���ȷ
  if(flg  == 0)
  {  
    DeviceTxSetMessAsk(0x00);
  }
  else
  {
    DeviceTxSetMessAsk(0x01);
  }
}



/*!
 * \fn     DeviceTxSetMessAsk
 * \brief  �����豸��ϢӦ��
 *          
 * \param  [in] u8           Result   #
 * 
 * \retval void
 */
static void DeviceTxSetMessAsk(unsigned char Result)

{
  DeviceDataStru *pbuf;
  
  DeviceUartStru  *pDevice;
  
  pDevice   = &gstvDevice;
  pbuf      = &pDevice->WaitTxData;
  
  pbuf->Key = cDeviceTxSetMessAsk;  // �����豸��ϢӦ��
  
  pbuf->Data[0] = Result;
  
  pbuf->Len   = 1 ;                // ���ݳ���
}


/*!
 * \fn     DeviceRxWebRtcIdMess
 * \brief  ����webrtc�豸ID
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxWebRtcIdMess(void)
{
  DeviceUartStru    *pDevice;
  unsigned char     ParaLen;  // ��������
  unsigned char     buf[255];
  
  pDevice = &gstvDevice;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  ParaLen = pDevice->RxBuf[3];

  MemCopySramToSram((unsigned char *)&pDevice->RxBuf[4], (unsigned char *)&buf[0], ParaLen);

  /* д�뵽INI�ı��� */
  printf("%s\n", buf);
  EeWriteWebrtcIdParamToIni((unsigned char *)&buf[0]);

  DeviceTxWebrtcSetMessAsk(0);
}



/*!
 * \fn     DeviceTxWebrtcSetMessAsk
 * \brief  �����豸��ϢӦ��
 *          
 * \param  [in] unsigned char Result   #
 * 
 * \retval static void
 */
static void DeviceTxWebrtcSetMessAsk(unsigned char Result)
{
  DeviceDataStru *pbuf;
  
  DeviceUartStru  *pDevice;
  
  pDevice   = &gstvDevice;
  pbuf      = &pDevice->WaitTxData;
  
  pbuf->Key = cDeviceTxWebrtcSetMessAsk;  // �����豸��ϢӦ��
  
  pbuf->Data[0] = Result;
  
  pbuf->Len   = 1 ;                // ���ݳ���
}


/*!
 * \fn     DecodeDeviceIDHandle
 * \brief  �����豸ID
 *          
 * \param  [in] void      *pSrouce   #
 * \param  [in] void      *pDestin   #
 * 
 * \retval bool
 */
bool DecodeDeviceIDHandle(unsigned char *pSrouce, unsigned char *pDestin)     
{
  unsigned char    idbuf1[6];
  unsigned char    idbuf2[6];
  unsigned char    *pSrc;  
  unsigned char    *pDst;  
  
  pSrc    = pSrouce;  
  pDst    = pDestin;
  
  MemCopySramToSram(pSrc, idbuf1, 6);
  
  // ����λ�� + ��̶��������
  idbuf2[0]  = idbuf1[4] ^ 0x2d;
  idbuf2[1]  = idbuf1[2] ^ 0x63;
  idbuf2[2]  = idbuf1[5] ^ 0x15;
  idbuf2[3]  = idbuf1[0] ^ 0xed;
  idbuf2[4]  = idbuf1[1] ^ 0x97;
  idbuf2[5]  = idbuf1[3] ^ 0xca;
  
  // �������У��
  if(idbuf2[4] == (DataCheckAccumulateXOR(&idbuf2[0], cDeviceEuqiIDSize)))
  {
    if(0x00 == (DataCheckCrcByte(&idbuf2[0], cDeviceEuqiIDSize+2)))
    {
      // ��ID�Ŵ���flash
      MemCopySramToSram(&idbuf2[0], pDst, cDeviceEuqiIDSize);
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}


/*!
 * \fn     Encrypt_HCS301
 * \brief  ���������
 *          
 * \param  [in] unsigned char *keyin   #
 * \param  [in] unsigned char *datin   #
 * 
 * \retval void
 */
void Encrypt_HCS301(unsigned char *keyin, unsigned char *datin)
{  
  unsigned char p;  
  unsigned int q;
  
  for(q=0;q<576;q++)       //ѭ��576��
  {
    if(q<528)             //���48�ν�ִ����Կ��λ�� Ϊ���ǽ���Կ��ԭ
    {
      if(datin[2]&0x10)  //�����ĵ����ֽ�d4λ=1��p=16�� ����p=1
        p=0x10;            
      else
        p=0x01;
    
      if(datin[1]&0x02)  //�����ĵڶ��ֽ�d1λ=1��p������λ
        p<<=2;
      if(datin[0]&0x02)  //�����ĵ�һ�ֽ�d1λ=1��p������һλ
        p<<=1; 
      
      if((datin[3]&0x80)&&(datin[3]&0x04))      
        p&=0x3a;           //�����ĵ����ֽ�d7λd2λ=11�� p����0x3a 
      else if((datin[3]&0x80)&&((datin[3]&0x04)==0))      
        p&=0x5c;           //�����ĵ����ֽ�d7λd2λ=10�� p����0x5c 
      else if(((datin[3]&0x80)==0)&&(datin[3]&0x04))      
        p&=0x74;           //�����ĵ����ֽ�d7λd2λ=01�� p����0x74 
      else if(((datin[3]&0x80)==0)&&((datin[3]&0x04)==0))      
        p&=0x2e;           //�����ĵ����ֽ�d7λd2λ=00�� p����0x2e 

      if(p)              //p������0����p=0x01
        p=0x01;
        
      p^=datin[0];       //p����������ĵ�һ�ֽڡ����ĵ����ֽڡ���Կ��һ�ֽ�
      p^=datin[2];
      p^=keyin[0];

      //��������p�����ĵ����ֽڡ����ĵ����ֽڡ����ĵڶ��ֽڡ����ĵ�һ�ֽ�
      datin[0]=(datin[0]>>1)|(datin[1]<<7);
      datin[1]=(datin[1]>>1)|(datin[2]<<7);
      datin[2]=(datin[2]>>1)|(datin[3]<<7);
      datin[3]=(datin[3]>>1)|(p<<7);
    }

    p=keyin[0];           //ѭ��������Կ�ڰ��ֽ�-��Կ��һ�ֽ�
    keyin[0]=(keyin[0]>>1)|(keyin[1]<<7);
    keyin[1]=(keyin[1]>>1)|(keyin[2]<<7);
    keyin[2]=(keyin[2]>>1)|(keyin[3]<<7);
    keyin[3]=(keyin[3]>>1)|(keyin[4]<<7);
    keyin[4]=(keyin[4]>>1)|(keyin[5]<<7);
    keyin[5]=(keyin[5]>>1)|(keyin[6]<<7);
    keyin[6]=(keyin[6]>>1)|(keyin[7]<<7);
    keyin[7]=(keyin[7]>>1)|(p<<7);
  }   
  
  gstvDevice.EncryptDat[0]=datin[0]; 
  gstvDevice.EncryptDat[1]=datin[1]; 
  gstvDevice.EncryptDat[2]=datin[2]; 
  gstvDevice.EncryptDat[3]=datin[3]; 
}  




/*!
 * \fn     DeviceRxDatacIsr
 * \brief  �豸���������жϴ���
 *          
 * \param  [in] unsigned char *Data   #
 * \param  [in] unsigned short len    #
 * 
 * \retval void
 */
void DeviceRxDatacIsr(unsigned char *Data, unsigned short len)
{
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  gbitDevice.RxByteOver = SET;
  gstvDevice.RxByteOver = TimerGet1msInc();

  gstvDevice.RxSize = 0;
  MemClearSram2(&gstvDevice.RxBuf[0], cDeviceRxBufSize);

  gstvDevice.RxSize = len;
  MemCopySramToSram(Data, &gstvDevice.RxBuf[0], len);

  printf("Data len = %d: ", len);
  for(int i = 0; i < len; i++)
  {
    printf("0x%x ", gstvDevice.RxBuf[i]);
  }
  printf("\r\n");
}


/*!
 * \fn     DeviceTxWebrtcSetMessTest
 * \brief  ��������
 *          
 * \param  [in] unsigned char Result   #
 * 
 * \retval static void
 */
void  DeviceTxWebrtcSetMessTest(void)
{
  DeviceDataStru *pbuf;
  unsigned char WebrtcId[32] = "RHZL-00-ZD6R-N5HF-00000405";
  unsigned char i = 0;
  
  DeviceUartStru  *pDevice;
  
  pDevice   = &gstvDevice;
  pbuf      = &pDevice->WaitTxData;
  
  pbuf->Key = 0x83;  // �����豸��ϢӦ��

  MemCopySramToSram((unsigned char *)&WebrtcId[0], (unsigned char *)&pbuf->Data[0], sizeof(WebrtcId));


  for(i = 0; i < sizeof(WebrtcId); i++)
  {
    if(WebrtcId[i] == 0x00)
      break;
  }
  pbuf->Len   = i+1;                // ���ݳ���

  gbitDevice.TranData  = SET;
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
