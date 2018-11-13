#include "includes.h"
#include "cpid.h"
#include "filter.h"
#include "SetParameter.h"
#include "location.h"
#include "bsp_stmflash.h"
#include "Kalman.h"
#include "add_bullet.h"


u16 DUOJI_OPEN = 700,DUOJI_CLOSE=1585;

FUZZYPID PitchPID(0,0,0,0,-6000, +6000, -5000, +5000, -5000, +5000,0,0,0,\
		 40.0f,20.0f,0.0f,\
		 100.00f,90.00f,95.00f,
		 500.0f,500.0f,600.0f,\
		 2.2,2.5,1.8);
FUZZYPID YawPID(8.5,0.1,1.1,0,-4500, +4500, -500, +500, -4500, +4500,-1.701411834604E+38,1.701411834604E+38,0,\
		 120.0f,60.0f,0.0f,\
		 80.0f,70.0f,75.00f,\
		 0.0f,0.0f,0.0f,\
		 1.2,4.20,2.80);

//CPID YawSpeedPID(0.1,50.0,0.1,0,-6000, +6000, -3000, +3000, -5000, +5000,-500,+500,0);

//FUZZYSPID CarFramePID(8.5,0.1,1.1,0,-20000, +20000, -3000, +3000, -5000, +5000,-1.701411834604E+38,1.701411834604E+38,60,\
//		 300.0f,200.0f,100.0f,\
//		 10.05f,10.05f,10.05f,\
//		 0.0f,0.0f,0.0f,\
//		 -0.0069f,-0.0069f,-0.0069f);
SCPID CarFramePID(0,0,0,0,-20000, +20000, -3000, +3000, -5000, +5000,-1.701411834604E+38,1.701411834604E+38,60);//PID������void bsp_ParaInit()������
//SCPID TurnplatePID(25.20,0.0,0.001,0.0,-50, +50, -50, +50, -50, +50,-50,50,1.5,0);

//SCPID TurnplatePID(5.0,0.0,0.0,0,-1000, +1000, -500, +500, -1000, +1000,-1.701411834604E+38,1.701411834604E+38,0);
FUZZYSPID TurnplatePID(5.0,0.0,0.0001,0,-1000, +1000, -500, +500, -1000, +1000,-1.701411834604E+38,1.701411834604E+38,0,\
		 400.0f,300.0f,200.0f,\
		 15.0f,7.0f,3.0f,\
		 0.0f,0.0f,0.0f,\
		 0.0f,0.3f,0.05f);

static u8  arrive_position;
static CVStruct cvstruct;
/**
 *����������
 */
static void AppTaskCreate (void);
static void AppObjCreate (void);

__task void TaskStart(void);
__task void DEBUG_DATA(void);
__task void DR16_DATA_Rec(void);
__task void CAN2_DATA0_Rec(void);
__task void CAN2_DATA1_Rec(void);
__task void CAN1_DATA0_Rec(void);
__task void DisconnectCheck(void);
__task void EXTITask(void);
__task void PitchYaw_Aguest(void);
__task void CarFrame_Aguest(void);
__task void PCData_Aguest(void);
__task void Turnplate_Aguest(void);
__task void DR16_SxValue(void);
__task void Boot_Sequence(void);

/***
 *����ջ
 */
static uint64_t TaskStartStk[512/8];
static uint64_t DEBUG_DATAStk[512/8];
static uint64_t DR16_DATA_RecStk[512/8];
static uint64_t CAN2_DATA0_RecStk[512/8];
static uint64_t CAN2_DATA1_RecStk[512/8];
static uint64_t CAN1_DATA0_RecStk[512/8];
static uint64_t DisconnectCheckStk[512/8];
static uint64_t EXTITaskStk[512/8];
static uint64_t PitchYaw_AguestStk[512/8];
static uint64_t CarFrame_AguestStk[512/8];
static uint64_t PCData_AguestStk[512/8];
static uint64_t Turnplate_AguestStk[512/8];
static uint64_t DR16_SxValueStk[512/8];
static uint64_t Boot_SequenceStk[512/8];


/**
 *������
 */
OS_TID HandleDEBUG_DATA       = NULL;
OS_TID HandleDR16_DATA_Rec    = NULL;
OS_TID HandleCAN2_DATA0_Rec   = NULL;
OS_TID HandleCAN2_DATA1_Rec   = NULL;
OS_TID HandleCAN1_DATA0_Rec   = NULL;
OS_TID HandleDisconnectCheck  = NULL;
OS_TID HandleEXTITask         = NULL;
OS_TID HandlePitchYaw_Aguest  = NULL;
OS_TID HandleCarFrame_Aguest  = NULL;
OS_TID HandlePCData_Aguest  = NULL;
OS_TID HandleTurnplate_Aguest = NULL;
OS_TID HandleDR16_SxValue     = NULL;
OS_TID HandleBoot_Sequence    = NULL;

u8 timer200msflag = 0,positionreachflag = 0,shootedflag = 0,shootonce = 0,shootcons = 0,hurt_id=0x06;
 
static u8 hurt_origin;
/**
 * [TaskStart ��ʼ�����½��������Ϣ����ͬʱ��������̶�ʱ����������������⴮�ڵ�FIFO�Ƿ�Ϊ��]
 */
