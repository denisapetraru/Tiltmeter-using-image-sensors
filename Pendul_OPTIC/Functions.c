#include "Functions.h"
#include "UsedVariable.h"
#include "Functions.h"
#include "TSL1412S.h"
#include "uC_Periferic_Initialisation.h"
#include "stdlib.h"
#include <avr/wdt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char display_Chars_12[12]={'x','=','2', '5', '6' ,',', '9','9','D','C', 0x0D, 0x0A};
		
	 uint8_t Index_Receptie_PC =0;
	 uint8_t Index_Receptie_PC_OLD =0 ;
	 

	 
//input parameter 
//unsigned char cmd  comanda de intrare x sau y , 
//unsigned long int valoare valoarea de trimis pe UDR0
//out void      cmd=|___|oEcrLf  
//terminal apare x=921.230E  ASCII string  10+crlf

void Send_Value (unsigned char cmd, unsigned long int valoare)
{
	volatile unsigned char Local_vr = 0, Local_vr1 = 0, *point_display;
	volatile unsigned long int Diplay_Val=valoare;
	
		for (Local_vr=0;Local_vr <12; Local_vr++)  display_Chars_12[Local_vr] =48;
		
	
	if ((cmd =='x') ||(cmd =='y'))
	{		
		display_Chars_12[0] =cmd;
		display_Chars_12[1] ='=';
		display_Chars_12[5] =0x2E;  //','
				
				if (cmd =='x'){
					 Diplay_Val = WirePosition.mm_X;
					 Local_vr = (WirePosition.error_Xmm);//error x  error_Xmm
				 }
				if (cmd =='y'){
					Diplay_Val = WirePosition.mm_Y;
					Local_vr = (WirePosition.error_Ymm);
				}
		point_display =((display_Chars_12) + 8);
        point_display = itoa(Local_vr, point_display, 16);
		if ((Local_vr & 0xF0)==0)  display_Chars_12[9] = 48;
		
		display_Chars_12[10] =0x0A;
		display_Chars_12[11] =0x0D;		
		
		
				//Diplay_Val = valoare;
				for (Local_vr=7;Local_vr > 1;Local_vr--)
				{
					if (Diplay_Val !=0)
					{
						if (Local_vr!=5)//poz lui ','
						{
							display_Chars_12[Local_vr] = (Diplay_Val%10)+48;
							Diplay_Val = Diplay_Val/10;
						}
					}
					else break;			
				}

				for (Local_vr1=0;Local_vr1 <12; Local_vr1++)
				{
			
					Local_vr = UCSR0A &  0x20;
					while(Local_vr ==0) Local_vr = UCSR0A & 0x20;
					UCSR0A |= 0x20;
					UDR0 = display_Chars_12[Local_vr1];
				}
	}
	else  // afisez datele din memorie
	{
		for (Local_vr1=0;Local_vr1<80;Local_vr1++)
		{
			//Local_vr = UCSR0A &  0x20;
			//while(Local_vr ==0) Local_vr = UCSR0A & 0x20;
			//UCSR0A |= 0x20;
			//UDR0 = (INEL_DATE[data_int])>>8;
			//
			//Local_vr = UCSR0A &  0x20;
			//while(Local_vr ==0) Local_vr = UCSR0A & 0x20;
			//UCSR0A |= 0x20;
			//UDR0 = INEL_DATE[data_int];
		}
	}
}
/////////////////////


//////////////////////////////////////////////////////////////////

