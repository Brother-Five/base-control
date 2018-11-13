/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/
#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB )//LED1~LED4����������
#define __LED1						//ʹ��LED1
#define GPIO_PORT_LED1  GPIOC 		//LED1��GPIO����
#define GPIO_PIN_LED1	GPIO_Pin_6 	//LED1��GPIO����

#define __LED2						//ʹ��LED2
#define GPIO_PORT_LED2  GPIOB 		//LED2��GPIO����
#define GPIO_PIN_LED2	GPIO_Pin_15 //LED2��GPIO����

#define __LED3						//ʹ��LED3
#define GPIO_PORT_LED3  GPIOC 		//LED3��GPIO����
#define GPIO_PIN_LED3	GPIO_Pin_13 //LED3��GPIO����

//#define __LED4					//ʹ��LED4
#define GPIO_PORT_LED4  GPIOC 		//LED4��GPIO����
#define GPIO_PIN_LED4	GPIO_Pin_14 //LED4��GPIO����
/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/

/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);				//LEDӲ����ʼ��
void bsp_LedOn(uint8_t _no);		//LED��
void bsp_LedOff(uint8_t _no);		//LED��
void bsp_LedToggle(uint8_t _no);	//LED��ת
uint8_t bsp_IsLedOn(uint8_t _no);	//���LED״̬


#ifdef __cplusplus
}
#endif

#endif
