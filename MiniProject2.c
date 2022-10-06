

#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

unsigned char sec_flag=0;
unsigned char seconds_1=0;
unsigned char seconds_2=0;
unsigned char minutes_2=0;
unsigned char minutes_1=0;
unsigned char hours_1 =0;
unsigned char hours_2 =0;


void Timer1_init(){
TCCR1A=(1<<FOC1A)|(1<<FOC1B);
TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM12);
TCNT1=0;
OCR1A=1000;
TIMSK|=(1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect){

	sec_flag=1;

}

void INT0_init(){
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);
	MCUCR&=(~(1<<ISC00));
	DDRD&=(~(1<<PD2));
	PORTD|=(1<<PD2);

}

void INT1_init(){
	DDRD&=(~(1<<PD3));
	MCUCR|=(1<<ISC10)|(1<<ISC11);
	GICR|=(1<<INT1);
}


void INT2_init(){
	GICR|=(1<<INT2);
	MCUCSR &= !(1<<ISC2);
	DDRB&=(~(1<<PB2));
	PORTB|=(1<<PB2);

}

ISR(INT0_vect){
	seconds_1=0;
	seconds_2=0;
	minutes_1=0;
	minutes_2=0;
	hours_1=0;
	hours_2=0;
}

ISR(INT1_vect){
	TCCR1B &= (~(1<<CS10)) & (~(1<<CS11))&(~(1<<CS12));

}
ISR(INT2_vect){

	TCCR1B|=(1<<CS12)|(1<<CS10);
}


int main(void){

	DDRC|=0x0F;
	PORTC&=0xF0;
	DDRA|=0x3F;
	PORTA|=0x3F;
	Timer1_init();
	INT0_init();
	INT1_init();
	INT2_init();
	SREG|=(1<<7);


	while(1){
		if(sec_flag==1){
			seconds_1++;
			if(seconds_1==10){
				seconds_2++;
				seconds_1=0;
			}
			if(seconds_2==6){
				minutes_1++;
				seconds_1=0;
				seconds_2=0;
			}
			if(minutes_1==10){
				minutes_2++;
				minutes_1=0;
			}
			if(minutes_2==6){
				hours_1++;
				minutes_1=0;
				minutes_2=0;
			}
			if(hours_2==10){
				hours_2=0;
				hours_1=0;
				minutes_1=0;
				minutes_2=0;
				seconds_1=0;
				seconds_2=0;
			}
			sec_flag=0;

		}
		else{

			PORTA =(PORTA & 0xC0)|(0x01);
			PORTC =(PORTC & 0xF0)|(seconds_1);

			_delay_ms(2);

			PORTA =(PORTA & 0xC0)|(0x02);
			PORTC =(PORTC & 0xF0)|(seconds_2);

			_delay_ms(2);

			PORTA =(PORTA & 0xC0)|(0x04);
			PORTC =(PORTC & 0xF0) | (minutes_1);

			_delay_ms(2);

			PORTA =(PORTA & 0xC0)|(0x08);
			PORTC =(PORTC & 0xF0)|(minutes_2);

			_delay_ms(2);

			PORTA =(PORTA & 0xC0)|(0x10);
			PORTC =(PORTC & 0xF0)|(hours_1);

			_delay_ms(2);

			PORTA =(PORTA & 0xC0)|(0x20);
			PORTC =(PORTC & 0xF0)|(hours_2);

		    _delay_ms(2);


		}

	}

}
