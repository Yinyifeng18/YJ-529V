/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      main.h
 * \author    yifeng
 * \version   1.0
 * \date      2023��3��11��
 * \brief     ������ͷ�ļ�
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


/*=======================================================================================
��������ָ���Ǳ�ͷ�ļ����������Ԥ����ָ����ڷ�ֹ��ͷ�ļ��������ļ��ظ����á�
=======================================================================================*/
#ifndef __Main_H__
#define __Main_H__

/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <libgen.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <net/route.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <math.h>
#include <netinet/ip_icmp.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <ctype.h>
#include <sys/un.h>
#include <linux/types.h>
#include <linux/netlink.h>



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
/* Exported types ------------------------------------------------------------*/
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;
typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */
typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;
typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;
typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

#define TRUE                  1               // ���塰TRUE�� �ַ�ֵΪ 1
#define FALSE                 0               // ���塰FALSE���ַ�ֵΪ 0

#define SET                   1               // ���塰SET�� �ַ�ֵΪ 1
#define CLEAR                 0               // ���塰CLEAR���ַ�ֵΪ 0

typedef char                  char8;          // char8   �����з����ַ�����������
typedef unsigned char         Uchar8;         // Uchar8  �����޷����ַ�����������
typedef short int             int16;          // int16   �����з�����������������
typedef unsigned short int    Uint16;         // Uint16  �����޷�����������������
typedef long int              int32;          // int32   �����з��ų���������������
typedef unsigned long int     Uint32;         // Uint32  �����޷��ų���������������


typedef union
{
    Uint16 word;
    Uchar8 bytes[2];
    struct
    {
      Uchar8 low;   // ���ֽ�
      Uchar8 high;  // ���ֽ�
      // 0x0001    low = 0x01,high = 0x00;
    } byte;

    struct
    {
        unsigned char bit0:1;
        unsigned char bit1:1;
        unsigned char bit2:1;
        unsigned char bit3:1;
        unsigned char bit4:1;
        unsigned char bit5:1;
        unsigned char bit6:1;
        unsigned char bit7:1;
        unsigned char bit8:1;
        unsigned char bit9:1;
        unsigned char bit10:1;
        unsigned char bit11:1;
        unsigned char bit12:1;
        unsigned char bit13:1;
        unsigned char bit14:1;
        unsigned char bit15:1;
    } bits;
} WORD_BYTES;



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
  unsigned char quit;        // �˳����
     
  unsigned char Alarming;    // ���ڱ���      û�б���

  unsigned char P2pRun;      // ���б��
  unsigned char StartP2p;
}SysRunStru;

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
extern SysRunStru gstvSysRun;

/*=======================================================================================
                                 Դ�����ļ��еĺ���ԭ������
=======================================================================================*/












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

