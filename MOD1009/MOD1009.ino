
/* Test sketch for MOD1009 barometric pressure and temperature sensor*/

#include <MOD1009.h>
#include <Wire.h>

double celsius, millibar;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Welcome to the MOD1009 (MS5611) test sketch");
  Serial.println("Embedded Adventures (www.embeddedadventures.com)\n");
  mod1009.setup();
  
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
