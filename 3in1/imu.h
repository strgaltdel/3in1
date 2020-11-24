/*
- header file: 	imu.h
- content:		definition of a class to obtain IMU data for Arduino
- date:			15. Nov 2020
- rev.:			1.0
by strgaltdel

class AngleSens:
init(int angleSensorNum);
getRawAccValues(int angleSensorNum);
getfilteredAngleValues(int angleSensorNum);
getDEMAfilteredAngleValues(int angleSensorNum);
getImuCalValues(int num)

in order to adopt other IMU types you have to change: 
		- AngleSens::init()
		- AngleSens::getRawAccValues()
		- IMUSCALE; angSensor_Addr
		
		
		  
  1.0	erstes release
  1.1	Methoden zur Ermittlung von RAW-Durschnittswerte & Streuung/Varianz einer Meßreihe
		Methode für Verbindungstest
*/




#ifndef IMU_H
#define IMU_H

#include "includes_main.h"

// IMU specific:
#define IMUSCALE 16384														// IMU resolution / scaling of sensor (mpu6050=16bit, so 90 degree=14bit >> 2^14=16384)
int angSensor_Addr[5]={0x68,0x69,0x69,0x68,0x69};							// enter addresses from sensor0, 1, 2, 3, 4 i.e. GY521: 0x68 or 0x69

#define MPU6050_RA_WHO_AM_I         0x75
#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6


float filterFrequency = 0.2;    											// LP: filters out changes faster than x Hz  ; higher value >> faster sensor response, more noise
#define EMAALPHA  0.6														// ema & double ema declarations
//#include <Filters.h> 														// get filter classes




/*****************    struct for IMU acc-raw-values (acc only)  ************/
typedef struct rawAccImu {
   int32_t aX;
   int32_t aY;
   int32_t aZ;
} RawAsensor;

/*****************    struct for IMU angle values           ************/
typedef struct {
   float X;
   float Y;
   float Z;
   float pitch;
   float roll;
   float yaw;
} Isensor;

/*****************    struct for variance data              ************/
typedef struct {
   int16_t avgX;
   int16_t avgY;
   float varianceX;
   float varianceY;
} SensVar;


#ifndef EEPROM_H															// if not defined in eeprom.h init ImuCal array
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
#endif



	

													
// internal use / debugging
int32_t  xcounter;															

// ema filter calculation
int X1ema_ema, Y1ema_ema, Z1ema_ema, X2ema_ema, Y2ema_ema, Z2ema_ema;
int X1ema, Y1ema, Z1ema, X2ema, Y2ema, Z2ema;
int X1Dema, Y1Dema, Z1Dema, X2Dema, Y2Dema, Z2Dema;
int X1storedEma, Y1storedEma, Z1storedEma, X2storedEma, Y2storedEma, Z2storedEma;

