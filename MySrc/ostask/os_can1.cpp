#include "bsp.h"
#include "cpid.h"
#include "SetParameter.h"
#define  Y_run_speed            3000
#define  X_run_speed            3500
#define  Y_addA_speed           1500
#define  X_addA_speed           2000
#define  distance_threshold_X   450
#define  distance_threshold_Y   550
#define  GYOX                   600
#define  GYOY                   600

FUZZYPID CVPID(8.5,0.1,1.1,0,-4500, +4500, -3000, +3000, -4500, +4500,-500,500,0,\
		 100.0f,50.0f,20.0f,\
//		 0.3f,0.6f,0.5f,\
		 0.0f,0.0f,0.0f,\
		 0.8,1.0,0.8);
		  0.5f,0.5f,0.5f,\
//		 0.4f,0.2f,0.25f,
		 1.0f,1.5f,3.0f,\
		 0.1,0.1,0.1);
FUZZYPID CVPitchPID(8.5,0.1,1.1,0,-4500, +4500, -3000, +3000, -4500, +4500,-500,500,0,\
		 100.0f,50.0f,20.0f,\
//		 0.3f,0.6f,0.5f,\
		 0.0f,0.0f,0.0f,\
		 0.8,1.0,0.8);
		  0.5f,0.5f,0.5f,\
//		 0.4f,0.2f,0.25f,
		 1.0f,1.5f,3.0f,\
		 0.1,0.1,0.1);
u8 cv_flag;
u8 auto_attack_flag=0;
extern FUZZYPID YawPID,PitchPID;
extern CPID CarFramePID;
static u8   round;
float  last_yawangle;
short distance1,distance2,distance3,distance4;
int temp_1 = 0,temp_2 = 0,temp_3=0,temp_4=0;
short adcx;
int  Left_or_Right=1;
int  Up_or_Down=1;
u8  hide=0;
u8  rand_gox;
u8  rand_goy;
u8  rand_left_right;
u8  rand_up_down;
u8  direction;
u8  rand_result;
u8  zero_flag;
u8  spin_flag;
u8  rand_flag=1;
u8  across_times;
u8  direction_select_flag;
u8  ultrasonic_correct_flag;
u8  spin_direction_flag;
u8  Utc_Answer;
u16 Gyo_X,Gyo_Y;
float Gyo_Z=5000;
float direction_zero;
u16 ultrasonic_correct_threshold;
float yaw_angle,yaw_angle_360,yaw_angle_180;
int RandomMotor[4];
int distance_error;
int add_A1 = 0,add_A2 = 0,add_A3=0,add_A4=0;
int add_B1 = 0,add_B2 = 0,add_B3=0,add_B4=0;
static u8 rec_times,dec_times=100;
INT16_UNION Yaw;
void right(void)         {temp_1=X_run_speed,temp_2=X_run_speed,temp_3=-X_run_speed,temp_4=-X_run_speed;direction=0;}
void left(void)          {temp_1=-X_run_speed,temp_2=-X_run_speed,temp_3=X_run_speed,temp_4=X_run_speed;direction=1;}
void back(void)          {temp_1=-Y_run_speed,temp_2=Y_run_speed,temp_3=Y_run_speed,temp_4=-Y_run_speed;direction=2;}
void forward(void)       {temp_1=Y_run_speed,temp_2=-Y_run_speed,temp_3=-Y_run_speed,temp_4=Y_run_speed;direction=3;}
void clockwise(void)     {temp_1=4000,temp_2=4000,temp_3=4000,temp_4=4000;}
void anticlockwise(void) {temp_1=-4000,temp_2=-4000,temp_3=-4000,temp_4=-4000;}
void stop(void)    {temp_1=0,temp_2=0,temp_3=0,temp_4=0;direction=4;}
int RandomMotor1,RandomMotor2,RandomMotor3,RandomMotor4;

int Get_RandomMotor1(void){return RandomMotor1;}
int Get_RandomMotor2(void){return RandomMotor2;}
int Get_RandomMotor3(void){return RandomMotor3;}
int Get_RandomMotor4(void){return RandomMotor4;}



