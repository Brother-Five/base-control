/*********************************************************************************************************
*
*	模块名称 : 串口
*	文件名称 : bsp_uart.h
*	版    本 : V1.0
*	说    明 : C文件
*	1)采用DMA不定长发送，DMA的不定长接收
*********************************************************************************************************
*/


#include "bsp.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"


static u8 UsartDmaTxFlag = 0;//1:正在发送   0:发送完成

static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
__align(8) u8 USART1_TX_BUF[2048]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节

static void InitHardUart(void);
static void ConfigUartNVIC(void);


/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{

	InitHardUart();		/* 配置串口的硬件参数(波特率等) */
	ConfigUartNVIC();	/* 配置串口中断 */
	UsartDMA_Init();
}

/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* 第1步： 配置GPIO */
	#if UART1_PIN == 0	/* TX = PA9   RX = PA10 */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 将 PA9 映射为 USART1_TX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

		/* 将 PA10 映射为 USART1_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#elif UART1_PIN == 1	/* TX = PB6   RX = PB7  */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 将 PB6 映射为 USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* 将 PB7 映射为 USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
//	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */

}

/*
*********************************************************************************************************
*	函 数 名: ConfigUartNVIC
*	功能说明: 配置串口硬件中断.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART1_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART1_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 0	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	comSendChar(COM1, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 0	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}


void UsartDMA_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	USART_Cmd(USART1,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	/* -------------- Configure NVIC ---------------------------------------*/
//	{

		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
//	}
	/* -------------- Configure DMA_RX -----------------------------------------*/
	{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA2_Stream2);
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_RxBuf1;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = UART1_RX_BUF_SIZE;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2,&DMA_InitStructure);
		// DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);//用了该方法后不需要DMA中断
		DMA_Cmd(DMA2_Stream2,ENABLE);
	}
	/* -------------- Configure DMA_TX -----------------------------------------*/
	{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA2_Stream7);
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
//		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)table;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//		DMA_InitStructure.DMA_BufferSize = 30;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream7,&DMA_InitStructure);
//		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream7,DISABLE);//初始化时要失能。。。不能！！！
	}
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//空闲中断,用于不定长接收

}

/**
 * [USART1_DMA_Send 通过DMA向串口发送不定长度的数据，看函数名就知道]
 * @param pbuffer [数据的传递指针]
 * @param size    [数据的长度]
 *  如： USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
 */
void USART1_DMA_Send(uint8_t *pbuffer, uint32_t size)
{

	if(UsartDmaTxFlag == 0)//发送数据完成后才能开始发送
	{
		DMA_Cmd (DMA2_Stream7,DISABLE);
		while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}
	//	DMA2_Stream7->M0AR =  (u32)pbuffer;
	//	DMA2_Stream7->NDTR = size;
		DMA_MemoryTargetConfig(DMA2_Stream7,(u32)pbuffer,DMA_Memory_0);
		DMA_SetCurrDataCounter(DMA2_Stream7,size);
	 	DMA_Cmd (DMA2_Stream7,ENABLE);//使能DMA,开始发送
		UsartDmaTxFlag=1; //数据发送中
	}
}

/**
 * [u1_printf printf 函数]
 */
void u1_printf(char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	USART1_DMA_Send(USART1_TX_BUF, strlen((const char*)USART1_TX_BUF));
//	for(j=0;j<i;j++)//循环发送数据
//	{
//	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);  //等待上次传输完成
//		USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]); 	 //发送数据到串口3
//	}
}

/**
 * [USART_DMA_RX_LEN 返回串口DMA不定长接收的接收长度]
 * @param  DMAy_Streamx [串口DMA]
 * @param  BufSize      [BUF的长度]
 * @return              [接收数据的长度]
 */
uint16_t USART_DMA_RX_LEN(DMA_Stream_TypeDef* DMAy_Streamx,uint16_t BufSize)
{
	return (BufSize - DMAy_Streamx->NDTR);
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

//extern DJM djm;
void USART1_IRQHandler(void)
{
	
	u8 clear = clear;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲中断,
	{
//		djm = *(DJM*)(g_RxBuf1);
//		ANO_Data1_Send(0xf1,djm.current*djm.volate*100);
//		u1_printf("V:%d\tI:%d\tP:%d\r\n",(int)(djm.volate*100),(int)(djm.current*100),(int)(djm.current*djm.volate*100));
//		u1_printf("%f\t%f\t%d\t%d\t%f\t%f\t%d\t%d\t%d\t\r\n",djm.shoot_speed,djm.shoot_frequency,djm.time,djm.blood,djm.volate,djm.current,\
//		djm.id1,djm.id1,djm.id1);
//		if(g_RxBuf1[0] == 0xaa && g_RxBuf1[1] == 0x55)
//			UsasrFrameRec(g_RxBuf1);
//		else
			run(g_RxBuf1);
//		u1_printf("%d\r\n",USART_DMA_RX_LEN(DMA2_Stream2,UART1_RX_BUF_SIZE));

		DMA_Cmd(DMA2_Stream2,DISABLE);//DMA失能
		while(DMA_GetCmdStatus(DMA2_Stream2));//检测是否失能成功，DMA失能时需要等待少许时间才失能成功
		DMA_SetCurrDataCounter(DMA2_Stream2,UART1_RX_BUF_SIZE);//数据传输量
		DMA_Cmd(DMA2_Stream2,ENABLE);//DMA重新使能
		clear = USART1->SR;//这两步为清除空闲中断标志
		clear = USART1->DR;//这两步为清除空闲中断标志
	}
}


void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{
		UsartDmaTxFlag = 0;
		// printf("ok\r\n");
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);

	}
}

#ifdef __cplusplus
}
#endif

