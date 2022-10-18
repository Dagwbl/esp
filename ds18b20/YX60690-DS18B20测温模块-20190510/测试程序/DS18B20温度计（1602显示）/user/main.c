/**********************TS-51A单片机开发板例程************************
*		Copyright (c) 2013,52探索 单片机开发板
*		All rights reserved
*       
*		文件名称：main.c
*		功能描述：操作DS18B20转换温度，读取并显示在数码管上。
* 
*		当前版本：1.0
*		作    者：52探索
*       完成日期：2013-5-29
*
*		取代版本：无
*       改进内容：无
*
*		使用环境：TS-51A开发板 + STC89C52 + 11.0592M晶振 + Keil C51 V9.0 
*       
*		淘宝地址：http://shop104208028.taobao.com/
*		QQ      : 10903659
*		QQ群    ：336397723  336398729   336398900
******************************************************************/

////#include <REGX52.H>
//#include <intrins.h>
#include "lcd1602.h"

#define uchar unsigned char 
#define uint  unsigned int
#define DataPort P0

sbit wx = P2^6;
sbit dx = P2^7;
sbit DS_Data = P2^0;

unsigned char code SMG[]  = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned char code str1[] = "The temperature ";
unsigned char code str2[] = "is :            ";
 
/*
* 功  能：清除数码管的显示内容
* 参  数：无
* 返回值：无
*/
void SMG_CLR( void )
{
	P0 = 0x00;	 	//发送要显示的段码
	dx = 1;
	dx = 0;
	
	P0 = 0x00;		//发送要显示的位码,只显示一位数字
	wx = 1;
	wx = 0;
}

/*
* 功  能：DS18B20 复位
* 参  数：无
* 返回值：无
*/
void DS18B20_Reset(void)
{
	unsigned int i;

	DS_Data = 0;
	i = 103;
	while(i>0)	i--;

	DS_Data = 1;
	i = 4;
	while(i>0)	i--;
}

/*
* 功  能：DS18B20 读取一位数据
* 参  数：无
* 返回值：读取的位
*/
bit DS18B20_ReadBit(void)	
{
	unsigned int i;
	bit dat;

	DS_Data = 0;
	i++;
	DS_Data = 1;
	i++;
	i++;
	dat = DS_Data;

	i = 8;
	while(i>0)	i--;

	return (dat);
}

/*
* 功  能：DS18B20 读取一个字节
* 参  数：无
* 返回值：读取的字节
*/
uchar DS18B20_ReadByte(void)	
{
	unsigned char i,j,dat;
	
	dat=0;
	for(i=1; i<=8; i++)
	{
		j = DS18B20_ReadBit();
		dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
	}

	return(dat);
}

/*
* 功  能：DS18B20 写入一个字节
* 参  数：要写入的字节
* 返回值：无
*/
void DS18B20_WriteByte(unsigned char dat)	
{
	unsigned int i;
	unsigned char j, k;
	bit b;

	k = dat;

	for(j=1; j<=8; j++)
	{
		b = k & 0x01;
		k = k >> 1;
		if(b)     				//write 1
		{
			DS_Data = 0;
			i++; i++;
			DS_Data = 1;
			i = 8;
			while(i>0) i--;
		}
		else
		{
			DS_Data = 0;       		//write 0
			i = 8;
			while(i>0)	i--;
			DS_Data = 1;
			i++; i++;
		}
	
	}
}

/*
* 功  能：DS18B20 开始温度转换
* 参  数：无
* 返回值：无
*/
void DS18B20_Change(void)	
{
	DS18B20_Reset();
	DelayMs(5);
	DS18B20_WriteByte(0xcc); 	 // address all drivers on bus
	DS18B20_WriteByte(0x44);  	//  initiates a single temperature conversion  
}

/*
* 功  能：DS18B20 得到温度值
* 参  数：无
* 返回值：无
*/
int DS18B20_GetTemp()			//get the temperature
{
	float t;
	unsigned char a, b;
	int temp;
	
	DS18B20_Reset();
	DelayMs(5);
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
	a=DS18B20_ReadByte();
	b=DS18B20_ReadByte();
	
	temp = b;
	temp <<= 8;             		//two byte  compose a int variable
	temp = temp|a;
	t = temp*0.0625;
	temp = t*10+0.5;
	
	return temp;
}


void main()
{
	int temp;
	unsigned char d1, d2, d3;

	SMG_CLR();
	LCD1602_Init();
	LCD1602_ShowString (0, str1);
	LCD1602_ShowString (1, str2);

	while(1)
	{	  
		DS18B20_Change();  					//开始转换

		temp = DS18B20_GetTemp(); 		 	//得到的温度保存在temp中

		d1 = temp / 100;
		d2 = (temp % 100) / 10;
		d3 = temp % 10;						//计算出十位、个位和小数位

		if(temp<0)						  	//温度小于0度
		{		   
			LCD1602_ShowChar(0x16, '-');	//显示负号
		}
		LCD1602_ShowChar(0x16, '0'+d1);	  	//显示十位
		LCD1602_ShowChar(0x17, '0'+d2);		//显示个位，减0x80是为了能显示小数点
		LCD1602_ShowChar(0x18, '.');
		LCD1602_ShowChar(0x19, '0'+d3);		//显示小数位
	}
}
