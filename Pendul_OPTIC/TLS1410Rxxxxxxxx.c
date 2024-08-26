#include "TSL1410R.h"
#include "UsedVariable.h"
#include "uC_Periferic_Initialisation.h"
#include "FunctiiOperare.h"

//sensor signals
uint8_t  TSL_SI_HOLD_x = 2;		// bit 2
uint8_t TSL_SO_x = 1;			// bit 1
uint8_t TSL_AO_x = 0;			// bit 6
uint8_t TSL_SI_HOLD_y = 5;		// bit 4
uint8_t TSL_SO_y = 4;			// bit 5
uint8_t TSL_AO_y = 3;			// bit 3
uint8_t TSL_CLK = 6;			// bit 7
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
uint8_t TSL_Port_initalization(uint8_t TSL_SI_HOLD,uint8_t TSL_SO ,uint8_t TSL_AO ){
	volatile  uint8_t TSL_local_data=0;
	//reset port


	
	TSL_local_data = (TSL_DDR_uC & (~((1<<TSL_SI_HOLD_x)| (1<<TSL_SO_x) | (1<<TSL_CLK) | (1<<TSL_AO_x) | (1<<TSL_SI_HOLD_y)| (1<<TSL_SO_y) | (1<<TSL_AO_y))));
	TSL_local_data |= ((1<<TSL_SI_HOLD_x)| (0<<TSL_SO_x) | (1<<TSL_CLK) | (0<<TSL_AO_x) |(1<<TSL_SI_HOLD_y)| (0<<TSL_SO_y) | (0<<TSL_AO_y));
	//TSL_SI_HOLD=out  TSL_SO =int   TSL_CLK = out    TSL_AO=z analog
	TSL_DDR_uC = TSL_local_data;
	
	TSL_local_data = (TSL_PORT_uC & (~((1<<TSL_SI_HOLD_x)| (1<<TSL_SO_x) | (1<<TSL_CLK) | (1<<TSL_AO_x)| (1<<TSL_SI_HOLD_y)| (1<<TSL_SO_y) | (1<<TSL_AO_y))));
	TSL_local_data |= ((0<<TSL_SI_HOLD_x)| (0<<TSL_SO_x) | (1<<TSL_CLK) | (0<<TSL_AO_x)|(0<<TSL_SI_HOLD_y)| (0<<TSL_SO_y) | (0<<TSL_AO_y));
	TSL_PORT_uC = TSL_local_data;
	//check  TSL_SO = low
	//wait for pin TSL_SO = to be low
	TSL_nop();
	//TSL_nop();
	//TSL_nop();
	
	TSL_local_data = (TSL_PinPort_uC & (1<<TSL_SO)) ;
	if ((TSL_local_data) != 0 ) return NotOK; //0x10
	else return OK;//if SO=0 return OK;TSL_SO
}
//xxxxxxxxxxxxxxx


