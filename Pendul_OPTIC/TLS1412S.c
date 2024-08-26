#include "TSL1412S.h"
#include "UsedVariable.h"
#include "uC_Periferic_Initialisation.h"
#include "Functions.h"

//sensor signals
uint8_t  TSL_SI_HOLD_x = 2;		// bit 2
uint8_t TSL_SO_x = 1;			// bit 1
uint8_t TSL_AO_x = 0;			// bit 6
uint8_t TSL_SI_HOLD_y = 5;		// bit 4
uint8_t TSL_SO_y = 4;			// bit 5
uint8_t TSL_AO_y = 3;			// bit 3
uint8_t TSL_CLK_y = 6;			// bit 7
uint8_t TSL_CLK_x = 7;			// bit 7

//sensor signals end

//uint8_t TSL_Port_initalization(uint8_t TSL_SI_HOLD,uint8_t TSL_SO ,uint8_t TSL_AO ){
//volatile  uint8_t TSL_local_data=0;
 ////reset port
//
//
 //
//TSL_local_data = (TSL_DDR_uC & (~(1<<TSL_SI_HOLD)| (1<<TSL_SO) | (1<<TSL_CLK) | (1<<TSL_AO)));	
//TSL_local_data |= ((1<<TSL_SI_HOLD)| (0<<TSL_SO) | (1<<TSL_CLK) | (0<<TSL_AO));
                  ////TSL_SI_HOLD=out  TSL_SO =int   TSL_CLK = out    TSL_AO=z analog
//TSL_DDR_uC = TSL_local_data;
	//
//TSL_local_data = (TSL_PORT_uC & (~(1<<TSL_SI_HOLD)| (1<<TSL_SO) | (1<<TSL_CLK) | (1<<TSL_AO)));
//TSL_local_data |= ((0<<TSL_SI_HOLD)| (0<<TSL_SO) | (1<<TSL_CLK) | (0<<TSL_AO));
//TSL_PORT_uC = TSL_local_data;
////check  TSL_SO = low
////wait for pin TSL_SO = to be low
//TSL_nop();
//
//
 //
        //TSL_local_data = (TSL_PinPort_uC & (1<<TSL_SO)) ;
		//if ((TSL_local_data) != 0 ) return NotOK; //0x10
		//else return OK;//if SO=0 return OK;TSL_SO
//}

//xxxxxxxxxxxxxxx
uint8_t TSL_Port_initalization(uint8_t TSL_SI_HOLD,uint8_t TSL_SO ,uint8_t TSL_AO , uint8_t TSL_clk){
	volatile  uint8_t TSL_local_data=0;
	//reset port


	
	TSL_local_data = (TSL_DDR_uC & (~((1<<TSL_SI_HOLD)| (1<<TSL_SO)  | (1<<TSL_clk) | (1<<TSL_AO))));
	TSL_local_data |= ( (1<<TSL_SI_HOLD)| (0<<TSL_SO) | (1<<TSL_clk) | (0<<TSL_AO) );
	//TSL_SI_HOLD=out  TSL_SO =int   TSL_CLK = out    TSL_AO=z analog
	TSL_DDR_uC = TSL_local_data;//directie
	
	TSL_local_data = (TSL_PORT_uC & (~((1<<TSL_SI_HOLD) | (1<<TSL_SO) | (1<<TSL_clk) | (1<<TSL_AO))));
	TSL_local_data |= ((0<<TSL_SI_HOLD)| (0<<TSL_SO) | (1<<TSL_clk) | (0<<TSL_AO));
	TSL_PORT_uC = TSL_local_data;
	//check  TSL_SO = low
	//wait for pin TSL_SO = to be low
	TSL_nop();
	
	TSL_local_data = (TSL_PinPort_uC & (1<<TSL_SO)) ;
	if ((TSL_local_data) != 0 ) return NotOK; //0x10
	else return OK;//if SO=0 return OK;TSL_SO
}
//xxxxxxxxxxxxxxx