__task void TaskStart(void)
{
	AppTaskCreate();/* �������� */
	AppObjCreate();/* ��������ͨ�Ż��ƺ���ʱ����Ҫ�ĳ�ʼ������ */
	while(1)
	{
		CAN_FIFO_IRQ();		//����FIFO������CAN���ݣ���Ҫ��ʱ��ѯFIFO�Ƿ�������Ҫ��
//		bsp_LedToggle(1);	//ָʾϵͳ������״̬��
		os_dly_wait(1);
	}
}


/**
 * [DEBUG_DATA ����������λ���ĵ�������]
 */
//extern float errorC;
//extern float yaw_angle_6050;
//extern union ParameterUnion Parameter;

extern MOTORSTRUCT MotorMassage;
extern LOCATIONSTRUCT car_location;
extern float errorC;
extern int camera_angle;
extern float body_X,body_Y,body_C,YAW_Angle,YAW_Angle_Zero;
float tmee,tmeee;
extern float yaw_angle_ADXRS622;
extern short aa;
extern AngleCorrectStruct AngleCorrect;
extern MotorEncoderStruct MotorEncoder;
__task void DEBUG_DATA(void)
{
//	delay_ms(5000);
	while(1)
	{
//		{
//			CVStruct temp;
//			temp.ID = 0xfa;
//			temp.Flag = 0x55;
//			temp.CV_X = 0x1111;
//			temp.CV_Y = 0x2222;
//			temp.RES =  0x3333;
//			CV_ToPC(&temp);
//		}
		u1_printf("X%d\t\tY%d\t\tRES%d\r\n",cvstruct.CV_X,cvstruct.CV_Y,cvstruct.RES);	
//   u1_printf("1234\r\n");
//	u1_printf("%d\t%d\t%d\t%d\r\n",runstart_flag,distance2,distance3,distance4);
//		u1_printf("%f\r\n",Parameter_Operater.data.PITCH_Encoder_Mid);
//		u1_printf("%d\r\n",(short)yaw_angle);
		os_dly_wait (50);
	}
}


/**
 * [DisconnectCheck ���߱�������]
 */

uint8_t PIDBegin_DR16=0,PIDBegin_CAN=0,CAN1_Angle=0,CAN2_PitchMotor=0,CAN2_YawMotor=0,
        ExtiFlag=1,Refree_Message=0,CAN1_Ultrasonic=0,Can2_NewBoard=0;
__task void DisconnectCheck(void)
{

	while(1)
	{
		if (os_evt_wait_or (BIT_0, 50) == OS_R_TMO)
		{
			PIDBegin_DR16 = 0;//���ȴ���ʱ���ñ�־��Ϊ0
			ESC_STOP();
//			os_evt_set (BIT_6, HandleDisconnectCheck);//���߱���
			u1_printf("PIDBegin_DR16 = 0\r\n");
		}
		else PIDBegin_DR16 = 1;
		
    if (os_evt_wait_or (BIT_1, 50) == OS_R_TMO) {}
		else PIDBegin_CAN = 1;
			
		if (os_evt_wait_or (BIT_2, 50) == OS_R_TMO)
		{
			CAN1_Angle = 0;//���ȴ���ʱ���ñ�־��Ϊ0
			u1_printf("CAN1_Angle = 0\r\n");
		}
		else CAN1_Angle = 1;

		if (os_evt_wait_or (BIT_3, 50) == OS_R_TMO)
		{
			CAN2_PitchMotor = 0;//���ȴ���ʱ���ñ�־��Ϊ0
			u1_printf("CAN2_PitchMotor = 0\r\n");
		}
		else CAN2_PitchMotor = 1;
		if (os_evt_wait_or (BIT_4, 50) == OS_R_TMO)
		{
			CAN2_YawMotor = 0;//���ȴ���ʱ���ñ�־��Ϊ0
			u1_printf("CAN2_YawMotor = 0\r\n");

		}
		else CAN2_YawMotor = 1;
		
		if (os_evt_wait_or (BIT_5, 50) == OS_R_TMO)
		{
			Refree_Message = 0;//���ȴ���ʱ���ñ�־��Ϊ0
//			Motor_Power(0x000f,(u16)((83.2 - 2.5)/GameInfo.realChassisOutV*248.2));
			u1_printf("Refree_Message = 0\r\n");

		}
		else Refree_Message = 1;
		
	  if (os_evt_wait_or (BIT_6, 50) == OS_R_TMO)
		{
			CAN1_Ultrasonic = 0;//���ȴ���ʱ���ñ�־��Ϊ0
//			Motor_Power(0x000f,(u16)((83.2 - 2.5)/GameInfo.realChassisOutV*248.2));
			u1_printf("CAN1_Ultrasonic = 0\r\n");

		}
		else CAN1_Ultrasonic = 1;
		
		if (os_evt_wait_or (BIT_7, 50) == OS_R_TMO)
		{
			Can2_NewBoard = 0;//���ȴ���ʱ���ñ�־��Ϊ0
//			Motor_Power(0x000f,(u16)((83.2 - 2.5)/GameInfo.realChassisOutV*248.2));
//			u1_printf("Can2_NewBoard = 0\r\n");

		}
		else Can2_NewBoard = 1;
		
		

	}
}

/**
 * [EXTITask ���߱�������]
 */
void ttp(void)
{
	Location(&MotorMassage);
}

extern float YAW_Angle_Zero;
__task void EXTITask(void)
{
	while(1)
	{
		delay_ms(50);
		if(Refree_Message==0)Motor_Power(0x000f,(u16)((83.2 - 2.5)/24*248.2));
		if(!OVERPOWER_BUTTON )Send_OverPowerFlag(0x01);
	}
		
}

