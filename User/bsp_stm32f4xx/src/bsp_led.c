#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

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
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
		default:printf("LED%dδ����ȴ��ʹ��\r\n",_no);

	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
		default:printf("LED%dδ����ȴ��ʹ��\r\n",_no);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
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
		default:printf("LED%dδ����ȴ��ʹ��\r\n",_no);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
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
		default:printf("LED%dδ����ȴ��ʹ��\r\n",_no);

	}
	return 0;
}

