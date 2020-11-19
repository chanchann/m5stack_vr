// define must ahead #include <M5Stack.h>
//#define M5STACK_MPU6886 
 #define M5STACK_MPU9250 
// #define M5STACK_MPU6050
// #define M5STACK_200Q

#include <M5Stack.h>
#include <MadgwickAHRS.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
Madgwick filter;
unsigned long microsPerReading, microsPrevious;

// acc : accelerometer 加速度
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

// 角速度
float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

// pitch是围绕X轴旋转，也叫做俯仰角
// yaw是围绕Y轴旋转，也叫偏航角
// roll是围绕Z轴旋转，也叫翻滚角
float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;


// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  // return 0 is ok, return -1 is unknow
  int init_status = M5.IMU.Init();
  Serial.print(init_status);

  Serial.begin(115200);
  SerialBT.begin("M5Stack");

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);

  
  filter.begin(10);  // 10Hz  filter初始化

  microsPerReading = 1000000 / 10;
  microsPrevious = micros();

}

#define INTERVAL 5
float rolls[INTERVAL], pitchs[INTERVAL], yaws[INTERVAL];
int maidx = 0; // moving average index

// the loop routine runs over and over again forever
void loop() {
  if (micros() - microsPrevious >= microsPerReading) {
      // put your main code here, to run repeatedly:
    M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
    M5.IMU.getAccelData(&accX,&accY,&accZ);
    M5.IMU.getAhrsData(&pitch,&roll,&yaw);
    
    filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
    //  移动平均一下
    rolls[maidx % INTERVAL] = roll;
    pitchs[maidx % INTERVAL] = pitch;
    yaws[maidx % INTERVAL] = yaw;
    maidx++;
    for (int i = 0; i < INTERVAL; i++) {
      roll += rolls[i];
      pitch += pitchs[i];
      yaw += yaws[i];
    }
    roll /= INTERVAL;
    pitch /= INTERVAL;
    yaw /= INTERVAL;
    
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
    Serial.printf("%6.2f  %6.2f  %6.2f\r\n", gyroX, gyroY, gyroZ);
  //  SerialBT.printf("%6.2f  %6.2f  %6.2f\r\n", gyroX, gyroY, gyroZ);
  
    M5.Lcd.setCursor(220, 42);
    M5.Lcd.print(" o/s");
    
    M5.Lcd.setCursor(0, 65);
    M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
    Serial.printf(" %5.2f   %5.2f   %5.2f\r\n", accX, accY, accZ);
  //  SerialBT.printf("%6.2f  %6.2f  %6.2f\r\n", gyroX, gyroY, gyroZ);
  
    M5.Lcd.setCursor(220, 87);
    M5.Lcd.print(" G");
  
 
    M5.Lcd.setCursor(0, 110);
    M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
    Serial.printf(" %5.2f   %5.2f   %5.2fr\n", pitch, roll, yaw);
  //  SerialBT.printf(" %5.2f   %5.2f   %5.2fr\n", pitch, roll, yaw);
    SerialBT.printf("%5.2f, %5.2f, %5.2f\r\n", roll, pitch, yaw);
    microsPrevious = microsPrevious + microsPerReading;
  }
}
