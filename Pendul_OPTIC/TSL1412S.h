#ifndef __TSL1412S_H
#define __TSL1412S_H

#ifdef __cplusplus
extern "C" {
	#endif
/////////////////////////____ Sensor TSL1410R pins declarations /////////////////////////////////////////////////
//SI must go low before the rising edge of the next clock pulse.
// SI -----|______
// clk   _____|-----
#include <avr/io.h>
//sensor signals
extern uint8_t  TSL_SI_HOLD_x;		// bit 2
extern uint8_t TSL_SO_x ;			// bit 1
extern uint8_t TSL_AO_x ;			// bit 6
extern uint8_t TSL_SI_HOLD_y ;		// bit 4
extern uint8_t TSL_SO_y ;			// bit 5
extern uint8_t TSL_AO_y ;			// bit 3
extern uint8_t TSL_CLK_y ;			// bit 7
extern uint8_t TSL_CLK_x ;			// bit 8

//sensor signals end

#define TSL_pixel_lenght 1536 //1280
#define TSL_pixel_Half_lenght 768 //640
//wire dimension in pixels
#define NoisWireWidth 5
#define MinimWireWidth 11
#define MaxWireWidth 63
#define Pixelsixe_uM 6.35//63.5 //um


#define TSL_PORT_uC PORTA
#define TSL_DDR_uC DDRA
#define TSL_PinPort_uC PINA



/////////////////////////____ End Sensor TSL1410R pins  /////////////////////////////////////////////////

uint8_t TSL_Port_initalization(uint8_t TSL_SI_HOLD,uint8_t TSL_SO ,uint8_t TSL_AO, uint8_t TSL_clk );
uint8_t  ReadPozitionfromTSL(uint8_t TSL_SI_HOLD,uint8_t TSL_SO, uint8_t TSL_clk, uint8_t XorY);
void  ReadPixelValuefromTSL(uint16_t PixelPosition,uint8_t XorY);
uint8_t DetermineWirePosition( uint8_t XorY);
uint8_t CalculezPozitieFirMM( uint16_t PixelOrderCalc, uint8_t xyClac);
void Captura_AxaX(uint8_t x_cmd);
void Captura_AxaY(uint8_t y_cmd);

void TSL_nop(void);

//#endif
#ifdef __cplusplus
}
#endif

#endif /* CHEADER_H_INCLUDED */