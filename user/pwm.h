#ifndef __PWM_H__
#define __PWM_H__

/************�ܽŶ���***********************
 * PWM1------P4.5-----IN1   ����
 * PWM2------P4.4-----IN2   ����
 *******************************************
 * PWM3------P0.6-----IN3   �ҵ��
 * PWM4------P0.7-----IN4   �ҵ��
 *******************************************/
void PWM_Init(void);      // PWM ��ʼ������PWMģ��
void PWM1_Out(unsigned int Duty); // ռ�ձ������Χ0-100
void PWM2_Out(unsigned int Duty); // ռ�ձ������Χ0-100
	
void PWM3_Out(unsigned int Duty); // ռ�ձ������Χ0-100
void PWM4_Out(unsigned int Duty); // ռ�ձ������Χ0-100

void Go_Forward(unsigned int Speed); // ��ǰ��ʻ    Speed������Χ:0-100
void Turn_Left(unsigned int vl,unsigned int vr);  // ������ת:ת��,�Ƕ�       (Speed+angel)������Χ:0-100
void Turn_Right(unsigned int vl,unsigned int vr); // ������ת:ת��,�Ƕ�     	(Speed+angel)������Χ:0-100
void FreeCar(void);	        // ����
void StopCar(void);       // ��ɲ
void Turn(unsigned int vl,unsigned int vr);


#endif