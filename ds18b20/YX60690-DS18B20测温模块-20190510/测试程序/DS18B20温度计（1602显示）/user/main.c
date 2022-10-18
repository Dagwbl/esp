/**********************TS-51A��Ƭ������������************************
*		Copyright (c) 2013,52̽�� ��Ƭ��������
*		All rights reserved
*       
*		�ļ����ƣ�main.c
*		��������������DS18B20ת���¶ȣ���ȡ����ʾ��������ϡ�
* 
*		��ǰ�汾��1.0
*		��    �ߣ�52̽��
*       ������ڣ�2013-5-29
*
*		ȡ���汾����
*       �Ľ����ݣ���
*
*		ʹ�û�����TS-51A������ + STC89C52 + 11.0592M���� + Keil C51 V9.0 
*       
*		�Ա���ַ��http://shop104208028.taobao.com/
*		QQ      : 10903659
*		QQȺ    ��336397723  336398729   336398900
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
* ��  �ܣ��������ܵ���ʾ����
* ��  ������
* ����ֵ����
*/
void SMG_CLR( void )
{
	P0 = 0x00;	 	//����Ҫ��ʾ�Ķ���
	dx = 1;
	dx = 0;
	
	P0 = 0x00;		//����Ҫ��ʾ��λ��,ֻ��ʾһλ����
	wx = 1;
	wx = 0;
}

/*
* ��  �ܣ�DS18B20 ��λ
* ��  ������
* ����ֵ����
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
* ��  �ܣ�DS18B20 ��ȡһλ����
* ��  ������
* ����ֵ����ȡ��λ
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
* ��  �ܣ�DS18B20 ��ȡһ���ֽ�
* ��  ������
* ����ֵ����ȡ���ֽ�
*/
uchar DS18B20_ReadByte(void)	
{
	unsigned char i,j,dat;
	
	dat=0;
	for(i=1; i<=8; i++)
	{
		j = DS18B20_ReadBit();
		dat=(j<<7)|(dat>>1);   //�������������λ����ǰ�棬�����պ�һ���ֽ���DAT��
	}

	return(dat);
}

/*
* ��  �ܣ�DS18B20 д��һ���ֽ�
* ��  ����Ҫд����ֽ�
* ����ֵ����
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
* ��  �ܣ�DS18B20 ��ʼ�¶�ת��
* ��  ������
* ����ֵ����
*/
void DS18B20_Change(void)	
{
	DS18B20_Reset();
	DelayMs(5);
	DS18B20_WriteByte(0xcc); 	 // address all drivers on bus
	DS18B20_WriteByte(0x44);  	//  initiates a single temperature conversion  
}

/*
* ��  �ܣ�DS18B20 �õ��¶�ֵ
* ��  ������
* ����ֵ����
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
		DS18B20_Change();  					//��ʼת��

		temp = DS18B20_GetTemp(); 		 	//�õ����¶ȱ�����temp��

		d1 = temp / 100;
		d2 = (temp % 100) / 10;
		d3 = temp % 10;						//�����ʮλ����λ��С��λ

		if(temp<0)						  	//�¶�С��0��
		{		   
			LCD1602_ShowChar(0x16, '-');	//��ʾ����
		}
		LCD1602_ShowChar(0x16, '0'+d1);	  	//��ʾʮλ
		LCD1602_ShowChar(0x17, '0'+d2);		//��ʾ��λ����0x80��Ϊ������ʾС����
		LCD1602_ShowChar(0x18, '.');
		LCD1602_ShowChar(0x19, '0'+d3);		//��ʾС��λ
	}
}
