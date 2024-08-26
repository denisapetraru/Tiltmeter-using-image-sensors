#ifndef __UsedVariable_H
#define __UsedVariable_H

#ifdef __cplusplus
extern "C" {
	#endif
//////////////////////////////////////////////////////////////////////////
#include "TSL1412S.h"

#include <avr/wdt.h>
//#define 	wdt_reset()  ;// __asm__ __volatile__ ("wdr")

#define OK 1
#define NotOK 0
#define SaveDataLenght 32


 //////////////  RX1   ////////////////////////
 //################    ADRESA MICROCONTROLLERULUI   ###################33
 #define Adr_uC  0x71//;FD    //0x71, 0x72; 0x73
 //######################################################33
 #define AXA_x  0
 #define AXA_y  1

 //################    CMD MICROCONTROLLERULUI   ###################33
 #define RESET_uC 0x00
 
 #define CitesteMasTrad_uC 0x05 //cit
 #define CitesteVal_uC_x 0x0A  //sel
 #define CitesteVal_uC_y 0x0A  //sel
 
 #define Citire_Instant_x 0x0B
 #define Citire_Instant_y 0x0B
 //#define CitesteVal_uC_3 0x03
 //
 //#define CitesteVal_M_1 0x19
 //#define CitesteVal_M_2 0x20

 #define Antet_Receptie_uC 0x25
 #define Antet_Transmisie_uC_To_PC 0x21
 #define Selectie_manuala 0x05


struct FiltruPixel{
	uint16_t PozitiePixel[SaveDataLenght]; 
	uint8_t PozitieError[SaveDataLenght];
	} FirX, FirY ;



struct Position{
	uint16_t N_X;
	uint8_t error_X;
	uint16_t N_Y;
	uint8_t error_Y;	 
	uint16_t mm_X;
	uint8_t error_Xmm;
	uint16_t mm_Y;
	uint8_t error_Ymm;
	uint16_t X_mm_mediu;
	uint8_t errorXmediu;
	uint16_t Y_mm_mediu;
	uint8_t errorYmediu;	
	} WirePosition;
	
	struct Receptie{
		uint8_t Antet;
		uint8_t Adres_uC;
		uint8_t Cmd_PC;
		uint8_t Adr_Trad;
		uint8_t ValoareH;
		uint8_t ValoareL;
		uint8_t OctetE;
		uint8_t Reg_SC;
	} BuffRx_PC;
	
	/////////////
	//error pixel position message
	#define OK_shadow 0x00  // ok
	#define smal_shadow 0x40 // fir pre subtire
	#define large_shadow 0x41 // umbra prea mare
	#define numerous_shadow 0x42 // umbre multiple
	#define Noise_shadow 0x43  //umbra zgomotoaza
	#define TSL_dont_answar 0x44//0x06  // senzor nu raspunde
    #define TSL_init_faild 0x45  // initializare esuata
    #define ErrorLed 0x46	// led defect
	#define MasuratoareIstabila 0x03  // masuratoare instabila
	//error pixel position MM message
	//#define Error_mm_outOfRange 0xC0  //depasire domeniu
	#define LipsaFir 0x35  //Fir lipsa

//////////////////////////////////////////////////////////////////////////
#define MaXLenghtPixel TSL_pixel_lenght//1536//1280

extern unsigned char CharData;
extern unsigned char PixelsValue_X[TSL_pixel_lenght];
extern unsigned char PixelsValue_Y[TSL_pixel_lenght];

//extern 

extern  uint8_t Actiune;

////////////////////////////////////////

////////////////////////////////////////
//#endif
#ifdef __cplusplus
}
#endif

#endif /* CHEADER_H_INCLUDED */