unsigned char Transmission_function(unsigned char CMD)
{
    volatile uint16_t LocalData=0;
	uint8_t  *PointerSTR  ;
	
	//trec pe transmisie uC -> PC
	/////////////////////*
	
	RS485_uC_Transmit;
	_delay_ms(50);//delay_ms(100);
	//////////////////////

	BuffRx_PC.Antet = Antet_Transmisie_uC_To_PC; //uC
	BuffRx_PC.Adres_uC = Adr_uC; //uC
	BuffRx_PC.Cmd_PC = CMD; //uC  Stare_uC
	//BuffRx_PC.Adr_Trad = Reg_Adr_Trad; //uC  Stare_uC

	Index_Receptie_PC = 0;


		if(( CMD == CitesteVal_uC_x ) && (BuffRx_PC.Adr_Trad ==AXA_x))  //SelX   0A
		{
			LocalData = WirePosition.mm_X;
			BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
			LocalData = LocalData >> 8;
			BuffRx_PC.ValoareL = LocalData; //Temp High
			BuffRx_PC.OctetE = WirePosition.error_Xmm; //	
	  }
		if(( CMD == CitesteVal_uC_y ) && (BuffRx_PC.Adr_Trad ==AXA_y))  //SelY  0A
		{
			LocalData = WirePosition.mm_Y;
			BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
			LocalData = LocalData >> 8;
			BuffRx_PC.ValoareL = LocalData; //Temp High
			BuffRx_PC.OctetE = WirePosition.error_Ymm; //
		}
	////////////XXXXX comanda instant
			if(( CMD == Citire_Instant_x ) && (BuffRx_PC.Adr_Trad ==AXA_x))  //SelX   0A
			{
				LocalData = WirePosition.mm_X;
				BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
				LocalData = LocalData >> 8;
				BuffRx_PC.ValoareL = LocalData; //Temp High
				BuffRx_PC.OctetE = WirePosition.error_Xmm; //
			}
			if(( CMD == Citire_Instant_y ) && (BuffRx_PC.Adr_Trad ==AXA_y))  //SelY  0A
			{
				LocalData = WirePosition.mm_Y;
				BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
				LocalData = LocalData >> 8;
				BuffRx_PC.ValoareL = LocalData; //Temp High
				BuffRx_PC.OctetE = WirePosition.error_Ymm; //
			} 
	
	///////////////////////////////////////	
		if(( CMD == CitesteMasTrad_uC) && ( BuffRx_PC.Adr_Trad ==AXA_x ) )  //citire X
		{
			LocalData = WirePosition.X_mm_mediu;
			BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
			LocalData = LocalData >> 8;
			BuffRx_PC.ValoareL = LocalData; //Temp High
			BuffRx_PC.OctetE = WirePosition.errorXmediu; //
		}
		if(( CMD == CitesteMasTrad_uC) && ( BuffRx_PC.Adr_Trad ==AXA_y ) )  //citire Y
		{
			LocalData = WirePosition.Y_mm_mediu;
			BuffRx_PC.ValoareH = (LocalData & 0xff); //Temp LOW
			LocalData = LocalData >> 8;
			BuffRx_PC.ValoareL = LocalData; //Temp High
			BuffRx_PC.OctetE = WirePosition.errorYmediu; //
		}		
	
	if( CMD == RESET_uC)
	{
		//Actiune = Actiune | Start_masura;  //urmeaza peste 16ms masuratoarea
	}
   
    LocalData = ((BuffRx_PC.Antet + BuffRx_PC.Adres_uC + BuffRx_PC.Cmd_PC + BuffRx_PC.Adr_Trad + BuffRx_PC.ValoareH + BuffRx_PC.ValoareL + BuffRx_PC.OctetE) & 0xFF);
	BuffRx_PC.Reg_SC = (~LocalData) +1;
	
	//transmisie_PC efectiv
	ON_LED1;
	PointerSTR =  &BuffRx_PC.Antet; 
	for ( Index_Receptie_PC = 0; Index_Receptie_PC < 8; Index_Receptie_PC ++)
	{  
					LocalData = UCSR1A &  0x20;
					while(LocalData ==0) LocalData = UCSR1A & 0x20;
					UCSR1A |= 0x20;
		UDR1 = *PointerSTR ;
		PointerSTR++;
	}
						LocalData = UCSR1A &  (1<< TXC1);
						while(LocalData ==0) LocalData = UCSR1A &  (1<< TXC1);
						UCSR1A |= 0x20;
	
	Index_Receptie_PC = 0;
	_delay_ms(10);//obtional
	RS485_uC_Receptie;
    OFF_LED1;
	return 0;
}
//////////////////////////////////////////////////////////////////

 void VerificPachete_Intirziate(void){
	uint16_t static Contor_local = 0 ;   //se incrementeaza la 4ms
	 
            if (Index_Receptie_PC_OLD != Index_Receptie_PC)
            {
	            Index_Receptie_PC_OLD = Index_Receptie_PC;
	            Contor_local = 0;
            }
            //if ((Index_Receptie_PC_OLD == Index_Receptie_PC))
            else
			{
						Contor_local ++;
						if (Contor_local >= 10) //40ms x 20  163ms 325ms  //9  DACA SUNT INTRERUPERI IN RECEPTIE  (cond 2 == -> >=)
						{
							Index_Receptie_PC =0;
							Index_Receptie_PC_OLD =0;
							Contor_local = 0;
							RS485_uC_Receptie; // am completat in 2020 //obs la oasa se bloca la datele de la alt uC  (cond 2)
							wdt_reset();
						}
             }

}

