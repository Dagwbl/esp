#include "lcd1602.h"

unsigned char TempData[8];
unsigned char SecondLine[]="                ";
unsigned char FirstLine[] ="                ";


/*
* ��  �ܣ���11.0592M�����µ���ʱʱ��Ϊ 1ms
* ��  ����������
* ����ֵ����
*/
void DelayMs(unsigned int i)
{
	unsigned int j,k;

	for(j=i; j>0; j--)
		for(k=114; k>0; k--);
}

/*
* ��  �ܣ�LCD1602д����
* ��  ����Ҫд��������ֽ�
* ����ֵ����
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
* ��  �ܣ�LCD1602д����
* ��  ����Ҫд��������ֽ�
* ����ֵ����
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
* ��  �ܣ�LCD1602��ָ����λ����ʾһ���ַ�
* ��  ����pos:Ҫ��ʾ��λ�ã�c:Ҫ��ʾ���ַ�
* ����ֵ����
*/
void LCD1602_ShowChar(unsigned char pos, unsigned char c)
{
	unsigned char p;

	if (pos >= 0x10)
	{
		p = pos + 0xb0; 		//�ǵڶ�������������4λΪ0xc
	}
	else 
	{
		p = pos + 0x80; 		//�ǵڶ�������������4λΪ0x8
	}

	WriteCommand (p);			//write command
	WriteData(c);   			//write data
}

/*
* ��  �ܣ�LCD1602��ʾһ���ַ���
* ��  ����line:Ҫ��ʾ�ڵڼ��У�ptr��Ҫ��ʾ���ַ���ָ��
* ����ֵ����
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
* ��  �ܣ�LCD1602��ʼ��
* ��  ������
* ����ֵ����
*/
void Lcd1602_Init(void)
{
	DelayMs(15);
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x38); 		//display mode
	WriteCommand(0x06); 		//��ʾ����ƶ�λ��
	WriteCommand(0x0c); 		//��ʾ�����������
	WriteCommand(0x01); 		//��ʾ����

}
