#ifndef _PRIOTITY_H_
#define _PRIOTITY_H_

#ifdef __cplusplus
 extern "C" {
#endif


//USART
#define Priority_UART1_Pre 0
#define Priority_UART1_Sub 1

#define Priority_UART2_Pre 0
#define Priority_UART2_Sub 2

#define Priority_UART3_Pre 0
#define Priority_UART3_Sub 3

#define Priority_UART4_Pre 0
#define Priority_UART4_Sub 4

#define Priority_UART5_Pre 0
#define Priority_UART5_Sub 5

#define Priority_UART6_Pre 0
#define Priority_UART6_Sub 6

//TIM
#define Priority_TIM2_Pre 0
#define Priority_TIM2_Sub 2

#define Priority_TIM3_Pre 0
#define Priority_TIM3_Sub 3

#define Priority_TIM4_Pre 0
#define Priority_TIM4_Sub 4

#define Priority_TIM5_Pre 0
#define Priority_TIM5_Sub 5


//EXTI
#define Priority_EXTI0_Pre 0
#define Priority_EXTI0_Sub 1

#define Priority_EXTI1_Pre 0
#define Priority_EXTI1_Sub 1

#define Priority_EXTI2_Pre 0
#define Priority_EXTI2_Sub 2

#define Priority_EXTI3_Pre 0
#define Priority_EXTI3_Sub 3

#define Priority_EXTI4_Pre 0
#define Priority_EXTI4_Sub 4

#define Priority_EXTI9_Pre 0
#define Priority_EXTI9_Sub 5

#define Priority_EXTI15_Pre 0
#define Priority_EXTI15_Sub 6

//CAN
#define Priority_CAN1_T_Pre 0
#define Priority_CAN1_T_Sub 1

#define Priority_CAN1_R0_Pre 0
#define Priority_CAN1_R0_Sub 2

#define Priority_CAN1_R1_Pre 0
#define Priority_CAN1_R1_Sub 3

#define Priority_CAN2_T_Pre 0
#define Priority_CAN2_T_Sub 0

#define Priority_CAN2_R0_Pre 0
#define Priority_CAN2_R0_Sub 2

#define Priority_CAN2_R1_Pre 0
#define Priority_CAN2_R1_Sub 3

//DR16
#define Priority_DR16_DMA_Pre 1
#define Priority_DR16_DMA_Sub 1


#ifdef __cplusplus
}
#endif
#endif
