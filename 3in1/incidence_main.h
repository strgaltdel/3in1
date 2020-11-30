/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	incidence_main.h													##
##  content:		read IMU incidence values from wing & elevator and and print data	##
##  date:			27 Nov 2020															##
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
	
**************************************************************************************
*/



#ifndef INCIDENCE_MAIN_H
#define INCIDENCE_MAIN_H


#include "includes_main.h"


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


#define PAIRING_RUNS1 800						// number iof loops used for pairing 2 imus



/*************************************************************************************/
/*****************    	selection which IMU should be calibrated           ***********/
/*************************************************************************************/
int select_inc_imu()
{
	int i;
	int handler = 99;

	// define buttons for this routine
	Button but_sub[5];
	int numButtons = 2;
	but_sub[0] = {BUT_XXL, 	BUTCOLOUR, 	str_imuEWD0, 		BUTTXTCOLOUR, 		tWidth* 0.1,	tHeight* 0.3};	
	but_sub[1] = {BUT_XXL, 	BUTCOLOUR, 	str_imuEWD1, 		BUTTXTCOLOUR, 		tWidth* 0.1,	tHeight* 0.5};	
	but_sub[2] = {BUT_XXL, 	BUTCOLOUR, 	str_imuEWD2, 		BUTTXTCOLOUR, 		tWidth* 0.1,	tHeight* 0.7};	
	

	
	
	tft.fillScreen(BACKGROUND);
	// draw Buttons 						************
	for (i=0; i<=numButtons; i++) {
		DrawButton(but_sub[i]);
	}
		
	while (handler  > 2)
	{	
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
		
	}

	return(handler);
}

	
	
	
/*************************************************************************************/
/*****************    	main function / display flap deflection           ************/
/*************************************************************************************/


void incidence()
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

	Button but_sub[5];							// declare buttons
	int numButtons = 3;							// number of buttons on this screen, starting with 0
	int i, handler, imu_number,imu2;
	int incStatus = 1;
	bool update_buttons = true;
	bool update_screen = true;
	char strDummy[20];

	int valueSize = 3;		// char size of measured values
	int diffSize = 5;		// char size of difference
	
	int up1;                        				// 1 -> up, 2 -> down
	int up2;                        				// 1 -> up, 2 -> down

	unsigned long startMillis = 0;  		// used to calculate lcd refresh
	unsigned long actualMillis  = 0;		// used to calculate lcd refresh



	/************************************ symbols ***********************************/
	uint8_t degree[8]  = {140, 146, 146, 140, 128, 128, 128, 128};      // °
	uint8_t arrowup[8]  = {132, 142, 132, 132, 132, 128, 128, 128};     // Arrow up
	uint8_t arrowdown[8]  = {128, 128, 128, 132, 132, 132, 142, 132,};  // Arrow down

	
												// float to char dummies (dtostrf)
												// used to "overwrite" old values on screen without filling area black
												// (performancewise):

	char old1[20] = " ";						// avoid foo fighters in first loop
	char old2[20] = " ";
	char old5[20] = " ";	
	
	char old3[20], old4[20], old6[20];
	char new1[20], new2[20], new3[20], new4[20], new5[20], new6[20];

	
	
	
	int val_col1 = tWidth * 0.05;			// left disp column (values)
	int val_col2 = tWidth * 0.50;			// right disp column (values)
	int val_center = tWidth* .12;			// center column (values)
	int val_row1 = tHeight* 0.07;			// center column (values)
	int val_row2 = tHeight* 0.17;
	int val_row3 = tHeight* 0.31;
	int val_xOffs1 = 20;
	
	// define buttons for this routine

	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_TARA	,	BUTTXTCOLOUR, tWidth*0.32, 	tHeight * 0.65};	
	but_sub[1] = {BUT_MID, 	BUTCOLOUR, 	str_TOG_Vtail,	BUTTXTCOLOUR, 1,			tHeight * 0.65};
	but_sub[2] = {BUT_S, 	RED, 		str_CALIBRATE, 	BUTTXTCOLOUR, 1, 			tHeight * 0.83  +15};
	but_sub[3] = {BUT_XL, 	GREEN, 		str_BACK, 		BUTTXTCOLOUR, tWidth*0.32, 	tHeight * 0.83};


	

	