void Run_Random(CanRxMsg *msg_rece)       
{
	      distance3=msg_rece->Data[0]*256+msg_rece->Data[1];
				distance4=msg_rece->Data[2]*256+msg_rece->Data[3];
				distance1=msg_rece->Data[4]*256+msg_rece->Data[5];
  		  distance2=msg_rece->Data[6]*256+msg_rece->Data[7];
//				u1_printf("%d\t%d\t%d\t%d\r\n",distance1,distance2,distance3,distance4);
				while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
				adcx = RNG_GetRandomNumber(); 
//        adcx=Get_Adc(ADC_Channel_5);
        srand((short)adcx);
//        printf("%d\r\n",distance1);				
        if(distance1<distance_threshold_X)
        {
				   if(distance2<distance_threshold_Y)      
             {if(rand_flag!=1)rand_result=rand()%2;
						 rand_flag=1;
						 if(rand_result)              right();
						 else                         back();
						 }
		  else if(distance4<distance_threshold_Y) 
             {if(rand_flag!=7)rand_result=rand()%2;
						 rand_flag=7;
             if(rand_result)              forward();
						 else                         right();	 
						 }
			else                   
             {if(rand_flag!=8)rand_result=rand()%3;
						 rand_flag=8;
             if(rand_result==0)           forward();
						 else if(rand_result==1)      back(); 
						 else                         right();
						 }
				}
				else
        {
				  if(distance3<distance_threshold_X)
				 {
				    if(distance2<distance_threshold_Y)      
             {if(rand_flag!=3)rand_result=rand()%2;
						 rand_flag=3;
             if(rand_result)              back();
						 else                         left();	  
						 }
			 else if(distance4<distance_threshold_Y) 
             {if(rand_flag!=5)rand_result=rand()%2;
						 rand_flag=5;
             if(rand_result)              left();
						 else                         forward();	 
						 }
			 else                   
             {if(rand_flag!=4)rand_result=rand()%3;
						 rand_flag=4;
             if(rand_result==0)           left();
						 else if(rand_result==1)      forward();
						 else                         back();
						 }	
				 }
				  else
				 {
					  if(distance2<distance_threshold_Y)      
            {if(rand_flag!=2)rand_result=rand()%3;
						 rand_flag=2;
             if(rand_result==0)           back();
						 else if(rand_result==1)      left();	 
						 else                         right();
						}
			 else if(distance4<distance_threshold_Y) 
            {if(rand_flag!=6)rand_result=rand()%3;
						 rand_flag=6;
             if(rand_result==0)           forward();
						 else if(rand_result==1)      left();	 
						 else                         right();
//						if(rand_result==0)           stop();
//						 else if(rand_result==1)     stop();	 
//						 else                        stop();
						}
			 else                   
            {				 
							rand_flag=9;
						}	
				 }
			}
}


void Run_Correct(CanRxMsg *msg_rece) 
{
	  int cnt;
	  yaw_angle = *((__packed float *)msg_rece->Data);
    cnt=(int)(yaw_angle/360);
	  yaw_angle_360=yaw_angle-360*cnt;	  
	  if(yaw_angle_360<0)yaw_angle_360=yaw_angle_360+360;
	  if(yaw_angle_360<180)yaw_angle_180=yaw_angle_360;
	  else yaw_angle_180=yaw_angle_360-180;
	yaw_angle_180=-yaw_angle_180;
		if(zero_flag==0)direction_zero=yaw_angle_180;	
		zero_flag=1;
		add_A1=(int)(direction_zero-yaw_angle_180)*50;
		add_A2=(int)(direction_zero-yaw_angle_180)*50;
		add_A3=(int)(direction_zero-yaw_angle_180)*50;
		add_A4=(int)(direction_zero-yaw_angle_180)*50;
}
void Run_Protect(void)
{   
	  add_B1=0,add_B2=0,add_B3=0,add_B4=0;
    if((distance1<500))
		{
		 add_B1=X_addA_speed,add_B2=X_addA_speed,add_B3=-X_addA_speed,add_B4=-X_addA_speed;
		}
    if((distance2<500))
		{
		 add_B1=-Y_addA_speed,add_B2=Y_addA_speed,add_B3=Y_addA_speed,add_B4=-Y_addA_speed;
		}
    if((distance3<500))
		{
		 add_B1=-X_addA_speed,add_B2=-X_addA_speed,add_B3=X_addA_speed,add_B4=X_addA_speed;
		}
    if((distance4<500))
		{
		 add_B1=Y_addA_speed,add_B2=-Y_addA_speed,add_B3=-Y_addA_speed,add_B4=Y_addA_speed;
		}				
}

