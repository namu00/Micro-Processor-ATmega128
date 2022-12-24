#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#define NULL 0
#define DO 523
#define RE 587
#define MI 659
#define FA 700
#define SOL 784
#define RA 880
#define SI 988
#define DDO 1047


#define N2 100
void init_uart1( )
{ // UART1 초기화 함수
	UCSR1B = 0b00011000; // TX/RX 인에이블
	UCSR1C = 0b00000110; // 8비트 데이터, 패리티 없음, 정지 비트 1비트
	UBRR1H = 0; // 데이터율 세팅
	UBRR1L = 8; // 16Mhz, 115200데이터율, 10진수
}

void putchar1(char c)
{ // 문자 전송 함수
	while(!(UCSR1A & (1<<UDRE1))); // (1<<UDRE1)==0x20 (#define UDRE1 5)
	UDR1 = c; // 문자 전송
}
char getchar1( )
{ // 문자 전송 함수
	while (!(UCSR1A & (1<<RXC1))); // (1<<RXC1)==0x80 (#define RXC1 7)
	return(UDR1); // 문자 수신
}
void puts1(char *ptr)
{ // 문자열 송신 함수
	while (*ptr!=NULL) // 문자열의 마지막인지 검사
	putchar1 (*ptr++); // 1문자 송신
}
void buzzer(int hz, int count)
{
	int i, ms, us;
	ms = 1000/(2*hz);
	us = (1000.0/(2*hz) - 1000/(2*hz)) * 1000;
	for(i=0; i < count; i++ )
	{
		PORTE |= 1 << 4;
		_delay_ms(ms);
		_delay_us(us);
		PORTE &= ~(1 << 4);
		_delay_ms(ms);
		_delay_us(us);	
	}
}

int main(){
	char c;
	DDRE = 0x10;
	init_uart1(); // UART1 초기화 함수
	while(1)
	{
		c = getchar1(); // 스마트폰으로 부터 1 문자 수신
		putchar1(c);
		if (c == '1')
		{
			puts1("Do");
			buzzer(DO,DO);
		}
		else if(c == '2')
		{
			puts1("RE");
			buzzer(RE,RE);
		}
		else if(c == '3')
		{
			puts1("MI");
			buzzer(MI,MI);
		}
		else if(c == '4')
		{
			puts1("FA");
			buzzer(FA,FA);
		}
		else if(c == '5')
		{
			puts1("SOL");
			buzzer(SOL,SOL);
		}
		else if(c == '6')
		{
			puts1("RA");
			buzzer(RA,RA);
		}
		else if(c == '7')
		{
			puts1("SI");
			buzzer(SI,SI);
		}
		else if(c == '8')
		{
			puts1("DDO");
			buzzer(DDO,DDO);
		}
		else ;
	}
}