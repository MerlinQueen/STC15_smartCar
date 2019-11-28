#include <STC15.h>
#include "pwm.h"
#include "delay.h"
#include "ADC.h"
#include "OLED_SPI_hardware.h"
#include "public.h"
#include "timer.h"

sbit led1 = P5 ^ 0;
sbit led2 = P5 ^ 1;
sbit LED = P5 ^ 2;
sbit Stop_Car = P0 ^ 5; // 霍尔开关或者干簧管引脚, 检测停止位
sbit KEY_Start = P2 ^ 0;
volatile u8 countt, OnFlag = 0; // countt记录第几次检测到停止信号(也就是跑了多少圈),OnFlag启动标志位

u8 AD_Buffer[3];  // 保存AD采集到的数据(10位AD最大1023),可用于发送给OLED显示
u8 AD_Left[4];	// 左侧电感采样值
u8 AD_Mid[4]; 	// 中间电感采样值
u8 AD_Right[4];   //左侧电感采样值

void GPIO_Init(void);  // GPIO初始
void ADC_Sample(void);  // ADC数据采集
void Runing(void);  	// 启动函数
void Tracking(void);    // 寻迹函数,判断是否偏离轨道



void GPIO_Init(void)
{
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;

}

void ADC_Sample(void)  // 把三路电感传感器检测到的,电压采样求取平均值
{
	AD_Left[1] = getADC10bitResult(0); // 左电感电压
	AD_Left[2] = getADC10bitResult(0); // 左电感电压
	AD_Left[3] = getADC10bitResult(0); // 左电感电压
	AD_Left[0] = (AD_Left[1] + AD_Left[2] + AD_Left[3]) / 3; // 求平均值

	AD_Mid[1] = getADC10bitResult(1); // 中间电感电压
	AD_Mid[2] = getADC10bitResult(1); // 中间电感电压
	AD_Mid[3] = getADC10bitResult(1); // 中间电感电压
	AD_Mid[0] = (AD_Mid[1] + AD_Mid[2] + AD_Mid[3]) / 3;

	AD_Right[1] = getADC10bitResult(2); // 右侧电感电压
	AD_Right[2] = getADC10bitResult(2); // 右侧电感电压
	AD_Right[3] = getADC10bitResult(2); // 右侧电感电压
	AD_Right[0] = (AD_Right[1] + AD_Right[2] + AD_Right[3]) / 3;
}

void Runing(void)
{
	if (KEY_Start == 0)
	{
		DelayMS(15); // 消抖
		if (KEY_Start == 0)
		{
			OnFlag = 1;
			LED = 0;    // 小车启动提示LED灯
			TR0 = 1;   //定时器0开始计时
		}
	}
	if (OnFlag == 1)
	{
		if (Stop_Car == 0)
		{
			StopCar();  // 急刹,停车
			OnFlag = 0;
			TR0 = 0;  // 停止计时
			LED = 1; // 熄灭LED提示灯
			DelayMS(2000);
			FreeCar();   // 待机
		}
		else
		{
			Tracking(); //寻迹
		}
	}
}

/* 传感器的信号与电机空中的逻辑判断表 						7种状态判断
 * ************************************************************************
 * 	左AD 		中AD			右AD*	左电机		右电机		状态
 * ************************************************************************
 *  弱			强			弱	*	快			快			直行
 * ************************************************************************
 *  强       				弱	*	慢			快			弯道左转
 * ************************************************************************
 *  弱           			强	*	快			慢			弯道右转
 * ************************************************************************
 *  无          	     	弱	*	慢			停			小车偏左差小
 * ************************************************************************
 *  无             			强	*	快			停			小车偏左差大
 * ************************************************************************
 *  弱             			无	*	停			慢			小车偏右差小
 * ************************************************************************
 *  强             			无	*	停			快			小车偏右差大
 * ************************************************************************
 */

void Tracking(void)
{
	/************mid大于70*****直行*********************/
	if (AD_Mid[0] >= 70)
	{
		Go_Forward(99);
	}
	/*************mid40-70***直线偏移角小于60°*********************/
	else if (AD_Mid[0] >= 40 && AD_Mid[0] <= 70 )
	{

		/*****************左偏*********************/
		if (AD_Left[0] >= 30 && AD_Right[0] < 30)
		{

			if (AD_Left[0] >= 30 && AD_Left[0] < 35)
			{
				Turn(80, 90);
			}
			else if (AD_Left[0] >= 35 && AD_Left[0] < 40)
			{
				Turn(75, 90);
			}
			else if (AD_Left[0] >= 40 && AD_Left[0] < 45)
			{
				Turn(65, 85);
			}
			else if (AD_Left[0] >= 45 && AD_Left[0] < 50)
			{
				Turn(55, 85);
			}
			else if (AD_Left[0] >= 50 && AD_Left[0] < 55)
			{
				Turn(40, 80);
			}
			else if (AD_Left[0] >= 50 && AD_Left[0] < 60)
			{
				Turn(35, 70);
			}
			else
			{
				Turn(0, 60);
			}
		}

		/*****************右偏*********************/
		if (AD_Left[0] < 30 && AD_Right[0] >= 30)
		{

			if (AD_Right[0] >= 30 && AD_Right[0] < 35)
			{
				Turn(90, 80);
			}
			else if (AD_Right[0] >= 35 && AD_Right[0] < 40)
			{
				Turn(90, 75);
			}
			else if (AD_Right[0] >= 40 && AD_Right[0] < 45)
			{
				Turn(85, 65);
			}
			else if (AD_Right[0] >= 45 && AD_Right[0] < 50)
			{
				Turn(85, 55);
			}
			else if (AD_Right[0] >= 50 && AD_Right[0] < 55)
			{
				Turn(80, 40);
			}
			else if (AD_Right[0] >= 50 && AD_Right[0] < 60)
			{
				Turn(70, 35);
			}
			else
			{
				Turn(60, 0);
			}
		}
	}
	/*************mid小于30****急转弯*********************/
	else
	{
		/*****************左急转***********************/
		if (AD_Left[0] > AD_Right[0])
		{
			Turn_Left(35, 50);
		}
		/*****************右急转**********************/
		else if(AD_Left[0] < AD_Right[0])
		{
			Turn_Right(50, 35);
		}
		/***************其余特殊情况*********************/
		else
		{
			Go_Forward(35);
		}
	}
}



void main(void)
{
	GPIO_Init();
	OLED_Init();  // OLED初始化
	PWM_Init();   // PWM初始化
	ADC_Init();   // 电压采集初始化P1口8路均可采集
	TIMER_Init();   // 定时器初始化
	EA = 1;			// 开启总中断
	DelayUS(100);  //  等待系统初始化完成
	OLED_display_str(1, 20, "SmartCar-V1.0");  // 显示系统准备就绪
	OLED_display_str(3, 0, "Equipment is OK");  // 显示系统准备就绪
	OLED_display_str(5, 0, "Time:      S");  // 显示系统准备就绪
	LED = 0;  // 表示自检完成
	while (1)
	{
		ADC_Sample();  //  电压采样,并保存;
		Runing();      //寻迹
		TimeDispaly();//显示用时
	}
}
