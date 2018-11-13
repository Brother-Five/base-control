#include "bsp.h"
#include "bsp_can.h"
#include "RTL.h"
#include "add_bullet.h"
//case KEY_PRESSED_OFFSET_SHIFT +KEY_PRESSED_OFFSET_CTRL + KEY_PRESSED_OFFSET_A:bumark=1;lmark=1;break;     //添加于遥控器那里
//case KEY_PRESSED_OFFSET_SHIFT +KEY_PRESSED_OFFSET_CTRL + KEY_PRESSED_OFFSET_F:bumark=1;rmark=1;break;	  //补弹按键设置

//	can_filter_Mask_config(CanFilter_5|CanFifo_0|Can_STDID|Can_DataType,0x207,0x3ff);
//	can_nvic_config(CAN1_R0); 															 //添加于task void EXTITask(void)中

//else if(PIDBegin_CAN&&cir_mark==0&&bu_mark==0&&vmark==0&&bumark==0)    //  添加于 CarFrame_Agues任务那里
//if(PIDBegin_DR16 && PIDBegin_CAN && CAN2_YawAngle && CAN2_YawMotor && CAN2_PitchMotor&&vmark==0&&cir_mark==0&&bu_mark==0&&bumark==0)

//	case BIT_0:	ESC_Toggle();delay_ms(10);vmark=1;USART1_DMA_Send(circlebuf,sizeof(circlebuf));break;//vmark=1;break;//无刷电机delay_ms(10);USART1_DMA_Send(rbuf,sizeof(rbuf));
//				case BIT_1: Turnplate_motor_ONCE();vmark=0;break;//拨盘电机（单次）USART1_DMA_Send(circlebuf,sizeof(circlebuf));
//				case BIT_2:
////						FollowMode =!FollowMode;
//						Duoji_Toggle();//舵机
//				bumark=1;
//测试用

//u32 UART1_BAUD=115200;
int16_t temp[4],i;
u8 chessbuf[3]={0x55,0x66,0x77}; //85 102 119
u8 circlebuf[3]={0x55,0x88,0x99};// 85 -120 103

u8 lbuf[5]={0xA1,0xF1,0x11,0x22,0x33};     //红外串口发送数组
u8 rbuf[5]={0xA1,0xF1,0x11,0x44,0x55};

u8 bumark=0,timemark=0,vmark=0,redmark=0,lmark=0,rmark=0,bu_mark=0,cir_mark=0;
uint16_t rx=1024,rrx=1024,rry=1024,ry=1024,rrth=0,rth=1024;
extern OS_TID HandleDisconnectCheck;

void add_bullet(CanRxMsg *msg_rece)
{

	 //if(bumark||vmark||cir_mark||timemark)FollowMode =1;else{FollowMode = 0;}
//			if(vmark)Motor_Aguest(rrx,rry,rrth); //标定板定位
	if(bumark)//os_evt_wait_or (BIT_0, 200) == OS_R_EVT)
		{
					// os_evt_set (BIT_3, HandleDisconnectCheck);//断线保护
					// os_evt_set (BIT_4, HandleDisconnectCheck);//断线保护
					for(i=0;i<4;i++)																		//循线程序代码
					{
						temp[i]=(((int16_t)msg_rece->Data[2*i])<<8)+msg_rece->Data[2*i+1];
					}
//					if(temp[0]==1&&temp[1]==1&&temp[2]==1&&temp[3]==1)
//						{bumark=0;USART1_DMA_Send(chessbuf,sizeof(chessbuf));vmark=1;}   //巡到公路出口，依靠棋盘标定

//					if(temp[0]==0&&temp[1]==0&&temp[2]==0&&temp[3]==0)
//						{lmark=1;bumark=0;redmark=1;Motor_Aguest(1024,1024,0);timemark=1;}		//printf("out bumark\r\n");}							//发信号延时等待落弹开始
//					else
					Motor_CarFrame(temp[0],temp[1],temp[2],temp[3] );//printf("send\r\n");
			}
		
}

void send_cmd()
{
//	if(cir_mark)							//由侧面转为正面
//			{
//
//				Motor_Aguest(224,1824,0);
//				delay_ms(500);
//				Motor_Aguest(224,1824,0);
//				delay_ms(500);
//				Motor_Aguest(124,1724,1000);
//				delay_ms(500);
//				Motor_Aguest(24,1624,3200);
//				delay_ms(500);
//				cir_mark=0;
//				//bumark=1;
//				//USART1_DMA_Send(chessbuf,sizeof(chessbuf)); //通知PC启动圆形标定识别
//				vmark=1;
////				bu_mark=1;
//			}
//		if(bu_mark)
//			{
//
//				Motor_Aguest(324,1924,0);
//				delay_ms(500);
//				Motor_Aguest(24,1924,0);
//				delay_ms(500);
//			    Motor_Aguest(1024,1334,0);//			   Motor_Aguest(1024,1424,0);
//				delay_ms(500);
//				bu_mark=0;
////				Motor_Aguest(1024,1424,0);
////				delay_ms(500);
////				printf("t\r\n");
//				Motor_Aguest(1024,1024,0);
//				delay_ms(100);
//				printf("v0");
//				bumark=1;									//圆形标定完后直走后启动循线补弹
//		    }
//
//		if(redmark&&lmark)						//红外发送,通知左边落弹
//			{
//				//delay_ms(200);
//				UART1_BAUD=9600;
//				bsp_InitUart();
//				//delay_ms(200);
//				USART1_DMA_Send(lbuf,sizeof(lbuf));
//				delay_ms(20);USART1_DMA_Send(lbuf,sizeof(lbuf));
//				delay_ms(20);USART1_DMA_Send(lbuf,sizeof(lbuf));
//				lmark=redmark=0;
//				delay_ms(200);
//				UART1_BAUD=115200;
//				bsp_InitUart();
//				//delay_ms(200);
//
//			}
//		if(redmark&&rmark)						//红外发送,通知右边落弹
//			{
//				UART1_BAUD=9600;
//				bsp_InitUart();
//				USART1_DMA_Send(rbuf,sizeof(rbuf));
//				delay_ms(20);USART1_DMA_Send(rbuf,sizeof(rbuf));
//				delay_ms(20);USART1_DMA_Send(rbuf,sizeof(rbuf));
//				delay_ms(200);
//				UART1_BAUD=115200;
//				bsp_InitUart();
//				rmark=redmark=0;
//			}
//		if(timemark)
//		{
//			delay_ms(3000);				//收到信号开始延时2.5s收弹
//			delay_ms(2000);
//			redmark=1;
//			timemark=0;
//			bumark=0;
//		}
//
		delay_ms(10);
}

void rece_usart(u8* uart_rx)
{
			if(uart_rx[0]==0xaa)
	       {
			rx=uart_rx[1]<<8;
			rx|=uart_rx[2];
			ry=uart_rx[3]<<8;
			ry|=uart_rx[4];
			rth=uart_rx[5]<<8;
			rth|=uart_rx[6];
			//printf("%d\t%d\t%d\t",rx,ry,rth);
			}
		   if(rx==666&&vmark==1){vmark=0;cir_mark=1;rrx=rry=1024;rrth=0;}
		   else {rrx=rx;rry=ry;rrth=(int)(rth-1024)*7;}
		    if(rx==888&&vmark==1){vmark=0;rrx=rry=1024;rrth=0;}//bu_mark=1;
		   else
			{rrx=rx;rry=ry;rrth=(int)(rth-1024)*7;}
}

