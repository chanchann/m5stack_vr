#include "MPU9250.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
MPU9250 mpu;
unsigned long microsPerReading, microsPrevious;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.setup();
    SerialBT.begin("ESP32"); 

    // calibrate anytime you want to
    mpu.calibrateAccelGyro();
    mpu.calibrateMag();
    microsPerReading = 1000000 / 100;
    microsPrevious = micros();
}

#define INTERVAL 5
float rolls[INTERVAL], pitchs[INTERVAL], yaws[INTERVAL];
int maidx = 0; // moving average index
bool once = true;

void loop() {
  if (micros() - microsPrevious >= microsPerReading) {
    mpu.update();
//    mpu.print();
//    while(!mpu.available());
    rolls[maidx % INTERVAL] = mpu.getRoll();
    pitchs[maidx % INTERVAL] = mpu.getPitch();
    yaws[maidx % INTERVAL] = mpu.getYaw();
    maidx++;

    float roll = 0.0;
    float pitch = 0.0;
    float yaw = 0.0;
    for (int i = 0; i < INTERVAL; i++) {
        roll += rolls[i];
        pitch += pitchs[i];
        yaw += yaws[i];
    }
    roll /= INTERVAL;
    pitch /= INTERVAL;
    yaw /= INTERVAL;
    Serial.printf("%6.2f, %6.2f, %6.2f\r\n", roll, pitch, yaw);
    SerialBT.printf("%6.2f, %6.2f, %6.2f\r\n", roll, pitch, yaw);
    
    microsPrevious = microsPrevious + microsPerReading;
  }
}
