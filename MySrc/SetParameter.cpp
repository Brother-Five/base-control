#include "SetParameter.h"
#include "bsp_stmflash.h"
#include "bsp.h"
#include "cpid.h"
#include "Kalman.h"

extern CPID CarFramePID;
extern FUZZYPID PitchPID,YawPID;

union Parameter_User_Union Parameter_User;
union Parameter_Operater_Union Parameter_Operater;

u8 ParaFlag = 0;//设置参数ing的标志

/**
 * [InitParameter_User 默认值]
 */
void InitParameter_User(void)
{
	Parameter_User.data.Begin          = 0x55;
	Parameter_User.data.End            = 0x66;
	//键鼠
	Parameter_User.data.SPEED1         = 400;//一档速度(0~660)
	Parameter_User.data.SPEED2         = 660;//二档速度(0~660)
	Parameter_User.data.PITCHWEIGHT    = 0.2;//PITCH电机调节速度（越大越快）
	Parameter_User.data.YAWWEIGHT[0]   = 1.0;;//YAW电机调节速度（越大越快）
	Parameter_User.data.YAWWEIGHT[1]   = 2.0;//YAW电机调节速度（越大越快）
	//遥控
	Parameter_User.data.DR_SPEED       = 1.5;//速度（越大越快）
	Parameter_User.data.PitchScale     = 60;//PITCH电机调节速度（越小越快）
	Parameter_User.data.YawScale       = 12;//YAW电机调节速度（越小越快）

	Parameter_User.data.CarframeWeight = 5.5 ;//底盘跟随速度
}

/**
 * [GetParameter_User 读取Flash中保存的值，若帧头或帧尾不正确则使用默认值]
 */
void GetParameter_User(void)
{
	STMFLASH_Read(ADDR_FLASH_SECTOR_10,Parameter_User.FlashData,U32_BUFSIZE);
	if(Parameter_User.data.Begin != 0x55 || Parameter_User.data.End != 0x66)
	{
		InitParameter_User();
		SetParameter_User();
	}
}

/**
 * [SetParameter_User 设置参数，存入Flash，自动保存帧头帧尾]
 */
void SetParameter_User(void)
{
	Parameter_User.data.Begin       = 0x55;
	Parameter_User.data.End         = 0x66;
	STMFLASH_Write(ADDR_FLASH_SECTOR_10,Parameter_User.FlashData,U32_BUFSIZE);
}
/**
 * [CheckParameter_User 串口打印当前数据]
 */
void CheckParameter_User(void)
{
//键鼠
	u1_printf("\
	KM:\t \
	SPEED1:%d\t\
	SPEED2:%d\t\
	PITCHWEIGHT:%5.2f\t\
	YAWWEIGHT1:%5.2f\
	YAWWEIGHT2:%5.2f\r\n\
	DR:\t \
	DR_SPEED:%5.2f\t\
	PitchScale:%5.2f\t\
	YawScale:%5.2f\r\n\
	CarFollow:%5.2f\
	\r\n\r\n", \

			Parameter_User.data.SPEED1,Parameter_User.data.SPEED2, \
			Parameter_User.data.PITCHWEIGHT,Parameter_User.data.YAWWEIGHT[0],Parameter_User.data.YAWWEIGHT[1], \
			Parameter_User.data.DR_SPEED,Parameter_User.data.PitchScale, \
			Parameter_User.data.YawScale,Parameter_User.data.CarframeWeight);
}


/**
 * [InitParameter_Operater 默认值]
 */
void InitParameter_Operater(void)
{
	Parameter_Operater.data.Begin              = 0x55;
	Parameter_Operater.data.End                = 0x66;
  
	Parameter_Operater.data.PITCH_Encoder_Mid  = 3965.0;//一档速度(0~660)
	Parameter_Operater.data.PITCH_Encoder_Up   = 3600.0;//二档速度(0~660)
	Parameter_Operater.data.PITCH_Encoder_Dowm = 4580.0;//PITCH电机调节速度（越大越快）
	Parameter_Operater.data.YAW_Encoder_Mid    = 4800.0;//YAW电机调节速度（越大越快）
	Parameter_Operater.data.PITCH_6050_Offset  = 0;//6050角速度offset
	Parameter_Operater.data.YAW_6050_Offset    = 0;//6050角速度offset
  
}

/**
 * [GetParameter_Operater 读取Flash中保存的值，若帧头或帧尾不正确则使用默认值]
 */
