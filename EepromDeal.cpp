/*!
 ***************************************************************************************
 *
 *  Copyright ? 2017-2018 yifneg. All Rights Reserved.
 *
 * \file      EepromDeal.c
 * \author    yifeng
 * \version   1.0
 * \date      2023��7��1��
 * \brief     Eeprom��������Դ����
 *
 *--------------------------------------------------------------------------------------
 * \attention
 *
 *
 ***************************************************************************************
 */

/*****************************************************************************************
 change history: 
    1.date  : 2023��7��1��
      author: yifeng
      change: create file

*****************************************************************************************/


/*========================================================================================
                                 ��Դ���������ͷ�ļ�                                     
���飺��������Ŀ���ļ�ʹ�� #include "�ļ���.��չ��"��                                   
      ����ϵͳ����ļ�ʹ�� #include <�ļ���.��չ��> ��                                    
========================================================================================*/
#include "EepromDeal.h"

#include "iniparser.h"
#include "CommFun.h"


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


/*=======================================================================================
                             ��Դ�����ļ��ڲ�ʹ�õľֲ���������                          
=======================================================================================*/




/*
*****************************************************************************************
                            ��       ����ָ������嵥
*****************************************************************************************
*/




/*!
 * \fn     EepromDeal_Init
 * \brief  ��ʼ��
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
 * \brief  �ָ�����
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
 * \brief  ��EEPROM�����ݶ���SRARM����
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
 * \brief  д�豸��Ϣ���ļ�
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
  
  //�����ļ�
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

  /* ��һ���ֽڲ��2���ֽ� */
  DecompressByte1To2((unsigned char *)&DeviceID[0], (unsigned char *)&buf[0], 6);
  printf("DecompressByte1To2: ");
  for(int j = 0; j < 12; j++)
  {
    printf("0x%x ", buf[j]);
  }
  printf("\r\n");
  
  /* ת����ASCII���� */
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
  
  // ����DeviceId:EuqiID
  iniparser_set(ini, "DeviceId:EuqiID", (char *)&AscBuf[0]);

  printf("%s\n", iniparser_getstring(ini, "DeviceId:EuqiID", "null"));

  //д���ļ�
  fp = fopen(cEeDeviceCfgIniPath, "w");
  if( fp == NULL ) {
      printf("stone:fopen error!\n");
      return 1;
  }
  
  iniparser_dump_ini(ini, fp);

  fclose(fp);
  
  //�ͷ�
  iniparser_freedict(ini);
  
  return  (0);
}



/*!
 * \fn     EeReadDeviceParamToSram
 * \brief  ��ȡ�豸��Ϣ���ڴ�
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

  // ��ȡ����
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
 * \brief  дWebrtc Id���ļ�
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
  
  //�����ļ�
  ini = iniparser_load(cEeDeviceWebrtcCfgIniPath);
  if(ini == NULL)
  {
    printf("inipar_load  failure\n");
    return 1;
  } 

  // ����DeviceId:EuqiID
  iniparser_set(ini, "DeviceId:WebrtcID", (char *)&WebrtcID[0]);
  
  printf("%s\n", iniparser_getstring(ini, "DeviceId:WebrtcID", "null"));

  //д���ļ�
  fp = fopen(cEeDeviceWebrtcCfgIniPath, "w");
  if( fp == NULL ) {
      printf("stone:fopen error!\n");
      return 1;
  }
  
  iniparser_dump_ini(ini, fp);
  
  fclose(fp);
  
  //�ͷ�
  iniparser_freedict(ini);
  
  return  (0);
}


/*!
 * \fn     EeReadWebrtcIDParamToSram
 * \brief  ��ȡ�豸��Ϣ���ڴ�
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

  // ��ȡ����
  EuqiID_src = iniparser_getstring(ini, "DeviceId:WebrtcID", "null");
  printf("apartment:username: %s\n", EuqiID_src);
  
  MemCopySramToSram((unsigned char *)&EuqiID_src[0], (unsigned char *)&WebrtcID[0], sizeof(EuqiID_src));
  
  iniparser_freedict(ini);

  return 0;
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
