#include "timer.h"
#include "stdio.h"
#include "OLED_SPI_hardware.h"
u16 TimerCnt=0; 
u16 Second=0;
u8 Time_Buffer[4]; // 保存行驶时间
void TIMER_Init(void)
{	
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式0
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xB8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
    ET0 = 1;                        //使能定时器0中断
}

void timer0_isr() interrupt 1
{
	TimerCnt++;
	if(1000==TimerCnt)     // 计时1s
	{
		Second++;    // 每个1s  Sec+1  并返回数值
		TimerCnt=0; // 清零,从新累计
	}
}

void TimeDispaly(void)        // 传入一个数组显示全程用时
{
	if((0==TR0)&&(Second!=0))  // 到达终点停止计时
	{
		sprintf(Time_Buffer,"%4d",Second);
		OLED_display_str(5,30,Time_Buffer);    // 显示系统准备就绪
		while(TR0!=0);
		Second =0;  // 清零时间
	}
//	else
//	{
//	  OLED_display_str(5,30,"0");    // 显示系统准备就绪
//	}
}
