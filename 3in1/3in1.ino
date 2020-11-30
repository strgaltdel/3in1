/*
###################################################################################
#######																		#######
#######		    			"3in1", a tool to measure						#######
#######	  			flap deflection, EWD & CG of flying rc gear				#######
#######																		#######
#######	      			inspired by the dedicated ardu-apps					#######
#######					 which were published on RCN & RCG					#######
#######			   thanks to all who gave valuable infor & inputs 			#######
#######																		#######
#######																		#######
#######																		#######
#######	 Rev 1.00															#######
#######	 02 DEC 2020														#######
#######	 by strgaltdel														#######
###################################################################################

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
*************************************************************************************/

#define INTRO
#define MAIN_ID 999
bool mainRefStatus = true;									// 1 = refresh screen

#include "includes_main.h"




void setup() {

  bool hx711Status = false;	
	
  Serial.begin(115200);
  /*
  eepromValues = Eeprom::getValues();						// get eeprom values
  
															// init units
  if (eepromValues.metricImpOption == 0) {					// metric:												
				unit_lenght = str_mm;
				unit_lenght2 = str_mm2;
				unit_factorLN = 1.00;						
				unit_weight = str_g;
				unit_weight2 = str_g2;
				unit_factorWG = 1.0;	
		}
  else {													// imperial		
				unit_lenght = str_inch;
				unit_lenght2 = str_inch2;
				unit_factorLN = 1.0/25.4;					
				unit_weight = str_ounce;
				unit_weight2 = str_ounce2;
				unit_factorWG = 0.035274;	
		}														
  */
  updateEEpromValues();										// get eeprom values
  tftinit();											  	// init tft
  Serial.println("Rev 1.00  29.Nov 2020");
  
  frontCell.begin(FRONT_CELL_DOUT, FRONT_CELL_SCK);  		// init load cells (ADA)

	
  if (frontCell.wait_ready_timeout(200)) {
    long reading = frontCell.read();
    Serial.println("HX711 front reading.. ");
	hx711Status = true;
    } 
  else {
    Serial.println("HX711 front not found.");
    }
    
  rearCell.begin(REAR_CELL_DOUT, REAR_CELL_SCK);
  
   if (rearCell.wait_ready_timeout(100)) {
    long reading = rearCell.read();
    Serial.println("HX711 rear reading.. ");
   } else {
    Serial.println("HX711 rear not found.");
	hx711Status = false;
   }
 

  if(hx711Status == true){									// init&tara if hx711 found
  
	frontCell.set_scale(eepromValues.frontScale);
	rearCell.set_scale(eepromValues.rearScale); 
  }
  
  // Initialize Imu Sensors 
  IMUsensor.init(0);										//init IMU sensor:0 (wing)
  IMUsensor.init(1);										//init IMU sensor:1	(tail)
  IMUsensor.init(2);										//init IMU sensor:2 (V-tail)
  IMUsensor.init(3);										//init IMU sensor:3 (Flap#0)
  IMUsensor.init(4);										//init IMU sensor:4 (Flap#1)
  
}

void dispHeader() {

	tft.setTextSize(4);					
	tft.setTextColor(WHITE);
	tft.setCursor(tWidth*0.5 -45, tHeight*0.04);
	tft.println("3in1");
				
	tft.setTextSize(1);	
	tft.setTextColor(CYAN);
	tft.setCursor(tWidth*0.5-58, tHeight*0.16);
	tft.println("Rev 1.0 by NOWAtec");
//	tft.println("Rev 1.0, dem Paul seine");
}

void loop() {

	int i, handler;									// handler is used to control menue options
	Button  but[6];									// some buttons have to be used in this menue
	int numButtons = 2;								// number of buttons on this screen, starting with 0
	
	// description of buttons:
	but[0].type 		= BUT_XXL;					// button size
	but[0].colour		= BUTCOLOUR;				// button colour
	but[0].label		= str_CGSCALE;				// label
	but[0].labelcolour 	= BUTTXTCOLOUR;				// label - colour
	but[0].xpos			= 30;						// X position
	but[0].ypos			= 100;						// Y position
	
	// and for the next buttons we go short:
	but[1] = {BUT_XXL, BUTCOLOUR, 	str_EWDSCALE, 	BUTTXTCOLOUR, 30, 170};	
	but[2] = {BUT_XXL, BUTCOLOUR, 	str_DEFLSCALE, 	BUTTXTCOLOUR, 30, 240};
	
	but[3] = {BUT_XXL, BUTCOLOUR, 	str_Intro_1, 	BUTTXTCOLOUR, 30, 100};	
	but[4] = {BUT_XXL, BUTCOLOUR, 	str_Intro_2, 	BUTTXTCOLOUR, 30, 170};	
	but[5] = {BUT_XXL, BUTCOLOUR, 	str_Intro_3, 	BUTTXTCOLOUR, 30, 240};
	


	
	

	// draw Buttons 		************

	if (mainRefStatus == true) {
	  tft.fillScreen(BACKGROUND);
	  if (introFlag) {
	  // intro
	    #ifdef INTRO
		DrawButton(but[3]);
		delay(1600);		
		tft.fillRect(0,100,tWidth,55, BLACK);
		DrawButton(but[4]);
		delay(1600);		
		tft.fillRect(0,170,tWidth,55, BLACK);
		DrawButton(but[5]);
		delay(1600);
		introFlag=false;
	    #endif 
	  }
	  
	  for (i=0; i<=2; i++) {
		DrawButton(but[i]);
	  }
	  mainRefStatus = false;			// at this moment no screen refresh needed
	  dispHeader();
	}
	
	handler = MAIN_ID;				// init handler

	
	
														//  *****    get touched Button:  **********		
	if ( checkPressed()==true){							// touch was pressed ?
		for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
			if (checkButton(but[i],i)) {				
				handler = i;							// if button was pressed, return handler
				mainRefStatus = true;					// need screen to refresh 
			}
		}		
	}
	
	// handler				*************
	switch (handler)
	{
		case 0:
			Serial.println("Jump to cg scale");
			cg_main();		//jump to incidence subroutine
			break;
		case 1:
			Serial.println("Jump to incidence meter");
			incidence();	//jump to incidence subroutine
			break;

		case 2:
			Serial.println("Jump to deflection");
			deflection();	//jump to deflection subroutine
			break;
			
	handler = MAIN_ID;
	}

 }


 
 


