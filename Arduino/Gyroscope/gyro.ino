float gyroread[3];
float Xo=0,Yo=0,Zo=0;
uint8_t read8(uint8_t reg)
{
  uint8_t value;
  Wire.beginTransmission(0b11010001);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0b11010011,1);
  while(Wire.available()==0);
  value=Wire.read();
  return value;
}
int read16(uint8_t reg)
{
  int value;
  uint8_t l,h;
  Wire.beginTransmission(0b11010011);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0b11010011);
  while(Wire.available()==0);
  
    l=Wire.read();
    h=Wire.read();
  }
  value = ((l<<8)|h);
  return value;

}
void write8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(0b11010011);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
void getgyroreadings(float gyroread[]) {
  
  gyroread[0] = (float)read16(0x28)-(float)Xo;
  gyroread[1] = (float)read16(0x2A)-(float)Yo;
  gyroread[2] = (float)read16(0x2C)-(float)Zo;
} 
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  write8(0x20,0b00001111);
  for(i=1;i<=1000;i++)
  {
  getGyroReadings(gyroread);
  Xo += gyroread[0];
  Yo += gyroread[1];
  Zo += gyroread[2];
  }
  Xo=Xo/1000.0;
  Yo=Yo/1000.0;
  Zo=Zo/1000.0;
}
/*
 * number = number - 1;
 * l=number;
 * count=0
 * while(l>0)
 * {count=count+1;
 * l=l/10;}
 * l=number;
 * int a[count]={0};
 * 
 * for(i=0;i<count;i++)
 * {int rem = l%10;
 * a[count-1-i]=rem;
 * l=l/10;
 * }
 */


void loop() {
  // put your main code here, to run repeatedly:
  getgyroreadings(gyroread);
  /*
   * 
   */
}
