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
sbit Stop_Car = P0 ^ 5; // �������ػ��߸ɻɹ�����, ���ֹͣλ
sbit KEY_Start = P2 ^ 0;
volatile u8 countt, OnFlag = 0; // countt��¼�ڼ��μ�⵽ֹͣ�ź�(Ҳ�������˶���Ȧ),OnFlag������־λ

u8 AD_Buffer[3];  // ����AD�ɼ���������(10λAD���1023),�����ڷ��͸�OLED��ʾ
u8 AD_Left[4];	// ����в���ֵ
u8 AD_Mid[4]; 	// �м��в���ֵ
u8 AD_Right[4];   //����в���ֵ

void GPIO_Init(void);  // GPIO��ʼ
void ADC_Sample(void);  // ADC���ݲɼ�
void Runing(void);  	// ��������
void Tracking(void);    // Ѱ������,�ж��Ƿ�ƫ����



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

void ADC_Sample(void)  // ����·��д�������⵽��,��ѹ������ȡƽ��ֵ
{
	AD_Left[1] = getADC10bitResult(0); // ���е�ѹ
	AD_Left[2] = getADC10bitResult(0); // ���е�ѹ
	AD_Left[3] = getADC10bitResult(0); // ���е�ѹ
	AD_Left[0] = (AD_Left[1] + AD_Left[2] + AD_Left[3]) / 3; // ��ƽ��ֵ

	AD_Mid[1] = getADC10bitResult(1); // �м��е�ѹ
	AD_Mid[2] = getADC10bitResult(1); // �м��е�ѹ
	AD_Mid[3] = getADC10bitResult(1); // �м��е�ѹ
	AD_Mid[0] = (AD_Mid[1] + AD_Mid[2] + AD_Mid[3]) / 3;

	AD_Right[1] = getADC10bitResult(2); // �Ҳ��е�ѹ
	AD_Right[2] = getADC10bitResult(2); // �Ҳ��е�ѹ
	AD_Right[3] = getADC10bitResult(2); // �Ҳ��е�ѹ
	AD_Right[0] = (AD_Right[1] + AD_Right[2] + AD_Right[3]) / 3;
}

void Runing(void)
{
	if (KEY_Start == 0)
	{
		DelayMS(15); // ����
		if (KEY_Start == 0)
		{
			OnFlag = 1;
			LED = 0;    // С��������ʾLED��
			TR0 = 1;   //��ʱ��0��ʼ��ʱ
		}
	}
	if (OnFlag == 1)
	{
		if (Stop_Car == 0)
		{
			StopCar();  // ��ɲ,ͣ��
			OnFlag = 0;
			TR0 = 0;  // ֹͣ��ʱ
			LED = 1; // Ϩ��LED��ʾ��
			DelayMS(2000);
			FreeCar();   // ����
		}
		else
		{
			Tracking(); //Ѱ��
		}
	}
}

/* ���������ź��������е��߼��жϱ� 						7��״̬�ж�
 * ************************************************************************
 * 	��AD 		��AD			��AD*	����		�ҵ��		״̬
 * ************************************************************************
 *  ��			ǿ			��	*	��			��			ֱ��
 * ************************************************************************
 *  ǿ       				��	*	��			��			�����ת
 * ************************************************************************
 *  ��           			ǿ	*	��			��			�����ת
 * ************************************************************************
 *  ��          	     	��	*	��			ͣ			С��ƫ���С
 * ************************************************************************
 *  ��             			ǿ	*	��			ͣ			С��ƫ����
 * ************************************************************************
 *  ��             			��	*	ͣ			��			С��ƫ�Ҳ�С
 * ************************************************************************
 *  ǿ             			��	*	ͣ			��			С��ƫ�Ҳ��
 * ************************************************************************
 */

void Tracking(void)
{
	/************mid����70*****ֱ��*********************/
	if (AD_Mid[0] >= 70)
	{
		Go_Forward(99);
	}
	/*************mid40-70***ֱ��ƫ�ƽ�С��60��*********************/
	else if (AD_Mid[0] >= 40 && AD_Mid[0] <= 70 )
	{

		/*****************��ƫ*********************/
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

		/*****************��ƫ*********************/
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
	/*************midС��30****��ת��*********************/
	else
	{
		/*****************��ת***********************/
		if (AD_Left[0] > AD_Right[0])
		{
			Turn_Left(35, 50);
		}
		/*****************�Ҽ�ת**********************/
		else if(AD_Left[0] < AD_Right[0])
		{
			Turn_Right(50, 35);
		}
		/***************�����������*********************/
		else
		{
			Go_Forward(35);
		}
	}
}



void main(void)
{
	GPIO_Init();
	OLED_Init();  // OLED��ʼ��
	PWM_Init();   // PWM��ʼ��
	ADC_Init();   // ��ѹ�ɼ���ʼ��P1��8·���ɲɼ�
	TIMER_Init();   // ��ʱ����ʼ��
	EA = 1;			// �������ж�
	DelayUS(100);  //  �ȴ�ϵͳ��ʼ�����
	OLED_display_str(1, 20, "SmartCar-V1.0");  // ��ʾϵͳ׼������
	OLED_display_str(3, 0, "Equipment is OK");  // ��ʾϵͳ׼������
	OLED_display_str(5, 0, "Time:      S");  // ��ʾϵͳ׼������
	LED = 0;  // ��ʾ�Լ����
	while (1)
	{
		ADC_Sample();  //  ��ѹ����,������;
		Runing();      //Ѱ��
		TimeDispaly();//��ʾ��ʱ
	}
}