//xxxxxxxxxxxxxxxxxxx
uint8_t  ReadPozitionfromTSL(uint8_t TSL_SI_HOLD,uint8_t TSL_SO){
	volatile uint16_t clock_index =0;
	volatile uint8_t  RP_local_data =0;
	volatile uint8_t  Returned_value =0;
	//RP_local_data =
	TSL_PORT_uC |=((1<<TSL_CLK));// clk=1;          0____/----1
	TSL_nop();
	TSL_PORT_uC &=(~(1<<TSL_CLK));// clk=0;          1----\____0
	TSL_nop();
	TSL_PORT_uC |=((1<<TSL_SI_HOLD_x)|(1<<TSL_SI_HOLD_y));// SI=1;;      __0_______/----1
	TSL_nop();

	//for (clock_index =0;clock_index < (TSL_pixel_lenght+10); clock_index++)  //10 cresc integrasion time
	for (clock_index =0;clock_index < (TSL_pixel_lenght+10); clock_index++)  //10 cresc integrasion time
	{
		TSL_PORT_uC |=((1<<TSL_CLK));// clk=1;          0____/----1
		TSL_nop();
		TSL_PORT_uC &=(~((1<<TSL_SI_HOLD_x)|(1<<TSL_SI_HOLD_y)));// SI=0;;      1----\_________0
		TSL_nop();
		TSL_PORT_uC &=(~(1<<TSL_CLK));// clk=0;          1----\____0
		TSL_nop();
		// TSL_nop();   //one ore more; to have an certain settling time of min120ns
		///////////////////
		//Red analog out of sensor or pixel value
		if (clock_index < TSL_pixel_lenght) ReadPixelValuefromTSL(clock_index);
		////////////////////
		if(clock_index == (TSL_pixel_lenght-1))//TSL_pixel_Half_lenght
		{   
			TSL_nop();
			RP_local_data = TSL_PinPort_uC;//(TSL_PinPort_uC & (1<<TSL_SO)) ;
			RP_local_data &= (1<<TSL_SO) ;
			
			if ((RP_local_data) != 0 ) Returned_value = OK; // if SO=1 return OK;TSL_SO
			else Returned_value = NotOK;
		}
		TSL_nop();
	}
	//////end for
	//last value for clk and SI hold 0;
	TSL_PORT_uC |=((1<<TSL_SI_HOLD_x)|(1<<TSL_SI_HOLD_y));// TSL_SI_HOLD=1;          0____/----1
	TSL_nop();
	TSL_PORT_uC |=((1<<TSL_CLK));// TSL_SI_HOLD=1;          0____/----1
	TSL_nop();
	TSL_PORT_uC &=(~((1<<TSL_SI_HOLD_x) |(1<<TSL_SI_HOLD_y)));// TSL_SI_HOLD=0;          1----\____0
	TSL_nop();
	TSL_PORT_uC &=(~(1<<TSL_CLK));// TSL_CLK=0;;      1----\_________0
	//				TSL_PORT_uC &=(~((1<<TSL_SI_HOLD)|(1<<TSL_CLK)));// SI=Clk=0;;      1----\_________0
	return Returned_value;
}


//xxxxxxxxxxxxxxxxxxx
// shift out 1280 pixels
//input void, return [NotOK] if SO is not active at half time 

//uint8_t  ReadPozitionfromTSL(uint8_t TSL_SI_HOLD,uint8_t TSL_SO){	
	//volatile uint16_t clock_index =0;
	//volatile uint8_t  RP_local_data =0;
	//volatile uint8_t  Returned_value =0;
	////RP_local_data = 
	//TSL_PORT_uC |=((1<<TSL_CLK));// clk=1;          0____/----1
    //TSL_nop(); 
	//TSL_PORT_uC &=(~(1<<TSL_CLK));// clk=0;          1----\____0
	//TSL_nop();
	//TSL_PORT_uC |=((1<<TSL_SI_HOLD));// SI=1;;      __0_______/----1
	//TSL_nop();
//
	//for (clock_index =0;clock_index < (TSL_pixel_lenght+10); clock_index++)
	//{
			//TSL_PORT_uC |=((1<<TSL_CLK));// clk=1;          0____/----1
			//TSL_nop();
			//TSL_PORT_uC &=(~(1<<TSL_SI_HOLD));// SI=0;;      1----\_________0
			//TSL_nop();
			//TSL_PORT_uC &=(~(1<<TSL_CLK));// clk=0;          1----\____0
	        //TSL_nop();
		   //// TSL_nop();   //one ore more; to have an certain settling time of min120ns
/////////////////////
			////Red analog out of sensor or pixel value
			//ReadPixelValuefromTSL(clock_index);
