/**
 * RTX系统中CAN2相关任务中的执行的子函数
 */

#include "bsp.h"
#include "cpid.h"
#include "filter.h"
#include "SetParameter.h"
#include "Kalman.h"
#define Num 5

//#define Middle 2678  //Yaw轴中间的码臘

extern union Parameter_Operater_Union Parameter_Operater;
extern FUZZYPID PitchPID,YawPID;
extern FUZZYSPID CarFramePID;
extern SCPID TurnplatePID;

extern OS_TID HandleDR16_SxValue;
extern OS_TID HandleDisconnectCheck;
u8 YawChangeFlag=0;
float YAW_Angle,YAW_Angle_Zero;
u8 Infrared_High,Infrared_Cnt,Infrared_Message;
u8 runstart_flag;
MotorEncoderStruct MotorEncoder;
AngleCorrectStruct AngleCorrect;

#define PITCHSPEEDCODER 0
#define YAWSPEEDCODER   1

EncoderStuct Speed6623[2];
void Set_Speed6623(int32_t temp,u8 id)
{
	static int last_position[2];
	static int last_coder[2],this_coder[2],turn_cnt[2];

	this_coder[id] = temp;

	if(this_coder[id] - last_coder[id] > 4096)
	{
		turn_cnt[id]--;
	}
	else if(this_coder[id] - last_coder[id] < -4096)
	{
		turn_cnt[id]++;
	}
	last_coder[id] = this_coder[id];

	Speed6623[id].Position = this_coder[id] + turn_cnt[id] * 8192;
	Speed6623[id].Speed = Speed6623[id].Position - last_position[id];
	last_position[id] = Speed6623[id].Position;
}




EncoderStuct Get_Speed6623(u8 id)
{
	return Speed6623[id];
}

/**
 * [PitchMotor Pitch轴电机数据处理]
 * @param msg_rece [CAN接收的结构体]
 */
void PitchMotor(CanRxMsg *msg_rece)
{
	// short TempPitch[4];
	static u16 flagPitch = 0, initcountPitch = 0;
	static short filter_bufPitch[Num];
	static unsigned char iPitch=0;

//=====================================================================================
	static short last_angle,this_angle;
	static int turn_cnt,first_flag = 1;
	last_angle = this_angle;
	this_angle = (u16)msg_rece->Data[0] <<8 | (u16)msg_rece->Data[1];

	if(first_flag)//第一次检测处理
	{
		last_angle = this_angle;
		first_flag = 0;
	}
	if(last_angle < 2048 && this_angle > 6144 )
	{
		turn_cnt--;
	}
	else if(this_angle < 2048 && last_angle > 6144)
	{
		turn_cnt++;
	}
	MotorEncoder.PitchEncoder = this_angle + turn_cnt*8192;
//=====================================================================================


	MotorEncoder.PitchEncoder = Middle_filter(filter_bufPitch,&iPitch,Num,MotorEncoder.PitchEncoder);//中值滤波
//	Set_Speed6623(MotorEncoder.PitchEncoder,PITCHSPEEDCODER);
//	u1_printf("%d\r\n",MotorEncoder.PitchEncoder);//调试用
	if(flagPitch)
	{
		PitchPID.SetCurrent((float)MotorEncoder.PitchEncoder);//Pitch轴码盘输出给PID，进行控制
	}
	else
	{
		initcountPitch ++;
		PitchPID.SetCurrent((float)MotorEncoder.PitchEncoder);//Pitch轴码盘输出给PID,避免阶跃造成大幅度震动
		if(initcountPitch == 50)//Pitch和Yaw轴50，底盘800代表着每个PID的启动顺序
		{
			flagPitch = 1;
		}
	}
}

/**
 * [YawMotor Yaw轴电机数据处理]
 * @param msg_rece [CAN接收的结构体]
 */