void Run_Random_v2(CanRxMsg *msg_rece)
{
			distance3=msg_rece->Data[0]*256+msg_rece->Data[1];
			distance4=msg_rece->Data[2]*256+msg_rece->Data[3];
			distance1=msg_rece->Data[4]*256+msg_rece->Data[5];
  		distance2=msg_rece->Data[6]*256+msg_rece->Data[7];
			while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
			adcx = RNG_GetRandomNumber(); 
	    if((distance1<450)&&(distance3>450)&&(distance2<550)&&(distance4>550))
			{
			 if(rand_flag!=1){rand_result=rand()%2;across_times++;}			  
				rand_flag=1;
				if(rand_result)              right();
			  else                         back();
			}
			else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4>550))
			{	
				if(rand_flag!=8){rand_result=rand()%3;across_times++;}			
				rand_flag=8;
			 if(rand_result==0)           forward();
			 else if(rand_result==1)      back(); 
			 else                         right();
		  }
			else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4<550))
			{
			 if(rand_flag!=7){rand_result=rand()%2;across_times++;} 
				rand_flag=7;
			 if(rand_result)              forward();
			 else                         right();	 
			}
			else if((distance1>450)&&(distance3<450)&&(distance2<550)&&(distance4>550))
			{
			if(rand_flag!=3){rand_result=rand()%2;across_times++;}
			rand_flag=3;
			if(rand_result)              left();
			else                         back();	 			
			}
			else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4>550))
			{
				if(rand_flag!=4){rand_result=rand()%3;across_times++;}
				rand_flag=4;
			 if(rand_result==0)           forward();
			 else if(rand_result==1)      back(); 
			 else                         left();
			}				
			else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4<550))
			{
				if(rand_flag!=5){rand_result=rand()%2;across_times++;}
				rand_flag=5;
				if(rand_result)              left();
			  else                         forward();
			}
			else if((distance1>450)&&(distance3>450)&&(distance2<550)&&(distance4>550))
			{
				if(rand_flag!=2){rand_result=rand()%3;across_times++;}
				rand_flag=2;
			 if(rand_result==0)           left();
			 else if(rand_result==1)      back(); 
			 else                         right();				
			}
			else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4>550))
			{
				rand_flag=9;
			}	
			else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4<550))
			{
				if(rand_flag!=6){rand_result=rand()%3;across_times++;}				
				rand_flag=6;
				if(rand_result==0)           left();
			  else if(rand_result==1)      forward(); 
			  else                         right();	
      } 
				else rand_flag=10;
}



void CV_ToPC(CVStruct *pMsg)
{
	CanTxMsg msg_send = { 0x500, 0x500, CAN_Id_Standard, CAN_RTR_Data, 0x08, {0, 0, 0, 0, 0, 0, 0, 0}};
	*ToCVStructPointer(msg_send.Data) = *ToCVStructPointer(pMsg);
    while(CAN_Transmit(CAN1,&msg_send) == CAN_TxStatus_NoMailBox);
}
void CV_FromPC(CanRxMsg *pMsg)
{
	static u8 filtercnt = 0;
//	u1_printf("%x\t%x\t%d\t%d\t%d\r\n",ToCVStructPointer(pMsg->Data)->ID,ToCVStructPointer(pMsg->Data)->Flag,ToCVStructPointer(pMsg->Data)->CV_X,ToCVStructPointer(pMsg->Data)->CV_Y,ToCVStructPointer(pMsg->Data)->RES);
	if(cv_flag == 1)
	{
		if(ToCVStructPointer(pMsg->Data)->ID == 0x01)
		{

				YawPID.SetTarget( (YawPID.Current -  CarFramePID.Current ) - (ToCVStructPointer(pMsg->Data)->CV_X/100.0f)*22.7556f + CVPID.Out);
			filtercnt = 0;
		}
		else filtercnt++;
		if(filtercnt == 30)
		{
			CVPID.ITerm = 0;
			CVPitchPID.ITerm = 0;
			YawPID.SetTarget( (YawPID.Current -  CarFramePID.Current));
//			PitchPID.SetTarget( Parameter_Operater.data.PITCH_Encoder_Mid);
			filtercnt = 0;
		}
	}
}


