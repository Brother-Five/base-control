#include "bsp_HardTimer.h"
#include "bsp.h"


// /* ���� TIM��ʱ�жϵ���ִ�еĻص�����ָ�� */
// static void (*s_TIM_CallBack1)(void);
// static void (*s_TIM_CallBack2)(void);
// static void (*s_TIM_CallBack3)(void);
// static void (*s_TIM_CallBack4)(void);

/* ����ÿ�����ڽṹ����� */
#ifdef USE_TIM2
	static TIM_T g_tTIM2;
#endif
#ifdef USE_TIM3
    static TIM_T g_tTIM3;
#endif
#ifdef USE_TIM4
    static TIM_T g_tTIM4;
#endif
#ifdef USE_TIM5
    static TIM_T g_tTIM5;
#endif



void bsp_InitHardTimer(void)
{
    TIMVarInit();
    InitHardTIM();
    ConfigTIMNVIC();
}
/*
*********************************************************************************************************
*	�� �� ��: TIMVarInit
*	����˵��: ��ʼ��TIM��صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIMVarInit(void)
{
#ifdef USE_TIM2
    g_tTIM2._uMode[0]          = ONCE_MODE;
    g_tTIM2._uMode[1]          = ONCE_MODE;
    g_tTIM2._uMode[2]          = ONCE_MODE;
    g_tTIM2._uMode[3]          = ONCE_MODE;
    g_tTIM2._Prescaler     = 84 - 1;                  /*1us */
    g_tTIM2._Period        = 0xFFFFFFFF;              /* ��� */
    g_tTIM2._uTimeOut[0]    = 0;
    g_tTIM2._uTimeOut[1]    = 0;
    g_tTIM2._uTimeOut[2]    = 0;
    g_tTIM2._uTimeOut[3]    = 0;

    g_tTIM2.s_TIM_CallBack1 = 0;
    g_tTIM2.s_TIM_CallBack2 = 0;
    g_tTIM2.s_TIM_CallBack3 = 0;
    g_tTIM2.s_TIM_CallBack4 = 0;
#endif

#ifdef USE_TIM3
    g_tTIM3._uMode[0]          = ONCE_MODE;
    g_tTIM3._uMode[1]          = ONCE_MODE;
    g_tTIM3._uMode[2]          = ONCE_MODE;
    g_tTIM3._uMode[3]          = ONCE_MODE;
    g_tTIM3._Prescaler     = 84 - 1;                  /*1us */
    g_tTIM3._Period        = 0xFFFF;              /* ��� */
    g_tTIM3._uTimeOut[0]    = 0;
    g_tTIM3._uTimeOut[1]    = 0;
    g_tTIM3._uTimeOut[2]    = 0;
    g_tTIM3._uTimeOut[3]    = 0;

    g_tTIM3.s_TIM_CallBack1 = 0;
    g_tTIM3.s_TIM_CallBack2 = 0;
    g_tTIM3.s_TIM_CallBack3 = 0;
    g_tTIM3.s_TIM_CallBack4 = 0;
#endif

#ifdef USE_TIM4
    g_tTIM4._uMode[0]          = ONCE_MODE;
    g_tTIM4._uMode[1]          = ONCE_MODE;
    g_tTIM4._uMode[2]          = ONCE_MODE;
    g_tTIM4._uMode[3]          = ONCE_MODE;
    g_tTIM4._Prescaler     = 84 - 1;                  /*1us */
    g_tTIM4._Period        = 0xFFFF;              /* ��� */
    g_tTIM4._uTimeOut[0]    = 0;
    g_tTIM4._uTimeOut[1]    = 0;
    g_tTIM4._uTimeOut[2]    = 0;
    g_tTIM4._uTimeOut[3]    = 0;

    g_tTIM4.s_TIM_CallBack1 = 0;
    g_tTIM4.s_TIM_CallBack2 = 0;
    g_tTIM4.s_TIM_CallBack3 = 0;
    g_tTIM4.s_TIM_CallBack4 = 0;
#endif

