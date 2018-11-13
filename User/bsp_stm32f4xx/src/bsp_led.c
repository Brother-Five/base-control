#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

#ifdef __LED1
	bsp_LedOff(1);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);
#endif

#ifdef __LED2
	bsp_LedOff(2);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);
#endif

#ifdef __LED3
	bsp_LedOff(3);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
	GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);
#endif

#ifdef __LED4
	bsp_LedOff(4);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
	GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	switch (_no)
	{
#ifdef __LED1
		case 1:GPIO_PORT_LED1->BSRRH = GPIO_PIN_LED1;break;
#endif

#ifdef __LED2
		case 2:GPIO_PORT_LED2->BSRRH = GPIO_PIN_LED2;break;
#endif

#ifdef __LED3
		case 3:GPIO_PORT_LED3->BSRRH = GPIO_PIN_LED3;break;
#endif

#ifdef __LED4
		case 4:GPIO_PORT_LED4->BSRRH = GPIO_PIN_LED4;break;
#endif
		default:printf("LED%d未开启却被使用\r\n",_no);

	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	switch (_no)
	{
#ifdef __LED1
		case 1:GPIO_PORT_LED1->BSRRL = GPIO_PIN_LED1;break;
#endif

#ifdef __LED2
		case 2:GPIO_PORT_LED2->BSRRL = GPIO_PIN_LED2;break;
#endif

#ifdef __LED3
		case 3:GPIO_PORT_LED3->BSRRL = GPIO_PIN_LED3;break;
#endif

#ifdef __LED4
		case 4:GPIO_PORT_LED4->BSRRL = GPIO_PIN_LED4;break;
#endif
		default:printf("LED%d未开启却被使用\r\n",_no);
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	switch (_no)
	{
#ifdef __LED1
		case 1:GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;break;
#endif

#ifdef __LED2
		case 2:GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;break;
#endif

#ifdef __LED3
		case 3:GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;break;
#endif

#ifdef __LED4
		case 4:GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;break;
#endif
		default:printf("LED%d未开启却被使用\r\n",_no);
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	switch (_no)
	{
#ifdef __LED1
		case 1:	if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 0) return 1;break;
#endif


#ifdef __LED2
		case 2:	if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 0) return 1;break;
#endif


#ifdef __LED3
		case 3:	if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) == 0) return 1;break;
#endif


#ifdef __LED4
		case 4:	if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) == 0) return 1;break;
#endif
		default:printf("LED%d未开启却被使用\r\n",_no);

	}
	return 0;
}

