#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define RA 5
#define SI 6
#define DDO 7
#define REST 8
#define EOS -1

#define ON 0
#define OFF 1

#define N2 1250
#define N4  (N2/2)
#define N8 (N2/4)
#define N16 (N2/8)
#define N8N16 (N8+N16)
#define R 1

volatile int state, tone;
char f_table[] = {17, 43, 66, 77, 97, 114, 129, 137, 255};
int song[] = {SOL, MI, REST, MI, SOL, MI, DO, RE, MI, RE, DO, MI, SOL, DDO, SOL, DDO, SOL, DDO, SOL, MI, SOL, RE, FA, MI, RE, DO, EOS};
int time[] = {N4, N8, R, N8, N8, N8, N4, N4, N8, N8, N8, N8, N4, N8N16, N16, N8, N8, N8, N8, N4, N4, N8, N8, N8, N8, N4, N4};
char LED[] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0x00};
	
ISR (TIMER0_OVF_vect)
{
	TCNT0 = f_table[tone];
	if (state == OFF)
	{
		PORTB |= 1 << 4;
		state = ON;
	}	
	else
	{
		PORTB &= ~(1<<4);
		state = OFF;
	}
}

int main(void)
{
	int i = 0;
	DDRC = 0xff;
	DDRB |= 0x1;
	TCCR0 = 0x03;
	TIMSK = 0x01;
	TCNT0 = f_table[song[i]];
	sei();
	
	while(1)
	{
		i = 0;
		do{
			tone = song[i];
			if(tone == REST) PORTC = LED[song[i-1]]; //REST일 때, 이전 음계 정보 Display
			else PORTC = LED[tone];					 //현제 음계 정보 Display
			_delay_ms(time[i++]);
		}while(song[i]!=EOS); //EOS == End Of Song
	}
}