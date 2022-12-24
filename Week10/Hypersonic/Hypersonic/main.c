#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


#define SV 340UL
#define TRIG 6
#define ECHO 7
#define INT4 4
#define BIT4_LINE2_DOT58 0x28
#define DISPON_CUROFF_BLKOFF 0x0C
#define DISPOFF_CUROFF_BLKOFF 0x08
#define INC_NOSHIFT 0x06
#define DISPCLEAR 0x01
#define CUR1LINE 0x80
#define CUR2LINE 0xC0
#define NULL 0x00

#define Go 100
#define Stop 110

void init_CLCD();
void init_interrupt();
void CLCD_cmd(char);
void CLCD_data(char);
void CLCD_puts(char*);
void CLCD_num_display(int);
int read_distance();

int state = Go;
char Distance[] = "Distance: ";
char CLCD_NUM[] = "000.0";

int rec = 0;
int main(void)
{
	DDRA = ((DDRA | (1 <<TRIG)) & ~(1 << ECHO));
	DDRC = 0xff;
	DDRD = 0xff;
	DDRE = 0x00;
	init_interrupt();
	init_CLCD();
	while(1)
	{
		CLCD_cmd(DISPON_CUROFF_BLKOFF);
		CLCD_cmd(CUR1LINE);
		if (state == Go)
		{
			_delay_ms(100);
			rec = read_distance();
		}
		CLCD_puts(Distance);
		CLCD_num_display(rec);
	}
}

void init_interrupt()
{
	EICRB = 0x02;
	EIMSK = 0x10;
	SREG |= 0x80;
	sei();
}

void init_CLCD()
{
	_delay_ms(50);
	PORTC = 0x00;
	CLCD_cmd(BIT4_LINE2_DOT58);
	CLCD_cmd(DISPON_CUROFF_BLKOFF);
	CLCD_cmd(INC_NOSHIFT);
	CLCD_cmd(DISPCLEAR);
	_delay_ms(2);
}

ISR(INT4_vect)
{
	_delay_ms(50);
	if ((PINE &= 0x10) != 0x00) return;
	EIFR |=  1 << 4;

	if (state == Go) state = Stop;
	else state = Go;
}


int read_distance()
{
	unsigned int distance = 0;
	
	TCCR1B = 0x03;
	PORTA &= ~(1<<TRIG); _delay_us(10);
	PORTA |= (1<<TRIG); _delay_us(10);
	PORTA &= ~(1<<TRIG);

	while(!(PINA & (1<<ECHO)));
	TCNT1 = 0x0000;
	while (PINA & (1<<ECHO));
	TCCR1B = 0x00;
	distance = (unsigned int)(SV * (TCNT1 * 4 / 2) / 1000);
	return(distance);
}

void CLCD_puts(char *ptr)
{
	while(*ptr != NULL) CLCD_data(*ptr++);
}

void CLCD_num_display(int num)
{
	CLCD_NUM[0] = (num/1000)%10 + 0x30;
	CLCD_NUM[1] = (num/100)%10 + 0x30;
	CLCD_NUM[2] = (num/10)%10 + 0x30;
	CLCD_NUM[3] = '.';
	CLCD_NUM[4] = (num/1)%10 + 0x30;
	CLCD_NUM[5] = NULL;
	CLCD_puts(CLCD_NUM);
}

void CLCD_data(char data)
{
	PORTD = 0x04;
	_delay_us(1);
	PORTD = 0x14;
	PORTC = data & 0xf0;
	PORTD = 0x04;
	_delay_us(2);
	PORTD = 0x14;
	PORTC = (data << 4) & 0xf0;
	PORTD = 0x04;
	_delay_ms(1);
}

void CLCD_cmd(char cmd)
{
	PORTD = 0x00;
	_delay_us(1);
	PORTD = 0x10;
	PORTC = cmd & 0xf0;
	PORTD = 0x00;
	_delay_us(2);
	PORTD = 0x10;
	PORTC = (cmd << 4) & 0xf0;
	PORTD = 0x00;
	_delay_ms(1);
}