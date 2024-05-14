/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      NetDataDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月3日
 * \brief     协议数据处理源程序
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


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "NetDataDeal.h"
#include "CommFun.h"
#include "TimerDeal.h"
#include "WiredNetDeal.h"
#include "UartDevice.h"
#include "EventDeal.h"
#include "PlayPcm.h"

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
static void *NetDataDeal_Thread(void *arg);

static void *NetData_Receive_Thread(void *param);
static void NetDataRecvHandle(void);

static void NetWaitTxAckOverHandle(void);  

static void NetHeartKeepHandle(void);

static void NetRxDataHandle(void);

static void NetTxDataHandle(void);


static void NetRxGeneralRespone(void);
static void NetRxLoginRespone(void);
static void NetRxEquiCtrl(void);



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
const NetKeySubStru cNetKeyTable[]	=
{
  {  0x8000,  NetRxGeneralRespone           },  

  {  0x8001,  NetRxLoginRespone             }, 

  {  0x9004,  NetRxEquiCtrl                 },  

};

/*=======================================================================================
                                         全局变量定义                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局变量定义：
---------------------------------------------------------------------------------------*/
NetDealBits  gbitNet;
NetDealStru  gstvNet;

NetDataQueue gstvNetQue;  

/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
*****************************************************************************************
*/



/*!
 * \fn     Start_NetDataDeal_Monitor_Thread
 * \brief  协议数据处理操作
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_NetDataDeal_Monitor_Thread(void)
{
  pthread_t net_tidp;
  pthread_t recv_tid;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pthread_create(&net_tidp, NULL, NetDataDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: WiredNetDeal_Thread");
  } 

  if(pthread_create(&recv_tid, NULL, NetData_Receive_Thread, NULL) < 0)
  {
    perror("pthread_create: NetData_Receive_Thread");
  }
}



/*!
 * \fn     NetDataDeal_Thread
 * \brief  协议处理
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *NetDataDeal_Thread(void *arg)
{
  NetDealStru    *pNet;

  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());
  
  pNet    = &gstvNet;

  memset(&gstvNet, 0, sizeof(NetDealStru));
  memset(&gbitNet, 0, sizeof(NetDealBits));  

  NetDataRxCrirInit();
  NetDataLoginServerInit();

  while(!gstvSysRun.quit)
  {
    if(TimerGet1msSubb(pNet->RxTxTick, 100))
    {
      pNet->RxTxTick  = TimerGet1msInc();

      if((gbitWired.WiredOk  == SET)
      &&(gbitWired.ConnectBoot == SET))
      {
        NetTxDataHandle();    // 发送数据处理
        
        NetRxDataHandle();    // 接收数据处理
      }
    }

    if(TimerGet1msSubb(pNet->UpdateStateTick, 1000))
    {
      pNet->UpdateStateTick  = TimerGet1msInc();
      
      if((gbitWired.WiredOk  == 0x01)
      &&(gbitWired.ConnectBoot == 0x01))
      {
        NetWaitTxAckOverHandle();
        NetHeartKeepHandle();  
      }
    }


    if(gstvNet.TxMKeyBak != gstvNet.TxMKey)
    {
      printf("====>TxMKey: %d\n", gstvNet.TxMKey);
      gstvNet.TxMKeyBak = gstvNet.TxMKey;
    }

    if((TimerGet1sSubb(gstvNet.TestDelay, 10))
    &&(gbitNet.LoginSuccuess == SET))
    {
      gstvNet.TestDelay = TimerGet1sInc();

      EventCodeWriteAlarmToEvent(EnAcSosAlarm, 1);  
    }
    
    usleep(5*1000);
  }

  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  printf("==========> NetDataDeal_Thread <==============");
  
  pthread_exit(0);
}

/*!
 * \fn     NetData_Receive_Thread
 * \brief  网络数据接收处理线程
 *          
 * \param  [in] void *param   #
 * 
 * \retval static void *
 */
static void *NetData_Receive_Thread(void *param)
{
  pthread_detach(pthread_self());
  
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  QueueInit(&gstvNetQue, sizeof(NetDataQueue), cNetDataNum, sizeof(NetDataStru));

  while(!gstvSysRun.quit)
  {
    NetDataRecvHandle();

    usleep(5*1000);
  }

  pthread_exit(0);
}


