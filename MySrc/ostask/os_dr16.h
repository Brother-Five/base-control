#ifndef _OS_DR16_H___
#define _OS_DR16_H___
#include <stm32f4xx.h>


//void os_dr16(void);
void os_dr16(RC_Value *ptrmsg);
short Get_AxicX(void);
short Get_AxicY(void);
short Get_AxicZ(void);
u8 Run_Start(void);
#endif

