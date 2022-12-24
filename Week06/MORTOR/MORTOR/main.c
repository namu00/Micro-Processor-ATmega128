#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define MOTOR_CW 0xb0
#define MOTOR_CCW 0x70
#define MOTOR_BRAKE 0xd0
#define MOTOR_STOP 0x30

int state = MOTOR_STOP;

ISR(INT4_vect) //FORWARD
{
	_delay_ms(50);
	if((PINE & 0x10) == 0x10) return;
	EIFR |= 1<<4;
	state = MOTOR_CW;
}

ISR(INT5_vect) //REVERSE
{
	_delay_ms(50);
	if((PINE & 0x20) == 0x20) return;
	EIFR |= 1<<5;
	state = MOTOR_CCW;
}

int main(void)
{
	DDRB = 0xf0; //motor 1
	DDRE = 0x00; //Switch
	DDRG = 0x03; //LED
	EICRB = 0x0a;
	EIMSK = 0x30;
	sei();
	while(1)
	{	
		if(state == MOTOR_CW) PORTG = 0x01; //정방향 LED 점등
		else if(state == MOTOR_CCW) PORTG = 0x02; //역방향 LED 점등
		else PORTC = 0x00;
		
		PORTB = state;	
	}	
}