// delay la start bling led de 4 ori marcheaza pornirea aplicatiei
void Semnalizare_Pornire(void)
{ uint8_t Repet_blink=0;
	//
	for(Repet_blink=0;Repet_blink < 4; Repet_blink++)
	{    wdt_reset();
		_delay_ms(300);//delay_ms(500);
		ON_LED1;
		 wdt_reset();
		_delay_ms(300);//delay_ms(500);
		OFF_LED1;	
	}
}
//////////////////////////////////////////

void SalvezDate( uint8_t AXA){
	volatile static uint8_t ContorX=0, ContorY=0;
	
	switch (AXA)
	{
		case 'x':
				//if (WirePosition.error_Xmm ==0)
				//{
					FirX.PozitiePixel[ContorX] = WirePosition.mm_X;
					FirX.PozitieError[ContorX] = WirePosition.error_Xmm;
					ContorX++;
					if (ContorX >= SaveDataLenght) ContorX =0;
				//}
		
		break;
		
		case 'y':
				//if (WirePosition.error_Ymm ==0)
				//{
					FirY.PozitiePixel[ContorY] = WirePosition.mm_Y;
					FirY.PozitieError[ContorY] = WirePosition.error_Ymm;
					ContorY++;
					if (ContorY >= SaveDataLenght) ContorY =0;
				//}
		break;
	}
}
//////  CALCULEZ MEDIA ULTIMELOR 32 DE DATE //ELIMIN DATELE ERONATE
	uint8_t MedieDate( uint8_t AXA){
		
		volatile uint8_t index_date=0,ErrorCount=0,DataCount=0;
		volatile uint32_t LocalDataMedie=0;
		volatile uint16_t *PointData ;
		volatile uint8_t *PointError ;

		 //for ( index_date=0; index_date < SaveDataLenght;index_date++)
		 //{
			 //FirX.PozitiePixel[index_date] = 8000;
			 //FirX.PozitieError[index_date] =0;
			 ////if ((index_date> 10) && (index_date<22)) FirX.PozitieError[index_date] =2;
		 //}			 
		 // incar tebelul cu masuratori in pointer
		switch(AXA){
			case 'x':
				PointData = (uint16_t) &FirX.PozitiePixel[0];
				PointError =(uint16_t) &FirX.PozitieError[0];
			break;
			case 'y':
				PointData =(uint16_t)&FirY.PozitiePixel[0];
				PointError =(uint16_t) &FirY.PozitieError[0];			
			break;
			default:
							PointData = (uint16_t)&FirX.PozitiePixel[0];
							PointError = (uint16_t)&FirX.PozitieError[0];
		}
 // calculez media datelor cu eroare zero		 
		 LocalDataMedie = 0;
		 for ( index_date=0; index_date < (SaveDataLenght);index_date++)
		 {			 
			 if ((*(PointError+index_date)) == 0)
			 {	 LocalDataMedie+= *(PointData+index_date);
				 DataCount++;				 
			 }		 
			  //PointError++;
			  //PointData++;
		 }		 
//verific cite date au fost cu eroare 0;		 
		 if (DataCount >= (SaveDataLenght-4))
		 {
				if (AXA == 'x') {
					WirePosition.X_mm_mediu = LocalDataMedie/DataCount;   // media valorilor
					WirePosition.errorXmediu = 0;                         // media valorilor
					}
				if (AXA == 'y') {
					WirePosition.Y_mm_mediu = LocalDataMedie/DataCount;
					WirePosition.errorYmediu = 0;
				}
			return OK;				
		 }
		else
		{
				if (AXA == 'x')
				{
					WirePosition.X_mm_mediu = WirePosition.mm_X;  //ultima valoare
						if (DataCount >= (SaveDataLenght/2))    WirePosition.errorXmediu = MasuratoareIstabila;  //daca sunt peste 16 instabil 
						else WirePosition.errorXmediu = WirePosition.error_Xmm;  //daca sub 16 copiez eroarea
				}
				if (AXA == 'y')
				{
					WirePosition.Y_mm_mediu = WirePosition.mm_Y;  //ultima valoare
						if (DataCount >= (SaveDataLenght/2)) WirePosition.errorYmediu = MasuratoareIstabila;
						else WirePosition.errorYmediu = WirePosition.error_Ymm;
				}
			return NotOK;				 
		} 
		return NotOK;
	}
