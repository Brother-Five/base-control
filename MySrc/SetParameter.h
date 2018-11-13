#ifndef _SETPARAMETER_H_
#define _SETPARAMETER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stm32f4xx.h>

#define U32_BUFSIZE 16 //32位发送数组中元素个数
extern union Parameter_User_Union Parameter_User;
extern union Parameter_Operater_Union Parameter_Operater;
struct Parameter_User_Struct
{
	unsigned char Begin;//帧头
	//键鼠
	uint16_t SPEED1;//一档速度(0~660)
	uint16_t SPEED2;//二档速度(0~660)
	float PITCHWEIGHT;//PITCH电机调节速度（越大越快）
	float YAWWEIGHT[2];//YAW电机调节速度（越大越快）

	//遥控
	float DR_SPEED;//速度（越大越快）
	float PitchScale;//PITCH电机调节速度（越小越快）
	float YawScale;//YAW电机调节速度（越小越快）

	float CarframeWeight;
	unsigned char End;//帧尾
};
union Parameter_User_Union
{
	struct Parameter_User_Struct data;
	u32 FlashData[U32_BUFSIZE];
};


struct Parameter_Operater_Struct
{
	unsigned char Begin;//帧头
    float PITCH_Encoder_Mid;//PITCH电机码盘值
    float PITCH_Encoder_Up;//PITCH电机码盘值
    float PITCH_Encoder_Dowm;//PITCH电机码盘值
    float YAW_Encoder_Mid;//YAW电机码盘值
	float PITCH_6050_Offset;//6050角速度offset
    float YAW_6050_Offset;//6050角速度offset
	unsigned char End;//帧尾
};

union Parameter_Operater_Union
{
	struct Parameter_Operater_Struct data;
	u32 FlashData[U32_BUFSIZE];
};



//#define YAW_ENCODER_MID  2646	//YAW电机码盘值
//#define YAW_ENCODER_MIN	 1246	//YAW电机码盘值

//#define YAW_ENCODER_MAX  4023	//YAW电机码盘值
// #define Middle 5679  //YAW电机中间码盘值


// #define PITCH_ENCODER_MID   2545	//PITCH电机码盘值
// #define PITCH_ENCODER_UP	2110	//PITCH电机码盘值
// #define PITCH_ENCODER_DOWM	2700	//PITCH电机码盘值

// #define CARFRAMEWEIGHT 4.0	//底盘跟随速度


void InitParameter_User(void);
void GetParameter_User(void);
void SetParameter_User(void);
void CheckParameter_User(void);

void InitParameter_Operater(void);
void GetParameter_Operater(void);
void SetParameter_Operater(void);
void CheckParameter_Operater(void);

void bsp_ParaInit(void);

u8 GetParaFlag(void);
void SetParaFlag(u8 _pFlag);
void SetPara_DR(float _DR_SPEED,float _PitchScale,float _YawScale,float CARFRAMEWEIGHT);
void SetPara_KM(float SPEED1,float SPEED2,float PITCHWEIGHT,float YAWWEIGHT1,float YAWWEIGHT2,float CARFRAMEWEIGHT);
void SetPara_Operater(float mode);
void ParaSetting(float _pFlag);
#ifdef __cplusplus
}
#endif

#endif

