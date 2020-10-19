/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	includes_main.h														##
##  content:		all header files needed for app										##
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



#ifndef MASTERINCLUDES_H
#define MASTERINCLUDES_H

enum Status { WORKING, WAITING };
Status status;




#include <EEPROM.h>				// eeprom classes								https://www.arduino.cc/en/Reference/EEPROM

#include <Wire.h>				// imu communication  							https://www.i2cdevlib.com
#include <TouchScreen.h>		// touch support								https://github.com/adafruit/Adafruit_TouchScreen
#include <Adafruit_GFX.h>		// used for rounded buttons						https://github.com/adafruit/Adafruit-GFX-Library
#include <MCUFRIEND_kbv.h>		// flexible lcd support							https://www.arduinolibraries.info/libraries/mcufriend_kbv
#include <Filters.h> 			// Filter classes for smoothing IMU data		https://playground.arduino.cc/Code/Filters/														// get filter classes
#include <HX711.h>				// load cell support	 (ADA)
//#include <HX711_ADC.h>		// load cell support							https://github.com/olkal/HX711_ADC c					
#include <MovingAverage.h>		// simple way to build moving avg/used in cg	https://github.com/sofian/MovingAverage
#include "custom.h"				// definitions / some colour customizing		## app coding



MCUFRIEND_kbv tft;

HX711 frontCell;  				// HX711 constructor (ada)
HX711 rearCell; 
//HX711_ADC frontCell(51, 49); 	//HX711_ADC lib: pins front sensor (DOUT, PD_SCK)
//HX711_ADC rearCell(47, 45); 



#include "lcd_config.h"			// lcd config, colours and touch calibration data		## app coding
#include "eeprom.h"				// eeprom class (storing and reading specific values)   ## app coding

#include "language.h"			// language file										## app coding
#include "touch.h"				// some tft relevant functions							## app coding
#include "imu.h"				// IMU class											## app coding	
#include "bitmaps_3in1.h"	
#include "buttons.h"			// buttons class										## app coding

#include "screenServer.h"		// upload screens into processing client

#include "numpad.h"				// menue numpad											## app coding
#include "cg_main.h"			// menue cg scale										## app coding
#include "incidence_main.h"		// menue incidence meter								## app coding
#include "deflection_main.h"	// menue deflection										## app coding


#endif

