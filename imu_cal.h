/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	imu_cal.h															##
##  content:		calibration of imu													##
##  date:			12 Jan 2020															##
##  rev.:			0.9																	##
##  by strgaltdel 																		##
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
	
**************************************************************************************/




#ifndef IMU_CAL_H
#define IMU_CAL_H

#include "includes_main.h"



/************** Offset calculation **********/
int calc_ImuOffset(int16_t max, int16_t min)
{
  int offset = 0;
  offset = int(0.5*(max+min));
  return(offset);
}



/************** Gain calculation **********/
float calc_ImuGain(int16_t max, int16_t min)
{
  float gain = 0.0;
  gain = float((0.5*(1.0*max-1.0*min)/IMUSCALE));		// yepp, without 1.0 wrong calc!
  return(gain);
}



/**************        overall calibration values calculation     **********/
/*    evaluate max and min values per axis and write back into structure   */
/*        				calculate gain & offset per axis          **********/

void getImuCalData(int IMUnum)															// input: IMU number
{
	RawAsensor	IMUraw;																	// struct to read RAW values from sensor
	int32_t avrgX, avrgY, avrgZ;
	int numReadings = 500;																// number of readings to built average
	
	
	for(int j=0; j<numReadings; j++ ){
		
		IMUraw = IMUsensor.getRawAccValues(IMUnum);										// get RAW values from sensor and sum up
		avrgX = avrgX + IMUraw.aX;
		avrgY = avrgY + IMUraw.aY;
		avrgZ = avrgZ + IMUraw.aZ;
	}
	avrgX= avrgX/numReadings;															// built average
	avrgY= avrgY/numReadings;	
	avrgZ= avrgZ/numReadings;


		
	if (avrgX < ImuCAL[IMUnum].minX) ImuCAL[IMUnum].minX = avrgX;						// evaluate Min/Max data
	if (avrgX > ImuCAL[IMUnum].maxX) ImuCAL[IMUnum].maxX = avrgX;
    
	if (avrgY < ImuCAL[IMUnum].minY) ImuCAL[IMUnum].minY = avrgY;	
	if (avrgY > ImuCAL[IMUnum].maxY) ImuCAL[IMUnum].maxY = avrgY;
  
	if (avrgZ < ImuCAL[IMUnum].minZ) ImuCAL[IMUnum].minZ = avrgZ;	
	if (avrgZ > ImuCAL[IMUnum].maxZ) ImuCAL[IMUnum].maxZ = avrgZ;	
		

	ImuCAL[IMUnum].offsX = calc_ImuOffset(ImuCAL[IMUnum].maxX,ImuCAL[IMUnum].minX);
	ImuCAL[IMUnum].offsY = calc_ImuOffset(ImuCAL[IMUnum].maxY,ImuCAL[IMUnum].minY);
	ImuCAL[IMUnum].offsZ = calc_ImuOffset(ImuCAL[IMUnum].maxZ,ImuCAL[IMUnum].minZ);
	
	ImuCAL[IMUnum].gainX = calc_ImuGain(ImuCAL[IMUnum].maxX,ImuCAL[IMUnum].minX);
	ImuCAL[IMUnum].gainY = calc_ImuGain(ImuCAL[IMUnum].maxY,ImuCAL[IMUnum].minY);
	ImuCAL[IMUnum].gainZ = calc_ImuGain(ImuCAL[IMUnum].maxZ,ImuCAL[IMUnum].minZ);
	
	return;
}



void dispImuOffs(int16_t imu_offs){
	if (abs(imu_offs) < IMUSCALE*0.08) tft.setTextColor(GREEN);
	else {tft.setTextColor(RED);}		
	tft.println(imu_offs);
	tft.setTextColor(VALUECOLOUR);
	return;	
}




void dispImuGain(float imu_gain){
	char strTmp[20];
	dtostrf(imu_gain,8,6,strTmp);
	
	if (abs(imu_gain -1) < 0.3) tft.setTextColor(GREEN);
	else tft.setTextColor(RED);		
	tft.println(strTmp);
	tft.setTextColor(VALUECOLOUR);
	return;	
}



