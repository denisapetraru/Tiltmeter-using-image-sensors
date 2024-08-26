#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
	#endif
	/////////////////////////____ Sensor TSL1410R pins declarations /////////////////////////////////////////////////
	//SI must go low before the rising edge of the next clock pulse.
	// SI -----|______
	// clk   _____|-----
	#include <avr/io.h>
	#include "UsedVariable.h"
	
	//RS485

	////////////////////////////temporar var
	extern uint8_t display_Chars_12[12];
	
	 extern uint8_t Index_Receptie_PC;
	 extern uint8_t Index_Receptie_PC_OLD ;
	//extern uint8_t Reg_O_Er;
	//extern uint8_t Reg_Cmd_PC;
	//extern uint8_t Reg_SC;
	//extern uint8_t Reg_Adr_Trad;
	//
	
	

	void Send_Value (unsigned char cmd, unsigned long int valoare);
	unsigned char Transmission_function(unsigned char CMD);
	void VerificPachete_Intirziate(void);
	void Semnalizare_Pornire(void);
	void SalvezDate( uint8_t AXA);
	uint8_t MedieDate(uint8_t AXA);
		////////////////////////////////////////
	//#endif
	#ifdef __cplusplus
}
#endif

#endif /* CHEADER_H_INCLUDED */