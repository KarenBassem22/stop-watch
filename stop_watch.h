#include<stdio.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

void port_init(void);
void timer1_init(void);
void INT0_init(void);    //PD2
void INT1_init(void);    //PD3
void INT2_init(void);    //PB2