// 	#############################################    user menu block     ##############################################################	

	imu2=IMU_eleS;												// start using standard tail imu2=3 >> Vtail: imu2=4											

		
	
  while (incStatus == 1)
  {
	handler = 99;			// init handler to accept new user inputs
// 	#############################################    read & calc block     ##############################################################


//	IMU_0 = IMUsensor.getfilteredAngleValues(3);		// get LP smoothed angle values from sensor:3
//	IMU_1 = IMUsensor.getfilteredAngleValues(4);		// get LP smoothed angle values from sensor:4
//	IMU_0 = IMUsensor.getDEMAfilteredAngleValues(3);	// get DEMA smoothed angle values from sensor:3
//	IMU_1 = IMUsensor.getDEMAfilteredAngleValues(4);	// get DEMA smoothed angle values from sensor:4
	IMU_0 = IMUsensor.getAngleValues(IMU_Wing);				// get unsmoothed angle values from sensor:3
	IMU_1 = IMUsensor.getAngleValues(imu2);				// get unsmoothed angle values from sensor:4/5
	
	
	
	//Serial.print(IMU_0.pitch);Serial.print("\t");Serial.println(IMU_1.pitch);


	if (update_screen ) {	
	// 	##################  static display  block   #############		
		tft.fillScreen(BACKGROUND);
				
		tft.setTextColor(VALUECOLOUR);									// IMU Angles
		tft.setTextSize(valueSize-2);
		tft.setCursor(val_col1+val_xOffs1 + 70, val_row1 -4 );		
		tft.println("o");
		tft.setCursor(val_col2+val_xOffs1 + 70, val_row1 -4);		
		tft.println("o");
	
		tft.setTextSize(valueSize-2);									
		tft.setTextColor(VALUECOLOUR);	
		tft.setCursor(2, tHeight * 0.6 );					
		tft.println(str_stabStd);
	 		
		// draw Buttons 						************
		for (i=0; i<=numButtons; i++) {
			DrawButton(but_sub[i]);
		}
		
		update_screen = false;
	}
	
	
	// check if you are in a time slot to read values and built average
	if((millis()-startMillis) >= (INC_REFRESHRATE - INC_AVERAGETIME)){
	// and now fill variables to built average over n readings
		avCounter++;
		avSum1 += IMU_0.roll;   
		avSum2 += IMU_1.roll;    
	}
 

		
	// check if new values should be displayed and refresh values
	if(millis()-startMillis >= INC_REFRESHRATE){
	
		angle1 = avSum1/avCounter;		// refresh angle values
		angle2 = avSum2/avCounter;

	
		// reset loop
		avCounter = 0;
		startMillis = millis();
		avSum1 = 0;
		avSum2 = 0;
	

		//angle1 = 48.12345;   //test values
		//angle2 = 7.24345; 
		
		if ((angle1 - relative1) > 0) {
			up1 = 1;                     //arrow ↑↑↑
		}
		else {
			up1 = 2;                     //arrow ↓↓↓
		}
		if ((angle2 - relative2) > 0) {
			up2 = 1;                     //arrow ↑↑↑
		}
		else {
			up2 = 2;                     //arrow ↓↓↓
		}
	

		/**********************   interim  Calulations     *****************************************/
//		rad1 = abs(angle1 - relative1) * 0.017453;
//		rad2 = abs(angle2 - relative2) * 0.017453;

	
// 	#############################################  display actual values block   #############################################################	

	
		dtostrf(angle1 - relative1,5,1,new1);						// angle wing
		dtostrf(angle2 - relative2,5,1,new2);						// angle stab

		dtostrf((angle1 - relative1) + (angle2 - relative2),5,1,new5);	//  angle between wing and elevator
	
		tft.setTextColor(VALUECOLOUR);
	
		tft.setTextSize(valueSize-2);								// print header
		tft.setCursor(val_col1, 1 );			
		tft.print("       ");tft.println(str_wing);
		tft.setCursor(val_col2, 1 );							
		tft.print("       ");tft.println(str_stab);
	

		tft.setTextSize(valueSize);									// delete old imu values
		tft.setTextColor(BACKGROUND);
		tft.setCursor(val_col1, val_row1 );							// angle wing
		tft.println(old1);											
		tft.setCursor(val_col2, val_row1 );							// angle stab / evelator
		tft.println(old2);
	

		tft.setTextColor(VALUECOLOUR);								// print new imu values
		//tft.setTextSize(valueSize);
		tft.setCursor(val_col1, val_row1 );						
		tft.println(new1);
		tft.setCursor(val_col2, val_row1 );							
		tft.println(new2);

	
		tft.setTextSize(diffSize);									// delete old diff angle
		tft.setTextColor(BACKGROUND);
		tft.setCursor(val_center, val_row3 );						// angle imu1-imu2
		tft.println(old5);

		if ((((angle1 - relative1) + (angle2 - relative2)) < 0.4) or (((angle1 - relative1) + (angle2 - relative2)) > 2.0)) {
			tft.setTextColor(RED);									// uncommon values: red
		}
		else {tft.setTextColor(GREEN);}	
		
		tft.setCursor(val_center, val_row3 );						// print new difference angle		
		tft.println(new5);						

		dtostrf(angle1 - relative1,5,1,old1);						// store actual values into "old"
		dtostrf(angle2 - relative2,5,1,old2);

		dtostrf((angle1 - relative1) + (angle2 - relative2),5,1,old5);	
	}																// end of display new values

	
		
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
		case 0:										// set both imu to zero
		    relative1=0; relative2=0;
		  for (int j=0; j<PAIRING_RUNS1; j++){
			relative1 = relative1 + angle1;				// zero-delta imu0
			relative2 = relative2 + angle2;				// zero-delta imu1
		  }
		    relative1 = relative1/PAIRING_RUNS1;
			relative2 = relative2/PAIRING_RUNS1;
			update_screen = true;
			break;
		case 1:								// toggle standard <> V-tail
			if (imu2==IMU_eleS){
				imu2=IMU_eleV;
				tft.setTextSize(valueSize-2);									
				tft.setTextColor(BACKGROUND);
				tft.setCursor(2, tHeight * 0.6 );					
				tft.println(str_stabStd);
				tft.setTextColor(VALUECOLOUR);				
				tft.setCursor(2, tHeight * 0.6  );							
				tft.println(str_stabV);				
				}
			else {
				imu2=IMU_eleS;
				tft.setTextSize(valueSize-2);									
				tft.setTextColor(BACKGROUND);
				tft.setCursor(2, tHeight * 0.6 );					
				tft.println(str_stabV);
				tft.setTextColor(VALUECOLOUR);				
				tft.setCursor(2, tHeight * 0.6  );							
				tft.println(str_stabStd);					
				
				}
			break;

		case 2:								// start calibration
			imu_number = select_inc_imu();
			if 		(imu_number==0)	{handler = deflection_cal(IMU_Wing);}
			else if (imu_number==1)	{handler = deflection_cal(IMU_eleS);}					
			else 					{handler = deflection_cal(IMU_eleV);}				
			update_screen = true;
			break;


		case 3:								// return to main menu
			delay(WAIT_RETURN);
			incStatus=0;
			#ifdef DEBUG
					Serial.println ("exit:  incidence main");
			#endif
			break;
	}

  }											// end of while loop
		
}

#endif
	


