/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      WiredNetDeal.c
 * \author    yifeng
 * \version   1.0
 * \date      2023��6��30��
 * \brief     ���������������
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
#include "WiredNetDeal.h"

#include "TimerDeal.h"
#include "EepromDeal.h"
#include "NtpDeal.h"
#include "PingDeal.h"
#include "NetDataDeal.h"

#include "PlayPcm.h"

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
static void *WiredNetDeal_Thread(void *arg);

static void WiredLinkDetHandle(void);
static int WiredCheckNic(char *nic_name, int* sock);
static void WiredNetHandle(void);
static bool WiredInitBootHandle(void);
static bool WireIPParaSetHandle(void);
static bool WiredDHCPBootHandle(void);
static int GetIpAddress(char* ifaceName, char *ipaddr);
static bool WiredDNSTcpBootHandle(void);
static bool WiredTcpConnectBootHandle(void);






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
static char* eth0_ifname = "ens33";
static char* ServerHostName = "hz.heyitech.com";
static int ServerPort = 8031;

/*=======================================================================================
                                         ȫ�ֱ�������                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
��ȫ�ֱ������壺
---------------------------------------------------------------------------------------*/
WiredDealStru gstvWired;
WiredDealBits gbitWired;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/



/*!
 * \fn     Start_WiredNetDeal_Monitor_Thread
 * \brief  �������紦�����
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_WiredNetDeal_Monitor_Thread(void)
{
  pthread_t net_tidp;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  memset(&gstvWired, 0, sizeof(WiredDealStru));
  memset(&gbitWired, 0, sizeof(WiredDealBits));
  
  WiredStateInputHandle(EnWsInit, 0);

  
  if(pthread_create(&net_tidp, NULL, WiredNetDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: WiredNetDeal_Thread");
  }  
   
}



/*!
 * \fn     WiredNetDeal_Thread
 * \brief  �������紦���߳�
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *WiredNetDeal_Thread(void *arg)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());


  while(!gstvSysRun.quit)
  {
    /* ������� */
    WiredLinkDetHandle();     
    
    /* ���紦������ */
    WiredNetHandle();

    
    usleep(1000);
  }

  pthread_exit(0);
}



/*!
 * \fn     WireLinkDetHandle
 * \brief  �ж���������״̬
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void WiredLinkDetHandle(void)
{
  int sockfd = 0;
    
  /* ��ʱ1�������� */
  if(TimerGet1msSubb(gstvWired.LinkTiming1s, 1000))
  {
    gstvWired.LinkTiming1s = TimerGet1msInc();
  
    if(WiredCheckNic(eth0_ifname, &sockfd) == 0)  // �в���������
    {
      gstvWired.LinkDetHave++;
      gstvWired.LinkDetLoss = 0;

      printf("gstvWired.LinkDetHave is %d\n", gstvWired.LinkDetHave);
    }
    else  
    {
      gstvWired.LinkDetLoss++;
      gstvWired.LinkDetHave = 0;

      printf("gstvWired.LinkDetLoss is %d\n", gstvWired.LinkDetLoss);
    }

    if(gstvWired.LinkDetHave >= 3)
    {
      gstvWired.LinkDetHave = 0;

      if(gbitWired.LinkState == 0x00)
      {
        printf("PHP Link Success!\r\n");

        /* �����ʼ��ģʽ */
        WiredStateInputHandle(EnWsInit, 0);

        NetDataRxCrirInit();
        NetDataLoginServerInit();

        /* ����NTP���� */
        NtpDealRestartHandle();

        /* ֹͣ */
        StopP2pTaskThread();
      }

      gbitWired.LinkState = 0x01;
    }
    else if(gstvWired.LinkDetLoss >= 3)
    {
      gstvWired.LinkDetLoss = 0;

      printf("PHP Link Error\r\n");
    }
  }
}

/*!
 * \fn     check_nic
 * \brief  ��������Ƿ����
 *          
 * \param  [in] char *nic_name   #
 * \param  [in] int* sock        #
 * 
 * \retval static int
 */
static int WiredCheckNic(char *nic_name, int* sock)
{
  struct ifreq ifr;
  int skfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (-1 == skfd)
  {
    fprintf(stderr, "%s,%s,%d,check_nic sock error\n", __FILE__, __FUNCTION__, __LINE__);
    return -1;
  }
  *sock = skfd;
  strcpy(ifr.ifr_name, nic_name);
  if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
  {
    close(skfd);
    return -1;
  }

  if(ifr.ifr_flags &  IFF_RUNNING)
  {
    close(skfd);
    return 0;  // �����Ѳ�������
  }
  else
  {
    close(skfd);
    return -1;
  }
}






