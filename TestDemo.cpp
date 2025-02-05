/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      TestDemo.cpp
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月12日
 * \brief     单元测试 demo
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年7月12日
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
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


/*=======================================================================================
                                         全局变量定义                                    
=======================================================================================*/
/*---------------------------------------------------------------------------------------
的全局变量定义：
---------------------------------------------------------------------------------------*/


/*=======================================================================================
                             本源程序文件内部使用的局部变量定义                          
=======================================================================================*/




/*
*****************************************************************************************
                            　       程序指令代码清单
*****************************************************************************************
*/



/*!
 * \fn     get_pid_by_name
 * \brief  获取PID
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
 * \fn     main
 * \brief  主程序
 *          
 * \param  [in] int argc       #
 * \param  [in] char *argv[]   #
 * 
 * \retval int
 */
int main(int argc, char *argv[])
{
  int pid = -1;
  char cmd[20] = {0};

  sprintf(cmd, "./JY-529V &");
  system(cmd);    

  sleep(10);
  
  pid = get_pid_by_name("JY-529V");
  if(pid != -1)
  {
    sprintf(cmd, "kill -9 %d", pid);
    printf("system(%s)\n", cmd);       // kill -9 pid
    system(cmd);
  }
  else 
  {
    printf("Get Pid Failed!\n");
  }
  return 0;
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
