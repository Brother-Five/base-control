
/*********************************************************************************************************
*
*	ģ������ : CAN����
*	�ļ����� : bsp_can.h
*	��    �� : V1.2
*	˵    �� : ͷ�ļ�
*	1)������ͬһ�����������ӵ���ͬ��FIFO��
*	2)����FIFO��д�������ͼ��׶����۶�С��
*	����������ᶪ�������Ͷ�һ���������İ�����Ҫ��������׼ȷ����ط���ȥ
*   3)������FIFO������ʹ��CAN�ķ����ж�,Ϊ��ʵʱ��,���ղ���FIFO��
*********************************************************************************************************
*/

#ifndef _BSP_CAN_H
#define _BSP_CAN_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "bsp.h"


/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/
#define USE_CAN1
#define USE_CAN2
#define CAN2_StartBank	14  //CAN2Filter����ʼ�洢��,
							// #0 ʱ������Ϊ CAN1 �����κ�ɸѡ����
							// #28 ʱ������ʹ�� CAN1 ������ɸѡ����
							// #14 ʱCAN1(0~13),CAN2(14~27)
#ifdef USE_CAN1
	/* CAN1�����ţ�ʱ�Ӷ��� */
	#define CAN1_RX_PIN                 GPIO_Pin_11
	#define CAN1_RX_SOURCE              GPIO_PinSource11
	#define CAN1_GPIO_RX_PORT           GPIOA
	#define CAN1_GPIO_RX_CLK            RCC_AHB1Periph_GPIOA

	#define CAN1_TX_PIN                 GPIO_Pin_12
	#define CAN1_TX_SOURCE              GPIO_PinSource12
	#define CAN1_GPIO_TX_PORT           GPIOA
	#define CAN1_GPIO_TX_CLK            RCC_AHB1Periph_GPIOA

	#define _CAN1_TTCM_  DISABLE			/* ��ֹʱ�䴥��ģʽ��������ʱ���), T  */
	#define _CAN1_ABOM_  ENABLE			    /* �����Զ����߹رչ��� */
	#define _CAN1_AWUM_  DISABLE			/* ��ֹ�Զ�����ģʽ */
	#define _CAN1_NART_  ENABLE		    	/* ��ֹ�ٲö�ʧ��������Զ��ش����� (��ֹ�Զ��ط��ͼĴ�����ENABLEΪ���ط�)*/
	#define _CAN1_RFLM_  DISABLE			/* ��ֹ����FIFO����ģʽ */
	#define _CAN1_TXFP_  DISABLE			/* ��ֹ����FIFO���ȼ� */
	#define _CAN1_Mode_  CAN_Mode_Normal	/* ����CANΪ��������ģʽ */
#endif
#ifdef USE_CAN2
	/* CAN2�����ţ�ʱ�Ӷ��� */
	#define CAN2_RX_PIN                 GPIO_Pin_12
	#define CAN2_RX_SOURCE              GPIO_PinSource12
	#define CAN2_GPIO_RX_PORT           GPIOB
	#define CAN2_GPIO_RX_CLK            RCC_AHB1Periph_GPIOB

	#define CAN2_TX_PIN                 GPIO_Pin_13
	#define CAN2_TX_SOURCE              GPIO_PinSource13
	#define CAN2_GPIO_TX_PORT           GPIOB
	#define CAN2_GPIO_TX_CLK            RCC_AHB1Periph_GPIOB

	#define _CAN2_TTCM_  DISABLE			/* ��ֹʱ�䴥��ģʽ��������ʱ���), T  */
	#define _CAN2_ABOM_  ENABLE				/* �����Զ����߹رչ��� */
	#define _CAN2_AWUM_  DISABLE			/* ��ֹ�Զ�����ģʽ */
	#define _CAN2_NART_  ENABLE			/* ��ֹ�ٲö�ʧ��������Զ��ش����� (��ֹ�Զ��ط��ͼĴ�����ENABLEΪ���ط�)*/
	#define _CAN2_RFLM_  DISABLE			/* ��ֹ����FIFO����ģʽ */
	#define _CAN2_TXFP_  DISABLE			/* ��ֹ����FIFO���ȼ� */
	#define _CAN2_Mode_  CAN_Mode_Normal	/* ����CANΪ��������ģʽ */
