#include "bsp.h"

uint32_t _EXTI_MODE=0;
#ifdef USE_EXTI_LINE_0
void (*s_EXTI_LINE0)(void);
#endif

#ifdef USE_EXTI_LINE_1
void (*s_EXTI_LINE1)(void);
#endif

#ifdef USE_EXTI_LINE_2
void (*s_EXTI_LINE2)(void);
#endif

#ifdef USE_EXTI_LINE_3
void (*s_EXTI_LINE3)(void);
#endif

#ifdef USE_EXTI_LINE_4
void (*s_EXTI_LINE4)(void);
#endif

#ifdef USE_EXTI_LINE_5
void (*s_EXTI_LINE5)(void);
#endif

#ifdef USE_EXTI_LINE_6
void (*s_EXTI_LINE6)(void);
#endif

#ifdef USE_EXTI_LINE_7
void (*s_EXTI_LINE7)(void);
#endif

#ifdef USE_EXTI_LINE_8
void (*s_EXTI_LINE8)(void);
#endif

#ifdef USE_EXTI_LINE_9
void (*s_EXTI_LINE9)(void);
#endif

#ifdef USE_EXTI_LINE_10
void (*s_EXTI_LINE10)(void);
#endif

#ifdef USE_EXTI_LINE_11
void (*s_EXTI_LINE11)(void);
#endif

#ifdef USE_EXTI_LINE_12
void (*s_EXTI_LINE12)(void);
#endif

#ifdef USE_EXTI_LINE_13
void (*s_EXTI_LINE13)(void);
#endif

#ifdef USE_EXTI_LINE_14
void (*s_EXTI_LINE14)(void);
#endif

#ifdef USE_EXTI_LINE_15
void (*s_EXTI_LINE15)(void);
#endif



