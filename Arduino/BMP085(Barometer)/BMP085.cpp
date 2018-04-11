#include "BMP085.h"

BMP085::BMP085() 
{

}


boolean BMP085::begin(uint8_t mode) {
  if (mode > BMP085_ULTRAHIGHRES)
    mode = BMP085_ULTRAHIGHRES;
  oversampling = mode;

  Wire1.begin();
  count = 0;
  if (read8(0xD0) != 0x55) return false;

  /* read calibration data */
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

int32_t BMP085::computeB5(int32_t UT) {
  int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
  int32_t X2 = ((int32_t)mc << 11) / (X1+(int32_t)md);
  return X1 + X2;
}

uint16_t BMP085::readRawTemperature(void) {
  write8(BMP085_CONTROL, BMP085_READTEMPCMD);
  delay(5);
  UT= read16(BMP085_TEMPDATA);
  return UT;
}

uint32_t BMP085::readRawPressure(void) {
  uint32_t raw;

  write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6));

  if (oversampling == BMP085_ULTRALOWPOWER)
    delay(5);
  else if (oversampling == BMP085_STANDARD)
    delay(8);
  else if (oversampling == BMP085_HIGHRES)
    delay(14);
  else
    delay(26);

  raw = read16(BMP085_PRESSUREDATA);

  raw <<= 8;
  raw |= read8(BMP085_PRESSUREDATA+2);
  raw >>= (8 - oversampling);
  return raw;
}


int32_t BMP085::readPressure(void) {
  int32_t  UP, B3,  B6, X1, X2, X3, p;
  uint32_t B4, B7;

  UP = readRawPressure();
  B6 = B5 - 4000;
  X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
  X2 = ((int32_t)ac2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;

  X1 = ((int32_t)ac3 * B6) >> 13;
  X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
  B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

  if (B7 < 0x80000000) {
    p = (B7 * 2) / B4;
  } else {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

  p = p + ((X1 + X2 + (int32_t)3791)>>4);
  return p;
}


float BMP085::readTemperature(void) {

  float temp;

  UT = readRawTemperature();
  B5 = computeB5(UT);
  temp = (B5+8) >> 4;
  temp /= 10;

  return temp;
}

void BMP085::calibrate(){
  long pressure1 = 0;
  float gp[1000],t;
  for (int i=0;i<1000;i++)
  {
    t = readTemperature();
    gp[i] = readPressure();
  }
  for(int i = 0; i<1000;i++)
  {
    pressure1 = pressure1 + gp[i];
  }
  gpressure = pressure1 / 1000;
}
float BMP085::calcpressureheight(void)
{
  if(count==0)
  {
    temperature = readTemperature();
    pressure = readPressure();
  }
  else
  {
    pressure = readPressure();
    if(count==4)
    {
      count = 0;
    }
  }
  height = (float)44330*((float)1-(float)(pow(((float)pressure/(float)gpressure),((float)1/5.255))));
}

uint8_t BMP085::read8(uint8_t a) {
  uint8_t ret;

  Wire1.beginTransmission(BMP085_I2CADDR);
  Wire1.write(a);
  Wire1.requestFrom(BMP085_I2CADDR, 1);
  ret = Wire1.read();
  Wire1.endTransmission();

  return ret;
}

uint16_t BMP085::read16(uint8_t a) {
  uint16_t ret;

  Wire1.beginTransmission(BMP085_I2CADDR);
  Wire1.write(a);
  Wire1.endTransmission();
  Wire1.beginTransmission(BMP085_I2CADDR);
  Wire1.requestFrom(BMP085_I2CADDR, 2);

  ret = Wire1.read();
  ret <<= 8;
  ret |= Wire1.read();

  Wire1.endTransmission();

  return ret;
}

void BMP085::write8(uint8_t a, uint8_t d) {
  Wire1.beginTransmission(BMP085_I2CADDR);
  Wire1.write(a);
  Wire1.write(d);
  Wire1.endTransmission();
}