#endif
/************************************************************************************************/
/**************************************** �ӿڷָ��� ********************************************/
/************************************************************************************************/


/************************************************************************************************/
/**************************************** ɸѡ������ ********************************************/
/************************************************************************************************/

#define CanFilter_0		(0  << 3)
#define CanFilter_1		(1  << 3)
#define CanFilter_2		(2  << 3)
#define CanFilter_3		(3  << 3)
#define CanFilter_4		(4  << 3)
#define CanFilter_5		(5  << 3)
#define CanFilter_6		(6  << 3)
#define CanFilter_7		(7  << 3)
#define CanFilter_8		(8  << 3)
#define CanFilter_9		(9  << 3)
#define CanFilter_10	(10 << 3)
#define CanFilter_11	(11 << 3)
#define CanFilter_12	(12 << 3)
#define CanFilter_13	(13 << 3)
#define CanFilter_14	(14 << 3)
#define CanFilter_15	(15 << 3)
#define CanFilter_16	(16 << 3)
#define CanFilter_17	(17 << 3)
#define CanFilter_18	(18 << 3)
#define CanFilter_19	(19 << 3)
#define CanFilter_20	(20 << 3)
#define CanFilter_21	(21 << 3)
#define CanFilter_22	(22 << 3)
#define CanFilter_23	(23 << 3)
#define CanFilter_24	(24 << 3)
#define CanFilter_25	(25 << 3)
#define CanFilter_26	(26 << 3)
#define CanFilter_27	(27 << 3)

#define CanFifo_0 		(0 << 2)
#define CanFifo_1 		(1 << 2)

#define Can_STDID  		(0 << 1)
#define Can_EXTID   	(1 << 1)

#define Can_DataType    (0 << 0)
#define Can_RemoteType  (1 << 0)

/************************************************************************************************/
/**************************************** ɸѡ������ ********************************************/
/************************************************************************************************/


#define CAN_send PUSH_CAN_FIFO //��CAN_send���ڷ���

#define mbx_arr_declare(name,arr_num,cnt) uint32_t name[arr_num][4 + cnt]

typedef enum
{
	CAN1_T  = CAN1_TX_IRQn,//��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
	CAN1_R0 = CAN1_RX0_IRQn,
	CAN1_R1 = CAN1_RX1_IRQn,
	CAN2_T  = CAN2_TX_IRQn,//��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
	CAN2_R0 = CAN2_RX0_IRQn,
	CAN2_R1 = CAN2_RX1_IRQn,
}CAN_INT_E;

#define CAN_FIFO_SIZE 1024
typedef struct
{
	CanTxMsg _CanBuf[CAN_FIFO_SIZE];
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */
	CanTxMsg _tmeptosend[3];	//�����ݴ浱ǰ���͵����������ֵ
	uint8_t MailBoxFlag;	    //��ʼ״̬��Ҫ���
}CAN_T;


/* ���ⲿ���õĺ������� */
//void hardinit(void);
void bsp_InitCAN(void);
void InitHardCAN(void);
void CanFifoInit(void);
void CANCheckFifo(void);
// void can_filter_Mask_config(uint8_t _filterid,uint8_t _fifox,uint32_t IdHigh,uint32_t MaskIdHigh);
// void can_nvic_config(CAN_INT_E _temp,void * _pcallback);
void can_filter_Mask_config(uint8_t object_para,uint32_t Id,uint32_t MaskId);
void can_nvic_config(CAN_INT_E _temp);//,void * _pcallback

uint8_t POP_CAN_FIFO(CAN_TypeDef* CANx,CanTxMsg *Msgtemp);
void PUSH_CAN_FIFO(CAN_TypeDef* CANx,CanTxMsg *Msgtemp);
void CAN_FIFO_ToSend(CAN_TypeDef* CANx);
void CAN_FIFO_IRQ(void);
void CAN_TransmitForMailbox(CAN_TypeDef* CANx,uint8_t transmit_mailbox, CanTxMsg* TxMessage);
uint8_t MyCAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox);

void CAN_mailbox_init(uint32_t ctrl);
uint8_t CAN_receive(uint32_t ctrl, uint8_t _fifo,CanRxMsg **msg, uint16_t timeout);//CAN_msg

#ifdef __cplusplus
}
#endif
#endif


