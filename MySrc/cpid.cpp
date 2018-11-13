#include "cpid.h"
#include "bsp.h"
#include "filter.h"
static float TRI(float X,float DOWN,float MID,float UP);
static float LAD_DOWN(float X,float MID,float UP);
static float LAD_UP(float X,float DOWN,float MID);


//PID参数设定:16位参数


/*******
KP,KI,KD                           //设置PID的参数
TARGET                             //目标
RPI,RII,RDI                          
RPA,RIA,RDA                        
TI,TA                                //目标的最大值最小值
DEADZONE,OUT,MODE
TARGETOFFSET                         //目标的偏移量
*******/


PIDBase::PIDBase(float m_Kp, float m_Ki, float m_Kd, float m_Target,
float m_Rpi, float m_Rpa, float m_Rii, float m_Ria, float m_Rdi, float m_Rda, float m_Ti, float m_Ta,float m_DeadZone){
	Kp       = m_Kp;
	Ki       = m_Ki;
	Kd       = m_Kd;
	Target   = m_Target;
	Rpi      = m_Rpi;
	Rii      = m_Rii;
	Rdi      = m_Rdi;
	Rpa      = m_Rpa;
	Ria      = m_Ria;
	Rda      = m_Rda;
	Ti       = m_Ti;
	Ta       = m_Ta;
	DeadZone = m_DeadZone;
	Out      = 0;
	MODE     = 0;

}
void PIDBase::SetTarget(float target){
	Target = Constrain(target,Ti,Ta);
}
void PIDBase::SetCurrent(float current){
	Current = current;
}

void PIDBase::SetPID(float m_Kp, float m_Ki, float m_Kd){
	Kp = m_Kp;
	Ki = m_Ki;
	Kd = m_Kd;
	// printf("P:%5.2f\tI:%5.2f\tD:%5.2f\r\n",this->Kp,this->Ki,this->Kd);//调试用
}

void PIDBase::SetTargetMaxMin( float m_Ti, float m_Ta){
	Ti = m_Ti;
	Ta = m_Ta;
	// printf("Ti:%5.2f\tTa:%5.2f\r\n",this->Ti,this->Ta);//调试用
}
void PIDBase::SetPMaxMin(float m_Rpi, float m_Rpa){
	Rpi = m_Rpi;
	Rpa = m_Rpa;
	// printf("Rpi:%5.2f\tRpa:%5.2f\r\n",this->Rpi,this->Rpa);//调试用
}

void PIDBase::SetIMaxMin(float m_Rii, float m_Ria){
	Rii = m_Rii;
	Ria = m_Ria;
	// printf("Rii:%5.2f\tRia:%5.2f\r\n",this->Rii,this->Ria);//调试用
}
void PIDBase::SetDMaxMin(float m_Rdi, float m_Rda){
	Rdi = m_Rdi;
	Rda = m_Rda;
	// printf("Rdi:%5.2f\tRda:%5.2f\r\n",this->Rdi,this->Rda);//调试用
}

void PIDBase::SetDeadZone(float m_DeadZone){
 	DeadZone = m_DeadZone;
 	// printf("DeadZone:%5.2f\tD_dt_time:%d\r\n",this->DeadZone,this->D_dt_time);//调试用
}


unsigned char PIDTimer::UpdataTimeStamp(void)
{
	u32 now_time;

	//系统时间的获取
	if(last_time == 0)
	{
		last_time = micros();
		return 1;
	}
	now_time = micros();

	if(now_time < last_time)
	{
	  	dt = (float)(now_time + (0xFFFFFFFF - last_time) );
	}
	else
	{
		dt = (float)(now_time - last_time);

	}

  	last_time = now_time ;

	dt /= 1000000.0f;

	return 0;

}


void CPID::AdjustPID(void)
{
	float now_error;

	if(UpdataTimeStamp()) return;


	now_error = Target - Current;
	now_error = Slider_Filter(filterstruct.filterbuff[0],&filterstruct.num[0],5,now_error);
	

	//P
	PTerm = Kp * now_error;
	PTerm = Constrain(PTerm, Rpi, Rpa);

	//I

	ITerm +=  Ki * now_error * dt;

	ITerm = Constrain(ITerm, Rii, Ria);



	Omega = Slider_Filter(filterstruct.filterbuff[1],&filterstruct.num[1],5,Omega);
	DTerm =   Kd * (Omega - Omega_Offset);
	Out = PTerm + ITerm + DTerm;
	Out = Slider_Filter(filterstruct.filterbuff[2],&filterstruct.num[2],5,Out);//滤波，输出更加平滑

	if(myabs(now_error) <= DeadZone)
	{
		counter++;
		if(counter >= 1024)
			counter = 1024;
	}
	else
	{
		counter--;
		if(counter <= 0)
			counter = 0;
	}
	if(counter >= 1000)
	{
		Out = 0;
		ITerm = 0;
	}
}