/*!
 * \fn     WiredNetHandle
 * \brief  ���������������
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void WiredNetHandle(void)
{
  int ret = -1;

  if(gbitWired.LinkState == 0x00)
  {
    return ;
  }

  switch(gstvWired.WiredState)
  {
    case EnWsInit:   // ��ʼ��
    {
      ret = WiredInitBootHandle();
      if(ret == true)
      {
        WiredStateInputHandle(EnWsLinkDet, 1);
      }
      else
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      break;
    }
    case EnWsLinkDet:  // �������
    {
      if(TimerGet1sSubb(gstvWired.StateDelay, 10))
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      else
      {
        if(gbitWired.LinkState == 0x01)  // ������
        {
          WiredStateInputHandle(EnWsIpParaSet, 0);
        }
      }
      break;
    }
    case EnWsIpParaSet:  // ����IP, ��̬��̬
    {
      ret = WireIPParaSetHandle();
      if(ret == true)
      {
        WiredStateInputHandle(EnWsDHCP, 0);
      }
      else
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      break;
    }
    case EnWsDHCP:  // ��ȡIP
    {
      ret = WiredDHCPBootHandle();
      if(ret == true)
      {
        WiredStateInputHandle(EnwSDNS, 0);
      }
      else
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      break;
    }
    case EnWsPing:  // ping baidu
    {
      ret = PingDealHanle("www.baidu.com");
      if(ret == 0)
      {
        WiredStateInputHandle(EnwSDNS, 1);
      }
      else if(ret == 2)
      {
      }
      else 
      {
        WiredStateInputHandle(EnWsFail, 1);
      }
      break;
    }
    case EnwSDNS:
    {
      ret = WiredDNSTcpBootHandle();
      if(ret == true)
      {
        WiredStateInputHandle(EnWsTcpConnect, 0);
      }
      else
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      break;
    
      break;
    }
    case EnWsTcpConnect:  // ���ӷ�����
    {
      ret = WiredTcpConnectBootHandle();
      if(ret == true)
      {
        WiredStateInputHandle(EnWsSucc, 0);
      }
      else
      {
        WiredStateInputHandle(EnWsFail, 0);
      }
      break;
    }
    case EnWsCloseTcpSock:  // �ر�socket
    {
      gbitWired.ConnectBoot = 0x00;
      
      if(TimerGet1msSubb(gstvWired.StateDelay, 3000))
      {
        WiredStateInputHandle(EnWsInit, 0);
      }
      break;
    }
    case EnWsFail:
    {
      gbitWired.WiredOk = CLEAR;
      
      gstvWired.ErrorTry++;
      if(gstvWired.ErrorTry >= 5)
      {
        WiredStateInputHandle(EnWsInit, 0);
      }
      else
      {
        WiredStateInputHandle(EnWsFailDelay, 1);
      }
      break;
    }
    case EnWsFailDelay:
    {
      if(TimerGet1sSubb(gstvWired.StateDelay, 10))
      {
        if(gbitWired.LinkState == 0x01)
        {
          WiredStateInputHandle(gstvWired.WiredStateBak, 0);
        }
      }
      break;
    }
    case EnWsSucc:
    {
      gbitWired.WiredOk = 0x01;
      gstvWired.ErrorTry = 0;
      
      break;
    }
    default:
    {
      WiredStateInputHandle(EnWsInit, 0);
      
      break;
    }
  }
}

/*!
 * \fn     WiredInitBootHandle
 * \brief  ��ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static bool WiredInitBootHandle(void)
{  
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  gstvWired.WiredStateBak = EnWsInit;

  /* ����д���socket���ȹر� */
  if(gbitWired.UseSocket == 0x01)
  {
    close(gstvWired.WiredSock);
    printf("Close Wired Sock!\r\n");
  }

  /* ��ʼ�����в��� */
  memset(&gstvWired, 0, sizeof(WiredDealStru));
  memset(&gbitWired, 0, sizeof(WiredDealBits));

  gbitWired.LinkState = 0x01;
  gbitWired.InitBoot = 0x01;

  memcpy(&gstvWired.HostName, ServerHostName, 128);
  printf("HostName: %s\r\n", gstvWired.HostName);
  
  gstvWired.Port = ServerPort;
    
  return true;
}

/*!
 * \fn     WireIPParaSetHandle
 * \brief  ���þ�̬IP��̬IP
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static bool WireIPParaSetHandle(void)
{
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  if(gstvWired.DhcpMode == 0x00)  /* �Զ���ȡ */
  {
    system("udhcpc");
    printf("system(\"udhcpc\");\n");
  }
  //else
  {
    /* ��̬����IP */

    /* ����IP��ַ */

    /* ����Mac��ַ */

    /* ����Bcast��ַ */

    /* ����Mask��ַ */
  }

  printf("IpParaSet succeed!\r\n");
  
  gbitWired.DHCPBoot = 0x01;
  gstvWired.WiredStateBak = EnWsIpParaSet;
  
  return true;
}


/*!
 * \fn     WiredDHCPBootHandle
 * \brief  DHCP��ȡ�������
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static bool WiredDHCPBootHandle(void)
{
  int ret = 0;
  char msg[64];

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  gstvWired.WiredStateBak = EnWsDHCP;
    
  ret = GetIpAddress(eth0_ifname, msg);
  if(ret == 0)
  {
    printf("GetIpAddress: %s, ret = %d\n", msg, ret);

    gbitWired.DHCPBoot = 0x01;
  }
  else 
  {
    printf("GetIpAddress failed!\n");

    gbitWired.DHCPBoot = 0x00; 

    /* һֱʧ�ܾ������������� */

    return false;
  }

  return true;
}

