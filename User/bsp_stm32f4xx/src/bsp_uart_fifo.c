/*********************************************************************************************************
*
*	ģ������ : ����
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : C�ļ�
*	1)ʹ��FIFO��ʽ������
*	2)����DMA���������ͣ�DMA�Ĳ���������
*	3)������FIFO�����ѭ��������
*********************************************************************************************************
*/


#include "bsp.h"

/* ����ÿ�����ڽṹ����� */
/* ����ÿ�����ڽṹ����� */
#if USE_UART1_FIFO_DMA
u8 UsartDmaTxFlag = 0;//1:���ڷ���   0:�������
#endif

#ifdef USE_UART1_FIFO
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#ifdef USE_UART2_FIFO
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#ifdef USE_UART3_FIFO
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#ifdef USE_UART4_FIFO
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#ifdef USE_UART5_FIFO
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#ifdef USE_UART6_FIFO
	static UART_T g_tUart6;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ���ջ����� */
#endif

static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);
static void ConfigUartNVIC(void);


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */

	InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

	ConfigUartNVIC();	/* ���ô����ж� */
#if	USE_UART1_FIFO_DMA
	UsartDMA_Init();
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: uartָ��
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#ifdef USE_UART1_FIFO
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#ifdef USE_UART2_FIFO
			return &g_tUart2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#ifdef USE_UART3_FIFO
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#ifdef USE_UART4_FIFO
			return &g_tUart4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#ifdef USE_UART5_FIFO
			return &g_tUart5;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM6)
	{
		#ifdef USE_UART6_FIFO
			return &g_tUart6;
		#else
			return 0;
		#endif
	}
	else
	{
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: comSendBuf
*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucaBuf: �����͵����ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	// if (pUart->SendBefor != 0)
	// {
	// 	pUart->SendBefor();		/* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
	// }

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: comSendChar
*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucByte: �����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	�� �� ��: comGetChar
*	����˵��: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _pByte: ���յ������ݴ���������ַ
*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}


/*
*********************************************************************************************************
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#ifdef USE_UART1_FIFO
	g_tUart1.uart = USART1;						/* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart1.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usTxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart1.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart1.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart1.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart1.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#ifdef USE_UART2_FIFO
	g_tUart2.uart = USART2;						/* STM32 �����豸 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* ���ͻ�����ָ�� */
	g_tUart2.pRxBuf = g_RxBuf2;					/* ���ջ�����ָ�� */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart2.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usTxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usRxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart2.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart2.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart2.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart2.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#ifdef USE_UART3_FIFO
	g_tUart3.uart = USART3;						/* STM32 �����豸 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* ���ͻ�����ָ�� */
	g_tUart3.pRxBuf = g_RxBuf3;					/* ���ջ�����ָ�� */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart3.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usTxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usRxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart3.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart3.SendBefor = 0;		/* ��������ǰ�Ļص����� */
	g_tUart3.SendOver = 0;			/* ������Ϻ�Ļص����� */
	g_tUart3.ReciveNew = 0;		/* ���յ������ݺ�Ļص����� */
//	g_tUart3.SendBefor = RS485_SendBefor;		/* ��������ǰ�Ļص����� */
//	g_tUart3.SendOver = RS485_SendOver;			/* ������Ϻ�Ļص����� */
//	g_tUart3.ReciveNew = RS485_ReciveNew;		/* ���յ������ݺ�Ļص����� */
#endif

#ifdef USE_UART4_FIFO
	g_tUart4.uart = UART4;						/* STM32 �����豸 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* ���ͻ�����ָ�� */
	g_tUart4.pRxBuf = g_RxBuf4;					/* ���ջ�����ָ�� */
	g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart4.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart4.usTxRead = 0;						/* ����FIFO������ */
	g_tUart4.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart4.usRxRead = 0;						/* ����FIFO������ */
	g_tUart4.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart4.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart4.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart4.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart4.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#ifdef USE_UART5_FIFO
	g_tUart5.uart = UART5;						/* STM32 �����豸 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* ���ͻ�����ָ�� */
	g_tUart5.pRxBuf = g_RxBuf5;					/* ���ջ�����ָ�� */
	g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart5.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart5.usTxRead = 0;						/* ����FIFO������ */
	g_tUart5.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart5.usRxRead = 0;						/* ����FIFO������ */
	g_tUart5.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart5.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart5.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart5.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart5.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif


#ifdef USE_UART6_FIFO
	g_tUart6.uart = USART6;						/* STM32 �����豸 */
	g_tUart6.pTxBuf = g_TxBuf6;					/* ���ͻ�����ָ�� */
	g_tUart6.pRxBuf = g_RxBuf6;					/* ���ջ�����ָ�� */
	g_tUart6.usTxBufSize = UART6_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart6.usRxBufSize = UART6_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart6.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart6.usTxRead = 0;						/* ����FIFO������ */
	g_tUart6.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart6.usRxRead = 0;						/* ����FIFO������ */
	g_tUart6.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart6.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart6.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart6.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart6.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