void SCPID::AdjustPID(void)
{
	if(UpdataTimeStamp()) return;

	last_error = now_error;
	now_error = Target - Current;
	now_error = Slider_Filter(filterstruct.filterbuff[0],&filterstruct.num[0],5,now_error);

	//P
	PTerm = Kp * now_error;
	PTerm = Constrain(PTerm, Rpi, Rpa);

	//I
	ITerm +=  Ki * now_error * dt;
	ITerm = Constrain(ITerm, Rii, Ria);

	//D
	DTerm =   Kd * (now_error - last_error)/dt;
	DTerm = Constrain(DTerm, Rdi, Rda);
	DTerm = Slider_Filter(filterstruct.filterbuff[1],&filterstruct.num[1],5,DTerm);//滤波，输出更加平滑

	Out = PTerm + ITerm + DTerm;
	Out = Slider_Filter(filterstruct.filterbuff[2],&filterstruct.num[2],5,Out);//滤波，输出更加平滑
	
	if(myabs(now_error) <= DeadZone)
	{
		counter++;
		if(counter >= 1024)
			counter = 1024;
	}
	else
	{
		counter--;
		if(counter <= 0)
			counter = 0;
	}
	if(counter >= 1000)
	{
		Out = 0;
		ITerm = 0;
	}
}





void FUZZYPID::FuzzyPID(float error,float errorC)
{
	float Ux_S,Ux_M,Ux_B;
	float kp,ki,kd;
	Ux_S  = LAD_DOWN(myabs(error),this->NS_PS,this->NM_PM);
	Ux_M  = TRI(myabs(error),this->NS_PS,this->NM_PM,this->NB_PB);
	Ux_B  = LAD_UP(myabs(error),this->NM_PM,this->NB_PB);

	// Ux_S  = LAD_DOWN(myabs(error),this->NS_PS,this->NM_PM);
	// Ux_M  = TRI(myabs(error),this->NS_PS,this->NM_PM,this->NB_PB);
	// Ux_B  = LAD_UP(myabs(error),this->NM_PM,this->NB_PB);

	kp = (Ux_S * this->NS_PS_kp + Ux_M * this->NM_PM_kp + Ux_B * this->NB_PB_kp)/(Ux_S + Ux_M + Ux_B);
	ki = (Ux_S * this->NS_PS_ki + Ux_M * this->NM_PM_ki + Ux_B * this->NB_PB_ki)/(Ux_S + Ux_M + Ux_B);
	kd = (Ux_S * this->NS_PS_kd + Ux_M * this->NM_PM_kd + Ux_B * this->NB_PB_kd)/(Ux_S + Ux_M + Ux_B);

//	kp = CLimitSlider_Filter(Kpfilter,5,kp,10000);//滤波
//	ki = CLimitSlider_Filter(Kifilter,5,ki,10000);//滤波
//	kd = CLimitSlider_Filter(Kdfilter,5,kd,10000);//滤波

//	kp = Constrain(kp, 0.0f, this->NB_PB_kp);
//	ki = Constrain(ki, 0.0f, this->NB_PB_ki);
//	kd = Constrain(kd, 0.0f, this->NB_PB_kd);
//	if(myabs(error) <= NS_PS)
//	{
//		kd = NS_PS_kd*errorC;
//		kp = NS_PS_kp/errorC;
//	}
	SetPID(kp,ki,kd);
}


FUZZYPID::FUZZYPID(float m_Kp, float m_Ki, float m_Kd, float m_Target,\
		 float m_Rpi, float m_Rpa, float m_Rii, float m_Ria,\
		 float m_Rdi, float m_Rda, float m_Ti, float m_Ta, float m_DeadZone,\
		 float m_NB_PB, float m_NM_PM, float m_NS_PS,\
		 float m_NB_PB_kp,float m_NM_PM_kp, float m_NS_PS_kp,\
		 float m_NB_PB_ki,float m_NM_PM_ki, float m_NS_PS_ki,\
		 float m_NB_PB_kd,float m_NM_PM_kd, float m_NS_PS_kd)
		:PIDBase(m_Kp,m_Ki,m_Kd,m_Target,\
				m_Rpi,m_Rpa,m_Rii,m_Ria,\
				m_Rdi,m_Rda,m_Ti,m_Ta,m_DeadZone),
		NB_PB (m_NB_PB),
		NM_PM (m_NM_PM),
		NS_PS (m_NS_PS),
		NB_PB_kp (m_NB_PB_kp),
		NM_PM_kp (m_NM_PM_kp),
		NS_PS_kp (m_NS_PS_kp),
		NB_PB_ki (m_NB_PB_ki),
		NM_PM_ki (m_NM_PM_ki),
		NS_PS_ki (m_NS_PS_ki),
		NB_PB_kd (m_NB_PB_kd),
		NM_PM_kd (m_NM_PM_kd),
		NS_PS_kd (m_NS_PS_kd)
		{
			Kec = 0.00001f;
		};