void YawMotor(CanRxMsg *msg_rece)
{
	// short TempYaw[4];
	static u16 flagYaw = 0, initcount = 0;
	static short filter_bufYaw[Num];
	static unsigned char iYaw=0;

//=====================================================================================
	static short last_angle,this_angle;
	static int turn_cnt,first_flag=1;
	last_angle = this_angle;
	this_angle = (short)msg_rece->Data[0] <<8 | (short)msg_rece->Data[1];
	if(first_flag)//第一次检测处理
	{
		last_angle = this_angle;
		first_flag = 0;
	}
	if(last_angle < 2048 && this_angle > 6144 )
	{
		turn_cnt--;
	}
	else if(this_angle < 2048 && last_angle > 6144)
	{
		turn_cnt++;
	}
	MotorEncoder.YawEncoder = this_angle + turn_cnt*8192;
//=====================================================================================
//
	MotorEncoder.YawEncoder = Middle_filter(filter_bufYaw,&iYaw,Num,MotorEncoder.YawEncoder);//滑动滤波
	AngleCorrect.w3 = MotorEncoder.YawEncoder;
//	Set_Speed6623(MotorEncoder.YawEncoder,YAWSPEEDCODER);

//	u1_printf("%d\r\n",MotorEncoder.YawEncoder);//调试用
	 if(YawChangeFlag == 0 && initcount == 40)//
	 {
		 YAW_Angle_Zero =  (MotorEncoder.YawEncoder - Parameter_Operater.data.YAW_Encoder_Mid)/CodedDiscToAngle;//计算好陀螺仪和码盘初始角度差，给陀螺仪零度校正		 
		 YawChangeFlag = 1;//使能陀螺仪输出给PID，进行控制
	 }
	if(flagYaw)
	{ 
		{
			if(FollowMode ==1)
			{	YawPID.SetCurrent((MotorEncoder.YawEncoder - Parameter_Operater.data.YAW_Encoder_Mid + YawPID.Target));
			  
				}
			else if(FollowMode == 0)
			{
				CarFramePID.SetCurrent((float)(MotorEncoder.YawEncoder - Parameter_Operater.data.YAW_Encoder_Mid));//Yaw轴码盘输出给PID，进行控制

//				u1_printf("%d\r\n",(int)CarFramePID.Current);
//				ANO_Data3_Send(0xf1,((float)camera_angle - Middle)/22.7556f,camera_angle/22.7556f,(CarFramePID.Current)/22.7556f);
			}
		}
	}
	else
	{
		initcount ++;
		if(initcount == 50)
		{
			flagYaw = 1;
		}
	}
}



/**
 * [YawAngle 单轴陀螺仪数据处理]
 * @param msg_rece [CAN接收的结构体]
 */
void YawAngle(CanRxMsg *msg_rece)
{
	static float this_yaw_angle,last_yaw_angle;
    float temp_yaw_angle;
	static int turn_cnt;

	temp_yaw_angle = (int32_t)(msg_rece->Data[0]<<24)|(int32_t)(msg_rece->Data[1]<<16)
	| (int32_t)(msg_rece->Data[2]<<8) | (int32_t)(msg_rece->Data[3]);

	last_yaw_angle = this_yaw_angle;
	this_yaw_angle = -((float)temp_yaw_angle*0.01f);
	if(this_yaw_angle - last_yaw_angle > 180)
	{
		turn_cnt++;
	}
	else if(this_yaw_angle - last_yaw_angle < -180)//180
	{
		turn_cnt--;
	}
	YAW_Angle = -(this_yaw_angle + turn_cnt*360);
//	u1_printf("%d.%d\t%d\r\n",(int)(YAW_Angle),myabs((int)((YAW_Angle - (int)(YAW_Angle))*1000)),CarFramePID.counter);

//	u1_printf("%d\r\n",(short)(YAW_Angle*1000));
//				printf("%f\r\n",YAW_Angle);
	if(YawChangeFlag == 1)
	{
		if(FollowMode == 0)
		{
			YawPID.SetCurrent(((YAW_Angle - YAW_Angle_Zero)*CodedDiscToAngle));
//			camera_a = yaw_angle_6050 + Middle/CodedDiscToAngle;
		}
		else if(FollowMode == 1)
			YAW_Angle_Zero = YAW_Angle - YawPID.Target/CodedDiscToAngle;
//			temp_angle = yaw_angle_6050;
//					printf("%f\r\n",-((YAW_Angle - YAW_Angle_Zero)*CodedDiscToAngle));
	}

}



/**
 * [YawAngle_6050 6050陀螺仪数据处理]
 * @param msg_rece [CAN接收的结构体]
 */

