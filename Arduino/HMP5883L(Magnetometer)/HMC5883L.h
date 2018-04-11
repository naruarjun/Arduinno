#define HMC5883Ladd (0x1E)
#define HMC5883LCTRA (0x00)
#define HMC5883L_REG_CONFIG_B (0x01)
#define HMC5883LMODE (0x02)
#define HMC5883LXL (0x03)
#define HMC5883LXH (0x04)
#define HMC5883LZL (0x05)
#define HMC5883LZH (0x06)
#define HMC5883LYL (0x07)
#define HMC5883LYH (0x08)
#define HMC5883LIDA (0x0A)
#define HMC5883LIDB (0x0B)
#define HMC5883LIDC (0x0C)

typedef enum
{
    HMC5883L_SAMPLES_8 = 0b11,
    HMC5883L_SAMPLES_4 = 0b10,
    HMC5883L_SAMPLES_2 = 0b01,
    HMC5883L_SAMPLES_1 = 0b00
} hmc5883lsample;

typedef enum
{
    HMC5883L_DATARATE_75HZ = 0b110,
    HMC5883L_DATARATE_30HZ = 0b101,
    HMC5883L_DATARATE_15HZ = 0b100,
    HMC5883L_DATARATE_7_5HZ = 0b011,
    HMC5883L_DATARATE_3HZ = 0b010,
    HMC5883L_DATARATE_1_5HZ = 0b001,
    HMC5883L_DATARATE_0_75_HZ = 0b000
} hmc5883lDOR;

typedef enum
{
    HMC5883L_RANGE_8_1GA = 0b111,
    HMC5883L_RANGE_5_6GA = 0b110,
    HMC5883L_RANGE_4_7GA = 0b101,
    HMC5883L_RANGE_4GA = 0b100,
    HMC5883L_RANGE_2_5GA = 0b011,
    HMC5883L_RANGE_1_9GA = 0b010,
    HMC5883L_RANGE_1_3GA = 0b001,
    HMC5883L_RANGE_0_88GA = 0b000
} hmc5883lres;

typedef enum
{
    HMC5883L_IDLE = 0b10,
    HMC5883L_SINGLE = 0b01,
    HMC5883L_CONTINOUS = 0b00
} hmc5883lopmode;
typedef enum
{
  HMC5883L_HIGH3200 = 0b1,
  HMC5883L_LOW = 0b0
}hmc5883li2cspeed;
typedef enum
{
  HMC5883L_NORMAL = 0b00
  HMC5883L_POSITIVE = 0b01,
  HMC5883L_NEGATIVE = 0b10
}hmc5883lmm;

class HMC5883L
{
  void write8(uint8_t reg,uint8_t value);
  uint8_t read8( uint8_t reg);
  int16_t read16(uint8_t reg);
  float Xo,Yo,Zo,magread[3],sf;
  
  
  public
  void offset(float x ,float y,float z);
  void sampling(hmc5883lsample sample);
  void DOR(hmc5883lDOR dataoutrate);
  void measuremode(hmc5883lmm mm);
  void resolution(hmc5883lres res);
  void highspeed(hmc5883li2cspeed i2cspeed);
  void HMC5883L::opmode(hmc5883lopmode opmode);
  bool begin();
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