//////////////////////			
			//if(clock_index == TSL_pixel_Half_lenght)
			//{
			        //RP_local_data = TSL_PinPort_uC;//(TSL_PinPort_uC & (1<<TSL_SO)) ;
			        //RP_local_data &= (1<<TSL_SO) ;
					//
					//if ((RP_local_data) != 0 ) Returned_value = OK; // if SO=1 return OK;TSL_SO
					//else Returned_value = NotOK;	
			//}
			//TSL_nop();
	//}
	////////end for
          ////last value for clk and SI hold 0;
				//TSL_PORT_uC |=((1<<TSL_SI_HOLD));// TSL_SI_HOLD=1;          0____/----1				
				//TSL_nop();
				//TSL_PORT_uC |=((1<<TSL_CLK));// TSL_SI_HOLD=1;          0____/----1
				//TSL_nop();				
				//TSL_PORT_uC &=(~(1<<TSL_SI_HOLD));// TSL_SI_HOLD=0;          1----\____0
				//TSL_nop();
				//TSL_PORT_uC &=(~(1<<TSL_CLK));// TSL_CLK=0;;      1----\_________0
////				TSL_PORT_uC &=(~((1<<TSL_SI_HOLD)|(1<<TSL_CLK)));// SI=Clk=0;;      1----\_________0
	//return Returned_value;
//}


/////////////////////   -nop /////////////////////

void TSL_nop(void){
		asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
		//asm ("nop");
	}
/////////////////////   end -nop /////////////////////

/////////////////////   ReadPixelValuefromTSL /////////////////////
//  valoarea pizelului "PixelPosition" este incarcata in sirul PixelsValue pe pozitia  PixelPosition
// 
void  ReadPixelValuefromTSL(uint16_t PixelPosition)
{
	//PixelsValue[PixelPosition] =PixelPosition;
	if ((ACSR & (1<<ACO)) !=0) PixelsValue[PixelPosition] =0;//0;//0;
	else PixelsValue[PixelPosition] =1;//1;//1;	
}
/////////////////////   end -ReadPixelValuefromTSL /////////////////////

////input = pointer to the string  1280 lenght
////Return wire position (the middle wire position) as index pixel of 63um 1280*63um=80.640mm
uint8_t DetermineWirePosition( unsigned char *PointerToPixels,uint8_t XorY){
	volatile unsigned char *PixelsOrder, *OldPixelsOrder;
	//volatile unsigned char OldPixelValue=0;
	volatile uint16_t WireWidth=0;
    volatile uint16_t ShadowWidth=0;
	volatile uint8_t count_SmalShadow=0,count_LargeShadow=0;
	
	
OldPixelsOrder	= PointerToPixels;

	//PixelsOrder = (PointerToPixels+245);
	//
	//for (WireWidth =0;WireWidth < 16 ; WireWidth++)
		//{
			//*(PixelsOrder+WireWidth)=0;
		//}
		
PixelsOrder = PointerToPixels;	
		//for (PixelIndex =0;PixelIndex < TSL_pixel_lenght ; PixelIndex++)
		while(( PixelsOrder) < (PointerToPixels+MaXLenghtPixel))
		{
			if ((*(PixelsOrder))< 1 )  // valoare pixel mai mic de 1/ pixel value low that 1
			{
                OldPixelsOrder = PixelsOrder;
				WireWidth=0;//fir size=0
				while (((*(PixelsOrder))< 1) && (WireWidth < MaxWireWidth)) // valoare pixel mai mic de 1
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
						{	WirePosition.N_X = ((OldPixelsOrder - PointerToPixels)+(WireWidth>>1)-1);
							WirePosition.error_X = OK_shadow;
						}
						if (XorY == 'y') 
						{   WirePosition.N_Y = ((OldPixelsOrder - PointerToPixels)+(WireWidth>>1)-1);
							WirePosition.error_Y = OK_shadow;
						}
						WireWidth =0;
						
					}  //end if compute position				
			}//end if valoare pixel mai mic de 1
			PixelsOrder++;
		}//end for
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

