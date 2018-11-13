#include "stm32f4xx.h"
#include  "arm_math.h"
#include "filter.h"

#define myabs(x) ((x)>0? (x):(-(x)))



int Middle_filter(int *temp,unsigned char *k, unsigned char num,int current)
{
	q31_t TempBuf[50];
//	q31_t sum;


//	if(*k != 0)
//		if(myabs(current - temp[*k - 1]) >= 1000)
//			current = temp[*k - 1];

	temp[*k] = current;
	(*k)++;
	if ( *k == num  ) *k = 0;

	arm_copy_q31(temp,TempBuf,num);

	bubble_sort(TempBuf,num);
	return TempBuf[(num-1)/2];
//	arm_mean_q31( &(TempBuf[1]) , num-2 , &sum);
//	return sum;

//	中位值平均滤波法（又称防脉冲干扰平均滤波法）
	// for(count=1,TempChange = 0;count<num-1;count++)
	// 	TempChange += TempBuf[count];
	// return TempChange/(num-2);
}

short Middle_filter(short *temp,unsigned char *k, unsigned char num,short current)
{
	q15_t TempBuf[50];
	// q15_t sum;

//	if(*k != 0)
//		if(myabs(current - temp[*k - 1]) >= 1000)
//			current = temp[*k - 1];

	temp[*k] = current;
	(*k)++;
	if ( *k == num  ) *k = 0;

	arm_copy_q15(temp,TempBuf,num);

	bubble_sort(TempBuf,num);
	// return TempBuf[(num-1)/2];
	return TempBuf[(num-1)/2];
//	return temp[_tempi];

//	arm_mean_q15(&(TempBuf[1]) , num-2 , &sum);
//	temp[_tempi] = sum;
//	return sum;

//	中位值平均滤波法（又称防脉冲干扰平均滤波法）
	// for(count=1,TempChange = 0;count<num-1;count++)
	// 	TempChange += TempBuf[count];
	// return TempChange/(num-2);
}

float Middle_filter(float *temp,unsigned char *k, unsigned char num,float current)
{
	float32_t TempBuf[50];
//	float32_t sum;

//	if(*k != 0)
//		if(myabs(current - temp[*k - 1]) >= 1000)
//			current = temp[*k - 1];

	temp[*k] = current;
	(*k)++;
	if ( *k == num  ) *k = 0;

	arm_copy_f32(temp,TempBuf,num);

	bubble_sort(TempBuf,num);
	return TempBuf[(num-1)/2];

//	arm_mean_f32( &(TempBuf[1]) , num-2 , &sum);
//	return sum;
//	中位值平均滤波法（又称防脉冲干扰平均滤波法）
	// for(count=1,TempChange = 0;count<num-1;count++)
	// 	TempChange += TempBuf[count];
	// return TempChange/(num-2);
}


int Slider_Filter(int *temp,unsigned char *i, unsigned char num,int current)
{
	q31_t sum=0;

	temp[*i] = current;
	(*i)++;
	if ( *i == num  ) *i = 0;

	arm_mean_q31(temp, num, &sum);
	return sum;
}


short Slider_Filter(short *temp,unsigned char *i, unsigned char num,short current)
{
	q15_t sum=0;

	temp[*i] = current;
	(*i)++;
	if ( *i == num  ) *i = 0;

	arm_mean_q15(temp, num, &sum);
	return sum;
}
float Slider_Filter(float *temp,unsigned char *i, unsigned char num,float current)
{
	float32_t sum=0;

	temp[*i] = current;
	(*i)++;
	if ( *i == num  ) *i = 0;

	arm_mean_f32(temp, num, &sum);
	return sum;
}
