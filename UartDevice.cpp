/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      UartDevice.c
 * \author    yifeng
 * \version   1.0
 * \date      2023年6月30日
 * \brief     烧录设备ID号等处理源程序
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
#include "UartDevice.h"

#include "CommFun.h"
#include "UsartDeal.h"
#include "TimerDeal.h"
#include "EepromDeal.h"

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
const  DeviceMessageKeyStru cDeviceKeyTable[] =
{
  {  0x80,  DeviceRxQueryMess           },
  {  0x81,  DeviceRxSetMess             },

  {  0x83,  DeviceRxWebRtcIdMess        },
};


unsigned char cNetRollKey[8]={0X15,0XFB,0XC3,0X31,0X97,0X7C,0Xfe,0X65}; 

/*=======================================================================================
                                         全局变量定义                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局变量定义：
---------------------------------------------------------------------------------------*/
DeviceUartStru  gstvDevice;

DeviceUartBits  gbitDevice;


/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
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
 * \brief  烧录ID等处理线程
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *DeviceUart_Thread(void *arg)
{
  pthread_detach(pthread_self());
   
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  /* 初始化，清除所有内存数据 */
  memset(&gbitDevice, 0, sizeof(DeviceUartBits));
  memset(&gstvDevice, 0, sizeof(DeviceUartStru));

  DeviceInfoGeneration();


  while(!gstvSysRun.quit)
  {
    /* 数据接收处理 */
    if(gbitDevice.RxByteOver == SET)
    {
      if(TimerGet1msSubb(gstvDevice.RxByteOver, 10))
      {
        gbitDevice.RxByteOver = CLEAR;
        gstvDevice.RxByteOver = TimerGet1msInc();
        
        DeviceReadDataHandle();
      }
    }
    
    /* 数据发送处理 */
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
 * \brief  设备信息生成
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceInfoGeneration(void)
{
  // {0xbf, 0xe6, 0x40, 0xf6, 0x2d, 0x15}
  unsigned char  EuqiID[8] = {0};
  
  
  /* 读取ID 并生成MAC 地址 */
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
    // 设备ID存储错误
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
 * \brief  发送数据处理
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
                                    
    ptr            +=  len;                   // 指针指向最后一个字节（ＣＲＣ码）
    *ptr            =  DataCheckAccumulateXOR(&pDevice->TxBuf[2], len+2);
    len            +=  5; 
    
    pDevice->TxSize   = len;                     // 置本次发送数据的总个数值  
    pDevice->TxCnt    = 0x00;  

    pDevice->WaitTxData.Len = 0x00;
    
    UsartDeal_InputData(pDevice->TxBuf, pDevice->TxSize);
  }
}

/*!
 * \fn     DeviceReadDataHandle
 * \brief  数据处理
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
  
  key     = pDevice->RxBuf[2];    // 关键字

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  if((pDevice->RxBuf[0]  == cDeviceHead1)
  &&(pDevice->RxBuf[1]  == cDeviceHead2))
  {
    printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
    // 检查收到全部报文的ＣＲＣ码是否正确？
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
 * \brief  查询设备信息
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxQueryMess(void)
{
  DeviceUartStru     *pDevice;
  unsigned char        ParaSum;  // 参数总数
  
  pDevice    = &gstvDevice;
  ParaSum      = pDevice->RxBuf[4];

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  DeviceTxQueryMessAsk(ParaSum, &pDevice->RxBuf[5]);
}

/*!
 * \fn     DeviceTxQueryMessAsk
 * \brief  查询设备信息应答。
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
  
  pbuf->Key   = cDeviceTxQueryMessAsk;  // 查询设备信息应答
  
  pbuf->Data[0] = ParaSum;
  
  // 解析每个关键字
  for(i=0; i<ParaSum; i++)
  {
    *ptr++ = *pPara;   // 参数ID
    if(*pPara == 0x01)  // 设备ID号
    {
      temp     = cDeviceEuqiIDSize;    // 参数长度
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.EuqiID[0], ptr, temp);
    }
    else if(*pPara == 0x02)  //IMEI号
    {
      temp     = cDeviceGsmImeiSize;    // 参数长度
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.GsmImei[0], ptr, temp);
    }
    else if(*pPara == 0x03)  //MAC地址
    {
      temp     = cDeviceMacSize;    // 参数长度
      *ptr++   =  temp;
      MemCopySramToSram(&gstvDevice.MacAddr[0], ptr, temp);
    }
    else if(*pPara == 0x04)  //硬件版本号
    {
      temp     = 1;    // 参数长度
      *ptr++   =  temp;
      *ptr     = cSysHardwareVer;
    }
    else if(*pPara == 0x05)  //固件版本号
    {
      temp     = 2;    // 参数长度
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
  
  pbuf->Len   = len ;                // 数据长度
}


/*!
 * \fn     DeviceRxSetMess
 * \brief  设置设备信息
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxSetMess(void)
{
  DeviceUartStru    *pDevice;
  unsigned char     ParaID;   // 参数ID
  unsigned char     ParaLen;  // 参数长度
  unsigned char     ParaSum;  // 参数总数
  unsigned char     *ptr;
  unsigned char     i, j;
  unsigned char     EuqiID[cDeviceEuqiIDSize];
  unsigned char     buf[6];
  unsigned char     flg=0;    // 参数出错标示

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  pDevice    = &gstvDevice;
  ParaSum    = pDevice->RxBuf[4];
  ptr        = &pDevice->RxBuf[5];
  
  // 解析每个关键字
  for(i=0; i<ParaSum; i++)
  {
    ParaID  = *ptr++;
    ParaLen = *ptr++;
    
    if(ParaID == 0x01)  // 设备ID号
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

          /* 把设备ID写到 INI 文件里 */
          EeWriteDeviceParamToIni(&buf[0]);          
          ptr   += 6;
        }
        else
        {
          flg =1;
          break;
        }
      }
      else  // 出错
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x02)  // IMEI号
    {
      if(ParaLen  == 0x0f)
      {
        ptr   += 0x0f;
      }
      else  // 出错
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x03)  // MAC地址
    {
      if(ParaLen  == 0x06)
      {
        ptr   += 6;
      }
      else  // 出错
      {
        flg =1;
        break;
      }
    }
    else if(ParaID == 0x04)  // 硬件版本号
    {
      if(ParaLen  == 0x01)
      {
        ptr   += 1;
      }
      else  // 出错
      {
        flg =1;
        break;
      }  
    }
    else if(ParaID == 0x05)  // 固件版本号
    {
      if(ParaLen  == 0x02)
      {
        ptr   += 2;
      }
      else  // 出错
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
  
  // 判断数据是否正确
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
 * \brief  设置设备信息应答
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
  
  pbuf->Key = cDeviceTxSetMessAsk;  // 设置设备信息应答
  
  pbuf->Data[0] = Result;
  
  pbuf->Len   = 1 ;                // 数据长度
}


