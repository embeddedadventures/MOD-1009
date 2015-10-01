/*

Copyright (c) 2015, Embedded Adventures
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

/* Test sketch for MOD1009 barometric pressure and temperature sensor*/

#include <MOD1009.h>
#include <Wire.h>

double celsius, millibar;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Welcome to the MOD1009 (MS5611) test sketch");
  Serial.println("Embedded Adventures (www.embeddedadventures.com)\n");
  mod1009.init();
  
  uns16* C = mod1009.readCoefficients();
  Serial.println("MOD1009 Coefficients");
  for (int i = 0; i < 7; i++) {
    Serial.print("C");
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(C[i]);
  }
}

void loop() {
  mod1009.readMeasurements();
  celsius  = mod1009.getTemperature();
  millibar = mod1009.getPressure();
  serial_print();
  delay(1500);
}

//Print out the info in a neater way
void serial_print() {
  Serial.println("MOD1009 Measurements");
  Serial.println("--------------------");
  Serial.print("Pressure: ");
  Serial.print(millibar);
  Serial.print(" mB\t");
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.print(" C\t");
  Serial.print(fahrenheit());
  Serial.println(" F");
  Serial.println();  
}

//Convert temp in C to F
double fahrenheit() {
  double fah = (celsius * 1.8) + 32;
  return fah;
}
