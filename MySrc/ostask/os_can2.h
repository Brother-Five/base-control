#ifndef _OS_CAN2_H___
#define _OS_CAN2_H___
#include <stm32f4xx.h>

#define  CodedDiscToAngle 22.7556f

typedef struct MotorEncoder_Struct
{
	short PitchEncoder;
	short YawEncoder;
}MotorEncoderStruct;

typedef struct AngleCorrect_Struct
{
	float w1;
	float w2;
	float _w2;
	float w3;
	float _w3;
	float angle_offset;
}AngleCorrectStruct;
extern u8 Infrared_High,Infrared_Cnt,Infrared_Message;
extern u8 runstart_flag;
void os_can2_data0(CanRxMsg *msg_rece);
void PitchMotor(CanRxMsg *msg_rece);
void YawMotor(CanRxMsg *msg_rece);
void YawAngle(CanRxMsg *msg_rece);
void YawAngle_6050(CanRxMsg *msg_rece);
void YawAngle_ADXRS622(CanRxMsg *msg_rece);
void os_can2_data1(void);

EncoderStuct Get_Speed6623(u8 id);
MotorEncoderStruct GetMotorEncoder(void);
void New_Board(CanRxMsg *msg_rece);
#endif

