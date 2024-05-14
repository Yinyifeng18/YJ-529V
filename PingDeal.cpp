/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      PingDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023��3��16��
 * \brief     Ping����Դ����
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��3��16��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "PingDeal.h"
#include "TimerDeal.h"

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
struct timeval FirstSendTime; //���Լ����ܵ�ʱ��
struct timeval LastRecvTime;
struct hostent *pHost = NULL;  //����������Ϣ

PingDealStru gstvPing;

struct sockaddr_in dest_addr;  //IPv4ר��socket��ַ,����Ŀ�ĵ�ַ

/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/





/*!
 * \fn     Compute_cksum
 * \brief  У��
 *          
 * \param  [in] struct icmp *pIcmp   #
 * 
 * \retval u16
 */
u16 Compute_cksum(struct icmp *pIcmp)
{
  u16 *data = (u16 *)pIcmp;
  int len = ICMP_LEN;
  u32 sum = 0;
  
  while (len > 1)
  {
    sum += *data++;
    len -= 2;
  }
  if (1 == len)
  {
    u16 tmp = *data;
    tmp &= 0xff00;
    sum += tmp;
  }
  
  //ICMPУ��ʹ���λ
  while (sum >> 16)
  sum = (sum >> 16) + (sum & 0x0000ffff);
  sum = ~sum;
  
  return sum;
}
 
/*!
 * \fn     SetICMP
 * \brief  ����ICMP
 *          
 * \param  [in] u16 seq   #
 * 
 * \retval void
 */
void SetICMP(u16 seq)
{
  struct icmp *pIcmp;
  struct timeval *pTime;
  
  pIcmp = (struct icmp*)&gstvPing.SendBuffer;
  
  /* ���ͺʹ���ֱ�ΪICMP_ECHO,0����������� */
  pIcmp->icmp_type  = ICMP_ECHO;
  pIcmp->icmp_code  = 0;
  pIcmp->icmp_cksum = 0;  //У���
  pIcmp->icmp_seq   = seq;  //���
  pIcmp->icmp_id    = getpid(); //ȡ���̺���Ϊ��־
  pTime = (struct timeval *)pIcmp->icmp_data;
  gettimeofday(pTime, NULL); //���ݶδ�ŷ���ʱ��
  pIcmp->icmp_cksum = Compute_cksum(pIcmp);
  
  if (1 == seq)
  FirstSendTime = *pTime;
}
 
/*!
 * \fn     SendPacket
 * \brief  �������ݱ�
 *          
 * \param  [in] int sock_icmp                   #
 * \param  [in] struct sockaddr_in *dest_addr   #
 * \param  [in] int nSend                       #
 * 
 * \retval void
 */
void SendPacket(int sock_icmp, struct sockaddr_in *dest_addr, int nSend)
{
  SetICMP(nSend);
  if (sendto(sock_icmp, (char *)&gstvPing.SendBuffer[0], ICMP_LEN, 0,
   (struct sockaddr *)dest_addr, sizeof(struct sockaddr_in)) < 0)
  {
    perror("sendto");
    return;
  }
}
 
/*!
 * \fn     GetRtt
 * \brief  ���RTT
 *          
 * \param  [in] struct timeval *RecvTime   #
 * \param  [in] struct timeval *SendTime   #
 * 
 * \retval double
 */
double GetRtt(struct timeval *RecvTime, struct timeval *SendTime)
{
  struct timeval sub = *RecvTime;
  
  if ((sub.tv_usec -= SendTime->tv_usec) < 0)
  {
    --(sub.tv_sec);
    sub.tv_usec += 1000000;
  }
  sub.tv_sec -= SendTime->tv_sec;
  
  return sub.tv_sec * 1000.0 + sub.tv_usec / 1000.0; //ת����λΪ����
}

 
/*!
 * \fn     unpack
 * \brief  ��������
 *          
 * \param  [in] struct timeval *RecvTime   #
 * 
 * \retval int
 */
int unpack(struct timeval *RecvTime)
{
  struct ip *Ip = (struct ip *)&gstvPing.RecvBuffer;
  struct icmp *Icmp;
  int ipHeadLen;
  double rtt;
  
  ipHeadLen = Ip->ip_hl << 2; //ip_hl�ֶε�λΪ4�ֽ�
  Icmp = (struct icmp *)(gstvPing.RecvBuffer + ipHeadLen);
  
  //�жϽ��յ��ı����Ƿ����Լ��������ĵ���Ӧ
  if ((Icmp->icmp_type == ICMP_ECHOREPLY) && Icmp->icmp_id == getpid())
  {
    struct timeval *SendTime = (struct timeval *)Icmp->icmp_data;
    rtt = GetRtt(RecvTime, SendTime);
  
    printf("%u bytes from %s: icmp_seq=%u ttl=%u time=%.1f ms\n",
    ntohs(Ip->ip_len) - ipHeadLen,
    inet_ntoa(Ip->ip_src),
    Icmp->icmp_seq,
    Ip->ip_ttl,
    rtt);
  
    if (rtt < gstvPing.min || 0 == gstvPing.min)
      gstvPing.min = rtt;
    if (rtt > gstvPing.max)
      gstvPing.max = rtt;
    gstvPing.avg += rtt;
    gstvPing.mdev += rtt * rtt;
   
    return 0;
  }
  
  return -1;
}
 
 
/*!
 * \fn     Statistics
 * \brief  ����
 *          
 * 
 * \retval int
 */
