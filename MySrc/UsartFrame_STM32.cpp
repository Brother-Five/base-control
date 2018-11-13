 #include "UsartFrame_STM32.h"
 #include "RTL.h"
#include "bsp.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "cpid.h"
#include "SetParameter.h"


//CPID CVPID(0.0,1.0,0.0,0,-4500, +4500, -2000, +2000, -4500, +4500,-500,500,5.0,5);
os_mbx_declare(UsartFrame_mailbox,1024);


__align(8) u8 USART3_TX_BUF[1024]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
/**
 * [UsasrFrameInit 使能STM32CRC32的时钟]
 */
void UsasrFrameInit(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
}


/**
 * [CRC_STM32Hard STM32硬件CRC32,数据长度必须是4的倍数(STM32是32位的)]
 * @param  pBuffer      [数组指针]
 * @param  BufferLength [数据长度]
 * @return              [CRC32校验码]
 */
uint32_t CRC_STM32Hard(uint8_t* pBuffer, uint32_t BufferLength)
{
  uint32_t i;
//  uint32_t temp = ((BufferLength-1)>>2) + 1;//除与4
  uint32_t temp = BufferLength>>2;
  uint32_t tempcount;
  CRC->CR = CRC_CR_RESET;
//  if(BufferLength%4)
  for(i = 0; i < temp; i++)
  {
	tempcount = i<<2;
    CRC->DR = pBuffer[tempcount]<<24 | pBuffer[tempcount+1]<<16 | pBuffer[tempcount+2]<<8 | pBuffer[tempcount+3];
  }
  return (CRC->DR);
}



/**
 * [CRC_STM32Check STM32CRC32检查,数据长度必须是4的倍数(STM32是32位的)]
 * @param  pBuffer      [数组指针]
 * @param  BufferLength [数据长度]
 * @param  CRC32Rec     [接收的CRC32校验码]
 * @return              [0:正确无误码，OTHERS:有误码，数据有错误]
 */
uint32_t CRC_STM32Check(uint8_t* pBuffer, uint32_t BufferLength , uint32_t CRC32Rec)
{
  uint32_t i;
//  uint32_t temp = ((BufferLength-1)>>2) + 1;//除与4
  uint32_t temp = BufferLength>>2;
  uint32_t tempcount;
  CRC->CR = CRC_CR_RESET;
  for(i = 0; i < temp; i++)
  {
	tempcount = i<<2;//乘与4
    CRC->DR = pBuffer[tempcount]<<24 | pBuffer[tempcount+1]<<16 | pBuffer[tempcount+2]<<8 | pBuffer[tempcount+3];//1|2|3|4转化成小端格式
  }
  CRC->DR = CRC32Rec;//最后是CRC32校验码
  return (CRC->DR);
}


/**
 * [UsasrFrameSend 协议发送函数]
 * @param pBuffer [发送结构体的指针]
 */
void UsasrFrameSend(FRAME * pBuffer)
{
	unsigned char i,temp;
	pBuffer->SOF = 0xAA;
	temp = (pBuffer->LEN)%4;
	if(temp)
		pBuffer->MOD = 4 - temp;
	else
		pBuffer->MOD = 0;
	for (i = 0; i < pBuffer->MOD; ++i)
	{
		pBuffer->DATA[pBuffer->LEN + i] = 0xff;
	}
	pBuffer->CRC32 = CRC_STM32Hard(pBuffer->DATA,pBuffer->LEN + pBuffer->MOD);
	USART1_DMA_Send((uint8_t*)pBuffer,pBuffer->LEN + pBuffer->MOD + 8);
}

/**
 * [senddemo 发送实例]
 */
void senddemo(void)
{
	FRAME Ftemp;
	Ftemp.LEN = 0x07;
	Ftemp.DATA[0] = 0x00;
	Ftemp.DATA[1] = 0x11;
	Ftemp.DATA[2] = 0x22;
	Ftemp.DATA[3] = 0x33;
	Ftemp.DATA[4] = 0x44;
	Ftemp.DATA[5] = 0x55;
	Ftemp.DATA[6] = 0x66;
	Ftemp.DATA[7] = 0x77;
	UsasrFrameSend(&Ftemp);
}

/**
 * [recdemo 接收实例]
 * @param  str [接收地址]
 * @return     [0:接收数据正确，1:接收数据错误]
 */