void GetParameter_Operater(void)
{
	STMFLASH_Read(ADDR_FLASH_SECTOR_11,Parameter_Operater.FlashData,U32_BUFSIZE);
	if(Parameter_Operater.data.Begin != 0x55 || Parameter_Operater.data.End != 0x66)
	{
		InitParameter_Operater();
		SetParameter_Operater();
	}
}

/**
 * [SetParameter_Operater 设置参数，存入Flash，自动保存帧头帧尾]
 */
void SetParameter_Operater(void)
{
	Parameter_Operater.data.Begin       = 0x55;
	Parameter_Operater.data.End         = 0x66;
	STMFLASH_Write(ADDR_FLASH_SECTOR_11,Parameter_Operater.FlashData,U32_BUFSIZE);
}
/**
 * [CheckParameter_Operater 串口打印当前数据]
 */
void CheckParameter_Operater(void)
{
//键鼠
	u1_printf("\
	PITCH_Encoder_Mid:%5.2f\t\
	PITCH_Encoder_Up:%5.2f\t\
	PITCH_Encoder_Dowm:%5.2f\t\
	YAW_Encoder_Mid:%5.2f\t\
	PITCH_6050_Offset:%5.2f\t\
	YAW_6050_Offset:%5.2f\t\
	\r\n\r\n",\
			Parameter_Operater.data.PITCH_Encoder_Mid,Parameter_Operater.data.PITCH_Encoder_Up, \
			Parameter_Operater.data.PITCH_Encoder_Dowm,Parameter_Operater.data.YAW_Encoder_Mid,\
			Parameter_Operater.data.PITCH_6050_Offset,Parameter_Operater.data.YAW_6050_Offset
			);
}

void SetParaFlag(u8 _pFlag)
{
	ParaFlag = _pFlag;
};
u8 GetParaFlag(void)
{
	return ParaFlag;
};

/**
 * [SetPara_DR 设置遥控操控的参数]
 * @param _DR_SPEED      [速度参数]
 * @param _PitchScale    [PITCH电机速度]
 * @param _YawScale      [Yaw电机速度]
 * @param CARFRAMEWEIGHT [跟随速度]
 */
void SetPara_DR(float _DR_SPEED,float _PitchScale,float _YawScale,float CARFRAMEWEIGHT)
{
	Parameter_User.data.DR_SPEED       = _DR_SPEED;//速度（越大越快）
	Parameter_User.data.PitchScale     = _PitchScale;//PITCH电机调节速度（越小越快）
	Parameter_User.data.YawScale       = _YawScale;//YAW电机调节速度（越小越快）
//	Parameter_User.data.CarframeWeight = CARFRAMEWEIGHT ;//底盘跟随速度
//	CarFramePID.SetPID((float)Parameter_User.data.CarframeWeight,0.00,0.00);//设置跟随速度
	CheckParameter_User();
}

/**
 * [SetPara_KM 设置键鼠操控的参数]
 * @param SPEED1         [第一档速度]
 * @param SPEED2         [第二档速度]
 * @param PITCHWEIGHT[2]    [PITCH电机速度]
 * @param YAWWEIGHT      [Yaw电机速度]
 * @param CARFRAMEWEIGHT [跟随速度]
 */
void SetPara_KM(float SPEED1,float SPEED2,float PITCHWEIGHT,float YAWWEIGHT1,float YAWWEIGHT2,float CARFRAMEWEIGHT)
{
	Parameter_User.data.SPEED1         = SPEED1;//一档速度(0~660)
	Parameter_User.data.SPEED2         = SPEED2;//二档速度(0~660)
	Parameter_User.data.PITCHWEIGHT    = PITCHWEIGHT;//PITCH电机调节速度（越大越快）
	Parameter_User.data.YAWWEIGHT[0]      = YAWWEIGHT1;//YAW电机调节速度（越大越快）
	Parameter_User.data.YAWWEIGHT[1]      = YAWWEIGHT2;//YAW电机调节速度（越大越快）
//	Parameter_User.data.CarframeWeight = CARFRAMEWEIGHT;//底盘跟随速度
//	CarFramePID.SetPID((float)Parameter_User.data.CarframeWeight,0.00,0.00);//设置跟随速度
	CheckParameter_User();
}

/**
 * [SetPara_Operater 管理员参数设置]
 * @param mode [0：PITCH_Encoder_Mid
 *             1：PITCH_Encoder_Up
 *             2：PITCH_Encoder_Dowm
 *             3：YAW_Encoder_Mid]
 */
