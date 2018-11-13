/*
*********************************************************************************************************
*
*	模块名称 : 外部中断模块
*	文件名称 : bsp_exit.h
*	版    本 : V1.0
*	说    明 : 外部中断触发方式头文件
*	修改记录 :
*
*********************************************************************************************************
*/

#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "bsp.h"

/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/
extern void EXTI_CallBack0(void);
#define USE_EXTI_LINE_8		//使能中断线8
#define EXTI_LINE_8_PortSource 		EXTI_PortSourceGPIOC
#define EXTI_LINE_8_Line    		EXTI_Line8
#define EXTI_LINE_8_Trigger 		EXTI_Trigger_Rising
#define EXTI_LINE_8_Mode 		    AUTO_MODE               //1:只触发一次 0：连续可触发
#define EXTI_LINE_8_CallBack 		(void *)EXTI_CallBack0;

extern void EXTI_CallBack1(void);
#define USE_EXTI_LINE_9		//使能中断线9
#define EXTI_LINE_9_PortSource 		EXTI_PortSourceGPIOC
#define EXTI_LINE_9_Line    		EXTI_Line9
#define EXTI_LINE_9_Trigger 		EXTI_Trigger_Rising
#define EXTI_LINE_9_Mode 		    AUTO_MODE               //1:只触发一次 0：连续可触发
#define EXTI_LINE_9_CallBack 		(void *)EXTI_CallBack1;
/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/

void bsp_InitEXTI(void);
void ExtiConfiguration(uint8_t EXTI_PortSourceGPIOx,uint8_t EXTI_PinSourcex,EXTITrigger_TypeDef _EXTITrigger,void * _pCallBack,MODE_T _mode);
void EXTI_Cmd(uint32_t Linex,FunctionalState NewState);

#ifdef __cplusplus
 }
#endif

#endif

