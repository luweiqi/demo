#ifndef __5110_H__
#define __5110_H__
#include "stm32f10x.h"

//l reset
#define LED_RES_PORT GPIOA 
#define LED_RES_PORT_PIN GPIO_Pin_3
//h data l command
#define LED_RS_PORT GPIOA
#define LED_RS_PORT_PIN GPIO_Pin_4

#define LED_CS_PORT GPIOA
#define LED_CS_PORT_PIN GPIO_Pin_2

#define uchar unsigned char
#define uint unsigned int
void delay_ms(unsigned int ms);
void delay_us(unsigned int us)  ;
void LCD_write_char(unsigned char c);
void LCD_set_XY(unsigned char X, unsigned char Y);

void LCD_write_String(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_Init(void);
void GUI_Point(unsigned char x,unsigned char y,unsigned char color);
void LCD_Draw_Line_x( unsigned char X0, unsigned char X1, unsigned char Y, unsigned char Color );
void LCD_Draw_Line_Y( unsigned char X, unsigned char Y0, unsigned char Y1, unsigned char Color );
char DrawLine(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color);
void fsin2(void);
void GUI_Circle(unsigned char x0,unsigned char y0,unsigned char r,unsigned char color);
char DrawFillRectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color);
char DrawFillSquare(unsigned char x0,unsigned char y0,unsigned char width,unsigned char color);
char DrawRectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color);
char DrawSquare(unsigned char x0,unsigned char y0,unsigned char width,unsigned char color);
void clear_wave(void);
void GUI_Point2(unsigned char x,unsigned char y,unsigned char color);
void Display(void);
void LCD_clear(void);
void mai(void);
#endif
