/*
* LED_Blinking.c
*
* Created: 2022-09-08 오후 02:48:19
* Author :


#include <avr/io.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL

#include <util/delay.h>


int main(void)
{
DDRA = 0xff;
int i = 0x01;

 Replace with your application code 
//while (1)
{
PORTA = 0x0f;
_delay_ms(200);
PORTA = 0xf0;
_delay_ms(200);
i = 0x01;
for(int k = 0; k < 8; k++){
PORTA = i;
_delay_ms(200);
i = i << 1;
}
i = 0x80;
for(int k = 0; k < 8; k++){
PORTA = i;
_delay_ms(200);
i = i >> 1;
}
}
}*/
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL

#include <util/delay.h>

int main(void)
{
	DDRA =0xff;
	while(1)
	{
		PORTA = 0x01;
		for(int i = 0; i < 8; i++){
			PORTA = i << 1;
			_delay_ms(1000);
		}
	}
}