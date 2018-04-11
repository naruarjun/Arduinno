#include <Wire.h>
int count;
int oss, ac1, ac2, ac3, b1, b2, mb, mc, md, ac4, ac5, ac6, UT,B5;
#define BMP085_I2CADDR (0x77)

#define BMP085_ULTRALOWPOWER 0
#define BMP085_STANDARD      1
#define BMP085_HIGHRES       2
#define BMP085_ULTRAHIGHRES  3
#define BMP085_CAL_AC1           (0xAA)
#define BMP085_CAL_AC2           (0xAC)
#define BMP085_CAL_AC3           (0xAE)
#define BMP085_CAL_AC4           (0xB0)
#define BMP085_CAL_AC5           (0xB2)
#define BMP085_CAL_AC6           (0xB4)
#define BMP085_CAL_B1            (0xB6)
#define BMP085_CAL_B2            (0xB8)
#define BMP085_CAL_MB            (0xBA)
#define BMP085_CAL_MC            (0xBC)
#define BMP085_CAL_MD            (0xBE)

#define BMP085_CONTROL           (0xF4)
#define BMP085_TEMPDATA          (0xF6)
#define BMP085_PRESSUREDATA      (0xF6)
#define BMP085_READTEMPCMD       (0x2E)


int read16(int reg)
{
  int value;
  int l,h;
  Wire.beginTransmission(BMP085_I2CADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_I2CADDR,2);
  while(Wire.available()==0);
  
    l=Wire.read();
    h=Wire.read();
  
  value = ((l<<8)|h);
  return value;

}
void write8(int reg, int value)
{
  Wire.beginTransmission(BMP085_I2CADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
int read8(int reg)
{
  int value;
  Wire.beginTransmission(BMP085_I2CADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_I2CADDR,1);
  while(Wire.available()==0);
  value=Wire.read();
  return value;
}
boolean begin()
{
  if (read8(0xD0) != 0x55) return false;
  count =0 ;
  ac1 = read16(BMP085_CAL_AC1);
  ac2 = read16(BMP085_CAL_AC2);
  ac3 = read16(BMP085_CAL_AC3);
  ac4 = read16(BMP085_CAL_AC4);
  ac5 = read16(BMP085_CAL_AC5);
  ac6 = read16(BMP085_CAL_AC6);

  b1 = read16(BMP085_CAL_B1);
  b2 = read16(BMP085_CAL_B2);

  mb = read16(BMP085_CAL_MB);
  mc = read16(BMP085_CAL_MC);
  md = read16(BMP085_CAL_MD);


  return true;
}
int B5comp(int UT)
{
  int X1 = (UT - (int)ac6) * ((int)ac5) >> 15;
  int X2 = ((int)mc << 11) / (X1+md);
  return X1 + X2;
}
int rawtemp()
{
  write8(0xF4,0x2E);
  delay(5);
  UT = read16(0xF6);
  return UT;
}
int rawpressure()
{
  int pres;
  write8(0xF4,(0x34+(oss<<6)));
  if (oss == BMP085_ULTRALOWPOWER)
    delay(5);
  else if (oss == BMP085_STANDARD)
    delay(8);
  else if (oss == BMP085_HIGHRES)
    delay(14);
  else
    delay(26);
  pres = read16(0xF6);
  pres= pres<<8;
  pres = pres|read8(0xF8);
  pres = pres>>(8-oss);
  
    
}
int readpres()
{
  int  UP, B3,  B6, X1, X2, X3, p;
  int B4, B7;

  UP = rawpressure();
  B6 = B5 - 4000;
  X1 = (b2 * ( (B6 * B6)>>12 )) >> 11;
  X2 = (ac2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = (((ac1*4 + X3) << oss) + 2) / 4;

  X1 = ((int)ac3 * B6) >> 13;
  X2 = ((int)b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = ((int)ac4 * (int)(X3 + 32768)) >> 15;
  B7 = ((int)UP - B3) * (int)( 50000UL >> oss );

  if (B7 < 0x80000000) {
    p = (B7 * 2) / B4;
  } else {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

  p = p + ((X1 + X2 + (int)3791)>>4);
  return p;
}
float readTemp(void) {

  float temp;

  UT = rawtemp();
  B5 = B5comp(UT);
  temp = (B5+8) >> 4;
  temp /= 10;

  return temp;
}

float off=0;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  while(!begin())
  {
    Serial.println("sensor is fucked up");
  }
  rawtemp();
  B5comp(UT);
  for(int i=0;i<1000;i++)
  {
    off = off + readpres();
  }
  off = off/1000.0f;
}
 float pres = 0;
void loop() {
  // put your main code here, to run repeatedly:
  pres = readpres()-off;
  Serial.println(pres);
}
