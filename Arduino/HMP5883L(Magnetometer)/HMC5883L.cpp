#include <Wire.h>

uint8_t HMC5883L::read8(uint8_t reg)
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
int16_t HMC5883L::read16(uint8_t reg)
{
  int16_t value;
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
void HMC5883L::write8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(HMC5883Ladd);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
void HMC5883L::sampling(hmc5883lsample sample)
{
  uint8_t value;
  value=read8(HMC5883LCTRA);
  value = value&0b10011111;
  value = value|(sample<<5);
  write8(HMC5883LCTRA,value);
}
void HMC5883L::DOR(hmc5883lDOR dataoutrate)
{
  uint8_t value
  value = read8(HMC5883LCTRA);
  value = value&(0b11100011);
  value = value|(dataoutrate<<2);
  write8(HMC5883LCTRA,value);
}
void HMC5883L::measuremode(hmc5883lmm mm)
{
  uint8_t value;
  value=read8(HMC5883LCTRA);
  value = value&0b11111100;
  value = value|(mm);
  write8(HMC5883LCTRA,value);
 
}
void HMC5883L::resolution(hmc5883lres res)
{
  
  uint8_t value;
  value=read8(HMC5883LCTRB);
  value = value&0b00011111;
  value = value|(res<<5);
  write8(HMC5883LCTRB,value);
}
void HMC5883L::highspeed(hmc5883li2cspeed i2cspeed)
{
  
  uint8_t value;
  value=read8(HMC5883LMODE);
  value = value&0b01111111;
  value = value|(i2cspeed<<7);
  write8(HMC5883LMODE,value);
}
void HMC5883L::opmode(hmc5883lopmode opmode)
{
  
  uint8_t value;
  value=read8(HMC5883LMODE);
  value = value&0b11111100;
  value = value|(opmode);
  write8(HMC5883LMODE,value);
}
bool HMC5883L::begin()
{
  Wire.begin();
  if(read8(HMC5883LIDA) != 0x48)|| (read8(HMC5883LIDB) != 0x34)||(read8(HMC5883LIDC != 0x33)
  {
    return false;
  }
  resolution(HMC5883L_RANGE_1_3GA);
  DOR(HMC5883L_DATAREAD_15);
  measuremode(HMC5883L_CONTINUOUS);
  sampling(HMC5883L_SAMPLES_1);
  return true;
}
void HMC5883L::offset(float x ,float y,float z)
{
  Xo=x;
  Yo=y; 
  Zo=z;
}
void HMC5883L::valuemag()
{
  magread[0] = ((float)read16(HMC5883LXL)-Xo)*sf;
  magread[1] = ((float)read16(HMC5883LYL)-Yo)*sf;
  magread[2] = ((float)read16(HMC5883LZL)-Zo)*sf;
}
void HMC5883L::sfset(hmc5883lres res)
{
    switch(res)
    {
  case HMC5883L_RANGE_0_88GA:
      sf = 0.073f;
      break;

  case HMC5883L_RANGE_1_3GA:
      sf = 0.92f;
      break;

  case HMC5883L_RANGE_1_9GA:
      sf = 1.22f;
      break;

  case HMC5883L_RANGE_2_5GA:
      sf = 1.52f;
      break;

  case HMC5883L_RANGE_4GA:
      sf = 2.27f;
      break;

  case HMC5883L_RANGE_4_7GA:
      sf = 2.56f;
      break;

  case HMC5883L_RANGE_5_6GA:
      sf = 3.03f;
      break;

  case HMC5883L_RANGE_8_1GA:
      sf = 4.35f;
      break;
    }
}

void setup() {
  // put your setup code here, to run once:

}