u8 Get_cv_flag(void)
{
	return cv_flag;
}
void Set_cv_flag(u8 tflag)
{
	cv_flag = tflag;
}


void RealShoot_Massage(uint8_t *pMsg)
{
    CanTxMsg msg_send = { 0x502, 0x502, CAN_Id_Standard, CAN_RTR_Data, 0x08, {0, 0, 0, 0, 0, 0, 0, 0}};
	*(tRealShootData*)(msg_send.Data) = *(tRealShootData*)(pMsg);
    // msg_send.Data[0] = 0x55;
//	*((CVStruct*)Ftemp.DATA) = *((CVStruct*)(&pMsg->Data[1]));
	
//	*((CVStruct*)(&msg_send.Data[0])) = *((CVStruct*)pMsg);
    while(CAN_Transmit(CAN1,&msg_send) == CAN_TxStatus_NoMailBox);

}

void Hardware_Random_Init(void)
{
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);
  RNG_Cmd(ENABLE);
}
void Send_OverPowerFlag(u8 flag)
{
    CanTxMsg msg_send = { 0x520, 0x520, CAN_Id_Standard, CAN_RTR_Data, 0x08, {(unsigned char)flag, 0, 0, 0, 0, 0, 0, 0}};
		while(CAN_Transmit(CAN1,&msg_send) == CAN_TxStatus_NoMailBox);
}
//void Run_By_Surround(CanRxMsg *msg_rece)
//{
//	distance3=msg_rece->Data[0]*256+msg_rece->Data[1];
//	distance4=msg_crece->Data[2]*256+msg_rece->Data[3];
//	distance1=msg_rece->Data[4]*256+msg_rece->Data[5];
//	distance2=msg_rece->Data[6]*256+msg_rece->Data[7];
//	if()
//}