//xxxxxxxxxxxxxxxxxxx
uint8_t  ReadPozitionfromTSL(uint8_t TSL_SI_HOLD,uint8_t TSL_SO, uint8_t TSL_clk, uint8_t XorY){
	volatile uint16_t clock_index =0;
	volatile uint8_t  RP_local_data =0;
	
	//test SO low
				//RP_local_data = TSL_PinPort_uC;//(TSL_PinPort_uC & (1<<TSL_SO)) ;
				//RP_local_data &= (1<<TSL_SO) ;  // ar trebui sa fie 0 daca nu-i mai astept ALTA DATA
	
	
	//RP_local_data =
	TSL_PORT_uC |=((1<<TSL_clk));// clk=1;          0____/----1
	TSL_nop();
	TSL_PORT_uC &=(~(1<<TSL_clk));// clk=0;          1----\____0
	TSL_nop();
	TSL_PORT_uC |=(1<<TSL_SI_HOLD);// SI=1;;      __0_______/----1
	TSL_nop();
	TSL_nop();
	TSL_nop();

	for (clock_index =0;clock_index < (TSL_pixel_lenght + 300 ); clock_index++)  //10 cresc integrasion time
	//for (clock_index =0;clock_index < (TSL_pixel_lenght-1); clock_index++)  //10 cresc integrasion time
	{
		TSL_PORT_uC |=((1<<TSL_clk));// clk=1;          0____/----1
		TSL_nop();
		TSL_nop();
		TSL_nop();
		TSL_nop();
		TSL_nop();
	
		TSL_PORT_uC &=(~((1<<TSL_SI_HOLD)));// SI=0;;      1----\_________0
		TSL_nop();

		TSL_PORT_uC &=(~(1<<TSL_clk));// clk=0;          1----\____0
		TSL_nop();
		//TSL_nop();   //one ore more; to have an certain settling time of min120ns
		///////////////////
		//Red analog out of sensor or pixel value
		if (clock_index < TSL_pixel_lenght) ReadPixelValuefromTSL(clock_index, XorY);
		////////////////////
		//if(clock_index == (TSL_pixel_lenght-1))//TSL_pixel_Half_lenght
		if((clock_index >= (TSL_pixel_lenght-5)) && (RP_local_data ==0))//TSL_pixel_Half_lenght  //testez inainte cu 5 pixeli de final daca vine SO
		{   
			TSL_nop();
			RP_local_data = TSL_PinPort_uC;//(TSL_PinPort_uC & (1<<TSL_SO)) ;
			RP_local_data &= (1<<TSL_SO) ;

		}
		TSL_nop();
	}
	//////end for
	//last value for clk and SI hold 0;
	TSL_PORT_uC |=((1<<TSL_SI_HOLD));// TSL_SI_HOLD=1;          0____/----1
	TSL_nop();
	TSL_PORT_uC |=((1<<TSL_clk));// TSL_SI_HOLD=1;          0____/----1
	TSL_nop();
	TSL_nop();
	TSL_nop();
	TSL_nop();


	TSL_PORT_uC &=(~((1<<TSL_SI_HOLD)));// TSL_SI_HOLD=0;          1----\____0
	TSL_nop();
	TSL_DDR_uC = 0xE4;
	TSL_PORT_uC = 0x1B;
	//TSL_PORT_uC = 0x1B;//(~(1<<TSL_clk));// TSL_CLK=0;;      1----\_________0
	//				TSL_PORT_uC &=(~((1<<TSL_SI_HOLD)|(1<<TSL_CLK)));// SI=Clk=0;;      1----\_________0
	
				if ((RP_local_data) != 0 ) return OK; // if SO=1 return OK;TSL_SO
				else return NotOK;
	
}




void TSL_nop(void){
				asm ("nop");
				asm ("nop");
				asm ("nop");
				asm ("nop");
				asm ("nop"); 
				asm ("nop");
				asm ("nop");
				asm ("nop");
				asm ("nop");
				asm ("nop");
	}
/////////////////////   end -nop /////////////////////

