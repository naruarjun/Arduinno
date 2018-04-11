#include <Wire.h>
#define HMC5883Ladd (0x1E)
#define HMC5883LXL (0x03)
#define HMC5883LXH (0x04)
#define HMC5883LZL (0x05)
#define HMC5883LZH (0x06)
#define HMC5883LYL (0x07)
#define HMC5883LYH (0x08)
float magread[0],Xo=0,Yo=0,Zo=0,sf;
void write8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(HMC5883Ladd);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
uint8_t read8(uint8_t reg)
{
  uint8_t value;
  Wire.beginTransmission(HMC5883Ladd);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(HMC5883Ladd,1);
  while(Wire.available()==0);
  value=Wire.read();
  return value;
}
int read16(uint8_t reg)
{
  int value;
  uint8_t l,h;
  Wire.beginTransmission(HMC5883Ladd);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(HMC5883Ladd,2);
  while(Wire.available()>=2);
  {
    l=Wire.read();
    h=Wire.read();
  }
  value = ((h<<8)|l);
  return value;

}
void takemagvalues(float magread[])
{
  magread[0] = ((float)read16(HMC5883LXL)-(float)Xo)*sf;
  magread[1] = ((float)read16(HMC5883LYL)-(float)Yo)*sf;
  magread[2] = ((float)read16(HMC5883LZL)-(float)Zo)*sf;
}





void setup() {
  int i=0;
  // put your setup code here, to run once:
 Wire.begin();
 Serial.begin(9600);
  for(i=0;i<1000;i++)
  {
    takemagvalues(magread);
    Xo += magread[0];
    Yo += magread[1];
    Zo += magread[2];
  }
  Xo = Xo/1000;
  Yo = Yo/1000;
  Zo = Zo/1000;

}

void loop() {
  
  // put your main code here, to run repeatedly:

}
