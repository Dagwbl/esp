#include "lcd1602.h"

unsigned char TempData[8];
unsigned char SecondLine[]="                ";
unsigned char FirstLine[] ="                ";


/*
* 功  能：在11.0592M晶振下的延时时间为 1ms
* 参  数：毫秒数
* 返回值：无
*/
void DelayMs(unsigned int i)
{
	unsigned int j,k;

	for(j=i; j>0; j--)
		for(k=114; k>0; k--);
}

/*
* 功  能：LCD1602写命令
* 参  数：要写入的命令字节
* 返回值：无
*/
void WriteCommand(unsigned char c)
{
	DelayMs(5);		
	LCD1602_EN = 0;
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	_nop_();
	LCD1602_EN = 1;
	Data = c;
	LCD1602_EN = 0;
}

/*
* 功  能：LCD1602写数据
* 参  数：要写入的数据字节
* 返回值：无
*/
void WriteData(unsigned char c)
{
	DelayMs(5);	
	LCD1602_EN = 0;
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	_nop_();
	LCD1602_EN = 1;
	Data = c;
	LCD1602_EN = 0;
	LCD1602_RS = 0;
}

/*
* 功  能：LCD1602在指定的位置显示一个字符
* 参  数：pos:要显示的位置，c:要显示的字符
* 返回值：无
*/
void LCD1602_ShowChar(unsigned char pos, unsigned char c)
{
	unsigned char p;

	if (pos >= 0x10)
	{
		p = pos + 0xb0; 		//是第二行则命令代码高4位为0xc
	}
	else 
	{
		p = pos + 0x80; 		//是第二行则命令代码高4位为0x8
	}

	WriteCommand (p);			//write command
	WriteData(c);   			//write data
}

/*
* 功  能：LCD1602显示一个字符串
* 参  数：line:要显示在第几行，ptr：要显示的字符串指针
* 返回值：无
*/
void LCD1602_ShowString (unsigned char line, char *ptr)
{
	unsigned char i, j;

	i = line<<4;

	for (j=0; j<16; j++)
	{
		LCD1602_ShowChar (i++, *(ptr+j));	
	}
}

/*
* 功  能：LCD1602初始化
* 参  数：无
* 返回值：无
*/
void Lcd1602_Init(void)
{
	DelayMs(15);
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x06); 		//显示光标移动位置
	WriteCommand(0x0c); 		//显示开及光标设置
	WriteCommand(0x01); 		//显示清屏

}