/**
 * [DR16_DATA_Rec DR16���ݴ���ͨ����������жϵ�����]
 */
 __task void DR16_DATA_Rec(void)
 {
 	RC_Value *ptrmsg;
 	while(1)
 	{

 		// if(os_mbx_wait(dr16_mailbox, (void **)&ptrmsg,200) != OS_R_TMO)//���յ�ң��������
 		if(DR16_receive(&ptrmsg, 200) != OS_R_TMO )
 		{
 			os_evt_set (BIT_0, HandleDisconnectCheck);//���߱���
 			os_dr16(ptrmsg);
 		}

 	}
 }


// /**
// * [CAN1_DATA0_Rec CAN1���ݵĴ���]
// */
//__task void CAN1_DATA0_Rec(void)
//{
//	u8 shootcnt = 0;
//	CanRxMsg *msg_rece;
////	can_filter_Mask_config(CanFilter_5|CanFifo_0|Can_STDID|Can_DataType,0x110,0x3ff);
//	can_filter_Mask_config(CanFilter_7|CanFifo_0|Can_STDID|Can_DataType,0x502,0x3ff);
//	can_filter_Mask_config(CanFilter_1|CanFifo_0|Can_STDID|Can_DataType,0x402,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
//	can_filter_Mask_config(CanFilter_2|CanFifo_0|Can_STDID|Can_DataType,0x88,0x3ff);
//	can_filter_Mask_config(CanFilter_3|CanFifo_0|Can_STDID|Can_DataType,0x503,0x3ff);
//	can_filter_Mask_config(CanFilter_4|CanFifo_0|Can_STDID|Can_DataType,0x504,0x3ff);
//	can_nvic_config(CAN1_R0);
//	while(1)
//	{
//		if (CAN_receive (1, 0,&msg_rece,200) != OS_R_TMO)//���յ�CAN����
//		{
//			if(msg_rece->StdId == 0x502)
//			{
//				if(timer200msflag == 1)
//				{
//						shootedflag = 1;
//				}
//				shootcnt++;
//				u1_printf("%d\t%d\t%d\r\n",shootcnt,(int)(((tRealShootDataBase*)(msg_rece->Data))->realShootSpeed*100),(int)(((tRealShootDataBase*)(msg_rece->Data))->realShootFreq)*100);

////				u1_printf("%d\t%d\r\n",(int)((tRealShootData*)(msg_rece))->realBulletShootSpeed,(int)((tRealShootData*)(msg_rece))->realBulletShootFreq);
////				add_bullet(msg_rece);
//			}
//				if(msg_rece->StdId == 0x402)
//			{
//				os_evt_set (BIT_2, HandleDisconnectCheck);
//				if(spin_flag)                  Run_Spin(msg_rece);
//				else                           Run_Correct(msg_rece);
////			 os_evt_set (BIT_2, HandleDisconnectCheck);//���߱���
////        u1_printf("a");      
////				Run_Correct(msg_rece);
//			}
//			if(msg_rece->StdId == 0x88)
//			{
//       os_evt_set (BIT_6, HandleDisconnectCheck);				
//      if(ADDBULLET_BUTTON){Run_Random_v2(msg_rece);}
//			else
//			{
//			 if((!spin_flag))
//			 {
//				Run_Random_v2(msg_rece);
//				Run_Protect();				
//			 } 
//			 if((rand_flag==9)&&(across_times>4)) {spin_flag=1;across_times=0;last_yawangle=yaw_angle;}
//		  }
////			 u1_printf("b");         
////			 Run_Protect();
//	    }	
//     	if(msg_rece->StdId == 0x503)
//			{
//         if(msg_rece->Data[0]==0x01){runstart_flag=1;ADDBULLET_BUTTON=0;}
////				 if(msg_rece->Data[1]==0x01)Utc_Answer=1;
//			}
//			if(msg_rece->StdId == 0x504)
//			{
//         os_evt_set(BIT_5, HandleDisconnectCheck);
//			}
//		}
//	}

//}
/**
 * [CAN1_DATA0_Rec CAN1���ݵĴ���]
 */
