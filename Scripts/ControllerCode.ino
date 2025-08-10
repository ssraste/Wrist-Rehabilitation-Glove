#include <Wire.h>
#include <MPU6050_light.h>
#include <Stepper.h>

MPU6050 mpu(Wire);

const float STEPS_PER_REV = 32;     // Amount of gear reduction
const float GEAR_RED = 64;          // Gear reduction ratio
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED; // Steps per geared output rotation

// no. of steps per revolution
const int stepsPerRevolution = STEPS_PER_OUT_REV;

// Pins entered in sequence IN1–IN3–IN2–IN4 for Stepper
Stepper myStepper1(stepsPerRevolution, 8, 10, 9, 11);
Stepper myStepper2(stepsPerRevolution, 4, 6, 5, 7);

char data;

void setup() {
  Serial.begin(115200); 
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);

  while (status != 0) {
  }

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); 
  Serial.println("Done!");
  // mpu.upsideDownMounting = true; // Uncomment if MPU6050 is mounted upside down
}

void loop() {
  if (Serial.available()) {
    data = Serial.read();
  }

  if (data == 'u') {
    // Serial.println("Up");
    myStepper1.setSpeed(10);
    myStepper1.step(stepsPerRevolution / 6);
    // delay(1000);
  }
  else if (data == 'd') {
    myStepper1.setSpeed(10);
    myStepper1.step(-stepsPerRevolution / 6);
    // delay(1000);
  }

  if (data == 'l') {
    myStepper2.setSpeed(10);
    myStepper2.step(stepsPerRevolution / 6);
  }
  else if (data == 'r') {
    myStepper2.setSpeed(10);
    myStepper2.step(-stepsPerRevolution / 6);
  }

  mpu.update();

  int yaw = map(int(mpu.getAngleZ()), -24, 50, 800, 0);
  int pitch = map(int(mpu.getAngleY()), -45, 60, 600, 0);

  Serial.print("Yaw: ");
  Serial.print(yaw);
  Serial.print(", Pitch: ");
  Serial.println(pitch);

  delay(100); 
}
