#include <REGX52.H>
#include "intrins.h"			//_nop_();��ʱ������

#define uchar unsigned char
#define uint  unsigned int

sbit  SO  = P3^6;  				//P3.6����SO����
sbit  SCK = P3^4; 				//P3.4����SCK����
sbit  CS  = P3^5;  				//P3.5����CS����

sbit wx = P2^6;
sbit dx = P2^7;

const unsigned char DevID = 1;		//�����豸ID
unsigned char buffer;			//���ڽ��ջ�����
unsigned char rcv_buffer[20];
unsigned char send_buffer[20];
unsigned char flag_ok = 0;		//������ϵı�ʶ
unsigned int MAX6675_Temp;
unsigned char Flag_connect;
unsigned char CRCH,CRCL;
unsigned char wd[2];	  		//wd[]��BCD����¶�
unsigned char flag_dis;			//��ʾˢ�µı�ʶ

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
* ��  �ܣ���ʱ�����ɺ���
* ��  ����������
* ����ֵ����
*/
void DelayMs(unsigned int i)	//һ����ʱ���������������11.0592M�����µ���ʱʱ����Ϊ1ms
{
	unsigned int j,k;

	for(j=i; j>0; j--)
		for(k=114; k>0; k--);
}
/*
* ��  �ܣ��������ָ����λ����ʾָ�����ַ�
* ��  ����place:Ҫ��ʾ��λ�ã�num:Ҫ��ʾ���ַ�
* ����ֵ����
*/
void SMG_DisChar(unsigned char place, unsigned char num)
{
	unsigned char i;

	i = 0x80 >> place;		//������ʾλ��ѡ����Ӧ�������

	P0 = SMG[num];	 	//����Ҫ��ʾ�Ķ���
	dx = 1;
	dx = 0;
	
	P0 = i;		   			//����Ҫ��ʾ��λ��,ֻ��ʾһλ����
	wx = 1;
	wx = 0;

	DelayMs(1);	
}
 
void Display(unsigned int temp)
{
	if(Flag_connect==0)		  	//Flag_connectΪ0 ��ʾ�ȵ�ż�����ӣ���ʱ��ʾ�¶�
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
	else						//Flag_connectΪ1��ʾ �ȵ�żδ���ӣ���ʱ��ʾFFFF
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

  		Flag_connect=t&0x04;		 	//�������ݵ�D2λ���ȵ�ż���߱�־λ����λΪ1��ʾ���ߣ���λΪ0��ʾ����
  		Flag_connect=Flag_connect>>2; 	//MAX6675�Ƿ�����

		t = t<<1;					   	//�����������ݵ�D3~D14���¶�ֵ
		t = t>>4;
		MAX6675_Temp = t/4;				//��õ��¶ȵ�λ��0.25������Ҫ����0.25��������4�����ܵõ��Զ�Ϊ��λ���¶�ֵ
		
		for(t=0; t<200; t++)			//��ʾ�¶�
		{  
			Display(MAX6675_Temp);
		}

	}
}
