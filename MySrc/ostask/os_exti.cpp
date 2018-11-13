/**
 * RTX系统中EXTI任务中的执行的子函数
 */
#include "RTL.h"
#include "bsp.h"
#include "cpid.h"
#include "filter.h"

extern OS_TID HandleEXTITask;
void ExtiGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	/* 上拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}
void EXTI_CallBack0(void)
{
	isr_evt_set(BIT_0, HandleEXTITask);
	// bsp_LedToggle(1);
	// printf("exti0\r\n");
}
void EXTI_CallBack1(void)
{
	isr_evt_set(BIT_1, HandleEXTITask);
	// bsp_LedToggle(2);
	// printf("exti1\r\n");
}
