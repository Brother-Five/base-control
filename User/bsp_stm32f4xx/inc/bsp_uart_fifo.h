/*********************************************************************************************************
*
*	ģ������ : ����
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	1)ʹ��FIFO��ʽ������
*	2)����DMA���������ͣ�DMA�Ĳ���������
*	3)������FIFO�����ѭ��������
*********************************************************************************************************
*/


#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#ifdef __cplusplus
 extern "C" {
#endif

//#include "..\SYSTEM\sys\sys.h"

/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/
/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
#define	USE_UART1_FIFO
//#define	USE_UART2_FIFO
//#define	USE_UART3_FIFO
//#define	USE_UART4_FIFO
//#define	USE_UART5_FIFO
//#define	USE_UART6_FIFO

#define USE_UART1_FIFO_DMA 1
/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */


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

#ifdef USE_UART4_FIFO  	   	 	/* ����4 TX = PC10   RX = PC11 */
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#ifdef USE_UART5_FIFO 			/* ����5 TX = PC12   RX = PD2 */
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
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/


/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 ��  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 �� PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 */
}COM_PORT_E;

/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	uint16_t usRxRead;			/* ���ջ�������ָ�� */
	uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
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

