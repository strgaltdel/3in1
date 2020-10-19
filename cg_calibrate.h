/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	cg_calibrate.h														##
##  content:		calibrate hx711 cells												##
##  date:			20 Feb 2020															##
##  rev.:			0.8																	##
##  by strgaltdel																		##
##########################################################################################


    This program is free software: you can redistribut_sube it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distribut_subed in the hope that it will be useful,
    but_sub WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
***************************************************************************************/



#ifndef CG_CALIBRATE
#define CG_CALIBRATE

#include "includes_main.h"


int cg_Calibrate(){

  static int step = 0;
 //static float calibrationWeight=798;		//ref weight for test purpose
  static long cellEmpty;
  static long cellWeighted;
  
  
  int handler = MAIN_ID;
  bool finished = false;
  
  int sizeHeader = 2;
  int yLine1 = tHeight * 0.05;
  int yLine2 = tHeight * 0.15;
  int yLine3 = tHeight * 0.3;
  int xLine  = tWidth * 0.0;
  int numMeasurements = 200;
  tft.setTextSize(sizeHeader);
  
  
  // define but_subtons for this routine
  Button but_sub[5];
  int numButtons = 1;
	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_BACK, 		BUTTXTCOLOUR, tWidth * 0.05, 		tHeight*0.85};	
	but_sub[1] = {BUT_XL, 	BUTCOLOUR, 	str_NEXT, 		BUTTXTCOLOUR, tWidth * 0.55, 		tHeight*0.85};	
	but_sub[2] = {BUT_XL, 	BUTCOLOUR, 	str_NEXT, 		BUTTXTCOLOUR, 10, 					tHeight*0.85};	
	but_sub[3] = {BUT_XL, 	GREEN	, 	str_SAVE, 		BUTTXTCOLOUR, tWidth * 0.05, 		tHeight*0.85};
	
 tft.fillScreen(BACKGROUND);
	
 if (status == WAITING)
  {
    status = WORKING;
    step = 0;
  }
  
  #ifdef DEBUG
		Serial.println("init cg_calibrate");
  #endif


  
  // start screen:
  DrawButton(but_sub[0]);
  DrawButton(but_sub[1]);
  tft.setTextColor(VALUECOLOUR);
  tft.setTextSize(sizeHeader);
  tft.setCursor(xLine, yLine1 );
  tft.println(str_CGcal0); 

  // loop steps:
 while (finished==false)
 {

  

	switch (step) {
		case 0:	
			if ( checkPressed()==true and checkButton(but_sub[0],0))  {								// exit button pressed
						return MAIN_ID;			
			}
			if ( checkPressed()==true and checkButton(but_sub[1],1))  {								// next button pressed
				step++;				
				status = WORKING;
      		}
      		break;
			
    	case 1:																						// front cradle empty ?
			if (status == WORKING){
				tft.setTextColor(BACKGROUND);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal0);	
	  
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal1);
				status = WAITING;
				Serial.println("wait for empty front cradle");
				tft.fillRect(but_sub[0].xpos, but_sub[0].ypos,90 , 50, BACKGROUND);					// erase button "back" 	

			}
      		if ( checkPressed()==true and checkButton(but_sub[1],1) ) {								// check if button was pressed & GO
				tft.setTextColor(GREEN);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGtara);
				delay(500);
				step++;
				status = WORKING;
				cellEmpty = frontCell.read_average(CELL_SAMPLE_ITERATIONS * numMeasurements);		// read empty front cell & built average
				Serial.println("front tare OK");
      		}
      		break;
    	case 2:																						// front cradle loaded ?
			if (status == WORKING){
				tft.setTextColor(BACKGROUND);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal1);	 
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGtara);	 
	  
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal1b);
				Serial.println("wait for loaded front cradle");
				status = WAITING;
			}
      		if ( checkPressed()==true and checkButton(but_sub[1],1)) {								// check if button was pressed & GO
				tft.setTextColor(GREEN);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGcal1c);
				delay(500);
				status = WORKING;
				cellWeighted = frontCell.read_average(CELL_SAMPLE_ITERATIONS * numMeasurements);
				eepromValues.frontScale = (cellWeighted - cellEmpty) / eepromValues.calibrationWeight;
				//eepromValues.frontScale = (cellWeighted - cellEmpty) /calibrationWeight; // test only
				frontCell.set_scale(eepromValues.frontScale);
				Serial.println("front loaded OK");
				step++;
      		}

			break;
		
		case 3:																						// rear cradle empty ?
			if (status == WORKING){
				tft.setTextColor(BACKGROUND);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal1b);	
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGcal1c);
	  
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal2);
				status = WAITING;
				Serial.println("wait for empty rear cradle");
			}
      		if ( checkPressed()==true and checkButton(but_sub[1],1) ) {								// check if button was pressed & GO
				tft.setTextColor(GREEN);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGtara);
        		cellEmpty = rearCell.read_average(CELL_SAMPLE_ITERATIONS * numMeasurements);
				step++;
				status = WORKING;
				Serial.print(status);Serial.println("\t tare rear cradle OK");
				delay(500);
			}
			
      		break;
		case 4:																						// rear cradle loaded ?
			if (status == WORKING){
				tft.setTextColor(BACKGROUND);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal2);	 
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGtara);	 
				
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(xLine, yLine1 );
				tft.println(str_CGcal2b);
				status = WAITING;
				Serial.println("wait for loaded rear cradle");
				delay(500);
			}

      		if ( checkPressed()==true and checkButton(but_sub[1],1) ) {								// check if button was pressed & GO
				tft.setTextColor(GREEN);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGcal2c);
				status = WORKING;
				cellWeighted = rearCell.read_average(CELL_SAMPLE_ITERATIONS * numMeasurements);
        		eepromValues.rearScale = (cellWeighted - cellEmpty) / eepromValues.calibrationWeight;
//				eepromValues.rearScale = (cellWeighted - cellEmpty) /calibrationWeight; // test only
				rearCell.set_scale(eepromValues.rearScale);
				tft.setTextColor(BACKGROUND);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGcal2c);	 

				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(xLine, yLine3 );
				tft.println(str_CGsave);
				Eeprom::putCGValues();										// write all cg related config/calib values into eeprom
				delay(300);
				Serial.println("calibration finished");
				finished = true;
			}
			break;
		}						// end "switch"
  
 }								// end "while"
 return MAIN_ID;
}
#endif