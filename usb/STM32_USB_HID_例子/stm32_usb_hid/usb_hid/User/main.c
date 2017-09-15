/**
  ******************************************************************************
  * @file    main.c
  * $Author: wdluo $
  * $Revision: 67 $
  * $Date:: 2012-08-15 19:00:29 +0800 #$
  * @brief   主函数.
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
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usbio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t USB_Received_Flag;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  串口打印输出
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t data[64];
	uint32_t i=0,ret=0;
	Set_System();//系统时钟初始化
	USART_Configuration();//串口1初始化
	printf("\x0c\0");printf("\x0c\0");//超级终端清屏
	printf("\033[1;40;32m");//设置超级终端背景为黑色，字符为绿色
	printf("\r\n*******************************************************************************");
	printf("\r\n************************ Copyright 2009-2012, ViewTool ************************");
	printf("\r\n*************************** http://www.viewtool.com ***************************");
	printf("\r\n***************************** All Rights Reserved *****************************");
	printf("\r\n*******************************************************************************");
	printf("\r\n");

	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();

	while(1)
	{
		if(USB_Received_Flag){
			USB_Received_Flag=0;
			ret = USB_GetData(data,sizeof(data));
			printf("usb get data %d byte data\n\r",ret);
			for(i=0;i<ret;i++){
				printf("0x%02X ",data[i]);
			}
			printf("\n\r");
			USB_SendData(data,sizeof(data));
		}
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  报告在检查参数发生错误时的源文件名和错误行数
  * @param  file 源文件名
  * @param  line 错误所在行数 
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    /* 无限循环 */
    while (1)
    {
    }
}
#endif

/*********************************END OF FILE**********************************/
