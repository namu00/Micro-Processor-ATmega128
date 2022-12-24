#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define NULL 0x00

#define BIT4_LINE2_DOT58 0x28 // 4 비트 모드, 2줄, 5x8도트
#define DISPON_CUROFF_BLKOFF 0x0C // 디스플레이 ON, 커서 OFF, 블링크 OFF
#define DISPOFF_CUROFF_BLKOFF 0x08
#define DISPCLEAR 0x01 // 디스플레이 클리어
#define INC_NOSHIFT 0x06 // 커서 증가, Delete/Insert
#define CUR1LINE 0x80
#define CUR2LINE 0xC0

#define OFF 10
#define ON 11

int state = OFF;
void CLCD_cmd(char); // 명령어 전송 함수
void CLCD_data(char);
void CLCD_puts(char *);

ISR(INT4_vect)
{
	_delay_ms(60);
	if((PINE & 0x10) == 0x10) return;
	
	EIFR |= 1 <<4;
	if(state == OFF) state = ON;
	else state = OFF;
}
char motto1[] = "Life Companion";
char motto2[] = "JUST Do IT";

int main(void){
	DDRC = 0xff; // PORTC : 명령어/데이터 포트
	DDRD = 0xff; // PORTD : 제어 포트
	// 초기화 시작
	_delay_ms(50); // 전원 인가 후 CLCD 셋업 시간
	CLCD_cmd(BIT4_LINE2_DOT58); // 4 비트 모드, 2줄, 5x8도트
	CLCD_cmd(DISPON_CUROFF_BLKOFF); // 디스플레이 ON, 커서 OFF, 블링크 OFF
	CLCD_cmd(INC_NOSHIFT); // 커서 증가, Delete/Insert
	CLCD_cmd(DISPCLEAR); // 디스플레이 클리어
	_delay_ms(2); // 디스플레이 클리어 실행 시간 동안 대기
	// 초기화 끝
	CLCD_cmd(CUR1LINE);
	CLCD_puts(motto1);
	CLCD_cmd(CUR2LINE);
	CLCD_puts(motto2);
	
	DDRE = 0x00;
	EICRB = 0x0a;
	EIMSK = 0x30;
	SREG |= 0x80;
	
	while(1)
	{
		if(state == ON) CLCD_cmd(DISPON_CUROFF_BLKOFF);
		else CLCD_cmd( DISPOFF_CUROFF_BLKOFF);
	}
	
}

void CLCD_data(char data){
	PORTD = 0x04; // 0b00000100, E(4)=0, R/W(3)=0, RS(2)=1, 데이터 사이클 시작
	_delay_us(1); //Setup Time
	PORTD = 0x14; //0b00010100, E(4)=1, R/W(3)=0, RS(2)=1, 명령어 활성화/Write/데이터 모드
	PORTC = data & 0xf0; // 상위 4비트 전송
	PORTD = 0x04; // 0b00000100, E(4)=0, R/W(3)=0, RS(2)=1, 연속 데이터 사이클(시작+종료)
	_delay_us(2); // 이전 사이클의 Hold Time & 다음 사이클의 Setup Time
	PORTD = 0x14; //0b00010100, E(4)=1, R/W(3)=0, RS(2)=1, 명령어 활성화/Write/데이터 모드
	PORTC = (data << 4) & 0xf0; // 하위 4비트 전송
	PORTD = 0x04; // 0b00000100, E(4)=0, R/W(3)=0, RS(2)=1, 명령어 사이클 종료
	_delay_ms(1);
}

void CLCD_puts(char *ptr){
	while(*ptr != NULL)
		CLCD_data(*ptr++);
}

void CLCD_cmd(char cmd){
	PORTD = 0x00; // 0b00000000, E(4)=0, R/W(3)=0, RS(2)=0, 명령어 사이클 시작
	_delay_us(1); //Setup Time
	PORTD = 0x10; //0b00010000, E(4)=1, R/W(3)=0, RS(2)=0, 명령어 활성화/Write/명령어 모드
	PORTC = cmd & 0xf0; // 상위 4비트 전송
	PORTD = 0x00; // 0b00000000, E(4)=0, R/W(3)=0, RS(2)=0, 연속 명령어 사이클(시작+종료)
	_delay_us(2); // 이전 사이클의 Hold Time & 다음 사이클의 Setup Time
	PORTD = 0x10; // 0b00010000, E(4)=1, R/W(3)=0, RS(2)=0, 명령어 활성화/Write/명령어 모드
	PORTC = (cmd << 4) & 0xf0; // 하위 4비트 전송
	PORTD = 0x00; // 0b00000000, E(4)=0, R/W(3)=0, RS(2)=0, 명령어 사이클 종료
	_delay_ms(1); // Hold Time
}