#ifdef USE_UART1_FIFO		/* ����1 TX = PA9   RX = PA10 �� TX = PB6   RX = PB7*/

	/* ��1���� ����GPIO */
	#if UART1_PIN == 0	/* TX = PA9   RX = PA10 */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* �� PA9 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

		/* �� PA10 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#elif UART1_PIN == 1	/* TX = PB6   RX = PB7  */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* �� PB6 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* �� PB7 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

#ifdef USE_UART2_FIFO		/* ����2 TX = PD5   RX = PD6 ��  TX = PA2�� RX = PA3  */
	/* ��1���� ����GPIO */
	#if UART2_PIN == 0	/* ����2 TX = PD5   RX = PD6 */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* �� PD5 ӳ��Ϊ USART2_TX */
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

		/* �� PD6 ӳ��Ϊ USART2_RX */
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

	#elif UART2_PIN == 1	/* ����2   TX = PA2�� RX = PA3 */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* �� PA2 ӳ��Ϊ USART2_TX. ��STM32-V5���У�PA2 �ܽ�������̫�� */
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

		/* �� PA3 ӳ��Ϊ USART2_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART2_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;		/* ��ѡ�����ģʽ */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART2, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

#ifdef USE_UART3_FIFO			/* ����3 TX = PB10   RX = PB11 */

	#if UART3_PIN ==0	/* ����3 TX = PB10   RX = PB11 */
	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* �� PB10 ӳ��Ϊ USART3_TX */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

	/* �� PB11 ӳ��Ϊ USART3_RX */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	#elif UART3_PIN ==1  /* ����3 TX = PD8   RX = PD9 */
		/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* �� PB10 ӳ��Ϊ USART3_TX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	/* �� PB11 ӳ��Ϊ USART3_RX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	#elif UART3_PIN ==2 /* ����3 TX = PC10   RX = PC11 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* �� PB10 ӳ��Ϊ USART3_TX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);

	/* �� PB11 ӳ��Ϊ USART3_RX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART3_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */

#endif

#ifdef USE_UART4_FIFO			/* ����4 TX = PC10   RX = PC11 */
	/* ��1���� ����GPIO */

	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* �� PC10 ӳ��Ϊ UART4_TX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART1);

	/* �� PC11 ӳ��Ϊ UART4_RX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART1);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(UART4, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART4, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

#ifdef USE_UART5_FIFO			/* ����5 TX = PC12   RX = PD2 */
	/* ��1���� ����GPIO */

	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* �� PC12 ӳ��Ϊ UART5_TX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

	/* �� PD2 ӳ��Ϊ UART5_RX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

	/* ���� UART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ���� UART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART5_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(UART5, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

#ifdef USE_UART6_FIFO			/* PG14/USART6_TX , PC7/USART6_RX,PG8/USART6_RTS, PG15/USART6_CTS */
	/* ��1���� ����GPIO */

	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOG, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	/* �� PG14 ӳ��Ϊ USART6_TX */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);

	/* �� PC7 ӳ��Ϊ USART6_RX */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	/* �� PG8 ӳ��Ϊ USART6_RTS */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource8, GPIO_AF_USART6);

	/* �� PG15 ӳ��Ϊ USART6_CTS */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_USART6);

	/* ���� PG14/USART6_TX Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* ���� PC7/USART6_RX Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ���� PG8/USART6_RTS Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* ���� PG15/USART6_CTS Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART6_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;	/* ѡ��Ӳ������ */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART6, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART6, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: ConfigUartNVIC
*	����˵��: ���ô���Ӳ���ж�.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	/*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- �� bsp.c �� bsp_Init() �������ж����ȼ��� */

#ifdef USE_UART1_FIFO
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART1_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART1_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_UART2_FIFO
	/* ʹ�ܴ���2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART2_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART2_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_UART3_FIFO
	/* ʹ�ܴ���3�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART3_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART3_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_UART4_FIFO
	/* ʹ�ܴ���4�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART4_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART4_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_UART5_FIFO
	/* ʹ�ܴ���5�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART5_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART5_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef USE_UART6_FIFO
	/* ʹ�ܴ���6�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_UART6_Pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = Priority_UART6_Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* ������ͻ������Ѿ����ˣ���ȴ��������� */

		/* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
		while (1)
		{
			uint16_t usCount;

			DISABLE_INT();
			usCount = _pUart->usTxCount;
			ENABLE_INT();

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
#if USE_UART1_FIFO_DMA
			UsartDMAIRQ();		//����FIFO�����ʹ������ݣ���Ҫ��ʱ��ѯFIFO�Ƿ�������Ҫ��
#endif
		}


		/* �����������뷢�ͻ����� */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

		DISABLE_INT();
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
		ENABLE_INT();
	}
	// UsartDMAIRQ();