// init LP filters
FilterOnePole lowpassFilter0_AX{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter (sensor 0)
FilterOnePole lowpassFilter0_AY{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter0_AZ{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter

FilterOnePole lowpassFilter1_AX{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter (sensor 1)
FilterOnePole lowpassFilter1_AY{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter1_AZ{ LOWPASS, filterFrequency };    			// create a one pole (RC) lowpass filter





int EMA_function(float alpha, int latest, int stored){						// ema function
	return round(alpha*latest) + round((1-alpha)*stored);
	}


	
void getImuCalValues(int num){

  if(READ_EEPROM) {  
	Serial.println("get eeprom cal data");
	Eeprom::getIMUValues(num);
  }
  else {
	 Serial.println("get coded cal data");
	// 		in case you dont want to use eeprom stored values:
	// 		define the calibration sets you need for imu sensors 0-n here:
		//   		{Xoff, Yoff, Zoff,   Xgain, 		Ygain, 		Zgain	, 	maxmin dummies}

	ImuCAL[0] =		{   0,    0,    0, 1.000000000, 1.000000000, 1.000000000,	1,-1,1,-1,1,-1};
	ImuCAL[1] =		{   0,    0,    0, 1.000000000, 1.000000000, 1.000000000,	1,-1,1,-1,1,-1};
	ImuCAL[2] =		{   0,    0,    0, 1.000000000, 1.000000000, 1.000000000,	1,-1,1,-1,1,-1};
	ImuCAL[3] =		{   0,    0,    0, 1.000000000, 1.000000000, 1.000000000,	1,-1,1,-1,1,-1};
	ImuCAL[4] =		{   0,    0,    0, 1.000000000, 1.000000000, 1.000000000,	1,-1,1,-1,1,-1};
  }
}

// definition class
class AngleSens {
  private:
    uint8_t buffer[14];
  
  public:
    AngleSens() {};
	void init(int angleSensorNum);
	uint8_t getDeviceID(uint8_t id);
	void setDeviceID(uint8_t imuAddr, uint8_t id);
	bool testConnection(uint8_t id);

	
    RawAsensor getRawAccValues(int angleSensorNum);
	Isensor getAngleValues(int angleSensorNum);
	Isensor getfilteredAngleValues(int angleSensorNum);
	Isensor getDEMAfilteredAngleValues(int angleSensorNum);
	Isensor getrawValues(int angleSensorNum);
	SensVar calcVariance(int angleSensorNum);

};



// method "init"
void AngleSens::init(int angleSensorNum)
{
  // init sensor (used in main setup, dependent on sensor))
  // mpu6050 (GY-521):
  Wire.begin();
  Wire.beginTransmission(angSensor_Addr[angleSensorNum]);								// select sensor
  Wire.write(0x6B);																		// powermode register mpu6050
  Wire.write(0);																		// "wakeup call"
  // Wire.write(0x1c);																	// acc sensitivity register mpu6050
  // Wire.write(0);																		// set to 0  =2g
  Wire.endTransmission(true);															// end init

  Serial.print("init IMU  0x");
  Serial.println(angSensor_Addr[angleSensorNum],HEX);
    getImuCalValues(angleSensorNum);														// get calibration data from function
  
}


// ############  method "getRawAccValues"

RawAsensor AngleSens::getRawAccValues	(int angleSensorNum)
{
	RawAsensor RawImu;
	int dummy;
	int x,y,z;
	// prepare reading from sensor (dependent on sensor)
	Wire.beginTransmission(angSensor_Addr[angleSensorNum]);
	Wire.write(0x3B);																	// get access to acc registers
	Wire.endTransmission(false);
	Wire.requestFrom(angSensor_Addr[angleSensorNum],6,true);							// request 6 bytes
	//Serial.println(angSensor_Addr[angleSensorNum],HEX);
	if(not(Wire.available())) {
		//Serial.print("kein Sensor ");Serial.println(angleSensorNum);
	}
	else {
		while(Wire.available() < 6);													// await availability
		// read raw acc values from sensor (dependent on sensor)
		RawImu.aX = Wire.read()<<8|Wire.read();
		RawImu.aY = Wire.read()<<8|Wire.read();
		RawImu.aZ = Wire.read()<<8|Wire.read();
		Wire.endTransmission(true);	
	}
	xcounter++;

	//Serial.print(angleSensorNum);Serial.print("\t");Serial.print(RawImu.aX);Serial.print("\t");Serial.print(RawImu.aX);Serial.print("\t");Serial.println(RawImu.aX);
	return (RawImu);																	// return structure
}



// ############  method "getAngleValues"

Isensor AngleSens::getAngleValues	(int angleSensorNum)
{
	Isensor tmp_imu;																	// declare structure
	float xAng, yAng, zAng;																// temp "smoothed" values
	RawAsensor tmpraw_A;																// raw data used in this run

	tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);								// get raw data from imu (not calibrated)


	
	// data calibration:
	xAng = (tmpraw_A.aX - ImuCAL[angleSensorNum].offsX)/(ImuCAL[angleSensorNum].gainX*IMUSCALE);   
	yAng = (tmpraw_A.aY - ImuCAL[angleSensorNum].offsY)/(ImuCAL[angleSensorNum].gainY*IMUSCALE);   
	zAng = (tmpraw_A.aZ - ImuCAL[angleSensorNum].offsZ)/(ImuCAL[angleSensorNum].gainZ*IMUSCALE);  
	
	// calc angle values (DEG)
    tmp_imu.X= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);									// x-angle
    tmp_imu.Y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);									// y-angle
    tmp_imu.Z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);									// z-angle
	tmp_imu.pitch  = (atan2(yAng, (sqrt(xAng * xAng + zAng * zAng))) * 180.0) / PI;     // roll
	tmp_imu.roll   = (atan2(xAng, (sqrt(yAng * yAng + zAng * zAng))) * 180.0) / PI;     // pitch	
	tmp_imu.yaw    = (atan2(zAng, (sqrt(xAng * xAng + yAng * yAng))) * 180.0) / PI;     // roll

	return (tmp_imu);																	// return structure
}


// ############  method "getrawValues"

Isensor AngleSens::getrawValues	(int angleSensorNum)
{
	Isensor tmp_imu;																	// declare structure
	float xAng, yAng, zAng;																// temp "smoothed" values
	RawAsensor tmpraw_A;																// raw data used in this run

	tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);								// get raw data from imu (not calibrated)


	
	// raw data :
	xAng = (tmpraw_A.aX);
	yAng = (tmpraw_A.aY);
	zAng = (tmpraw_A.aZ);
	Serial.println(xAng);
	
	// return values
	tmp_imu.pitch  = xAng;     // roll
	tmp_imu.roll   = yAng;     // pitch	
	tmp_imu.yaw    = zAng;     // roll

	return (tmp_imu);																	// return structure
}


