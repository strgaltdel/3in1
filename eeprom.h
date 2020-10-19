/* ***************************************************************************************
this code is part of "3in1.ino", an app to measure cg, incidence & deflection of rc models

##########################################################################################
##																						##
##  header file: 	eeprom.h															##
##  content:		save customized values into EEprom; define std values &addresses..	##
##  date:			20 Feb 2020															##
##  rev.:			0.8																	##
##  by strgaltdel with special thanks to ds beach, code was inspired from his work		##
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

methods:

getValues		()
putALLValues	(EepromValues eepromValues);
putCGValues		();
putIMUValues	(ImuCalValues IMUtmp, int imuTarget)
getIMUValues	(int imuTarget)
putFlapValues	();


*/
#ifndef EEPROM_H
#define EEPROM_H
 


#define FRONT_SCALE_ADDRESS 		0
#define REAR_SCALE_ADDRESS 			4
#define CALIBRATION_WEIGHT_ADDRESS 	8
#define SPAN_DISTANCE_ADDRESS 		12
#define PEG_DISTANCE_ADDRESS 		16
#define METRIC_IMP_OPTION_ADDRESS   20

#define FLAPWIDTH_ADDRESS 			24
#define RESERVED_OPTION_ADDRESS 	28
#define TAN_SEC_OPTION_ADDRESS 	    30

/*
... some bytes reserved for later use
*/

// **** IMU calibration *****
#define IMU1_OFFSX_ADDRESS 62
#define IMU1_OFFSY_ADDRESS 64
#define IMU1_OFFSZ_ADDRESS 66
#define IMU1_GAINX_ADDRESS 68
#define IMU1_GAINY_ADDRESS 72
#define IMU1_GAINZ_ADDRESS 76

#define IMU2_OFFSX_ADDRESS 80
#define IMU2_OFFSY_ADDRESS 82
#define IMU2_OFFSZ_ADDRESS 84
#define IMU2_GAINX_ADDRESS 86
#define IMU2_GAINY_ADDRESS 90
#define IMU2_GAINZ_ADDRESS 94

#define IMU3_OFFSX_ADDRESS 98
#define IMU3_OFFSY_ADDRESS 100
#define IMU3_OFFSZ_ADDRESS 102
#define IMU3_GAINX_ADDRESS 104
#define IMU3_GAINY_ADDRESS 108
#define IMU3_GAINZ_ADDRESS 112

#define IMU4_OFFSX_ADDRESS 116
#define IMU4_OFFSY_ADDRESS 118
#define IMU4_OFFSZ_ADDRESS 120
#define IMU4_GAINX_ADDRESS 122
#define IMU4_GAINY_ADDRESS 126
#define IMU4_GAINZ_ADDRESS 130

#define IMU5_OFFSX_ADDRESS 134
#define IMU5_OFFSY_ADDRESS 136
#define IMU5_OFFSZ_ADDRESS 138
#define IMU5_GAINX_ADDRESS 140
#define IMU5_GAINY_ADDRESS 144
#define IMU5_GAINZ_ADDRESS 148


/*****************   struct for IMU calibration values      ************/
typedef struct {
   int offsX;
   int offsY;
   int offsZ;
   float gainX;
   float gainY;
   float gainZ;
   int16_t maxX;
   int16_t minX;
   int16_t maxY;
   int16_t minY;
   int16_t maxZ;
   int16_t minZ;
} ImuCalValues;
ImuCalValues ImuCAL[5];




struct EepromValues {
  float frontScale;
  float rearScale;
  int 	calibrationWeight;
  int	spanDistance; 					// distance between pivot points
  int 	pegDistance; 					// leading edge peg to first pivot point
  int 	metricImpOption;				// metric vs imperial units
  float flapwidth; 						// widt of flap, needed for sec. meas.
  int   reservedOption;
  int 	IMU1_OFFSX;
  int 	IMU1_OFFSY;
  int 	IMU1_OFFSZ;
  float IMU1_GAINX;
  float IMU1_GAINY;
  float IMU1_GAINZ;
  int 	IMU2_OFFSX;
  int 	IMU2_OFFSY;
  int 	IMU2_OFFSZ;
  float IMU2_GAINX;
  float IMU2_GAINY;
  float IMU2_GAINZ;
  int 	IMU3_OFFSX;
  int 	IMU3_OFFSY;
  int 	IMU3_OFFSZ;
  float IMU3_GAINX;
  float IMU3_GAINY;
  float IMU3_GAINZ;
  int 	IMU4_OFFSX;
  int 	IMU4_OFFSY;
  int 	IMU4_OFFSZ;
  float IMU4_GAINX;
  float IMU4_GAINY;
  float IMU4_GAINZ;
  int 	IMU5_OFFSX;
  int 	IMU5_OFFSY;
  int	IMU5_OFFSZ;
  float IMU5_GAINX;
  float IMU5_GAINY;
  float IMU5_GAINZ;
  uint32_t TanSecOption;
};

