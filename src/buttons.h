/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	buttons.h															##
##  content:		definition of different button types & methods to get use of them	##
##  date:			2 Mar 2020															##
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
	
*************************************************************************************/


#ifndef BUTTONS_H
#define BUTTONS_H


#include "includes_main.h"
// Definitions



#define BUT_XXL  0
#define BUT_XL 	 1
#define BUT_MID  2
#define BUT_S 	 3
#define BUT_pad1 4
#define BUT_pad2 5



struct button
{
	int type;
	int colour;
	String label;
	int labelcolour;
	int xpos;
	int ypos;
};
typedef struct button 	Button;



// sizing:
// height, width, txt_size, XOffset, Yoffset txt, diam
int button[6][6] = {
	{50, 180, 3, 10, 15, 16},													// XXL
	{50,  90, 3, 10, 15, 16},													// XL
	{50,  60, 1,  3, 23, 16},													// NORMAL / MIDsize
	{30,  60, 1,  3, 13, 6},													// S
	{(tft.height()* 0.6/4 ) -4 , 	(tft.width()*0.8/3)     -2,	2, 10, 13, 5},	// numpad NORMAL
	{(tft.height()* 0.6/4 ) -4,		(tft.width()*0.8/3)*0.6 -2, 2, 2, 13, 5},	// numpad BacKspace 
	};


	
	
// draw simple, "rounded" button:
void DrawButton(Button but_tmp) {
	
	// draw rect
	tft.fillRoundRect(but_tmp.xpos+FRAMELINE,  but_tmp.ypos +FRAMELINE, button[but_tmp.type][1] -FRAMELINE*2, button[but_tmp.type][0] -FRAMELINE*2, button[but_tmp.type][5], but_tmp.colour);
	// draw frame if needed
	if (FRAMELINE >0){
		for (int i=0;i<FRAMELINE;i++){
			tft.drawRoundRect(but_tmp.xpos +i,  but_tmp.ypos +i, button[but_tmp.type][1] -i*2, button[but_tmp.type][0] - i*2, button[but_tmp.type][5], BUT_FRAMECOLOUR);
		}
	}
	// print label
	tft.setTextSize(button[but_tmp.type][2]);
	tft.setCursor(but_tmp.xpos +  button[but_tmp.type][3], but_tmp.ypos + button[but_tmp.type][4] );
	tft.setTextColor(but_tmp.labelcolour);
	tft.println(but_tmp.label);
}




// check if button was pressed (rev 0.1)
// like checkButt, but without update via ts.getPoint(),
// so this func evaluates last pressed area 
// .. checkButt() was deleted in final app

bool checkButton(Button but, int xx) {

	bool state = false;
	int j=0;

	#ifdef DEBUG_coord	
		tft.drawLine(0,		Touch_Y,	tWidth,	Touch_Y,	WHITE);
		tft.drawLine(Touch_X,	0,		Touch_X,	tHeight,	WHITE);
		Serial.print("checked Button :"),Serial.print(xx);
    #endif
	
	
	if ( (Touch_X > but.xpos) && (Touch_X < (but.xpos + button[but.type][1])) && (Touch_Y > but.ypos) && (Touch_Y < (but.ypos + button[but.type][0])) ) {
		state = true;
	}
	
	#ifdef PRINT_TOUCH_COORD	
		Serial.print("TOUCH XYZ"),Serial.print("\t"),Serial.print(Touch_X);Serial.print("\t");Serial.print(Touch_Y);Serial.print("\t");Serial.println(Touch_Z);	
	#endif
	return state;
}




#endif