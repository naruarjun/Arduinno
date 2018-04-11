#include <Wire.h>
float gyroread[3];
float Xo=0,Yo=0,Zo=0;
float DegX=0,DegY=0,DegZ=0;
int read8(int reg)
{
  int value;
  Wire.beginTransmission(0x69);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0x69,1);
  while(Wire.available()==0);
  value=Wire.read();
  return value;
}
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
void getgyroreadings(float gyroread[]) {
  
  gyroread[0] = ((float)read16(0x28)-(float)Xo)*0.07f;
  gyroread[1] = ((float)read16(0x2A)-(float)Yo)*0.07f;
  gyroread[2] = ((float)read16(0x2C)-(float)Zo)*0.07f;
} 
void integrate(float dt)
{
  DegX = DegX + gyroread[0]*dt;
  DegY = DegY + gyroread[1]*dt;
  DegZ = DegZ + gyroread[2]*dt;
}

float Time=0.00,t=0.00;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  write8(0x20,0b10101111);
  write8(0x23,0x00110000);
for(int i=1;i<=50;i++)
  {
    getgyroreadings(gyroread);
  
  Xo = Xo +  gyroread[0];
  Yo = Yo +  gyroread[1];
  Zo = Zo +  gyroread[2];
  delay(50);
  }
  Xo=Xo/50.0;
  Yo=Yo/50.0;
  Zo=Zo/50.0;
 /* for(int i=1;i<=1000;i++)
  {
  getgyroreadings(gyroread);
  Time=(millis()-t)/1000.0;
  t=millis();
DegX=DegX + gyroread[0]*Time;
DegY=DegY + gyroread[1]*Time;
DegZ=DegZ + gyroread[2]*Time;
delay(10);
*/
  }
/*Xo = DegX/1000.0;
Yo = DegY/1000.0;
Zo = DegZ/1000.0  ;
}*/
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
 t=millis();
void loop() {
//  // put your main code here, to run repeatedly:
//  Time=millis();

 Time=(millis()-t)/1000.0;
getgyroreadings(gyroread);  
DegX=(DegX + gyroread[0]*Time);
DegY=(DegY + gyroread[1]*Time);
DegZ=(DegZ + gyroread[2]*Time);

Serial.print(DegX);
Serial.print(" ");
Serial.print(DegY);
Serial.print(" ");
Serial.println(DegZ);
delay(10);
t=millis();
}
