#include <Wire.h>
float gyroread[3];
float Xo=0,Yo=0,Zo=0;
float DegX=0,DegY=0,DegZ=0;

int read16(int reg)
{
  int value;
  int l,h;
  Wire.beginTransmission(0x69);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0x69,2);
  while(Wire.available()==0);
  
    l=Wire.read();
    h=Wire.read();
  
  value = ((h<<8)|l);
  return value;

}
void write8(int reg, int value)
{
  Wire.beginTransmission(0x69);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
void getgyroreadings() 
{
  
  gyroread[0] = (float)read16(0x28)*0.07f;
  gyroread[1] = (float)read16(0x2A)*0.07f;
  gyroread[2] = (float)read16(0x2C)*0.07f;
} 

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  write8(0x20,0b10101111);
  write8(0x23,0x00110000);

  }


void loop() {
  getgyroreadings();  
  //Serial.print(DegX);
  //Serial.print(" ");
  //Serial.print(DegY);
  //Serial.print(" ");
  //Serial.println(DegZ);
  Serial.print(gyroread[0]);
  Serial.print(" ");
  Serial.print(gyroread[1]);
  Serial.print(" ");
  Serial.println(gyroread[2]);
  delay(10);

}