/////////////////////   ReadPixelValuefromTSL /////////////////////
//  valoarea pizelului "PixelPosition" este incarcata in sirul PixelsValue pe pozitia  PixelPosition
// 
void  ReadPixelValuefromTSL(uint16_t PixelPosition,uint8_t XorY)
{
	//PixelsValue[PixelPosition] =PixelPosition;
	if (XorY =='y')//trebuie inversat pt ca pe scanarea lui x citesc de fapt y si invers
	{   
					//if ((PixelPosition > 100) && (PixelPosition < 150))  PixelsValue_Y[PixelPosition] =0;
					//else  PixelsValue_Y[PixelPosition] =2;
		
		if ((ACSR & (1<<ACO)) !=0) PixelsValue_Y[PixelPosition] =0;//0;//0;
		else PixelsValue_Y[PixelPosition] =1;//1;//1;		
	}
	if (XorY =='x')
	{
					//if ((PixelPosition > 500) && (PixelPosition < 550))  PixelsValue_X[PixelPosition] =0;
					//else  PixelsValue_X[PixelPosition] =1;		
		if ((ACSR & (1<<ACO)) !=0) PixelsValue_X[PixelPosition] =0;//0;//0;
		else PixelsValue_X[PixelPosition] =1;//1;//1;
	}


}
/////////////////////   end -ReadPixelValuefromTSL /////////////////////

////input = pointer to the string  1280 lenght
////Return wire position (the middle wire position) as index pixel of 63um 1280*63um=80.640mm
uint8_t DetermineWirePosition( uint8_t XorY){
	volatile uint16_t WireWidth=0;
    volatile uint16_t ShadowWidth=0;
	volatile uint8_t count_SmalShadow=0,count_LargeShadow=0;
	volatile uint16_t OldPixelsOrder=0, PixelsOrder=0;
	volatile uint8_t * PointerPixelArray;
	
	//RESETEZ POZITIA SI EROAREA CORESPUNZATOARE DIRECTIE CITITE	
	if (XorY == 'x')
	{	WirePosition.mm_X = 0;
		WirePosition.N_X = 0;
		WirePosition.error_X = 0;
		PointerPixelArray = PixelsValue_X; //pointer la situl lui X
	}
	if (XorY == 'y')
	{   WirePosition.mm_Y = 0;
		WirePosition.N_Y = 0;
		WirePosition.error_Y = 0;
		PointerPixelArray = PixelsValue_Y; //pointer la situl lui Y
		
	}  
////////////////////////////////////////////////////////	
//-> INCEP DETERMINAREA POZITIEI	
		PixelsOrder = 0;
		//for (PixelIndex =0;PixelIndex < TSL_pixel_lenght ; PixelIndex++)
		while(( PixelsOrder) < (MaXLenghtPixel-20))  //nu citesc pina la ultimi pixeli s-ara fie zero   1111 fir 11000 
		{
			if ((*(PointerPixelArray +PixelsOrder))< 1 )  // valoare pixel mai mic de 1/ pixel value low that 1
			{
                OldPixelsOrder = PixelsOrder;
				WireWidth=0;//fir size=0
				while (((*(PointerPixelArray +PixelsOrder))< 1) && (WireWidth < MaxWireWidth) && (PixelsOrder < MaXLenghtPixel)) // valoare pixel mai mic de 1
				{   
					PixelsOrder++;  //increment pointer
					WireWidth ++;  //determin dimensiune umbrei /shadow with 
					ShadowWidth++;//determin dimensiune umbrey totale+eroare/shadow with with
					
				}				    
					if ((WireWidth > NoisWireWidth) && (WireWidth < MinimWireWidth))
					{   count_SmalShadow++;  // riplu mic 
						if (XorY == 'x') WirePosition.error_X = smal_shadow;
						if (XorY == 'y') WirePosition.error_Y = smal_shadow;
						WireWidth = 0;
					}//end if min
					if (WireWidth > MaxWireWidth)  
					{   count_LargeShadow++;
						if (XorY == 'x') WirePosition.error_X = large_shadow;
						if (XorY == 'y') WirePosition.error_Y = large_shadow;
						return NotOK; 
					}//end if max
					if ((WireWidth >= MinimWireWidth) && ((WireWidth <= MaxWireWidth)))
					{   count_LargeShadow ++; //riplu mare
						if (XorY == 'x') 
						{	WirePosition.N_X = (OldPixelsOrder +(WireWidth>>1)-1);
							WirePosition.error_X = OK_shadow;
						}
						if (XorY == 'y') 
						{   WirePosition.N_Y = (OldPixelsOrder +(WireWidth>>1)-1);
							WirePosition.error_Y = OK_shadow;
						}
						WireWidth =0;
						
					}  //end if compute position				
			}//end if valoare pixel mai mic de 1
			PixelsOrder++;
		}//end while  // AM TERMINAT DE DETERMINAT POZITIA

// OARE AM DETERMINAT CEVA TESTEZ POTITIA RESPECTIV EROAREA PINA AICI
			 if(( XorY == 'x') && (ShadowWidth !=0) && (WirePosition.N_X == 0) && (WirePosition.error_X == 0))
			 {    WirePosition.error_X=LipsaFir;
				  return NotOK ;
			 }
			 if(( XorY == 'y') && (ShadowWidth !=0) && (WirePosition.N_Y == 0) && (WirePosition.error_Y == 0)){
				   WirePosition.error_Y=LipsaFir;
				  return  NotOK;
			 }
////////////////////////////////////////////   IES CU LIPSA FIR
		
			if ((ShadowWidth >(MaxWireWidth+(MaxWireWidth/2))) ||(count_SmalShadow >40))
			{
				if (XorY == 'x') WirePosition.error_X = Noise_shadow;
				if (XorY == 'y') WirePosition.error_Y = Noise_shadow;
				return  NotOK;
			}		    	
			
			if ((ShadowWidth >(MaxWireWidth+(MaxWireWidth/2))) ||(count_LargeShadow >10))
			{				
				if (XorY == 'x') WirePosition.error_X = numerous_shadow;
				if (XorY == 'y') WirePosition.error_Y = numerous_shadow;
				return  NotOK;
			} 
			else return  OK	;	
}
//input parameter [PixelOrder] wire position in order number[1-1280],axis x or y  [xy]
//return true if calculation is OK NotOK if error
//load x, y value of wire position in [WirePosition.mm_X] [WirePosition.mm_Y]