void Run_By_Surround(CanRxMsg *msg_rece)
{
	distance3=msg_rece->Data[0]*256+msg_rece->Data[1];
	distance4=msg_rece->Data[2]*256+msg_rece->Data[3];
	distance1=msg_rece->Data[4]*256+msg_rece->Data[5];
  	distance2=msg_rece->Data[6]*256+msg_rece->Data[7];
	if((distance1<450)&&(distance3>450)&&(distance2<550)&&(distance4>550))	    {rand_flag=1;Left_or_Right=1;Up_or_Down=1;hide=1;}
	else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4>550)) {rand_flag=8;Left_or_Right=1;hide=1;}
	else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4<550)) {rand_flag=7;Left_or_Right=1;Up_or_Down=-1;hide=1;}
	else if((distance1>450)&&(distance3<450)&&(distance2<550)&&(distance4>550)) {rand_flag=3;Left_or_Right=-1;Up_or_Down=1;hide=1;}
	else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4>550)) {rand_flag=4;Left_or_Right=-1;hide=1;}		
	else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4<550)) {rand_flag=5;Left_or_Right=-1;Up_or_Down=-1;hide=1;}	
	else if((distance1>450)&&(distance3>450)&&(distance2<550)&&(distance4>550)) {rand_flag=2;Up_or_Down=1;hide=1;}		
	else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4<550)) {rand_flag=6;Up_or_Down=-1;hide=1;}
	else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4>550))
	{  
		if(rand_flag!=9)
		{
			while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
			adcx = RNG_GetRandomNumber(); 
			srand((short)adcx);
			rand_gox=rand()%3;
			rand_goy=rand()%3;
			rand_left_right=rand()%3;
			rand_up_down=rand()%3;
//		    u1_printf("%d,%d,%d\r\n",rand_result,Left_or_Right,Up_or_Down);		
			if(rand_gox)
            {
				if(rand_left_right)Gyo_X=1024-(GYOX*Left_or_Right);
				else               Gyo_X=1024+(GYOX*Left_or_Right);

		    }
			else
			{
				if(Gyo_Y!=1024)Gyo_X=1024;
				else  Gyo_X=1024-(GYOX*Left_or_Right);
			}
			if(rand_goy)
			{
				if(rand_up_down)Gyo_Y=1024+(GYOY*Up_or_Down);
				else          Gyo_Y=1024-(GYOY*Up_or_Down);
			}
			else
			{
				if(Gyo_X!=1024)Gyo_Y=1024;
				else  Gyo_Y=1024-(GYOY*Left_or_Right);
			}
//			hide=1;
		}
		rand_flag=9;         
	}
	else rand_flag=10;
	switch(rand_flag)
	{
		case 1:Gyo_X=1024-GYOX;Gyo_Y=1024+GYOY;break;
		case 2:Gyo_X=1024;     Gyo_Y=1024+GYOY;break;
		case 3:Gyo_X=1024+GYOX;Gyo_Y=1024+GYOY;break;
		case 4:Gyo_X=1024+GYOX;Gyo_Y=1024;     break;
		case 5:Gyo_X=1024+GYOX;Gyo_Y=1024-GYOY;break;
		case 6:Gyo_X=1024;     Gyo_Y=1024-GYOY;break;
		case 7:Gyo_X=1024-GYOX;Gyo_Y=1024-GYOY;break;
		case 8:Gyo_X=1024-GYOX;Gyo_Y=1024;     break;
		case 9:break;
        case 10:break;
	}
}
void Run_Spin(CanRxMsg *msg_rece)
{
	  float total_angle;
	  int cnt;
   	yaw_angle = *((__packed float *)msg_rece->Data);
    cnt=(int)(yaw_angle/360);
	  yaw_angle_360=yaw_angle-360*cnt;
	  if(yaw_angle_360<0)yaw_angle_360=yaw_angle_360+360;
//	  if(spin_flag)
//		{
		 if(last_yawangle>yaw_angle_360)round++;		 
		 total_angle=round*360+yaw_angle_360;
			
//			u1_printf("%d\r\n",(short)total_angle);
//		 if(last_yawangle-yaw_angle>300){spin_flag=0;round=0;forward();spin_direction_flag=!spin_direction_flag;}		
//	   if(spin_flag);
//        clockwise(); 
//   {
//		 {if(!spin_direction_flag)		 
//		 Motor_Aguest(2000,2000,-4000,0);
//		 else 
//		 Motor_Aguest(-2000,-2000,4000,0);}	
//	 }

//     if((abs(last_yawangle-yaw_angle)>450)||(rand_flag==9)){Gyo_Z=-Gyo_Z;last_yawangle=yaw_angle;}
       
//       if(abs(last_yawangle-yaw_angle)>450){last_yawangle=yaw_angle;}
			 if((abs(last_yawangle-yaw_angle)>450)){Send_OverPowerFlag(0x00);Gyo_Z=-Gyo_Z;last_yawangle=yaw_angle;}
			 else if(abs(last_yawangle-yaw_angle)>300){Send_OverPowerFlag(0x01);}
//			 			 if((abs(last_yawangle-yaw_angle)>600)){Send_OverPowerFlag(0x00);Gyo_Z=-Gyo_Z;last_yawangle=yaw_angle;}
//			 else if(abs(last_yawangle-yaw_angle)>300){Send_OverPowerFlag(0x01);}
//     last_yawangle=yaw_angle_360;		 
//    }
}
void Motor_Aguest_Random(u8 hurt_sta,u8 hurt_origin)
{ 	
	      
				Motor_Aguest(Gyo_X,Gyo_Y,Gyo_Z,0);			

}