EepromValues eepromValues;


class Eeprom {
  private:
    Eeprom() {};
  public:
    static EepromValues getValues();
	static void putALLValues(EepromValues eepromValues);
//    static void putCGValues(EepromValues eepromValues);
	static void putCGValues();
	static void putFlapValues();
	static void putIMUValues(ImuCalValues IMUtmp, int imuTarget);
	static void getIMUValues(int imuTarget);
};

#endif



#include "includes_main.h"

EepromValues Eeprom::getValues()
{


  //reset EEPROM if initiated
  if (false)
    {
    for (int i = 0;i<EEPROM.length();i++) EEPROM.write(i, 0xff);  
    eepromValues.frontScale = 1000.0;
    eepromValues.rearScale = 1000.0;
    eepromValues.calibrationWeight = 1000.0;
    eepromValues.spanDistance = 120.0;
    eepromValues.pegDistance = 30.0;
    eepromValues.metricImpOption = 0;
	eepromValues.flapwidth = 45;
	eepromValues.reservedOption = 0;
	eepromValues.IMU1_OFFSX = 0;
	eepromValues.IMU1_OFFSY = 0;
	eepromValues.IMU1_OFFSZ = 0;
	eepromValues.IMU1_GAINX = 1.0;
	eepromValues.IMU1_GAINY = 1.0;
	eepromValues.IMU1_GAINZ = 1.0;
	eepromValues.IMU2_OFFSX = 0;
	eepromValues.IMU2_OFFSY = 0;
	eepromValues.IMU2_OFFSZ = 0;
	eepromValues.IMU2_GAINX = 1.0;
	eepromValues.IMU2_GAINY = 1.0;
	eepromValues.IMU2_GAINZ = 1.0;
	eepromValues.IMU3_OFFSX = 0;
	eepromValues.IMU3_OFFSY = 0;
	eepromValues.IMU3_OFFSZ = 0;
	eepromValues.IMU3_GAINX = 1.0;
	eepromValues.IMU3_GAINY = 1.0;
	eepromValues.IMU3_GAINZ = 1.0;
	eepromValues.IMU4_OFFSX = 0;
	eepromValues.IMU4_OFFSY = 0;
	eepromValues.IMU4_OFFSZ = 0;
	eepromValues.IMU4_GAINX = 1.0;
	eepromValues.IMU4_GAINY = 1.0;
	eepromValues.IMU4_GAINZ = 1.0;
	eepromValues.IMU5_OFFSX = 0;
	eepromValues.IMU5_OFFSY = 0;
	eepromValues.IMU5_OFFSZ = 0;
	eepromValues.IMU5_GAINX = 1.0;
	eepromValues.IMU5_GAINY = 1.0;
	eepromValues.IMU5_GAINZ = 1.0;
	eepromValues.TanSecOption = 0;
    putALLValues(eepromValues);
    }
    else //read values from eeprom
    {
    EEPROM.get(FRONT_SCALE_ADDRESS,eepromValues.frontScale);
    EEPROM.get(REAR_SCALE_ADDRESS,eepromValues.rearScale);
    EEPROM.get(CALIBRATION_WEIGHT_ADDRESS, eepromValues.calibrationWeight);  
    EEPROM.get(SPAN_DISTANCE_ADDRESS, eepromValues.spanDistance);
    EEPROM.get(PEG_DISTANCE_ADDRESS, eepromValues.pegDistance);
    EEPROM.get(METRIC_IMP_OPTION_ADDRESS, eepromValues.metricImpOption);
	EEPROM.get(FLAPWIDTH_ADDRESS, eepromValues.flapwidth);
	EEPROM.get(RESERVED_OPTION_ADDRESS, eepromValues.reservedOption);
	EEPROM.get(IMU1_OFFSX_ADDRESS, eepromValues.IMU1_OFFSX);
	EEPROM.get(IMU1_OFFSY_ADDRESS, eepromValues.IMU1_OFFSY);
	EEPROM.get(IMU1_OFFSZ_ADDRESS, eepromValues.IMU1_OFFSZ);
	EEPROM.get(IMU1_GAINX_ADDRESS, eepromValues.IMU1_GAINX);
	EEPROM.get(IMU1_GAINY_ADDRESS, eepromValues.IMU1_GAINY);
	EEPROM.get(IMU1_GAINZ_ADDRESS, eepromValues.IMU1_GAINZ);
	EEPROM.get(IMU2_OFFSX_ADDRESS, eepromValues.IMU2_OFFSX);
	EEPROM.get(IMU2_OFFSY_ADDRESS, eepromValues.IMU2_OFFSY);
	EEPROM.get(IMU2_OFFSZ_ADDRESS, eepromValues.IMU2_OFFSZ);
	EEPROM.get(IMU2_GAINX_ADDRESS, eepromValues.IMU2_GAINX);
	EEPROM.get(IMU2_GAINY_ADDRESS, eepromValues.IMU2_GAINY);
	EEPROM.get(IMU2_GAINZ_ADDRESS, eepromValues.IMU2_GAINZ);
	EEPROM.get(IMU3_OFFSX_ADDRESS, eepromValues.IMU3_OFFSX);
	EEPROM.get(IMU3_OFFSY_ADDRESS, eepromValues.IMU3_OFFSY);
	EEPROM.get(IMU3_OFFSZ_ADDRESS, eepromValues.IMU3_OFFSZ);
	EEPROM.get(IMU3_GAINX_ADDRESS, eepromValues.IMU3_GAINX);
	EEPROM.get(IMU3_GAINY_ADDRESS, eepromValues.IMU3_GAINY);
	EEPROM.get(IMU3_GAINZ_ADDRESS, eepromValues.IMU3_GAINZ);
	EEPROM.get(IMU4_OFFSX_ADDRESS, eepromValues.IMU4_OFFSX);
	EEPROM.get(IMU4_OFFSY_ADDRESS, eepromValues.IMU4_OFFSY);
	EEPROM.get(IMU4_OFFSZ_ADDRESS, eepromValues.IMU4_OFFSZ);
	EEPROM.get(IMU4_GAINX_ADDRESS, eepromValues.IMU4_GAINX);
	EEPROM.get(IMU4_GAINY_ADDRESS, eepromValues.IMU4_GAINY);
	EEPROM.get(IMU4_GAINZ_ADDRESS, eepromValues.IMU4_GAINZ);
	EEPROM.get(IMU5_OFFSX_ADDRESS, eepromValues.IMU5_OFFSX);
	EEPROM.get(IMU5_OFFSY_ADDRESS, eepromValues.IMU5_OFFSY);
	EEPROM.get(IMU5_OFFSZ_ADDRESS, eepromValues.IMU5_OFFSZ);
	EEPROM.get(IMU5_GAINX_ADDRESS, eepromValues.IMU5_GAINX);
	EEPROM.get(IMU5_GAINY_ADDRESS, eepromValues.IMU5_GAINY);
	EEPROM.get(IMU5_GAINZ_ADDRESS, eepromValues.IMU5_GAINZ);
	EEPROM.get(TAN_SEC_OPTION_ADDRESS, eepromValues.TanSecOption);
	
    }
	
    if (isnan(eepromValues.frontScale) || (eepromValues.frontScale == 0.0)) { eepromValues.frontScale = 1000.0; }
    if (isnan(eepromValues.rearScale) || (eepromValues.rearScale == 0.0)) { eepromValues.rearScale = 1000.0; }
    if (isnan(eepromValues.calibrationWeight) || (eepromValues.calibrationWeight == 0.0)) { eepromValues.calibrationWeight = 1000.0; }
    if (isnan(eepromValues.spanDistance) || (eepromValues.spanDistance == 0.0)) { eepromValues.spanDistance = 120.0; }
    if (isnan(eepromValues.pegDistance) || (eepromValues.pegDistance == 0.0)) { eepromValues.pegDistance = 30.0; }
    if (isnan(eepromValues.metricImpOption) || eepromValues.metricImpOption > 2) { eepromValues.metricImpOption = 0; }
    if (isnan(eepromValues.flapwidth) || (eepromValues.flapwidth == 0.0)) { eepromValues.flapwidth = 45.0; }
	if (isnan(eepromValues.reservedOption) || eepromValues.reservedOption > 2) { eepromValues.reservedOption = 0; }
	if (isnan(eepromValues.TanSecOption) || eepromValues.TanSecOption > 2) { eepromValues.TanSecOption = 0; }
	
	if (isnan(eepromValues.IMU1_OFFSX)) { eepromValues.IMU1_OFFSX = 0; }
	if (isnan(eepromValues.IMU1_OFFSY)) { eepromValues.IMU1_OFFSY = 0; }
	if (isnan(eepromValues.IMU1_OFFSZ)) { eepromValues.IMU1_OFFSZ = 0; }	
	if (isnan(eepromValues.IMU1_GAINX) || eepromValues.IMU1_GAINX == 0.0) { eepromValues.IMU1_GAINX = 1.0; }
	if (isnan(eepromValues.IMU1_GAINY) || eepromValues.IMU1_GAINY == 0.0) { eepromValues.IMU1_GAINY = 1.0; }
	if (isnan(eepromValues.IMU1_GAINZ) || eepromValues.IMU1_GAINZ == 0.0) { eepromValues.IMU1_GAINZ = 1.0; }

	if (isnan(eepromValues.IMU2_OFFSX)) { eepromValues.IMU2_OFFSX = 0; }
	if (isnan(eepromValues.IMU2_OFFSY)) { eepromValues.IMU2_OFFSY = 0; }
	if (isnan(eepromValues.IMU2_OFFSZ)) { eepromValues.IMU2_OFFSZ = 0; }	
	if (isnan(eepromValues.IMU2_GAINX) || eepromValues.IMU2_GAINX == 0.0) { eepromValues.IMU2_GAINX = 1.0; }
	if (isnan(eepromValues.IMU2_GAINY) || eepromValues.IMU2_GAINY == 0.0) { eepromValues.IMU2_GAINY = 1.0; }
	if (isnan(eepromValues.IMU2_GAINZ) || eepromValues.IMU2_GAINZ == 0.0) { eepromValues.IMU2_GAINZ = 1.0; }

	if (isnan(eepromValues.IMU3_OFFSX)) { eepromValues.IMU3_OFFSX = 0; }
	if (isnan(eepromValues.IMU3_OFFSY)) { eepromValues.IMU3_OFFSY = 0; }
	if (isnan(eepromValues.IMU3_OFFSZ)) { eepromValues.IMU3_OFFSZ = 0; }	
	if (isnan(eepromValues.IMU3_GAINX) || eepromValues.IMU3_GAINX == 0.0) { eepromValues.IMU3_GAINX = 1.0; }
	if (isnan(eepromValues.IMU3_GAINY) || eepromValues.IMU3_GAINY == 0.0) { eepromValues.IMU3_GAINY = 1.0; }
	if (isnan(eepromValues.IMU3_GAINZ) || eepromValues.IMU3_GAINZ == 0.0) { eepromValues.IMU3_GAINZ = 1.0; }

	if (isnan(eepromValues.IMU4_OFFSX)) { eepromValues.IMU4_OFFSX = 0; }
	if (isnan(eepromValues.IMU4_OFFSY)) { eepromValues.IMU4_OFFSY = 0; }
	if (isnan(eepromValues.IMU4_OFFSZ)) { eepromValues.IMU4_OFFSZ = 0; }	
	if (isnan(eepromValues.IMU4_GAINX) || eepromValues.IMU4_GAINX == 0.0) { eepromValues.IMU4_GAINX = 1.0; }
	if (isnan(eepromValues.IMU4_GAINY) || eepromValues.IMU4_GAINY == 0.0) { eepromValues.IMU4_GAINY = 1.0; }
	if (isnan(eepromValues.IMU4_GAINZ) || eepromValues.IMU4_GAINZ == 0.0) { eepromValues.IMU4_GAINZ = 1.0; }

	if (isnan(eepromValues.IMU5_OFFSX)) { eepromValues.IMU5_OFFSX = 0; }
	if (isnan(eepromValues.IMU5_OFFSY)) { eepromValues.IMU5_OFFSY = 0; }
	if (isnan(eepromValues.IMU5_OFFSZ)) { eepromValues.IMU5_OFFSZ = 0; }	
	if (isnan(eepromValues.IMU5_GAINX) || eepromValues.IMU5_GAINX == 0.0) { eepromValues.IMU5_GAINX = 1.0; }
	if (isnan(eepromValues.IMU5_GAINY) || eepromValues.IMU5_GAINY == 0.0) { eepromValues.IMU5_GAINY = 1.0; }
	if (isnan(eepromValues.IMU5_GAINZ) || eepromValues.IMU5_GAINZ == 0.0) { eepromValues.IMU5_GAINZ = 1.0; }
    
	Serial.println();
	Serial.println("Reading EEPROM");
	Serial.println("--------------");
    Serial.println("Front scale: \t" + String(eepromValues.frontScale));
    Serial.println("Rear scale: \t" + String(eepromValues.rearScale));
    Serial.println("Cal. weight: \t" + String(eepromValues.calibrationWeight));
    Serial.print("Span distance: \t");Serial.println(eepromValues.spanDistance);
    Serial.println("Peg distance: \t" + String(eepromValues.pegDistance));
    Serial.println("G/ounces Opt:\t" + String(eepromValues.metricImpOption));
	Serial.println("flapwidth:\t"+ String(eepromValues.flapwidth));
    Serial.println("reserved Opt:\t"+ String(eepromValues.reservedOption));
	
	
	Serial.print("imu");Serial.print("\t\t");Serial.print("OffsX");Serial.print("\t");Serial.print("OffsY");Serial.print("\t");Serial.print("OffsZ");Serial.print("\t");Serial.print("GainX");Serial.print("\t");Serial.print("GainY");Serial.print("\t");Serial.println("GainZ");
	Serial.print("Wing");Serial.print("\t\t");Serial.print(eepromValues.IMU1_OFFSX);Serial.print("\t");Serial.print(eepromValues.IMU1_OFFSY);Serial.print("\t");Serial.print(eepromValues.IMU1_OFFSZ);Serial.print("\t");Serial.print(eepromValues.IMU1_GAINX);Serial.print("\t");Serial.print(eepromValues.IMU1_GAINY);Serial.print("\t");Serial.println(eepromValues.IMU1_GAINZ);
	Serial.print("Tail");Serial.print("\t\t");Serial.print(eepromValues.IMU2_OFFSX);Serial.print("\t");Serial.print(eepromValues.IMU2_OFFSY);Serial.print("\t");Serial.print(eepromValues.IMU2_OFFSZ);Serial.print("\t");Serial.print(eepromValues.IMU2_GAINX);Serial.print("\t");Serial.print(eepromValues.IMU2_GAINY);Serial.print("\t");Serial.println(eepromValues.IMU2_GAINZ);
	Serial.print("V-Tail");Serial.print("\t\t");Serial.print(eepromValues.IMU3_OFFSX);Serial.print("\t");Serial.print(eepromValues.IMU3_OFFSY);Serial.print("\t");Serial.print(eepromValues.IMU3_OFFSZ);Serial.print("\t");Serial.print(eepromValues.IMU3_GAINX);Serial.print("\t");Serial.print(eepromValues.IMU3_GAINY);Serial.print("\t");Serial.println(eepromValues.IMU3_GAINZ);
	Serial.print("Flap0");Serial.print("\t\t");Serial.print(eepromValues.IMU4_OFFSX);Serial.print("\t");Serial.print(eepromValues.IMU4_OFFSY);Serial.print("\t");Serial.print(eepromValues.IMU4_OFFSZ);Serial.print("\t");Serial.print(eepromValues.IMU4_GAINX);Serial.print("\t");Serial.print(eepromValues.IMU4_GAINY);Serial.print("\t");Serial.println(eepromValues.IMU4_GAINZ);
	Serial.print("Flap1");Serial.print("\t\t");Serial.print(eepromValues.IMU5_OFFSX);Serial.print("\t");Serial.print(eepromValues.IMU5_OFFSY);Serial.print("\t");Serial.print(eepromValues.IMU5_OFFSZ);Serial.print("\t");Serial.print(eepromValues.IMU5_GAINX);Serial.print("\t");Serial.print(eepromValues.IMU5_GAINY);Serial.print("\t");Serial.println(eepromValues.IMU5_GAINZ);

	
	
	
	//	Serial.print("Tail\t" +	String(eepromValues.IMU2_OFFSX) + "\t"+ String(eepromValues.IMU1_OFFSY) + "\t"+ String(eepromValues.IMU1_OFFSZ)  + "\t" +	String(eepromValues.IMU2_GAINX) + "\t"+ String(eepromValues.IMU1_GAINY) + "\t"+ String(eepromValues.IMU1_GAINZ));
	
	/*
	Serial.println("IMU1 (Wing):");
	Serial.println("X Offset: \t"+ String(eepromValues.IMU1_OFFSX));
	Serial.println("Y Offset: \t"+ String(eepromValues.IMU1_OFFSY));
	Serial.println("Z Offset: \t"+ String(eepromValues.IMU1_OFFSZ));
	Serial.println("X GAIN: \t"+ String(eepromValues.IMU1_GAINX));
	Serial.println("Y GAIN: \t"+ String(eepromValues.IMU1_GAINY));
	Serial.println("Z GAIN: \t"+ String(eepromValues.IMU1_GAINZ));
	Serial.println("IMU2 (Tail):");
	Serial.println("X Offset: \t"+ String(eepromValues.IMU2_OFFSX));
	Serial.println("Y Offset: \t"+ String(eepromValues.IMU2_OFFSY));
	Serial.println("Z Offset: \t"+ String(eepromValues.IMU2_OFFSZ));
	Serial.println("X GAIN: \t"+ String(eepromValues.IMU2_GAINX));
	Serial.println("Y GAIN: \t"+ String(eepromValues.IMU2_GAINY));
	Serial.println("Z GAIN: \t"+ String(eepromValues.IMU2_GAINZ));
	Serial.println("IMU3 (V-tail):");
	Serial.println("X Offset: \t"+ String(eepromValues.IMU3_OFFSX));
	Serial.println("Y Offset: \t"+ String(eepromValues.IMU3_OFFSY));
	Serial.println("Z Offset: \t"+ String(eepromValues.IMU3_OFFSZ));
	Serial.println("X GAIN: \t"+ String(eepromValues.IMU3_GAINX));
	Serial.println("Y GAIN: \t"+ String(eepromValues.IMU3_GAINY));
	Serial.println("Z GAIN: \t"+ String(eepromValues.IMU3_GAINZ));	
	Serial.println("IMU4 (Deflection1):");
	Serial.println("X Offset: \t"+ String(eepromValues.IMU4_OFFSX));
	Serial.println("Y Offset: \t"+ String(eepromValues.IMU4_OFFSY));
	Serial.println("Z Offset: \t"+ String(eepromValues.IMU4_OFFSZ));
	Serial.println("X GAIN: \t"+ String(eepromValues.IMU4_GAINX));
	Serial.println("Y GAIN: \t"+ String(eepromValues.IMU4_GAINY));
	Serial.println("Z GAIN: \t"+ String(eepromValues.IMU4_GAINZ));		
	Serial.println("IMU5 (Deflection2):");
	Serial.println("X Offset: \t"+ String(eepromValues.IMU5_OFFSX));
	Serial.println("Y Offset: \t"+ String(eepromValues.IMU5_OFFSY));
	Serial.println("Z Offset: \t"+ String(eepromValues.IMU5_OFFSZ));
	Serial.println("X GAIN: \t"+ String(eepromValues.IMU5_GAINX));
	Serial.println("Y GAIN: \t"+ String(eepromValues.IMU5_GAINY));
	Serial.println("Z GAIN: \t"+ String(eepromValues.IMU5_GAINZ));			
  */
  return eepromValues;  
}  