#ifdef USE_TIM5
    g_tTIM5._uMode[0]       = ONCE_MODE;
    g_tTIM5._uMode[1]       = ONCE_MODE;
    g_tTIM5._uMode[2]       = ONCE_MODE;
    g_tTIM5._uMode[3]       = ONCE_MODE;
    g_tTIM5._Prescaler     = TIM5_Prescaler;                  /*1us */
    g_tTIM5._Period        = TIM5_Period;              /* ��� */
    g_tTIM5._uTimeOut[0]    = 0;
    g_tTIM5._uTimeOut[1]    = 0;
    g_tTIM5._uTimeOut[2]    = 0;
    g_tTIM5._uTimeOut[3]    = 0;

    g_tTIM5.s_TIM_CallBack1 = 0;
    g_tTIM5.s_TIM_CallBack2 = 0;
    g_tTIM5.s_TIM_CallBack3 = 0;
    g_tTIM5.s_TIM_CallBack4 = 0;

#endif
}

/*
*********************************************************************************************************
*   �� �� ��: bsp_InitHardTimer
*   ����˵��: ���� TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.
*           TIMx������TIM2 - TIM5 ֮���TIM, ��ЩTIM��4��ͨ��, ���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void InitHardTIM(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* ʹ��TIMʱ�� */
    // #ifdef (USE_TIM2||USE_TIM3||USE_TIM4||USE_TIM5||USE_TIM6||USE_TIM7||USE_TIM13||USE_TIM14)



    /*-----------------------------------------------------------------------
        system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

        HCLK = SYSCLK / 1     (AHB1Periph)
        PCLK2 = HCLK / 2      (APB2Periph)
        PCLK1 = HCLK / 4      (APB1Periph)

        ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
        ��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

        APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
        APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

    ----------------------------------------------------------------------- */
#ifdef USE_TIM2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM2_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);/* TIMx enable counter */
#endif

#ifdef USE_TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM3_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM3, ENABLE);/* TIMx enable counter */
#endif

#ifdef USE_TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM4_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM4, ENABLE);/* TIMx enable counter */
#endif

#ifdef USE_TIM5
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIM5_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM5_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM5, ENABLE);/* TIMx enable counter */
#endif
}

/*********************************************************************************************************
*   �� �� ��: ConfigTIMNVIC
*   ����˵��: ����TIMӲ���ж�.
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfigTIMNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    /*  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- �� bsp.c �� bsp_Init() �������ж����ȼ��� */

#ifdef USE_TIM2
    /* ʹ��TIM2�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM2_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM2_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM3
    /* ʹ��TIM3�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM3_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM3_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM4
    /* ʹ��TIM4�ж�t */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM4_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM4_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM5
    /* ʹ��TIM5�ж�t */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM5_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM5_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM2-5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ�����������š�
*             ��ʱ��������10us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*			 TIM2��TIM5 ��32λ��ʱ������ʱ��Χ�ܴ�
*			 TIM3��TIM4 ��16λ��ʱ����
*	��    ��: _CC : ����ͨ������1��2��3, 4
*             _uiTimeOut : ��ʱʱ��, ��λ 1us.       ����16λ��ʱ������� 65.5ms; ����32λ��ʱ������� 4294��
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(TIM_TypeDef *TIMx,uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack,MODE_T _mode)
{
    TIM_T *T;
    switch ((uint32_t)TIMx)
    {

#ifdef USE_TIM2
        case (uint32_t)TIM2: T = &g_tTIM2;break;
#endif

#ifdef USE_TIM3
        case (uint32_t)TIM3: T = &g_tTIM3;break;
#endif

#ifdef USE_TIM4
        case (uint32_t)TIM4: T = &g_tTIM4;break;
#endif

#ifdef USE_TIM5
        case (uint32_t)TIM5: T = &g_tTIM5;break;
#endif
		default:printf("��TIMδ����ȴ��ʹ��\r\n");
    }

    T->_uTimeOut[_CC -1] = TIM_GetCounter(TIMx) + _uiTimeOut;
    T->_uMode[_CC -1]    = _mode;
    if (_CC == 1)
    {

        T->s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIMx, T->_uTimeOut[_CC -1]);      	/* ���ò���Ƚϼ�����CC1 */
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC1);
		TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	/* ʹ��CC1�ж� */

    }
    else if (_CC == 2)
    {

		T->s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIMx, T->_uTimeOut[_CC -1]);      	/* ���ò���Ƚϼ�����CC2 */
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC2);
		TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	/* ʹ��CC2�ж� */
    }
    else if (_CC == 3)
    {

        T->s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIMx, T->_uTimeOut[_CC -1]);      	/* ���ò���Ƚϼ�����CC3 */
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC3);
		TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	/* ʹ��CC3�ж� */
    }
    else if (_CC == 4)
    {

        T->s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIMx, T->_uTimeOut[_CC -1]);      	/* ���ò���Ƚϼ�����CC4 */
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC4);
		TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	/* ʹ��CC4�ж� */
    }

}



