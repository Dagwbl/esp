/**********************TS-51A��Ƭ������������************************
*		Copyright (c) 2013,52̽�� ��Ƭ��������
*		All rights reserved
*       
*		�ļ����ƣ�led.c
*		������������ȡDS18B20��64λ�����벢��ʾ��1602Һ����
*					�����ȡ��ȷ���������Һ����һ����ʾDS18B20 OK���ڶ�����ʾ������
*					�����ȡʧ�ܣ�����Һ������ʾDS18B20  ERR0R PLEASE CHECK 
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

void Delay(unsigned int num)
{
  while( --num );
}
/*
* ��  �ܣ�DS18B20 ��λ
* ��  ������
* ����ֵ����
*/
void DS18B20_Reset(void)
{  
     DS_Data = 1;      //DQ��λ
     Delay(8);    //������ʱ

     DS_Data = 0;      //��DQ����
     Delay(90);   //��ȷ��ʱ ���� 480us

     DS_Data = 1;       //��������
     Delay(8);

     presence = DS_Data;    //��ȡ�����ź�
     Delay(100);
     DS_Data = 1; 
     
//     return(presence); //�����źţ�0=presence,1= no presence
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
* ����ֵ�������ŵ��¶�ֵ
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
* ��  �ܣ�DS18B20 ��ȡ���к�
* ��  ������
* ����ֵ����
*/
void DS18B20_ReadSN(void)
{
	unsigned char i;

	DS18B20_Reset();	
	DS18B20_WriteByte(0x33);		// ��������Ĳ���

	for (i=0; i<8; i++)
	{
		RomCode[i] = DS18B20_ReadByte() ; 
	}
}

/*
* ��  �ܣ�DS18B20��CRC8У��
* ��  ������
* ����ֵ��CRC���
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
* ��  �ܣ��Ѷ�ȡ�����к�ת���ɿ�����1602����ʾ���ַ���
* ��  ������
* ����ֵ����
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
* ��  �ܣ�����������
* ��  ������
* ����ֵ����
*/
void Beep(void)
{
	unsigned char i;

	for (i=0; i<100; i++)
	{
		DelayMs(1);
		BEEP = !BEEP;               		//BEEPȡ��
	} 
	BEEP = 1;                      		//�رշ�����
}


/*
* ��  �ܣ���ʾ��ȷ�˵�
* ��  ������
* ����ֵ����
*/
void  Ok_Menu ()
{ 
	LCD1602_ShowString(0, str3);

	LCD1602_ShowString(1, str4);
}

/*
* ��  �ܣ���ʾ����˵�
* ��  ������
* ����ֵ����
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
		DS18B20_ReadSN();    	//��ȡ64λ������
		CRC = DS18B20_CRC8(); 	//CRCЧ��

		if(CRC == 0)         	//CRCЧ����ȷ
		{
			Conv_RomCode();		//��ʾ64λ������ 
			Ok_Menu ();
			Beep();
			DelayMs(1000);			
		}
		else			   		//CRCʧ�ܵ���ʾ
		{
			LCD1602_ShowString(1, " CRC is wrong . ");
			DelayMs(1000);	
		}
		while(!presence)	  	//���DS18B20�Ƿ����
	 	{						//������ھͻ���������ѭ����
	  		DS18B20_Reset();	//���DS18B20�����ڣ�����������
	  		DelayMs(1000);
	 	}
		   	
		DS18B20_Reset();   		//���DS18B20�Ƿ����	
		while(presence)	   		//��������ھͻ���������ѭ��
		{  					   	//���DS18B20���ڣ�����������
			Error_Menu (); 
			DS18B20_Reset();
			Beep();
			DelayMs(1000);

		}
	}
}
