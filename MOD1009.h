#ifndef __MOD1009_h
#define __MOD1009_h

#include "Arduino.h"

#define uns8 unsigned char
#define uns16 unsigned int
#define uns32 unsigned long int
#define snd32 signed long int
#define uns64 unsigned long long int
#define snd64 signed long long int

#define MOD1009_ADDR 0x76
#define CMD_RESET 0x1E
#define READ_ADC 0x00
#define READ_TEMP 0x58
#define READ_MBAR 0x48

class MOD1009Class
{
private:
	/*Read 3 bytes from ADC. Returns raw ADC data*/
	uns32 readADC(uns8 code);
	
	/*Uses the coefficients read and the formulas from the datasheet to calculate temperature and pressure*/
	void calculate();
	
public:
	/*Reset device, load the coefficients from PROM*/
	void setup();
	
	/*Read calibration coefficients at addresses 0xA2-0xAE, store into array C[]*/
	uns16* readCoefficients();
	
	/*Read digital temperature and pressure measurements*/
	void readMeasurements(); 
	
	/*Returns actual temperature value in Celsius*/
	double getTemperature();
	
	/*Returns actual pressure value in millibars*/
	double getPressure();
};

extern MOD1009Class mod1009;
#endif