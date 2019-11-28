#ifndef __ADC_H__
#define __ADC_H__
#include "public.h"
void ADC_Init(void);
u16 getADC10bitResult(char channel);  //获取ADC返回的数据 ,channel 0-7 对应ADCP1^0-P1^7端口


#endif                  