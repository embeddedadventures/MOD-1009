#include "MOD1009.h"
#include "Wire.h"

uns16 C[7];
uns32 D1, D2;			//D1 = pressure, D2 = temperature
snd32 TEMP, P;			//actual temperature and pressure



void MOD1009Class::setup() {
	Wire.beginTransmission(MOD1009_ADDR);
	Wire.write(CMD_RESET);
	Wire.endTransmission();
}
	
uns16* MOD1009Class::readCoefficients() {
	uns8 high_byte, low_byte;
	uns8 calib_addr;
	delay(5);
	for (int i = 0; i < 7; i++)
	{
		calib_addr = 0xA0 + (i * 2);
		Wire.beginTransmission(MOD1009_ADDR);
		Wire.write(calib_addr);
		Wire.endTransmission();

		Wire.requestFrom(MOD1009_ADDR, 2);
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

uns32 MOD1009Class::readADC(uns8 code) {
	uns8 raw[3];

	Wire.beginTransmission(MOD1009_ADDR);
	Wire.write(code);
	Wire.endTransmission();
	delay(10);
	Wire.beginTransmission(MOD1009_ADDR);
	Wire.write(READ_ADC);
	Wire.endTransmission();

	Wire.requestFrom(MOD1009_ADDR, 3);
	if (Wire.available()) {
		raw[2] = Wire.read();
		raw[1] = Wire.read();
		raw[0] = Wire.read();
	}
	return raw[0] + (raw[1] * 256) + (raw[2] * 65536);
}

void MOD1009Class::calculate() {
	snd32 dT = D2 - ((uns64)C[5] * 256);
	snd64 OFF = ((snd64)C[2] * 65536) + ((C[4] * dT) / 128);
	snd64 SENS = ((snd32)C[1] * 32768) + ((C[3] * dT) / 256);
	TEMP = 2000 + (((snd64)dT * (snd64)C[6]) / 8388608);
	P = ((((snd64)D1 * SENS) / 2097152) - OFF) / 32768;
}


void MOD1009Class::readMeasurements() {
	D1 = readADC(READ_MBAR);
	D2 = readADC(READ_TEMP);
	calculate();
}


double MOD1009Class::getTemperature() {
	return TEMP / 100.0;
}


double MOD1009Class::getPressure() {
	return P / 100.0;
}

MOD1009Class mod1009;