unsigned char recdemo(unsigned char * str)
{
	if(CRC_STM32Check( ((FRAME *)str) ->DATA , ((FRAME *)str)->LEN + ((FRAME *)str)->MOD,((FRAME *)str)->CRC32)!= 0)
	{
		return 1;
	}
	printf("value = %08X\r\n",CRC_STM32Hard (((FRAME *)str) ->DATA, ((FRAME *)str)->LEN + ((FRAME *)str)->MOD));
	return 0;
}


/**
 * [UsasrFrameRec 串口接收中断调用函数]
 * @param  str [接收BUF指针]
 * @return     [0:正确运行]
 */
unsigned char UsasrFrameRec(unsigned char * str)
{
	senddemo();
	return recdemo(str);
//	return 0;
}









/* ------ ----------------- Internal Data ----------------------------------- */
volatile unsigned char frame_rx_buffer[25];

void UsartFrame_Init(void)
{
//	u8 ptrmsg;
	/* -------------- Enable Module Clock Source ----------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_UsartFrame, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_Periph_UsartFrame, ENABLE);
	GPIO_PinAFConfig(GPIO_PORT_Rx_UsartFrame,GPIO_PinSource_Rx_UsartFrame, GPIO_AF_UsartFrame);
	GPIO_PinAFConfig(GPIO_PORT_Tx_UsartFrame,GPIO_PinSource_Tx_UsartFrame, GPIO_AF_UsartFrame);
	/* dr16_UsartFrame_mailbox 初始化*/
//	ptrmsg = 1; //!< Channel 0

//	os_mbx_send (UsartFrame_mailbox, &ptrmsg,200);
	/* -------------- Configure GPIO ---------------------------------------*/
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Rx_UsartFrame ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_PORT_Rx_UsartFrame, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Tx_UsartFrame ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_PORT_Tx_UsartFrame, &GPIO_InitStructure);

		USART_DeInit(USART_UsartFrame);
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART_UsartFrame,&USART_InitStructure);
		USART_Cmd(USART_UsartFrame,ENABLE);
		USART_DMACmd(USART_UsartFrame,USART_DMAReq_Rx,ENABLE);
	}
	/* -------------- Configure NVIC ---------------------------------------*/
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQn_Rx_UsartFrame;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQn_Tx_UsartFrame;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	/* -------------- Configure DMA Rx -----------------------------------------*/
	{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA_Stream_Rx_UsartFrame);
		DMA_InitStructure.DMA_Channel = DMA_Channel_Rx_UsartFrame;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART_UsartFrame->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)frame_rx_buffer;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = 1024;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA_Stream_Rx_UsartFrame,&DMA_InitStructure);
		// DMA_ITConfig(DMA_Stream_UsartFrame,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA_Stream_Rx_UsartFrame,DISABLE);
		USART_ITConfig(USART_UsartFrame, USART_IT_IDLE, ENABLE);//空闲中断,用于不定长接收
	}

	/* -------------- Configure DMA Tx -----------------------------------------*/
	{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA_Stream_Tx_UsartFrame);
		DMA_InitStructure.DMA_Channel = DMA_Channel_Tx_UsartFrame;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART_UsartFrame->DR);
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
		DMA_Init(DMA_Stream_Tx_UsartFrame,&DMA_InitStructure);
		USART_DMACmd(USART_UsartFrame,USART_DMAReq_Tx,ENABLE);
		DMA_ITConfig(DMA_Stream_Tx_UsartFrame,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA_Stream_Tx_UsartFrame,DISABLE);//初始化时要失能。。。不能！！！
	}
}

u8 TxFlag;
void USARTFrame_DMA_Send(uint8_t *pbuffer, uint32_t size)
{
	if(TxFlag == 0)//发送数据完成后才能开始发送
	{
		DMA_Cmd (DMA_Stream_Tx_UsartFrame,DISABLE);
		while (DMA_GetCmdStatus(DMA_Stream_Tx_UsartFrame) != DISABLE){}
	//	DMA_Stream_Tx_UsartFrame->M0AR =  (u32)pbuffer;
	//	DMA_Stream_Tx_UsartFrame->NDTR = size;
		DMA_MemoryTargetConfig(DMA_Stream_Tx_UsartFrame,(u32)pbuffer,DMA_Memory_0);
		DMA_SetCurrDataCounter(DMA_Stream_Tx_UsartFrame,size);
	 	DMA_Cmd (DMA_Stream_Tx_UsartFrame,ENABLE);//使能DMA,开始发送
	 	TxFlag=1; //数据发送中
	}
}
// __align(8) u8 USART2_TX_BUF[1024]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
 void u3_printf(char* fmt,...)
 {
 	va_list ap;
 	va_start(ap,fmt);
 	vsprintf((char*)USART3_TX_BUF,fmt,ap);
 	va_end(ap);
 	USARTFrame_DMA_Send(USART3_TX_BUF, strlen((const char*)USART3_TX_BUF));
 //	for(j=0;j<i;j++)//循环发送数据
 //	{
 //	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);  //等待上次传输完成
 //		USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]); 	 //发送数据到串口3
 //	}
 }



