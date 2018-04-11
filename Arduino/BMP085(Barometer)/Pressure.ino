#include <Wire.h>
#include <BMP085.h>
#include <SD.h>

 
BMP085 bmp;
void setup() {
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  Serial.begin(250000);
  if (!bmp.begin(0)) {

	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      delay(1000);
	digitalWrite(8,LOW);
  }
  else{
    if (!SD.begin(6)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
    digitalWrite(8,LOW);
  }
  Serial.println("card initialized.");
}
    delay(500);
    bmp.calibrate();
    delay(500);
    Serial.println("BMP085 initialized and ready");
    delay(500);
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
    Serial.print("BMP");
    Serial.print(" ");
    Serial.println("millis");
    dataFile.print(bmp.calcpressureheight());
    dataFile.print(" ");
    dataFile.println(millis());
    dataFile.close();
}
void loop() {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    Serial.print(bmp.calcpressureheight());
    Serial.print(" ");
    Serial.println(millis());
    dataFile.print(bmp.calcpressureheight());
    dataFile.print(" ");
    dataFile.println(millis());
    dataFile.close();
}
