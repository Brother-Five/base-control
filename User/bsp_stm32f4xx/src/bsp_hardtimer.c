#include "bsp_HardTimer.h"
#include "bsp.h"


// /* 保存 TIM定时中断到后执行的回调函数指针 */
// static void (*s_TIM_CallBack1)(void);
// static void (*s_TIM_CallBack2)(void);
// static void (*s_TIM_CallBack3)(void);
// static void (*s_TIM_CallBack4)(void);

/* 定义每个串口结构体变量 */
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
*	函 数 名: TIMVarInit
*	功能说明: 初始化TIM相关的变量
*	形    参: 无
*	返 回 值: 无
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
    g_tTIM2._Period        = 0xFFFFFFFF;              /* 最大 */
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
    g_tTIM3._Period        = 0xFFFF;              /* 最大 */
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
    g_tTIM4._Period        = 0xFFFF;              /* 最大 */
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
    g_tTIM5._Period        = TIM5_Period;              /* 最大 */
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
*   函 数 名: bsp_InitHardTimer
*   功能说明: 配置 TIMx，用于us级别硬件定时。TIMx将自由运行，永不停止.
*           TIMx可以用TIM2 - TIM5 之间的TIM, 这些TIM有4个通道, 挂在 APB1 上，输入时钟=SystemCoreClock / 2
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
void InitHardTIM(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 使能TIM时钟 */
    // #ifdef (USE_TIM2||USE_TIM3||USE_TIM4||USE_TIM5||USE_TIM6||USE_TIM7||USE_TIM13||USE_TIM14)



    /*-----------------------------------------------------------------------
        system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

        HCLK = SYSCLK / 1     (AHB1Periph)
        PCLK2 = HCLK / 2      (APB2Periph)
        PCLK1 = HCLK / 4      (APB1Periph)

        因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
        因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

        APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
        APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

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
*   函 数 名: ConfigTIMNVIC
*   功能说明: 配置TIM硬件中断.
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
static void ConfigTIMNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    /*  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- 在 bsp.c 中 bsp_Init() 中配置中断优先级组 */

#ifdef USE_TIM2
    /* 使能TIM2中断 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM2_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM2_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM3
    /* 使能TIM3中断 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM3_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM3_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM4
    /* 使能TIM4中断t */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM4_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM4_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_TIM5
    /* 使能TIM5中断t */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_TIM5_Pre;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_TIM5_Sub;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}
/*
*********************************************************************************************************
*	函 数 名: bsp_StartHardTimer
*	功能说明: 使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器，互不干扰。
*             定时精度正负10us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
*			 TIM2和TIM5 是32位定时器。定时范围很大
*			 TIM3和TIM4 是16位定时器。
*	形    参: _CC : 捕获通道几，1，2，3, 4
*             _uiTimeOut : 超时时间, 单位 1us.       对于16位定时器，最大 65.5ms; 对于32位定时器，最大 4294秒
*             _pCallBack : 定时时间到后，被执行的函数
*	返 回 值: 无
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
		default:printf("有TIM未开启却被使用\r\n");
    }

    T->_uTimeOut[_CC -1] = TIM_GetCounter(TIMx) + _uiTimeOut;
    T->_uMode[_CC -1]    = _mode;
    if (_CC == 1)
    {

        T->s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIMx, T->_uTimeOut[_CC -1]);      	/* 设置捕获比较计数器CC1 */
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC1);
		TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	/* 使能CC1中断 */

    }
    else if (_CC == 2)
    {

		T->s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIMx, T->_uTimeOut[_CC -1]);      	/* 设置捕获比较计数器CC2 */
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC2);
		TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	/* 使能CC2中断 */
    }
    else if (_CC == 3)
    {

        T->s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIMx, T->_uTimeOut[_CC -1]);      	/* 设置捕获比较计数器CC3 */
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC3);
		TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	/* 使能CC3中断 */
    }
    else if (_CC == 4)
    {

        T->s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIMx, T->_uTimeOut[_CC -1]);      	/* 设置捕获比较计数器CC4 */
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC4);
		TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	/* 使能CC4中断 */
    }

}



