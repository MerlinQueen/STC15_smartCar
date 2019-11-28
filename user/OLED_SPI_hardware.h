#ifndef __OLED_SPI_HARDWARE_H__
#define __OLED_SPI_HARDWARE_H__
#include <STC15.h>
#include "public.h"
#define	SPIF	0x80  // SPSTAT.7 SPIF传输完成标志位 写1清零
#define WCOL	0x40 // SPISTAT.6 SPI写冲突标志位 写1清零

#define	SSIG	1  	// SPCTL.7  1:忽略SS脚,MSTR位决定主机还是从机  0:SS脚用于决定主从机
#define SPEN 	1 // SPCTL.6 1:允许SPI 0:禁止SPI所有管脚均可为普通IO
#define DORD	0 //SPCTL.5 	Data order 数据顺序, 1:LSB先发,0:MSB先发
#define MSTR	1 //SPCTL.4  MASTER 主从机选择  1:主机模式 0:从机模式
#define CPOL	0 //SPCTL.3	clock polarity时钟极性 ,1:空闲时SCLK高电平 ,0:空闲时SCLK为低电平
#define CPHA	0 // SPCTL.2	Clock phase 时钟相位,决定何时采样何时发送数据 1:奇次沿采样,偶次沿发送 0:偶次沿采样奇次沿采样
#define SPR1	0 // SPI rate
#define SPR0 	0 // SPI Rate  //时钟速率选择位  00  foc/4 01foc/8
//sbit SCLK = P2^ 1;         // 辅助寄存器配置,配置高速SPI的端口 SSP2.4,MOSI 2.3,MISO 2.2,SCLK 2.1
//sbit MOSI = P2 ^ 3;
sbit OLED_RES = P2^ 4;
sbit OLED_DC  = P2 ^ 5;   //0写命令,1写数据;
sbit OLED_CS  = P2^ 6;
void OledWriteCom(u8 Oled_com);   // OLED写命令
void OledWriteDat(u8 Oled_dat);  // OLED写数据
//void OLED_ON(void);  // OLED唤醒
//void OLED_OFF(void); // OELD休眠
//void OLED_display_str(u8 y, u8 x,u8 *input_str,u8 font_size); // 在(0-7)y行,(0-127)x列  显示字符串内容  字体大小1,2
void OLED_Init(void);  // 初始化
void OLED_cls(void);	// 清屏
void OLED_display_str(u8 y, u8 x, u8 *input_str); // 在(0-7)y行,(0-127)x列  显示字符串内容
//void OLED_Show_Chinese(u8 y, u8 x, u8 N); //  显示ASCII_image.H中存储的第N个汉字
//void OLED_DrawBMP(u8 y0,u8 x0,u8 y1, u8 x1,u8 N_bmp); // 画点 ,在以x,y为起点显示BMP图像数组   y0,x0显示起点,y1,x1显示结束位置,N_bmp显示第N副图
#endif