// ############  method "getDeviceID"
uint8_t AngleSens::getDeviceID(uint8_t imuAddr) {
	buffer[0] = 0;
	I2Cdev::readBits(imuAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
    return buffer[0];
}

void AngleSens::setDeviceID(uint8_t imuAddr, uint8_t id) {
    I2Cdev::writeBits(imuAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id);
}

// ############  method "testConnection"
bool AngleSens::testConnection(uint8_t imuAddr) {
	Serial.print("Connection  0x"); Serial.println(imuAddr);
	Serial.println(getDeviceID(imuAddr));
    return getDeviceID(imuAddr) == 0x34;
}




// ############  method "calcVariance"

SensVar AngleSens::calcVariance	(int angleSensorNum)
{
	SensVar tmp_var;																	// declare structure
	RawAsensor tmpraw_A;																// raw data used in this run

	int 	avgLoops = 700;																// number of loops to build average
	int 	measLoops = 1500;															// number of loops to eval variance
	int32_t SensAverageX = 0;
	int32_t SensAverageY = 0;
	int32_t	varX =0;
    int32_t	varY =0;
	int		i;

	Serial.print("averaging  IMU");Serial.print(angleSensorNum);
	
	// build average
	for (i=0; i<avgLoops; i++) {
		tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);							// get raw data from imu (not calibrated)
		SensAverageX += tmpraw_A.aX;
		SensAverageY += tmpraw_A.aY;
		//Serial.println(tmpraw_A.aX);
	}	
	SensAverageX = SensAverageX/avgLoops;
	SensAverageY = SensAverageY/avgLoops;
	
	Serial.println("variance");
	
	// eval variance
	for (i=0; i<measLoops; i++) {
		tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);							// get raw data from imu (not calibrated)
		varX += sq(tmpraw_A.aX - SensAverageX);											// built square from difference
		varY += sq(tmpraw_A.aY - SensAverageY);
		//Serial.print((tmpraw_A.aX - SensAverageX));Serial.print("      ");Serial.print(sq(tmpraw_A.aX - SensAverageX));;Serial.print("      ");Serial.println(varX);
	}	
	varX = sqrt(varX/(measLoops-1));
    varY = sqrt(varY/(measLoops-1));

	
	// return values
	tmp_var.avgX  		= SensAverageX;     											// averageX
	tmp_var.avgY  		= SensAverageY;     										
	tmp_var.varianceX  	= varX;     													// variance
	tmp_var.varianceY  	= varY; 	
	Serial.print("AVG  x: ");Serial.print(SensAverageX);Serial.print("  variance  x: ");Serial.println(varX);

	Serial.print("OK   IMU");Serial.print(angleSensorNum);
	return (tmp_var);																	// return structure
}


