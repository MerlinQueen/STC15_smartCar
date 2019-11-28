#include "timer.h"
#include "stdio.h"
#include "OLED_SPI_hardware.h"
u16 TimerCnt=0; 
u16 Second=0;
u8 Time_Buffer[4]; // ������ʻʱ��
void TIMER_Init(void)
{	
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ0
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xB8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
    ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
}

void timer0_isr() interrupt 1
{
	TimerCnt++;
	if(1000==TimerCnt)     // ��ʱ1s
	{
		Second++;    // ÿ��1s  Sec+1  ��������ֵ
		TimerCnt=0; // ����,�����ۼ�
	}
}

void TimeDispaly(void)        // ����һ��������ʾȫ����ʱ
{
	if((0==TR0)&&(Second!=0))  // �����յ�ֹͣ��ʱ
	{
		sprintf(Time_Buffer,"%4d",Second);
		OLED_display_str(5,30,Time_Buffer);    // ��ʾϵͳ׼������
		while(TR0!=0);
		Second =0;  // ����ʱ��
	}
//	else
//	{
//	  OLED_display_str(5,30,"0");    // ��ʾϵͳ׼������
//	}
}
