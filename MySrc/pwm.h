#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f4xx.h"


#define PWM1  TIM2->CCR1
#define PWM2  TIM2->CCR2
#define PWM3  TIM2->CCR3
#define PWM4  TIM2->CCR4
#define PWM5  TIM3->CCR1
#define PWM6  TIM3->CCR2
#define PWM7  TIM3->CCR3
#define PWM8  TIM3->CCR4
#define MODEL_BUTTON1 	PBin(0)
#define MODEL_BUTTON2 	PBin(1)	
#define MODEL_BUTTON3 	PBin(10)
#define MODEL_BUTTON4 	PCin(11)
//#define STILL_BUTTON             MODEL_BUTTON1 	
//#define OVERPOWER_BUTTON         MODEL_BUTTON2 		
//#define ADDBULLET_BUTTON         MODEL_BUTTON3 
//#define STARTBYHURT_BUTTON       MODEL_BUTTON4 
//#define MODEL           MODEL_BUTTON1<<2|MODEL_BUTTON2<<1|MODEL_BUTTON3;
void TIM2_PWM_Configuration(void);
void TIM3_PWM_Configuration(void);
void ESC_Init(void);
void ESC_STOP(void);
void ESC_RUN(u8 pwm1,u8 pwm2);
void ESC_Toggle(void);
u8 Get_ESC_Flag(void);
u8 Set_ESC_Flag(u8 flag);
void Duoji_Open(void);
void Duoji_Close(void);
void Duoji_Toggle(void);
void Model_Select_Initial(void);
extern u8 Run_Model,STILL_BUTTON,OVERPOWER_BUTTON,ADDBULLET_BUTTON,STARTBYHURT_BUTTON;
#endif


