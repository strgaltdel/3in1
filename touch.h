/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	touch.h																##
##  content:		touch related functions (init, checkpressed, checkArea )			##
##  date:			3 Feb 2020															##
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
	
***************************************************************************************/



#ifndef TOUCHTFT_H
#define TOUCHTFT_H

#include "includes_main.h"

// Pressure Defs
#define MINPRESSURE 3
#define MAXPRESSURE 2000

#define TOUCHPRESSURE 20


// Touch Coordinates
int Touch_X, Touch_Y, Touch_Z;



void tftinit() {

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(TFTMODE); 
 
  #ifndef TFT_SIZE
   #define TFT_SIZE
   tHeight = tft.height() - 1; //portrait;
   tWidth = tft.width();
  #endif
  //Background colour
  tft.fillScreen(BACKGROUND);

  
   #ifdef DEBUG 
    Serial.println(F("X Y size "));
    Serial.println(tWidth);
    Serial.println(tHeight);
   #endif
 }
 
 
  // check if panel was presses within area:
 bool checkPressed(){
	 
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //If sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // Touch coord.mapping
  
  if (TFTMODE == 0)				// look in lcd_config.h for actual mode
  {
  //portrait
	Touch_X = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
	Touch_Y = tft.height()- map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
  }
  else
  {
  // landscape
	Touch_X = tft.width() - map(p.y, TS_MAXY, TS_MINY, 0, tft.width());
	Touch_Y = tft.height()- map(p.x, TS_MAXX, TS_MINX, 0, tft.height());
  }
  Touch_Z = p.z;

  // now we have the coordinates and can check;
  // at the end: print touched coordinates	
  
  if (Touch_Z > 30) {
		return true;
	}
  return false;
 }
 
 // check if panel was presses within area:
 bool checkArea(int X1, int Y1, int XWidth, int YLenght) {
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //If sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // Touch coord.mapping
  
  if (TFTMODE == 0)				// look in lcd_config.h for actual mode
  {
  //portrait
	Touch_X = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
	Touch_Y = tft.height()- map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
  }
  else
  {
  // landscape
	Touch_X = tft.width() - map(p.y, TS_MAXY, TS_MINY, 0, tft.width());
	Touch_Y = tft.height()- map(p.x, TS_MAXX, TS_MINX, 0, tft.height());
  }
  Touch_Z = p.z;

  // now we have the coordinates and can check;
  // at the end: print touched coordinates	
  
  if (Touch_Z > 30) {
	  Serial.print("touch: \t");Serial.print(Touch_X);Serial.print(" \t");Serial.println(Touch_Y);
		if ( (Touch_X > X1) && (Touch_X < (X1 + XWidth)) && (Touch_Y > Y1) && (Touch_Y < (Y1 + YLenght))) {
			return true;
		}
  }
	
  return false;
}

#endif