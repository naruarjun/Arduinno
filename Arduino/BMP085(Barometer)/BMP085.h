#ifndef BMP085_H
#define BMP085_H
#include "Arduino.h"
#include "Wire.h"

#define BMP085_I2CADDR 0x77

#define BMP085_ULTRALOWPOWER 0
#define BMP085_STANDARD      1
#define BMP085_HIGHRES       2
#define BMP085_ULTRAHIGHRES  3
#define BMP085_CAL_AC1           0xAA
#define BMP085_CAL_AC2           0xAC
#define BMP085_CAL_AC3           0xAE
#define BMP085_CAL_AC4           0xB0
#define BMP085_CAL_AC5           0xB2
#define BMP085_CAL_AC6           0xB4
#define BMP085_CAL_B1            0xB6
#define BMP085_CAL_B2            0xB8
#define BMP085_CAL_MB            0xBA
#define BMP085_CAL_MC            0xBC
#define BMP085_CAL_MD            0xBE

#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD          0x2E
#define BMP085_READPRESSURECMD            0x34


class BMP085 {
 public:
  BMP085();
  boolean begin(uint8_t mode = BMP085_ULTRALOWPOWER);  // by default go standard
  void calibrate(void);
  float readTemperature(void);
  int32_t readPressure(void);
  uint16_t readRawTemperature(void);
  uint32_t readRawPressure(void);
  float calcpressureheight(void);


 private:

 int32_t computeB5(int32_t UT);
  uint8_t read8(uint8_t addr);
  uint16_t read16(uint8_t addr);
  void write8(uint8_t addr, uint8_t data);

  int count;
  uint8_t oversampling;
  int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
  uint16_t ac4, ac5, ac6;
  int32_t UT,B5,pressure,gpressure;
  float height, temperature ;
};


#endif
