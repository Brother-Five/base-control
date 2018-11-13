#include "bsp.h"

u8 data_to_send[50]={0};

void ANO_Data6_Send(u8 temp,float Data1, float Data2, float Data3, float Data4, float Data5, float Data6)
{
	u8 _cnt=0;
	u8 sum= 0;
	u8 i=0;
	int16_t _temp=0;

	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=temp;//功能字
	data_to_send[_cnt++]=0;//数据长度

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data2;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data3;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data4;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
		_temp = (int)Data5;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data6;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	 //_temp=0;//先清零
	data_to_send[3] = _cnt-4;//数据长度
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
	data_to_send[_cnt++]=sum;//校验值

	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//#if USE_UART1_FIFO_DMA
//	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//#else
//	USART1_SendString(_cnt,data_to_send);
//#endif
}


//void ANO_Data3_32_Send(u8 temp,float Data1, float Data2, float Data3)
//{
//	u8 _cnt=0;
//	u8 sum= 0;
//	u8 i=0;
//	int16_t _temp=0;

//	data_to_send[_cnt++]=0xAA;//帧头
//	data_to_send[_cnt++]=0xAA;//帧头
//	data_to_send[_cnt++]=temp;//功能字
//	data_to_send[_cnt++]=0;//数据长度

//	_temp = (int)Data1;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = (int)Data2;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = (int)Data3;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	 //_temp=0;//先清零
//	data_to_send[3] = _cnt-4;//数据长度
//	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
//	data_to_send[_cnt++]=sum;//校验值

//#if USE_UART1_FIFO_DMA
//	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//#else
//	USART1_SendString(_cnt,data_to_send);
//#endif
//}

void ANO_Data3_Send(u8 temp,float Data1, float Data2, float Data3)
{
	u8 _cnt=0;
	u8 sum= 0;
	u8 i=0;
	int16_t _temp=0;

	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=temp;//功能字
	data_to_send[_cnt++]=0;//数据长度

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data2;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data3;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	 //_temp=0;//先清零
	data_to_send[3] = _cnt-4;//数据长度
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
	data_to_send[_cnt++]=sum;//校验值

#if USE_UART1_FIFO_DMA
	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
#else
	USART1_SendString(_cnt,data_to_send);
#endif
}

void ANO_Data2_Send(u8 temp,float Data1, float Data2)
{
	u8 _cnt=0;
	u8 sum= 0;
	u8 i=0;
	int16_t _temp=0;

	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=temp;//功能字
	data_to_send[_cnt++]=0;//数据长度

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data2;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	 //_temp=0;//先清零
	data_to_send[3] = _cnt-4;//数据长度
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
	data_to_send[_cnt++]=sum;//校验值

#if USE_UART1_FIFO_DMA
	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
#else
	USART1_SendString(_cnt,data_to_send);
#endif
}

void ANO_Data1_Send(u8 temp,float Data1)
{
	u8 _cnt=0;
	u8 sum= 0;
	u8 i=0;
	int16_t _temp=0;

	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=0xAA;//帧头
	data_to_send[_cnt++]=temp;//功能字
	data_to_send[_cnt++]=0;//数据长度

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	data_to_send[3] = _cnt-4;//数据长度
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
	data_to_send[_cnt++]=sum;//校验值

#if USE_UART1_FIFO_DMA
	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
#else
	USART1_SendString(_cnt,data_to_send);
#endif
}

void USART1_SendString(u8 len,u8 *data)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		USART1->DR=(uint16_t)data[i];
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//等待发送结束
	}
}


