/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	deflection_main.h													##
##  content:		read IMU deflection values and display data on screen				##
##  date:			21.Nov 2020															##
##  rev.:			1.0																	##
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


#ifndef DEFLECTION_MAIN_H
#define DEFLECTION_MAIN_H


#include "includes_main.h"
#include "deflection_flap.h" 					// sub-menue setting flapwidth


#ifndef MAIN_VARS
#define MAIN_VARS
	float avSum1, avSum2, avCounter;				// sum up read values for building average
#endif



#ifndef IMUSENSOR
	#define IMUSENSOR
	AngleSens IMUsensor = AngleSens();			// init class IMUsensor
	Isensor IMU_0, IMU_1;						// init struct IMU  (Angle values sensor:0 & 1)
	#include "imu_cal.h"						// calibrate IMU										## app coding


// define vars used by angle calculation

float angle1;									// averaged value imu0
float angle2;									// averaged value imu1
float relative1;                  				// for zeroing
float relative2;                  				// for zeroing
float rad1;										// rad imu0
float rad2;										// rad imu1

#endif											

#define DIFF_OK	0.65							// angle difference between Flaps which is OK 

/*************************************************************************************/
/*****************    	selection which IMU should be calibrated           ***********/
/*************************************************************************************/

int select_imu()
{
	int i;
	int handler = 99;

	// define buttons for this routine
	Button but_sub2[5];
	int numButtons = 1;
	but_sub2[0] = {BUT_XXL, 	BUTCOLOUR, 	str_imuFlap0, 		BUTTXTCOLOUR, 		tWidth* 0.1,	tHeight* 0.4};	
	but_sub2[1] = {BUT_XXL, 	BUTCOLOUR, 	str_imuFlap1, 		BUTTXTCOLOUR, 		tWidth* 0.1,	tHeight* 0.7};	


	tft.fillScreen(BACKGROUND);
	// draw Buttons 						************
	for (i=0; i<=numButtons; i++) {
		DrawButton(but_sub2[i]);
	}
		
	while (handler  > numButtons)
	{	
		
														//  *****    get touched Button:  **********		
		if ( checkPressed()==true){							// touch was pressed ?
			for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
				if (checkButton(but_sub2[i],i)) {				
					handler = i;							// if button was pressed, return handler
					#ifdef DEBUG
						Serial.print("pressed  ");Serial.println(but_sub2[i].label);
					#endif
				}
			}		
		}

		
	}
	return(handler);
}

	
	
	
/*************************************************************************************/
/*****************    	main function / display flap deflection           ************/
/*************************************************************************************/


void deflection()
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

	
	
	Button but_sub[5];
	int numButtons = 4;
	int i, handler, imu_number;
	int deflStatus = 1;
	bool update_buttons = true;
	bool update_screen = true;
	char strDummy[20];

	int valueSize = 3;							// char size of measured values
	int diffSize = 5;							// char size of deltaValue

	float throw1;								// throw imu0
	float throw2;								// throw imu1
	int up1;                        			// 1 -> up, 2 -> down
	int up2;                        			// 1 -> up, 2 -> down


	unsigned long startMillis = 0;  			// used to calculate lcd refresh
	unsigned long actualMillis  = 0;			// used to calculate lcd refresh


												// float to char dummies (dtostrf)
												// used to "overwrite" old values on screen without filling area black
												// (performancewise):
	char old1[20], old2[20], old3[20], old4[20], old5[20], old6[20];
	char new1[20], new2[20], new3[20], new4[20], new5[20], new6[20];
	
	dtostrf(0,5,1,old1);						// definitions for 1st run
	dtostrf(0,5,1,old2);
	dtostrf(0,5,1,old3);
	dtostrf(0,5,1,old4);
	dtostrf(0,5,1,old5);


	
	int val_col1 = tWidth * 0.05;				// left disp column (values)
	int val_col2 = tWidth * 0.50;				// right disp column (values)
	int val_center = tWidth* .12;				// center column (values)
	int val_row1 = tHeight* 0.07;				// ....
	int val_row2 = tHeight* 0.17;
	int val_row3 = tHeight* 0.31;
	int val_xOffs1 = 20;
	
	// define buttons for this routine

	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_TARA, 		BUTTXTCOLOUR, tWidth*0.32, 	tHeight * 0.65};	
	but_sub[1] = {BUT_MID, 	BUTCOLOUR, 	str_TOG_degsek, BUTTXTCOLOUR, tWidth*0.75, 	tHeight * 0.83};
	but_sub[2] = {BUT_S, 	RED, 		str_CALIBRATE, 	BUTTXTCOLOUR, 1, 			tHeight * 0.83  +15};
	but_sub[3] = {BUT_MID, 	BUTCOLOUR, 	str_SET_flap, 	BUTTXTCOLOUR, 1,			tHeight * 0.65};
	but_sub[4] = {BUT_XL, 	GREEN, 		str_BACK, 		BUTTXTCOLOUR, tWidth*0.32, 	tHeight * 0.83};
	
	
	

	
