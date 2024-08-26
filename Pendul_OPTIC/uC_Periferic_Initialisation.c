
#include <avr/io.h>
#include "uC_Periferic_Initialisation.h"

void Port_initialisation(){
  
  DDRB |=((1<<1)|(1<<0));//led 0, 1  flag_LED
  PORTB |=((1<<1)|(1<<0));;//led 0, 1 flag_LED
  
  DDRD |=(1<< PORTD4);    //Enable RS485
  RS485_uC_Receptie;    //Enable RS485
}


 void AnalogComparator_Initialisation( char channel){
	 
	 Comparator_Enable;//ACSR &= (1<<ACD);  //enable Compar; 
	 //ACSR |= (1<<ACBG); // AIN0=referinta 1.1v AIN1 for sensor  
	 ADC_off; //ADC off
	 ADMUX = channel; 	 
	 ADCSRB |=(1<< ACME);
	 _delay_ms(5); 	 
	 ;}
	 
 void Timer_1_Initialisation_0_1S(void){
	 TCCR1A = 0;
	 TCCR1B = ((1<<WGM12)|(1<<CS12)|(0<<CS11)|(0<<CS10));  //CTC foscil/256;
	 OCR1A = 31250;  
	 TIMSK1 |= (1<<OCIE1A);
 }
 
 
 void RX_TX_0_Initialisation(void){
 	UCSR0B  = 0b10011000;
 	UBRR0H = 0;    //
 	UBRR0L = 103;  //9600biti/sec
 }
 
 void Timer_0_Initialisation_4mS (void){

	 TCCR0B = (1 << CS02);// 0x05;  //0.01636,//   /
	 TCNT0=0x00;
	 TIMSK0 |= (1<<TOIE0);
 }
 
 void Initializare_TX_RX_1_RS485 (void){
	 //transmisie/receptie 1200biti pe secunda
	 UCSR1A = 0x00;
	 //UCSRB = 0b10011000;
	 UCSR1B=0x98;
	 UCSR1C=0x86;
	 UBRR1H=0x03;
	 UBRR1L=0x40;
 }