int Statistics(void)
{
  double tmp;
  gstvPing.avg /= gstvPing.nRecv;
  tmp = gstvPing.mdev / gstvPing.nRecv - gstvPing.avg * gstvPing.avg;
  gstvPing.mdev = sqrt(tmp);
  
  printf("%d packets transmitted, %d received, %d%% packet loss, time %dms\n"
    , gstvPing.nSend
    , gstvPing.nRecv
    , (gstvPing.nSend - gstvPing.nRecv) / gstvPing.nSend * 100
    , (int)GetRtt(&LastRecvTime, &FirstSendTime));
  printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
    gstvPing.min, gstvPing.avg, gstvPing.max, gstvPing.mdev);
  
  close(gstvPing.sock_icmp);
  return (gstvPing.nSend - gstvPing.nRecv)==gstvPing.nSend?1:0;
}
 
/*!
 * \fn     RecvePacket
 * \brief  �������ݰ�
 *          
 * \param  [in] int sock_icmp                   #
 * \param  [in] struct sockaddr_in *dest_addr   #
 * 
 * \retval int
 */
int RecvePacket(int sock_icmp, struct sockaddr_in *dest_addr)
{
  int RecvBytes = 0;
  int addrlen = sizeof(struct sockaddr_in);
  struct timeval RecvTime;
  
  if((RecvBytes = recvfrom(sock_icmp, (char *)&gstvPing.RecvBuffer[0], RECV_BUFFER_SIZE, 0, (struct sockaddr *)dest_addr, (socklen_t *)&addrlen)) < 0)
  {
    perror("recvfrom");
    return 0;
  }
 
  gettimeofday(&RecvTime, NULL);
  LastRecvTime = RecvTime;
  
  if (unpack(&RecvTime) == -1)
  {
    return -1;
  }
  gstvPing.nRecv++;
}



/*!
 * \fn     PingDealHanle
 * \brief  Ping������
 *          
 * \param  [in] char *ip   # ����IP��ַ
 * 
 * \retval int
 */
int PingDealHanle(char *ip)
{
  int ret=-1;
  struct protoent *protocol;
  in_addr_t inaddr;  //ip��ַ�������ֽ���
  struct timeval timeout = {3, 0};
  int TimeLen = sizeof(timeout);
  u8    Step;
  int unpack_ret;

  Step = gstvPing.Step;

  switch(Step)
  {
    case 0:
    {
      gstvPing.nSend=0;
      gstvPing.nRecv = 0;
      pHost=NULL;
      gstvPing.min=0.0;
      gstvPing.avg=0.0;
      gstvPing.max=0.0;
      gstvPing.mdev=0.0;
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      ret = 2;
      Step++;
      break;
    }
    case 1:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      printf("PING %s", ip);
      
      if ((protocol = getprotobyname("icmp")) == NULL)
      {
        perror("getprotobyname");
        gstvPing.Step = 88;
        return -1;
      }
      
      /* ����ICMP�׽��� */
      //AF_INET:IPv4, SOCK_RAW:IPЭ�����ݱ��ӿ�, IPPROTO_ICMP:ICMPЭ��
      if ((gstvPing.sock_icmp = socket(PF_INET, SOCK_RAW, protocol->p_proto/*IPPROTO_ICMP*/)) < 0)
      {
        perror("socket");
        gstvPing.Step = 88;
        return -1;
      }
      
      setsockopt(gstvPing.sock_icmp, SOL_SOCKET, SO_RCVTIMEO, &timeout, TimeLen);
      dest_addr.sin_family = AF_INET;
      
      /* �����ʮ����ip��ַת��Ϊ�����ֽ��� */
      if ((inaddr = inet_addr(ip)) == INADDR_NONE)
      {
        /* ת��ʧ�ܣ�������������,��ͨ����������ȡip */
        if ((pHost = gethostbyname(ip)) == NULL)
        {
          herror("gethostbyname()");
          gstvPing.Step = 88;
          return -1;
        }
        memmove(&dest_addr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
      }
      else
      {
        memmove(&dest_addr.sin_addr, &inaddr, sizeof(struct in_addr));
      }
      
      if (NULL != pHost)
      {
        printf("PING %s", pHost->h_name);
      }
      else
      {
        printf("PING %s", ip);
        printf("(%s) %d bytes of data.\n", inet_ntoa(dest_addr.sin_addr), ICMP_LEN);
      }
      
      Step++;
      ret = 2;
      break;
    }
    case 2:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      SendPacket(gstvPing.sock_icmp, &dest_addr, gstvPing.nSend);
      unpack_ret = RecvePacket(gstvPing.sock_icmp, &dest_addr);
      if (-1 == unpack_ret) //��ping�ػ�ʱ���յ����Լ������ı���,���µȴ�����
        RecvePacket(gstvPing.sock_icmp, &dest_addr);
            
      Step++;
      gstvPing.Delay = TimerGet1msInc();

      ret = 2;
      
      break; 
    }
    case 3:
    {      
      if(TimerGet1msSubb(gstvPing.Delay, 1000))
      {
        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
        gstvPing.nSend++;
        if(gstvPing.nSend < SEND_NUM)
        {
          Step = 2;
        }
        else
        {
          Step++;
        }
      }

      ret = 2;
      break;
    }
    case 4:
    {
      ret=Statistics(); //�����Ϣ���ر��׽���
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      break;
    }
    default:
    {
      PingDealParaInit();
      ret = -1;
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      break;
    }
  }
  
  gstvPing.Step = Step;

  return ret;
}
 
/*!
 * \fn     PingDealParaInit
 * \brief  ������ʼ��
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void PingDealParaInit(void)
{
  memset(&gstvPing, 0, sizeof(PingDealStru));
  gstvPing.Step = 0;
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

 
 