//	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);//��ʹ��DMAʱ


#if !USE_UART1_FIFO_DMA
	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);//��ʹ��DMAʱ
#else

	if(_pUart != &g_tUart1)//ʹ��DMAʱ
		USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
	else return;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	u8 clear = clear;
	if (USART_GetITStatus(_pUart->uart, USART_IT_IDLE) != RESET)//�����ж�,
	{
		run(g_tUart1.pRxBuf);

//		printf("NUM:%d\r\n",dlfnalsdf);
//		printf("NUM:%d\r\n",g_tUart1.usRxCount);
//		run(g_tUart1.pRxBuf);
//		printf("NUM:%d\r\n",g_tUart1.usRxCount);
		DMA_Cmd(DMA2_Stream2,DISABLE);//DMAʧ��
		while(DMA_GetCmdStatus(DMA2_Stream2));//����Ƿ�ʧ�ܳɹ���DMAʧ��ʱ��Ҫ�ȴ�����ʱ���ʧ�ܳɹ�
		DMA_SetCurrDataCounter(DMA2_Stream2,UART1_RX_BUF_SIZE);//���ݴ�����
		DMA_Cmd(DMA2_Stream2,ENABLE);//DMA����ʹ��
		TIM_Cmd(TIM1,DISABLE);

		clear = USART1->SR;//������Ϊ��������жϱ�־
		clear = USART1->DR;//������Ϊ��������жϱ�־
	}
	/* ��������ж�  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
		uint8_t ch;

		ch = USART_ReceiveData(_pUart->uart);
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}

		/* �ص�����,֪ͨӦ�ó����յ�������,һ���Ƿ���1����Ϣ��������һ����� */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew(ch);
			}
		}
	}

	/* �����ͻ��������ж� */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

			/* ʹ�����ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
	/* ����bitλȫ��������ϵ��ж� */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

			/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
			// if (_pUart->SendOver)
			// {
			// 	_pUart->SendOver();
			// }
		}
		else
		{
			/* ��������£��������˷�֧ */

			/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}





/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	comSendChar(COM1, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 0	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}

void UasrtCheckFifo(void)
{
	printf("1:%d\r\n",g_tUart1.usTxCount);
}


#if USE_UART1_FIFO_DMA
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
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_tUart1.pRxBuf;
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
		// DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);//���˸÷�������ҪDMA�ж�
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
		DMA_Cmd(DMA2_Stream7,DISABLE);//��ʼ��ʱҪʧ�ܡ��������ܣ�����
	}
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//�����ж�,���ڲ���������

}

/**
 * [USART1_DMA_Send ͨ��DMA�򴮿ڷ��Ͳ������ȵ����ݣ�����������֪��]
 * @param pbuffer [���ݵĴ���ָ��]
 * @param size    [���ݵĳ���]
 *  �磺 USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
 */
void USART1_DMA_Send(uint8_t *pbuffer, uint32_t size)
{

	if(UsartDmaTxFlag == 0)//����������ɺ���ܿ�ʼ����
	{
		DMA_Cmd (DMA2_Stream7,DISABLE);
		while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}
	//	DMA2_Stream7->M0AR =  (u32)pbuffer;
	//	DMA2_Stream7->NDTR = size;
		DMA_MemoryTargetConfig(DMA2_Stream7,(u32)pbuffer,DMA_Memory_0);
		DMA_SetCurrDataCounter(DMA2_Stream7,size);
	 	DMA_Cmd (DMA2_Stream7,ENABLE);//ʹ��DMA,��ʼ����
		UsartDmaTxFlag=1; //���ݷ�����
		UsartDMAIRQ();		//����FIFO�����ʹ������ݣ���Ҫ��ʱ��ѯFIFO�Ƿ�������Ҫ��
	}
}

void UsartDMAIRQ(void)
{
	if(g_tUart1.usTxCount != 0)
	{
		USART1_DMA_Send(g_tUart1.pTxBuf, g_tUart1.usTxCount);
		comClearTxFifo(COM1);
	}

}

#endif
/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#ifdef USE_UART1_FIFO
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif

#ifdef USE_UART2_FIFO
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

#ifdef USE_UART3_FIFO
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}
#endif

#ifdef USE_UART4_FIFO
void UART4_IRQHandler(void)
{
	UartIRQ(&g_tUart4);
}
#endif

#ifdef USE_UART5_FIFO
void UART5_IRQHandler(void)
{
	UartIRQ(&g_tUart5);
}
#endif

#ifdef USE_UART6_FIFO
void USART6_IRQHandler(void)
{
	UartIRQ(&g_tUart6);
}
#endif

#if USE_UART1_FIFO_DMA
void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{
		UsartDmaTxFlag = 0;
		// printf("ok\r\n");
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);

	}
}


#endif

#ifdef __cplusplus
}
#endif

