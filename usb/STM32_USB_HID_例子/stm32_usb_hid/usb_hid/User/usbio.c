/**
  ******************************************************************************
  * @file    usbio.c
  * $Author: wdluo $
  * $Revision: 67 $
  * $Date:: 2012-08-15 19:00:29 +0800 #$
  * @brief   USB�ϲ㺯��.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_desc.h"
#include "usb_lib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
extern uint8_t USB_Receive_Buffer[REPORT_COUNT];
extern uint8_t USB_Send_Buffer[REPORT_COUNT];
uint8_t g_dataSerialNumber;
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

  
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ͨ��USB��������
  * @param  data ���ݴ洢�׵�ַ
  * @param  dataNum ���͵������ֽ���
  * @retval ���͵��ֽ���
  */
uint32_t USB_SendData(uint8_t *data,uint32_t dataNum)
{
	#ifndef STM32F10X_CL
	//������ͨ��USB���ͳ�ȥ
	UserToPMABufferCopy(data, ENDP2_TXADDR, dataNum);
	SetEPTxCount(ENDP2, REPORT_COUNT);
	SetEPTxValid(ENDP2);
	#else
	USB_SIL_Write(EP2_IN, data, dataNum);
	#endif
	return dataNum;  
}
/**
  * @brief  ���մ�USB��ȡ������
  * @param  data ���ݴ洢�׵�ַ
  * @param  dataNum ׼����ȡ�������ֽ���
  * @retval ��ȡ���ֽ���
  */
uint32_t USB_GetData(uint8_t *data,uint32_t dataNum)
{
    uint32_t len=0;
	if(dataNum>sizeof(USB_Receive_Buffer)){
		dataNum = sizeof(USB_Receive_Buffer);
	}
	for(len=0;len<dataNum;len++){
		*data=USB_Receive_Buffer[len];
		data++;
	}
    return dataNum;
}

/*********************************END OF FILE**********************************/

