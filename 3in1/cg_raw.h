/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	cg_raw.h															##
##  content:		show RAW cell loads													##
##  date:			14 Feb 2020															##
##  rev.:			1.0																	##
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



#ifndef CG_RAW
#define CG_RAW

#include "includes_main.h"



int cg_Raw(){


  int i,j;
  
  bool waiting = false;
  bool working = true;
  bool status = waiting;
  
  static MovingAverage avgFront(CELL_SAMPLE_ITERATIONS);
  static MovingAverage avgRear(CELL_SAMPLE_ITERATIONS);
  
  int handler = MAIN_ID;
  bool finished = false;
  
  int textsize = 3;
  int sizeHeader = 2;
  int yLine1 = tHeight * 0.05;
  int yLine1b= tHeight * 0.13;
  int yLine2 = tHeight * 0.45;
  int yLine3 = yLine2 + (tHeight * 0.06);
  int xCol1  = tWidth * 0.1;
  int xCol2  = tWidth * 0.6;
  
  unsigned long startMillis = 0;  			// used to calculate lcd refresh
  unsigned long actualMillis  = 0;			// used to calculate lcd refresh

  
 // double front,rear,frontAvg, rearAvg;
  float front,rear,frontAvg, rearAvg;

  float frontUnits;													// front raw measurement
  float rearUnits;
  char new1[4],new2[4], old1[4],old2[4];
  dtostrf(0,4,0,old1);
  dtostrf(0,4,0,old2);
  
  
  // define but_subtons for this routine
  Button but_sub[5];
  int numButtons = 1;
	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_TARA, 		BUTTXTCOLOUR, tWidth * 0.05, 		tHeight*0.85};	
	but_sub[1] = {BUT_XL, 	GREEN, 		str_BACK, 		BUTTXTCOLOUR, tWidth * 0.55, 		tHeight*0.85};	

 tft.fillScreen(BACKGROUND);

	
 if (status == waiting)
  {
    status = working;
  }

  
  // start screen:
  DrawButton(but_sub[0]);					// print header
  DrawButton(but_sub[1]);
  tft.setTextColor(VALUECOLOUR);
  tft.setTextSize(sizeHeader);
  tft.setCursor(0,yLine1 );
  tft.println(str_CGraw0); 
  tft.setCursor(tWidth*0.37,yLine1b );
  tft.println(unit_weight2); 
  
  tft.setTextSize(textsize -2);				// print sub-header 
  tft.setCursor(xCol1, yLine2);
  tft.println(str_CGraw1);
  tft.setCursor(xCol2, yLine2);
  tft.println(str_CGraw2);
  
  tft.drawBitmap(tWidth*0.4,tHeight*0.25,LOADxl, 50, 80, LIGHTGREY);
  
  	startMillis = millis();												// init refresh counter							

  
  // loop steps:
 while (finished==false)
 {

	while(j<30 and handler>99){								// ensure user has enough time to press a button
		j++;
		if ( checkPressed()==true){							// touch was pressed ?
			for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
				if (checkButton(but_sub[i],i)) {				
					handler = i;							// if button was pressed, return handler
				}
			}		
		}
	}
	j=0;
 

	switch (handler) {
		case 0:												// TARA
			frontCell.tare(CELL_SAMPLE_ITERATIONS);
			avgFront.reset(frontCell.get_units(1));
			rearCell.tare(CELL_SAMPLE_ITERATIONS);
			avgRear.reset(rearCell.get_units(1));
			handler = MAIN_ID;
      		break;
			
    	case 1:												// Exit
			finished = true;		
      		break;

	}
	
	// display data
	
	front = frontCell.get_units(1);					// average of 8 readings from the ADC minus tare weight divided by the SCALE parameter
	rear = rearCell.get_units(1);

	frontAvg 	= avgFront.update(front);			// .. feed moving average
	rearAvg		= avgRear.update(rear);
	frontUnits 	= roundf(avgFront.get());
	rearUnits	= roundf(avgRear.get());

	// check if new values should be displayed and refresh values
	if(millis()-startMillis >= INC_REFRESHRATE){	
	
		dtostrf(frontUnits * unit_factorWG,4,0,new1);			// weight front
		dtostrf(rearUnits  * unit_factorWG,4,0,new2);			// weight rear


		tft.setTextColor(BACKGROUND);							// erase old values
		tft.setTextSize(textsize);
		tft.setCursor(xCol1, yLine3);
		tft.println(old1);
		tft.setCursor(xCol2, yLine3);
		tft.println(old2);
	
		tft.setTextColor(VALUECOLOUR);							// print new values
		tft.setCursor(xCol1, yLine3);
		tft.println(new1);
		tft.setCursor(xCol2, yLine3);
		tft.println(new2);

		dtostrf(frontUnits,4,0,old1);
		dtostrf(rearUnits,4,0,old2);
		
		// reset loop
		startMillis = millis();
	}

	
  }

 return MAIN_ID;
}
#endif