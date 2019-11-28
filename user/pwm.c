#include <STC15.h>
#include "pwm.h"
#define CYCLE   0x7000L     //定义PWM周期(最大值为32767)  ,28627,周期只影响精度

sbit PWM3 = P4 ^ 5;
sbit PWM4 = P4 ^ 4; //左电机 PWM输出管脚定义

sbit PWM6 = P0 ^ 6;
sbit PWM7 = P0 ^ 7;	//右电机 PWM输出管脚定义


void PWM_Init(void)
{
	P_SW2 |= 0x80; 					//使能访问PWM在扩展RAM区的特殊功能寄存器XSFR
	PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平,即第一次翻转电平为低电平
	PWMCKS = 0x03;                  //选择PWM的时钟为Fosc/(0+1)   系统时钟4分频
	PWMC = CYCLE;                   //设置PWM周期,定义PWM周期(最大值为32767)

	PWM3CR = 0x08;                  //选择PWM2输出到P4.5,不使能中断
	PWM4CR = 0x08;					//选择PWM2输出到P4.4,不使能中断
	PWM6CR = 0x08;					//选择PWM2输出到P0.6,不使能中断
	PWM7CR = 0x08;					//选择PWM2输出到P0.7,不使能中断

	PWM3T1 = 0x0001;				// 设置第一次翻转时间
	PWM3T2 = 0;						// 设置第一次翻转时间
	PWM4T1 = 0x0001;
	PWM4T2 = 0;
	PWM6T1 = 0x0001;   		 // 防止翻转时间全为0 ,产生竞争
	PWM6T2 = 0;
	PWM7T1 = 0x0001;
	PWM7T2 = 0;
	PWMCR |= 0x80;                  //使能PWM模块
	P_SW2 &= ~0x80;
}

void PWM1_Out(unsigned int Duty) // 输出范围0-100
{
	if (Duty > 100)   // Duty=0 时,IO口输出低电平
	{
		PWMCR &= ~(1 << 1); // 设置引脚P4.5状态为通用IO,IO口复用
		PWM3 = 0;
	}
	else
	{
		P_SW2 |= 0x80;  	 // 开启XSRF
		PWM3T1 = 0x0001;
		PWM3T2 = CYCLE * Duty / 100;	// 重新设置翻转时间,即高电平持续时间
		P_SW2 &= ~0x80;			// 关闭XSRF
		PWMCR |= (1 << 1); // 开启PWM3输出
	}
}

void PWM2_Out(unsigned int Duty) // 输出范围0-100
{
	if (Duty > 100)   // Duty=0 时,IO口输出低电平
	{
		PWMCR &= ~(1 << 2); // 设置引脚P4.4状态为通用IO,IO口复用
		PWM4 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM4T1 = 0x0001;
		PWM4T2 = CYCLE * Duty / 100;	// 重新设置翻转时间,即高电平持续时间
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 2); // 开启PWM3输出
	}
}

void PWM3_Out(unsigned int Duty) // 输出范围0-100
{
	if (Duty > 100)   // Duty=0 时,IO口输出低电平
	{
		PWMCR &= ~(1 << 5); //设置引脚P0.7状态为通用IO
		PWM7 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM7T1 = 0x0001;
		PWM7T2 = CYCLE * Duty / 100;	// 重新设置翻转时间,即高电平持续时间
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 5); // 开启PWM3输出
	}
}

void PWM4_Out(unsigned int Duty) // 输出范围0-100
{
	if (Duty > 100)   // Duty=0 时,IO口输出低电平
	{
		PWMCR &= ~(1 << 4); //设置引脚P0.6状态为通用IO
		PWM6 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM6T1 = 0x0001;
		PWM6T2 = CYCLE * Duty / 100;	// 重新设置翻转时间,即高电平持续时间
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 4); // 开启PWM3输出
	}
}


void Go_Forward(unsigned int Speed)  // 转速设置0-100 ,0停止1全速前进
{
	PWM1_Out(Speed);
	PWM2_Out(0);

	PWM3_Out(Speed);
	PWM4_Out(0);
}

void Turn_Left(unsigned int vl,unsigned int vr)  // 设置左转:转速,角度
{
	 PWM1_Out(0);
	 PWM2_Out(vl);  		//  右轮速度大于左轮,左转

	 PWM3_Out(vr);
	 PWM4_Out(0);
}

void Turn_Right(unsigned int vl,unsigned int vr) // 设置右转:转速,角度
{
	 PWM1_Out(vl);
	 PWM2_Out(0);
					// 右轮速度小于左轮,右转
	 PWM3_Out(0);
	 PWM4_Out(vr);
}


void Turn(unsigned int vl,unsigned int vr) // 设置右转:转速,角度
{
	 PWM1_Out(vl);
	 PWM2_Out(0);
					// 右轮速度小于左轮,右转
	 PWM3_Out(vr);
	 PWM4_Out(0);
}


void StopCar(void)
{
	PWM1_Out(1);
	PWM2_Out(1);

	PWM3_Out(1);
	PWM4_Out(1);
}

void FreeCar(void)
{
	PWM1_Out(0);
	PWM2_Out(0);

	PWM3_Out(0);
	PWM4_Out(0);
}



