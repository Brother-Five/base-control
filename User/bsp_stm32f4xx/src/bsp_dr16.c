#include "bsp.h"
#include <RTL.h>                      /* RTX kernel functions & defines      */
#include "bsp_can.h"                      /* RTX kernel functions & defines      */


// DR16_msgpool_declare(DR16_mpool,20);
os_mbx_declare(dr16_mailbox,1024);

/* ------ ----------------- Internal Data ----------------------------------- */
volatile unsigned char sbus_rx_buffer[25];
//static RC_Ctl_t RC_Ctl;

// u16 channel_0=1024,channel_1=1024,channel_2=1024,channel_3=1024;
// unsigned char OutEnableFlag = 1;
// extern CPID PitchPID,YawPID,CarFramePID;
/* ----------------------- Function Implements ---------------------------- */
/******************************************************************************
* @fn RC_Init
*
* @brief configure stm32 USART2 port
* - USART Parameters
* - 100Kbps
* - 8-N-1
* - DMA Mode
*
* @return None.
*
* @note This code is fully tested on STM32F405RGT6 Platform, You can port it
* to the other platform.
*/
void bsp_RC_Init(void)
{
//	u8 ptrmsg;
	/* -------------- Enable Module Clock Source ----------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_DR16, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_Periph_DR16, ENABLE);
	GPIO_PinAFConfig(GPIO_PORT_DR16,GPIO_PinSource_DR16, GPIO_AF_DR16);

	/* dr16_dr16_mailbox 初始化*/
//	ptrmsg = 1; //!< Channel 0

//	os_mbx_send (dr16_mailbox, &ptrmsg,200);
	/* -------------- Configure GPIO ---------------------------------------*/
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DR16 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIO_PORT_DR16, &GPIO_InitStructure);
		USART_DeInit(USART_DR16);
		USART_InitStructure.USART_BaudRate = 100000;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_Mode = USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART_DR16,&USART_InitStructure);
		USART_Cmd(USART_DR16,ENABLE);
		USART_DMACmd(USART_DR16,USART_DMAReq_Rx,ENABLE);
	}
	/* -------------- Configure NVIC ---------------------------------------*/
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQn_DR16;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_DR16_DMA_Pre;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = Priority_DR16_DMA_Sub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	/* -------------- Configure DMA -----------------------------------------*/
	{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA_Stream_DR16);
		DMA_InitStructure.DMA_Channel = DMA_Channel_DR16;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART_DR16->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = 18;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA_Stream_DR16,&DMA_InitStructure);
		DMA_ITConfig(DMA_Stream_DR16,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA_Stream_DR16,ENABLE);
	}
}


void DR16_mailbox_init(void)
{
	os_mbx_init (&dr16_mailbox, sizeof(dr16_mailbox));//DR16邮箱初始化
}

uint8_t DR16_receive(RC_Value **ptrmsg, uint16_t timeout)
{
	return os_mbx_wait(dr16_mailbox, (void **)ptrmsg,200);
}
/******************************************************************************
* @fn DMA_IRQHandler_DR16
*
* @brief USART_DR16 DMA ISR
*
* @return None.
*
* @note This code is fully tested on STM32F405RGT6 Platform, You can port it
* to the other platform.
*/

//单片机中断不能用C++来编译，否则程序会跑死
#ifdef __cplusplus
 extern "C" {
#endif
//S键 ：上1 中3 下2

void DMA_IRQHandler_DR16(void)
{
//	static RC_Value ptrmsg;
	if(DMA_GetITStatus(DMA_Stream_DR16, DMA_StreamIT_TCIFx_DR16))
	{
//		printf("key:%d\tmouse.x:%d\tmouse.y:%d\tmouse.z:%d\tpress_l:%d\tpress_r:%d\t\r\n",((temp*)sbus_rx_buffer)->key,((temp*)sbus_rx_buffer)->x ,((temp*)sbus_rx_buffer)->y,((temp*)sbus_rx_buffer)->z,((temp*)sbus_rx_buffer)->press_l,((temp*)sbus_rx_buffer)->press_r);
//		printf("ch0:%d\tch1:%d\tch2:%d\tch3:%d\t\r\n",((temp*)sbus_rx_buffer)->ch0,((temp*)sbus_rx_buffer)->ch1,((temp*)sbus_rx_buffer)->ch2,((temp*)sbus_rx_buffer)->ch3);

		if (isr_mbx_check (&dr16_mailbox) != 0)
		{
//				ptrmsg.rc.ch0= (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
//				ptrmsg.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
//				ptrmsg.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |(sbus_rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
//				ptrmsg.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
//				ptrmsg.rc.s1 =  ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
//				ptrmsg.rc.s2 =  ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right

//				ptrmsg.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
//				ptrmsg.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
//				ptrmsg.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
//				ptrmsg.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
//				ptrmsg.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
//				ptrmsg.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
				// printf("ch0:%d\tch1:%d\tch2:%d\tch3:%d\t\r\n",ptrmsg.rc.ch0,ptrmsg.rc.ch1,ptrmsg.rc.ch2,ptrmsg.rc.ch3);
//				printf("key:%d\tmouse.x:%d\tmouse.y:%d\tmouse.z:%d\tpress_l:%d\tpress_r:%d\t\r\n",ptrmsg.key.v,ptrmsg.mouse.x ,ptrmsg.mouse.y,ptrmsg.mouse.z,ptrmsg.mouse.press_l,ptrmsg.mouse.press_r);
				isr_mbx_send (&dr16_mailbox,(void *)sbus_rx_buffer);
		}
		DMA_ClearITPendingBit(DMA_Stream_DR16, DMA_StreamIT_TCIFx_DR16);
	}
}
#ifdef __cplusplus
}
#endif