/*!
 * \fn     DeviceRxWebRtcIdMess
 * \brief  接收webrtc设备ID
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void DeviceRxWebRtcIdMess(void)
{
  DeviceUartStru    *pDevice;
  unsigned char     ParaLen;  // 参数长度
  unsigned char     buf[255];
  
  pDevice = &gstvDevice;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  ParaLen = pDevice->RxBuf[3];

  MemCopySramToSram((unsigned char *)&pDevice->RxBuf[4], (unsigned char *)&buf[0], ParaLen);

  /* 写入到INI文本内 */
  printf("%s\n", buf);
  EeWriteWebrtcIdParamToIni((unsigned char *)&buf[0]);

  DeviceTxWebrtcSetMessAsk(0);
}



/*!
 * \fn     DeviceTxWebrtcSetMessAsk
 * \brief  设置设备信息应答
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
  
  pbuf->Key = cDeviceTxWebrtcSetMessAsk;  // 设置设备信息应答
  
  pbuf->Data[0] = Result;
  
  pbuf->Len   = 1 ;                // 数据长度
}


/*!
 * \fn     DecodeDeviceIDHandle
 * \brief  解析设备ID
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
  
  // 调换位置 + 与固定数组异或
  idbuf2[0]  = idbuf1[4] ^ 0x2d;
  idbuf2[1]  = idbuf1[2] ^ 0x63;
  idbuf2[2]  = idbuf1[5] ^ 0x15;
  idbuf2[3]  = idbuf1[0] ^ 0xed;
  idbuf2[4]  = idbuf1[1] ^ 0x97;
  idbuf2[5]  = idbuf1[3] ^ 0xca;
  
  // 计算异或校验
  if(idbuf2[4] == (DataCheckAccumulateXOR(&idbuf2[0], cDeviceEuqiIDSize)))
  {
    if(0x00 == (DataCheckCrcByte(&idbuf2[0], cDeviceEuqiIDSize+2)))
    {
      // 把ID号存入flash
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
 * \brief  滚动码程序
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
  
  for(q=0;q<576;q++)       //循环576次
  {
    if(q<528)             //最后48次仅执行密钥移位， 为的是将密钥还原
    {
      if(datin[2]&0x10)  //若明文第三字节d4位=1则p=16， 否则p=1
        p=0x10;            
      else
        p=0x01;
    
      if(datin[1]&0x02)  //若明文第二字节d1位=1则p左移两位
        p<<=2;
      if(datin[0]&0x02)  //若明文第一字节d1位=1则p再左移一位
        p<<=1; 
      
      if((datin[3]&0x80)&&(datin[3]&0x04))      
        p&=0x3a;           //若明文第四字节d7位d2位=11， p与上0x3a 
      else if((datin[3]&0x80)&&((datin[3]&0x04)==0))      
        p&=0x5c;           //若明文第四字节d7位d2位=10， p与上0x5c 
      else if(((datin[3]&0x80)==0)&&(datin[3]&0x04))      
        p&=0x74;           //若明文第四字节d7位d2位=01， p与上0x74 
      else if(((datin[3]&0x80)==0)&&((datin[3]&0x04)==0))      
        p&=0x2e;           //若明文第四字节d7位d2位=00， p与上0x2e 

      if(p)              //p不等于0，令p=0x01
        p=0x01;
        
      p^=datin[0];       //p连续异或明文第一字节、明文第三字节、密钥第一字节
      p^=datin[2];
      p^=keyin[0];

      //连续右移p、密文第四字节、密文第三字节、密文第二字节、密文第一字节
      datin[0]=(datin[0]>>1)|(datin[1]<<7);
      datin[1]=(datin[1]>>1)|(datin[2]<<7);
      datin[2]=(datin[2]>>1)|(datin[3]<<7);
      datin[3]=(datin[3]>>1)|(p<<7);
    }

    p=keyin[0];           //循环右移密钥第八字节-密钥第一字节
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
 * \brief  设备接收数据中断处理
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
 * \brief  测试数据
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
  
  pbuf->Key = 0x83;  // 设置设备信息应答

  MemCopySramToSram((unsigned char *)&WebrtcId[0], (unsigned char *)&pbuf->Data[0], sizeof(WebrtcId));


  for(i = 0; i < sizeof(WebrtcId); i++)
  {
    if(WebrtcId[i] == 0x00)
      break;
  }
  pbuf->Len   = i+1;                // 数据长度

  gbitDevice.TranData  = SET;
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
