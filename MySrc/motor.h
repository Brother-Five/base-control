#ifndef __MOTOR_H__
#define __MOTOR_H__
#ifdef __cplusplus
 extern "C" {
#endif

#include <stm32f4xx.h>

#define myabs(x) ((x)>0? (x):(-(x)))



typedef enum {GameInfoID = 0x0001, RealBloodChangedDataID = 0x0002,RealShootDataID = 0x0003} FrameID;
typedef __packed struct
{
	uint8_t SOF;
	uint16_t DataLength;
	uint8_t CRC8;
	uint16_t CmdID;
}FrameHeader;
typedef enum{
	BUFF_TYPE_NONE, //无效
	BUFF_TYPE_ARMOR = 0x01, //防御符
	BUFF_TYPE_SUPPLY = 0x04, //加血符
	BUFF_TYPE_BULLFTS= 0x08, //加弹符
}eBuffType;
typedef __packed struct
{
	uint32_t remainTime;
	uint16_t remainLifeValue;
	float realChassisOutV;
	float realChassisOutA;
	uint8_t runeStatus[4];
	uint8_t bigRune0Status;
	uint8_t bigRune1status;
	uint8_t conveyorBelts0:2;
	uint8_t conveyorBelts1:2;
	uint8_t parkingApron0:1;
	uint8_t parkingApron1:1;
	uint8_t parkingApron2:1;
	uint8_t parkingApron3:1;
	__packed struct{
		uint8_t flag; //0 无效， 1 有效
		uint32_t x;
		uint32_t y;
		uint32_t z;
		uint32_t compass;
	} gpsData;
}tGameInfo;
typedef __packed struct
{
	uint8_t weakId:4;
	uint8_t way:4;
	uint16_t value;
}tRealBloodChangedData;
typedef __packed struct
{
	float realShootSpeed;
	float realShootFreq;

}tRealShootDataBase;

typedef __packed struct
{
	tRealShootDataBase Bullet;
	tRealShootDataBase Golf;
}tRealShootData;
#define Motor1  0
#define Motor2  1
#define Motor3  2
#define Motor4  3

#define No1 7  //4500/660 = 6.8
void Motor_CarFrame(int16_t current_201,int16_t current_202,int16_t current_203,int16_t current_204);
void Motor_Init(int8_t mode,int8_t ret);


void Motor_YawPitch(int16_t current_205,int16_t current_206);
void Motor_Aguest(u16 Ax,u16 Ay,float Az,float _mAngle);
void X_Axic(int *temp,int power);
void Y_Axic(int *temp,int power);
void Z_Axic(int *temp,int power);
void Motor_DataScale(int *MotorData,int max);
void MOTOR_Init( short id, short mode);
void MOTOR_SetPWM(short pwm,short id);
void Motor(int *temp,u8 motorx,int power);
void Stop(void);
void Motor_Power(u16 id,u16 current);
#ifdef __cplusplus
}
#endif
#endif