// ############  method "getfilteredAngleValues"

Isensor AngleSens::getfilteredAngleValues	(int angleSensorNum)
{
	Isensor tmp_imu;																	// declare structure
	float xAng, yAng, zAng;																// temp "smoothed" values
	RawAsensor tmpraw_A;																// raw data used in this run
	
	tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);								// get raw data from imu (not calibrated)
	
	// use LP filter
	if (angleSensorNum == 0 or angleSensorNum == 3) {									// use filter#0 (only sensor 0&3 (wing & Flap0))
		xAng = (lowpassFilter0_AX.input(tmpraw_A.aX) - ImuCAL[angleSensorNum].offsX)/(ImuCAL[angleSensorNum].gainX*IMUSCALE);    //update filter & smooth sensor noise & calibrate
		yAng = (lowpassFilter0_AY.input(tmpraw_A.aY) - ImuCAL[angleSensorNum].offsY)/(ImuCAL[angleSensorNum].gainY*IMUSCALE);    //update filter & smooth sensor noise & calibrate
		zAng = (lowpassFilter0_AZ.input(tmpraw_A.aZ) - ImuCAL[angleSensorNum].offsZ)/(ImuCAL[angleSensorNum].gainZ*IMUSCALE);    //update filter & smooth sensor noise & calibrate
	}
	
	else {										// use filter#1
		xAng = (lowpassFilter1_AX.input(tmpraw_A.aX) - ImuCAL[angleSensorNum].offsX)/(ImuCAL[angleSensorNum].gainX*IMUSCALE);    //update filter & smooth sensor noise & calibrate
		yAng = (lowpassFilter1_AY.input(tmpraw_A.aY) - ImuCAL[angleSensorNum].offsY)/(ImuCAL[angleSensorNum].gainY*IMUSCALE);    //update filter & smooth sensor noise & calibrate
		zAng = (lowpassFilter1_AZ.input(tmpraw_A.aZ) - ImuCAL[angleSensorNum].offsZ)/(ImuCAL[angleSensorNum].gainZ*IMUSCALE);    //update filter & smooth sensor noise & calibrate
	}
	
	// calc values
    tmp_imu.X= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);									// x-angle
    tmp_imu.Y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);									// y-angle
    tmp_imu.Z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);									// z-angle
	tmp_imu.pitch  = (atan2(yAng, (sqrt(xAng * xAng + zAng * zAng))) * 180.0) / PI;     // roll
	tmp_imu.roll   = (atan2(xAng, (sqrt(yAng * yAng + zAng * zAng))) * 180.0) / PI;     // pitch	
	tmp_imu.yaw    = (atan2(zAng, (sqrt(xAng * xAng + yAng * yAng))) * 180.0) / PI;     // roll
	
	return (tmp_imu);																	// return structure
}




// ############  method "getDEMAfilteredAngleValues"

