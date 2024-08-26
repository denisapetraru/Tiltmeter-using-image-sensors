/*
 * Pendul_OPTIC.cpp
 *
 * Created: 02.01.2023 12:33:04
 * Author : POP Septimiu
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


#include "UsedVariable.h"
#include "uC_Periferic_Initialisation.h"
#include "Functions.h"
#include "TSL1412S.h"
#include <avr/wdt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
unsigned char CharData =0;
unsigned char PixelsValue_X[]="";
unsigned char PixelsValue_Y[]="";

uint8_t Actiune = 0;
	

//////////////
int main(void)
{
    /* Replace with your application code */
	 
	 Timer_1_Initialisation_0_1S();  //thred citire sensor	 
	 
	 Port_initialisation();         //initializare port
	 RX_TX_0_Initialisation();      //initializare RX0	
	 
	 Timer_0_Initialisation_4mS();   //thred determinare pachete intirziate 
	 Initializare_TX_RX_1_RS485();        //initializare RX1 RS485
	 
	
		//watchdog timer is a simple countdown
		cli();
		wdt_reset(); 
		WDTCSR |= (1<<WDCE) | (1<<WDE);        WDTCSR = ((1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0));  //0.5s
		///////// end watchdog///////
		Semnalizare_Pornire();
		RS485_uC_Receptie;
		Index_Receptie_PC = 0;
		Index_Receptie_PC_OLD=0;
		sei();		
	//wdt_reset();	
    while (1) 
    {
		
    }//end while
} //end main
//ISR Rx
ISR(USART0_RX_vect) {
	
	//afisez 99999 = 999.99*100;  folosesc 2 zecimale
	//intreruperea de receptie seiriala
	uint16_t data=0;
	uint8_t Local_vr=0;
	ON_LED2;// testez rutina rx
	
	data = UDR0; //citesc comanda receptionata
	
	switch(data)
	{
		case 'x': //rezultat ADC
		Send_Value('x', WirePosition.N_X*6.35);		
		break;
		case 'e'://Volt medie
		Send_Value('e', WirePosition.error_X);
		break;
		case 'm':// memoria x
			for (data=0;data <MaXLenghtPixel; data++)
			{
				Local_vr = UCSR0A &  0x20;
				while(Local_vr ==0) Local_vr = UCSR0A & 0x20;
				UCSR0A |= 0x20;
				UDR0 = (PixelsValue_X[data]+48); //conversie din hexa in ascii
			}		
		break;		
		case 'n':// memoria y
			for (data=0;data <MaXLenghtPixel; data++)
			{
				Local_vr = UCSR0A &  0x20;
				while(Local_vr ==0) Local_vr = UCSR0A & 0x20;
				UCSR0A |= 0x20;
				UDR0 = (PixelsValue_Y[data]+48);
			}
		//break;
	}
	
	OFF_LED2;
}
/////////// end  USART0_RX_vect  //// 

// interrupt of 0.1 sec
ISR(TIMER1_COMPA_vect)
{
	volatile static uint16_t timerCounter=0;
	wdt_reset();
	timerCounter ++;
	if (timerCounter == 20)  //10
	{	//timerCounter=0;
		ON_LED1;//
		//wdt_reset();
			Captura_AxaX('x');
		OFF_LED1;
	}//end 
	if (timerCounter >= 40)  //20
	{	ON_LED2;//
		//wdt_reset();
		timerCounter=0;
			Captura_AxaY('y');
		OFF_LED2;
	}//end	
}//end isr
/////////////////////

