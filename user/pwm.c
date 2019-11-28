#include <STC15.h>
#include "pwm.h"
#define CYCLE   0x7000L     //����PWM����(���ֵΪ32767)  ,28627,����ֻӰ�쾫��

sbit PWM3 = P4 ^ 5;
sbit PWM4 = P4 ^ 4; //���� PWM����ܽŶ���

sbit PWM6 = P0 ^ 6;
sbit PWM7 = P0 ^ 7;	//�ҵ�� PWM����ܽŶ���


void PWM_Init(void)
{
	P_SW2 |= 0x80; 					//ʹ�ܷ���PWM����չRAM�������⹦�ܼĴ���XSFR
	PWMCFG = 0x00;                  //����PWM�������ʼ��ƽΪ�͵�ƽ,����һ�η�ת��ƽΪ�͵�ƽ
	PWMCKS = 0x03;                  //ѡ��PWM��ʱ��ΪFosc/(0+1)   ϵͳʱ��4��Ƶ
	PWMC = CYCLE;                   //����PWM����,����PWM����(���ֵΪ32767)

	PWM3CR = 0x08;                  //ѡ��PWM2�����P4.5,��ʹ���ж�
	PWM4CR = 0x08;					//ѡ��PWM2�����P4.4,��ʹ���ж�
	PWM6CR = 0x08;					//ѡ��PWM2�����P0.6,��ʹ���ж�
	PWM7CR = 0x08;					//ѡ��PWM2�����P0.7,��ʹ���ж�

	PWM3T1 = 0x0001;				// ���õ�һ�η�תʱ��
	PWM3T2 = 0;						// ���õ�һ�η�תʱ��
	PWM4T1 = 0x0001;
	PWM4T2 = 0;
	PWM6T1 = 0x0001;   		 // ��ֹ��תʱ��ȫΪ0 ,��������
	PWM6T2 = 0;
	PWM7T1 = 0x0001;
	PWM7T2 = 0;
	PWMCR |= 0x80;                  //ʹ��PWMģ��
	P_SW2 &= ~0x80;
}

void PWM1_Out(unsigned int Duty) // �����Χ0-100
{
	if (Duty > 100)   // Duty=0 ʱ,IO������͵�ƽ
	{
		PWMCR &= ~(1 << 1); // ��������P4.5״̬Ϊͨ��IO,IO�ڸ���
		PWM3 = 0;
	}
	else
	{
		P_SW2 |= 0x80;  	 // ����XSRF
		PWM3T1 = 0x0001;
		PWM3T2 = CYCLE * Duty / 100;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
		P_SW2 &= ~0x80;			// �ر�XSRF
		PWMCR |= (1 << 1); // ����PWM3���
	}
}

void PWM2_Out(unsigned int Duty) // �����Χ0-100
{
	if (Duty > 100)   // Duty=0 ʱ,IO������͵�ƽ
	{
		PWMCR &= ~(1 << 2); // ��������P4.4״̬Ϊͨ��IO,IO�ڸ���
		PWM4 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM4T1 = 0x0001;
		PWM4T2 = CYCLE * Duty / 100;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 2); // ����PWM3���
	}
}

void PWM3_Out(unsigned int Duty) // �����Χ0-100
{
	if (Duty > 100)   // Duty=0 ʱ,IO������͵�ƽ
	{
		PWMCR &= ~(1 << 5); //��������P0.7״̬Ϊͨ��IO
		PWM7 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM7T1 = 0x0001;
		PWM7T2 = CYCLE * Duty / 100;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 5); // ����PWM3���
	}
}

void PWM4_Out(unsigned int Duty) // �����Χ0-100
{
	if (Duty > 100)   // Duty=0 ʱ,IO������͵�ƽ
	{
		PWMCR &= ~(1 << 4); //��������P0.6״̬Ϊͨ��IO
		PWM6 = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM6T1 = 0x0001;
		PWM6T2 = CYCLE * Duty / 100;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 4); // ����PWM3���
	}
}


void Go_Forward(unsigned int Speed)  // ת������0-100 ,0ֹͣ1ȫ��ǰ��
{
	PWM1_Out(Speed);
	PWM2_Out(0);

	PWM3_Out(Speed);
	PWM4_Out(0);
}

void Turn_Left(unsigned int vl,unsigned int vr)  // ������ת:ת��,�Ƕ�
{
	 PWM1_Out(0);
	 PWM2_Out(vl);  		//  �����ٶȴ�������,��ת

	 PWM3_Out(vr);
	 PWM4_Out(0);
}

void Turn_Right(unsigned int vl,unsigned int vr) // ������ת:ת��,�Ƕ�
{
	 PWM1_Out(vl);
	 PWM2_Out(0);
					// �����ٶ�С������,��ת
	 PWM3_Out(0);
	 PWM4_Out(vr);
}


void Turn(unsigned int vl,unsigned int vr) // ������ת:ת��,�Ƕ�
{
	 PWM1_Out(vl);
	 PWM2_Out(0);
					// �����ٶ�С������,��ת
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


