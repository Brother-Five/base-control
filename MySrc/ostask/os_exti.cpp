/**
 * RTXϵͳ��EXTI�����е�ִ�е��Ӻ���
 */
#include "RTL.h"
#include "bsp.h"
#include "cpid.h"
#include "filter.h"

extern OS_TID HandleEXTITask;
void ExtiGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	/* �������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */
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
