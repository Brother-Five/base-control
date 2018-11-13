#ifndef _FILTER_H__
#define _FILTER_H__
#include "stm32f4xx.h"
#include "arm_math.h"

//#ifdef __cplusplus
// extern "C" {
//#endif
	 
#define FLOAT_FILTER 1
#define INT_FILTER 0
//float Slider_Filter(float *temp,unsigned char i, unsigned char num,float current);
int Middle_filter(int *temp,unsigned char *i, unsigned char num,int current);
short Middle_filter(short *temp,unsigned char *i, unsigned char num,short current);
float Middle_filter(float *temp,unsigned char *i, unsigned char num,float current);

int Slider_Filter(int *temp,unsigned char *i, unsigned char num,int current);
short Slider_Filter(short *temp,unsigned char *i, unsigned char num,short current);
float Slider_Filter(float *temp,unsigned char *i, unsigned char num,float current);

template<class Type>
Type Constrain(Type input,Type min,Type max){
if (input <= min)
	return min;
else if(input >= max)
	return max;
else return input;
}

template<class T>
void bubble_sort(T a[], int n)
{
    int i,j;
//	char flag;
	T temp;
    for (j = 0; j < n - 1; j++)
	{
//		if(flag)
//			break;
//		flag = 1;
        for (i = 0; i < n - 1 - j; i++)
        {
            if(a[i] > a[i + 1])
            {
                temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
//				flag = 0;
            }
        }
	}
}
//#ifdef __cplusplus
//}
//#endif
#endif