/*!
 * \fn     NetDataRecvHandle
 * \brief  数据接收处理(阻塞模式)
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NetDataRecvHandle(void)
{
  int ret = 0;
  int i = 0;
  unsigned char Recv_Buf[1024];
  
  if(GetWiredTcpConnectStatus() == 0)
  {
    return ;
  }

  ret = recv(GetWiredTcpSocket(), Recv_Buf, sizeof(Recv_Buf), 0);
  if(ret > 0)
  {
    printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

    /* 接收到数据，放到队列处理 */
    CircInsert((unsigned char *)&gstvNet.RxCrir, (unsigned char *)&Recv_Buf[0], ret);  // 把接收到的数据插入到环形队列中 

    printf("NetDataCrir->Front: %d\n",  gstvNet.RxCrir.Front);
    printf("NetDataCrir->Rear: %d\n",  gstvNet.RxCrir.Rear);
    printf("NetDataCrir->Size: %d\n",  gstvNet.RxCrir.Size);
    printf("NetDataCrir->Escape: %d\n",  gstvNet.RxCrir.Escape);
    printf("NetDataCrir->ByEscape: %d\n",  gstvNet.RxCrir.ByEscape);
    printf("NetDataCrir->Buf: %d\n",  gstvNet.RxCrir.Buf[0]);
    
    gstvNet.RxDelay = TimerGet1minInc();
    
    printf("Net Receive: ");
    for(i = 0; i < ret; i++)
    {
      printf("0x%x ", Recv_Buf[i]);
    }
    printf("\r\n");
  }
  else if(ret <= 0) 
  {
    gbitNet.TxFlg = 0x00;
    WiredStateInputHandle(EnWsCloseTcpSock, 0);
    NetDataRxCrirInit();
    NetDataLoginServerInit();
  }
  else
  {
  }
}


/*!
 * \fn     NetWaitTxAckOverHandle
 * \brief  终端等待服务器应答处理
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NetWaitTxAckOverHandle(void)  
{
  NetDealStru    *pNet;
  unsigned short front;
  
  pNet    = &gstvNet;
  
  // 等待终端登录请求应答
  if(gbitNet.WaitLoginAck  == SET)
  {
    if(TimerGet1sSubb(pNet->WaitLoginAckDelay, cNetWaitLoginAckTick))
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

      gbitNet.WaitLoginAck = CLEAR;
      pNet->WaitLoginAckCnt++;
      if(pNet->WaitLoginAckCnt >= cNetWaitLoginAckMax)
      {
        pNet->WaitLoginAckCnt = 0;
        pNet->TxSerial.word++;
       
        WiredStateInputHandle(EnWsCloseTcpSock, 0);
        NetDataRxCrirInit();
        NetDataLoginServerInit();
      }
    }
  }

  // 等待终端心跳应答
  if(gbitNet.WaitHeartAck  == SET)
  {
    if(TimerGet1sSubb(pNet->WaitHeartAckDelay, cNetWaitHeartAckTick))
    {
      gbitNet.WaitHeartAck  = CLEAR;
      pNet->TxMKey  = EnTxCtrlData;
      pNet->WaitHeartAckCnt++;
      if(pNet->WaitHeartAckCnt >= cNetWaitHeartAckMax)
      {
        pNet->WaitHeartAckCnt = 0;
        pNet->TxSerial.word++;

        WiredStateInputHandle(EnWsCloseTcpSock, 0);
        NetDataRxCrirInit();
        NetDataLoginServerInit();
      }
    }
  }

  // 等待终端控制数据请求应答
  if(gbitNet.WaitDataAck  == SET)
  {
    if(TimerGet1sSubb(pNet->WaitDataAckDelay, cNetWaitDataAckTick))
    {
      gbitNet.WaitDataAck  = CLEAR;
      pNet->WaitDataAckCnt++;
      if(pNet->WaitDataAckCnt >= cNetWaitDataAckMax)
      {
        pNet->WaitDataAckCnt = 0;
        // 删除缓存区
        pNet->TxSerial.word++;
        QueueDelete(&gstvNetQue);
      }
    }
  }

  // 等待终端报警数据应答
  if(gbitNet.WaitAlarmAck  == SET)
  {
    if(TimerGet1sSubb(pNet->WaitAlarmAckDelay, cNetWaitAlarmAckTick*(pNet->WaitAlarmAckCnt+1)))
    {
      gbitNet.WaitAlarmAck  = CLEAR;
      pNet->WaitAlarmAckCnt++;
      
      if(pNet->WaitAlarmAckCnt >= cNetWaitAlarmAckMax)
      {
        pNet->WaitAlarmAckCnt = 0;
        pNet->TxSerial.word++;
        
        // 删除报警缓存区
        front = gstvEvent.Front;
        front  += pNet->TxAlarmCnt;
        if(front >= cEventCodeNum)
        {
          front -= cEventCodeNum;
        }
        
        gstvEvent.Front = front;
        if(gstvEvent.Front > gstvEvent.Rear)
        {
          gstvEvent.Front = gstvEvent.Rear;
        }

        WiredStateInputHandle(EnWsCloseTcpSock, 0);
        NetDataRxCrirInit();
        NetDataLoginServerInit();
      }
    }
  }
  
}


/*!
 * \fn     NetHeartKeepHandle
 * \brief  终端心跳触发处理
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NetHeartKeepHandle(void)
{
  /* 每60秒一次心跳 */
  if(TimerGet1sSubb(gstvNet.HeartTick, cNetHeartTickOver))
  {
    printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    gstvNet.HeartTick  = TimerGet1sInc();  // 更新心跳时间
    
    if((gbitNet.WaitLoginAck  == CLEAR)    // 等待终端登录请求应答
     &&(gbitNet.WaitHeartAck  == CLEAR)    // 等待终端心跳应答
     &&(gbitNet.WaitDataAck   == CLEAR)    // 等待终端控制数据请求应答
     &&(gbitNet.WaitAlarmAck  == CLEAR)    // 等待终端报警数据应答
     &&(gbitNet.LoginSuccuess == SET))
    {
      gstvNet.TxMKey = EnTxHeartBeat; 
    }
  }
}



