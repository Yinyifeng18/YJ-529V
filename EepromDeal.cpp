/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      EepromDeal.c
 * \author    yifeng
 * \version   1.0
 * \date      2023年7月1日
 * \brief     Eeprom操作处理源程序
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023年7月1日
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 本源程序包括的头文件                                     
建议：包含本项目的文件使用 #include "文件名.扩展名"，                                   
      包含系统库的文件使用 #include <文件名.扩展名> 。                                    
========================================================================================*/
#include "EepromDeal.h"

#include "iniparser.h"
#include "CommFun.h"


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
 * \fn     EepromDeal_Init
 * \brief  初始化
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void EepromDeal_Init(void)
{
  
}


/*!
 * \fn     EeResetEepromData
 * \brief  恢复参数
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void EeResetEepromData(void)
{

}


/*!
 * \fn     EeReadEepromToSram
 * \brief  从EEPROM的数据读到SRARM操作
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
void EeReadEepromToSram(void)
{
  
}



/*!
 * \fn     EeWriteDeviceParamToIni
 * \brief  写设备信息到文件
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
int EeWriteDeviceParamToIni(unsigned char *DeviceID)
{
  FILE       *fp  = NULL;
  dictionary *ini = NULL;
  unsigned char i = 0;
  unsigned char buf[12] = {0};
  unsigned char AscBuf[16] = {0};
  unsigned char temp;
  

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  //加载文件
  ini = iniparser_load(cEeDeviceCfgIniPath);
  if(ini == NULL)
  {
    printf("inipar_load  failure\n");
    return 1;
  } 

  printf("DeviceID: ");
  for(int j = 0; j < 6; j++)
  {
    printf("0x%x ", DeviceID[j]);
  }
  printf("\r\n");

  /* 把一个字节拆成2个字节 */
  DecompressByte1To2((unsigned char *)&DeviceID[0], (unsigned char *)&buf[0], 6);
  printf("DecompressByte1To2: ");
  for(int j = 0; j < 12; j++)
  {
    printf("0x%x ", buf[j]);
  }
  printf("\r\n");
  
  /* 转换成ASCII编码 */
  for(i=0; i<12; i++)
  {
    temp    =  buf[i];
    if (temp > 9)
    {
      temp   +=  'A' - 10;
    }
    else
    {
      temp   +=  '0';
    }
    AscBuf[i]   =  temp;
  }
  
  printf("AtHexConvAsc: ");
  for(int j = 0; j < 12; j++)
  {
    printf("0x%x ", AscBuf[j]);
  }
  printf("\r\n");  
  
  // 设置DeviceId:EuqiID
  iniparser_set(ini, "DeviceId:EuqiID", (char *)&AscBuf[0]);

  printf("%s\n", iniparser_getstring(ini, "DeviceId:EuqiID", "null"));

  //写入文件
  fp = fopen(cEeDeviceCfgIniPath, "w");
  if( fp == NULL ) {
      printf("stone:fopen error!\n");
      return 1;
  }
  
  iniparser_dump_ini(ini, fp);

  fclose(fp);
  
  //释放
  iniparser_freedict(ini);
  
  return  (0);
}



/*!
 * \fn     EeReadDeviceParamToSram
 * \brief  读取设备信息到内存
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
int EeReadDeviceParamToSram(unsigned char *EuqiID)
{
  const char *EuqiID_src;
  unsigned char temp;
  unsigned char HexBuf[12] = {0};
  dictionary  *ini;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  ini = iniparser_load(cEeDeviceCfgIniPath);
  if (ini==NULL)
  {
    fprintf(stderr, "cannot parse file: %s\n", cEeDeviceCfgIniPath);
    return -1;
  }

  // 获取数据
  EuqiID_src = iniparser_getstring(ini, "DeviceId:EuqiID", "null");
  printf("apartment:username: %s\n", EuqiID_src);
  
  for(int i=0; i < 12; i++)
  {
    temp    =  EuqiID_src[i];
    if (temp >= 'a')
    {
      temp   -=  'a'- 10;
    }
    else if (temp >= 'A')
    {
      temp   -=  'A'- 10;
    }
    else
    {
      temp   -=  '0';
    }
    HexBuf[i]   =  temp;
  }
 
  CompressByte2To1((unsigned char *)&HexBuf[0], (unsigned char *)&EuqiID[0], 12);
  printf("EuqiID: ");
  for(int j = 0; j < 6; j++)
  {
    printf("0x%x ", EuqiID[j]);
  }
  printf("\r\n");  
  
  iniparser_freedict(ini);

  return 0;
}



/*!
 * \fn     EeWriteWebrtcIdParamToIni
 * \brief  写Webrtc Id到文件
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
int EeWriteWebrtcIdParamToIni(unsigned char *WebrtcID)
{
  FILE       *fp  = NULL;
  dictionary *ini = NULL;  

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  //加载文件
  ini = iniparser_load(cEeDeviceWebrtcCfgIniPath);
  if(ini == NULL)
  {
    printf("inipar_load  failure\n");
    return 1;
  } 

  // 设置DeviceId:EuqiID
  iniparser_set(ini, "DeviceId:WebrtcID", (char *)&WebrtcID[0]);
  
  printf("%s\n", iniparser_getstring(ini, "DeviceId:WebrtcID", "null"));

  //写入文件
  fp = fopen(cEeDeviceWebrtcCfgIniPath, "w");
  if( fp == NULL ) {
      printf("stone:fopen error!\n");
      return 1;
  }
  
  iniparser_dump_ini(ini, fp);
  
  fclose(fp);
  
  //释放
  iniparser_freedict(ini);
  
  return  (0);
}


/*!
 * \fn     EeReadWebrtcIDParamToSram
 * \brief  读取设备信息到内存
 *          
 * \param  [in] void   #
 * 
 * \retval void
 */
int EeReadWebrtcIDParamToSram(unsigned char *WebrtcID)
{
  const char *EuqiID_src;
  dictionary  *ini;

  printf("file: %s, line: %d, function: %s !\n", __FILE__, __LINE__, __FUNCTION__);
  
  ini = iniparser_load(cEeDeviceWebrtcCfgIniPath);
  if (ini==NULL)
  {
    fprintf(stderr, "cannot parse file: %s\n", cEeDeviceWebrtcCfgIniPath);
    return -1;
  }

  // 获取数据
  EuqiID_src = iniparser_getstring(ini, "DeviceId:WebrtcID", "null");
  printf("apartment:username: %s\n", EuqiID_src);
  
  MemCopySramToSram((unsigned char *)&EuqiID_src[0], (unsigned char *)&WebrtcID[0], sizeof(EuqiID_src));
  
  iniparser_freedict(ini);

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
