#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IDLE 0
#define STOP 1
#define GO 2

volatile int state = IDLE;
volatile unsigned int count=9999;
unsigned int number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};
unsigned int fnd_sel[4] = {0x01, 0x02, 0x04,0x08};


ISR(INT4_vect) //STOP OR GO
{
	_delay_ms(50);
	if((PINE & 0x10) == 0x10)
	return;
	
	EIFR |= 1 << 4;
	if(state == IDLE || state == STOP) state = GO;
	else{
		state = STOP;
	}
}

ISR(INT5_vect) //PLUSE 1000
{
	_delay_ms(50);
	if((PINE & 0x20) == 0x20) return;
	
	EIFR |= 1 << 5;
	if(count + 1000 > 9999) count = 9999;
	else count += 1000;
}

void display_fnd(int count){ // 함수 전체 실행시간은 약 0.01초
	int i, k, fnd[4]; // 배열 선언
	fnd[3] = (count/1000)%10; // 1000 자리
	fnd[2] = (count/100)%10; // 100 자리
	fnd[1] = (count/10)%10; // 10 자리
	fnd[0] = count%10; // 1 자리
	for (k = 0; k<10; k ++){
		for (i=0; i<4; i++) {
			PORTC = number[fnd[i]];
			PORTG = fnd_sel[i];
			if (i%2) _delay_ms(2); // 2ms 지연
			else _delay_ms(3); // 3ms 지연, 총 10ms 지연
		}
	}
}

int main(void)
{
	DDRC=0xff; //FND
	DDRG=0x0f; //COM OUT
	DDRE = 0x00; //SWITCH
	sei();
	EICRB = 0x0a;
	EIMSK = 0x30;
	
	int n4,n3,n2,n1;
	
	while (1)
	{
		if(state == IDLE || state == GO)
		{
			display_fnd(count);
			count--;
		}
		else if(count == 0 & (state == IDLE || state == GO ))
		{
			count = 0;
		}
		else
		{
			n4 = (count/1000)%10; // 1000 자리
			n3 = (count/100)%10; // 100 자리
			n2 = (count/10)%10; // 10 자리
			n1 = count%10; // 1 자리
			
			PORTC = number[n4]; PORTG = 0x08; _delay_ms(5);
			PORTC = number[n3]; PORTG = 0x04; _delay_ms(5);
			PORTC = number[n2]; PORTG = 0x02; _delay_ms(5);
			PORTC = number[n1]; PORTG = 0x01; _delay_ms(5);
		}
	}
}