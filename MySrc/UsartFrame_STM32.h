#ifndef _USARTFRAME_STM32_H___
#define _USARTFRAME_STM32_H___

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

extern unsigned int UsartFrame_mailbox[4+1024];

//取指针，将不同类型的类型转化成8位的char型
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define RCC_UsartFrame 				RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_DMA1
#define RCC_Periph_UsartFrame 		RCC_APB1Periph_USART3
#define GPIO_AF_UsartFrame 			GPIO_AF_USART3
#define USART_UsartFrame 		   	USART3

//Rx
#define GPIO_PinSource_Rx_UsartFrame  	GPIO_PinSource11
#define GPIO_Pin_Rx_UsartFrame 			GPIO_Pin_11
#define GPIO_PORT_Rx_UsartFrame 		GPIOC
#define DMA_Stream_Rx_UsartFrame			DMA1_Stream1
#define DMA_StreamIT_Rx_TCIFx_UsartFrame	DMA_IT_TCIF1
#define DMA_Channel_Rx_UsartFrame		    DMA_Channel_4
#define DMA_IRQn_Rx_UsartFrame				USART3_IRQn
#define DMA_IRQHandler_Rx_UsartFrame		USART3_IRQHandler

//Tx
#define GPIO_PinSource_Tx_UsartFrame  	GPIO_PinSource10
#define GPIO_Pin_Tx_UsartFrame 			GPIO_Pin_10
#define GPIO_PORT_Tx_UsartFrame 		GPIOB
#define DMA_Stream_Tx_UsartFrame			DMA1_Stream3
#define DMA_StreamIT_Tx_TCIFx_UsartFrame	DMA_IT_TCIF3
#define DMA_Channel_Tx_UsartFrame		    DMA_Channel_4
#define DMA_IRQn_Tx_UsartFrame				DMA1_Stream3_IRQn
#define DMA_IRQHandler_Tx_UsartFrame		DMA1_Stream3_IRQHandler

#pragma pack (4)

#define SUM  256  //4的整数
typedef struct
{
	unsigned char  SOF;//帧头
	unsigned char  MOD;//(4 - (数据长度%4))
	unsigned short LEN;//数据长度
	unsigned int   CRC32;//CRC32校验码
	unsigned char  DATA[16];//数据BUF
}FRAME;

//typedef struct CV_Stuct
//{
//	struct {
//		int16_t ID;
//		int16_t Flag;
//	}State;
//	int16_t CV_X;
//	int16_t CV_Y;
//}CVStuct;

typedef struct CV_ALL_Stuct
{
//	struct {
//		int8_t Flag1;
//		int8_t Flag2;
//		int8_t Flag3;
//		int8_t Flag4;
//	}State;
//	int16_t Flag1;
//	int16_t Flag2;

	int16_t Strike_Angle_X;
	int16_t Strike_Angle_Y;
	int16_t Strike_Coordinate_X;
	int16_t Strike_Coordinate_Y;
	int16_t DaFu_Angle_X;
	int16_t DaFu_Angle_Y;
}CVALLStuct;


void UsasrFrameInit(void);
uint32_t CRC_STM32Hard(uint8_t* pBuffer, uint32_t BufferLength);
uint32_t CRC_STM32Check(uint8_t* pBuffer, uint32_t BufferLength , uint32_t CRC32Rec);
void UsasrFrameSend(FRAME * pBuffer);
unsigned char UsasrFrameRec(unsigned char * str);



void UsartFrame_Init(void);
void UsartFrame_mailbox_init(void);
uint8_t UsartFrame_receive(FRAME **ptrmsg, uint16_t timeout);
void USARTFrame_DMA_Send(uint8_t *pbuffer, uint32_t size);
void u3_printf(char* fmt,...);

#ifdef __cplusplus
 }
#endif


#endif