float yaw_angle_6050;
void YawAngle_6050(CanRxMsg *msg_rece)
{
	static float this_yaw_angle,last_yaw_angle;
    float temp_yaw_angle;
	static int turn_cnt;

//	temp_yaw_angle = (float)(((u16)msg_rece->Data[1])<<8 | (u16)msg_rece->Data[0])/ 100.0f;
//	printf("angle:%f\r\n",temp_yaw_angle);
	temp_yaw_angle = (int16_t)(((u16)msg_rece->Data[1])<<8 | (u16)msg_rece->Data[0]);
//	yaw_angle_6050 = temp_yaw_angle * 0.01f;
	last_yaw_angle = this_yaw_angle;
	this_yaw_angle = ((float)temp_yaw_angle*0.01f);
	if(this_yaw_angle - last_yaw_angle > 180)
	{
		turn_cnt--;
	}
	else if(this_yaw_angle - last_yaw_angle < -180)//180
	{
		turn_cnt++;
	}
	yaw_angle_6050 = this_yaw_angle + turn_cnt*360;
	AngleCorrect.w2 = yaw_angle_6050;
//	ANO_Data1_Send(0xf1,yaw_angle_6050*100);
//	u1_printf("%d\r\n",(short)(yaw_angle_6050*100));
	if(YawChangeFlag == 1)
	{
		if(FollowMode == 0)
		{
			YawPID.SetCurrent(((yaw_angle_6050 - YAW_Angle_Zero)*CodedDiscToAngle));
//			camera_a = yaw_angle_6050 + Middle/CodedDiscToAngle;
		}
		else if(FollowMode == 1)
			YAW_Angle_Zero = yaw_angle_6050 - YawPID.Target/CodedDiscToAngle;
//			temp_angle = yaw_angle_6050;
//					printf("%f\r\n",-((YAW_Angle - YAW_Angle_Zero)*CodedDiscToAngle));
	}
}

union{
	struct
	{
		u8 index[4];
	}byte;
	float value;
}tt;

float yaw_angle_ADXRS622;
void YawAngle_ADXRS622(CanRxMsg *msg_rece)
{
//	float *p;
	// yaw_angle_ADXRS622 = ((float)((msg_rece->Data[1]<<8 | msg_rece->Data[0]))/1000.0f);
	tt.byte.index[0] = msg_rece->Data[0];
	tt.byte.index[1] = msg_rece->Data[1];
	tt.byte.index[2] = msg_rece->Data[2];
	tt.byte.index[3] = msg_rece->Data[3];
	yaw_angle_ADXRS622 = *((float *)(&msg_rece->Data[1]))/1000.0f;
//	u1_printf("%d\t%d\t%d\t%d\r\n",&msg_rece->Data[0],&msg_rece->Data[1],&msg_rece->Data[2],&msg_rece->Data[3]);
//	u1_printf("%d\t%d\t%d\t%d\r\n",&tt.byte.index[0],&tt.byte.index[1],&tt.byte.index[2],&tt.byte.index[3]);
//	u1_printf("%d.%d\t%d\r\n",(int)(yaw_angle_ADXRS622),myabs((int)((yaw_angle_ADXRS622 - (int)(yaw_angle_ADXRS622))*1000)),CarFramePID.counter);
	AngleCorrect.w2 = yaw_angle_ADXRS622;
	if(YawChangeFlag == 1)
	{
		if(FollowMode == 0)
		{
			YawPID.SetCurrent(((yaw_angle_ADXRS622 - YAW_Angle_Zero)*CodedDiscToAngle));
//			camera_a = yaw_angle_ADXRS622 + Middle/CodedDiscToAngle;
		}
		else if(FollowMode == 1)
			YAW_Angle_Zero = yaw_angle_ADXRS622 - YawPID.Target/CodedDiscToAngle;
//			temp_angle = yaw_angle_ADXRS622;
//					printf("%f\r\n",-((YAW_Angle - YAW_Angle_Zero)*CodedDiscToAngle));
	}
}


MotorEncoderStruct GetMotorEncoder(void)
{
	return	MotorEncoder;
}


void New_Board(CanRxMsg *msg_rece)
{
//	 u1_printf("123");
   Infrared_Message=msg_rece->Data[0];
	 if(Infrared_Message==0x01){ADDBULLET_BUTTON=!ADDBULLET_BUTTON;}
	 if(Infrared_Message==0x02){Infrared_Cnt=!Infrared_Cnt;}
	 if(Infrared_Message==0x03){runstart_flag=0;} 
   if(Infrared_Message==0x04){runstart_flag=1;}
}
