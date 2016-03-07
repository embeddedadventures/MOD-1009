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

#include "MS5611.h"
#include "Wire.h"



void MS5611Class::init() {
	Wire.beginTransmission(MS5611_ADDR);
	Wire.write(CMD_RESET);
	Wire.endTransmission();
}
	
uns16* MS5611Class::readCoefficients() {
	uns8 high_byte, low_byte;
	uns8 calib_addr;
	delay(5);
	for (int i = 0; i < 7; i++)
	{
		calib_addr = 0xA0 + (i * 2);
		Wire.beginTransmission(MS5611_ADDR);
		Wire.write(calib_addr);
		Wire.endTransmission();

		Wire.requestFrom(MS5611_ADDR, 2);
		if (Wire.available()) {
			high_byte = Wire.read();
			low_byte = Wire.read();
		}
		else {
			Serial.println("Data unavailable...");
		}

		C[i] = (high_byte << 8) | low_byte;
	}
	return C;
}

uns32 MS5611Class::readADC(uns8 code) {
	uns8 raw[3];

	Wire.beginTransmission(MS5611_ADDR);
	Wire.write(code);
	Wire.endTransmission();
	delay(10);
	Wire.beginTransmission(MS5611_ADDR);
	Wire.write(READ_ADC);
	Wire.endTransmission();

	Wire.requestFrom(MS5611_ADDR, 3);
	if (Wire.available()) {
		raw[2] = Wire.read();
		raw[1] = Wire.read();
		raw[0] = Wire.read();
	}
	return raw[0] + (raw[1] * 256) + (raw[2] * 65536);
}

void MS5611Class::calculate() {
	snd32 dT = D2 - ((uns64)C[5] * 256);
	snd64 OFF = ((snd64)C[2] * 65536) + ((C[4] * dT) / 128);
	snd64 SENS = ((snd32)C[1] * 32768) + ((C[3] * dT) / 256);
	TEMP = 2000 + (((snd64)dT * (snd64)C[6]) / 8388608);
	P = ((((snd64)D1 * SENS) / 2097152) - OFF) / 32768;
}

void MS5611Class::readMeasurements() {
	D1 = readADC(READ_MBAR);
	D2 = readADC(READ_TEMP);
	calculate();
}

double MS5611Class::getTemperature() {
	return TEMP / 100.0;
}

double MS5611Class::getPressure() {
	return P / 100.0;
}

MS5611Class ms5611;

