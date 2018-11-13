/**
 * RTX系统中DR16任务中的执行的子函数
 */
#include "RTL.h"
#include "bsp.h"
#include "cpid.h"
#include "filter.h"
#include "SetParameter.h"


//#define  CodedDiscToAngle 22.7556f
//#define YAWMAX  135.0f*CodedDiscToAngle//YAW轴error最大值，防止Target值增长太快，调节更不上就失控
//#define YAWMIN -135.0f*CodedDiscToAngle//YAW轴error最小值，防止Target值增长太快，调节更不上就失控

extern FUZZYPID PitchPID,YawPID;
extern CPID CarFramePID;
extern FUZZYPID TurnplatePID;

extern OS_TID HandleIdle_Task;
extern OS_TID HandleDR16_SxValue;

extern union Parameter_User_Union Parameter_User;
extern u8 PIDBegin_CAN;
short AxicX = 1024,AxicY = 1024,AxicZ=1024;
#define scale 22/660

u8 run_flag;
typedef struct KM_KEY_STRUCT
{
	char KMcounter;
	char KMFlag;
}KM_KEY;
extern void DR16_KM(RC_Value *ptrmsg,uint16_t KEY_PRESSED_OFFSET,KM_KEY * _pKEY);


void os_dr16(RC_Value *ptrmsg)
{
	float TempPitch,TempYaw;

	static int16_t TempAxicX=0,TempAxicY=0;
	static uint16_t s1_flag=0,s2_flag=0;
	static uint8_t count=0,constantly = 0;
	static uint16_t mouse_l_flag=0,mouse_r_flag = 0;
	static KM_KEY KM[6];

//	TurnplatePID.SetTarget(-((short)ptrmsg->ch1 - 1024)*scale);
//	Turnplate_motor_PWM = ((short)ptrmsg->ch1 - 1024)*scale;
	if(!PIDBegin_CAN)
	{
		ptrmsg->s2 = RC_SW_MID;
	}
	if(ptrmsg->s2 != RC_SW_DOWN)
	{
////////////////////////////////////////////////////////////////////////////
/////////////////////////////Pitch电机//////////////////////////////////////

		if(Get_cv_flag() == 0)
		{

////////////////////////////////////////////////////////////////////////////
//////////////////////////////Yaw电机///////////////////////////////////////

//			TempPitch = PitchPID.Target;
//			if(myabs(ptrmsg->ch1 - 1024) > 10)
//				TempPitch += (float)(1024 - (short)ptrmsg->ch1)/Parameter_User.data.PitchScale;
//			PitchPID.SetTarget(TempPitch);
//
//			TempYaw = YawPID.Target;
//			if(myabs(ptrmsg->ch0 - 1024) > 10)
//			{
//				AxicZ = (ptrmsg->ch0 - 1024)*No1;
//				TempYaw += (float)(1024 - (short)ptrmsg->ch0)/Parameter_User.data.YawScale;
//			}
//			else
//				AxicZ = 0;
//			if(FollowMode == 0 )
//					YawPID.SetTarget(TempYaw);



			if(myabs(ptrmsg->ch0 - 1024)>5)
			{
//				AxicZ = ptrmsg->ch0;
				TempYaw = YawPID.Target;
				TempYaw += (float)(1024 - (short)ptrmsg->ch0)/Parameter_User.data.YawScale;
				if(CarFramePID.Current/22.7556f >=85)
				{
					if(YawPID.Target < TempYaw)
						TempYaw = YawPID.Target;
				}
				else if(CarFramePID.Current/22.7556f <=-85)
				{
					if(YawPID.Target > TempYaw)
						TempYaw = YawPID.Target;
				}
				
//				u1_printf("%d\t%d\r\n",((int)((TempYaw - CarFramePID.Current)/22.7556f))%360,(int)(CarFramePID.Current/22.7556f));
				if(FollowMode == 0 )
					YawPID.SetTarget(TempYaw);
					
				
			}
//			if(counter[1] == 00)
//			{
//				TempPitch = PitchPID.Target;
//				TempPitch += (float)(1024 - (short)ptrmsg->ch1)/Parameter_User.data.PitchScale;
//				PitchPID.SetTarget(TempPitch);
//			}
		}
		
		if(myabs(ptrmsg->ch1 - 1024)>5)
		{
//			if(myabs(CarFramePID.Current)<90*22.7556f)
			{
				TempPitch = PitchPID.Target;
				TempPitch += (float)(1024 - (short)ptrmsg->ch1)/Parameter_User.data.PitchScale;//660/1750 
				PitchPID.SetTarget(TempPitch);
				
			}
		}
		
		if(myabs(ptrmsg->ch0 - 1024)>5)
		{
			AxicZ = (int)((1024 - (short)ptrmsg->ch0)*No1);
		}
		else
			AxicZ = 0;

////////////////////////////////////////////////////////////////////////////
//////////////////////////底盘//////////////////////////////////////////////

		if(myabs(ptrmsg->ch2 - 1024)>5)
			AxicX = 1024 - (ptrmsg->ch2 - 1024) * Parameter_User.data.DR_SPEED;
		else
			AxicX = 1024;

		if(myabs(ptrmsg->ch3 - 1024)>5)
			AxicY = 1024 - (ptrmsg->ch3 - 1024) * Parameter_User.data.DR_SPEED;
		else
			AxicY = 1024;

//		if(myabs(ptrmsg->ch2 - 1024) > 10)
//			AxicX = 1024 + (ptrmsg->ch2 - 1024) * Parameter_User.data.DR_SPEED;
//		else
//			AxicX = 1024;

//		if(myabs(ptrmsg->ch3 - 1024) > 10)
//			AxicY = 1024 + (ptrmsg->ch3 - 1024) * Parameter_User.data.DR_SPEED;
//		else
//			AxicY = 1024;
	}
	else
	{

//			Parameter_User.data.SPEED1 = 660;
			float YawWeightTemp = Parameter_User.data.YAWWEIGHT[0];

			DR16_KM(ptrmsg,KEY_PRESSED_OFFSET_Q,&KM[0]);
			DR16_KM(ptrmsg,KEY_PRESSED_OFFSET_C,&KM[1]);
			DR16_KM(ptrmsg,KEY_PRESSED_OFFSET_V,&KM[2]);

//			if(KM[0].KMFlag == 0)
//			{
//				Set_cv_flag(0);
////				StrategicMode = 0;
//			}
			if(KM[2].KMFlag == 0 && KM[1].KMFlag == 0 && KM[0].KMFlag == 0)
			{
				
				
//				if(StrategicMode == 4)
//				{
////					YawPID.Target = YawPID.Current;
//					Set_cv_flag(0);
//				}
//				Set_cv_flag(0);
				if(KM[2].KMFlag == 0 && KM[1].KMFlag == 0 && !Get_cv_flag())
				{
					CarFramePID.SetTarget(0);
					StrategicMode = 0;
				}
			}

			switch(ptrmsg->key)
			{
				/**
				 * USEFOLLOWLINE
				 */
				{
					// case KEY_PRESSED_OFFSET_R :bumark=1;break;
					// case KEY_PRESSED_OFFSET_F :bumark=0;break;
				}
				/**
				 * USECV
				 */
				{

//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_W: AxicY = 1024 + Parameter_User.data.SPEED1; AxicX = 1024 ;Set_cv_flag(1);break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_S: AxicY = 1024 - Parameter_User.data.SPEED1; AxicX = 1024 ;Set_cv_flag(1);break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_A: AxicX = 1024 - Parameter_User.data.SPEED1; AxicY = 1024 ;Set_cv_flag(1);break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_D: AxicX = 1024 + Parameter_User.data.SPEED1; AxicY = 1024 ;Set_cv_flag(1);break;

//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: AxicY = 1024 + Parameter_User.data.SPEED1; AxicX = 1024 - Parameter_User.data.SPEED1; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: AxicY = 1024 + Parameter_User.data.SPEED1; AxicX = 1024 + Parameter_User.data.SPEED1; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: AxicY = 1024 - Parameter_User.data.SPEED1; AxicX = 1024 - Parameter_User.data.SPEED1; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: AxicY = 1024 - Parameter_User.data.SPEED1; AxicX = 1024 + Parameter_User.data.SPEED1; Set_cv_flag(1); break;

//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 + Parameter_User.data.SPEED2; AxicX = 1024; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 - Parameter_User.data.SPEED2; AxicX = 1024; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicX = 1024 - Parameter_User.data.SPEED2; AxicY = 1024; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicX = 1024 + Parameter_User.data.SPEED2; AxicY = 1024; Set_cv_flag(1); break;

//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 + Parameter_User.data.SPEED2; AxicX = 1024 - Parameter_User.data.SPEED2; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 + Parameter_User.data.SPEED2; AxicX = 1024 + Parameter_User.data.SPEED2; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 - Parameter_User.data.SPEED2; AxicX = 1024 - Parameter_User.data.SPEED2; Set_cv_flag(1); break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; AxicY = 1024 - Parameter_User.data.SPEED2; AxicX = 1024 + Parameter_User.data.SPEED2; Set_cv_flag(1); break;

//					case KEY_PRESSED_OFFSET_Q : AxicX = 1024,AxicY = 1024;Set_cv_flag(1);break;
//					case KEY_PRESSED_OFFSET_Q + KEY_PRESSED_OFFSET_SHIFT:YawWeightTemp = Parameter_User.data.YAWWEIGHT[1];AxicX = 1024,AxicY = 1024;Set_cv_flag(1);break;

				}
				/**
				 * NOUSECV
				 */
				{
					case KEY_PRESSED_OFFSET_Q :if(KM[0].KMFlag == 0)//cv
												{
													if(StrategicMode != 4)
														StrategicMode = 4;
													Set_cv_flag(!Get_cv_flag());
													if(Get_cv_flag() == 0)
													{
														StrategicMode = 0;
													}
//													else
//														StrategicMode = 4;
//													Set_cv_flag(1);
//													StrategicMode = 4;
													KM[0].KMFlag = 1;
												}
												break;
					case KEY_PRESSED_OFFSET_C :if(KM[1].KMFlag == 0)//底盘不动
												{
													StrategicMode = 2;
													KM[1].KMFlag = 1;
												}
												break;
					case KEY_PRESSED_OFFSET_V :if(KM[2].KMFlag == 0)//底盘抖动
												{
													StrategicMode = 3;
													KM[2].KMFlag = 1;
												}
												break;
					// case KEY_PRESSED_OFFSET_Q :Set_cv_flag(1);break;
					// case KEY_PRESSED_OFFSET_E :Set_cv_flag(0);break;
					case KEY_PRESSED_OFFSET_W: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024;break;
					case KEY_PRESSED_OFFSET_S: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024;break;
					case KEY_PRESSED_OFFSET_A: TempAxicX = 1024 - Parameter_User.data.SPEED1; TempAxicY = 1024;break;
					case KEY_PRESSED_OFFSET_D: TempAxicX = 1024 + Parameter_User.data.SPEED1; TempAxicY = 1024;break;

					case KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024 - Parameter_User.data.SPEED1; break;
					case KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024 + Parameter_User.data.SPEED1; break;
					case KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024 - Parameter_User.data.SPEED1; break;
					case KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024 + Parameter_User.data.SPEED1; break;

					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicX = 1024 - Parameter_User.data.SPEED2; TempAxicY = 1024; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicX = 1024 + Parameter_User.data.SPEED2; TempAxicY = 1024; break;

					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024 - Parameter_User.data.SPEED2; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024 + Parameter_User.data.SPEED2; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024 - Parameter_User.data.SPEED2; break;
					case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024 + Parameter_User.data.SPEED2; break;

					case KEY_PRESSED_OFFSET_SHIFT:YawWeightTemp = Parameter_User.data.YAWWEIGHT[1];TempAxicX = 1024,TempAxicY = 1024;break;
					// case KEY_PRESSED_OFFSET_W: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024; Set_cv_flag(0);break;
					// case KEY_PRESSED_OFFSET_S: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024; Set_cv_flag(0);break;
					// case KEY_PRESSED_OFFSET_A: TempAxicX = 1024 - Parameter_User.data.SPEED1; TempAxicY = 1024; Set_cv_flag(0);break;
					// case KEY_PRESSED_OFFSET_D: TempAxicX = 1024 + Parameter_User.data.SPEED1; TempAxicY = 1024; Set_cv_flag(0);break;

					// case KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024 - Parameter_User.data.SPEED1; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: TempAxicY = 1024 + Parameter_User.data.SPEED1; TempAxicX = 1024 + Parameter_User.data.SPEED1; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024 - Parameter_User.data.SPEED1; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: TempAxicY = 1024 - Parameter_User.data.SPEED1; TempAxicX = 1024 + Parameter_User.data.SPEED1; Set_cv_flag(0); break;

					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicX = 1024 - Parameter_User.data.SPEED2; TempAxicY = 1024; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicX = 1024 + Parameter_User.data.SPEED2; TempAxicY = 1024; Set_cv_flag(0); break;

					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024 - Parameter_User.data.SPEED2; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 + Parameter_User.data.SPEED2; TempAxicX = 1024 + Parameter_User.data.SPEED2; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_A: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024 - Parameter_User.data.SPEED2; Set_cv_flag(0); break;
					// case KEY_PRESSED_OFFSET_SHIFT + KEY_PRESSED_OFFSET_S + KEY_PRESSED_OFFSET_D: YawWeightTemp = Parameter_User.data.YAWWEIGHT[1]; TempAxicY = 1024 - Parameter_User.data.SPEED2; TempAxicX = 1024 + Parameter_User.data.SPEED2; Set_cv_flag(0); break;

					// case KEY_PRESSED_OFFSET_SHIFT:YawWeightTemp = Parameter_User.data.YAWWEIGHT[1];TempAxicX = 1024,TempAxicY = 1024;Set_cv_flag(0);break;
				}
				default: TempAxicX = 1024,TempAxicY = 1024;break;//bumark=0;

			}
//==========================================================
//处理按键产生的阶跃信号
			AxicX += (TempAxicX - AxicX)/1.2;
			if(myabs(AxicX - 1024)<5)
				AxicX = 1024;
			else if(AxicX - 1024>650) //1684
				AxicX = 1684;
			else if(AxicX - 1024<-650) //364
				AxicX = 364;

			AxicY += (TempAxicY - AxicY)/1.2;
			if(myabs(AxicY - 1024)<5)
				AxicY = 1024;
			else if(AxicY - 1024>650) //1684
				AxicY = 1684;
			else if(AxicY - 1024<-650) //364
				AxicY = 364;
//			u1_printf("%d\t%d\r\n",AxicX,AxicY);
//==========================================================

			TempPitch = PitchPID.Target;
			TempPitch += ptrmsg->y * Parameter_User.data.PITCHWEIGHT;
			PitchPID.SetTarget(TempPitch);

//			if(myabs(CarFramePID.Current)<90*22.7556f)
			{
				if(!Get_cv_flag())
				{
					TempYaw = YawPID.Target;
					
						
					TempYaw -= ptrmsg->x * YawWeightTemp;
					if(CarFramePID.Current/22.7556f >=85)
					{
						if(YawPID.Target < TempYaw)
							TempYaw = YawPID.Target;
					}
					else if(CarFramePID.Current/22.7556f <=-85)
					{
						if(YawPID.Target > TempYaw)
							TempYaw = YawPID.Target;
					}
//					u1_printf("%d\t%d\r\n",((int)((TempYaw - CarFramePID.Current)/22.7556f))%360,(int)(CarFramePID.Current/22.7556f));
					
//					if(myabs(CarFramePID.Current)<90*22.7556f)
					YawPID.SetTarget(TempYaw);
				}
				else
					AxicZ = ptrmsg->x * 100;
			}
	

			if(ptrmsg->press_r == 1 && mouse_r_flag == 0)
			{
				mouse_r_flag = 1;
				os_evt_set (BIT_0, HandleDR16_SxValue);
			}
			else if(ptrmsg->press_r == 0)
				mouse_r_flag = 0;

			// if(ptrmsg->press_l == 1 && mouse_r_flag)
			// {
			// 	mouse_r_flag = 0;
			// 	os_evt_set (BIT_1, HandleDR16_SxValue);
			// }
			// else if(ptrmsg->press_l == 0)
			// 	mouse_r_flag = 1;

			if(ptrmsg->press_l == 1 && mouse_l_flag == 0 )
			{
	//			count++;
	//			if(count == 2)
				{
					os_evt_set (BIT_4, HandleDR16_SxValue);
				}
				mouse_l_flag = 1;
			}
			else if( ptrmsg->press_l == 0 && mouse_l_flag == 1)
			{
	//			count = 0;
				os_evt_set (BIT_5, HandleDR16_SxValue);
				mouse_l_flag = 0;
			}

	}



////////////////////////////////////////////////////////////////////////////
//////////////////////////键位控制//////////////////////////////////////////

//s1
	if(ptrmsg->s1 == 1 && s1_flag)
	{
		s1_flag = 0;
		os_evt_set (BIT_0, HandleDR16_SxValue);
	}
	else if(ptrmsg->s1 == 2 && s1_flag)
	{
		s1_flag = 0;
		os_evt_set (BIT_1, HandleDR16_SxValue);
	}

	else if(ptrmsg->s1 == 3 )
		s1_flag = 1;

//s2
	if(ptrmsg->s2 == 1 && s2_flag)
	{
		s2_flag = 0;
//		os_evt_set (BIT_7, HandleDR16_SxValue);
		
	}
	else if(ptrmsg->s2 == 2 && s2_flag)
	{
		s2_flag = 0;
		os_evt_set (BIT_3, HandleDR16_SxValue);

	}

	else if(ptrmsg->s2 == 3 )
		s2_flag = 1;

//s键的其他处理
	if(ptrmsg->s1 == 2 )
	{
		count++;
		if(count == 50)
		{
			constantly = 1;
			os_evt_set (BIT_4, HandleDR16_SxValue);
		}
	}
	else if(constantly)
	{
		constantly = 0;

		os_evt_set (BIT_5, HandleDR16_SxValue);
	}
	else
		count = 0;
//	u1_printf("ch0:%d\r\n",ptrmsg->ch0);
//	u1_printf("ch0:%d\tch1:%d\tch2:%d\tch3:%d\t\r\n",ptrmsg->ch0,ptrmsg->ch1,ptrmsg->ch2,ptrmsg->ch3);
//	printf("key:%d\tx:%d\ty:%d\tz:%d\tpress_l:%d\tpress_r:%d\t\r\n",ptrmsg->key,ptrmsg->x ,ptrmsg->y,ptrmsg->z,ptrmsg->press_l,ptrmsg->press_r);
  if(ptrmsg->s2 == 2 )
	{
		run_flag = 1;
	}
	else
	{
		run_flag = 0;
	}
}
short Get_AxicX(void)
{
	return AxicX;
}
short Get_AxicY(void)
{
	return AxicY;
}
short Get_AxicZ(void)
{
	return AxicZ;
}

/**
 * [DR16_KM 相当于按键消抖，处理键盘不定时返回0的情况]
 * @param ptrmsg             [用来获取键盘数据的指针]
 * @param KEY_PRESSED_OFFSET [键盘对应按键的OFFSET值]
 * @param _pKEY              [相对应的结构体]
 * KMFlag = 0时按键弹起 KMFlag = 1按键长按
 */
void DR16_KM(RC_Value *ptrmsg,uint16_t KEY_PRESSED_OFFSET,KM_KEY * _pKEY)
{
	if(ptrmsg->key == KEY_PRESSED_OFFSET)
			{
				_pKEY->KMcounter++;
				_pKEY->KMcounter++;
				if(_pKEY->KMcounter>=10)
					_pKEY->KMcounter = 10;
			}
			else
			{
				_pKEY->KMcounter--;
				if(_pKEY->KMcounter<=0)
					_pKEY->KMcounter = 0;
			}
			if(_pKEY->KMcounter == 0 && _pKEY->KMFlag == 1)
				_pKEY->KMFlag = 0;
}
u8 Run_Start(void)
{
	return run_flag;
}
