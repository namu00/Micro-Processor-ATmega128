#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main()
{
	int H = 0x76;
	int E = 0x79;
	int L = 0x38;
	int P = 0x73;
	
	DDRC = 0xff;
	DDRG = 0x0f;
	while(1){
		PORTC = H; PORTG = 0x08; _delay_ms(5);
		PORTC = E; PORTG = 0x04; _delay_ms(5);
		PORTC = L; PORTG = 0x02; _delay_ms(5);
		PORTC = P; PORTG = 0x01; _delay_ms(5);
	}
}