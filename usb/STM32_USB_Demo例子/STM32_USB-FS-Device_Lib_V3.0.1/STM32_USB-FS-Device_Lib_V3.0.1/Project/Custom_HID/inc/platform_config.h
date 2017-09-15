/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL)
 //#define USE_STM3210B_EVAL
 #define USE_STM3210E_EVAL
#endif


/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL
  #define USB_DISCONNECT                      GPIOD  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD

  #define RCC_APB2Periph_GPIO_KEY             RCC_APB2Periph_GPIOB
  #define RCC_APB2Periph_GPIO_TAMPER          RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_GPIO_IOAIN           RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOC

  #define GPIO_KEY                            GPIOB
  #define GPIO_TAMPER                         GPIOC
  #define GPIO_IOAIN                          GPIOC
  #define GPIO_LED                            GPIOC

  #define GPIO_KEY_PIN                        GPIO_Pin_9   /* PB.09 */
  #define GPIO_TAMPER_PIN                     GPIO_Pin_13  /* PC.13 */
  #define GPIO_IOAIN_PIN                      GPIO_Pin_4   /* PC.04 */
  
  #define GPIO_LED1_PIN                       GPIO_Pin_6   /* PC.06 */
  #define GPIO_LED2_PIN                       GPIO_Pin_7   /* PC.07 */ 
  #define GPIO_LED3_PIN                       GPIO_Pin_8   /* PC.08 */
  #define GPIO_LED4_PIN                       GPIO_Pin_9   /* PC.09 */ 

  #define GPIO_KEY_PORTSOURCE                 GPIO_PortSourceGPIOB
  #define GPIO_KEY_PINSOURCE                  GPIO_PinSource9
  #define GPIO_KEY_EXTI_Line                  EXTI_Line9

  #define GPIO_TAMPER_PORTSOURCE              GPIO_PortSourceGPIOC
  #define GPIO_TAMPER_PINSOURCE               GPIO_PinSource13
  #define GPIO_TAMPER_EXTI_Line               EXTI_Line13

  #define ADC_AIN_CHANNEL                     ADC_Channel_14

#else /* USE_STM3210E_EVAL */
  #define USB_DISCONNECT                      GPIOB  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_14
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB

  #define RCC_APB2Periph_GPIO_KEY             RCC_APB2Periph_GPIOG
  #define RCC_APB2Periph_GPIO_TAMPER          RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_GPIO_IOAIN           RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOF

  #define GPIO_KEY                            GPIOG
  #define GPIO_TAMPER                         GPIOC
  #define GPIO_IOAIN                          GPIOC
  #define GPIO_LED                            GPIOF

  #define GPIO_KEY_PIN                        GPIO_Pin_8   /* PG.08 */
  #define GPIO_TAMPER_PIN                     GPIO_Pin_13  /* PC.13 */
  #define GPIO_IOAIN_PIN                      GPIO_Pin_4   /* PC.04 */
  
  #define GPIO_LED1_PIN                       GPIO_Pin_6   /* PF.06 */
  #define GPIO_LED2_PIN                       GPIO_Pin_7   /* PF.07 */ 
  #define GPIO_LED3_PIN                       GPIO_Pin_8   /* PF.08 */
  #define GPIO_LED4_PIN                       GPIO_Pin_9   /* PF.09 */ 

  #define GPIO_KEY_PORTSOURCE                 GPIO_PortSourceGPIOG
  #define GPIO_KEY_PINSOURCE                  GPIO_PinSource8
  #define GPIO_KEY_EXTI_Line                  EXTI_Line8

  #define GPIO_TAMPER_PORTSOURCE              GPIO_PortSourceGPIOC
  #define GPIO_TAMPER_PINSOURCE               GPIO_PinSource13
  #define GPIO_TAMPER_EXTI_Line               EXTI_Line13

  #define ADC_AIN_CHANNEL                     ADC_Channel_14


#endif /* USE_STM3210B_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

