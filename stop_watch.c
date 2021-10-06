#include "2nd_mini_project.h"

unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hr1=0;
unsigned char hr2=0;

ISR(INT0_vect){    //reset
    //PORTC=0;     //clear all variables
	sec1=0;
    sec2=0;
    min1=0;
    min2=0;
    hr1=0;
    hr2=0;
}

ISR(INT1_vect){    //pause
    TCCR1B &=~(1<<0) &~(1<<2);  //stop the clock
}

ISR(INT2_vect){    //resume
	TCCR1B |=(1<<0)|(1<<2);   //start clock again
}


ISR(TIMER1_COMPA_vect){  //for counting 1 sec
        sec1++;

	    if(sec1==10){
        	sec1=0;
            sec2++;
        }


        if((sec2==6) && (sec1==0)){
            sec2=0;
            min1++;
        }


        if((min1==10) && (sec2==0) && (sec1 ==0)){
        	min1=0;
        	min2++;
        }


        if((min2==6) && (min1==0) && (sec2==0) && (sec1==0)){
            min2=0;
            hr1++;
        }


        if((hr1==10) && (min2==0) && (min1==0) && (sec2==0) && (sec1==0)){
            hr1=0;
            hr2++;
        }


        if((hr2==10) && (hr1==0) && (min2==0) && (min1==0) && (sec2==0) && (sec1==0)){
           	hr2=0;
        }
}

void port_init(void){
	/* INITIALIZE I/O PORTS */
	DDRC=0x0F;   //0000 1111 make 4 pins output (PC0,PC1,PC2,PC3)
	DDRA=0x3F;   //0011 1111 make 6 pins output (PA0,PA1,PA2,PA3,PA4,PA5)
	PORTA=0;     //Initial value for the port
	PORTC=0;     //initial value for the port (PORTC&=0xF0;)
	DDRD=0x0C;   //0000 1100 make 2 pins input (PD2,PD3)
	DDRB=0x04;   //0000 0100 make 1 pin input (PB2)
	PORTD=0x04;  //activate internal pull_up resistor PD2
	PORTB=0x04;  //activate internal pull_up resistor PB2
}

void timer1_init(void){
	/* TIMER1 INITIALIZATION */
	TCNT1=0;                      //Set timer1 initial count to zero
	TCCR1A = (1<<FOC1A);          //force o/p compare for channel A
	TCCR1B|=(1<<0)|(1<<2)|(1<<3); //CTC bit 3 and clk bit (1024 prescaler)
	TIMSK|=(1<<4);                //OCIE1A bit 4
	OCR1A=1000;                   //interrupt every 1000 msec
	TIFR|=(1<<4);                 //OCF1A bit 4
}

void INT0_init(void){
	MCUCR|=(1<<ISC01) &~(1<<ISC00); //falling edge
}

void INT1_init(void){
	MCUCR|=(1<<ISC11)|(1<<ISC10); //rising edge
}

void INT2_init(void){
	MCUCSR &= ~(1<<ISC2); //falling edge
}

int main(){
port_init();
timer1_init();
INT0_init();    //PD2
INT1_init();    //PD3
INT2_init();    //PB2
GICR|=(1<<INT0)|(1<<INT1)|(1<<INT2); //general interrupt control register enable bits for external interrupts
SREG|=(1<<7);   //global interrupt enable
	while(1){

			PORTA=1;
			PORTC=(PORTC&0xF0) | (sec1&0x0F);
			_delay_ms(5);

		    PORTA=2;
		    PORTC=(PORTC&0xF0) | (sec2&0x0F);
		    _delay_ms(5);

	        PORTA=4;
		    PORTC=(PORTC&0xF0) | (min1&0x0F);
		    _delay_ms(5);

	        PORTA=8;
		    PORTC=(PORTC&0xF0) | (min2&0x0F);
		    _delay_ms(5);

	        PORTA=16;
		    PORTC=(PORTC&0xF0) | (hr1&0x0F);
		    _delay_ms(5);

	        PORTA=32;
		    PORTC=(PORTC&0xF0) | (hr2&0x0F);
		    _delay_ms(5);

	}
}
