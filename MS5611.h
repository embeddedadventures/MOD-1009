/*

Copyright (c) 2016, Embedded Adventures
All rights reserved.

Contact us at source [at] embeddedadventures.com
www.embeddedadventures.com

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

*/

// MS5611 MOD-1009 barometric pressure and temperature sensor Arduino library
// Written originally by Embedded Adventures

#ifndef __MS5611_h
#define __MS5611_h

#include "Arduino.h"

#define uns8 unsigned char
#define uns16 unsigned int
#define uns32 unsigned long int
#define snd32 signed long int
#define uns64 unsigned long long int
#define snd64 signed long long int

#define MS5611_ADDR	 	0x76
#define CMD_RESET 		0x1E
#define READ_ADC		0x00
#define READ_TEMP 		0x58
#define READ_MBAR 		0x48

class MS5611Class
{
private:
	uns16 C[7];
	uns32 D1, D2;			//D1 = pressure, D2 = temperature
	snd32 TEMP, P;			//actual temperature and pressure

	/*Read 3 bytes from ADC. Returns raw ADC data*/
	uns32 readADC(uns8 code);
	
	/*Uses the coefficients read and the formulas from the datasheet to calculate temperature and pressure*/
	void calculate();
	
public:
	/*Reset device, load the coefficients from PROM*/
	void init();
	
	/*Read calibration coefficients at addresses 0xA2-0xAE, store into array C[]*/
	uns16* readCoefficients();
	
	/*Read digital temperature and pressure measurements*/
	void readMeasurements(); 
	
	/*Returns actual temperature value in Celsius*/
	double getTemperature();
	
	/*Returns actual pressure value in millibars*/
	double getPressure();
};

extern MS5611Class ms5611;
#endif