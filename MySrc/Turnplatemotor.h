#ifndef _TURNPLATE_MOTOR_H___
#define _TURNPLATE_MOTOR_H___
#include <stm32f4xx.h>

#define Motor_Bin1	PCout(4)
#define Motor_Bin2	PAout(7)
#define MOS_PWM		PAout(6)

typedef struct Encoder_Stuct
{
	int16_t Speed;
	int Position;
}EncoderStuct;

#define Turnplate_motor_PWM  TIM3->CCR4
//#define Turnplate_motor_PWM_IO PAout(6)

void Turnplate_motor_cofiguration(void);
void Encoder_Start(void);
EncoderStuct Encoder_Get_CNT(void);
void Turnplate_motor_drive(u8 flag);
void Turnplate_motor_Toggle(void);
void Turnplate_motor_ONCE(void);
void Turnplate_motor_CONSTANTLY(void);
void Turnplate_motor_STOP(void);
void Turnplate_motor_KM_STOP(void);
void Turnplate_motor(int temp);
void Turn_PlateMotor_IO_Configuratuion(void);
#endif

