/*********************************************************************************************************
*
*	模块名称 : 串口
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*	1)使用FIFO方式来发送
*	2)采用DMA不定长发送，DMA的不定长接收
*	3)修正了FIFO溢出死循环的问题
*********************************************************************************************************
*/


#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#ifdef __cplusplus
 extern "C" {
#endif

//#include "..\SYSTEM\sys\sys.h"

/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/
/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
#define	USE_UART1_FIFO
//#define	USE_UART2_FIFO
//#define	USE_UART3_FIFO
//#define	USE_UART4_FIFO
//#define	USE_UART5_FIFO
//#define	USE_UART6_FIFO

#define USE_UART1_FIFO_DMA 1
/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */


#ifdef USE_UART1_FIFO
	#define UART1_PIN			0  //0:TX = PA9;RX = PA10  1:TX = PB6   RX = PB7
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#ifdef USE_UART2_FIFO
	#define UART2_PIN			0  //0:TX = PD5;RX = PD6  1:TX = PA2   RX = PA3
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	1
	#define UART2_RX_BUF_SIZE	2*1024
#endif

#ifdef USE_UART3_FIFO
	#define UART3_PIN			2  //0:TX = PB10;RX = PB11  1:TX = PD8   RX = PD9 2:TX = PC10   RX = PC11
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#ifdef USE_UART4_FIFO  	   	 	/* 串口4 TX = PC10   RX = PC11 */
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#ifdef USE_UART5_FIFO 			/* 串口5 TX = PC12   RX = PD2 */
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#ifdef USE_UART6_FIFO 			/* PG14/USART6_TX , PC7/USART6_RX,PG8/USART6_RTS, PG15/USART6_CTS */
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif
/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/


/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 或  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 或 PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 */
}COM_PORT_E;

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	uint16_t usTxRead;			/* 发送缓冲区读指针 */
	uint16_t usTxCount;			/* 等待发送的数据个数 */

	uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	uint16_t usRxRead;			/* 接收缓冲区读指针 */
	uint16_t usRxCount;			/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;

void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);
void UasrtCheckFifo(void);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

void UsartDMA_Init(void);
void USART1_DMA_Send(uint8_t *pbuffer, uint32_t size);
void UsartDMAIRQ(void);
#ifdef __cplusplus
}
#endif
#endif