void Eeprom::putALLValues(EepromValues eepromValues)
{
    EEPROM.put(FRONT_SCALE_ADDRESS,eepromValues.frontScale);
    EEPROM.put(REAR_SCALE_ADDRESS,eepromValues.rearScale);
    EEPROM.put(CALIBRATION_WEIGHT_ADDRESS, eepromValues.calibrationWeight);
    EEPROM.put(SPAN_DISTANCE_ADDRESS, eepromValues.spanDistance);
    EEPROM.put(PEG_DISTANCE_ADDRESS, eepromValues.pegDistance);
    EEPROM.put(METRIC_IMP_OPTION_ADDRESS, eepromValues.metricImpOption);
	EEPROM.put(FLAPWIDTH_ADDRESS, eepromValues.flapwidth);
	EEPROM.put(TAN_SEC_OPTION_ADDRESS, eepromValues.TanSecOption);
	
	EEPROM.put(IMU1_OFFSX_ADDRESS, eepromValues.IMU1_OFFSX);
	EEPROM.put(IMU1_OFFSY_ADDRESS, eepromValues.IMU1_OFFSY);
	EEPROM.put(IMU1_OFFSZ_ADDRESS, eepromValues.IMU1_OFFSZ);
	EEPROM.put(IMU1_GAINX_ADDRESS, eepromValues.IMU1_GAINX);
	EEPROM.put(IMU1_GAINY_ADDRESS, eepromValues.IMU1_GAINY);
	EEPROM.put(IMU1_GAINZ_ADDRESS, eepromValues.IMU1_GAINZ);
	
	EEPROM.put(IMU2_OFFSX_ADDRESS, eepromValues.IMU2_OFFSX);
	EEPROM.put(IMU2_OFFSY_ADDRESS, eepromValues.IMU2_OFFSY);
	EEPROM.put(IMU2_OFFSZ_ADDRESS, eepromValues.IMU2_OFFSZ);
	EEPROM.put(IMU2_GAINX_ADDRESS, eepromValues.IMU2_GAINX);
	EEPROM.put(IMU2_GAINY_ADDRESS, eepromValues.IMU2_GAINY);
	EEPROM.put(IMU2_GAINZ_ADDRESS, eepromValues.IMU2_GAINZ);
	
	EEPROM.put(IMU3_OFFSX_ADDRESS, eepromValues.IMU3_OFFSX);
	EEPROM.put(IMU3_OFFSY_ADDRESS, eepromValues.IMU3_OFFSY);
	EEPROM.put(IMU3_OFFSZ_ADDRESS, eepromValues.IMU3_OFFSZ);
	EEPROM.put(IMU3_GAINX_ADDRESS, eepromValues.IMU3_GAINX);
	EEPROM.put(IMU3_GAINY_ADDRESS, eepromValues.IMU3_GAINY);
	EEPROM.put(IMU3_GAINZ_ADDRESS, eepromValues.IMU3_GAINZ);	
	
	EEPROM.put(IMU4_OFFSX_ADDRESS, eepromValues.IMU4_OFFSX);
	EEPROM.put(IMU4_OFFSY_ADDRESS, eepromValues.IMU4_OFFSY);
	EEPROM.put(IMU4_OFFSZ_ADDRESS, eepromValues.IMU4_OFFSZ);
	EEPROM.put(IMU4_GAINX_ADDRESS, eepromValues.IMU4_GAINX);
	EEPROM.put(IMU4_GAINY_ADDRESS, eepromValues.IMU4_GAINY);
	EEPROM.put(IMU4_GAINZ_ADDRESS, eepromValues.IMU4_GAINZ);		
	
	EEPROM.put(IMU5_OFFSX_ADDRESS, eepromValues.IMU5_OFFSX);
	EEPROM.put(IMU5_OFFSY_ADDRESS, eepromValues.IMU5_OFFSY);
	EEPROM.put(IMU5_OFFSZ_ADDRESS, eepromValues.IMU5_OFFSZ);
	EEPROM.put(IMU5_GAINX_ADDRESS, eepromValues.IMU5_GAINX);
	EEPROM.put(IMU5_GAINY_ADDRESS, eepromValues.IMU5_GAINY);
	EEPROM.put(IMU5_GAINZ_ADDRESS, eepromValues.IMU5_GAINZ);	

}


