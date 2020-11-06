/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	numpad.h															##
##  content:		numpad to enter custom values like distances, reference weight ..	##
##  date:			29 Feb 2020															##
##  rev.:			0.9																##
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
	
***************************************************************************************/


#ifndef NumPad_H
#define NumPad_H

#include "includes_main.h"


  #ifndef TFT_SIZE
   #define TFT_SIZE
   tHeight = tft.height() - 1; //portrait;
   tWidth = tft.width();
  #endif

float NumPad(String header, String subheader, int maxpos )
{
  //(initial x,initial y,width,height,color)
  
  int rowh = (tft.height()* 0.6/4 );
  double row1 = tft.height()* 0.4;
  double row2 = row1 + rowh;
  double row3 = row2 + rowh;
  double row4 = row3 + rowh;
  
  int colw = (tft.width()*0.8/3);
  double col1 = tft.width()* 0.1;
  double col2 = col1 + colw;
  double col3 = col2 + colw;

  bool update_screen = true;
  int numButtons = 12;							// number of buttons on this screen, starting with 0
  int i, handler;
  int numStatus = 1;
  
  bool delaystatus = false;						// status of input delay
  int waitTime = 150;							// delay to prevent repeating
  
  float return_val =0;							// return value
  int digPos = 0;								// which digit position
  bool tenth = false;							// mode tenth on/off
  int inp;										// input number
  
  char string_now[10],string_old[10];
  int xPosString = tWidth  * 0.18;
  int yPosString = tHeight * 0.22;
  int sizeString = 4;
  bool isDecmode = false;
  int sizeHeader = 3;
  
 
  // declare buttons 
  Button numpad[14];
	numpad[0] = {BUT_pad1, 	BUTCOLOUR, 	" 0"	,	BUTTXTCOLOUR, col2, row4};	
	numpad[1] = {BUT_pad1, 	BUTCOLOUR, 	" 1"	,	BUTTXTCOLOUR, col1, row1};
	numpad[2] = {BUT_pad1, 	BUTCOLOUR, 	" 2"	,	BUTTXTCOLOUR, col2, row1};
	numpad[3] = {BUT_pad1, 	BUTCOLOUR, 	" 3"	,	BUTTXTCOLOUR, col3, row1};
	numpad[4] = {BUT_pad1, 	BUTCOLOUR, 	" 4"	,	BUTTXTCOLOUR, col1, row2};
	numpad[5] = {BUT_pad1, 	BUTCOLOUR, 	" 5"	,	BUTTXTCOLOUR, col2, row2};	
	numpad[6] = {BUT_pad1, 	BUTCOLOUR, 	" 6"	,	BUTTXTCOLOUR, col3, row2};	
	numpad[7] = {BUT_pad1, 	BUTCOLOUR, 	" 7"	,	BUTTXTCOLOUR, col1, row3};	
	numpad[8] = {BUT_pad1, 	BUTCOLOUR, 	" 8"	,	BUTTXTCOLOUR, col2, row3};
	numpad[9] = {BUT_pad1, 	BUTCOLOUR, 	" 9"	,	BUTTXTCOLOUR, col3, row3};	
	numpad[10]= {BUT_pad1, 	BUTCOLOUR, 	" ."	,	BUTTXTCOLOUR, col1, row4};	
	numpad[11]= {BUT_pad2, 	RED, 	  "Del"	,	BUTTXTCOLOUR, tWidth *0.78, yPosString *0.9};		// thin button
//	numpad[11]= {BUT_pad1, 	RED, 	  "Del"	,	BUTTXTCOLOUR, tWidth *0.72, yPosString *0.9};		// normal button
	numpad[12]= {BUT_pad1, 	GREEN, 	 "SAVE" ,	BUTTXTCOLOUR, col3, row4};



	dtostrf(return_val,maxpos,0,string_now);
	strcpy(string_old,string_now);
			
			
	tft.fillScreen(BACKGROUND);	
	
	tft.setTextColor(VALUECOLOUR);
	tft.setCursor(tWidth*0.2, 1 );
	tft.setTextSize(sizeHeader);
	tft.println(header);
	tft.setTextSize(sizeHeader - 1);
	tft.setCursor(tWidth*0.2, tHeight* 0.09 );
	tft.println(subheader);
	
	tft.setTextSize(sizeString);
	
	// draw Buttons 						************
	for (i=0; i<=numButtons; i++) {
			DrawButton(numpad[i]);
		}
	

	
  while (numStatus == 1)
  {
	handler = 99;			// init handler to accept new user inputs

	if (update_screen ) {	
	// 	##################  refresh screen   #############	
	
		tft.setTextSize(sizeString);
		tft.setTextColor(BACKGROUND);
		tft.setCursor(xPosString, yPosString );
		tft.println(string_old);
		tft.setTextColor(GREEN);
		tft.setCursor(xPosString, yPosString );
		tft.println(string_now);
		update_screen = false;
		
		}
	if (delaystatus){
		delay(waitTime);
		delaystatus = false;	
	}


														//  *****    get touched Button:  **********		
	if ( checkPressed()==true){							// touch was pressed ?
		for (i=0; i<=numButtons; i++) {					// if yes, check if it was a button
			if (checkButton(numpad[i],i)) {				
				handler = i;							// if button was pressed, return handler
				#ifdef DEBUG
					Serial.print("pressed  ");Serial.println(numpad[i].label);
				#endif
			}
		}		
	}
	
	
	// handler	/ user menue				******************
	
	switch (handler)
	{
		case 0:								
			digPos++;
			inp=0;
			update_screen = true;
			break;
			
		case 1:	
			digPos++;
			inp=1;
			update_screen = true;
			break;

		case 2:								
			digPos++;
			inp=2;
			update_screen = true;
			break;

		case 3:								
			digPos++;
			inp=3;
			update_screen = true;
			break;
			
		case 4:								
			digPos++;
			inp=4;
			update_screen = true;
			break;
			
		case 5:								
			digPos++;
			inp=5;
			update_screen = true;
			break;
			
		case 6:								
			digPos++;
			inp=6;
			update_screen = true;
			break;

		case 7:								
			digPos++;
			inp=7;
			update_screen = true;
			break;

		case 8:								
			digPos++;
			inp=8;
			update_screen = true;
			break;
			
		case 9:	
			digPos++;
			inp=9;
			update_screen = true;
			break;	
			
		case 10:								
			tenth = true;
			break;
			
		case 11:								
			if (tenth == false and digPos > 0){						// not on tenth position, not start position, so delete last digit 	
					return_val = (int)return_val/10; 
					digPos--;
			  }

			else {
				return_val = (int)return_val * 1.0;
				tenth = false;
//				digPos--;
			}	
			delay(100);
			update_screen = true;
			#ifdef DEBUG
				Serial.println("DEL\t");Serial.print(digPos);Serial.print("\t");Serial.println(return_val);
			#endif
			break;
		case 12:								
			numStatus=0;
			delay(WAIT_RETURN);
			#ifdef DEBUG
				Serial.println("pressed Back from numpad");
			#endif
			break;
			
			
			
	}
	

	if(handler < 99 ){											// pad was touched

	  if (handler < 10 ){										// number was entered
		Serial.print("digPospre \t");Serial.println(digPos);
		if (digPos ==maxpos+1){									// auto-change to 1/10 input if third num was entered
			digPos =maxpos;
			tenth = true;		
		}
		
		if (tenth == false){
			return_val = return_val*10 + handler; }				// construct new value
		else {
			return_val = (int)return_val + 1.0*handler/10;  	// 1/10 handling
		}
		Serial.print("digPos \t");Serial.print(digPos);Serial.print("\thandler \t");Serial.print(digPos);Serial.print("\t");Serial.print("value \t");Serial.print(return_val);Serial.println(tenth);
	  }		
	  strcpy(string_old,string_now);								// update strings
	  if (tenth == true) {
			dtostrf(return_val,maxpos+2,1,string_now);}				// format incl. decimal
	  else {
		  dtostrf(return_val,maxpos,0,string_now);					// format excl. decimal
	  }
	  
	  if (handler < 11){
			delaystatus = true;									// prevent from repeat
		}
		
	}
  }											
  

	#ifdef DEBUG
		Serial.println("return from numpad");
	#endif

	return(return_val);		// return number
}


#endif