void dispCalData(int num, int IMUnum){							// input: counter, IMU number
	
	int txtsize = 1;
	int col1 = 10;
	int col2 = int(tWidth * 0.5);
	int ystart = int(tHeight*0.2);
	int ymax = int(tHeight*0.5);
	int yoffs = 5;
	int yrow = (ymax-yoffs) / 7;
	char str_tmp[20];
	
	tft.fillRect(0, 0 , tWidth, ymax + ystart , BACKGROUND);	//clear display
	
	tft.setTextColor(VALUECOLOUR);
	tft.setTextSize(2);
	
	tft.setCursor(tWidth* 0.3, 2);
	tft.print("RUN:  ");
	tft.println(num);
	
	tft.setTextSize(txtsize);
	tft.setCursor(col1, ystart);
	tft.println("offset X:");
	tft.setCursor(col2, ystart);
	dispImuOffs(ImuCAL[IMUnum].offsX);
	
	tft.setCursor(col1, ystart + yrow);
	tft.println("offset Y:");
	tft.setCursor(col2, ystart + yrow);
	dispImuOffs(ImuCAL[IMUnum].offsY);
	
	tft.setCursor(col1, ystart + yrow*2);
	tft.println("offset Z:");
	tft.setCursor(col2, ystart + yrow*2);
	dispImuOffs(ImuCAL[IMUnum].offsZ);
	
	tft.setCursor(col1, ystart +yoffs + yrow*3);
	tft.println("  Gain X:");
	tft.setCursor(col2, ystart +yoffs + yrow*3);
	dispImuGain(ImuCAL[IMUnum].gainX);
	
	tft.setCursor(col1, ystart +yoffs + yrow*4);
	tft.println("  Gain Y:");
	tft.setCursor(col2, ystart +yoffs + yrow*4);
	dispImuGain(ImuCAL[IMUnum].gainY);
	
	tft.setCursor(col1, ystart +yoffs + yrow*5);
	tft.println("  Gain Z:");
	tft.setCursor(col2, ystart +yoffs+ yrow*5);
	dispImuGain(ImuCAL[IMUnum].gainZ);
}	
	


int deflection_cal(int imucal_num)
{
	Serial.print("calibrate IMU:  ");Serial.println(imucal_num);

	
	Button but_sub[5];
	int numButtons = 1;
	int i, handler2;
	int deflCalStatus = 1;
	bool update_buttons = true;
	int counter;
	
	// define buttons for this routine

	but_sub[0] = {BUT_XL, 	BUTCOLOUR, 	str_NEXT, 		BUTTXTCOLOUR, 10, 					tHeight*0.85};	
	but_sub[1] = {BUT_XL, 	GREEN	, 	str_SAVE, 		BUTTXTCOLOUR, (tWidth/2)+10, 	tHeight*0.85};

	ImuCAL[imucal_num].minX = 0;															// init Xtreme Values
	ImuCAL[imucal_num].maxX	= 0;
	ImuCAL[imucal_num].minY	= 0;
	ImuCAL[imucal_num].maxY	= 0;
	ImuCAL[imucal_num].minZ	= 0;
	ImuCAL[imucal_num].maxZ	= 0;	
	
	
	while (deflCalStatus == 1)
	{
	handler2 = MAIN_ID;									// reset handler

		
		
														//  *****    get touched Button:  **********		
	if ( checkPressed()==true){							// touch was pressed ?
		for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
			if (checkButton(but_sub[i],i)) {				
				handler2 = i;							// if button was pressed, return handler
				#ifdef DEBUG
					Serial.print("pressed  ");Serial.println(but_sub[i].label);
				#endif
			}
		}		
	}
		// handler				*************
		switch (handler2)
		{
			case 0:				// go to next measurement
				counter++;
				getImuCalData(imucal_num);
				dispCalData(counter,imucal_num);
				delay(WAIT_RETURN);				
				handler2 = 999;

				break;
			case 1:				// save values to eeprom and exit
				deflCalStatus=0;
				Eeprom::putIMUValues(ImuCAL[imucal_num],imucal_num);
				delay(WAIT_RETURN);
				handler2 = 999;
				break;	
		}
		
		// draw Buttons 		************
		if (update_buttons ) {
			tft.fillScreen(BACKGROUND);
			dispCalData(0,0);
			
			tft.setTextSize(2);
			tft.setCursor(tWidth*0.2, tHeight*0.75);
			tft.print("IMU: ");
			switch (imucal_num)
			{
				case 0:
					tft.print(str_imuEWD0);
					break;
				case 1:
					tft.print(str_imuEWD1);
					break;
				case 2:
					tft.print(str_imuEWD2);
					break;
				case 3:
					tft.print(str_imuFlap0);
					break;
				case 4:
					tft.print(str_imuFlap1);
					break;
			}

			
			
			for (i=0; i<=numButtons; i++) {
				DrawButton(but_sub[i]);
			}
			update_buttons = false;
		}
	}
	return MAIN_ID;		// return handler = 999 to prevent "re jump" into function
}


#endif