/*
*********************************************************************************************************
*	函 数 名: bsp_InitEXTI
*	功能说明: 无
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitEXTI(void)
{
	void * p;
#ifdef USE_EXTI_LINE_0
	p = EXTI_LINE_0_CallBack;
	ExtiConfiguration(EXTI_LINE_0_PortSource,EXTI_PinSource0,EXTI_LINE_0_Trigger,p,EXTI_LINE_0_Mode);
#endif

#ifdef USE_EXTI_LINE_1
	p = EXTI_LINE_1_CallBack;
	ExtiConfiguration(EXTI_LINE_1_PortSource,EXTI_PinSource1,EXTI_LINE_1_Trigger,p,EXTI_LINE_1_Mode);
#endif
#ifdef USE_EXTI_LINE_2
	p = EXTI_LINE_2_CallBack;
	ExtiConfiguration(EXTI_LINE_2_PortSource,EXTI_PinSource2,EXTI_LINE_2_Trigger,p,EXTI_LINE_2_Mode);
#endif

#ifdef USE_EXTI_LINE_3
	p = EXTI_LINE_3_CallBack;
	ExtiConfiguration(EXTI_LINE_3_PortSource,EXTI_PinSource3,EXTI_LINE_3_Trigger,p,EXTI_LINE_3_Mode);
#endif

#ifdef USE_EXTI_LINE_4
	p = EXTI_LINE_4_CallBack;
	ExtiConfiguration(EXTI_LINE_4_PortSource,EXTI_PinSource4,EXTI_LINE_4_Trigger,p,EXTI_LINE_4_Mode);
#endif

#ifdef USE_EXTI_LINE_5
	p = EXTI_LINE_5_CallBack;
	ExtiConfiguration(EXTI_LINE_5_PortSource,EXTI_PinSource5,EXTI_LINE_5_Trigger,p,EXTI_LINE_5_Mode);
#endif

#ifdef USE_EXTI_LINE_6
	p = EXTI_LINE_6_CallBack;
	ExtiConfiguration(EXTI_LINE_6_PortSource,EXTI_PinSource6,EXTI_LINE_6_Trigger,p,EXTI_LINE_6_Mode);
#endif

#ifdef USE_EXTI_LINE_7
	p = EXTI_LINE_7_CallBack;
	ExtiConfiguration(EXTI_LINE_7_PortSource,EXTI_PinSource7,EXTI_LINE_7_Trigger,p,EXTI_LINE_7_Mode);
#endif

#ifdef USE_EXTI_LINE_8
	p = EXTI_LINE_8_CallBack;
	ExtiConfiguration(EXTI_LINE_8_PortSource,EXTI_PinSource8,EXTI_LINE_8_Trigger,p,EXTI_LINE_8_Mode);
#endif

#ifdef USE_EXTI_LINE_9
	p = EXTI_LINE_9_CallBack;
	ExtiConfiguration(EXTI_LINE_9_PortSource,EXTI_PinSource9,EXTI_LINE_9_Trigger,p,EXTI_LINE_9_Mode);
#endif

#ifdef USE_EXTI_LINE_10
	p = EXTI_LINE_10_CallBack;
	ExtiConfiguration(EXTI_LINE_10_PortSource,EXTI_PinSource10,EXTI_LINE_10_Trigger,p,EXTI_LINE_10_Mode);
#endif

#ifdef USE_EXTI_LINE_11
	p = EXTI_LINE_11_CallBack;
	ExtiConfiguration(EXTI_LINE_11_PortSource,EXTI_PinSource11,EXTI_LINE_11_Trigger,p,EXTI_LINE_11_Mode);
#endif

#ifdef USE_EXTI_LINE_12
	p = EXTI_LINE_12_CallBack;
	ExtiConfiguration(EXTI_LINE_12_PortSource,EXTI_PinSource12,EXTI_LINE_12_Trigger,p,EXTI_LINE_12_Mode);
#endif

#ifdef USE_EXTI_LINE_13
	p = EXTI_LINE_13_CallBack;
	ExtiConfiguration(EXTI_LINE_13_PortSource,EXTI_PinSource13,EXTI_LINE_13_Trigger,p,EXTI_LINE_13_Mode);
#endif

#ifdef USE_EXTI_LINE_14
	p = EXTI_LINE_14_CallBack;
	ExtiConfiguration(EXTI_LINE_14_PortSource,EXTI_PinSource14,EXTI_LINE_14_Trigger,p,EXTI_LINE_14_Mode);
#endif

#ifdef USE_EXTI_LINE_15
	p = EXTI_LINE_15_CallBack;
	ExtiConfiguration(EXTI_LINE_15_PortSource,EXTI_PinSource15,EXTI_LINE_15_Trigger,p,EXTI_LINE_15_Mode);
#endif
}

void ExtiConfiguration(uint8_t EXTI_PortSourceGPIOx,uint8_t EXTI_PinSourcex,EXTITrigger_TypeDef _EXTITrigger,void * _pCallBack,MODE_T _mode)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
	EXTI_InitStructure.EXTI_Line    = 1<<EXTI_PinSourcex;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = _EXTITrigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	_EXTI_MODE |= _mode<<EXTI_PinSourcex;
//	_EXTI_MODE &= !_mode<<EXTI_PinSourcex;
	switch (EXTI_PinSourcex)
	{

#ifdef USE_EXTI_LINE_0
		case 0:s_EXTI_LINE0=_pCallBack;
				NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI0_Pre;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI0_Sub;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				break;
#endif



#ifdef USE_EXTI_LINE_1
		case 1:s_EXTI_LINE1=_pCallBack;
				NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI1_Pre;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI1_Sub;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				break;
#endif


#ifdef USE_EXTI_LINE_2
		case 2:s_EXTI_LINE2=_pCallBack;
				NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI2_Pre;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI2_Sub;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				break;
#endif



#ifdef USE_EXTI_LINE_3
		case 3:s_EXTI_LINE3=_pCallBack;
				NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI3_Pre;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI3_Sub;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				break;
#endif



#ifdef USE_EXTI_LINE_4
		case 4:s_EXTI_LINE4=_pCallBack;
				NVIC_InitStructure.NVIC_IRQChannel                   = EXTI4_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI4_Pre;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI4_Sub;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				break;
#endif



#ifdef USE_EXTI_LINE_5
		case 5:s_EXTI_LINE5=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_6
		case 6:s_EXTI_LINE6=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_7
		case 7:s_EXTI_LINE7=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_8
		case 8:s_EXTI_LINE8=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_9
		case 9:s_EXTI_LINE9=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_10
		case 10:s_EXTI_LINE10=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_11
		case 11:s_EXTI_LINE11=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_12
		case 12:s_EXTI_LINE12=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_13
		case 13:s_EXTI_LINE13=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_14
		case 14:s_EXTI_LINE14=_pCallBack;break;
#endif



#ifdef USE_EXTI_LINE_15
		case 15:s_EXTI_LINE15=_pCallBack;break;
#endif


		default : break;
	}
	if(EXTI_PinSourcex>=5 || EXTI_PinSourcex<=9)
	{
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI9_Pre;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI9_Sub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	if(EXTI_PinSourcex>=10 || EXTI_PinSourcex<=15)
	{
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_EXTI15_Pre;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_EXTI15_Sub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}



}
/**
 * [EXTI_Cmd 外部中断的的屏蔽位操作，官方库没有]
 * @param Linex    [外部中断线]
 *                  EXTI_Line0->EXTI_Line22
 * @param NewState [使能与否]
 *                 	ENABLE DISABLE
 */
