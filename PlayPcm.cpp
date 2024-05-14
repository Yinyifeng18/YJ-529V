/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      PlayPcm.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023��7��10��
 * \brief     ����PCMԴ����
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��7��10��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "PlayPcm.h"

#include "TimerDeal.h"
#include "WiredNetDeal.h"

#include <imp/imp_audio.h>
#include <imp/imp_log.h>



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
static void *PlayPcmDeal_Thread(void *arg);

static void PlayPcmDealHandle(void);

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
PlayPcmBits  gbitPlay;

PlayPcmStru  gstvPlay;

unsigned char *IMPAudioBuf;

FILE *play_file;


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/



/*!
 * \fn     Start_PlayPcmDeal_Monitor_Thread
 * \brief  PCM����
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void Start_PlayPcmDeal_Monitor_Thread(void)
{
  pthread_t ntp_tidp;
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  
  if(pthread_create(&ntp_tidp, NULL, PlayPcmDeal_Thread, NULL) != 0)
  {
    perror("pthread_create: KeyDeal_Thread");
  }
}



/*!
 * \fn     PlayPcmDeal_Thread
 * \brief  PCM����
 *          
 * \param  [in] void *arg   #
 * 
 * \retval static void *
 */
static void *PlayPcmDeal_Thread(void *arg)
{
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);

  pthread_detach(pthread_self());

  while(!gstvSysRun.quit)
  {
    PlayPcmDealHandle();

    if(gstvSysRun.P2pRun == 0x01)
    {
      if(TimerGet1sSubb(gstvPlay.P2pDelay, 180))
      {
        gstvPlay.P2pDelay = TimerGet1sInc();

        StopP2pTaskThread();
      }
    }
    
    usleep(10*1000);
  }
  
  pthread_exit(0);
}


/*!
 * \fn     PlayPcmDealHandle
 * \brief  PCM����
 *          
 * \param  [in] void   #
 * 
 * \retval static void
 */