uint8_t CalculezPozitieFirMM( uint16_t PixelOrderCalc, uint8_t xyClac)
{
	volatile uint8_t error_in;
	
	switch(xyClac){
			case 'x':
				error_in = WirePosition.error_X;
			break;	
			case 'y':
				error_in = WirePosition.error_Y;
			break;	
			default:
			return NotOK;
			}
	//error_in = 0;
	//if ( (error_in ==0) && (PixelOrder > MinimWireWidth) && (PixelOrder < (MaXLenghtPixel - MaxWireWidth)))  //[15  1580-63]
   if ((error_in != smal_shadow) && (PixelOrderCalc > MinimWireWidth) && (PixelOrderCalc < (MaXLenghtPixel - MaxWireWidth)))  //[15  1580-63]

	{		switch(xyClac){
					case 'x':
						WirePosition.mm_X = PixelOrderCalc*Pixelsixe_uM;
						WirePosition.error_Xmm = (error_in );
					break;
					case 'y':
						WirePosition.mm_Y = PixelOrderCalc*Pixelsixe_uM;
						WirePosition.error_Ymm = (error_in );
					break;
					}
			return OK;
	} 
	else 
	{
		switch(xyClac){
			case 'x':
					WirePosition.error_Xmm = LipsaFir;
			break;
			case 'y':
					WirePosition.error_Ymm = LipsaFir;
			break;
		return NotOK;
		}
	}
	return NotOK;
}

