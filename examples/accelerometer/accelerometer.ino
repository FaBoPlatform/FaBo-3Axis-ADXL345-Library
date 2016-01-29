#include "fabo-adxl345.h"
#include "Wire.h"

int x;
int y;
int z;
  
void setup()
{
  Serial.begin(9600); // シリアルの開始デバック用
  
  Serial.println("Checking I2C device...");
  
  if(fabo3Axis.searchDevice()){
    Serial.println("I am ADXL345");
  }
  Serial.println("Init...");
  fabo3Axis.configuration();
  fabo3Axis.powerOn();  
}

void loop() {

  fabo3Axis.readXYZ(&x,&y,&z);
  
  Serial.print("x: ");
  Serial.print( x );
  Serial.print(" y: ");
  Serial.print( y );
  Serial.print(" z: ");
  Serial.println( z );
  
  delay(100);
}