static void PlayPcmDealHandle(void)
{
  PlayPcmStru *pPlay;  
  unsigned char Flg = 0;
  int ret = 0;
  int size = 0;
  int devID = 0;
  int chnID = 0;
  
  pPlay = &gstvPlay;
 
  

  switch(pPlay->RunMode)
  {
    case EnPmOpenFile:
    {
      play_file = fopen(IMP_AUDIO_PLAY_FILE, "rb");
      if(play_file == NULL)
      {
        printf("[ERROR] %s: fopen %s failed\n", __func__, IMP_AUDIO_PLAY_FILE);
        
        fclose(play_file);
        
        PlayPcmInputRunMode(EnPmDefault);
      }
      else
      {
        PlayPcmInputRunMode(EnPmStart);
      }
      
      break;
    }
    case EnPmStart:  // ��ʼ��
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      gbitPlay.IMPAudioInit = 0x00;

      gstvPlay.Delay = TimerGet1sInc();
      
      IMPAudioBuf = (unsigned char *)malloc(IMP_AUDIO_BUF_SIZE);
      if(IMPAudioBuf == NULL)
      {
        printf("[ERROR] %s: malloc audio buf error\n", __func__);
      }
      else
      {      
        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
        /* Step.1 set public attribute of Ao device. */
        IMPAudioIOAttr  attr;
        attr.samplerate = AUDIO_SAMPLE_RATE_16000;
        attr.bitwidth   = AUDIO_BIT_WIDTH_16;
        attr.soundmode  = AUDIO_SOUND_MODE_MONO;
        attr.frmNum     = 20;
        attr.numPerFrm  = 640;
        attr.chnCnt     = 1;
        ret = IMP_AO_SetPubAttr(devID, &attr);
        if(ret != 0)
        {
          printf("set ao %d attr err: %d\n", devID, ret);
          Flg = 1;
        }
        
        memset(&attr, 0x0, sizeof(attr));
        ret = IMP_AO_GetPubAttr(devID, &attr);
        if(ret != 0)
        {
          printf("get ao %d attr err: %d\n", devID, ret);
          Flg = 1;
        }
        
        printf("Audio Out GetPubAttr samplerate:%d\n", attr.samplerate);
        printf("Audio Out GetPubAttr   bitwidth:%d\n", attr.bitwidth);
        printf("Audio Out GetPubAttr  soundmode:%d\n", attr.soundmode);
        printf("Audio Out GetPubAttr     frmNum:%d\n", attr.frmNum);
        printf("Audio Out GetPubAttr  numPerFrm:%d\n", attr.numPerFrm);
        printf("Audio Out GetPubAttr     chnCnt:%d\n", attr.chnCnt);
        
        /* Step.2 enable Ao device */
        ret = IMP_AO_Enable(devID);
        if(ret != 0)
        {
          printf("enable ao %d err\n", devID);
          Flg = 1;
        }
        
        /* Step.3 enable AI channel. */
        ret = IMP_AO_EnableChn(devID, chnID);
        if(ret != 0)
        {
          printf("Audio Play enable channel failed!\n");
          Flg = 1;
        }
        
        /* Step.4 Set audio channel volume. */
        int chnVol = 60;
        ret = IMP_AO_SetVol(devID, chnID, chnVol);
        if(ret != 0)
        {
          printf("Audio Play set volume failed\n");
          Flg = 1;
        }
        
        ret = IMP_AO_GetVol(devID, chnID, &chnVol);
        if(ret != 0)
        {
          printf("Audio Play get volume failed!\n");
          Flg = 1;
        }
        
        printf("Audio Out GetVol   vol:%d\n", chnVol);
        int aogain = 28;
        ret = IMP_AO_SetGain(devID, chnID, aogain);
        if(ret != 0)
        {
          printf("Audio Record Set Gain failed!\n");
          Flg = 1;
        }
        
        ret = IMP_AO_GetGain(devID, chnID, &aogain);
        if(ret != 0)
        {
          printf("Audio Record Get Gain failed!\n");
          Flg = 1;
        }
        printf("Audio Out GetGain    gain: %d\n", aogain);

        if(Flg == 1)  // ����
        {
          gbitPlay.IMPAudioInit = 0x00;
          
          PlayPcmInputRunMode(EnPmStop);
        }
        else   // ����
        {
          gbitPlay.IMPAudioInit = 0x01;

          PlayPcmInputRunMode(EnPmPlay);
        }
      }
      
      break;
    }
    case EnPmPlay:
    {
      size = fread(IMPAudioBuf, 1, IMP_AUDIO_BUF_SIZE, play_file);
      if(size < IMP_AUDIO_BUF_SIZE)
      {
        fseek(play_file, 0, SEEK_SET);    // ָ��ͷ��
        
        sleep(1);
      }
      else
      {
        /* Step.5 send frame data. */
        IMPAudioFrame frm;
        frm.virAddr = (uint32_t *)IMPAudioBuf;
        frm.len = size;
        ret = IMP_AO_SendFrame(devID, chnID, &frm, BLOCK);
        if(ret != 0)
        {
          printf("Send frame data error!\n");
          PlayPcmInputRunMode(EnPmStop);
        }
    
        IMPAudioOChnState play_status;
        ret = IMP_AO_QueryChnStat(devID, chnID, &play_status);
        if(ret != 0)
        {
          printf("IMP_AO_QueryChnStat error!\n");
          PlayPcmInputRunMode(EnPmStop);
        }
    
        printf("Play: TotalNum %d, FreeNum %d, BusyNum %d\n",
               play_status.chnTotalNum, play_status.chnFreeNum, play_status.chnBusyNum);
      }

      // ��ʱ�˳�
      if(TimerGet1sSubb(gstvPlay.Delay, 30))
      {
        PlayPcmInputRunMode(EnPmStop);
      }
      
      // ������ֱ�ӽ��룬���н���ǰ��Ҫ�ͷ���ز���
      if(gstvSysRun.StartP2p == 0x01)
      {
        PlayPcmInputRunMode(EnPmStop);
      }
      
      break;
    }
    case EnPmStop:
    {
      printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      
      fclose(play_file);
      free(IMPAudioBuf);
    
      if(gbitPlay.IMPAudioInit == 0x01)
      {
        /* Step.6 disable the audio channel. */
        ret = IMP_AO_DisableChn(devID, chnID);
        if(ret != 0)
        {
          printf("Audio channel disable error!\n");
        }
      
        /* Step.7 disable the audio devices. */
        ret = IMP_AO_Disable(devID);
        if(ret != 0)
        {
          printf("Audio device disable error\n");
        } 

        gbitPlay.IMPAudioInit = 0x00;
      }

      PlayPcmInputRunMode(EnPmP2p);
      gstvPlay.Delay = TimerGet1sInc();

      break;
    }
    case EnPmP2p:
    {
      if(TimerGet1sSubb(gstvPlay.Delay, 30))  // ��ʱ30��
      {
        PlayPcmInputRunMode(EnPmDefault);
        printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
      }
      else 
      {
        StartP2pTaskThread();
      }
      break;
    }
    case EnPmDefault:
    {
      gbitPlay.StartStop = CLEAR;
      gbitPlay.PlayNow = CLEAR;
      
      break;
    }
    default:
    {
      gbitPlay.StartStop = CLEAR;
      gbitPlay.PlayNow = CLEAR;
      
      break;
    }
  }
}


