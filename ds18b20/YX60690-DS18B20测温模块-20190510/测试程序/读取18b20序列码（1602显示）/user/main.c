/**********************TS-51A单片机开发板例程************************
*		Copyright (c) 2013,52探索 单片机开发板
*		All rights reserved
*       
*		文件名称：led.c
*		功能描述：读取DS18B20的64位序列码并显示在1602液晶上
*					如果读取正确结果，则在液晶第一行显示DS18B20 OK，第二行显示序列码
*					如果读取失败，则在液晶上显示DS18B20  ERR0R PLEASE CHECK 
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
sbit BEEP = P2^2;
sbit DS_Data = P2^0;

bit presence;
unsigned char code table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
unsigned char code  str1[ ] = {" DS18B20  ERR0R "};
unsigned char code  str2[ ] = {"  PLEASE CHECK  "};
unsigned char code  str3[ ] = {"   DS18B20 OK   "};
unsigned char str4[ ] = {"                "};
unsigned char RomCode[8]; 
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

void Delay(unsigned int num)
{
  while( --num );
}
/*
* 功  能：DS18B20 复位
* 参  数：无
* 返回值：无
*/
void DS18B20_Reset(void)
{  
     DS_Data = 1;      //DQ复位
     Delay(8);    //稍做延时

     DS_Data = 0;      //将DQ拉低
     Delay(90);   //精确延时 大于 480us

     DS_Data = 1;       //拉高总线
     Delay(8);

     presence = DS_Data;    //读取存在信号
     Delay(100);
     DS_Data = 1; 
     
//     return(presence); //返回信号，0=presence,1= no presence
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
* 返回值：带符号的温度值
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

/*
* 功  能：DS18B20 读取序列号
* 参  数：无
* 返回值：无
*/
void DS18B20_ReadSN(void)
{
	unsigned char i;

	DS18B20_Reset();	
	DS18B20_WriteByte(0x33);		// 读序列码的操作

	for (i=0; i<8; i++)
	{
		RomCode[i] = DS18B20_ReadByte() ; 
	}
}

/*
* 功  能：DS18B20的CRC8校验
* 参  数：无
* 返回值：CRC结果
*/
unsigned char DS18B20_CRC8(void) 
{ 
	unsigned char i, j, CRCBuf,CRC;
	
	CRC = 0;

	for(j=0; j<8; j++)
	{
		CRCBuf = RomCode[j];

		for(i=0; i<8; i++) 
		{ 
			if(((CRC^CRCBuf) & 0x01) == 0)
			{ 
				CRC >>= 1; 
			}
			else
			{ 
				CRC ^= 0x18;   //CRC=X8+X5+X4+1
				CRC >>= 1; 
				CRC |= 0x80; 
			}         
			CRCBuf >>= 1;       
		}
	}
	return CRC;	
}

/*
* 功  能：把读取的序列号转换成可以在1602上显示的字符串
* 参  数：无
* 返回值：无
*/
void Conv_RomCode(void)
{
	unsigned char i, j=0, k, t;
	
	for(i=0;i<8;i++)
	{
		k = RomCode[i];
		
		t = k>>4;
		if(t > 9)
		{
			t = t + 0x37;
		}
		else
		{
			t = t + 0x30;
		}
		str4[j++] = t;
		
		t = k & 0x0f;
		if(t > 9)
		{
			t = t + 0x37;
		}
		else 
		{
			t = t + 0x30;
		}
		
		str4[j++] = t;
	}
}	 

/*
* 功  能：蜂鸣器报警
* 参  数：无
* 返回值：无
*/
void Beep(void)
{
	unsigned char i;

	for (i=0; i<100; i++)
	{
		DelayMs(1);
		BEEP = !BEEP;               		//BEEP取反
	} 
	BEEP = 1;                      		//关闭蜂鸣器
}


/*
* 功  能：显示正确菜单
* 参  数：无
* 返回值：无
*/
void  Ok_Menu ()
{ 
	LCD1602_ShowString(0, str3);

	LCD1602_ShowString(1, str4);
}

/*
* 功  能：显示错误菜单
* 参  数：无
* 返回值：无
*/
void  Error_Menu (void)
{
	LCD1602_ShowString(0, str1);
	
	LCD1602_ShowString(1, str2);
}


void main()
{
	unsigned char CRC;

	SMG_CLR();
	LCD1602_Init();

	DS18B20_Change();	
	
	DS18B20_GetTemp();

	while(1)
	{
		DS18B20_ReadSN();    	//读取64位序列码
		CRC = DS18B20_CRC8(); 	//CRC效验

		if(CRC == 0)         	//CRC效验正确
		{
			Conv_RomCode();		//显示64位序列码 
			Ok_Menu ();
			Beep();
			DelayMs(1000);			
		}
		else			   		//CRC失败的显示
		{
			LCD1602_ShowString(1, " CRC is wrong . ");
			DelayMs(1000);	
		}
		while(!presence)	  	//检查DS18B20是否存在
	 	{						//如果存在就会在这里死循环，
	  		DS18B20_Reset();	//如果DS18B20不存在，则向下运行
	  		DelayMs(1000);
	 	}
		   	
		DS18B20_Reset();   		//检查DS18B20是否存在	
		while(presence)	   		//如果不存在就会在这里死循环
		{  					   	//如果DS18B20存在，则向下运行
			Error_Menu (); 
			DS18B20_Reset();
			Beep();
			DelayMs(1000);

		}
	}
}
