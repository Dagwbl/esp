#include<reg52.h>
#include<stdio.h>
#include<intrins.h>

sbit LCD1602_RS = P1^3; //定义端口 
sbit LCD1602_RW = P1^4;
sbit LCD1602_EN = P1^5;


#define Data  P0    	//数据端口

unsigned char TempData[];
unsigned char SecondLine[];
unsigned char FirstLine[];

void DelayMs(unsigned int ms);

void LCD1602_ShowChar(unsigned char pos ,unsigned char c);

void LCD1602_ShowString (unsigned char line, char *ptr);

void LCD1602_Init();