void EXTI_Cmd(uint32_t Linex,FunctionalState NewState)
{

	EXTI->PR = Linex; //清除Linex上的中断标志位
	if(NewState)
		EXTI->IMR|=Linex;
	else
		EXTI->IMR&=~Linex;
}
#ifdef USE_EXTI_LINE_0
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line0)//
			EXTI_Cmd(EXTI_Line0,DISABLE);
		s_EXTI_LINE0();
	}
}
#endif

#ifdef USE_EXTI_LINE_1
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line1)//
			EXTI_Cmd(EXTI_Line1,DISABLE);
		s_EXTI_LINE1();
	}
}
#endif

#ifdef USE_EXTI_LINE_2
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line2)//
			EXTI_Cmd(EXTI_Line2,DISABLE);
		s_EXTI_LINE2();
	}
}
#endif

#ifdef USE_EXTI_LINE_3
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line3)//
			EXTI_Cmd(EXTI_Line3,DISABLE);
		s_EXTI_LINE3();
	}
}
#endif

#ifdef USE_EXTI_LINE_4
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line4)//
			EXTI_Cmd(EXTI_Line4,DISABLE);
		s_EXTI_LINE4();
	}
}
#endif


void EXTI9_5_IRQHandler(void)
{
#ifdef USE_EXTI_LINE_5
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line5)//
			EXTI_Cmd(EXTI_Line5,DISABLE);
		s_EXTI_LINE5();
	}
#endif

#ifdef USE_EXTI_LINE_6
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line6)//
			EXTI_Cmd(EXTI_Line6,DISABLE);
		s_EXTI_LINE6();
	}
#endif

#ifdef USE_EXTI_LINE_7
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line7)//
			EXTI_Cmd(EXTI_Line7,DISABLE);
		s_EXTI_LINE7();
	}
#endif

#ifdef USE_EXTI_LINE_8
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line8)//
			EXTI_Cmd(EXTI_Line8,DISABLE);
		s_EXTI_LINE8();
	}
#endif

#ifdef USE_EXTI_LINE_9
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line9)//
			EXTI_Cmd(EXTI_Line9,DISABLE);
		s_EXTI_LINE9();
	}
#endif


}

void EXTI15_10_IRQHandler(void)
{

#ifdef USE_EXTI_LINE_10
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line10)//
			EXTI_Cmd(EXTI_Line10,DISABLE);
		s_EXTI_LINE10();
	}
#endif

#ifdef USE_EXTI_LINE_11
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line11); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line11)//
			EXTI_Cmd(EXTI_Line11,DISABLE);
		s_EXTI_LINE11();
	}
#endif

#ifdef USE_EXTI_LINE_12
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line12)//
			EXTI_Cmd(EXTI_Line12,DISABLE);
		s_EXTI_LINE12();
	}
#endif

#ifdef USE_EXTI_LINE_13
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line13); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line13)//
			EXTI_Cmd(EXTI_Line13,DISABLE);
		s_EXTI_LINE13();
	}
#endif

#ifdef USE_EXTI_LINE_14
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line14); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line14)//
			EXTI_Cmd(EXTI_Line14,DISABLE);
		s_EXTI_LINE14();
	}
#endif

#ifdef USE_EXTI_LINE_15
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15); /* 清除中断标志位 */
		if(_EXTI_MODE&EXTI_Line15)//
			EXTI_Cmd(EXTI_Line15,DISABLE);
		s_EXTI_LINE15();
	}
#endif
}


