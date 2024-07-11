/* im Hauptprogramm:
#define ScanI2C
#ifdef ScanI2C
#include "I2C_Scan.c"
#endif
*/
#ifdef ScanI2C
#include <Wire.h>
void I2C_Scan() {
  byte error, address;
  int nDevices = 0;

  Wire.begin();  Serial.println("Scanning I2C...");

  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    Wire.beginTransmission(address+1);
    if (error == 0 && Wire.endTransmission() != 0 ) // Special flag for SAMD Series
    {
      Serial.print("I2C device found at 0x");  if (address<16) Serial.print("0");  Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at 0x");     if (address<16)  Serial.print("0");  Serial.println(address,HEX); }
  }
  if (nDevices == 0)  Serial.println("No I2C devices found\n");
  else  Serial.println("done\n");
}
#endif
