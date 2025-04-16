/******************************************************************************
Basic_Readings.ino

https://github.com/sparkfun/SparkFun_Qwiic_6DoF_LSM6DSO
https://github.com/sparkfun/SparkFun_Qwiic_6DoF_LSM6DSO_Arduino_Library

Description:
Most basic example of use.

Example using the LSM6DSO with basic settings.  This sketch collects Gyro and
Accelerometer data every second, then presents it on the serial monitor.

Development environment tested:
Arduino IDE 1.8.2

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#define STEP_THRESHOLD 5.0f
#define X_INIT 0.0f
#define STEP_INIT 0
#define CALIBRATION_CYCLES 5
//#include "SPI.h"

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
float prevX;
int stepCounter;
float calibrationOffset;

void setup() {
  Serial.begin(9600);
  delay(500);

  
  
  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");


  // calibrate sensor 
  float calibrationSum = 0.0f;
  for (int i = 0; i < CALIBRATION_CYCLES; i++) {
    calibrationSum += myIMU.readFloatGyroX();
    delay(50);
  }

  calibrationOffset = calibrationSum / CALIBRATION_CYCLES; // find average offset
  

  prevX = X_INIT;
  stepCounter = STEP_INIT;
}


void loop()
{
  if (prevX != X_INIT) { // if a prev value was just read
    float currX = myIMU.readFloatGyroX();
    Serial.print("Curr x");
    Serial.println(currX);

    Serial.print("Prev x");
    Serial.println(prevX);

    if (abs(currX) - abs(prevX) >= STEP_THRESHOLD) { // if the magnitudes of the delta of the x gyro exceeds the threshold, add a step
      stepCounter++;
    }

    prevX = currX;

    Serial.println(prevX);
  } else { // if using gyro for first read, update with the 
    prevX = myIMU.readFloatGyroX();
  }

  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 3);

  Serial.print("Step count: ");
  Serial.println(stepCounter);

  Serial.println("------------------------------");
  
  delay(1000);
}