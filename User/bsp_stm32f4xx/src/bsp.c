#include "bsp.h"
#include "SetParameter.h"
#include "Kalman.h"
/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	
	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为168MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/
	UsasrFrameInit();
	bsp_InitUart();
	bsp_InitLed();
	bsp_ParaInit();
//	bsp_InitSoftTimer();
	bsp_InitHardTimer();
//	bsp_InitUart(); 	/* 初始化串口 */
//	bsp_InitKey();
//	bsp_InitEXTI();
	
	Initial_System_Timer();
	/* 针对不同的应用程序，添加需要的底层驱动模块初始化函数 */
	bsp_RC_Init();
	
	bsp_InitCAN();

	TIM2_PWM_Configuration();
	TIM3_PWM_Configuration();
	Turn_PlateMotor_IO_Configuratuion();
	Turnplate_motor_cofiguration();
//	bsp_InitEXTI();
//	ExtiGpioInit();
	Hardware_Random_Init();
//	UsartFrame_Init();
	Model_Select_Initial();
	Run_Model=MODEL_BUTTON1*8+MODEL_BUTTON2*4+MODEL_BUTTON3*2+MODEL_BUTTON4;
	STILL_BUTTON=0;
	OVERPOWER_BUTTON=1;
	ADDBULLET_BUTTON=0;
	STARTBYHURT_BUTTON=1;
	
	while(MPU_Init())
	{
		u1_printf("MPU Init Error\r\n");
	}
//	while(1)
//	{
//		{
//		short gx,gy,gz;
//		MPU_Get_Gyroscope(&gx,&gy,&gz);
//		u1_printf("%d\t%d\t%d\r\n",gx,gy,gz);
//		}
//		
//	}

}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理
*			的事务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	// bsp_KeyScan();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的
*			事务可以放在此函数。比如：触摸坐标扫描。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
	//TOUCH_Scan();	/* 触摸扫描 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 */

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 对于 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 对于 uIP 协议实现，可以插入uip轮询函数 */
}
