#include "bsp.h"

//初始化TIM5 32位定时器，用于做系统的时钟。
void Initial_System_Timer(void){

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM3时钟

    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFFFFFF; 	//自动重装载值，可计数到1.1930464711小时，即是71.582788266分钟
	TIM_TimeBaseInitStructure.TIM_Prescaler= 84-1;  //分出 1M 的时钟 保证每个周期为1us
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5
	TIM_Cmd(TIM5,ENABLE); //使能定时器3
}




//用来测RunTime(StartCheck)和RunTime(StopCheck)间的运行时间，单位为1us
void RunTime(unsigned char flag)
{
	static u32 last_time = 0,now_time = 0;
	u32 dt;

	if(flag == StartCheck)
		last_time = micros(); //读取系统时间
	else if(flag == StopCheck)
	{
		now_time = micros(); //读取系统时间
		if(now_time < last_time)
		{
			dt = (now_time + (0xFFFFFFFF - last_time) );
		}
		else
		{
			dt = (now_time - last_time);

		}
		printf("%d\r\n", dt);
	}

}

void delay_ms_nos(uint32_t cnt)
{
	u32 temp = cnt * 1000 + micros();
	while(temp >= micros());
}

void delay_us_nos(uint32_t cnt)
{
	u32 temp = cnt  + micros();

	while(temp >= micros());
}

