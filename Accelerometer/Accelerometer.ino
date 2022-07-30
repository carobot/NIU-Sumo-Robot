#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h" 

MMA8452Q accel;
int xAcceleration;
int yAcceleration;
int zAcceleration;

const int xThreshold=300;//adjust as needed
const int yThreshold=100;
const int zThreshold=100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  accel.init(SCALE_8G, ODR_800); // Init and customize the FSR and ODR

}

void loop() {
  accel.read();
  
  // Read Raw value
  
  xAcceleration = accel.x; // Read in raw x-axis acceleration data
  /*
  Serial.print("Acceleration on the x-axis is ");
  Serial.println(xAcceleration);
  yAcceleration = accel.y; // Read in raw x-axis acceleration data
  Serial.print("Acceleration on the y-axis is ");
  Serial.println(yAcceleration);
  zAcceleration = accel.z; // Read in raw x-axis acceleration data
  Serial.print("Acceleration on the z-axis is ");
  Serial.println(zAcceleration);
  */
  if (abs(xAcceleration) >xThreshold){
    Serial.println("Hit from x side");
  }
  
  //Read in calculated acceleration in unit of g
  
  //zAcceleration = accel.cz; // Read in calculated z-axis acceleration
  //Serial.print("Acceleration on the z-axis is: ");
  //Serial.print(zAcceleration);
  //Serial.println(" g's");

}
