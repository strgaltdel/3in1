/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	cg_main.h															##
##  content:		main menue cg scale													##
##  date:			30 Jan 2020															##
##  rev.:			1.0																	##
##  by strgaltdel																			##
##########################################################################################


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



#ifndef CG_MAIN_H
#define CG_MAIN_H


#include "includes_main.h"

#include "cg_conf.h"								// menue cg configuration
#include "cg_calibrate.h"							// menue cg calibration									
#include "cg_raw.h"									// menue cg raw values		

	
	
/*************************************************************************************/
/*****************    	main function / display cg readings               ************/
/*************************************************************************************/


void cg_main()
{
// 	#############################################    definition /declaration  block     ####################################################

	/*****************    	struct for layout (columns, offsets etc..)           ************/
	typedef struct {
		int col1;								// left disp column (values)
		int col2;								// right disp column (values)
		int center;								// center column (values)
		int row1;								// center column (values)
		int row2;
		int row3;
		int xOffs1;
	} layDefl;
	layDefl layout;

	Button but_sub[5];							// declare buttons
	int numButtons = 4;							// number of buttons on this screen, starting with 0
	int i, handler;
	bool cgStatus = true;
	bool update_buttons = true;
	bool update_screen = true;
	char strDummy[20];




	unsigned long startMillis = 0;  			// used to calculate lcd refresh
	unsigned long actualMillis  = 0;			// used to calculate lcd refresh

	
												// float to char dummies (dtostrf)
												// used to "overwrite" old values on screen without filling area black
												// (performancewise):
												
	char old1[20] = " ";						// avoid foo fighters in first loop
	char old2[20] = " ";
	char old3[20], old4[20], old5[20], old6[20];
	char new1[20], new2[20], new3[20], new4[20], new5[20], new6[20];

	
	int valueSize  = 5;						// char size of cg
	int valueSize2 = 4;						// char size of weight
	
	int xCol1 = tWidth * 0.08;				// left disp column ( cg )
	int xCol2 = tWidth * 0.55;				// right disp column (weight )
	int xCol3 = tWidth * 0.4;				// center column (values)
	int yLine1 = tHeight * 0.07;			// row1 ...
	int yLine2 = tHeight * 0.2;
	int yLine3 = yLine2  + (tHeight*0.09);
	int val_xOffs1 = 20;
	int val_xOffs2 = 0;
	const int COLOUR2 = LIGHTGREY;
	
	#ifdef TESTRES							// could be used for more cg value resolution; not evaluated
	 valueSize  = 5;						// char size of cg
	 valueSize2 = 3;						// char size of weight
	 xCol1 = tWidth * 0.02;					// left disp column ( cg )
	 xCol2 = tWidth * 0.7;					// right disp column (weight )
	 val_xOffs2 = -30;
	#endif
	
	// define buttons for this screen
	// row1
	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_TARA	,	BUTTXTCOLOUR, tWidth*0.32, tHeight * 0.65};	
	but_sub[1] = {BUT_MID, 	BUTCOLOUR, 	str_CG_raw,		BUTTXTCOLOUR, 1,				tHeight * 0.65};
	//row2	
	but_sub[2] = {BUT_S, 	RED, 		str_CALIBRATE, 	BUTTXTCOLOUR, 1, 				tHeight * 0.83  +15};
	but_sub[3] = {BUT_XL, 	GREEN, 		str_BACK, 		BUTTXTCOLOUR, tWidth*0.32, tHeight * 0.83};
	but_sub[4] = {BUT_MID, 	BUTCOLOUR, 	str_CONFIG,		BUTTXTCOLOUR, tWidth*0.75, tHeight * 0.83};


	// some scale vars
	static MovingAverage avgFront(CELL_SAMPLE_ITERATIONS);				// averaged measurement
	static MovingAverage avgRear(CELL_SAMPLE_ITERATIONS);				// need https://github.com/sofian/MovingAverage
	
	float frontUnits;													// front raw measurement
	float rearUnits;													// rear raw measurement
	float totalWeight;													// sum: front + rear (raw)
	float totalForce;													// force: weight x distance
	float cg;															// center of gravity
	
	startMillis = millis();												// init refresh counter							

	frontCell.tare(CELL_SAMPLE_ITERATIONS);								// tare at start
	rearCell.tare(CELL_SAMPLE_ITERATIONS);	
	
  while (cgStatus == true)
  {
	handler = MAIN_ID;			// init handler to accept new user inputs
    //#############################################    read & calc block     ##############################################################

	// measurement & calculations
	avgFront.update(frontCell.get_units(1));
	avgRear.update(rearCell.get_units(1));
	frontUnits = roundf(avgFront.get());
	rearUnits = roundf(avgRear.get());
	totalWeight = frontUnits + rearUnits;
	totalForce = (frontUnits * eepromValues.pegDistance)  + (rearUnits *  (eepromValues.pegDistance + eepromValues.spanDistance));
	cg = totalForce / totalWeight;


	
	// convert values into string
	/*
	dtostrf(cg,3,0,new1);				// center of gravity
	dtostrf(totalWeight,4,0,new2);		// total weight
	*/
	#ifndef TESTRES
	  dtostrf(cg * unit_factorLN,3,0,new1);				// center of gravity, int
	#else
  	  dtostrf(cg * unit_factorLN,5,1,new1);				// center of gravity, float(1dec)
	#endif
	dtostrf(totalWeight * unit_factorWG,4,0,new2);		// total weight

	if (update_screen ) {	
	// 	##################  static display  block   #############		
		tft.fillScreen(BACKGROUND);		
		// draw Buttons 						************
		for (i=0; i<=numButtons; i++) {
			DrawButton(but_sub[i]);
		}
		
		tft.drawBitmap(tWidth*0.18,tHeight*0.04,	CG, 	40, 40, VALUECOLOUR);
		tft.drawBitmap(tWidth*0.75,tHeight*0.02,	LOAD, 	30, 50, COLOUR2);
		
		tft.setTextColor(VALUECOLOUR);					// print new values	
		tft.setTextSize(1);	
		tft.setCursor(xCol1+tWidth*0.1,yLine2 );		// unit cg
		tft.println(unit_lenght2); 
		tft.setCursor(xCol2+tWidth*0.2 + val_xOffs2,yLine2 );		// unit weight
		tft.println(unit_weight2); 

		update_screen = false;
	}
	
	
		
	// check if new values should be displayed and refresh values
	if(millis()-startMillis >= INC_REFRESHRATE){	
	
		tft.setTextColor(BACKGROUND);					// erase old values
		tft.setTextSize(valueSize);
		tft.setCursor(xCol1,yLine3 );
		tft.println(old1); 								// cg
		tft.setTextSize(valueSize2);
		tft.setCursor(xCol2,yLine3 );
		tft.println(old2); 								// weight
  
		tft.setTextColor(VALUECOLOUR2);					// print new values
		tft.setTextSize(valueSize);
		tft.setCursor(xCol1, yLine3);
		tft.println(new1);
		tft.setTextColor(COLOUR2);
		tft.setTextSize(valueSize2);
		tft.setCursor(xCol2, yLine3);
		tft.println(new2);
	
		// convert values into string
		#ifndef TESTRES
		  dtostrf(cg * unit_factorLN,3,0,old1);				// center of gravity
		#else
		  dtostrf(cg * unit_factorLN,5,1,old1);	
		#endif
		dtostrf(totalWeight * unit_factorWG,4,0,old2);		// total weight
		
		// reset loop
		startMillis = millis();
	}
	
// 	#############################################  display actual values block   #############################################################	

														//  *****    get touched Button:  **********		
	if ( checkPressed()==true){							// touch was pressed ?
		for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
			if (checkButton(but_sub[i],i)) {				
				handler = i;							// if button was pressed, return handler
			}
		}		
	}

	// handler	/ user menue				******************
	switch (handler)
	{
		case 0:								// set both cells to zero
			frontCell.tare(CELL_SAMPLE_ITERATIONS);
			rearCell.tare(CELL_SAMPLE_ITERATIONS);	
			update_screen = true;
			break;
		case 1:								// show weight on each cell
			handler = cg_Raw();	
			update_screen = true;
			break;

		case 2:								// start calibration
			handler = cg_Calibrate();	
			update_screen = true;
			break;

		case 3:								// return to main menu
			cgStatus=false;
			delay(WAIT_RETURN);
			break;
		case 4:								// go to configuration
			handler = cg_conf();		
			update_screen = true;
			break;
	}

  }											// end of while loop
		
}



#endif
	


