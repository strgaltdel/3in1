/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	deflection_flap.h													##
##  content:		set flapwidt by using a "slider" and save value in EEprom			##
##  date:			02 Mar 2020															##
##  rev.:			1.0																##
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
	
**************************************************************************************/



#ifndef DEFLECTION_FLAP_H
#define DEFLECTION_FLAP_H

			
		
int setflap()
{
	Button but_sub[1];							// declare Buttons used in this part
	int numButtons = 0;							// number of buttons
	int i,j,yy, Y1, tmp1, tmp2;					// some vars 
	int handler = 99;							// handler for flow control
	int setflapStatus = 1;						// status of function, uses in while loop
	bool update_buttons = true;					// flag, used when buttons must be drawn
	bool firstrun = true;						// for initial calls 
	float flap;									// flapwidth
	char flapStr[20];							// flapwidth - formatted string
	
	// define buttons for this routine
	but_sub[0] = {BUT_XL, 	GREEN, 		str_OK3, 		BUTTXTCOLOUR, tWidth*0.02, 	tHeight -  button[BUT_XL][0]-3};
	
	float lineLenght = tHeight * 0.75;			// size of slider
	int lineColour = VALUECOLOUR;
	int lineThickness = 3;
	
	float Ystart = 5 + (tHeight-lineLenght)/2;	// y coord of graph
	float Xstart = tWidth * 0.8;				// X coord of graph
	
	float flapMin = 15* unit_factorLN;			// minimal flapsize (mm)
	float flapMax = 66* unit_factorLN;			// maximal flapsize
	float separator = 10* unit_factorLN;		// separators every x mm
	int separatorTH = 3;						// thickness od separators

	float markerstart = Xstart - tWidth*0.4;	// Xpos of marker
	float markerwidth = tWidth * 0.39;			// width of marker
	int markerthickness = 3;

	float markerold,marker ;					// values for last and actual position					
					

	String L_unit = " " + unit_lenght;							// mm <> inches
	flap = eepromValues.flapwidth* unit_factorLN;				// initial value (mm)

	tft.fillScreen(BACKGROUND);
				



	while (setflapStatus == 1)							// loop until "back" was pressed
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
		
		if (checkArea(markerstart-30,Ystart, markerwidth, lineLenght) || firstrun ) {
			// calc marker position of initial flap value
			if (firstrun){
				marker = ((flap - flapMin) * lineLenght/(flapMax - flapMin))  + Ystart ;}  // calc flap corresponding Y coordinates	
			else {
				marker = Touch_Y;}										// touched y coord
			// draw marker
			for (i=0; i < markerthickness; i++){
				tft.drawFastHLine(markerstart, markerold +i, markerwidth,  BACKGROUND);			// erase old marker
				tft.drawFastHLine(markerstart , marker+i, markerwidth, lineColour);				// draw new marker
			}
			
			// print new value		
			tmp1 = 25;																			// tmp X Coord displayValue
			tmp2 = 1;																			// tmp Y-Coord display Value			
			tft.fillRect(tmp1,tmp2,80,25,BACKGROUND);											// clear old area
			tft.setTextColor(lineColour);
			tft.setTextSize(3);
			tft.setCursor(tmp1,tmp2);
			
			if (eepromValues.metricImpOption == 0) {											// metric:
				flap = flapMin + ((marker-Ystart)/(lineLenght/(flapMax-flapMin)));			// calc flapvalue from touched y pos
				tft.print(flap,0);
				}
			else {																				// imperial
				flap = flapMin + ((marker-Ystart)/(lineLenght/(flapMax-flapMin)))+0.05;			// calc flapvalue from touched y pos
				tft.print(flap,1);
				}			

			tft.print(L_unit);

			markerold = marker;
			firstrun = false;
		}
		
		eepromValues.flapwidth = flap/unit_factorLN;
		
		// handler		
		switch (handler)
		{
			case 0:								// 0= button "back"
			
				setflapStatus=0;
				Eeprom::putFlapValues();		// save value
				delay(500);			
				break;
		}


		// draw Buttons etc	
		if (update_buttons ) {				// only if needed	
			// vertical line
			for (i=0; i < lineThickness; i++){
				tft.drawFastVLine(Xstart + i, Ystart, lineLenght, lineColour);} 
			// separators
			for (i=0; i <= (flapMax-flapMin)/separator; i++){
				yy =  Ystart + i*(lineLenght/((flapMax-flapMin)/separator));
				tft.drawFastHLine(Xstart, yy , tWidth * 0.05, lineColour); 			// sep lines
				tft.setCursor(Xstart + tWidth * 0.1, yy );							// set cursor for flap depth values
				tft.setTextSize(1);
				tft.setTextColor(lineColour);
				if (eepromValues.metricImpOption == 0) {							// sep values metric:
					tft.println(i*separator + flapMin,0);
					}
				else {																// imperial:
					tft.println(i*separator + flapMin,1);
					}
				}
				
			// Buttons
			for (i=0; i<=numButtons; i++) {
				DrawButton(but_sub[i]);
			}
			update_buttons = false;
		}
	}
	return 999;		// return handler = 999 to prevent "re jump" into function		
}


#endif