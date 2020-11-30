/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	custom.h															##
##  content:		customizing like colours, timings, debug status...					##
##  date:			25 Nov 2020															##
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



#ifndef CUST_H
#define CUST_H

// debug purpose
#define DEBUG 					// print debug infos
#define PROTO					// Prototype pcb

//#define DEBUG_coord 			// DRAW COORD
//#define PRINT_TOUCH_COORD		// print touched coordinates

bool introFlag = true;			// intro yes/no

//  ###############     global
#define READ_EEPROM true				// set true if reading IMU-CALIBRATION values from Eeprom ; else false
#define HEADERSIZE 2					// textsize of screen header


//  ###############     layout

#define BACKGROUND 		BLACK			// background colour
#define VALUECOLOUR 	WHITE			// datavalue colour
#define VALUECOLOUR2 	GREEN			// datavalue colour

#define BUTCOLOUR		LIGHTGREY		// button colour
#define BUTTXTCOLOUR	WHITE			// button text colour
#define BUT_FRAMECOLOUR WHITE			// button frame colour
#define FRAMELINE 		0				// button frame thickness (0=no frame)




//  ###############     deflection


#define DEFL_REFRESHRATE 900     		// routine deflection: 	lcd refresh rate (milliseconds)
#define DEFL_AVERAGETIME 895			// routine deflection: 	time to built arithm. average (ms)

//  ###############     incidence meter
#define INC_REFRESHRATE 1400     		// incidence meter: 	lcd refresh rate (milliseconds)
#define INC_AVERAGETIME 1395			// incidence meter: 	time to built arithm. average (ms)

#endif


// timings
#define WAIT_RETURN 50					// delay after pressing "back / return"



// load cells config
#define CELL_SAMPLE_ITERATIONS 20		// "one time" read


#ifdef PROTO
// prototype pcb
#define FRONT_CELL_DOUT 51
#define FRONT_CELL_SCK 49
#define REAR_CELL_DOUT 47
#define REAR_CELL_SCK 45
#endif

#ifndef PROTO
#define FRONT_CELL_DOUT 47
#define FRONT_CELL_SCK 45
#define REAR_CELL_DOUT 43
#define REAR_CELL_SCK 41
#endif

String unit_lenght = "mm";
String unit_lenght2 = " (mm)";
float unit_factorLN = 1.0;
	
String unit_weight = "g";
String unit_weight2 = " (g)";			// please keep same as in languiage file str_g2
float unit_factorWG = 1.0;