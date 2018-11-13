#ifndef _CPID_H__
#define _CPID_H__



#include "stm32f4xx.h"
#define myabs(x) ((x)>0? (x):(-(x)))

class PIDBase{
	public:
	float MODE;//目标值
	float Target;//目标值
	float Current;//当前值
	float Out;//解算后的输出值
	float I;//电流值
	float PTerm,ITerm,DTerm;//PID各个分量输出值
	float Kp, Ki, Kd;//PID各个参数
	float Rpi, Rii, Rdi, Ti;//最小值
	float Rpa, Ria, Rda, Ta;//最大值
	float DeadZone;
	short counter;
	float now_error,last_error;
	//
	PIDBase(float m_Kp, float m_Ki, float m_Kd, float m_Target,
		float m_Rpi, float m_Rpa, float m_Rii, float m_Ria, float m_Rdi, float m_Rda, float m_Ti, float m_Ta, float m_DeadZone);
	void SetTarget(float target);
	void SetCurrent(float current);
	void SetPID(float m_Kp, float m_Ki, float m_Kd);
	void SetTargetMaxMin( float m_Ti, float m_Ta);
	void SetPMaxMin(float m_Rpi, float m_Rpa);
	void SetIMaxMin(float m_Rii, float m_Ria);
	void SetDMaxMin(float m_Rdi, float m_Rda);
	void SetDeadZone(float m_DeadZone);
};

class PIDTimer
{
public:
	float dt;
	u32 last_time;//上次系统时间记录
	unsigned char UpdataTimeStamp(void);
// private:

};


class FUZZYPID:public PIDBase,public PIDTimer
{
public:
	float Omega,Omega_Offset;
	float NB_PB,NM_PM,NS_PS;
	float NB_PB_kp,NM_PM_kp,NS_PS_kp;
	float NB_PB_ki,NM_PM_ki,NS_PS_ki;
	float NB_PB_kd,NM_PM_kd,NS_PS_kd;

	float errorC,Kec;


	FUZZYPID(float m_Kp, float m_Ki, float m_Kd, float m_Target,\
		 float m_Rpi, float m_Rpa, float m_Rii, float m_Ria,\
		 float m_Rdi, float m_Rda, float m_Ti, float m_Ta,float m_DeadZone,

		 float m_NB_PB, float m_NM_PM, float m_NS_PS,\
		 float m_NB_PB_kp,float m_NM_PM_kp, float m_NS_PS_kp,\
		 float m_NB_PB_ki,float m_NM_PM_ki, float m_NS_PS_ki,\
		 float m_NB_PB_kd,float m_NM_PM_kd, float m_NS_PS_kd
		 );
	void AdjustPID(void);
	void FuzzyPID(float error,float errorC);
	void SetOmega(float data,unsigned char dir){
		Omega = (data - Omega_Offset);
		if(dir) Omega = -Omega;
	};
	void SetOmegaOffset(float data){
		Omega_Offset = data;
	};

	void SetFuzzy(float mode,float _NB_PB_kx1,float _NM_PM_kx2,float _NS_PS_kx3){
		switch((u8)mode){
			case 0:NB_PB = _NB_PB_kx1; 	NM_PM = _NM_PM_kx2;	 NS_PS = _NS_PS_kx3;break;
			case 1:NB_PB_kp = _NB_PB_kx1; NM_PM_kp = _NM_PM_kx2; NS_PS_kp = _NS_PS_kx3;break;
			case 2:NB_PB_ki = _NB_PB_kx1; NM_PM_ki = _NM_PM_kx2; NS_PS_ki = _NS_PS_kx3;break;
			case 3:NB_PB_kd = _NB_PB_kx1; NM_PM_kd = _NM_PM_kx2; NS_PS_kd = _NS_PS_kx3;break;
			default:break;
			}
		};

private:
	struct filter_struct
	{
		unsigned char num[3];
		float filterbuff[3][5];//滤波器的数组BUFF
	}filterstruct;
};


class FUZZYSPID:public FUZZYPID
{
public:
		
	FUZZYSPID(float m_Kp, float m_Ki, float m_Kd, float m_Target,\
		 float m_Rpi, float m_Rpa, float m_Rii, float m_Ria,\
		 float m_Rdi, float m_Rda, float m_Ti, float m_Ta,float m_DeadZone,

		 float m_NB_PB, float m_NM_PM, float m_NS_PS,\
		 float m_NB_PB_kp,float m_NM_PM_kp, float m_NS_PS_kp,\
		 float m_NB_PB_ki,float m_NM_PM_ki, float m_NS_PS_ki,\
		 float m_NB_PB_kd,float m_NM_PM_kd, float m_NS_PS_kd
			 ):FUZZYPID(m_Kp,m_Ki,m_Kd,m_Target,\
				m_Rpi,m_Rpa,m_Rii,m_Ria,\
				m_Rdi,m_Rda,m_Ti,m_Ta,m_DeadZone,\
				m_NB_PB,m_NM_PM,m_NS_PS,\
				m_NB_PB_kp,m_NM_PM_kp,m_NS_PS_kp,\
				m_NB_PB_ki,m_NM_PM_ki,m_NS_PS_ki,\
				m_NB_PB_kd,m_NM_PM_kd,m_NS_PS_kd)
		 {};
	void AdjustPID(void);
private:
	struct filter_struct
	{
		unsigned char num[3];
		float filterbuff[3][5];//滤波器的数组BUFF
	}filterstruct;
};


class CPID:public PIDBase,public PIDTimer
{
public:
	float Omega,Omega_Offset;
	CPID(float m_Kp, float m_Ki, float m_Kd, float m_Target,\
		 float m_Rpi, float m_Rpa, float m_Rii, float m_Ria,\
		 float m_Rdi, float m_Rda, float m_Ti, float m_Ta,float m_DeadZone):\
		PIDBase(m_Kp,m_Ki,m_Kd,m_Target,\
				m_Rpi,m_Rpa,m_Rii,m_Ria,\
				m_Rdi,m_Rda,m_Ti,m_Ta,m_DeadZone){}
	void AdjustPID(void);
	void SetOmega(float data,unsigned char dir){
		Omega = (data - Omega_Offset);
		if(dir) Omega = -Omega;
	};
	void SetOmegaOffset(float data){
		Omega_Offset = data;
	};

private:
	struct filter_struct
	{
		unsigned char num[3];
		float filterbuff[3][5];//滤波器的数组BUFF
	}filterstruct;

};

class SCPID:public PIDBase,public PIDTimer
{
public:

	SCPID(float m_Kp, float m_Ki, float m_Kd, float m_Target,\
		 float m_Rpi, float m_Rpa, float m_Rii, float m_Ria,\
		 float m_Rdi, float m_Rda, float m_Ti, float m_Ta,float m_DeadZone):\
		PIDBase(m_Kp,m_Ki,m_Kd,m_Target,\
				m_Rpi,m_Rpa,m_Rii,m_Ria,\
				m_Rdi,m_Rda,m_Ti,m_Ta,m_DeadZone){}
	void AdjustPID(void);
private:
	struct filter_struct
	{
		unsigned char num[3];
		float filterbuff[3][5];//滤波器的数组BUFF
	}filterstruct;

};






#endif