__task void CAN1_DATA0_Rec(void)
{
	u8 shootcnt = 0;
	CanRxMsg *msg_rece;
//	can_filter_Mask_config(CanFilter_5|CanFifo_0|Can_STDID|Can_DataType,0x110,0x3ff);
	can_filter_Mask_config(CanFilter_7|CanFifo_0|Can_STDID|Can_DataType,0x502,0x3ff);
	can_filter_Mask_config(CanFilter_1|CanFifo_0|Can_STDID|Can_DataType,0x402,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_filter_Mask_config(CanFilter_2|CanFifo_0|Can_STDID|Can_DataType,0x88,0x3ff);
	can_filter_Mask_config(CanFilter_3|CanFifo_0|Can_STDID|Can_DataType,0x503,0x3ff);
	can_filter_Mask_config(CanFilter_4|CanFifo_0|Can_STDID|Can_DataType,0x504,0x3ff);
	can_filter_Mask_config(CanFilter_5|CanFifo_0|Can_STDID|Can_DataType,0x501,0x3ff);
	can_nvic_config(CAN1_R0);
	while(1)
	{
		if (CAN_receive (1, 0,&msg_rece,200) != OS_R_TMO)//���յ�CAN����
		{
//			u1_printf("1234");	
			if(msg_rece->StdId == 0x502)
			{
				if(timer200msflag == 1)
				{
						shootedflag = 1;
				}
				shootcnt++;
//				u1_printf("%d\t%d\t%d\r\n",shootcnt,(int)(((tRealShootDataBase*)(msg_rece->Data))->realShootSpeed*100),(int)(((tRealShootDataBase*)(msg_rece->Data))->realShootFreq)*100);

//				u1_printf("%d\t%d\r\n",(int)((tRealShootData*)(msg_rece))->realBulletShootSpeed,(int)((tRealShootData*)(msg_rece))->realBulletShootFreq);
//				add_bullet(msg_rece);
			}
				if(msg_rece->StdId == 0x402)
			{
				os_evt_set (BIT_2, HandleDisconnectCheck);
//				if(spin_flag)                  Run_Spin(msg_rece);
//				else                           Run_Correct(msg_rece);
				Run_Spin(msg_rece);
//				yaw_angle
//			 os_evt_set (BIT_2, HandleDisconnectCheck);//���߱���
//        u1_printf("a");      
//				Run_Correct(msg_rece);
			}
			if(msg_rece->StdId == 0x88)
			{
       os_evt_set (BIT_6, HandleDisconnectCheck);	
        Moving_Gyo(msg_rece);				
//      if(ADDBULLET_BUTTON){Run_Random_v2(msg_rece);}
//			else
//			{
//			 if((!spin_flag))
//			 {
//				Run_Random_v2(msg_rece);
//				Run_Protect();				
//			 } 
//			 if((rand_flag==9)&&(across_times>4)) {spin_flag=1;across_times=0;last_yawangle=yaw_angle;}
//		  }
//				Run_By_Surround(msg_rece);
//				Moving_Gyo(msg_rece);
//			 u1_printf("b");         
//			 Run_Protect();
	    }	
     	if(msg_rece->StdId == 0x503)
			{
				 os_evt_set(BIT_8, HandleDisconnectCheck);
//         if(msg_rece->Data[1]==0x0){runstart_flag=1;ADDBULLET_BUTTON=0;}
//				  u1_printf("%d\r\n",msg_rece->Data[1]);
//				 hurt_origin=msg_rece->Data[1];
		         runstart_flag=msg_rece->Data[0];
//				 u1_printf("12334hurt_origin:%d\t%d\r\n",runstart_flag,hurt_origin);
//				 if(msg_rece->Data[1]==0x01)Utc_Answer=1;
			}
			if(msg_rece->StdId == 0x504)
			{
         os_evt_set(BIT_5, HandleDisconnectCheck);
//				 u1_printf("%d\r\n",msg_rece->Data[1]);
//			if(msg_rece->Data[1]==0x00)    OVERPOWER_BUTTON=1;
//			else (msg_rece->Data[1]==0x01) OVERPOWER_BUTTON=0;
//				 if(msg_rece->Data[0]==0x01)
			}
			if(msg_rece->StdId == 0x501)
			{
//        cvstruct.ID=(int8_t)msg_rece->Data[0];
				cvstruct.Flag=(int8_t)msg_rece->Data[1];
				cvstruct.CV_X=(int16_t)msg_rece->Data[3]<<8|msg_rece->Data[2];
				cvstruct.CV_Y=(int16_t)msg_rece->Data[5]<<8|msg_rece->Data[4];
				cvstruct.RES=(int16_t)msg_rece->Data[7]<<8|msg_rece->Data[6];
				Base_AutoAttack(&cvstruct);
//				u1_printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n",cvstruct.ID,cvstruct.Flag,cvstruct.CV_X,cvstruct.CV_Y,cvstruct.RES);							
			}
		}
	}

}

/**
 * [CAN2_DATA0_Rec CAN2���ݵĴ���]
 */
__task void CAN2_DATA0_Rec(void)
{
	CanRxMsg *msg_rece;
	can_filter_Mask_config(CanFilter_14|CanFifo_0|Can_STDID|Can_DataType,0x205,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_filter_Mask_config(CanFilter_15|CanFifo_0|Can_STDID|Can_DataType,0x206,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_nvic_config(CAN2_R0);
	while(1)
	{
		if (CAN_receive (2, 0,&msg_rece,200) != OS_R_TMO)//���յ�CAN����
		{
//			u1_printf("%a");
			if(msg_rece->StdId == 0x206){
				os_evt_set (BIT_3, HandleDisconnectCheck);//���߱���
				PitchMotor(msg_rece);
			}
			if(msg_rece->StdId == 0x205){
				os_evt_set (BIT_4, HandleDisconnectCheck);//���߱���
				YawMotor(msg_rece);
			}
		}
	}
}


/**
 * [CAN2_DATA1_Rec CAN2���ݵĴ���]
 */
__task void CAN2_DATA1_Rec(void)
{
	CanRxMsg *msg_rece;
	can_filter_Mask_config(CanFilter_16|CanFifo_1|Can_STDID|Can_DataType,0x401,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_filter_Mask_config(CanFilter_17|CanFifo_1|Can_STDID|Can_DataType,0x402,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_filter_Mask_config(CanFilter_18|CanFifo_1|Can_STDID|Can_DataType,0x403,0x3ff);//ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|
	can_filter_Mask_config(CanFilter_19|CanFifo_1|Can_STDID|Can_DataType,0x501,0x3ff);
	can_nvic_config(CAN2_R1);
	while(1)
	{
		if (CAN_receive (2, 1,&msg_rece,200) != OS_R_TMO)//���յ�CAN����
		{
			if(msg_rece->StdId == 0x401){

//				os_evt_set (BIT_2, HandleDisconnectCheck);//���߱���
				YawAngle(msg_rece);
			}
			if(msg_rece->StdId == 0x402){
//				os_evt_set (BIT_2, HandleDisconnectCheck);//���߱���
				YawAngle_6050(msg_rece);
			}
			if(msg_rece->StdId == 0x403){
//				os_evt_set (BIT_2, HandleDisconnectCheck);//���߱���
				YawAngle_ADXRS622(msg_rece);
			}
			if(msg_rece->StdId == 0x501){
				os_evt_set (BIT_7, HandleDisconnectCheck);//���߱���
        New_Board(msg_rece);
			}
		}
	}
}



void pitchyaw(void)
{
	if(PIDBegin_CAN &&  CAN2_YawMotor && CAN2_PitchMotor && CAN1_Ultrasonic && CAN1_Angle && !GetParaFlag() && !STILL_BUTTON)
	{
		PitchPID.AdjustPID();//Pitch���PID����
		YawPID.AdjustPID();//Yaw���PID����
		if(PIDBegin_DR16)
		{
			Motor_YawPitch(-YawPID.Out,-(PitchPID.Out));//���Pitch��Yaw�ĵ��������
		}
		else
		{
		  Motor_YawPitch(0,0);
		}
	}
	else if(PIDBegin_CAN)//PIDBegin_CAN=1ʱ����Stop,��Ȼ���RMDS-103����������Ϊ����ֹͣ�źŶ���ʼ�����ɹ�
	{
		Motor_YawPitch(0,0);
	}
	bsp_LedToggle(1);
}

extern FUZZYPID CVPID,CVPitchPID;
/**
 * [PitchYaw_Aguest PItCH��YAW������ĵ�PID����]
 */
__task void PitchYaw_Aguest(void)
{
	TIM1_timer(200,(void *)pitchyaw);
	os_itv_set(1);
	while(1)
	{
		{
//			short gx;
			u8 buf[6],res;
			res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
			if(res==0)
			{
//				PitchPID.SetOmega((float)Middle_filter(filter_buf[0],&index[0],5,(short)(((u16)buf[0]<<8)|buf[1])),1);
//				YawPID.SetOmega((float)Middle_filter(filter_buf[1],&index[1],5,(short)(((u16)buf[4]<<8)|buf[5])),0);
				PitchPID.SetOmega((float)(short)(((u16)buf[2]<<8)|buf[3]),1);
				YawPID.SetOmega( (float)(short)(((u16)buf[4]<<8)|buf[5]),0);
				AngleCorrect.w1 = YawPID.Omega;
//				PitchPID.Omega =-(float)(short)(((u16)buf[0]<<8)|buf[1]);
//				YawPID.Omega =(float)(short)(((u16)buf[4]<<8)|buf[5]);
//				u1_printf("%d\r\n",gx);
			}
			CVPID.AdjustPID();
			CVPitchPID.AdjustPID();

		}
		os_itv_wait ();

	}
}

/**
 * [CarFrame_Aguest ���̵���Ŀ���]
 */
u8 dddflag = 0;
extern float speed;
__task void CarFrame_Aguest(void)
{
   u16 X,Y,Z; 
//	delay_ms(2000);
	os_itv_set(1);	/* �����ӳ����� */
	while(1)
	{
//		StrategicMode = 2;
//		FollowMode = 1;
//		if(PIDBegin_DR16 && PIDBegin_CAN  && !GetParaFlag()
		FollowMode = 1;
//		if(PIDBegin_DR16 && PIDBegin_CAN && CAN2_YawAngle && CAN2_YawMotor && CAN2_PitchMotor && !GetParaFlag()
//			&& bumark==0)
		
		if( PIDBegin_DR16&&PIDBegin_CAN &&  !GetParaFlag() && CAN1_Ultrasonic && CAN1_Angle && bumark==0 && !STILL_BUTTON)
		{
//			CarFramePID.SetPID(3.5,0.0,0.0);
//			CarFramePID.AdjustPID();  
			if((Run_Start())&&(runstart_flag))
			{Motor_Aguest_Random(hurt_origin,hurt_id);}
			else if((Run_Start())&&auto_attack_flag)
			{
//				Motor_Aguest(Get_AxicX(),Get_AxicY(),-speed,0);
				Motor_Aguest(1024,1024,-speed,0);//λ�ö�׼
				if(myabs(speed)<100)
					Motor_Aguest(Get_AxicX(),Get_AxicY(),-speed,0);//λ�ÿ���
				 
			//	PitchPID.SetTarget(PitchPID.Current - 0*CodedDiscToAngle);

//	else  Motor_Aguest(1024,1024,-1000,0);			
			}
			else if(!(Run_Start())) 
			{
			   Motor_Aguest(Get_AxicX(),Get_AxicY(),Get_AxicZ(),0);
			}	
		}
		else if(PIDBegin_CAN && bumark==0)//PIDBegin_CAN=1ʱ����Stop,��Ȼ���RMDS-103����������Ϊ����ֹͣ�źŶ���ʼ�����ɹ�
		{
			Motor_Aguest(1024,1024,0,0);
		}
		os_itv_wait ();
	}
}
/**
 * [PCData_Aguest pc��������]
 */
extern short AxicX,AxicY,AxicZ;
__task void PCData_Aguest(void)
{
   u16 X,Y,Z; 
	u8 lastAAFlag=0,AArec_time=0;
//	delay_ms(2000);
	os_itv_set(1);	/* �����ӳ����� */
	while(1)
	{
//		StrategicMode = 2;
//		FollowMode = 1;
//		if(PIDBegin_DR16 && PIDBegin_CAN  && !GetParaFlag()
		FollowMode = 1;
		
		if( PIDBegin_DR16&&PIDBegin_CAN &&  !GetParaFlag() && CAN1_Ultrasonic && CAN1_Angle && bumark==0 && !STILL_BUTTON)
		{
			if(lastAAFlag!=auto_attack_flag){AArec_time++;}
				 if(AArec_time>2)
				 {
					 AArec_time=0;
				   lastAAFlag=auto_attack_flag;
					 ESC_Toggle();
					 Turnplate_motor_Toggle();
				 }
			/**************/

			//jijijijijijijijjjij	 
			//YawPID.SetTarget(YawPID.Current + 45*CodedDiscToAngle);
			//PitchPID.SetTarget(PitchPID.Current + 30	 *CodedDiscToAngle);
			//	 PitchPID.SetTarget(PitchPID.Current+0.0005*cvstruct.CV_Y*CodedDiscToAngle);//
		//��������룬С������̨������ճ���ʱ��ע��ص�Դ��
				 
				 
				 
			/**************/
      if((Run_Start())&&auto_attack_flag)
			{
				//�����
			 
				
						if((distance3>450)&&(distance2>550))
						{
							AxicX = 600;
						  AxicY = 600;
						}
						else
						{
							AxicX = 1024;
							AxicY = 1024;
						}
//  					if(	(PitchPID.Current - cvstruct.CV_Y*0.001*CodedDiscToAngle)
//							PitchPID.SetTarget(PitchPID.Current - cvstruct.CV_Y*0.00628571);//(660/(1750*60))=0.00628571
						
					
//						else if(cvstruct.CV_Y>350)
//							PitchPID.SetTarget(PitchPID.Current + 30	 *CodedDiscToAngle);
			}
		}
		os_itv_wait ();
	}
}

/**
 * [Turnplate_Aguest ���̵���ٶȱջ�]
 */
extern EncoderStuct EncoderData;
__task void Turnplate_Aguest(void)
{
//	uint16_t *temp;
	int last_position;
	uint16_t cnt,cnt1;
	os_itv_set(4);	/* �����ӳ����� */
	Encoder_Start();//���̵������ֵ��ʼ��
	Turnplate_motor_PWM = 0;
	while(1)
	{
//		Turnplate_motor_PWM = 1000;
		last_position = TurnplatePID.Current;
		TurnplatePID.SetCurrent(Encoder_Get_CNT().Position);
		TurnplatePID.AdjustPID();//���̵��PID����
//		if(TurnplatePID.Out<0)
//		{
//			Turnplate_motor_PWM = 0;
////			TurnplatePID.ITerm = 0;
//		}
//		else
//			Turnplate_motor_PWM = TurnplatePID.Out;

		Turnplate_motor(-TurnplatePID.Out);

//λ�û���
//=======================================================================================================================
		if(myabs(TurnplatePID.Current - last_position) < 10 && (TurnplatePID.Target - TurnplatePID.Current) > 1)
		{
			cnt1++;
			if(cnt1 >= 125)
			{
				cnt1 = 125;
			}
		}
		else
		{
			cnt1 = 0;
		}
		if(cnt1 >= 125)
		{
				TurnplatePID.Target-=300;
		}
		
		if(myabs(TurnplatePID.Current - last_position) < 10 && (TurnplatePID.Target - TurnplatePID.Current) < -1)
		{
			cnt++;
			if(cnt >= 125)
			{
				cnt = 125;
			}
		}
		else
		{
			cnt = 0;
		}
		if(cnt >= 125)
		{
				TurnplatePID.Target+=100;
		}
//				if(myabs(TurnplatePID.Current - last_position) < 10 && myabs(TurnplatePID.Current - TurnplatePID.Target) > 2)
//		{
//			cnt++;
//			if(cnt >= 125)
//			{
//				cnt = 125;
//			}
//		}
//		else
//		{
//			cnt = 0;
//		}
//		if(cnt >= 125)
//		{
//			//EncoderData.Position = TurnplatePID.Target;
//			if(TurnplatePID.Target >= TurnplatePID.Current)
//				TurnplatePID.Target-=300;
//			else if(TurnplatePID.Target < TurnplatePID.Current)
//				TurnplatePID.Target+=300;
//		}
//=======================================================================================================================
		if(timer200msflag == 1)
		{
			if(myabs(TurnplatePID.Current - TurnplatePID.Target) < 100)
				positionreachflag = 1;
		}
//		u1_printf("%d\r\n",cnt);
//		Turnplate_motor(1000);
		os_itv_wait ();
//		delay_ms(4);
	}

}


/**
 * [DR16_SxValue DR16�ļ�ֵ����ͨ���¼���]
 */

#define CV_MODE 1
OS_ID  tmr1;
__task void DR16_SxValue(void)
{
	OS_RESULT xResult;

//	delay_ms(2000);
//	ESC_Init();
	ESC_STOP();
//	os_itv_set(1000);	/* �����ӳ����� */
	while(1)
	{
		if (os_evt_wait_or (BIT_ALL, 200) == OS_R_EVT)
		{
			xResult = os_evt_get ();
			switch (xResult)
			{
				case BIT_0:	ESC_Toggle();break;//��ˢ���
				case BIT_1:	if(Get_ESC_Flag()) //Turnplate_motor_ONCE();
							shootonce = 1;
//							if(timer200msflag == 0)
//							{
//								positionreachflag = 0;
//								shootedflag = 0;
//								tmr1 = os_tmr_create (200,1);
//								 if (tmr1 != NULL)
//								 {
//									 timer200msflag = 1;
//									 TurnplatePID.SetTarget(TurnplatePID.Target + 833.33f);
//								 }
//							 }
//							else if(positionreachflag == 1)
//							{
//								if(shootedflag == 0)
//									cnt ++;
//								else
//									cnt =0;
//								if(cnt == 10)
//								{
//									positionreachflag = 0;
//									TurnplatePID.SetTarget(TurnplatePID.Target + 833.33f);
//								}
//							}
							break;//���̵�������Σ�
				case BIT_2://bumark=!bumark;


#if CV_MODE
						if(StrategicMode != 4)
							StrategicMode = 4;
						Set_cv_flag(!Get_cv_flag());
						if(Get_cv_flag() == 0)
						{
							YawPID.Target = YawPID.Current;
							StrategicMode = 0;
						}
#else
						if(FollowMode==0)
							FollowMode = 1;
						else if(FollowMode==1)
							FollowMode = 0;
						Duoji_Toggle();//���
#endif

						break;
				case BIT_4:if(Get_ESC_Flag()) //Turnplate_motor_CONSTANTLY();
					shootcons = 1;
					break;//���̵����������
				case BIT_5://if(Get_ESC_Flag()) Turnplate_motor_STOP();
					shootcons = 0;
					break;//���̵����ֹͣ��
				case BIT_6:ESC_STOP();break;
				default:break;
			}
//			 printf("Events %04x received.\r\n",xResult);
		}

	}
}

/**
 * [Boot_Sequence ����˳��]
 */
tGameInfo GameInfo;
__task void Boot_Sequence(void)
{
	static u8 cnt;
//	kalman_struct AngleKalman;
//	kalman_init(&AngleKalman,0, 5,0.1,30000);
	delay_ms(2500);
	FollowMode = 1;
	PIDBegin_CAN = 1;
	delay_ms(800);
	FollowMode = 0;
//	os_tsk_delete_self();
	while(1)
	{
		//shootedflag = 1;
		if(shootonce == 1 || shootcons == 1)
		{

			if(timer200msflag == 0)
			{
				positionreachflag = 0;
				shootedflag = 0;
				cnt = 0;
				tmr1 = os_tmr_create (167,1);
				 if (tmr1 != NULL)
				 {
					 timer200msflag = 1;
					 TurnplatePID.SetTarget(TurnplatePID.Target + 843.0f  +300);// + 300.0f);
				 }
			 }
			else if(positionreachflag == 1 && cnt == 0)
			{
				cnt = 1;
				TurnplatePID.SetTarget(TurnplatePID.Target - 300.0f);
			}
//			else if(positionreachflag == 1)
//			{
//				if(shootedflag == 0)
//					cnt ++;
//				else
//					cnt =0;
//				if(cnt >= 100)
//				{
//					positionreachflag = 0;
//					TurnplatePID.SetTarget(TurnplatePID.Target + 833.333333f);
//				}
//			}
		}
//		if(timer200msflag!=_timer200msflag || positionreachflag!=_positionreachflag || shootedflag!=_shootedflag || shootonce!=_shootonce)
//			u1_printf("%d\t%d\t%d\t%d\r\n",timer200msflag,positionreachflag,shootedflag,shootonce);
//		_timer200msflag=timer200msflag;
//		_positionreachflag=positionreachflag;
//		_shootedflag=shootedflag;
//		_shootonce=shootonce;

		delay_ms(1);
	}
//	while(1)
//	{
////		u1_printf("V:%d\tI:%d\tP:%d\r\n",(int)(djm.volate*100),(int)(djm.current*100),(int)(djm.current*djm.volate*100));
////		Motor_Power( (u16)(80 - 2.5)/djm.volate*248.2 );
//		if(GameInfo.realChassisOutV != 0)
//		{
////			u1_printf("%d\r\n",(u16)((80 - 2.5)/djm.volate*248.2));
//			Motor_Power((u16)((85.2 - 2.5)/GameInfo.realChassisOutV*248.2));

////			u1_printf("%d\t%d\t\r\n",GameInfo.remainTime,GameInfo.remainLifeValue);
////			ANO_Data2_Send(0xf1,GameInfo.realChassisOutV*GameInfo.realChassisOutA*100,GameInfo.realChassisOutV*100);
//		}
//		else
//			Motor_Power((u16)((85.2 - 2.5)/24*248.2));

//		delay_ms(100);
//	}

	// os_tsk_delete_self();
//	while(1)
//	{
//		{
//			static float tem;
//			static PIDTimer Anglecor;
//			static float Omega1,Omega2,Omega3;
//			if(Anglecor.UpdataTimeStamp()) return;
//			Omega1 = AngleCorrect.w1;
//			tem = kalman_filter(&AngleKalman,Omega1 - YawPID.Omega_Offset);
//			Omega2 = (AngleCorrect.w2 - AngleCorrect._w2);//Anglecor.dt
//			AngleCorrect._w2 = AngleCorrect.w2;
//			Omega3 = (AngleCorrect.w3 - AngleCorrect._w3)/CodedDiscToAngle/Anglecor.dt;//Anglecor.dt
//			AngleCorrect._w3 = AngleCorrect.w3;
//
//			u1_printf("%d\t%d\t%d\r\n",(int)((Omega1 - YawPID.Omega_Offset)*Anglecor.dt *100),(int)(AngleCorrect.w2*100),(int)(tem*Anglecor.dt *100));
////			u1_printf("%d\t%d\t%d\r\n",(int)(Omega1*100),(int)(AngleCorrect.w2*100),(int)(tem*Anglecor.dt));
//		}
//		delay_ms(4);
//	}

}

/**
 * [AppTaskCreate  ��������]
 */
static void AppTaskCreate (void)
{

	HandleDisconnectCheck = os_tsk_create_user(DisconnectCheck,             /* ������ */
	                                      1,                         /* �������ȼ� */
	                                      &DisconnectCheckStk,         /* ����ջ */
	                                      sizeof(DisconnectCheckStk)); /* ����ջ��С����λ�ֽ��� */

	HandleEXTITask = os_tsk_create_user(EXTITask,             /* ������ */
	                                      3,                         /* �������ȼ� */
	                                      &EXTITaskStk,         /* ����ջ */
	                                      sizeof(EXTITaskStk)); /* ����ջ��С����λ�ֽ��� */

	HandleDEBUG_DATA = os_tsk_create_user(DEBUG_DATA,             /* ������ */
	                                      2,                         /* �������ȼ� */
	                                      &DEBUG_DATAStk,         /* ����ջ */
	                                      sizeof(DEBUG_DATAStk)); /* ����ջ��С����λ�ֽ��� */

	HandleDR16_DATA_Rec = os_tsk_create_user(DR16_DATA_Rec,              /* ������ */
	                                   5,                       /* �������ȼ� */
	                                   &DR16_DATA_RecStk,          /* ����ջ */
	                                   sizeof(DR16_DATA_RecStk));  /* ����ջ��С����λ�ֽ��� */

	HandleCAN2_DATA0_Rec = os_tsk_create_user(CAN2_DATA0_Rec,             /* ������ */
	                                      4,                         /* �������ȼ� */
	                                      &CAN2_DATA0_RecStk,         /* ����ջ */
	                                      sizeof(CAN2_DATA0_RecStk)); /* ����ջ��С����λ�ֽ��� */
	HandleCAN2_DATA1_Rec = os_tsk_create_user(CAN2_DATA1_Rec,             /* ������ */
	                                      4,                         /* �������ȼ� */
	                                      &CAN2_DATA1_RecStk,         /* ����ջ */
	                                      sizeof(CAN2_DATA1_RecStk)); /* ����ջ��С����λ�ֽ��� */
	HandleCAN1_DATA0_Rec = os_tsk_create_user(CAN1_DATA0_Rec,             /* ������ */
	                                      5,                         /* �������ȼ� */
	                                      &CAN1_DATA0_RecStk,         /* ����ջ */
	                                      sizeof(CAN1_DATA0_RecStk)); /* ����ջ��С����λ�ֽ��� */

	HandlePitchYaw_Aguest = os_tsk_create_user(PitchYaw_Aguest,              /* ������ */
	                                   3,                       /* �������ȼ� */
	                                   &PitchYaw_AguestStk,          /* ����ջ */
	                                   sizeof(PitchYaw_AguestStk));  /* ����ջ��С����λ�ֽ��� */

	HandleCarFrame_Aguest = os_tsk_create_user(CarFrame_Aguest,              /* ������ */
	                                   6,                       /* �������ȼ� */
	                                   &CarFrame_AguestStk,          /* ����ջ */
	                                   sizeof(CarFrame_AguestStk));  /* ����ջ��С����λ�ֽ��� */
	HandlePCData_Aguest = os_tsk_create_user(PCData_Aguest,              /* ������ */
	                                   6,                       /* �������ȼ� */
	                                   &PCData_AguestStk,          /* ����ջ */
	                                   sizeof(PCData_AguestStk));  /* ����ջ��С����λ�ֽ��� */

	HandleTurnplate_Aguest = os_tsk_create_user(Turnplate_Aguest,              /* ������ */
	                                   6,                       /* �������ȼ� */
	                                   &Turnplate_AguestStk,          /* ����ջ */
	                                   sizeof(Turnplate_AguestStk));  /* ����ջ��С����λ�ֽ��� */

	HandleDR16_SxValue = os_tsk_create_user(DR16_SxValue,             /* ������ */
	                                      6,                         /* �������ȼ� */
	                                      &DR16_SxValueStk,         /* ����ջ */
	                                      sizeof(DR16_SxValueStk)); /* ����ջ��С����λ�ֽ��� */
	HandleBoot_Sequence = os_tsk_create_user(Boot_Sequence,             /* ������ */
                                      2,                         /* �������ȼ� */
                                      &Boot_SequenceStk,         /* ����ջ */
                                      sizeof(Boot_SequenceStk)); /* ����ջ��С����λ�ֽ��� */
}

/**
 * [AppObjCreate  ��������ͨ�Ż���]
 */
static void AppObjCreate (void)
{
	/* ������Ϣ���� */
	DR16_mailbox_init();//DR16�����ʼ��
	CAN_mailbox_init(1);//CAN1����ȳ�ʼ��
	CAN_mailbox_init(2);//CAN2����ȳ�ʼ��


	/* ��س�ʼ�� */
	delay_ms(100);
//	Motor_Init(0xf1,0);

}


/**
 * [main  ������]
 */
int main (void)
{
	/* ��ʼ������ */
	bsp_Init();

 	os_sys_init_user (TaskStart,              /* ������ */
	                  200,                         /* �������ȼ� */
	                  &TaskStartStk,          /* ����ջ */
	                  sizeof(TaskStartStk)); /* ����ջ��С����λ�ֽ��� */
	while(1)
	{;}

}