Isensor AngleSens::getDEMAfilteredAngleValues	(int angleSensorNum)
{
	Isensor tmp_imu;																	// declare structure
	float xAng, yAng, zAng;																// temp "smoothed" values	
	RawAsensor tmpraw_A;																// raw data used in this run
	
	tmpraw_A = AngleSens::getRawAccValues(angleSensorNum);								// get raw data from imu (not calibrated)

	// use ema filter 
	if (angleSensorNum == 0 or angleSensorNum == 3) {									// use filter#1 (only sensor 0&3 (wing & flap0))

		X1ema = EMA_function(EMAALPHA, tmpraw_A.aX, X1ema);
		X1ema_ema = EMA_function(EMAALPHA, X1ema, X1ema_ema);
		X1Dema = 2* X1ema-X1ema_ema;
		xAng =  (X1Dema  - ImuCAL[angleSensorNum].offsX)/(ImuCAL[angleSensorNum].gainX*IMUSCALE);
		
		Y1ema = EMA_function(EMAALPHA, tmpraw_A.aY, Y1ema);
		Y1ema_ema = EMA_function(EMAALPHA, Y1ema, Y1ema_ema);
		Y1Dema = 2* Y1ema-Y1ema_ema;
		yAng =  (Y1Dema  - ImuCAL[angleSensorNum].offsY)/(ImuCAL[angleSensorNum].gainY*IMUSCALE);
		
		Z1ema = EMA_function(EMAALPHA, tmpraw_A.aZ, Z1ema);
		Z1ema_ema = EMA_function(EMAALPHA, Z1ema, Z1ema_ema);
		Z1Dema = 2* Z1ema-Z1ema_ema;
		zAng =  (Z1Dema  - ImuCAL[angleSensorNum].offsZ)/(ImuCAL[angleSensorNum].gainZ*IMUSCALE);
		//Serial.print("\t");Serial.print(angleSensorNum);Serial.print("\t");Serial.print(tmpraw_A.aX);Serial.print("\t");Serial.print(tmpraw_A.aY);Serial.print("\t");Serial.println(tmpraw_A.aZ);
	}
	
	else {																				// use filter#2 (tail & flap1)
		X2ema = EMA_function(EMAALPHA, tmpraw_A.aX, X2ema);
		X2ema_ema = EMA_function(EMAALPHA, X2ema, X2ema_ema);
		X2Dema = 2* X2ema-X2ema_ema;
		xAng =  (X2Dema  - ImuCAL[angleSensorNum].offsX)/(ImuCAL[angleSensorNum].gainX*IMUSCALE);
		
		Y2ema = EMA_function(EMAALPHA, tmpraw_A.aY, Y2ema);
		Y2ema_ema = EMA_function(EMAALPHA, Y2ema, Y2ema_ema);
		Y2Dema = 2* Y2ema-Y2ema_ema;
		yAng =  (Y2Dema  - ImuCAL[angleSensorNum].offsY)/(ImuCAL[angleSensorNum].gainY*IMUSCALE);
		
		Z2ema = EMA_function(EMAALPHA, tmpraw_A.aZ, Z2ema);
		Z2ema_ema = EMA_function(EMAALPHA, Z2ema, Z2ema_ema);
		Z2Dema = 2* Z2ema-Z2ema_ema;
		zAng =  (Z2Dema  - ImuCAL[angleSensorNum].offsZ)/(ImuCAL[angleSensorNum].gainZ*IMUSCALE);
		//Serial.print("\t");Serial.print(angleSensorNum);Serial.print("\t");Serial.print(XAng);Serial.print("\t");Serial.print(YAng);Serial.print("\t");Serial.println(ZAng);
	}
	
	// calc values
    tmp_imu.X= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);									// x-angle
    tmp_imu.Y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);									// y-angle
    tmp_imu.Z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);									// z-angle
	tmp_imu.pitch  = (atan2(yAng, (sqrt(xAng * xAng + zAng * zAng))) * 180.0) / PI;     // roll
	tmp_imu.roll   = (atan2(xAng, (sqrt(yAng * yAng + zAng * zAng))) * 180.0) / PI;     // pitch	
	tmp_imu.yaw    = (atan2(zAng, (sqrt(xAng * xAng + yAng * yAng))) * 180.0) / PI;     // roll
	
	return (tmp_imu);																	// return structure
}





#endif
