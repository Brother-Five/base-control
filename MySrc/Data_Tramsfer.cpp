#include "bsp.h"

u8 data_to_send[50]={0};

void ANO_Data6_Send(u8 temp,float Data1, float Data2, float Data3, float Data4, float Data5, float Data6)
{
	u8 _cnt=0;
	u8 sum= 0;
	u8 i=0;
	int16_t _temp=0;

	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=temp;//������
	data_to_send[_cnt++]=0;//���ݳ���

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
	 //_temp=0;//������
	data_to_send[3] = _cnt-4;//���ݳ���
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//��ȡУ��ֵ
	data_to_send[_cnt++]=sum;//У��ֵ

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

//	data_to_send[_cnt++]=0xAA;//֡ͷ
//	data_to_send[_cnt++]=0xAA;//֡ͷ
//	data_to_send[_cnt++]=temp;//������
//	data_to_send[_cnt++]=0;//���ݳ���

//	_temp = (int)Data1;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = (int)Data2;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = (int)Data3;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	 //_temp=0;//������
//	data_to_send[3] = _cnt-4;//���ݳ���
//	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//��ȡУ��ֵ
//	data_to_send[_cnt++]=sum;//У��ֵ

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

	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=temp;//������
	data_to_send[_cnt++]=0;//���ݳ���

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data2;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data3;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	 //_temp=0;//������
	data_to_send[3] = _cnt-4;//���ݳ���
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//��ȡУ��ֵ
	data_to_send[_cnt++]=sum;//У��ֵ

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

	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=temp;//������
	data_to_send[_cnt++]=0;//���ݳ���

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)Data2;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	 //_temp=0;//������
	data_to_send[3] = _cnt-4;//���ݳ���
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//��ȡУ��ֵ
	data_to_send[_cnt++]=sum;//У��ֵ

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

	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=0xAA;//֡ͷ
	data_to_send[_cnt++]=temp;//������
	data_to_send[_cnt++]=0;//���ݳ���

	_temp = (int)Data1;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	data_to_send[3] = _cnt-4;//���ݳ���
	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//��ȡУ��ֵ
	data_to_send[_cnt++]=sum;//У��ֵ

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
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ����ͽ���
	}
}