void FUZZYPID::AdjustPID(void)
{
	if(UpdataTimeStamp()) return;

	now_error = Target - Current;
	now_error = Slider_Filter(filterstruct.filterbuff[0],&filterstruct.num[0],5,now_error);
	Omega = Slider_Filter(filterstruct.filterbuff[1],&filterstruct.num[1],5,Omega);

	errorC = Kec*Omega/dt;
	FuzzyPID(now_error,errorC);

	//P
	PTerm = Kp * now_error;
	PTerm = Constrain(PTerm, Rpi, Rpa);

	//I

	ITerm +=  Ki * now_error * dt;
	ITerm = Constrain(ITerm, Rii, Ria);

	//D
	DTerm =   Kd * (Omega - Omega_Offset);

	Out = PTerm + ITerm + DTerm;
	Out = Slider_Filter(filterstruct.filterbuff[2],&filterstruct.num[2],5,Out);//滤波，输出更加平滑
	
	if(myabs(now_error) <= DeadZone)
	{
		counter++;
		if(counter >= 1024)
			counter = 1024;
	}
	else
	{
		counter--;
		if(counter <= 0)
			counter = 0;
	}
	if(counter >= 1000)
	{
		Out = 0;
		ITerm = 0;
	}
}


void FUZZYSPID::AdjustPID(void)
{
	if(UpdataTimeStamp()) return;

	last_error = now_error;
	now_error = Target - Current;
	now_error = Slider_Filter(filterstruct.filterbuff[0],&filterstruct.num[0],5,now_error);

	errorC = Kec*Omega/dt;
	FuzzyPID(now_error,errorC);

	//P
	PTerm = Kp * now_error;
	PTerm = Constrain(PTerm, Rpi, Rpa);

	//I

	ITerm +=  Ki * now_error * dt;
	ITerm = Constrain(ITerm, Rii, Ria);

	//D
	DTerm =   Kd * (now_error - last_error)/dt;
	DTerm = Constrain(DTerm, Rdi, Rda);
	DTerm = Slider_Filter(filterstruct.filterbuff[1],&filterstruct.num[1],5,DTerm);//滤波，输出更加平滑
	
	Out = PTerm + ITerm + DTerm;
	Out = Slider_Filter(filterstruct.filterbuff[2],&filterstruct.num[2],5,Out);//滤波，输出更加平滑
	
	if(myabs(now_error) <= DeadZone)
	{
		counter++;
		if(counter >= 1024)
			counter = 1024;
	}
	else
	{
		counter--;
		if(counter <= 0)
			counter = 0;
	}
	if(counter >= 1000)
	{
		Out = 0;
		ITerm = 0;
	}
}

float TRI(float X,float DOWN,float MID,float UP)
{
	if( X >= DOWN && X <= UP)
		return ( 1 - myabs(X - MID)/( MID - DOWN) );
	else
		return 0;
}
float LAD_DOWN(float X,float MID,float UP)
{
	if(	X <= MID)
		return 1.0f;
	else if( X <= UP )
		return (X - UP) / (MID - UP);
	else
		return 0;
}

float LAD_UP(float X,float DOWN,float MID)
{
	if(	X >= MID)
		return 1.0f;
	else if( X >= DOWN )
		return (X - DOWN) / (MID - DOWN);
	else
		return 0;
}




//	temp[0] = ( now_error)/dt;
//	temp[1] = ( now_error);
		//D

//	if( myabs(now_error)>= 2)
//	{
//		if(iforad == D_dt_time -1)
//		{
//			Derror = now_error - prev_error ;

//			Derror = CLimitSlider_Filter(PitchEfilter,5,Derror,10000);//滤波，微分毛躁太多
//			DTerm =   Kd * ( Derror)/D_dt;
////			DTerm = KalmanFilter(DTerm,1,1);
//			DTerm = CLimitSlider_Filter(PitchDfilter,5,DTerm,10000);//滤波，微分毛躁太多
//			DTerm = Constrain(DTerm, Rdi, Rda);
//			//ANO_Data_Send(0xf2,D_dt*1000000.0f);
//			prev_error = now_error;
//			D_dt = 0;
//			iforad = 0;
//		}
//		else iforad++;

//	}
//	if( now_error == last_error && ITerm != 0)
//		i_error++;
//	else
//		i_error = 0;
//	if(i_error == 150)
//		ITerm = 0;