/*
*********************************************************************************************************
*	函 数 名: TIMx_IRQHandler
*	功能说明: TIM 中断服务程序
*	形    参：无
*	返 回 值: 无
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
           TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE); /* 禁能CC1中断 */
       else
            TIM_SetCompare1(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[0]);
        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
//      TIM_SetCounter(TIM2, 0);
        g_tTIM2.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        if(g_tTIM2._uMode[1])
           TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE); /* 禁能CC2中断 */
        else
            TIM_SetCompare2(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[1]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        g_tTIM2.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        if(g_tTIM2._uMode[2])
           TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE); /* 禁能CC3中断 */
        else
            TIM_SetCompare3(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[2]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
       g_tTIM2.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        if(g_tTIM2._uMode[3])
           TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE); /* 禁能CC4中断 */
        else
            TIM_SetCompare4(TIM2,TIM_GetCounter(TIM2) + g_tTIM2._uTimeOut[3]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
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
           TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE); /* 禁能CC1中断 */
       else
            TIM_SetCompare1(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[0]);
        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
//      TIM_SetCounter(TIM3, 0);
        g_tTIM3.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        if(g_tTIM3._uMode[1])
           TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE); /* 禁能CC2中断 */
        else
            TIM_SetCompare2(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[1]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        g_tTIM3.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        if(g_tTIM3._uMode[2])
           TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE); /* 禁能CC3中断 */
        else
            TIM_SetCompare3(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[2]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
       g_tTIM3.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        if(g_tTIM3._uMode[3])
           TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE); /* 禁能CC4中断 */
        else
            TIM_SetCompare4(TIM3,TIM_GetCounter(TIM3) + g_tTIM3._uTimeOut[3]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
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
           TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE); /* 禁能CC1中断 */
       else
            TIM_SetCompare1(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[0]);
        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
//      TIM_SetCounter(TIM4, 0);
        g_tTIM4.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
        if(g_tTIM4._uMode[1])
           TIM_ITConfig(TIM4, TIM_IT_CC2, DISABLE); /* 禁能CC2中断 */
        else
            TIM_SetCompare2(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[1]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        g_tTIM4.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
        if(g_tTIM4._uMode[2])
           TIM_ITConfig(TIM4, TIM_IT_CC3, DISABLE); /* 禁能CC3中断 */
        else
            TIM_SetCompare3(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[2]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
       g_tTIM4.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
        if(g_tTIM4._uMode[3])
           TIM_ITConfig(TIM4, TIM_IT_CC4, DISABLE); /* 禁能CC4中断 */
        else
            TIM_SetCompare4(TIM4,TIM_GetCounter(TIM4) + g_tTIM4._uTimeOut[3]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
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
           TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);	/* 禁能CC1中断 */
       else
            TIM_SetCompare1(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[0]);
        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
//		TIM_SetCounter(TIM5, 0);
        g_tTIM5.s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
        if(g_tTIM5._uMode[1] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);	/* 禁能CC2中断 */
        else
            TIM_SetCompare2(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[1]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        g_tTIM5.s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
        if(g_tTIM5._uMode[2] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE); /* 禁能CC3中断 */
        else
            TIM_SetCompare3(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[2]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
       g_tTIM5.s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM5, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
        if(g_tTIM5._uMode[3] == ONCE_MODE)
           TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE); /* 禁能CC4中断 */
        else
            TIM_SetCompare4(TIM5,TIM_GetCounter(TIM5) + g_tTIM5._uTimeOut[3]);

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        g_tTIM5.s_TIM_CallBack4();
    }
}

#endif

static void (*s_TIM1_CallBack)(void);
void TIM1_timer(uint32_t _uiTimeOut, void * _pCallBack){

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	s_TIM1_CallBack = (void (*)(void))_pCallBack;
	
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM1时钟

    TIM_TimeBaseInitStructure.TIM_Period = _uiTimeOut;     //自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler= 168-1;  //分出 1M 的时钟 保证每个周期为1us
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
    TIM_Cmd(TIM1,ENABLE); //使能定时器1

    NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
//	s_TIM1_CallBack = (void (*)(void))_pCallBack;
}
//定时器3中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
    {
		s_TIM1_CallBack();
    }
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}

