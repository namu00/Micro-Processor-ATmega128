/*
 * ADC.c
 *
 * Created: 2022-10-12 오후 11:28:49
 * Author : skagn
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define CDS_10 341
#define CDS_100 731

#define ON 0xff;
#define OFF 0x00;

unsigned state;
void init_adc();
unsigned short read_adc();
void show_adc_led(unsigned short data);

int main(void)
{
    unsigned short value;
	DDRA = 0xff;
	DDRF = 0x00;
	init_adc();
	while (1) 
    {
		value = read_adc();
		show_adc_led(value);
    }
}

void init_adc()
{
	ADMUX = 0x40;
	ADCSRA = 0x87;
}

unsigned short read_adc()
{
	unsigned char adc_low, adc_high;
	unsigned short value;
	ADCSRA |= 0x40;
	while((ADCSRA & 0x10) != 0x10);
	adc_low = ADCL;
	adc_high = ADCH;
	value = (adc_high << 8) | adc_low;
	return value;
}

void show_adc_led(unsigned short value)
{
	if(value <= CDS_10){ state = ON ; }
	else if(value >= CDS_100){ state = OFF; }
	else { state = state; }
	
	PORTA = state;
}