void SetPara_Operater(float mode)
{
	
	short gx,gy,gz;

	switch((u8)mode)
	{
		case 0: Parameter_Operater.data.PITCH_Encoder_Mid = GetMotorEncoder().PitchEncoder;//GetMotorEncoder().PitchEncoder;
				CheckParameter_Operater();
				break;
		case 1: Parameter_Operater.data.PITCH_Encoder_Up = GetMotorEncoder().PitchEncoder;
				CheckParameter_Operater();
				break;
		case 2: Parameter_Operater.data.PITCH_Encoder_Dowm = GetMotorEncoder().PitchEncoder;
				CheckParameter_Operater();
				break;
		case 3: Parameter_Operater.data.YAW_Encoder_Mid = GetMotorEncoder().YawEncoder;
				CheckParameter_Operater();
				break;
		case 4:	DISABLE_INT();
					{
						unsigned int i;
						float sum1,sum2;
						kalman_struct AngleKalman_PITCH,AngleKalman_YAW;
						kalman_init(&AngleKalman_PITCH,0, 5,0.1,30000);
						kalman_init(&AngleKalman_YAW,0, 5,0.1,30000);
						{
							
							for(i = 0;i < 20000;i++)
							{
								MPU_Get_Gyroscope(&gx,&gy,&gz);
//								sum1 += (float)gx/1024.0f;
//								sum2 += (float)gz/1024.0f;
								sum1 = kalman_filter(&AngleKalman_YAW,(float)gx);
								sum2 = kalman_filter(&AngleKalman_PITCH,(float)gz);
							}
							
							
							Parameter_Operater.data.PITCH_6050_Offset = sum1;
							Parameter_Operater.data.YAW_6050_Offset = sum2;

							bsp_LedToggle(1);
							delay_ms_nos(1);
						}
					}
				SetParameter_Operater();
				CheckParameter_Operater();
				ENABLE_INT();
				break;
		default:break;
	}
}

/**
 * [ParaSetting 参数设置]
 * @param _pFlag:[1：开始设置User参数
 *               2：取得默认User参数
 *               3：保存User参数
 *               4：开始设置Operater参数
 *               5：取得默认Operater参数
 *               6：保存Operater参数
 *               ]
 */
void ParaSetting(float _pFlag)
{
	
	switch((u8)_pFlag)
	{
		case 1 :SetParaFlag(1);
				GetParameter_User();
				CheckParameter_User();
				SetParaFlag(0);
				break;
		case 2 :SetParaFlag(1);
				InitParameter_User();
				CheckParameter_User();
				SetParaFlag(0);
				break;
		case 3 :SetParaFlag(1);
				SetParameter_User();
				CheckParameter_User();
				SetParaFlag(0);
				break;

		case 4 :SetParaFlag(1);
				GetParameter_Operater();
				CheckParameter_Operater();
				break;
		case 5 :InitParameter_Operater();
				CheckParameter_Operater();
				break;
		case 6 :SetParameter_Operater();
				CheckParameter_Operater();
				break;
		case 8 :
		{
			extern u8 PIDBegin_CAN;
			CanTxMsg msg_send = { 0x1ff, 0x1ff, CAN_Id_Standard, CAN_RTR_Data, 0x08,{0, 0, 0, 0, 0, 0, 0, 0}};
			PIDBegin_CAN = 0;
			delay_ms_nos(100);
			msg_send.Data[6] = 0x04;
			CAN_Transmit(CAN2,&msg_send);
		}
		break;
		default:break;
	}
	
}

/**
 * [bsp_ParaInit 从Flash中取得参数,相应设置好相关参数]
 */
void bsp_ParaInit(void)
{
	GetParameter_User();
	GetParameter_Operater();
	CarFramePID.SetPID(3.5,0.0,0.0);//设置跟随速度
//	SetPitchPid(8.5,0.0,-0.00495)
	PitchPID.SetTargetMaxMin(Parameter_Operater.data.PITCH_Encoder_Up,Parameter_Operater.data.PITCH_Encoder_Dowm);//先设置目标值的最大和最小值，再设置目标值
	PitchPID.SetTarget(Parameter_Operater.data.PITCH_Encoder_Mid);
	PitchPID.SetOmegaOffset(Parameter_Operater.data.PITCH_6050_Offset);
	PitchPID.SetOmegaOffset(Parameter_Operater.data.YAW_6050_Offset);
//	Parameter_Operater.data.YAW_Encoder_Mid +=1300;
}
