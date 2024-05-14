/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      NtpDeal.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023��3��11��
 * \brief     NTP����Դ����
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��3��11��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "NtpDeal.h"
#include "TimerDeal.h"
#include "EepromDeal.h"
#include "WiredNetDeal.h"

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
static void *NtpDeal_Thread(void *arg);
static int send_packet(int fd);
static int get_server_time(int sock, struct timeval *newtime);
static int mod_localtime(struct timeval newtime);

static void NtpDealRunTask(void);

static void NtpDealInputRunState(NtpStateEnum  NtpState);

/*=======================================================================================
                       ��Դ�����ļ��ڲ�ʹ�õľֲ��������ַ�����������                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
�ľֲ��������ַ����������壺
---------------------------------------------------------------------------------------*/
int addr_len = sizeof(struct sockaddr_in);
struct sockaddr_in addr_src; //���� socket  <netinet/in.h>
struct sockaddr_in addr_dst; //������ socket



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
NtpDealStru   gstvNtp;
NtpDealBits   gbitNtp;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/


/*!
 * \fn     Start_NtpDeal_Monitor_Thread
 * \brief  ����ʱ���ȡ
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_NtpDeal_Monitor_Thread(void)
{
  pthread_t ntp_tidp;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pthread_create(&ntp_tidp, NULL, NtpDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: NtpDeal_Thread");
  }
}

/*!
 * \fn     NtpDealRestartHandle
 * \brief  ����Ntp
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void NtpDealRestartHandle(void)
{
  /* �ȹر�sock */
  if(gbitNtp.HaveSocket == SET)
  {
    gbitNtp.HaveSocket = CLEAR;
    close(gstvNtp.sock);
  }
  
  memset(&gstvNtp, 0, sizeof(NtpDealStru));
  memset(&gbitNtp, 0, sizeof(NtpDealBits));

  NtpDealInputRunState(EnNrInit);
}


/*!
 * \fn     NtpDeal_Thread
 * \brief  ��ʱ��ȡ����ʱ��
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *NtpDeal_Thread(void *arg)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());

  memset(&gstvNtp, 0, sizeof(NtpDealStru));
  memset(&gbitNtp, 0, sizeof(NtpDealBits));

  NtpDealRestartHandle();

  while(!gstvSysRun.quit)
  {   
    /* �Զ���ȡʱ�� */    
    NtpDealRunTask();

    /* ÿСʱ��ȡһ��ʱ�� */
    if(TimerGet1minSubb(gstvNtp.GetTimeDelay, 60))
    {
      gstvNtp.GetTimeDelay = TimerGet1minInc();
      NtpDealRestartHandle();
    }
    
    sleep(5);
  }

  pthread_exit(0);
}

/*!
 * \fn     send_packet
 * \brief  �������ݰ�
 *          
 * \param  [in] int fd   #
 * 
 * \retval static int
 */
static int send_packet(int fd)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  unsigned int data[12];
  int ret;
  struct timeval now;
  if (sizeof(data) != 48)
  {
    PDEBUG("data ����С��48!");
    exit(1);
  }
  memset((char *)data, 0, sizeof(data));
  data[0] = htonl((LI << 30) | (VN << 27) | (MODE << 24) | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
  data[1] = htonl(1 << 16);
  data[2] = htonl(1 << 16);
  //��ñ���ʱ��
  gettimeofday(&now, NULL);

  data[10] = htonl(now.tv_sec + JAN_1970);
  data[11] = htonl(NTPFRAC(now.tv_usec));

  ret = send(fd, data, 48, 0);
  PDEBUG("�����������ݰ���������, ret: %d\n", ret);
}

/*!
 * \fn     get_server_time
 * \brief  ��ȡ������ʱ��
 *          
 * \param  [in] int sock                  #
 * \param  [in] struct timeval *newtime   #
 * 
 * \retval static int
 */
static int get_server_time(int sock, struct timeval *newtime)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  int ret;
  unsigned int data[12];
  NtpTime oritime, rectime, tratime, destime;
  struct timeval offtime, dlytime, new_1;
  struct timeval now;
  
  bzero(data, sizeof(data));
  ret = recvfrom(sock, data, sizeof(data), 0, NULL, 0);
  if (ret == -1)
  {
    PDEBUG("��ȡ��������ʧ��\n");
    exit(1);
  }
  else if (ret == 0)
  {
    PDEBUG("��ȡ���ٶȳ���: 0!\n");
    return 1;
  }

  //1970��ת����1900
  gettimeofday(&now, NULL);
  destime.integer = now.tv_sec + JAN_1970;
  destime.fraction = NTPFRAC(now.tv_usec);

  //�ֽ���ת��
  oritime.integer  = DATA(6);
  oritime.fraction = DATA(7);
  rectime.integer  = DATA(8);
  rectime.fraction = DATA(9);
  tratime.integer  = DATA(10);
  tratime.fraction = DATA(11);

  long long orius, recus, traus, desus, offus, dlyus;

  orius = TTLUSEC(MKSEC(oritime), MKUSEC(oritime));
  recus = TTLUSEC(MKSEC(rectime), MKUSEC(rectime));
  traus = TTLUSEC(MKSEC(tratime), MKUSEC(tratime));
  desus = TTLUSEC(now.tv_sec, now.tv_usec);

  offus = ((recus - orius) + (traus - desus)) / 2;
  dlyus = (recus - orius) + (desus - traus);

  offtime.tv_sec = GETSEC(offus);
  offtime.tv_usec = GETUSEC(offus);
  dlytime.tv_sec = GETSEC(dlyus);
  dlytime.tv_usec = GETUSEC(dlyus);

  new_1.tv_sec = destime.integer - JAN_1970 + offtime.tv_sec;
  new_1.tv_usec = USEC(destime.fraction) + offtime.tv_usec;

  //��ȡ���ںõ�ʱ��
  *newtime = new_1;
  return 0;
}


