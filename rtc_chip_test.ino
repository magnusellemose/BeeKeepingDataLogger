#include <DS3232RTC.h>



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 
 /*
 tmElements_t tm; 
 tm.Day = 23;
 tm.Month = 9;
 tm.Year = 2020 - 1970;
 tm.Minute = 38;
 tm.Second = 35;
 tm.Hour = 14;
  
 RTC.write(tm);

     */

setSyncProvider(RTC.get);
}

void loop() {
 

  Serial.print("Date : ");
 Serial.print(day());
Serial.print("/");
Serial.print(month());
Serial.print("/");
Serial.print(year());
Serial.print("\t");
Serial.print(hour());
Serial.print(":");
Serial.print(minute());
Serial.print(":");
Serial.print(second());
Serial.print("\n"); 
Serial.print(now());
Serial.print("\n"); 
 delay(5000);


}
