#include "bsp.h"
//b11 3r CH4
//a2 2t  CH3

#define Laser_On() bsp_LedOff(3)
#define Laser_Off() bsp_LedOn(3)

u8 Run_Model,STILL_BUTTON,OVERPOWER_BUTTON,ADDBULLET_BUTTON,STARTBYHURT_BUTTON;
void TIM2_PWM_Configuration(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);//打开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//GPIO 初始化
    gpio.GPIO_Pin = GPIO_Pin_3;//PB3 PB11
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_2;//PA2 PA15
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3, GPIO_AF_TIM2);//开启复用
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
    //定时器时钟配置
    tim.TIM_Prescaler = 84-1;//1MHZ
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 20000; //20ms
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&tim);

    oc.TIM_OCMode = TIM_OCMode_PWM2;
    oc.TIM_OutputState = TIM_OutputState_Enable;//输出使能
    oc.TIM_Pulse = 0;
    oc.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OC1Init(TIM2,&oc);
    TIM_OC2Init(TIM2,&oc);
	TIM_OC3Init(TIM2,&oc);
	

    TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	  TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
    

    TIM_ARRPreloadConfig(TIM2,ENABLE);

    TIM_Cmd(TIM2,ENABLE);
    Laser_Off();
	Duoji_Open();
//	Duoji_Close();
}

 void TIM3_PWM_Configuration(void)
 {
 	GPIO_InitTypeDef          gpio;
     TIM_TimeBaseInitTypeDef   tim;
     TIM_OCInitTypeDef         oc;

     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB,ENABLE);//打开时钟
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	//GPIO 初始化
//     gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//PB4 PB5
//     gpio.GPIO_Mode = GPIO_Mode_AF;
//     gpio.GPIO_Speed = GPIO_Speed_100MHz;
//     GPIO_Init(GPIOB,&gpio);

     gpio.GPIO_Pin = GPIO_Pin_9;//PC8 PC9
     gpio.GPIO_Mode = GPIO_Mode_AF;
     gpio.GPIO_Speed = GPIO_Speed_100MHz;
     GPIO_Init(GPIOC,&gpio);

//     GPIO_PinAFConfig(GPIOB,GPIO_PinSource4, GPIO_AF_TIM3);//开启复用
//     GPIO_PinAFConfig(GPIOB,GPIO_PinSource5, GPIO_AF_TIM3);
// 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8, GPIO_AF_TIM3);
     GPIO_PinAFConfig(GPIOC,GPIO_PinSource9, GPIO_AF_TIM3);
     //定时器时钟配置
     tim.TIM_Prescaler = 84-1;//1MHZ
     tim.TIM_CounterMode = TIM_CounterMode_Up;
     tim.TIM_Period = 1000; //20ms
     tim.TIM_ClockDivision = TIM_CKD_DIV1;
     TIM_TimeBaseInit(TIM3,&tim);

     oc.TIM_OCMode = TIM_OCMode_PWM2;
     oc.TIM_OutputState = TIM_OutputState_Enable;//输出使能
     oc.TIM_Pulse = 0;
     oc.TIM_OCPolarity = TIM_OCPolarity_Low;

//     TIM_OC1Init(TIM3,&oc);
//     TIM_OC2Init(TIM3,&oc);
// 	TIM_OC3Init(TIM3,&oc);
 	TIM_OC4Init(TIM3,&oc);

//     TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
//     TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
// 	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
     TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);

     TIM_ARRPreloadConfig(TIM3,ENABLE);

     TIM_Cmd(TIM3,ENABLE);
 }


void ESC_Init(void)
{
	PWM1=2000;//先给最高油门以让电调知道量程
	PWM2=2000;
	delay_ms(6000);
	PWM1=1000;//最低油门
	PWM2=1000;
	delay_ms(6000);
// 	PWM3=650;//
}

void ESC_RUN(u8 pwm1,u8 pwm2)
{
	PWM1=1000+10*pwm1;
	PWM2=1000+10*pwm2;
// 	PWM3=1000+10*pwm3;
// 	PWM4=1000+10*pwm4;
}

void ESC_STOP(void)
{
	PWM1=1000;//最低油门
	PWM2=1000;
// 	PWM3=1000;
// 	PWM4=1000;
	Set_ESC_Flag(0);
	Laser_Off();
}

u8 ESC_Flag=0;
extern u8 ESC_PWM1,ESC_PWM2;
void ESC_Toggle(void)
{
  
	ESC_Flag = !ESC_Flag;
    if(ESC_Flag == 1)
    {
        ESC_RUN(ESC_PWM1,ESC_PWM2);
        Laser_On();
        
    }
    else
    {
        ESC_STOP();
        Laser_Off();
	}
}
u8 Get_ESC_Flag(void)
{
    return ESC_Flag;
}
u8 Set_ESC_Flag(u8 flag)
{
    ESC_Flag = flag;
	return 0;
}

extern const u16 DUOJI_OPEN,DUOJI_CLOSE;
void Duoji_Open(void)
{

	PWM3=DUOJI_OPEN;
}

void Duoji_Close(void)
{
	PWM3=DUOJI_CLOSE;
}

void Duoji_Toggle(void)
{
    static u8 flag=1;
    if(flag == 0)
    {
        Duoji_Open();
        flag = !flag;
    }
    else
    {
        Duoji_Close();
        flag = !flag;
    }
}

void Model_Select_Initial(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}
