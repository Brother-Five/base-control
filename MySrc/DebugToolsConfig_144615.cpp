#include "..\DebugTools\Data_Tramsfer.h"
#include "..\DebugTools\DebugTools.h"
#include "..\DebugTools\SystemTimer.h"
#include "..\User\cpid.h"
#include "bsp.h"
void tips(void)
{
	unsigned char i;
	for(i=0;i<dev.fnum-1;i++)
	{
		printf("%s\r\n",dev.tab[i].tips);
	}
}

void test1(void)
{
	printf("1\r\n");
}
void test2(float tem)
{
	printf("%f\r\n",tem);
}

/**********************************
 * 给C语言编译的函数调用的接口
 *********************************/
extern CPID PitchPID,YawPID,CarFramePID;
void SetPitchPid(float m_Kp, float m_Ki, float m_Kd)
{
	CarFramePID.SetPID(m_Kp, m_Ki,m_Kd);
}
void SetTargetMaxMin(float m_Ti,float m_Ta){
	CarFramePID.SetTargetMaxMin(m_Ti,m_Ta);
}
void SetPMaxMin(float m_Rpi, float m_Rpa){
	CarFramePID.SetPMaxMin(m_Rpi,m_Rpa);
}
void SetIMaxMin(float m_Rii, float m_Ria){
	CarFramePID.SetIMaxMin(m_Rii,m_Ria);
}
void SetDMaxMin(float m_Rdi, float m_Rda){
	CarFramePID.SetDMaxMin(m_Rdi,m_Rda);
}
void SetDeadZone(float m_DeadZone,float m_D_dt_time){
//	CarFramePID.DeadZone = m_DeadZone;
	CarFramePID.SetDeadZone(m_DeadZone,m_D_dt_time);

}
void SetPictcPid(float m_Kp, float m_Ki, float m_Kd)
{
	CarFramePID.SetPID(m_Kp, m_Ki,m_Kd);
}
// *********************************

struct _m_nametab nametab[]=
{

	{(void *)SetPitchPid,"SetPitchPid","SetPitchPid(float m_Kp, float m_Ki, float m_Kd)"},
	{(void *)SetTargetMaxMin,"SetTargetMaxMin","SetTargetMaxMin(float m_Ti,float m_Ta)"},
	{(void *)SetPMaxMin,"SetPMaxMin","SetPMaxMin(float m_Rpi, float m_Rpa)"},
	{(void *)SetIMaxMin,"SetIMaxMin","SetIMaxMin(float m_Rii, float m_Ria)"},
	{(void *)SetDMaxMin,"SetDMaxMin","SetDMaxMin(float m_Rdi, float m_Rda)"},
	{(void *)SetDeadZone,"SetDeadZone","SetDeadZone(float m_DeadZone,u8 m_D_dt_time)"},
	{(void *)Motor_CarFrame,"Motor_CarFrame","Motor_CarFrame(int16_t current_201,int16_t current_202,int16_t current_203,int16_t current_204)"},
	{(void *)test1,"test1","test1(void)"},
	{(void *)test2,"test2","test2(float tem)"},
	{(void*)tips,"tips",""}
};

struct _m_dev dev=
{
	nametab,
	sizeof(nametab)/sizeof(struct _m_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
};

void DebugToolsInit(void)
{
	Initial_System_Timer();
//	UsartDMA_Init();
//	TIMResetModeForDMA();
//	EXTI_Configuration();
}


















