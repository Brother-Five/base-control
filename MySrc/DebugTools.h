#ifndef __DEBUGTOOLS_H
#define __DEBUGTOOLS_H

#ifdef __cplusplus
 extern "C" {
#endif

#define MAX_FNAME_LEN 32
#define MAX_PNAME_LEN 32
#define MAX_PNUM      12

struct _m_nametab
{
	void * func;			//函数指针
	const  char* name;		//函数名(查找串)
	const  char* tips;		//指示(查找串)
};
//控制管理器
struct _m_dev
{
	struct _m_nametab *tab;	//函数名指针
	unsigned char fnum; 				  		//函数数量
	unsigned char pnum;                        //参数数量
	unsigned char id;							//函数id
	float Parm[MAX_PNUM];//参数暂存数组
	unsigned char fname[MAX_FNAME_LEN];//函数名暂存数组
};

extern struct _m_nametab nametab[];	//在DebugToolsconfig.cpp里面定义
extern struct _m_dev dev;				//在DebugToolsconfig.cpp里面定义

void DebugToolsInit(void);
unsigned char run(unsigned char * str);
unsigned char function(unsigned char * str);
unsigned int mypow(unsigned char m,unsigned char n);
unsigned int getnum(unsigned char bnum ,unsigned char hexdec ,unsigned char* p);
unsigned char str2num(unsigned char*str,float *res);
unsigned char mystrcmp(unsigned char *str1,const  char *temp);
#ifdef __cplusplus
}
#endif


#endif