// 	#############################################    user menu block     ##############################################################	

	

	
  while (deflStatus == 1)
  {
	handler = MAIN_ID;			// init handler to accept new user inputs
// 	#############################################    read & calc block     ##############################################################


//	IMU_0 = IMUsensor.getfilteredAngleValues(3);		// get LP smoothed angle values from sensor:3
//	IMU_1 = IMUsensor.getfilteredAngleValues(4);		// get LP smoothed angle values from sensor:4
//	IMU_0 = IMUsensor.getDEMAfilteredAngleValues(3);	// get DEMA smoothed angle values from sensor:3
//	IMU_1 = IMUsensor.getDEMAfilteredAngleValues(4);	// get DEMA smoothed angle values from sensor:4
	IMU_0 = IMUsensor.getAngleValues(3);				// get unsmoothed angle values from sensor:3
	IMU_1 = IMUsensor.getAngleValues(4);				// get unsmoothed angle values from sensor:4
	
	
	
	//Serial.print(IMU_0.pitch);Serial.print("\t");Serial.println(IMU_1.pitch);


	if (update_screen ) {	
	// 	##################  static display  block   #############		
		tft.fillScreen(BACKGROUND);
				
		tft.setTextColor(VALUECOLOUR);																			// angle unit "o"
		tft.setTextSize(valueSize-2);
		tft.setCursor(val_col1+val_xOffs1 + 70, val_row1 -4 );		
		tft.println("o");
		tft.setCursor(val_col2+val_xOffs1 + 70, val_row1 -4);		
		tft.println("o");
	
	
		tft.setTextSize(valueSize-2);																			// lenght unit metric/imperial
		tft.setTextColor(VALUECOLOUR);	
		tft.setCursor(val_col1+val_xOffs1 + 65, val_row2 + 8);		
		tft.println(unit_lenght);
		tft.setCursor(val_col2+val_xOffs1 + 65, val_row2 + 8 );		
		tft.println(unit_lenght);
	
		tft.setTextSize(valueSize-2);																			// disp throw/secant mode 
		tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);	

		if (eepromValues.TanSecOption==0){
			tft.setTextColor(BACKGROUND);
			tft.println("mod.SEC");
			tft.setTextColor(VALUECOLOUR);
			tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);
			tft.println("mod.THROW");
			tft.fillRect(but_sub[1].xpos, but_sub[1].ypos -tHeight*0.17, 30,40,BACKGROUND);
			if(SWAP_RB) {tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_tan2, 40, 30);}	// red<>blue swapperd
			else		{tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_tan , 40, 30);}
		}
		else {
			tft.setTextColor(BACKGROUND);
			tft.println("mod.THROW");
			tft.setTextColor(VALUECOLOUR);
			tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);
			tft.println("mod.SEC");
			tft.fillRect(but_sub[1].xpos, but_sub[1].ypos -tHeight*0.17, 30,40,BACKGROUND);
			tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_sec, 40, 30);
		}
		tft.setCursor(but_sub[3].xpos+tWidth*0.04 , but_sub[3].ypos -tHeight*0.04);			// disp flapdepth
		if (eepromValues.metricImpOption == 0) {											// metric:
			dtostrf(eepromValues.flapwidth * unit_factorLN,4,0, strDummy);
			}
		else {																				// imperial
			dtostrf(eepromValues.flapwidth * unit_factorLN,4,1, strDummy);
			}	
		tft.print(strDummy);
	 	tft.print(" ");tft.println(unit_lenght);

		
		// draw Buttons 						************
		for (i=0; i<=numButtons; i++) {
			DrawButton(but_sub[i]);
		}
		
		update_screen = false;
	}
	
	
	// check if you are in a time slot to read values and built average
	if((millis()-startMillis) >= (DEFL_REFRESHRATE - DEFL_AVERAGETIME)){
	// and now fill variables to built average over n readings
		avCounter++;
		avSum1 += IMU_0.roll;   		// roll or pitch depends on mechanical pcb orientation
		avSum2 += IMU_1.roll;    
		//avSum1 += IMU_0.pitch;   
		//avSum2 += IMU_1.pitch;    
	}
 

		
	// check if new values should be displayed and refresh values
	if(millis()-startMillis >= DEFL_REFRESHRATE){
	
		angle1 = avSum1/avCounter;		// refresh angle values
		angle2 = avSum2/avCounter;

	
		// reset loop
		avCounter = 0;
		startMillis = millis();
		avSum1 = 0;
		avSum2 = 0;


		/**********************   interim  Calulations     *****************************************/
		rad1 = abs(angle1 - relative1) * 0.017453;
		rad2 = abs(angle2 - relative2) * 0.017453;

		if(eepromValues.TanSecOption==0){
			throw1 = (eepromValues.flapwidth * sin(rad1));				// sin calc
			throw2 = (eepromValues.flapwidth * sin(rad2));
		}
		else
		{
			throw1 = (2 * eepromValues.flapwidth * sin(rad1 / 2));		//secant calc
			throw2 = (2 * eepromValues.flapwidth * sin(rad2 / 2));
		}


		/*******************************************************************************************/	
	
// 	#############################################  display actual values block   #############################################################	

	
		dtostrf(angle1 - relative1,5,1,new1);
		dtostrf(angle2 - relative2,5,1,new2);
		dtostrf(throw1 * unit_factorLN,5,1,new3);
		dtostrf(throw2 * unit_factorLN,5,1,new4);
		dtostrf(angle1 - relative1 - angle2 + relative2,5,1,new5);
	
		tft.setTextColor(VALUECOLOUR);
	
		tft.setTextSize(valueSize-2);
		tft.setCursor(val_col1, 1 );			
		tft.println("       Flap 0");
		tft.setCursor(val_col2, 1 );							
		tft.println("       Flap 1");
	

		tft.setTextSize(valueSize);	
		tft.setTextColor(BACKGROUND);
		tft.setCursor(val_col1, val_row1 );							// angle imu1
		tft.println(old1);											
		tft.setCursor(val_col2, val_row1 );							// angle imu2
		tft.println(old2);
	

		tft.setTextColor(VALUECOLOUR);
		//tft.setTextSize(valueSize);
		tft.setCursor(val_col1, val_row1 );						
		tft.println(new1);
		tft.setCursor(val_col2, val_row1 );							
		tft.println(new2);

		
		tft.setTextColor(BACKGROUND);	
		tft.setTextSize(valueSize-1);
		tft.setCursor(val_col1+val_xOffs1, val_row2 );				//  throw/secant (0)
		tft.println(old3);
		tft.setCursor(val_col2+val_xOffs1, val_row2 );				//  throw/secant (1)
		tft.println(old4);

		tft.setTextColor(VALUECOLOUR);	
		tft.setCursor(val_col1+val_xOffs1, val_row2 );		
		tft.println(new3);
		tft.setCursor(val_col2+val_xOffs1, val_row2 );			
		tft.println(new4);

	
	
		tft.setTextSize(diffSize);
		tft.setTextColor(BACKGROUND);
		tft.setCursor(val_center, val_row3 );						// angle imu1-imu2
		tft.println(old5);

		if (abs(angle1 - relative1 - angle2 + relative2) < DIFF_OK) {	// small angle diff ? >> green
			tft.setTextColor(GREEN);
		}
		else {tft.setTextColor(VALUECOLOUR);}
		
		tft.setCursor(val_center, val_row3 );						
		tft.println(new5);

		dtostrf(angle1 - relative1,5,1,old1);
		dtostrf(angle2 - relative2,5,1,old2);
		dtostrf(throw1 * unit_factorLN,5,1,old3);
		dtostrf(throw2 * unit_factorLN,5,1,old4);
		dtostrf(angle1 - relative1 - angle2 + relative2,5,1,old5);	
	}			// end of display new values

	
															//  *****    get touched Button:  **********		
		if ( checkPressed()==true){							// touch was pressed ?
			for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
				if (checkButton(but_sub[i],i)) {				
					handler = i;							// if button was pressed, return handler
					#ifdef DEBUG
						Serial.print("pressed  ");Serial.println(but_sub[i].label);
					#endif
				}
			}		
		}

	// handler	/ user menue				******************
	switch (handler)
	{
		case 0:								// set both imu to zero
			relative1 = angle1;				// zero-delta imu0
			relative2 = angle2;				// zero-delta imu1
			update_screen = true;
			break;
		case 1:								// toggle throw vs. sekant 
			tft.setTextSize(valueSize-2);
			tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);
			if (eepromValues.TanSecOption==0){
				tft.setTextColor(BACKGROUND);
				tft.println("mod.THROW");
				tft.setTextSize(valueSize-1);
				tft.setCursor(val_col1+val_xOffs1, val_row2 );							//  delete old throw display
				tft.println(old3);
				tft.setCursor(val_col2+val_xOffs1, val_row2 );							
				tft.println(old4);
					
				tft.setTextSize(valueSize-2);
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);
				tft.println("mod.SEC");
				tft.fillRect(but_sub[1].xpos, but_sub[1].ypos -tHeight*0.17, 30,40,BACKGROUND);
				tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_sec, 40, 30);
				eepromValues.TanSecOption=1;
				}
			else {
				tft.setTextColor(BACKGROUND);
				tft.println("mod.SEC");
				tft.setTextSize(valueSize-1);
				tft.setCursor(val_col1+val_xOffs1, val_row2 );							//  delete old secant display
				tft.println(old3);
				tft.setCursor(val_col2+val_xOffs1, val_row2 );							
				tft.println(old4);
					
				tft.setTextSize(valueSize-2);					
				tft.setTextColor(VALUECOLOUR);
				tft.setCursor(but_sub[1].xpos+tWidth*0.01, but_sub[1].ypos -tHeight*0.04);
				tft.println("mod.THROW");
				tft.fillRect(but_sub[1].xpos, but_sub[1].ypos -tHeight*0.17, 30,40,BACKGROUND);
				if(SWAP_RB) {tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_tan2, 40, 30);}	// red<>blue swapperd
				else		{tft.drawRGBBitmap(but_sub[1].xpos+tWidth*0.05, but_sub[1].ypos -tHeight*0.15, bmp_tan , 40, 30);}
				eepromValues.TanSecOption=0;
			}
			Eeprom::putFlapValues();													// save new "throw<>sekant flag"
			// update_buttons = true;
			delay(400);
			break;

		case 2:								// start calibration
			delay(WAIT_RETURN);
			imu_number = select_imu();
			handler = deflection_cal(imu_number+3);		
			update_screen = true;
			break;
		case 3:								// set flapdepth
			handler = setflap();
			/* debug eeprom values instead setflap:
				SprintIMUValues();
				Serial.print("offsX:  \t");Serial.println(ImuCAL[3].offsX);
			*/
			update_screen = true;
			break;

		case 4:								// return to main menu
			delay(WAIT_RETURN);
			deflStatus=0;
			break;
	}

  }											// end of while loop
		
}

#endif
	


