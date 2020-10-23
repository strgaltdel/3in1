/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	cg conf.h															##
##  content:		configure cg related parameters										##
##  date:			30 Jan 2020															##
##  rev.:			0.7																	##
##  by strgaltdel																		##
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


#ifndef CG_conf_H
#define CG_conf_H


#include "includes_main.h"

	
/*************************************************************************************/
/*****************    	main function / display parameters           ************/
/*************************************************************************************/


int cg_conf()
{
// 	#############################################    definition /declaration  block     ####################################################

	/*****************    	struct for layout (columns, offsets etc..)           ************/
	typedef struct {
		int col1;			// left disp column (values)
		int col2;			// right disp column (values)
		int center;			// center column (values)
		int row1;			// center column (values)
		int row2;
		int row3;
		int xOffs1;
	} layDefl;
	layDefl layout;

	int numButtons = 4;							// number of buttons on this screen, starting with 0
	int i, handler;
	int confStatus = 1;
	bool update_buttons = true;
	bool update_screen = true;
	char strDummy[20];

	int textsize_std = 2;						// char size of text
	int textsize_sml = textsize_std-1;			// char size of text


	
												// float to char dummies (dtostrf)
												// used to "overwrite" old values on screen without filling area black
												// (performancewise):
												
	char old1[20], old2[20], old3[20], old4[20], old5[20], old6[20];
	char new1[20], new2[20], new3[20], new4[20], new5[20], new6[20];

	String newUnit = str_unitA;
	String oldUnit = str_unitB;


	dtostrf(eepromValues.spanDistance * unit_factorLN	,		5,0,new1);		// init parameter strings
	dtostrf(eepromValues.pegDistance * unit_factorLN	,		5,0,new2);
	dtostrf(eepromValues.calibrationWeight * unit_factorWG,	5,0,new3);
	
	
	if(eepromValues.metricImpOption==1) {		// check if app was started in imperial mode
		newUnit=str_unitB;
	 }

	
	
	int val_col1 = tWidth * 0.01;				// left disp column (values)
	int val_col2 = tWidth * 0.45;				// mid disp column (values)
	int val_col3 = tWidth * 0.75;				// right disp column (values)
	int val_center = tWidth* .12;				// center column (values)
	int val_rowH = tHeight*0.14;				// delta rows
	int val_row1 = tHeight* 0.3;				// start row1
	int val_row2 = val_row1 + val_rowH;
	int val_row3 = val_row2 + val_rowH;
	int val_row4 = val_row3 + val_rowH;
	int val_CR   = tHeight* 0.05;	
	int val_xOffs1 = 20;
	int val_yOffs1 = 10;
	
	// define buttons for this routine
	Button but_sub[5];						// declare buttons
	  but_sub[0] = {BUT_S, 	BUTCOLOUR, 	str_edit	,	BUTTXTCOLOUR, val_col3, val_row1 -val_yOffs1};					// span
	  but_sub[1] = {BUT_S, 	BUTCOLOUR, 	str_edit,		BUTTXTCOLOUR, val_col3, val_row2 -val_yOffs1};					// peg	  
	  but_sub[2] = {BUT_S, 	BUTCOLOUR, 	str_edit,		BUTTXTCOLOUR, val_col3, val_row3 -val_yOffs1};					// peg	
	  but_sub[3] = {BUT_S, 	BUTCOLOUR,	str_TOG_unit, 	BUTTXTCOLOUR, val_col3, val_row4 -val_yOffs1};					// unit
	  but_sub[4] = {BUT_XL, GREEN, 		str_BACK, 		BUTTXTCOLOUR, tWidth*0.32, tHeight * 0.83};			// back



										

	
  while (confStatus == 1)
  {
	handler = 99;			// init handler to accept new user inputs


	if (update_screen ) {	
																				// convert parameter strings
		if (eepromValues.metricImpOption == 0) {									// metric format:												
			dtostrf(eepromValues.spanDistance *  unit_factorLN	,5,0,new1);			// - span distance		
			dtostrf(eepromValues.pegDistance  *  unit_factorLN	,5,0,new2);			// - peg  distance
			dtostrf(eepromValues.calibrationWeight*unit_factorWG,5,0,new3);			// - calibration weight
			}
		else {																		// imperial format:		
			dtostrf(eepromValues.spanDistance *  unit_factorLN	,5,1,new1);			// - span distance		
			dtostrf(eepromValues.pegDistance  *  unit_factorLN	,5,1,new2);			// - peg  distance
			dtostrf(eepromValues.calibrationWeight*unit_factorWG,5,1,new3);			// - calibration weight
			}
	
	
		// 	##################  static display  block   #############		
		tft.fillScreen(BACKGROUND);	
		tft.setTextSize(textsize_sml);				// subtext
		tft.setTextColor(LIGHTGREY);	
	
		tft.setCursor(val_col1, val_row1+val_CR);		
		tft.print(str_CG_txt1);tft.print(unit_lenght);
		tft.setCursor(val_col1, val_row2+val_CR);		
		tft.print(str_CG_txt1);tft.print(unit_lenght);
		tft.setCursor(val_col1, val_row3+val_CR);		
		tft.print(str_CG_weight2);tft.print(unit_weight);
	
													
		tft.setTextSize(textsize_std);				// standard	text
		tft.setTextColor(VALUECOLOUR);	
		
		tft.setCursor(val_col1, 2);		
		tft.print(str_CG_confTXT);
		
		tft.setCursor(val_col1, val_row1);			// print string "span"
		tft.print(str_CG_span);
		
		tft.setCursor(val_col1, val_row2);			// print string "peg"	
		tft.print(str_CG_peg);
	
		tft.setCursor(val_col1, val_row3);			// print string "reference weight"	
		tft.print(str_CG_weight);
		
		tft.setCursor(val_col1, val_row4);			// print string "unit"	
		tft.print(str_unit);
	
		tft.setTextColor(GREEN);					// print var "span"
		tft.setCursor(val_col2, val_row1);		
		tft.print(new1);
		
		tft.setCursor(val_col2, val_row2);			// print var "peg"
		tft.print(new2);
	
		tft.setCursor(val_col2, val_row3);			// print var "Cal weight"
		tft.print(new3);
		
		tft.setCursor(val_col2-(tWidth/8), val_row4);		// print var "unit"		
		tft.print(newUnit);
	



		// draw Buttons 						************
		for (i=0; i<=numButtons; i++) {
			DrawButton(but_sub[i]);
			}
		
		update_screen = false;
	

	}													// end of update screen
		
														//  *****    get touched Button:  **********		
	if ( checkPressed()==true){							// touch was pressed ?
		for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
			if (checkButton(but_sub[i],i)) {				
				handler = i;							// if button was pressed, return handler
			}
		}		
	}
	
	
    if (handler<99){
	  //Serial.print("handler   \t");Serial.println(handler);
    }
	// handler	/ user menue				******************
	switch (handler)
	{
		case 0:								// set span
			eepromValues.spanDistance = (int)(NumPad(str_CG_span, String(str_unit) + " in " + unit_lenght,3 ) / unit_factorLN + 0.5);	// get new value from numpad,
																																		// recalc in case of imperial
																																		// 3 = how many digits without decimals																											
			Eeprom::putCGValues();																										// write (cg related) value into eeprom
//			dtostrf(eepromValues.spanDistance*unit_factorLN	,5,0,new1);						// refresh formatted string
			update_screen = true;																										// flag screen for update
;			


			break;
		case 1:												// set peg
			eepromValues.pegDistance = (int)(NumPad(str_CG_peg,  String(str_unit) + " in " + unit_lenght,3) / unit_factorLN + 0.5);		// get new value from numpad, typecast in case of imperial
			Eeprom::putCGValues();																										// write (cg related) value into eeprom
			update_screen = true;																										// flag screen for update

			break;

		case 2:												// reference weight
			eepromValues.calibrationWeight = (int)(NumPad(str_CG_weight,  String(str_unit) + " in " + unit_weight,4) / unit_factorWG + 0.5);	// get new value from numpad, typecast in case of imperial
			Eeprom::putCGValues();																												// write (cg related) value into eeprom
			update_screen = true;		
			break;
			
		case 3:												// toggle unit
			if (newUnit == str_unitA){
				newUnit = str_unitB;
				oldUnit = str_unitA;
				unit_lenght = str_inch;
				unit_lenght2 = str_inch2;
				unit_factorLN = 1.0/25.4;					// imperial lenght (inch) >> mm/inch
				unit_weight = str_ounce;
				unit_weight2 = str_ounce2;
				unit_factorWG = 0.035274;					// imperial weight (ounce)
				eepromValues.metricImpOption = 1;			// not 0=imperial
				Eeprom::putCGValues();	
				}
			else {
				newUnit = str_unitA;
				oldUnit = str_unitB;
				unit_lenght = str_mm;
				unit_lenght2 = str_mm2;
				unit_factorLN = 1.00;						// metric lenght (mm)
				unit_weight = str_g;
				unit_weight2 = str_g2;
				unit_factorWG = 1.0;						// metric weight (g)
				eepromValues.metricImpOption = 0;			// 0=metric
				Eeprom::putCGValues();					
				}
			update_screen = true;
			break;

		case 4:												// return to main menu
			confStatus=0;
			delay(WAIT_RETURN);

	handler = 99;
	}

  }															// end of while loop
  
  return(4);
		
}



#endif
	


