#define F_CPU 16000000UL
#define F_SCK 40000UL
#include <avr/io.h>
#include <util/delay.h>
#define LM75A_ADDR 0x90
#define LM75A_TEMP_REG 0

void init_twi_port();
int read_twi_2byte_nopreset(char reg);
void display_FND(int value);
void tone_buzzer(void);

int buzzer_onoff = 0;
int main(){
	int i, temperature;
	init_twi_port();
	while (1){
		for (i=0; i<30; i++){
			if (i == 0) temperature = read_twi_2byte_nopreset(LM75A_TEMP_REG);
			
			display_FND(temperature);
		}
	}
}

void init_twi_port(){
	DDRC = 0xff;
	DDRG = 0xff;
	TWSR = TWSR & 0xfc;
	TWBR = (F_CPU/F_SCK - 16) / 2;
}

int read_twi_2byte_nopreset(char reg){
	char high_byte, low_byte;
	TWCR = (1 << TWINT) | (1<<TWSTA) | (1<<TWEN);
	while (((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x08) ;
	
	
	TWDR = LM75A_ADDR | 0;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x18) ;
	
	TWDR = reg;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x28) ;
	
	TWCR = (1 << TWINT) | (1<<TWSTA) | (1<<TWEN);
	while (((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x10) ;
	
	TWDR = LM75A_ADDR | 1;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x40) ;
	
	TWCR = (1 << TWINT) | (1 << TWEN | 1 << TWEA);
	while(((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x50) ;
	
	high_byte = TWDR;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(((TWCR & (1 << TWINT)) == 0x00) || (TWSR & 0xf8) != 0x58) ;
	
	low_byte = TWDR;
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while ((TWCR & (1 << TWSTO))) ;
	return((high_byte<<8) | low_byte);
}

void display_FND(int value){
	char digit[12] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67, 0x00, 0x40};
	char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
	int value_int, value_deci, num[4], i;
	if ((value & 0x8000) != 0x8000)
	num[3] = 10;
	else{
		num[3] = 11;
		value = (~value)+1;
	}
	
	value_int = (value & 0x7F00) >> 8;
	
	value_deci = (value & 0x0080);
	
	num[2] = (value_int / 10) % 10;
	num[1] = value_int % 10;
	num[0] = (value_deci == 0x80) ? 5 : 0;
	
	if(value_int >= 30 || value_int < 20) buzzer_onoff = !buzzer_onoff;
	tone_buzzer();
	for(i=0; i<4; i++){
		PORTC = digit[num[i]]; PORTG = fnd_sel[i];
		if (i==1) PORTC |= 0x80;
		if (i%2) _delay_ms(2);
		else _delay_ms(3);
	}
}

void tone_buzzer(void)
{
	DDRB |= 0x10;
	if (buzzer_onoff)
	{
		PORTB |= 1 << 4;
		_delay_us(200);
	}
	
	else PORTB &= ~(1 << 4);
}