/*!
 * \fn     NetRxDataHandle
 * \brief  接收数据处理
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NetRxDataHandle(void)
{
  NetDealStru    *pNet;
  const NetKeySubStru  *pTab;
  unsigned short  len;
  unsigned char   buf[cNetRxBufSize];
  WORD_BYTES      CRC16;                      // CRC16校验
  WORD_BYTES      MessLen;                    // 消息体长度
  unsigned char   i;
  unsigned char   flg=0;
  
  pNet    = &gstvNet;
  pTab    = cNetKeyTable;

  if(CircBusy((unsigned char *)&gstvNet.RxCrir) == TRUE)   // 环形队列中有完整的数据
  {
    printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

    len    = CircGain((unsigned char *)&gstvNet.RxCrir, (unsigned char *)&buf[0]);
    len    = NetEscapeDecodingHandle((unsigned char *)&buf, (unsigned char *)&pNet->RxBuf, len);
    pNet->RxSize  = len;
    CRC16.word    = DataCheckAccumulateCRC16((unsigned char *)&pNet->RxBuf, len-2);  
    
    // 判断CRC16校验是否正确
    if((pNet->RxBuf[len-2] == CRC16.byte.high)
    &&(pNet->RxBuf[len-1] == CRC16.byte.low))
    {
      gstvNet.HeartTick  = TimerGet1sInc();  // 更新心跳时间
      
      pNet->CurrNetData  = 0x00;            // wifi数据
    
      flg  = 0;
      if(pNet->RxSize != pNet->RxSizeBak)
      {
        flg  = 1;
      }
      else
      {
        if(MemCompSramAndSram2((unsigned char *)&pNet->RxBuf[0], (unsigned char *)&pNet->RxBufBak[0], pNet->RxSize) == 1)
        {
          flg  = 1;
        }
      }
      
      if(flg  == 1)
      {
        pNet->RxSizeBak = pNet->RxSize;
        MemCopySramToSram2((unsigned char *)&pNet->RxBuf[0], (unsigned char *)&pNet->RxBufBak[0], pNet->RxSize);
        
        pNet->RxKeyWord.byte.high =  pNet->RxBuf[0];  // 对应的平台消息的ID
        pNet->RxKeyWord.byte.low  =  pNet->RxBuf[1];
        MessLen.byte.high         =  pNet->RxBuf[2];
        MessLen.byte.low          =  pNet->RxBuf[3];  // 消息体长度
        pNet->RxSize              =  MessLen.word &0x03ff;
        pNet->RxSerial.byte.high  =  pNet->RxBuf[8];
        pNet->RxSerial.byte.low   =  pNet->RxBuf[9]; // 对应的平台消息的流水号
        
        // 判断设备ID号是否一致。
        if(MemCompSramAndSram((unsigned char *)&pNet->RxBuf[4], (unsigned char *)&gstvDevice.EuqiID[0], cDeviceEuqiIDSize) ==0)
        {
          MemCopySramToSram2(&pNet->RxBuf[10], &pNet->RxBuf[0], pNet->RxSize);
          
          // 寻找消息ID所对应的函数
          for(i=0; i<sizeof(cNetKeyTable)/sizeof(NetKeySubStru); i++)
          {
            if(pNet->RxKeyWord.word == pTab->Key)
            {
              printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
              printf("Net RxKeyWord = 0x%x\r\n", pNet->RxKeyWord.word);
              pTab->KeySubProgram();
              break;
            }
            pTab++;
          }
          if(i >= sizeof(cNetKeyTable)/sizeof(NetKeySubStru))  // 消息ID不在表格内，回复通用应答
          {
            NetTxGeneralRespone(&pNet->RxSerial, &pNet->RxKeyWord, EnGrNoSupportPart);
          }
        }
        else    // 设备ID号是不一致
        { 
          len = 0;
        }
      }
    }
    else
    {
      len = 0;
    }
  }
}



/*!
 * \fn     NetTxDataHandle
 * \brief  发送数据处理
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NetTxDataHandle(void)
{
  NetDealStru     *pNet;
  unsigned char   buf[cNetRxBufSize];
  WORD_BYTES      CRC16;                      // CRC16校验
  unsigned short  len;  //总长度
  EventFormatStru Alarm;
  unsigned char   *ptr;
  unsigned short  front;
  unsigned char   AlarmCnt;
  unsigned short  MessLen;  // 消息体长度
  int  i = 0;
  
  pNet    = &gstvNet;

  if(pNet->TxMKey  == EnTxLoginRequest)
  {
    if((gbitNet.WaitLoginAck == CLEAR)
     &&(gbitDevice.NoGetID == CLEAR))
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      buf[0]  = cNetTxLoginRequest >> 8;
      buf[1]  = cNetTxLoginRequest;   // 消息ID
      buf[2]  = 0x00;
      buf[3]  = 0x0b;                  // 消息体属性
      MemCopySramToSram(&gstvDevice.EuqiID[0], &buf[4], cDeviceEuqiIDSize);
      buf[8]  = pNet->TxSerial.byte.high; 
      buf[9]  = pNet->TxSerial.byte.low; // 消息流水号
      buf[10] = cNetStategLan;     // 网络状态
      buf[11] = cSysHardwareVer;     // 硬件版本
      buf[12] = cSysSoftwareVer >>  8;
      buf[13] = cSysSoftwareVer & 0xff;     // 软件版本
      buf[14] = 0x06;     // MAC地址固定长度
      MemCopySramToSram(&gstvDevice.MacAddr[0], &buf[15], cDeviceMacSize);
      len     = 21;       // 数据长度，不包括校验
      CRC16.word = DataCheckAccumulateCRC16((unsigned char *)&buf[0], len);
      buf[21] = CRC16.byte.high;
      buf[22] = CRC16.byte.low;
      len     = 23;
  
      pNet->TxSize = NetEscapeCodingHandle(buf, &pNet->TxBuf[1], len);
      
      send(GetWiredTcpSocket(), pNet->TxBuf,  pNet->TxSize, 0);
      gbitNet.TxFlg = 0x01;

      printf("Send Net Data(Login): ");
      for(int i = 0; i < pNet->TxSize; i++)
      {
        printf("0x%x ", pNet->TxBuf[i]);
      }
      printf("\r\n");
      
      gbitNet.WaitLoginAck    = SET;
      pNet->WaitLoginAckDelay = TimerGet1sInc();

      pNet->TxKeyWord.word = cNetTxLoginRequest;
    }
  }
  else if(pNet->TxMKey  == EnTxHeartBeat)
  {
    if(gbitNet.WaitHeartAck  == CLEAR)
    {
      buf[0]  = cNetTxheartbeat >> 8;
      buf[1]  = (u8)cNetTxheartbeat;   // 消息ID
      buf[2]  = 0x00;
      buf[3]  = 0x05;                // 消息体属性
      MemCopySramToSram(&gstvDevice.EuqiID[0], &buf[4], cDeviceEuqiIDSize);
      buf[8]  = pNet->TxSerial.byte.high; 
      buf[9]  = pNet->TxSerial.byte.low; // 消息流水号
      buf[10] = cNetStateWifi;     // 网络状态
      buf[11] = pNet->StateFlags.byte.high; 
      buf[12] = pNet->StateFlags.byte.low; 
      buf[13] = pNet->AlarmFlags.byte.high;    
      buf[14] = pNet->AlarmFlags.byte.low;
      len     = 15;       // 数据长度，不包括校验
      CRC16.word = DataCheckAccumulateCRC16(buf, len);
      buf[15] = CRC16.byte.high;
      buf[16] = CRC16.byte.low;
      len     = 17;
  
      pNet->TxSize = NetEscapeCodingHandle(buf, &pNet->TxBuf[1], len);
           
      send(GetWiredTcpSocket(), pNet->TxBuf,  pNet->TxSize, 0);
     
      gbitNet.WaitHeartAck = SET;
      pNet->WaitHeartAckDelay = TimerGet1sInc();

      pNet->TxKeyWord.word = cNetTxheartbeat;
      
      printf("Send Net Data (heartbeat): ");
      for(int i = 0; i < pNet->TxSize; i++)
      {
        printf("0x%x ", pNet->TxBuf[i]);
      }
      printf("\r\n");
        
    }
  }
  else if(pNet->TxMKey  == EnTxAlarmData)
  {
    if(gbitNet.WaitAlarmAck == CLEAR)
    {
      if(gstvEvent.Front != gstvEvent.Rear)
      {
        buf[0] = cNetTxAlarmEvent >> 8;
        buf[1] = (u8)cNetTxAlarmEvent;   // 消息ID
        MemCopySramToSram(&gstvDevice.EuqiID[0], &buf[4], cDeviceEuqiIDSize);
        buf[8] = pNet->TxSerial.byte.high; 
        buf[9] = pNet->TxSerial.byte.low; // 消息流水号
        
        ptr   = &buf[11];
    
        AlarmCnt = 0;
        MessLen  = 0;
        front    = gstvEvent.Front;
        
        for(i=0; i<5; i++) // 每次最多发送5条警情
        {
          EventReadDataToEeprom(&Alarm, front);
          
          printf("EventReadDataToEeprom: \n");
          printf("Alarm[%d].AlarmEnum: 0x%x\n", front, Alarm.AlarmEnum);
          printf("Alarm[%d].List: 0x%x\n", front, Alarm.List);
          printf("Alarm[%d].YearL: 0x%x\n", front, Alarm.YearL);
          printf("Alarm[%d].Month: 0x%x\n", front, Alarm.Month);
          printf("Alarm[%d].Day: 0x%x\n", front, Alarm.Day);
          printf("Alarm[%d].Hour: 0x%x\n", front, Alarm.Hour);
          printf("Alarm[%d].Minute: 0x%x\n", front, Alarm.Minute);
          printf("Alarm[%d].Second: 0x%x\n", front, Alarm.Second);
          printf("Alarm[%d].Type: 0x%x\n", front, Alarm.Type);
          printf("Alarm[%d].AlarmCode: 0x%x\n", front, Alarm.AlarmCode);
          printf("Alarm[%d].Aear: 0x%x\n", front, Alarm.Aear);
          printf("Alarm[%d].Zone: 0x%x\n", front, Alarm.Zone);
          
          *ptr++ = Alarm.List;  // 警情分类
          *ptr++ = 12;  // 参数长度
          *ptr++ = Alarm.YearL;  // 年
          *ptr++ = Alarm.Month; // 月
          *ptr++ = Alarm.Day;   // 日
          *ptr++ = Alarm.Hour;  // 时
          *ptr++ = Alarm.Minute;// 分
          *ptr++ = Alarm.Second;// 秒
          *ptr++ = Alarm.Type;  // 警情类型
          *ptr++ = Alarm.AlarmCode >> 8;
          *ptr++ = Alarm.AlarmCode; // 警情内容
          *ptr++ = Alarm.Aear;  // 分区号
          *ptr++ = Alarm.Zone   >> 8;
          *ptr++ = Alarm.Zone; // 防区号
   
          front++;
          AlarmCnt++;  // 警情计数
          if(front >= cEventCodeNum)
          {
            front = 0;
          }
   
          if(front == gstvEvent.Rear)
          {
            break;
          }
        }
        
        pNet->TxAlarmCnt  = AlarmCnt;   // 本次发送的警情条数
        buf[10]  = AlarmCnt;            // 警情总数
        len      = 12 + 1+ AlarmCnt*14;
        MessLen  = AlarmCnt*14 +1 ;
          
        buf[2]  = MessLen >> 8;
        buf[3]  = MessLen;                  // 消息体属性
     
        CRC16.word = DataCheckAccumulateCRC16(buf, len-2);
        buf[len-2] = CRC16.byte.high ;
        buf[len-1] = CRC16.byte.low;
    
        pNet->TxSize = NetEscapeCodingHandle(buf, &pNet->TxBuf[1], len);
       
        send(GetWiredTcpSocket(), pNet->TxBuf,  pNet->TxSize, 0);
        
        gbitNet.WaitAlarmAck = SET;
        pNet->WaitAlarmAckDelay = TimerGet1sInc();

        pNet->TxKeyWord.word = cNetTxAlarmEvent;

        printf("Send Net Data (AlarmData): ");
        for(int i = 0; i < pNet->TxSize; i++)
        {
          printf("0x%x ", pNet->TxBuf[i]);
        }
        printf("\r\n");
      }
    }
  }
  else if(pNet->TxMKey  == EnTxCtrlData)
  {
    if(gbitNet.WaitDataAck == CLEAR)
    {
      if(QueueBusy(&gstvNetQue))
      {
        len     = 0;
        ptr     = &buf[0];
        front   = gstvNetQue.Front;
        *ptr++  = gstvNetQue.Buf[front].Key >> 8;
        *ptr++  = gstvNetQue.Buf[front].Key;
        ptr    += 2;
        MemCopySramToSram(&gstvDevice.EuqiID[0], ptr, cDeviceEuqiIDSize);
        ptr    += cDeviceEuqiIDSize;
        *ptr++  = pNet->TxSerial.byte.high; 
        *ptr++  = pNet->TxSerial.byte.low; // 消息流水号
        len     = 12; // 消息头+校校验
        MemCopySramToSram2((unsigned char *)&gstvNetQue.Buf[front].Data[0], ptr, gstvNetQue.Buf[front].Len);
        len    += gstvNetQue.Buf[front].Len;
        
        buf[2]  = gstvNetQue.Buf[front].Len >> 8;
        buf[3]  = gstvNetQue.Buf[front].Len;                  // 消息体属性
        
        CRC16.word = DataCheckAccumulateCRC16(buf, len-2);
        buf[len-2] = CRC16.byte.high;
        buf[len-1] = CRC16.byte.low;
        
        pNet->TxSize = NetEscapeCodingHandle(buf, &pNet->TxBuf[1], len);
        
        send(GetWiredTcpSocket(), pNet->TxBuf,  pNet->TxSize, 0);

        printf("Send Net Data (CtrlData): ");
        for(int i = 0; i < pNet->TxSize; i++)
        {
          printf("0x%x ", pNet->TxBuf[i]);
        }
        printf("\r\n");
        
        // 需要应答的消息ID
        if((gstvNetQue.Buf[front].Key  == cNetTxTimeRequest)
        ||(gstvNetQue.Buf[front].Key  == cNetTxServerOperate)
        ||(gstvNetQue.Buf[front].Key  == cNetTxUpdateState))
        {
          gbitNet.WaitDataAck = SET;
          pNet->WaitDataAckDelay = TimerGet1sInc();
        }
        else  //不用应答
        {
          gbitNet.WaitDataAck  = CLEAR;
          pNet->WaitDataAckCnt = 0;
          pNet->TxSerial.word++;
          QueueDelete(&gstvNetQue);
        }
        pNet->TxKeyWord.word = gstvNetQue.Buf[front].Key;
      }
      else
      {
        if(gbitNet.LoginSuccuess == CLEAR)
        {
          pNet->TxMKey  = EnTxLoginRequest;
          pNet->WaitLoginAckCnt = 0x00;
          gbitNet.WaitLoginAck  = CLEAR;
        }
        else
        {
          pNet->TxMKey  = EnTxAlarmData;
          gbitNet.WaitAlarmAck = CLEAR;
          gstvNet.WaitAlarmAckCnt = 0;
        }
      }
    }
  }
  else
  {
    if(gbitNet.LoginSuccuess == CLEAR)
    {
      pNet->TxMKey = EnTxLoginRequest;
      pNet->WaitLoginAckCnt = 0x00;
      gbitNet.WaitLoginAck  = CLEAR;
    }
    else
    {
      pNet->TxMKey = EnTxCtrlData;
      gbitNet.WaitDataAck    = CLEAR;
      gstvNet.WaitDataAckCnt = 0;
    }
  }
}



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
                                                 )           
{  
  unsigned char   *pSrc;  
  unsigned char   *pDst;  
  unsigned short  Len;  
  unsigned short  DstLen;  
  unsigned short  i;  
 
  pSrc    = pSrouce;  
  pDst    = pDestin;  
  Len     = DataLen;  
  DstLen  = 0x00;  
 
  for(i=0; i< Len; i++)  
  {  
    if(*pSrc == cNetEscapeChar)  
    {  
      pSrc++;  
      i++;  

      if(*pSrc == 0x01)  
      {  
        *pDst = cNetEscapeChar;  
        pDst++;  
        pSrc++;  
      }  
      else  
      {  
        *pDst = cNetByEscapeChar;  
        pDst++;  
        pSrc++;  
      }  
    }  
    else
    {  
      *pDst = *pSrc;  
      pDst++;  
      pSrc++;  
    }  
    DstLen++;  
  }  
  return(DstLen);
}

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
                           )    
{  
  unsigned char    *pSrc;  
  unsigned char    *pDst;  
  unsigned short    Len;  
  unsigned short    DstLen;  
  unsigned short    i;  
 
  pSrc    = pSrouce;  
  pDst    = pDestin;  
  Len     = DataLen;  
  DstLen  = 0x00;  
 
  for(i=0; i< Len; i++)  
  {  
    if(*pSrc == cNetByEscapeChar)  
    {  
      *pDst   = cNetEscapeChar;  
      pDst++;  
      *pDst   = 0x02;  
      pDst++;  
      DstLen++;  
      pSrc++;  
    }  
    else if(*pSrc == cNetEscapeChar)  
    {  
      *pDst   = cNetEscapeChar;  
      pDst++;  
      *pDst   = 0x01;  
      pDst++;  
      DstLen++;  
      pSrc++;  
    }  
    else  
    {   
      *pDst = *pSrc;  
      pDst++;  
      pSrc++;  
    }  
    DstLen++;  
  }  
  
  pDst    = pDestin;  
  pDst--;  
  *pDst   = cNetByEscapeChar;  
  DstLen++;  
  pDst   += DstLen;  
  *pDst   = cNetByEscapeChar;  
  DstLen++;  
  return(DstLen);  
}



/*!
 * \fn     NetDataLoginServerInit
 * \brief  重新登录服务器初始化
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataLoginServerInit(void)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  gbitNet.WaitLoginAck    = CLEAR;
  gbitNet.WaitHeartAck    = CLEAR;
  gbitNet.WaitDataAck     = CLEAR;
  gbitNet.WaitAlarmAck    = CLEAR;
  gbitNet.LoginSuccuess   = CLEAR;
  gstvNet.TxMKey          = EnTxLoginRequest;
  
  gstvNet.WaitLoginAckCnt = 0;
  gstvNet.WaitHeartAckCnt = 0;
  gstvNet.WaitDataAckCnt  = 0;
  gstvNet.WaitAlarmAckCnt = 0;
}


/*!
 * \fn     NetDataRxCrirInit
 * \brief  数据接收队列初始化
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NetDataRxCrirInit(void)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  CircInit((unsigned char *)&gstvNet.RxCrir, sizeof(NetDataCrir), cNetCrirBufSize, cNetByEscapeChar);

  printf("NetDataCrir->Front: %d\n",    gstvNet.RxCrir.Front);
  printf("NetDataCrir->Rear: %d\n",     gstvNet.RxCrir.Rear);
  printf("NetDataCrir->Size: %d\n",     gstvNet.RxCrir.Size);
  printf("NetDataCrir->Escape: %d\n",   gstvNet.RxCrir.Escape);
  printf("NetDataCrir->ByEscape: %d\n", gstvNet.RxCrir.ByEscape);
  printf("NetDataCrir->Buf: %d\n",      gstvNet.RxCrir.Buf[0]);
}
                      

/*------------------------------------------------------------------------------------  
 数据〖终端通用应答〗  
-------------------------------------------------------------------------------------*/  
void  NetTxGeneralRespone(WORD_BYTES* SerialNum,    
                          WORD_BYTES* KeyWord,    
                          NetGenResEnum  Results)     
{
  NetDataStru  buf;
  
  buf.Key     = cNetTxGeneralRespone;  // 终端通用应答
  buf.Len     = 0x0;                 // 数据长度
  buf.Data[0] = SerialNum->byte.high;
  buf.Data[1] = SerialNum->byte.low;
  buf.Data[2] = KeyWord->byte.high;
  buf.Data[3] = KeyWord->byte.low;
  buf.Data[4] = Results;
  
  QueueInsert((unsigned char *)&gstvNetQue, (unsigned char *)&buf, sizeof(NetDataStru));
}