void UsartFrame_mailbox_init(void)
{
	os_mbx_init (&UsartFrame_mailbox, sizeof(UsartFrame_mailbox));//UsartFrame邮箱初始化
}

uint8_t UsartFrame_receive(FRAME **ptrmsg, uint16_t timeout)
{
	return os_mbx_wait(UsartFrame_mailbox, (void **)ptrmsg,200);
}




//单片机中断不能用C++来编译，否则程序会跑死
#ifdef __cplusplus
 extern "C" {
#endif
CVStruct *CVValue;
CV_ALL_Stuct CVAllValue;
extern FUZZYPID YawPID,PitchPID;
extern CPID CarFramePID;
extern union Parameter_Operater_Union Parameter_Operater;

extern short tmeee;
extern tGameInfo GameInfo;
void DMA_IRQHandler_Rx_UsartFrame(void)
{
//	static FRAME ptrmsg;
	u8 clear = clear;

//	static short flag[3];//	static uint32_t temp;
//	static u8 count[2];
	if (USART_GetITStatus(USART_UsartFrame, USART_IT_IDLE) != RESET)//空闲中断,
	{
		

//		GameInfo = *(tGameInfo*)(frame_rx_buffer);
//============================================================================================================
//		if(CRC_STM32Check( ((FRAME *)frame_rx_buffer) ->DATA , ((FRAME *)frame_rx_buffer)->LEN + ((FRAME *)frame_rx_buffer)->MOD,((FRAME *)frame_rx_buffer)->CRC32)!= 0)
//		{
//			u1_printf("unmatch crc\r\n");
//		}
//		else
//		{

//			CVValue = (CVStuct *) (&((FRAME*)frame_rx_buffer)->DATA[0]);

//			switch(CVValue->State.ID)
//			{
//				case 1:

//						flag[0] = CVValue->State.Flag;
//						// temp[0] = CVValue->CV_X;
//						// temp[1] = CVValue->CV_Y;
//						CVAllValue.Strike_Angle_X = CVValue->CV_X;// - 1.38782f
//						CVAllValue.Strike_Angle_Y = CVValue->CV_Y;
//					break;
//				case 2:
//						flag[1] = CVValue->State.Flag;
//						// temp[2] = CVValue->CV_X;
//						// temp[3] = CVValue->CV_Y;
//						CVAllValue.Strike_Coordinate_X = CVValue->CV_X - 51;
//						CVAllValue.Strike_Coordinate_Y = CVValue->CV_Y;
//				case 3:
//						flag[2] = CVValue->State.Flag;
//						// temp[2] = CVValue->CV_X;
//						// temp[3] = CVValue->CV_Y;
//						CVAllValue.DaFu_Angle_X = CVValue->CV_X;
//						CVAllValue.DaFu_Angle_Y = CVValue->CV_Y;
//					break;
//				default:break;
//			}

////			(YawPID.Current -  CarFramePID.Current)/22.7556f//摄像头角度
//			if(cv_flag == 1)
//			{

////				YawPID.SetTarget( (YawPID.Current -  CarFramePID.Current ) - (CVAllValue.DaFu_Angle_X/100.0f)*22.7556f);
////				PitchPID.SetTarget(0.130883442399745*CVAllValue.DaFu_Angle_Y+2.377947784543123e+03 + 5);
//				if(flag[1] == 1)
//				{
//					CVPID.SetCurrent(CVAllValue.Strike_Coordinate_X);
//					CVPitchPID.SetCurrent(CVAllValue.Strike_Coordinate_Y);
//					flag[1] = 0;
//				}
//				if(flag[0] == 1)
//				{
////					CVPID.SetCurrent(CVAllValue.Strike_Coordinate_X);
////					CVPID.AdjustPID();
////					if(myabs(CVAllValue.Strike_Coordinate_X)<(30))
////					{
////						bsp_LedToggle(3);
////					}
////					else
////					{
////						bsp_LedOn(3);
////					}
//					YawPID.SetTarget( (YawPID.Current -  CarFramePID.Current ) - (CVAllValue.Strike_Angle_X/100.0f)*22.7556f + CVPID.Out);
////					u1_printf("%x\t%x\t%d\t%d\t%d\t%d\r\n",CVValue->State.ID,CVValue->State.Flag,CVAllValue.Strike_Angle_X,CVAllValue.Strike_Angle_Y,\
//			CVValue->CV_X,CVValue->CV_Y\
//			);
//// 					PitchPID.SetTarget(Parameter_Operater.data.PITCH_Encoder_Mid + (CVAllValue.Strike_Angle_Y/100.0f)*22.7556f);
//					// PitchPID.SetTarget(0.130883442399745*CVAllValue.Strike_Angle_Y+2.377947784543123e+03 - CVPitchPID.Out);
//					flag[0] = 0;
//					count[0] = 0;
//				}
//				else count[0]++;
//				if(count[0] == 30)
//				{
//					CVPID.ITerm = 0;
//					CVPitchPID.ITerm = 0;
//					YawPID.SetTarget( (YawPID.Current -  CarFramePID.Current));
//					YawPID.SetTarget( Parameter_Operater.data.PITCH_Encoder_Mid);
//					count[0] = 0;
//				}
////				u1_printf("%x\t%x\t%d\t%d\t%d\t%d\r\n",CVValue->State.ID,CVValue->State.Flag,CVAllValue.Strike_Angle_X,CVAllValue.Strike_Angle_Y,\
////			CVValue->CV_X,CVValue->CV_Y\
////			);
////				if(myabs(CVAllValue.Strike_Coordinate_Y)<3)
////					u1_printf("%d\t%d\r\n",CVAllValue.Strike_Angle_Y,tmeee);
////				ANO_Data1_Send(0xf1,CVAllValue.Strike_Coordinate_X);
////				u1_printf("%x\t%x\t%d\t%d\t%d\t%d\r\n",CVValue->State.ID,CVValue->State.Flag,CVAllValue.Strike_Angle_X,CVAllValue.Strike_Angle_Y,\
//			CVAllValue.Strike_Coordinate_X,CVAllValue.Strike_Coordinate_Y\
//			);

//			}
////			u1_printf("%x\t%x\t%d\t%d\r\n",CVValue->State.ID,CVValue->State.Flag,CVValue->CV_X,CVValue->CV_Y);
////			u1_printf("%d\t%d\r\n",YawPID.Current,CarFramePID.Current);
////			u1_printf("%d\t%d\r\n",Get_cv_x(),Get_cv_y());
////			if(CVValue->State.ID==2)
////			u1_printf("%x\t%d\t%d\r\n",CVValue->State.Flag,CVAllValue.Strike_Angle_X,CVAllValue.Strike_Angle_Y);
////			u1_printf("%x\t%x\t%d\t%d\t%d\t%d\r\n",CVValue->State.ID,CVValue->State.Flag,temp[0],temp[1],\
////			temp[2],temp[3]\
////			);
//		}
//============================================================================================================
		DMA_Cmd(DMA_Stream_Rx_UsartFrame,DISABLE);//DMA失能
		while(DMA_GetCmdStatus(DMA_Stream_Rx_UsartFrame));//检测是否失能成功，DMA失能时需要等待少许时间才失能成功
		DMA_SetCurrDataCounter(DMA_Stream_Rx_UsartFrame,1024);//数据传输量
		DMA_Cmd(DMA_Stream_Rx_UsartFrame,ENABLE);//DMA重新使能
		clear = USART_UsartFrame->SR;//这两步为清除空闲中断标志
		clear = USART_UsartFrame->DR;//这两步为清除空闲中断标志
	}
}

void DMA_IRQHandler_Tx_UsartFrame(void)
{
	if(DMA_GetITStatus(DMA_Stream_Tx_UsartFrame, DMA_StreamIT_Tx_TCIFx_UsartFrame))
	{
		TxFlag = 0;
		// printf("ok\r\n");
		DMA_ClearITPendingBit(DMA_Stream_Tx_UsartFrame, DMA_StreamIT_Tx_TCIFx_UsartFrame);
	}
}


#ifdef __cplusplus
}
#endif
