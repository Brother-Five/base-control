#include "bsp.h"
#include "cpid.h"
extern SCPID TurnplatePID;
//A:TIM4_CH4 PB9
//B:TIM4_CH3 PB8
//PWM:TIM13_CH1 PA6
//都不用重映射
void Turnplate_motor_cofiguration(void)
{
	GPIO_InitTypeDef          gpio;
//    TIM_TimeBaseInitTypeDef   tim;
//    TIM_OCInitTypeDef         oc;
// 	TIM_ICInitTypeDef		TIM_ICInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);//打开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//GPIO 初始化
    gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PB8 PB9
    gpio.GPIO_Mode = GPIO_Mode_AF;//复用功能
	gpio.GPIO_PuPd = GPIO_PuPd_UP;//输入上拉
//	gpio.GPIO_OType = GPIO_OType_PP;
//  gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&gpio);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_TIM4);//开启复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_TIM4);


//     tim.TIM_ClockDivision = TIM_CKD_DIV1;
//     tim.TIM_CounterMode = TIM_CounterMode_Up;
//     TIM_TimeBaseInit(TIM4, &tim);
//     TIM_ICInitStructure.TIM_ICFilter = 6;         //配置输入滤波器
//     TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	oc.TIM_OCMode = TIM_OCMode_PWM2;
//    oc.TIM_OutputState = TIM_OutputState_Enable;//输出使能
//    oc.TIM_Pulse = 0;
//    oc.TIM_OCPolarity = TIM_OCPolarity_Low;

 	 TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
//     TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	 TIM4->CR2|=0X80;//ch1~ch3异或
	 TIM_Cmd(TIM4,ENABLE);
}

void Turn_PlateMotor_IO_Configuratuion(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_4;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpio);

	Motor_Bin1=1;
	Motor_Bin2=0;
	MOS_PWM=1;
}


void Encoder_Start(void)
{
   TIM4->CNT = 0x7fff;
}


EncoderStuct EncoderData = {0,0};

EncoderStuct Encoder_Get_CNT(void)
{

//    int16_t cnt = 0;
    EncoderData.Speed = TIM4->CNT-0x7fff;
	EncoderData.Position += EncoderData.Speed;
    TIM4->CNT = 0x7fff;
    return EncoderData;
}

// u8 constantly_flag =
//#define RSF 100
u8 ESC_PWM1           = 40;//正常时刻PWM值
u8 ESC_PWM2           = 40;//正常时刻PWM值
u8 ESC_PWM_CONSTANTLY = 50;//连发PWM值
u16 ONCE_FIRE_TIME    = 300;//单发时间
u8 RSF                = 36;//射频

void ESCSetPara(float _mESC_PWM1,float _mESC_PWM2,float _mESC_PWM_CONSTANTLY,float _mONCE_FIRE_TIME,float _mRSF)
{
	ESC_PWM1           = (uint8_t)_mESC_PWM1;
	ESC_PWM2           = (uint8_t)_mESC_PWM2;
	ESC_PWM_CONSTANTLY = (uint8_t)_mESC_PWM_CONSTANTLY;
	ONCE_FIRE_TIME     = (uint16_t)_mONCE_FIRE_TIME;
	RSF          = (uint8_t)_mRSF;
	u1_printf("1:%d\t2:%d\tcon:%d\ttime:%d\tRSF:%d",ESC_PWM1,ESC_PWM2,ESC_PWM_CONSTANTLY,ONCE_FIRE_TIME,RSF);
}


void Turnplate_motor_drive(u8 flag)
{
	if(flag==0x0f)//正常发弹
	{
		TurnplatePID.SetTarget(RSF);
		// Turnplate_motor_PWM=10;
		delay_ms(ONCE_FIRE_TIME);

		TurnplatePID.SetTarget(-10);
		delay_ms(100);
		TurnplatePID.SetTarget(0.0);
		// Turnplate_motor_PWM=0;
		// ESC_RUN(ESC_PWM,ESC_PWM);
		delay_ms(200);
	}
	else if(flag==0xff)
	{
		TurnplatePID.SetTarget(RSF);
		// Turnplate_motor_PWM=20;//连发
		if(PWM1 == 1000+10*ESC_PWM1)
			ESC_RUN(ESC_PWM_CONSTANTLY,ESC_PWM_CONSTANTLY);
	}
	else if(flag==0x01)//
	{
		TurnplatePID.SetTarget(0.0);
		{
			float temp;
			temp = TurnplatePID.Target;
			TurnplatePID.SetTarget(-10);
			delay_ms(100);
			TurnplatePID.SetTarget(temp);
		}
		// Turnplate_motor_PWM=0;
		if(PWM1 == 1000+10*ESC_PWM_CONSTANTLY)
			ESC_RUN(ESC_PWM1,ESC_PWM2);//减速
	}
	else
	{
		TurnplatePID.SetTarget(0.0);
		// Turnplate_motor_PWM=0;
	}
}


void Turnplate_motor_ONCE(void)
{
	Turnplate_motor_drive(0x0f);
}

void Turnplate_motor_CONSTANTLY(void)
{
	Turnplate_motor_drive(0xff);
}
void Turnplate_motor_STOP(void)
{
	Turnplate_motor_drive(0x01);

}
void Turnplate_motor_KM_STOP(void)
{
	TurnplatePID.SetTarget(0.0);
}

void Turnplate_motor(int temp)
{
	if (temp < 0)
	{
		Motor_Bin1 = 1;
		Motor_Bin2 = 0;
		Turnplate_motor_PWM= -temp;
//		GPIO_SetBits(GPIOC,GPIO_Pin_5);
//		TIM_OC1PolarityConfig(TIM13,TIM_OCPolarity_High);

	}
	else if (temp >= 0)
	{
		Motor_Bin1 = 0;
		Motor_Bin2 = 1;
		Turnplate_motor_PWM= temp;
	}
}
