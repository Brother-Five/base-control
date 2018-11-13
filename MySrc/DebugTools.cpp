#include "bsp.h"
unsigned char run(unsigned char * str)
{
	unsigned char i;

	function(str);
	for(i= 0; i< dev.fnum ;i++)
	{
		if(mystrcmp(dev.fname,dev.tab[i].name) == 0)
		{
			switch(dev.pnum){
				case 0:(*(unsigned int(*)())dev.tab[i].func)();break;
				case 1:(*(unsigned int(*)(float))dev.tab[i].func)(dev.Parm[0]);break;
				case 2:(*(unsigned int(*)(float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1]);break;
				case 3:(*(unsigned int(*)(float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2]);break;
				case 4:(*(unsigned int(*)(float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3]);break;
				case 5:(*(unsigned int(*)(float,float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3],dev.Parm[4]);break;
				case 6:(*(unsigned int(*)(float,float,float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3],dev.Parm[4],dev.Parm[5]);break;
				case 7:(*(unsigned int(*)(float,float,float,float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3],dev.Parm[4],dev.Parm[5],dev.Parm[6]);break;
				case 8:(*(unsigned int(*)(float,float,float,float,float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3],dev.Parm[4],dev.Parm[5],dev.Parm[6],dev.Parm[7]);break;
				case 9:(*(unsigned int(*)(float,float,float,float,float,float,float,float,float))dev.tab[i].func)(dev.Parm[0],dev.Parm[1],dev.Parm[2],dev.Parm[3],dev.Parm[4],dev.Parm[5],dev.Parm[6],dev.Parm[7],dev.Parm[8]);break;
			}
		}
	}
	return 0;
}





/**
 * [function description]
 * @param  str   [输入的字符串地址]
 * @param  fname [函数名的字符串地址]
 * @param  temp  [参数数组地址]
 * @param  pnum  [参数个数]
 * @return
 * | 0 | 没错误           |
 * | 1 | 函数名字符串溢出 |
 * | 2 | 参数名字符串溢出 |
 * | 3 | 参数个数溢出     |
 */



unsigned char function(unsigned char * str)
{
	unsigned char parm[MAX_PNUM][MAX_PNAME_LEN];//缓存数组
	// unsigned char fnum;
	unsigned char count = 0 , i = 0;
	dev.pnum=0;
	for(count = 0;*str != '(';)//提取函数名
	{
		if(*str != ' ')
		{
			dev.fname[count] = *str;
			count++;
		}
		str++;
		if(count>MAX_FNAME_LEN-1) return 1;//函数名字符数量溢出
	}
	dev.fname[count] ='\0';//补充结尾
	str++;//跳过(
	while(*str != '\0')
	{
		for(count = 0;*str != ',' && *str != ')';)//分离提取参数
		{
			if(*str != ' ')//不考虑空格
			{
				parm[dev.pnum][count] = *str;
				count++;//参数加一
			}
			str++;
			if(count>MAX_PNAME_LEN-1) return 2;//参数字符数量溢出
		}
		parm[dev.pnum][count] = '\0';//补充结尾
		dev.pnum++;//参数数量加一
		if(dev.pnum>MAX_PNUM) return 3;//参数数量溢出
		if(*str != ')')//检查是否出现）,代表函数检查完毕
		{
			str++;
		}
		else break;
	}
//	printf("pnum:%d\r\n",dev.pnum);
	for(i =0 ; i<dev.pnum ; i++)
	{
		str2num(parm[i],&dev.Parm[i]);//str转化成数字
//		printf("%d:%f\t",i,dev.Parm[i]);
	}
//	printf("\r\n");

	return 0;
}

/**
 * [mypow 幂函数]
 * @param  m [description]
 * @param  n [description]
 * @return   [description]
 */
unsigned int mypow(unsigned char m,unsigned char n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}

/**
 * [getnum description]
 * @param  bnum   [数字个数]
 * @param  hexdec [10或16进制]
 * @param  p      [字符串地址]
 * @return        [转化后的unsigned int 型变量]
 */
unsigned int getnum(unsigned char bnum ,unsigned char hexdec ,unsigned char* p)
{
	unsigned int t;

	unsigned int res=0;
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//得到数字的值
		else t=*p-'A'+10;				//得到A~F对应的值
		res+=t*mypow(hexdec,bnum);
		p++;
		if(*p=='\0' ||*p=='.')
		{
			return res;//数据都查完了.
		}
	}
}

/**
 * [str2num 将字符串转化成float型的10进制，支持正负号，16进制]
 * @param  str [输入的参数的字符串地址]
 * @param  res [外界输入float形变量]
 * @return     [description]
 * | 0 | 没错误                                                              |
 * | 1 | 不全是十进制或者16进制数据                                          |
 * | 2 | 位数小于3，直接退出.因为0X就占了2个,如果0X后面不跟数据,则该数据非法 |
 * | 3 | 起始头的格式不对                                                    |
 * | 4 | 位数为0，直接退出                                                   |
 */

unsigned char str2num(unsigned char*str,float *res)
{
	unsigned char DotPreNum=0,DotNextNum=0;	//数字的位数
	unsigned char *P,*PP;
	unsigned char hexdec=10;//默认为十进制数据
	unsigned char polarity=0;
	unsigned char float_flag=0;
	P=str;
	*res=0;//清零.
	while(1)
	{
		if((*P=='.')||(*P=='-')||(*P<='9'&&*P>='0')||(*P<='F'&&*P>='A')||( (*P=='X' || *P=='x') &&DotPreNum==1))//参数合法
		{
			if(*P>='A')hexdec=16;	//字符串中存在字母,为16进制格式.
			if( *P=='-' )
			{
				polarity=1;
			}
			else if(*P=='.')
			{
				float_flag = 1;
				// *P = '\0';//将.转换成'\0'
				PP = P+1;
			}
			else
			{
				if(float_flag) DotNextNum++;
				else DotPreNum++;					//位数增加.
			}

		}
		else if(*P=='\0')break;	//碰到结束符,退出.
		else return 1;				//不全是十进制或者16进制数据.
		P++;
	}
	if(polarity) P=str+1;
	else P=str;			    //重新定位到字符串开始的地址.
	if(hexdec==16)		//16进制数据
	{
		if(DotPreNum<3)return 2;			//位数小于3，直接退出.因为0X就占了2个,如果0X后面不跟数据,则该数据非法.
		if(*P=='0' && ((*(P+1)=='X') || (*(P+1)=='x')) )//必须以'0X'开头.
		{
			P+=2;	//偏移到数据起始地址.
			DotPreNum-=2;//减去偏移量
		}else return 3;//起始头的格式不对
	}else if(DotPreNum==0)return 4;//位数为0，直接退出.
	 // printf("%d\t%d",DotPreNum,DotNextNum);
	if(float_flag)
		*res = (float)(getnum(DotPreNum,hexdec,P))+ (float)getnum(DotNextNum,hexdec,PP)/mypow(10,DotNextNum);
		// *res =  (float)getnum(DotNextNum,hexdec,PP);
	else
		*res = (float)getnum(DotPreNum,hexdec,P);

	if(polarity) *res = -*res;

	return 0;//成功转换
}

//////////////////////////////////////////////////////////////////////v
unsigned char mystrcmp(unsigned char *str1,const  char *str2)
{
	while(1)
	{
		if(*str1!=*str2)return 1;//不相等
		if(*str1=='\0')break;//对比完成了.
		str1++;
		str2++;
	}
	return 0;//两个字符串相等
}//////////////////////////////////////////////
