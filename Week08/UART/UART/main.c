#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void init_uart0()
{
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0; UBRR0L = 103;
}

void putchar0(char c) {
	while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = c;
}

char getchar0(){
	while(!(UCSR0A & (1 <<RXC0)));
	return(UDR0);
}

int main()
{
	char value;
	init_uart0();
	while(1){
		putchar0('>');
		putchar0('>');
		value = getchar0();
		putchar0(value);
		putchar0('\r');
		putchar0('\n');
		
		if((value >= '1') && (value <= '9'))
		{
			for(int i = 1; i < 10; i++)
			{
				int ans[2] = {0, 0};
				int res = (value - 48) * i;
				ans[1] = (res/10) + 48;
				ans[0] = (res%10) + 48;
				
				putchar0(value);
				putchar0(' ');
				putchar0('x');
				putchar0(' ');
				putchar0((i+48)); //Number -> Ascii Code
				putchar0(' ');
				putchar0('=');
				putchar0(' ');
				putchar0(ans[1]);
				putchar0(ans[0]);
				putchar0('\r');
				putchar0('\n');
			}
			putchar0('\n');
		}
		else if(value == '\r') putchar0('\n');
		else putchar0(value);
		putchar0('\n');
	}
}