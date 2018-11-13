#ifndef  _Data_Tramsfer_h_
#define  _Data_Tramsfer_h_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/**
 * 匿名上位机的协议
 */

//取指针，将不同类型的类型转化成8位的char型
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
void ANO_Data6_Send(u8 temp,float Data1, float Data2, float Data3, float Data4, float Data5, float Data6);
void ANO_Data3_Send(u8 temp,float Data1, float Data2, float Data3);
void ANO_Data2_Send(u8 temp,float Data1, float Data2);
void ANO_Data1_Send(u8 temp,float Data1);
void USART1_SendString(u8 len,u8 *data);

#ifdef __cplusplus
}
#endif
#endif


//extern u8 data_to_send[50];
////extern unsigned char UsartTxBuffer[DMA_BUFFERSIZE*2];
//template<class Type>
// void ANO_Data_Send(u8 temp,Type Data1, Type Data2, Type Data3, Type Data4, Type Data5, Type Data6)
// {
// 	u8 _cnt=0;
// 	u8 sum= 0;
// 	u8 i=0;
// 	int16_t _temp=0;

// 	data_to_send[_cnt++]=0xAA;//帧头
// 	data_to_send[_cnt++]=0xAA;//帧头
// 	data_to_send[_cnt++]=temp;//功能字
// 	data_to_send[_cnt++]=0;//数据长度

// 	_temp = (int)Data1;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 	_temp = (int)Data2;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 	_temp = (int)Data3;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 	_temp = (int)Data4;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 		_temp = (int)Data5;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 	_temp = (int)Data6;
// 	data_to_send[_cnt++]=BYTE1(_temp);
// 	data_to_send[_cnt++]=BYTE0(_temp);
// 	 //_temp=0;//先清零
// 	data_to_send[3] = _cnt-4;//数据长度
// 	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
// 	data_to_send[_cnt++]=sum;//校验值
// 	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
// 	// USART1_SendString(_cnt,data_to_send);
// }

//template<class Type>
//void ANO_Data_Send(u8 temp,Type Data1, Type Data2, Type Data3)
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
//	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//	// USART1_SendString(_cnt,data_to_send);

//}

//template<class Type>
//void ANO_Data_Send(u8 temp,Type Data1, Type Data2)
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
//	 //_temp=0;//先清零
//	data_to_send[3] = _cnt-4;//数据长度
//	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
//	data_to_send[_cnt++]=sum;//校验值
//	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//	// USART1_SendString(_cnt,data_to_send);
//}

//template<class Type>
//void ANO_Data_Send(u8 temp,Type Data1)
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

//	data_to_send[3] = _cnt-4;//数据长度
//	for(i=0;i<_cnt;i++) sum += data_to_send[i];	//求取校验值
//	data_to_send[_cnt++]=sum;//校验值
//	USART1_DMA_Send((uint8_t*)data_to_send,_cnt);
//	// USART1_SendString(_cnt,data_to_send);
//}

//#endif
