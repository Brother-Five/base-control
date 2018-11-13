#ifndef _OS_CAN1_H___
#define _OS_CAN1_H___
#include <stm32f4xx.h>
#include "includes.h"
typedef __packed struct
{
	int8_t ID;
	int8_t Flag;
	int16_t CV_X;
	int16_t CV_Y;
	int16_t RES;
}CVStruct;
#define ToCVStructPointer(_ptemp)  ((CVStruct*)_ptemp)
extern short distance1,distance2,distance3,distance4;
extern int temp_1,temp_2,temp_3,temp_4;
extern int add_A1,add_A2,add_A3,add_A4;
extern int add_B1,add_B2,add_B3,add_B4;
extern u8  rand_flag;
extern u8  zero_flag;
extern u8  direction;
extern u8 ultrasonic_correct_flag;
extern u8 auto_attack_flag;
extern INT16_UNION Yaw;
extern float direction_zero;
extern u16 ultrasonic_correct_threshold;
extern u8  direction_select_flag;
extern u8  spin_flag;
extern u8  across_times;
extern float yaw_angle,yaw_angle_360,yaw_angle_180;
extern u8  spin_direction_flag;
extern float  last_yawangle;
extern u16 Gyo_X,Gyo_Y;
extern u8 Utc_Answer;
extern float Gyo_Z;  
void left(void);
void right(void);
void forward(void);
void back(void);
void stop(void);
void Run_Random(CanRxMsg *msg_rece);
void Run_Correct(CanRxMsg *msg_rece);
void Run_Random_v2(CanRxMsg *msg_rece);
void Run_By_Surround(CanRxMsg *msg_rece);
void ULtrasonic_Correct(void);
u8 ULtrasonic_Correct_Start(void);
void Hardware_Random_Init(void);
void CV_ToPC(CVStruct *pMsg);
void CV_FromPC(CanRxMsg *pMsg);
u8 Get_cv_flag(void);
void Set_cv_flag(u8 tflag);
void Hardware_Random_Init(void);
void Run_Protect(void);
void Run_By_Surround(CanRxMsg *msg_rece);
int Get_RandomMotor1(void);
int Get_RandomMotor2(void);
int Get_RandomMotor3(void);
int Get_RandomMotor4(void);
void Motor_Aguest_Random(u8 hurt_origin,u8 hurt_id);
void Run_Spin(CanRxMsg *msg_rece);
void Send_OverPowerFlag(u8 flag);
void Moving_Gyo(CanRxMsg *msg_rece);
void Base_AutoAttack(CVStruct *CVData);
#endif