void Moving_Gyo(CanRxMsg *msg_rece)
{
	distance3=msg_rece->Data[0]*256+msg_rece->Data[1];
	distance4=msg_rece->Data[2]*256+msg_rece->Data[3];
	distance1=msg_rece->Data[4]*256+msg_rece->Data[5];
	distance2=msg_rece->Data[6]*256+msg_rece->Data[7];
	    if((distance1<450)&&(distance3>450)&&(distance2<550)&&(distance4>550))		rand_flag=1;
			else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4>550))rand_flag=8;
			else if((distance1<450)&&(distance3>450)&&(distance2>550)&&(distance4<550))rand_flag=7;
			else if((distance1>450)&&(distance3<450)&&(distance2<550)&&(distance4>550))rand_flag=3;
			else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4>550))rand_flag=4;		
			else if((distance1>450)&&(distance3<450)&&(distance2>550)&&(distance4<550))rand_flag=5;	
			else if((distance1>450)&&(distance3>450)&&(distance2<550)&&(distance4>550))rand_flag=2;		
			else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4<550))rand_flag=6;
	    else if((distance1>450)&&(distance3>450)&&(distance2>550)&&(distance4>550))
				{  
					if(rand_flag!=9)
					{
					while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
							 adcx = RNG_GetRandomNumber(); 
						   srand((short)adcx);
				       rand_result=rand()%6;
//						   u1_printf("%d\r\n",rand_result);		
					if(rand_result==0)     {Gyo_X=1024-GYOX;Gyo_Y=1024-GYOY;}
					else if(rand_result==1){Gyo_X=1024-GYOX;Gyo_Y=1024;}
					else if(rand_result==2){Gyo_X=1024;     Gyo_Y=1024-GYOY;}					
				  else if(rand_result==3){Gyo_X=1024+GYOX;Gyo_Y=1024+GYOY;}
					else if(rand_result==4){Gyo_X=1024+GYOX;Gyo_Y=1024;}
					else                   {Gyo_X=1024;     Gyo_Y=1024+GYOY;}
				  }
					rand_flag=9;         
				}
			else rand_flag=10;
			switch(rand_flag)
			{
				case 1:Gyo_X=1024-GYOX;Gyo_Y=1024+GYOY;break;
				case 2:Gyo_X=1024;     Gyo_Y=1024+GYOY;break;
				case 3:Gyo_X=1024+GYOX;Gyo_Y=1024+GYOY;break;
				case 4:Gyo_X=1024+GYOX;Gyo_Y=1024;     break;
				case 5:Gyo_X=1024+GYOX;Gyo_Y=1024-GYOY;break;
				case 6:Gyo_X=1024;     Gyo_Y=1024-GYOY;break;
				case 7:Gyo_X=1024-GYOX;Gyo_Y=1024-GYOY;break;
				case 8:Gyo_X=1024-GYOX;Gyo_Y=1024;     break;
				case 9:
//					     Gyo_X=1024-GYOX;Gyo_Y=1024-GYOY;
							 
				       		      
				       break;			
				case 10:break;
				
			}	
//			Gyo_Z=3000;
//		 Motor_Aguest(Gyo_X,Gyo_Y,Gyo_Z,yaw_angle);
   			
}


extern union Parameter_User_Union Parameter_User;
float speed,lastspeed,error,lasterror,kp=3,kd=0.5;
float speed_y,lastspeed_y,error_y,lasterror_y,y_kp=3,y_kd=0.5;
void Base_AutoAttack(CVStruct *CVData)
{ 
	float TempPitch,TempYaw;
	if(CVData->Flag){rec_times++;}
	else{dec_times--;}//未识别到，dec_times计数减一
	if(dec_times==0)
	{
		dec_times=100;
		rec_times=0;               //在下面的else下面
	}
	if(rec_times>1)//识别到两次
   {

	  runstart_flag=0;
		auto_attack_flag=1;
		error=CVData->CV_X-lastspeed;
		speed=kp*error+kd*(error-lasterror) ;
		lastspeed=CVData->CV_X;
		lasterror=error; 
		 
//		 
		
		
//		if(myabs(CVData->CV_X)<100)
//		{
//    ESC_Toggle();			
//		delay_ms(2000);
//		ESC_Toggle();	
//		rec_times=0;
//    runstart_flag=1;		
//    auto_attack_flag=0;
//		}			
	
	 }
	 else         
		 {
			 runstart_flag=1;
			 auto_attack_flag=0;
			// ESC_Toggle();	
		 }
		
	
}