#include <REGX52.H>
#include "intrins.h"			//_nop_();延时函数用

#define uchar unsigned char
#define uint  unsigned int

sbit  SO  = P3^6;  				//P3.6口与SO相连
sbit  SCK = P3^4; 				//P3.4口与SCK相连
sbit  CS  = P3^5;  				//P3.5口与CS相连

sbit wx = P2^6;
sbit dx = P2^7;

const unsigned char DevID = 1;		//本机设备ID
unsigned char buffer;			//串口接收缓冲区
unsigned char rcv_buffer[20];
unsigned char send_buffer[20];
unsigned char flag_ok = 0;		//接收完毕的标识
unsigned int MAX6675_Temp;
unsigned char Flag_connect;
unsigned char CRCH,CRCL;
unsigned char wd[2];	  		//wd[]：BCD码的温度
unsigned char flag_dis;			//显示刷新的标识

unsigned char dis[4] = { 0x00, 0x00, 0x00,0x00 };
unsigned char code SMG[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned int MAX6675_ReadReg(void)
{ 
	unsigned char i;   
	unsigned int dat;
	 
	i   = 0;   
	dat = 0;   
	
	CS  = 0; 
	SCK = 0;     
	
	for(i=0; i<16; i++)		//get D15-D0 from 6675  
	{      
		SCK = 1;     
		dat = dat<<1;    
		if( SO==1 )   
			dat = dat|0x01;   
		SCK = 0;    
	} 
	CS = 1;   
	  
	return dat;   
}  
/*
* 功  能：延时，若干毫秒
* 参  数：毫秒数
* 返回值：无
*/
void DelayMs(unsigned int i)	//一个延时函数，这个函数在11.0592M晶振下的延时时间大概为1ms
{
	unsigned int j,k;

	for(j=i; j>0; j--)
		for(k=114; k>0; k--);
}
/*
* 功  能：数码管在指定的位置显示指定的字符
* 参  数：place:要显示的位置，num:要显示的字符
* 返回值：无
*/
void SMG_DisChar(unsigned char place, unsigned char num)
{
	unsigned char i;

	i = 0x80 >> place;		//根据显示位置选择相应的数码管

	P0 = SMG[num];	 	//发送要显示的段码
	dx = 1;
	dx = 0;
	
	P0 = i;		   			//发送要显示的位码,只显示一位数字
	wx = 1;
	wx = 0;

	DelayMs(1);	
}
 
void Display(unsigned int temp)
{
	if(Flag_connect==0)		  	//Flag_connect为0 表示热电偶已连接，这时显示温度
	{
		if(temp > 999)
		{
			SMG_DisChar(3,temp/1000);
		}
		if(temp > 99)
		{
			SMG_DisChar(2,(temp%1000)/100);
		}
		if(temp > 9)
		{
			SMG_DisChar(1,(temp/10)%10);
		}
		SMG_DisChar(0,temp%10);
	}
	else						//Flag_connect为1表示 热电偶未连接，这时显示FFFF
	{
		SMG_DisChar(3, 0x0F);
		SMG_DisChar(2, 0x0F);
		SMG_DisChar(1, 0x0F);
		SMG_DisChar(0, 0x0F);
		
	}
}

void main(void)
{
	unsigned char cnt = 11;
	unsigned int t;

	while(1)
	{
		t=MAX6675_ReadReg();

  		Flag_connect=t&0x04;		 	//读出数据的D2位是热电偶掉线标志位，该位为1表示掉线，该位为0表示连接
  		Flag_connect=Flag_connect>>2; 	//MAX6675是否在线

		t = t<<1;					   	//读出来的数据的D3~D14是温度值
		t = t>>4;
		MAX6675_Temp = t/4;				//测得的温度单位是0.25，所以要乘以0.25（即除以4）才能得到以度为单位的温度值
		
		for(t=0; t<200; t++)			//显示温度
		{  
			Display(MAX6675_Temp);
		}

	}
}
