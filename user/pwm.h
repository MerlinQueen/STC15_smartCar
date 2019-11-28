#ifndef __PWM_H__
#define __PWM_H__

/************管脚定义***********************
 * PWM1------P4.5-----IN1   左电机
 * PWM2------P4.4-----IN2   左电机
 *******************************************
 * PWM3------P0.6-----IN3   右电机
 * PWM4------P0.7-----IN4   右电机
 *******************************************/
void PWM_Init(void);      // PWM 初始化开启PWM模块
void PWM1_Out(unsigned int Duty); // 占空比输出范围0-100
void PWM2_Out(unsigned int Duty); // 占空比输出范围0-100
	
void PWM3_Out(unsigned int Duty); // 占空比输出范围0-100
void PWM4_Out(unsigned int Duty); // 占空比输出范围0-100

void Go_Forward(unsigned int Speed); // 向前行驶    Speed参数范围:0-100
void Turn_Left(unsigned int vl,unsigned int vr);  // 设置左转:转速,角度       (Speed+angel)参数范围:0-100
void Turn_Right(unsigned int vl,unsigned int vr); // 设置右转:转速,角度     	(Speed+angel)参数范围:0-100
void FreeCar(void);	        // 待机
void StopCar(void);       // 急刹
void Turn(unsigned int vl,unsigned int vr);


#endif