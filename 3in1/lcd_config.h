/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	lcd_config.h														##
##  content:		touch lcd related definitions (calibr.values,colours, pinning..)	##
##  date:			13 Nov 2020															##
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


#ifndef LCDCONFIG_H
#define LCDCONFIG_H

#define TFTTYPE 0

//#define TFTTYPE 0  	// 0= MCU68090,white dampener, 	black pins	, "mcufriend" print, 		read-id =0x6809, no voltage reg
//#define TFTTYPE 1		// 1= ILI9329,black dampener, 	black pins 	, "mcufriend" print, 		read-id =0x0, no voltage reg, id 0xE300 ??
//#define TFTTYPE 2		// 2= ILI9341,					yellow pins	, "2.8 TFT shield" print,	read-id = 0x9341
//#define TFTTYPE 3		// 2= ILI9340,	2.4"				

// common shield pinnings (see mcufriend lib)
// S6D0154: YP=A1, XM=A2, YM=7, XP=6
// ST7783 : YP=A2, XM=A1, YM=6, XP=7
// ILI9320: YP=A2, XM=A3, YM=8, XP=9
// ILI9325: YP=A1, XM=A2, YM=7, XP=6
// ILI9325BG: YP=A2, XM=A1, YM=6, XP=7
// ILI9341: YP=A2, XM=A1, YM=7, XP=6
// ILI9488: YP=A1, XM=A2, YM=7, XP=6
// R65109V: YP=A2, XM=A1, YM=6, XP=7

// most mcufriend shields use these pins and Portrait mode:
// YP = A1;  // must be an analog pin, use "An" notation!
// XM = A2;  // must be an analog pin, use "An" notation!
// YM = 7;   // can be a digital pin
// XP = 6;   // can be a digital pin


#if TFTTYPE == 0
  // 2.8 lcd, blue pcb; "mcufriend", white dampener, 68090
  #define TFT_IDENTIFIER 0x6809		// touch panel id
  #define TFTMODE  0 				//0=portrait, 1 = landscape, 2 portrait rev, 3 landscape rev
  #define TFTCOLORINVERT 0			// invert colors
  #define SWAP_RB false					// define only if red and blue has to be swapped
  #define RESISTOR 270				// adopt for better pressure precision
  #define YP A2  					// must be an analog pin, use "An" notation!
  #define XM A1  					// must be an analog pin, use "An" notation!
  #define YM 6   					// can be a digital pin
  #define XP 7   					// can be a digital pin
  #define TOUCH_SWAPy false
  //SPI Communication
  #define LCD_CS A3
  #define LCD_CD A2
  #define LCD_WR A1
  #define LCD_RD A0
  #define LCD_RESET A4
  // calibration mins and max for raw data when touching edges of screen (Jan 2020)
  #define TS_MAXX 910
  #define TS_MINX 141
  #define TS_MAXY 906
  #define TS_MINY 130
#endif


#if TFTTYPE == 1
  //	2.8 lcd, blue pcb; "mcufriend" print, black dampener, ILI9329
  #define TFT_IDENTIFIER 0x9329		// touch panel id
  #define TFTMODE  0 				//0=portrait, 1 = landscape, 2....
  #define RESISTOR 260				// adopt for better pressure precision
  #define TFTCOLORINVERT 1		// invert colors
  #define SWAP_RB true					// define only if red and blue has to be swapped
  #define YP A2  					// must be an analog pin, use "An" notation!
  #define XM A1  					// must be an analog pin, use "An" notation!
  #define YM 6   					// can be a digital pin
  #define XP 7  					// can be a digital pin
  #define TOUCH_SWAPy false
  //SPI Communication
  #define LCD_CS A3
  #define LCD_CD A2
  #define LCD_WR A1
  #define LCD_RD A0
  #define LCD_RESET A4
  // calibration mins and max for raw data when touching edges of screen (Nov 2020)
  #define TS_MAXX 930
  #define TS_MINX 141
  #define TS_MAXY 955
  #define TS_MINY 157
#endif


#if TFTTYPE == 2
  //	2.8 lcd, chinese supplier, yellow pin header, "2.8 TFT shield"
  #define TFT_IDENTIFIER 0x9341	// touch panel id
  #define TFTMODE  0
  #define TFTCOLORINVERT 0		// invert colors
  #define SWAP_RB false					// define only if red and blue has to be swapped
  #define RESISTOR 285
  #define YP A3  
  #define XM A2
  #define YM 9 
  #define XP 8 
  #define TOUCH_SWAPy false
  //SPI Communication
  #define LCD_CS A3
  #define LCD_CD A2
  #define LCD_WR A1
  #define LCD_RD A0
  #define LCD_RESET A4
  // calibration mins and max for raw data when touching edges of screen (Feb 2020)
  #define TS_MAXX 912
  #define TS_MINX 140
  #define TS_MAXY 908
  #define TS_MINY 132
#endif


#if TFTTYPE == 3
  //	2.4 lcd, red pcb, ILI9340  //not tested 
  #define TFT_IDENTIFIER 0x9340
  #define TFTMODE  0
  #define RESISTOR 270
  #define TFTCOLORINVERT 0		// invert colors
  #define SWAP_RB false					// define only if red and blue has to be swapped
  #define YP A2  // Touch pinning
  #define XM A1 
  #define YM 6 
  #define XP 7 
  #define TOUCH_SWAPy true
						
  #define TS_MAXX 935	// calibration (Nov 2020)
  #define TS_MINX 180
  #define TS_MAXY 930
  #define TS_MINY 159
#endif

#define BLACK   0x0000
#define WHITE   0xFFFF					// standard text
#define GREEN   0x07E0					// green button	, "O.K. values"
#define RGB(r, g, b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))

#if SWAP_RB == false
#define RED     0xF800					// red button;  "not O.K. values"
#define BLUE    0x001F
#define GREY    0xCE79
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
// RGB 
#define DARKGREY  RGB(64, 64, 64)
#define TURQUOISE RGB(0, 128, 128)
#define PINK      RGB(255, 128, 192)
#define LIGHTBLUE RGB(215, 245, 255)
#define LIGHTGREY RGB(230, 230, 230)	// standard button


#else
// some displays show red and blue swapped, so this is the corresponding colour definitiion

  #define RED     	0x001F
  #define BLUE    	0xF800
  #define GREY  	RGB(118, 118, 118)
  #define CYAN		RGB(255, 255, 0)
  #define MAGENTA	RGB(255, 0, 255)
  #define YELLOW	RGB(0, 255, 255)
  #define DARKGREY  RGB(64, 64, 64)
  #define TURQUOISE RGB(128, 128, 0)
  #define PINK      RGB(192, 128, 255)
  #define LIGHTBLUE RGB(255, 245, 215)
  #define LIGHTGREY RGB(160, 160, 160)
#endif


// LCD Variables  

// For better pressure precision, we need to know the resistance ("RESISTOR")
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// Pins A2-A6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, RESISTOR);

// some tft vars
double tHeight, tWidth;


#endif