uint8_t CalculateWirePositionInMM( uint16_t PixelOrder, uint8_t xy)
{
	volatile uint8_t error_in;
	
	switch(xy){
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
   if ( (PixelOrder > MinimWireWidth) && (PixelOrder < (MaXLenghtPixel - MaxWireWidth)))  //[15  1580-63]

	{		switch(xy){
					case 'x':
						WirePosition.mm_X = PixelOrder*Pixelsixe_uM;
						WirePosition.error_Xmm = (error_in );
					break;
					case 'y':
						WirePosition.mm_Y = PixelOrder*Pixelsixe_uM;
						WirePosition.error_Ymm = (error_in );
					break;
					}
			return OK;
	} 
	else 
	{
		switch(xy){
			case 'x':
					WirePosition.error_Xmm = (error_in | Error_mm_outOfRange);
			break;
			case 'y':
					WirePosition.error_Ymm = (error_in | Error_mm_outOfRange);
			break;
		return NotOK;
		}
	}
	return NotOK;
}

// void  function 
//initializare comp, initializare port TSL, led on Citire, calcul + tx uder0 
void CitireAxaX(uint8_t x_cmd){
	volatile uint16_t local_sterg=0;

		WirePosition.error_Y = 0; //clear flag error
		WirePosition.error_Ymm = 0;
		WirePosition.N_Y = 0;	

// sterg citirile anterioare
	       for(local_sterg =0;local_sterg < TSL_pixel_lenght; local_sterg++) {PixelsValue[local_sterg] = 0;}	
	
			AnalogComparator_Initialisation(Channel_AO_y); //admux0Channel_AO_x
			ON_LED_x;
			if(TSL_Port_initalization(TSL_SI_HOLD_x, TSL_SO_x , TSL_AO_x ) !=NotOK)
			{  
				if( ReadPozitionfromTSL(TSL_SI_HOLD_x, TSL_SO_x) !=NotOK)
				 {      OFF_LED_x;
					    Comparator_Disable;
						Comparator_MuxDisable;
					    DetermineWirePosition(PixelsValue,x_cmd); 
						//if(	DetermineWirePosition(PixelsValue,x_cmd)!=NotOK)
						//{ON_LED2;
							WirePosition.mm_X = WirePosition.N_X;
						//CalculateWirePositionInMM(WirePosition.N_X,x_cmd);
				 }
			     else WirePosition.error_X = TSL_dont_answar;
				//}
			}
			else WirePosition.error_X = TSL_init_faild;//{//TSL initialization failed
		
		Trimite_Valoare('x',WirePosition.mm_X);//WirePosition.mm_X
}

//initializare comp, initializare port TSL, led on Citire, calcul + tx uder0
void CitireAxaY(uint8_t y_cmd){	
	volatile uint16_t local_sterg=0;
	
							//WirePosition.error_Y = 0; //clear flag error
							//WirePosition.error_Ymm = 0;
							//WirePosition.N_Y = 0;
	WirePosition.error_X = 0; //clear flag error
	WirePosition.error_Xmm = 0;
	WirePosition.N_X = 0;		
		
	for(local_sterg =0;local_sterg < TSL_pixel_lenght; local_sterg++) {PixelsValue[local_sterg] = 0;}
	
		AnalogComparator_Initialisation(Channel_AO_x); //admux0Channel_AO_y
		ON_LED_y;
				//if(TSL_Port_initalization(TSL_SI_HOLD_y, TSL_SO_y , TSL_AO_y ) !=NotOK)
				TSL_Port_initalization(TSL_SI_HOLD_y, TSL_SO_y , TSL_AO_y );
				//{
					//if( ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y) !=NotOK)
					ReadPozitionfromTSL(TSL_SI_HOLD_y, TSL_SO_y);
					//{   
						OFF_LED_y;
						Comparator_Disable;
						Comparator_MuxDisable;
						DetermineWirePosition(PixelsValue,y_cmd);
						//if(	DetermineWirePosition(PixelsValue,x_cmd)!=NotOK)
						//{ON_LED2;
							WirePosition.mm_Y = WirePosition.N_Y;
						//CalculateWirePositionInMM(WirePosition.N_Y,y_cmd);
					//}
					//else WirePosition.error_Y = TSL_dont_answar;
					//}
				//}
				//else WirePosition.error_Y = TSL_init_faild;//{//TSL initialization failed
	
					Trimite_Valoare('y',WirePosition.mm_Y);//WirePosition.mm_X
}