ISR(TIMER0_OVF_vect)
{
	uint16_t static Counter_1sec=0;
	wdt_reset();
	Counter_1sec++;
	
				if (Counter_1sec == 489) 
				{
					//Trimite_Valoare('x',WirePosition.mm_X );//WirePosition.mm_X  denisa
				}
				if (Counter_1sec >= 976)
				{ Counter_1sec =0;
					//Trimite_Valoare('y',WirePosition.mm_Y);//WirePosition.mm_X  denisa
				}
	
	 
	//verific pachete intirziate
	VerificPachete_Intirziate();
	
	//calculez X/Y medie
	//raspund la comanda de pe rs485
			switch(Actiune)
			{
				case 'x':
					MedieDate('x');
					Actiune=0;
				break;
		
				case 'y':
					MedieDate('y');
					Actiune=0;
				break;
				case 'X':
					WirePosition.X_mm_mediu = WirePosition.mm_X;  //ultima valoare
					WirePosition.errorXmediu = WirePosition.error_Xmm;  //daca sunt peste 16 instabil  //valoare instant X
					Actiune=0;
				break;				
				case 'Y':
					WirePosition.Y_mm_mediu = WirePosition.mm_Y;  //valoare instant Y
					WirePosition.errorYmediu = WirePosition.error_Ymm;  //valoare instant Y
					Actiune=0;
				break;				
				default:
				Actiune = 0;
			}
	
	
}

ISR(USART1_RX_vect)   //receptie RS485
{
	 volatile uint8_t Data_RX = 0;
     
	  Data_RX = UCSR0A;
      if((Data_RX & ((1<<DOR1) |(1<<FE1) |(1<UPE1))) == 0) //start1//am receptie un caracter
      { 
		Data_RX = UDR1;
		
		Index_Receptie_PC++;
		UDR0 = Index_Receptie_PC;
		
		
		//if (Index_Receptie_PC <= 5) Index_Receptie_PC++;
		//else Index_Receptie_PC=1;	 


			if((Index_Receptie_PC == 1) && (Data_RX != Antet_Receptie_uC))  Index_Receptie_PC = 6;
			
			if (Index_Receptie_PC <= 5)   //start2  //trebuie sa tratez daca nu vin intr-un interval de timp !!!!!
			{   
				if (Index_Receptie_PC == 1) BuffRx_PC.Antet = Data_RX; 
			   
				if((Index_Receptie_PC == 2) && (Data_RX != Adr_uC)) {
					Index_Receptie_PC = 6; 
				}
	           if (Index_Receptie_PC == 2) BuffRx_PC.Adres_uC = Data_RX; 
			   
				if((Index_Receptie_PC >= 3) && (Index_Receptie_PC <=5))
				{// start3
										
										
					if (Index_Receptie_PC == 3)  BuffRx_PC.Cmd_PC = Data_RX;
					if (Index_Receptie_PC == 4)  BuffRx_PC.Adr_Trad = Data_RX;
					if (Index_Receptie_PC == 5)
					{
						BuffRx_PC.Reg_SC = Antet_Receptie_uC + Adr_uC + BuffRx_PC.Cmd_PC + BuffRx_PC.Adr_Trad + Data_RX;
						Index_Receptie_PC = 0;
						  
							if (BuffRx_PC.Reg_SC == 0) {
								
								if (( BuffRx_PC.Cmd_PC == CitesteVal_uC_x ) && (BuffRx_PC.Adr_Trad == AXA_x )) Actiune ='x';
								if (( BuffRx_PC.Cmd_PC == CitesteVal_uC_y ) && (BuffRx_PC.Adr_Trad == AXA_y )) Actiune ='y';
								
								if (( BuffRx_PC.Cmd_PC == Citire_Instant_x ) && (BuffRx_PC.Adr_Trad == AXA_x )) Actiune ='X';
								if (( BuffRx_PC.Cmd_PC == Citire_Instant_y ) && (BuffRx_PC.Adr_Trad == AXA_y )) Actiune ='Y';								
								
								Transmission_function(BuffRx_PC.Cmd_PC);
								

								}

					}
				} //end start3
			 }//end start2
		} //end start1
	//else Index_Receptie_PC = 0;
	
	//if (Index_Receptie_PC > 5) Index_Receptie_PC = 0;//deocamdata

}