/**********************************************************************
 * Function:      GetIpAddress
 * Description:   ���ָ���˿ڵ�ip��ַ
 * Input:         ifaceName ������ӿ����� �磺eth0��IpAddress���õ���ip��ַ
 * Return:        �ɹ� 1��ʧ�� 0
 * Others:        
**********************************************************************/
static int GetIpAddress(char* ifaceName, char *ipaddr)
{
  int fd;
  char buffer[20];
  size_t len = 16;
  struct ifreq ifr;
  struct sockaddr_in *addr;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
  {
    strncpy(ifr.ifr_name, ifaceName, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
    {
      addr = (struct sockaddr_in *)&(ifr.ifr_addr);
      inet_ntop(AF_INET, &addr->sin_addr, buffer, 20);
    }
    else
    {
      close(fd);
      
      return(-1);
    }
  }
  else
  {
    return(-1);
  }

  if (strlen(buffer) > len-1)
  {
    return(-1);
  }
  
  strncpy(ipaddr, buffer, len);
  
  close(fd);
  
  return(0);
}



/*!
 * \fn     WiredDNSTcpBootHandle
 * \brief  ��������
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static bool WiredDNSTcpBootHandle(void)
{
  extern int h_errno;
  struct hostent *h;
  
  struct sockaddr_in addr_in;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  gstvWired.WiredStateBak = EnwSDNS;
      
  h=gethostbyname((const char*)&gstvWired.HostName);
  if(h==NULL)
  {
    printf("%s", hstrerror(h_errno));

    gbitWired.DnsTcpBoot = 0x00;

    return false;
  }
  else
  {
    memcpy(&addr_in.sin_addr.s_addr,h->h_addr,4);
    gstvWired.DnsIn.s_addr = addr_in.sin_addr.s_addr;
    printf("host name:%s\r\n",h->h_name);
    printf("ip lenght:%d\r\n",h->h_length);  //IPv4 or IPv6
    printf("type:%d\r\n",h->h_addrtype);
    printf("ip:%s\r\n",inet_ntoa(gstvWired.DnsIn));       //��һ��IPת����һ����������׼��ָ�ʽ���ַ���

    gbitWired.DnsTcpBoot = 0x01;
  }

  return true;
}


/*!
 * \fn     WiredTcpConnectBootHandle
 * \brief  TCP���ӷ�����ͨ�Ŵ���
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static bool WiredTcpConnectBootHandle(void)
{
  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);

  /* ��socket���ͷ� */
  if(gbitWired.UseSocket == 0x01)
  {
    close(gstvWired.WiredSock);
    printf("Close Wired Sock!\r\n");
    
    gbitWired.ConnectBoot = 0x00;
    gbitWired.UseSocket   = 0x00;
  }

  /* TCP client */
  gstvWired.WiredSock = socket(AF_INET, SOCK_STREAM, 0);
  if(-1 == gstvWired.WiredSock)
  {
    printf("Create Socket failed!\r\n");
 
    close(gstvWired.WiredSock);
    gbitWired.ConnectBoot = 0x00;
    gbitWired.UseSocket   = 0x00;
    
    return false;
  }
  
  struct sockaddr_in srv_addr;
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(gstvWired.DnsIn));
  srv_addr.sin_port = htons(gstvWired.Port);
  
  printf("TcpConnect ip:%s\r\n",inet_ntoa(gstvWired.DnsIn));     
  printf("TcpConnect port: %d\n", gstvWired.Port);
  
  if(-1 == connect(gstvWired.WiredSock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)))
  {
    printf("Tcp Connect failed!\r\n");

    close(gstvWired.WiredSock);
    gbitWired.ConnectBoot = 0x00;
    gbitWired.UseSocket   = 0x00;
    
    return false;
  }

  printf("TcpConnect Succeed!\r\n");
  gbitWired.ConnectBoot = 0x01;
  gbitWired.UseSocket   = 0x01;

  return true;  
}


/*!
 * \fn     GetWiredTcpSocket
 * \brief  ��ȡ��������tcp socket
 *          
 * \param  [in] void   #
 * 
 * \retval int
 */
int GetWiredTcpSocket(void)
{
  return gstvWired.WiredSock;
}

/*!
 * \fn     GetWiredTcpConnectStatus
 * \brief  ��ȡtcp����״̬
 *          
 * \param  [in] void   #
 * 
 * \retval int
 */
int GetWiredTcpConnectStatus(void)
{
  return gbitWired.ConnectBoot;
}


/*!
 * \fn     WiredStateInputHandle
 * \brief   ���������������״̬
 *          
 * \param  [in] WiredStateEnum WiredState   #
 * 
 * \retval void
 */
void WiredStateInputHandle(WiredStateEnum WiredState, unsigned char DelayMode)
{
  gstvWired.WiredState = WiredState;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  printf("WiredStateInputHandle: %d\n", WiredState);
  
  if(DelayMode == 0)
    gstvWired.StateDelay = TimerGet1msInc();
  else 
    gstvWired.StateDelay = TimerGet1sInc();
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