// void  function 
//initializare comp, initializare port TSL, led on Citire, calcul + tx uder0 
void Captura_AxaX(uint8_t x_cmd){
	volatile uint16_t local_sterg=0;

		WirePosition.error_X = 0; //clear flag error
		WirePosition.error_Xmm = 0;
		WirePosition.N_X = 0;	

// sterg citirile anterioare
	       for(local_sterg =0;local_sterg < TSL_pixel_lenght; local_sterg++) {PixelsValue_X[local_sterg] = 0;}	
	
			AnalogComparator_Initialisation(Channel_AO_x); //admux0Channel_AO_x
			ON_LED_x;

			//TSL_Port_initalization(TSL_SI_HOLD_x, TSL_SO_x , TSL_AO_x, TSL_CLK_x );
			if(TSL_Port_initalization(TSL_SI_HOLD_x, TSL_SO_x , TSL_AO_x, TSL_CLK_x ) !=NotOK)
			{  
				//ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x, TSL_CLK_x, 'x');
				//ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x, TSL_CLK_x, 'x');
				//ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x, TSL_CLK_x, 'x');  denisa

				//ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x, TSL_CLK_x, 'x');
				//WirePosition.N_X = 500;
				if( ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x, TSL_CLK_x, 'x') !=NotOK)
				 {      OFF_LED_x;
						OFF_LED_y; //adaugat 12.23
						
					    Comparator_Disable;
						Comparator_MuxDisable;
					    DetermineWirePosition('x'); 
						//if(	DetermineWirePosition(PixelsValue,x_cmd)!=NotOK)
						//{ON_LED2;
						
						CalculezPozitieFirMM(WirePosition.N_X,'x');
				 }
			     else WirePosition.error_X = TSL_dont_answar;
			}
			else WirePosition.error_X = TSL_init_faild;//{//TSL initialization failed
	   
		   
		   local_sterg = PINC;
		   local_sterg &= (1<<LED_X);  // am ajuns aici su nu am sters ledul ii bai
		   if (local_sterg !=0)
		   {
			   OFF_LED_x;
			   WirePosition.error_Xmm = ErrorLed;
		   }
		//WirePosition.mm_X = 500;   
		//Trimite_Valoare('x',WirePosition.mm_X);//WirePosition.mm_X		
		//salvez datele pentru filtrare suplimentara
		SalvezDate('x');
}

//initializare comp, initializare port TSL, led on Citire, calcul + tx uder0
void Captura_AxaY(uint8_t y_cmd){	
	volatile uint16_t local_sterg=0;
	
	WirePosition.error_Y = 0; //clear flag error
	WirePosition.error_Ymm = 0;
	WirePosition.N_Y = 0;		
		
	for(local_sterg =0;local_sterg < TSL_pixel_lenght; local_sterg++) {PixelsValue_Y[local_sterg] = 0;}
	
		AnalogComparator_Initialisation(Channel_AO_y); //admux0Channel_AO_y
		ON_LED_y;
		
				//TSL_Port_initalization(TSL_SI_HOLD_y, TSL_SO_y , TSL_AO_y, TSL_CLK_y );
				if(TSL_Port_initalization(TSL_SI_HOLD_y, TSL_SO_y , TSL_AO_y, TSL_CLK_y ) !=NotOK) //TSL_Port_initalization(TSL_SI_HOLD_y, TSL_SO_y , TSL_AO_y );
				{
					//ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y, TSL_CLK_y, 'y');
					//ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y, TSL_CLK_y, 'y');  denisa
					if( ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y, TSL_CLK_y, 'y') !=NotOK) //ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y);
					{   
						OFF_LED_y;
						OFF_LED_x; //adaugat 12.23
						Comparator_Disable;
						Comparator_MuxDisable;
						DetermineWirePosition('y');
						//if(	DetermineWirePosition(PixelsValue,x_cmd)!=NotOK)
						//{ON_LED2;
						CalculezPozitieFirMM(WirePosition.N_Y,'y');
					}
					else WirePosition.error_Y = TSL_dont_answar;
				}
				else WirePosition.error_Y = TSL_init_faild;//{//TSL initialization failed
					
						local_sterg = PINC;
						local_sterg &= (1<<LED_Y);  // am ajuns aici su nu am sters ledul ii bai
						if (local_sterg !=0)
						{
							OFF_LED_y;
							WirePosition.error_Ymm = ErrorLed;  //defapt este eroare pe Y
						}	
				//Trimite_Valoare('y',WirePosition.N_Y );//WirePosition.mm_X
				SalvezDate('y');
}