/*
*********************************************************************************************************
*	�� �� ��: TIMx_IRQHandler
*	����˵��: TIM �жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//void TIM2_IRQHandler(void)
//void TIM3_IRQHandler(void)
//void TIM4_IRQHandler(void)

#ifdef USE_TIM2
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
       if(g_tTIM2._uMode[0])
           TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE); /* ����CC1�ж� */
       else
            TIM_SetCompare1(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[0]);
        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
//      TIM_SetCounter(TIM2, 0);
        g_tTIM2.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        if(g_tTIM2._uMode[1])
           TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE); /* ����CC2�ж� */
        else
            TIM_SetCompare2(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[1]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM2.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        if(g_tTIM2._uMode[2])
           TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE); /* ����CC3�ж� */
        else
            TIM_SetCompare3(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[2]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
       g_tTIM2.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        if(g_tTIM2._uMode[3])
           TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE); /* ����CC4�ж� */
        else
            TIM_SetCompare4(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[3]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM2.s_TIM_CallBack4();
    }
}
#endif

#ifdef USE_TIM3
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
       if(g_tTIM3._uMode[0])
           TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE); /* ����CC1�ж� */
       else
            TIM_SetCompare1(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[0]);
        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
//      TIM_SetCounter(TIM3, 0);
        g_tTIM3.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        if(g_tTIM3._uMode[1])
           TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE); /* ����CC2�ж� */
        else
            TIM_SetCompare2(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[1]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM3.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        if(g_tTIM3._uMode[2])
           TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE); /* ����CC3�ж� */
        else
            TIM_SetCompare3(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[2]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
       g_tTIM3.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        if(g_tTIM3._uMode[3])
           TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE); /* ����CC4�ж� */
        else
            TIM_SetCompare4(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[3]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM3.s_TIM_CallBack4();
    }
}
#endif

#ifdef USE_TIM4
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
       if(g_tTIM4._uMode[0])
           TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE); /* ����CC1�ж� */
       else
            TIM_SetCompare1(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[0]);
        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
//      TIM_SetCounter(TIM4, 0);
        g_tTIM4.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
        if(g_tTIM4._uMode[1])
           TIM_ITConfig(TIM4, TIM_IT_CC2, DISABLE); /* ����CC2�ж� */
        else
            TIM_SetCompare2(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[1]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM4.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
        if(g_tTIM4._uMode[2])
           TIM_ITConfig(TIM4, TIM_IT_CC3, DISABLE); /* ����CC3�ж� */
        else
            TIM_SetCompare3(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[2]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
       g_tTIM4.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
        if(g_tTIM4._uMode[3])
           TIM_ITConfig(TIM4, TIM_IT_CC4, DISABLE); /* ����CC4�ж� */
        else
            TIM_SetCompare4(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[3]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM4.s_TIM_CallBack4();
    }
}
#endif


#ifdef USE_TIM5
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
       if(g_tTIM5._uMode[0] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);	/* ����CC1�ж� */
       else
            TIM_SetCompare1(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[0]);
        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
//		TIM_SetCounter(TIM5, 0);
        g_tTIM5.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
        if(g_tTIM5._uMode[1] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);	/* ����CC2�ж� */
        else
            TIM_SetCompare2(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[1]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM5.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
        if(g_tTIM5._uMode[2] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE); /* ����CC3�ж� */
        else
            TIM_SetCompare3(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[2]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
       g_tTIM5.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
        if(g_tTIM5._uMode[3] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE); /* ����CC4�ж� */
        else
            TIM_SetCompare4(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[3]);

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        g_tTIM5.s_TIM_CallBack4();
    }
}

#endif

static void (*s_TIM1_CallBack)(void);
void TIM1_timer(uint32_t _uiTimeOut, void * _pCallBack){

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	s_TIM1_CallBack = (void (*)(void))_pCallBack;
	
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///ʹ��TIM1ʱ��

    TIM_TimeBaseInitStructure.TIM_Period = _uiTimeOut;     //�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler= 168-1;  //�ֳ� 1M ��ʱ�� ��֤ÿ������Ϊ1us
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1

    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��1�����ж�
    TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1

    NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //��ʱ��3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
//	s_TIM1_CallBack = (void (*)(void))_pCallBack;
}
//��ʱ��3�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
    {
		s_TIM1_CallBack();
    }
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
}