// store cg relevant calibration values
void Eeprom::putCGValues()
{
    EEPROM.put(FRONT_SCALE_ADDRESS,eepromValues.frontScale);
    EEPROM.put(REAR_SCALE_ADDRESS,eepromValues.rearScale);
    EEPROM.put(CALIBRATION_WEIGHT_ADDRESS, eepromValues.calibrationWeight);
    EEPROM.put(SPAN_DISTANCE_ADDRESS, eepromValues.spanDistance);
    EEPROM.put(PEG_DISTANCE_ADDRESS, eepromValues.pegDistance);
    EEPROM.put(METRIC_IMP_OPTION_ADDRESS, eepromValues.metricImpOption);
	EEPROM.put(FLAPWIDTH_ADDRESS, eepromValues.flapwidth);
	Serial.println("wrote span dist   ");Serial.println(eepromValues.spanDistance);
	EEPROM.get(SPAN_DISTANCE_ADDRESS, eepromValues.spanDistance);
	Serial.println("got span dist     ");Serial.println(eepromValues.spanDistance);

}

// store calibration values into requested(imuTarget) imu set
void Eeprom::putIMUValues(ImuCalValues IMUtmp, int imuTarget)
{
	switch (imuTarget)
	{
		case 0:
			EEPROM.put(IMU1_OFFSX_ADDRESS, IMUtmp.offsX);
			EEPROM.put(IMU1_OFFSY_ADDRESS, IMUtmp.offsY);
			EEPROM.put(IMU1_OFFSZ_ADDRESS, IMUtmp.offsZ);
			EEPROM.put(IMU1_GAINX_ADDRESS, IMUtmp.gainX);
			EEPROM.put(IMU1_GAINY_ADDRESS, IMUtmp.gainY);
			EEPROM.put(IMU1_GAINZ_ADDRESS, IMUtmp.gainZ);
			break;	
			
		case 1:
			EEPROM.put(IMU2_OFFSX_ADDRESS, IMUtmp.offsX);
			EEPROM.put(IMU2_OFFSY_ADDRESS, IMUtmp.offsY);
			EEPROM.put(IMU2_OFFSZ_ADDRESS, IMUtmp.offsZ);
			EEPROM.put(IMU2_GAINX_ADDRESS, IMUtmp.gainX);
			EEPROM.put(IMU2_GAINY_ADDRESS, IMUtmp.gainY);
			EEPROM.put(IMU2_GAINZ_ADDRESS, IMUtmp.gainZ);
			break;

		case 2:
			EEPROM.put(IMU3_OFFSX_ADDRESS, IMUtmp.offsX);
			EEPROM.put(IMU3_OFFSY_ADDRESS, IMUtmp.offsY);
			EEPROM.put(IMU3_OFFSZ_ADDRESS, IMUtmp.offsZ);
			EEPROM.put(IMU3_GAINX_ADDRESS, IMUtmp.gainX);
			EEPROM.put(IMU3_GAINY_ADDRESS, IMUtmp.gainY);
			EEPROM.put(IMU3_GAINZ_ADDRESS, IMUtmp.gainZ);
			break;
			
		case 3:
			EEPROM.put(IMU4_OFFSX_ADDRESS, IMUtmp.offsX);
			EEPROM.put(IMU4_OFFSY_ADDRESS, IMUtmp.offsY);
			EEPROM.put(IMU4_OFFSZ_ADDRESS, IMUtmp.offsZ);
			EEPROM.put(IMU4_GAINX_ADDRESS, IMUtmp.gainX);
			EEPROM.put(IMU4_GAINY_ADDRESS, IMUtmp.gainY);
			EEPROM.put(IMU4_GAINZ_ADDRESS, IMUtmp.gainZ);
			break;
				
		case 4:
			EEPROM.put(IMU5_OFFSX_ADDRESS, IMUtmp.offsX);
			EEPROM.put(IMU5_OFFSY_ADDRESS, IMUtmp.offsY);
			EEPROM.put(IMU5_OFFSZ_ADDRESS, IMUtmp.offsZ);
			EEPROM.put(IMU5_GAINX_ADDRESS, IMUtmp.gainX);
			EEPROM.put(IMU5_GAINY_ADDRESS, IMUtmp.gainY);
			EEPROM.put(IMU5_GAINZ_ADDRESS, IMUtmp.gainZ);
			break;
	}
}

