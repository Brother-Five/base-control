/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/
#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB )//LED1~LED4所处的总线
#define __LED1						//使能LED1
#define GPIO_PORT_LED1  GPIOC 		//LED1的GPIO设置
#define GPIO_PIN_LED1	GPIO_Pin_6 	//LED1的GPIO设置

#define __LED2						//使能LED2
#define GPIO_PORT_LED2  GPIOB 		//LED2的GPIO设置
#define GPIO_PIN_LED2	GPIO_Pin_15 //LED2的GPIO设置

#define __LED3						//使能LED3
#define GPIO_PORT_LED3  GPIOC 		//LED3的GPIO设置
#define GPIO_PIN_LED3	GPIO_Pin_13 //LED3的GPIO设置

//#define __LED4					//使能LED4
#define GPIO_PORT_LED4  GPIOC 		//LED4的GPIO设置
#define GPIO_PIN_LED4	GPIO_Pin_14 //LED4的GPIO设置
/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/

/* 供外部调用的函数声明 */
void bsp_InitLed(void);				//LED硬件初始化
void bsp_LedOn(uint8_t _no);		//LED亮
void bsp_LedOff(uint8_t _no);		//LED灭
void bsp_LedToggle(uint8_t _no);	//LED翻转
uint8_t bsp_IsLedOn(uint8_t _no);	//检查LED状态


#ifdef __cplusplus
}
#endif

#endif
