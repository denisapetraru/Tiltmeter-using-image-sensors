
#ifndef __uC_Periferic_Initialisation_H
#define __uC_Periferic_Initialisation_H

#ifdef __cplusplus
extern "C" {
	#endif
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//flag seemnalizare
	#define ON_LED1 PORTB &= 0x0FE
	#define OFF_LED1 PORTB |= 0x01
	#define ON_LED2 PORTB &= 0x0FD
	#define OFF_LED2 PORTB |= 0x02
	
//LED_X/Y 
    #define LED_X 0
	#define LED_Y 1
	#define ON_LED_x PORTC |= 0x01 
	#define OFF_LED_x PORTC &= 0xFE  
	#define ON_LED_y PORTC |= 0x02
	#define OFF_LED_y PORTC &= 0xFD	
	
	
	  #define ADC_off  ADCSRA &= (~((1<<ADEN) |(1<<ADSC)))
	  #define Channel_AO_x 0
	  #define Channel_AO_y 3
	  #define Comparator_Enable ACSR &= (~(1<<ACD))  //enable Compar; 
	  #define Comparator_Disable ACSR |= (1<<ACD)    
	  #define Comparator_MuxDisable ADCSRB &= 0xB0 //(~(1<<ACME))
  

 
 //#define Start_masura 0x80;// IS CU SAU LOGIC
// #define Start_Transmisie 0x40;
 
 
 #define RS485_uC_Receptie PORTD |= (1<<PORTD4)
 #define RS485_uC_Transmit PORTD &=(~(1<<PORTD4)) 
 ///////////////////////////////////////// 
void Port_initialisation();
void   AnalogComparator_Initialisation( char channel);
void Timer_1_Initialisation_0_1S(void);
void RX_TX_0_Initialisation(void);
void Initializare_TX_RX_1_RS485(void);
void Timer_0_Initialisation_4mS (void);

//#endif
#ifdef __cplusplus
}
#endif

#endif /* CHEADER_H_INCLUDED */