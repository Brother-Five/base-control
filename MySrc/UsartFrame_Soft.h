#ifndef _USARTFRAME_SOFT_H___
#define _USARTFRAME_SOFT_H___

#ifdef __cplusplus
 extern "C" {
#endif

//取指针，将不同类型的类型转化成8位的char型
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#pragma pack (4)

#define SUM  1024  //4的整数
typedef struct
{
	unsigned char  SOF;//帧头
	unsigned char  MOD;//(4 - (数据长度%4))
	unsigned short LEN;//数据长度
	unsigned int   CRC32;//CRC32校验码
	unsigned char  DATA[SUM];//数据BUF
}FRAME;

unsigned int CRC_Soft32 (unsigned char* p, unsigned long len);
unsigned int CRC_Soft32Check (unsigned char* p, unsigned long len, uint32_t CRC32Rec);
void UsasrFrameSend(FRAME * pBuffer);
unsigned char UsasrFrameRec(unsigned char * str);

#ifdef __cplusplus
 }
#endif


#endif

