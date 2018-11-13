
/*********************************************************************************************************
*
*	模块名称 : CAN网络
*	文件名称 : bsp_can.h
*	版    本 : V1.2
*	说    明 : 头文件
*	1)不可以同一个过滤器连接到不同的FIFO中
*	2)采用FIFO的写法，发送检测阶段无论多小的
*	检查间隔都不会丢包，发送端一定是完整的把所有要发的数据准确无误地发出去
*   3)采用了FIFO，不能使用CAN的发送中断,为了实时性,接收不用FIFO，
*********************************************************************************************************
*/

#ifndef _BSP_CAN_H
#define _BSP_CAN_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "bsp.h"


/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/
#define USE_CAN1
#define USE_CAN2
#define CAN2_StartBank	14  //CAN2Filter的起始存储区,
							// #0 时，不会为 CAN1 分配任何筛选器。
							// #28 时，可以使用 CAN1 的所有筛选器。
							// #14 时CAN1(0~13),CAN2(14~27)
#ifdef USE_CAN1
	/* CAN1的引脚，时钟定义 */
	#define CAN1_RX_PIN                 GPIO_Pin_11
	#define CAN1_RX_SOURCE              GPIO_PinSource11
	#define CAN1_GPIO_RX_PORT           GPIOA
	#define CAN1_GPIO_RX_CLK            RCC_AHB1Periph_GPIOA

	#define CAN1_TX_PIN                 GPIO_Pin_12
	#define CAN1_TX_SOURCE              GPIO_PinSource12
	#define CAN1_GPIO_TX_PORT           GPIOA
	#define CAN1_GPIO_TX_CLK            RCC_AHB1Periph_GPIOA

	#define _CAN1_TTCM_  DISABLE			/* 禁止时间触发模式（不生成时间戳), T  */
	#define _CAN1_ABOM_  ENABLE			    /* 开启自动总线关闭管理 */
	#define _CAN1_AWUM_  DISABLE			/* 禁止自动唤醒模式 */
	#define _CAN1_NART_  ENABLE		    	/* 禁止仲裁丢失或出错后的自动重传功能 (禁止自动重发送寄存器，ENABLE为不重发)*/
	#define _CAN1_RFLM_  DISABLE			/* 禁止接收FIFO加锁模式 */
	#define _CAN1_TXFP_  DISABLE			/* 禁止传输FIFO优先级 */
	#define _CAN1_Mode_  CAN_Mode_Normal	/* 设置CAN为正常工作模式 */
#endif
#ifdef USE_CAN2
	/* CAN2的引脚，时钟定义 */
	#define CAN2_RX_PIN                 GPIO_Pin_12
	#define CAN2_RX_SOURCE              GPIO_PinSource12
	#define CAN2_GPIO_RX_PORT           GPIOB
	#define CAN2_GPIO_RX_CLK            RCC_AHB1Periph_GPIOB

	#define CAN2_TX_PIN                 GPIO_Pin_13
	#define CAN2_TX_SOURCE              GPIO_PinSource13
	#define CAN2_GPIO_TX_PORT           GPIOB
	#define CAN2_GPIO_TX_CLK            RCC_AHB1Periph_GPIOB

	#define _CAN2_TTCM_  DISABLE			/* 禁止时间触发模式（不生成时间戳), T  */
	#define _CAN2_ABOM_  ENABLE				/* 开启自动总线关闭管理 */
	#define _CAN2_AWUM_  DISABLE			/* 禁止自动唤醒模式 */
	#define _CAN2_NART_  ENABLE			/* 禁止仲裁丢失或出错后的自动重传功能 (禁止自动重发送寄存器，ENABLE为不重发)*/
	#define _CAN2_RFLM_  DISABLE			/* 禁止接收FIFO加锁模式 */
	#define _CAN2_TXFP_  DISABLE			/* 禁止传输FIFO优先级 */
	#define _CAN2_Mode_  CAN_Mode_Normal	/* 设置CAN为正常工作模式 */
#endif
/************************************************************************************************/
/**************************************** 接口分割线 ********************************************/
/************************************************************************************************/


/************************************************************************************************/
/**************************************** 筛选器配置 ********************************************/
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
/**************************************** 筛选器配置 ********************************************/
/************************************************************************************************/


#define CAN_send PUSH_CAN_FIFO //将CAN_send用于发送

#define mbx_arr_declare(name,arr_num,cnt) uint32_t name[arr_num][4 + cnt]

typedef enum
{
	CAN1_T  = CAN1_TX_IRQn,//因为用了FIFO来发送，不需要用到发生中断
	CAN1_R0 = CAN1_RX0_IRQn,
	CAN1_R1 = CAN1_RX1_IRQn,
	CAN2_T  = CAN2_TX_IRQn,//因为用了FIFO来发送，不需要用到发生中断
	CAN2_R0 = CAN2_RX0_IRQn,
	CAN2_R1 = CAN2_RX1_IRQn,
}CAN_INT_E;

#define CAN_FIFO_SIZE 1024
typedef struct
{
	CanTxMsg _CanBuf[CAN_FIFO_SIZE];
	uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	uint16_t usTxRead;			/* 发送缓冲区读指针 */
	uint16_t usTxCount;			/* 等待发送的数据个数 */
	CanTxMsg _tmeptosend[3];	//用来暂存当前发送的邮箱里面的值
	uint8_t MailBoxFlag;	    //初始状态需要检查
}CAN_T;


/* 供外部调用的函数声明 */
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