/*!
 * \fn     mod_localtime
 * \brief  ���±���ʱ��
 *          
 * \param  [in] struct timeval newtime   #
 * 
 * \retval int
 */
static int mod_localtime(struct timeval newtime)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  struct tm *tm_p;
  char time_buff[100];
  memset(time_buff, 0, 100);
  time_t time_sec = newtime.tv_sec + 28800;
  tm_p = gmtime(&time_sec);

  sprintf(time_buff, "date -s \"%d-%d-%d %d:%d:%d\"", tm_p->tm_year + 1900, tm_p->tm_mon + 1, tm_p->tm_mday, tm_p->tm_hour, tm_p->tm_min, tm_p->tm_sec);
  printf("mod_localtime: %s\n", time_buff);
  //system(time_buff);

  return 0;
}


/*!
 * \fn     NtpDealRunTask
 * \brief  Ntp��������
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void NtpDealRunTask(void)
{
  struct timeval newtime;
  struct timeval timeout; //<sys/time.h>
  fd_set fds_read;
  int ret;

  if(gbitWired.WiredOk == 0x00)
    return ;

  switch(gstvNtp.State)
  {
    case EnNrInit:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      memset(&addr_dst, 0, addr_len);
      addr_dst.sin_family = AF_INET;
      addr_dst.sin_port = htons(DEF_NTP_PORT);
      addr_dst.sin_addr.s_addr = inet_addr(DEF_NTP_SERVER_IP);

      NtpDealInputRunState(EnNrConnect);
      
      //UDP���ݱ��׽���
      gstvNtp.sock = socket(PF_INET, SOCK_DGRAM, 0);
      if (gstvNtp.sock == -1)
      {
        PDEBUG("�׽��ִ���ʧ�ܣ�������ֹ ! \n");
        NtpDealInputRunState(EnNrFail);
      }
      else
      {
        gbitNtp.HaveSocket = 0x01;
      }
      
      memset(&addr_src, 0, addr_len);
      addr_src.sin_family = AF_INET;
      addr_src.sin_port = htons(0);
      addr_src.sin_addr.s_addr = htonl(INADDR_ANY); //<arpa/inet.h>
      //�󶨱��ص�ַ
      if (-1 == bind(gstvNtp.sock, (struct sockaddr *)&addr_src, addr_len))
      {
        PDEBUG("��ʧ�ܣ�������ֹ !\n");
        NtpDealInputRunState(EnNrFail);
      }

      break;
    }
    case EnNrConnect:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      if (-1 == connect(gstvNtp.sock, (struct sockaddr *)&addr_dst, addr_len))
      {
        PDEBUG("���ӷ�����ʧ�ܣ�������ֹ !\n");
        NtpDealInputRunState(EnNrFail);
      }
      else
      {
        send_packet(gstvNtp.sock);
        NtpDealInputRunState(EnNrRun);
        gbitNtp.RefreshNtp = SET;
      }
      break;
    }
    case EnNrRun:
    {
      if(gbitNtp.RefreshNtp == SET)
      {
        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
        
        gbitNtp.RefreshNtp = CLEAR;        
        gstvNtp.Delay = TimerGet1sInc();
        
        FD_ZERO(&fds_read);
        FD_SET(gstvNtp.sock, &fds_read);
  
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        
        //���� ntp ��
        send_packet(gstvNtp.sock);
  
        ret = select(gstvNtp.sock + 1, &fds_read, NULL, NULL, &timeout);
        if (ret == -1)
        {
          PDEBUG("select��������������ֹ !\n");
          NtpDealInputRunState(EnNrFail);
        }
        if (ret == 0)
        {
          PDEBUG("�ȴ���������Ӧ��ʱ���ط����� !\n");
          //���������������
          send_packet(gstvNtp.sock);

          gstvNtp.ErrorCnt++;
          if(gstvNtp.ErrorCnt > 2)
          {
            gstvNtp.ErrorCnt = 0;
            NtpDealRestartHandle();
          }
        }
        else
        {
          if (FD_ISSET(gstvNtp.sock, &fds_read))
          {
            if (1 == get_server_time(gstvNtp.sock, &newtime))
            {
             
            }
            else
            {
              mod_localtime(newtime);
            }
          }
        }
      }
      break;
    }
    case EnNrFail:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      if(gbitNtp.HaveSocket == SET)
      {
        gbitNtp.HaveSocket = CLEAR;
        close(gstvNtp.sock);
      }

      NtpDealInputRunState(EnNrFailDelay);
      break;
    }
    case EnNrFailDelay:
    {
      if(TimerGet1sSubb(gstvNtp.Delay, 600*30))  // ��Сʱ
      {
        NtpDealInputRunState(EnNrInit);
      }

      break;
    }
    default:
    {
      NtpDealInputRunState(EnNrInit);
      break;
    }
  }
}

/*!
 * \fn     NtpDealInputRunState
 * \brief  �M�����״̬
 *          
 * \param  [in] NtpStateEnum  NtpState   #
 * 
 * \retval static void
 */
static void NtpDealInputRunState(NtpStateEnum  NtpState)
{
  gstvNtp.State = NtpState;

  gstvNtp.Delay = TimerGet1sInc();
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
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
