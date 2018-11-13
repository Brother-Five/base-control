#ifndef __BSP_HARDTIMER_H
#define __BSP_HARDTIMER_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "bsp.h"

/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/
/*
	��������Ӳ����ʱ����TIM�� ����ʹ TIM2 - TIM5
	TIM3 ��TIM4 ��16λ
	TIM2 ��TIM5 ��32λ
*/
//#define USE_TIM2
#define TIM2_Prescaler 84 - 1
#define TIM2_Period    0xFFFFFFFF

#define USE_TIM3
#define TIM3_Prescaler 84 - 1
#define TIM3_Period    0xFFFF

//#define USE_TIM4
#define TIM4_Prescaler 84 - 1
#define TIM4_Period    0xFFFF

//#define USE_TIM5
#define TIM5_Prescaler 84 - 1
#define TIM5_Period    0xFFFFFFFF
/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/
/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */


/* �����豸�ṹ�� */
typedef struct
{
	uint32_t _uTimeOut[4];
	uint32_t _uMode[4];
	uint32_t _Prescaler;
	uint32_t _Period;
	void (*s_TIM_CallBack1)(void);
	void (*s_TIM_CallBack2)(void);
	void (*s_TIM_CallBack3)(void);
	void (*s_TIM_CallBack4)(void);

}TIM_T;

void bsp_InitHardTimer(void);
void TIMVarInit(void);
void InitHardTIM(void);
void ConfigTIMNVIC(void);
void bsp_StartHardTimer(TIM_TypeDef *TIMx,uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack,MODE_T _mode);
void TIM1_timer(uint32_t _uiTimeOut, void * _pCallBack);
#ifdef __cplusplus
}
#endif
#endif