/*------------------------------------------------------------------------------------  
数据 〖平台通用应答〗
-------------------------------------------------------------------------------------*/  
static void NetRxGeneralRespone(void)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  NetDealStru    *pNet;
  WORD_BYTES     ResSerial;      // 对应的终端消息的流水号
  WORD_BYTES     ResKeyWord;     // 对应的终端消息的ID
  NetGenResEnum  result;         // 回复结果
  unsigned short front;

  pNet = &gstvNet;

  ResSerial.byte.high   = pNet->RxBuf[0];
  ResSerial.byte.low    = pNet->RxBuf[1];
  ResKeyWord.byte.high  = pNet->RxBuf[2];
  ResKeyWord.byte.low   = pNet->RxBuf[3];
  result                = (NetGenResEnum)pNet->RxBuf[4];

  if((pNet->TxKeyWord.word == ResKeyWord.word)
  &&(pNet->TxSerial.word == ResSerial.word))
  {
    if((result == EnGrSuccessPart)
    ||(result == EnGrNoSupportPart)
    ||(result == EnGrFalsePart))
    {
      if(pNet->TxKeyWord.word == cNetTxAlarmEvent)
      {
        gbitNet.WaitAlarmAck  = CLEAR;
        pNet->WaitAlarmAckCnt = 0;
        
        // 删除报警缓存区
        front = gstvEvent.Front;
        front  += pNet->TxAlarmCnt;
        if(front >= cEventCodeNum)
        {
          front -= cEventCodeNum;
        }
        gstvEvent.Front = front;
        if(gstvEvent.Front > gstvEvent.Rear)
        {
          gstvEvent.Front = gstvEvent.Rear;
        }
        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      }
      else if(pNet->TxKeyWord.word == cNetTxheartbeat)
      {
        gbitNet.WaitHeartAck = CLEAR;
        pNet->WaitHeartAckCnt = 0;

        pNet->TxMKey  = EnTxCtrlData;

        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      }
      else
      {
        gbitNet.WaitDataAck = CLEAR;
        pNet->WaitDataAckCnt = 0;
        QueueDelete(&gstvNetQue);
        printf("QueueDelete %s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      }
    }
    else if(result == EnGrCrcErrorPart)
    {
    }
    else if(result == EnGrAlarmSurePart)
    {
    }
    else
    {
    }

    pNet->TxSerial.word++;
  }
}


/*------------------------------------------------------------------------------------  
数据 〖终端登录应答〗
-------------------------------------------------------------------------------------*/ 
static void NetRxLoginRespone(void)
{
  unsigned char result;

  result = gstvNet.RxBuf[2];
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  printf("NetRxLoginRespone  NetRxLoginRespone: %d\n", result);
  
  if(result  == 0x00)
  {
    // 取消登录等待
    gbitNet.WaitLoginAck = CLEAR;
    gstvNet.WaitLoginAckCnt = 0;
  
    // 转到发送报警数据
    gstvNet.TxMKey  = EnTxCtrlData;   
    gbitNet.WaitDataAck    = CLEAR;
    gstvNet.WaitDataAckCnt = 0;
  
    gbitNet.LoginSuccuess = SET;   // 登陆成功
    gstvNet.LoginCnt   = 0;         // 登陆次数
  }
}

/*------------------------------------------------------------------------------------  
数据 〖终端控制〗
-------------------------------------------------------------------------------------*/ 
static void NetRxEquiCtrl(void)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  NetDealStru    *pNet;
  unsigned char  ParaID;  // 参数ID
  unsigned char  ParaVal;

  pNet = &gstvNet;
  
  ParaID  = pNet->RxBuf[0];
  ParaVal = pNet->RxBuf[2];

  if(ParaID == 0x01)  // 离家布防
  {
    NetTxCtrlRespone(&pNet->RxSerial, ParaID, 0x00);
  }
  else if(ParaID == 0x02)  // 在家布防
  {
    NetTxCtrlRespone(&pNet->RxSerial, ParaID, 0x00);
  }
  else if(ParaID  == 0x03)  // 撤防
  {
    NetTxCtrlRespone(&pNet->RxSerial, ParaID, 0x00);
  }
  else if(ParaID == 0x04)  // 开启报警声
  {
    gstvSysRun.StartP2p = 0x01;   // 启动信号
    NetTxCtrlRespone(&pNet->RxSerial, ParaID, 0x00);
  }
  else if(ParaID == 0x05)  // 关闭报警声
  {
    StopP2pTaskThread();
    NetTxCtrlRespone(&pNet->RxSerial, ParaID, 0x00);
  }
  else
  {
    
  }
}


/*------------------------------------------------------------------------------------  
 数据〖终端控制应答〗  
-------------------------------------------------------------------------------------*/ 
void  NetTxCtrlRespone(WORD_BYTES* SerialNum,     // 应答流水号
                       unsigned char Command,       // 命令字
                       unsigned char  Result         // 结果
                       )
{
  NetDataStru  buf;

  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  
  buf.Key     = cNetTxCtrlRespone;     // 终端控制应答
  buf.Len     = 0x0a;                  // 数据长度
  buf.Data[0] = SerialNum->byte.high;
  buf.Data[1] = SerialNum->byte.low;
  buf.Data[2] = Command;
  buf.Data[3] = Result;

  EventDateTimeConvToBCD();
  MemCopySramToSram(&gstvEvent.DateTime.YearL, &buf.Data[4], 0x06);
  
  QueueInsert((unsigned char *)&gstvNetQue, (unsigned char *)&buf, sizeof(NetDataStru));
  
  gstvNet.TxMKey  = EnTxCtrlData;
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