/*!
 * \fn     PlayPcmInputRunMode
 * \brief  ��������ģʽ
 *          
 * \param  [in] PlayModeEnum  PlayMode   #
 * 
 * \retval void
 */
void PlayPcmInputRunMode(PlayModeEnum PlayMode)
{
  gstvPlay.RunMode = PlayMode;

  if(PlayMode == EnPmStart)
  {
    gbitPlay.StartStop = SET;
    gbitPlay.PlayNow = CLEAR;
  }
  else if(PlayMode == EnPmPlay)
  {
    gbitPlay.StartStop = SET;
    gbitPlay.PlayNow = SET;
  }
  else if(PlayMode == EnPmStop)
  {
    gbitPlay.StartStop = CLEAR;
    gbitPlay.PlayNow = CLEAR;
  }
  else 
  {
  }
}




/*!
 * \fn     StartP2pTaskThread
 * \brief  ��ʼ����
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void StartP2pTaskThread(void)
{
  char cmd[32] = {0};
  
  printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
  if((gstvSysRun.StartP2p == 0x01)
  &&(gbitWired.ConnectBoot == SET))
  {
    printf("%s:%d==> [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    gstvSysRun.P2pRun = 0x01;
    gstvSysRun.StartP2p = 0x00;

    /* ����Ĭ�� */
    PlayPcmInputRunMode(EnPmDefault);
  
    /* ��ʼ���� */
    gstvPlay.P2pDelay = TimerGet1sInc();
  
    printf("===>>Start P2P Task\n");
    printf("system(./P2P_FILE)\n");
    sprintf(cmd, "./p2p_webrtc &");
    system(cmd);    
  }
}


/*!
 * \fn     get_pid_by_name
 * \brief  ��ȡPID
 *          
 * \param  [in] const char* task_name   #
 * 
 * \retval static int
 */
static int get_pid_by_name(const char* task_name)
{
  int pid_value = -1;
  DIR *dir;
  struct dirent *ptr;
  FILE *fp;
  char filepath[512] = { 0 };
  char cur_task_name[512] = { 0 };
  char buf[1024] = { 0 };
  
  dir = opendir("/proc");
  
  if (NULL != dir) 
  {
    while((ptr = readdir(dir)) != NULL) 
    {
      if((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))            
      continue;
      
      if(DT_DIR != ptr->d_type)
      continue;
      
      sprintf(filepath, "/proc/%s/status", ptr->d_name);
      
      fp = fopen(filepath, "r");
      if(NULL != fp)
      {
        if(fgets(buf, 1023, fp) == NULL)
        {
          fclose(fp);
          continue;
        }
      
        sscanf(buf, "%*s %s", cur_task_name);
        if (!strcmp(task_name, cur_task_name))
        {
          printf("PID:%s\n", ptr->d_name);
          pid_value = atoi(ptr->d_name);
        }
      
        fclose(fp);
      }
    }
    closedir(dir);
  } 
  else 
  {
    printf("open proc failed!\n");
  }
  
  return pid_value;
}


/*!
 * \fn     StopP2pTaskThread
 * \brief  ֹͣ����
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void StopP2pTaskThread(void)
{ 
  int pid = -1;
  char cmd[32];
  
  if(gstvSysRun.P2pRun == 0x01)
  {
    gstvSysRun.P2pRun = 0x00;
    gstvPlay.P2pDelay = TimerGet1sInc();
  
    printf("===>>Delete Task\n");
    pid = get_pid_by_name(P2P_FILE);
    if(pid != -1)
    {
      sprintf(cmd, "kill -9 %d", pid);
      printf("system(%s)\n", cmd);  // kill -9 pid
      system(cmd);
    }
    else 
    {
      printf("Get Pid Failed!\n");
    }
  }
  else
  {
    gstvSysRun.P2pRun = 0x00;
    gstvPlay.P2pDelay = TimerGet1sInc();
  }
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