// read calibration values into requested(imuTarget) imu set
void Eeprom::getIMUValues(int imuTarget)
{
	switch (imuTarget) {
		case 0:
			EEPROM.get(IMU1_OFFSX_ADDRESS, ImuCAL[0].offsX);
			EEPROM.get(IMU1_OFFSY_ADDRESS, ImuCAL[0].offsY);
			EEPROM.get(IMU1_OFFSZ_ADDRESS, ImuCAL[0].offsZ);
			EEPROM.get(IMU1_GAINX_ADDRESS, ImuCAL[0].gainX);
			EEPROM.get(IMU1_GAINY_ADDRESS, ImuCAL[0].gainY);
			EEPROM.get(IMU1_GAINZ_ADDRESS, ImuCAL[0].gainZ);
			break;				
		case 1:
			EEPROM.get(IMU2_OFFSX_ADDRESS, ImuCAL[1].offsX);
			EEPROM.get(IMU2_OFFSY_ADDRESS, ImuCAL[1].offsY);
			EEPROM.get(IMU2_OFFSZ_ADDRESS, ImuCAL[1].offsZ);
			EEPROM.get(IMU2_GAINX_ADDRESS, ImuCAL[1].gainX);
			EEPROM.get(IMU2_GAINY_ADDRESS, ImuCAL[1].gainY);
			EEPROM.get(IMU2_GAINZ_ADDRESS, ImuCAL[1].gainZ);
			break;
		case 2:
			EEPROM.get(IMU3_OFFSX_ADDRESS, ImuCAL[2].offsX);
			EEPROM.get(IMU3_OFFSY_ADDRESS, ImuCAL[2].offsY);
			EEPROM.get(IMU3_OFFSZ_ADDRESS, ImuCAL[2].offsZ);
			EEPROM.get(IMU3_GAINX_ADDRESS, ImuCAL[2].gainX);
			EEPROM.get(IMU3_GAINY_ADDRESS, ImuCAL[2].gainY);
			EEPROM.get(IMU3_GAINZ_ADDRESS, ImuCAL[2].gainZ);
			break;			
		case 3:
			EEPROM.get(IMU4_OFFSX_ADDRESS, ImuCAL[3].offsX);
			EEPROM.get(IMU4_OFFSY_ADDRESS, ImuCAL[3].offsY);
			EEPROM.get(IMU4_OFFSZ_ADDRESS, ImuCAL[3].offsZ);
			EEPROM.get(IMU4_GAINX_ADDRESS, ImuCAL[3].gainX);
			EEPROM.get(IMU4_GAINY_ADDRESS, ImuCAL[3].gainY);
			EEPROM.get(IMU4_GAINZ_ADDRESS, ImuCAL[3].gainZ);
			break;
		case 4:
			EEPROM.get(IMU5_OFFSX_ADDRESS, ImuCAL[4].offsX);
			EEPROM.get(IMU5_OFFSY_ADDRESS, ImuCAL[4].offsY);
			EEPROM.get(IMU5_OFFSZ_ADDRESS, ImuCAL[4].offsZ);
			EEPROM.get(IMU5_GAINX_ADDRESS, ImuCAL[4].gainX);
			EEPROM.get(IMU5_GAINY_ADDRESS, ImuCAL[4].gainY);
			EEPROM.get(IMU5_GAINZ_ADDRESS, ImuCAL[4].gainZ);
			break;		
	
	}

	
}


// store flap width
void Eeprom::putFlapValues()
{
    EEPROM.put(FLAPWIDTH_ADDRESS, eepromValues.flapwidth);
	EEPROM.put(TAN_SEC_OPTION_ADDRESS, eepromValues.TanSecOption);
}
