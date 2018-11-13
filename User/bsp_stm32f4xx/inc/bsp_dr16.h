#ifndef __DR16_
#define __DR16_

#ifdef __cplusplus
 extern "C" {
#endif
//#include "sys.h"
#include "bsp.h"
#include <RTL.h>

#define DR16_msgpool_declare(name,cnt)     uint32_t name[cnt*(sizeof(RC_Ctl_t)/4) + 3]

//extern U32 DR16_msgpool_declare[CAN_CTRL_MAX_NUM*(CAN_No_SendObjects+2*CAN_No_ReceiveObjects)*(sizeof(CAN_msg)/4) + 3];

/* Externaly declared mailbox, for CAN transmit messages                     */
extern U32 dr16_mailbox[4+1024];
/* Externaly declared mailbox, for CAN receive messages                      */



/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/

#define RCC_DR16 				RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_DMA1
#define RCC_Periph_DR16 		RCC_APB1Periph_USART2
#define GPIO_AF_DR16 			GPIO_AF_USART2
#define USART_DR16 		   		USART2
#define GPIO_PinSource_DR16  	GPIO_PinSource3
#define GPIO_Pin_DR16 			GPIO_Pin_3
#define GPIO_PORT_DR16 			GPIOA

#define DMA_Stream_DR16			DMA1_Stream5
#define DMA_StreamIT_TCIFx_DR16	DMA_IT_TCIF5
#define DMA_Channel_DR16		DMA_Channel_4

#define DMA_IRQn_DR16			DMA1_Stream5_IRQn
#define DMA_IRQHandler_DR16		DMA1_Stream5_IRQHandler

/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)
/* ----------------------- Data Struct ------------------------------------- */

typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;

typedef __packed struct
{
	uint64_t ch0:11;
	uint64_t ch1:11;
	uint64_t ch2:11;
	uint64_t ch3:11;
	uint64_t s2:2;
	uint64_t s1:2;
	int64_t x:16;
	int64_t y:16;
	int64_t z:16;
	uint64_t press_l:8;
	uint64_t press_r:8;
	uint64_t key:16;
}RC_Value;

void bsp_RC_Init(void);
void DR16_mailbox_init(void);
uint8_t DR16_receive(RC_Value **ptrmsg, uint16_t timeout);
#ifdef __cplusplus
}
#endif
#endif
