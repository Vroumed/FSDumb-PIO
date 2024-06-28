#include "Adeept_Motor_For_ESP32.h"



/////////////////////PCA9685 drive area///////////////////////////////////
#define PCA9685_SDA 13  //Define SDA pins
#define PCA9685_SCL 14  //Define SCL pins
#ifndef PCA9685_ADDRESS
#define PCA9685_ADDRESS 0x40
#endif
#define SERVO_FREQUENCY 50       //Define the operating frequency of servo
#define PCA9685_CHANNEL_0 0      //Define PCA9685 channel to control servo 1
#define PCA9685_CHANNEL_1 1      //Define the PCA9685 channel to control servo 2
#define PCA9685_CHANNEL_2  2         //Define the PCA9685 channel to control servo 3
#define SERVO_MIDDLE_POINT 1500  //Define the middle position of the servo
#define MOTOR_SPEED_MIN -4095    //Define a minimum speed limit for wheels
#define MOTOR_SPEED_MAX 4095     //Define a maximum speed limit for wheels

#define PIN_MOTOR_M1_IN1 15      //Define the positive pole of M1
#define PIN_MOTOR_M1_IN2 14      //Define the negative pole of M1
#define PIN_MOTOR_M2_IN1 12       //Define the positive pole of M2
#define PIN_MOTOR_M2_IN2 13       //Define the negative pole of M2
#define PIN_MOTOR_M3_IN1 11      //Define the positive pole of M3
#define PIN_MOTOR_M3_IN2 10      //Define the negative pole of M3
#define PIN_MOTOR_M4_IN1 8      //Define the positive pole of M4
#define PIN_MOTOR_M4_IN2 9      //Define the negative pole of M4

PCA9685 pca9685;  //Instantiate a PCA9685 object

//PCA9685 initialization
void PCA9685_Setup(void) {
  Wire.begin(PCA9685_SDA, PCA9685_SCL);

  Wire.beginTransmission(PCA9685_ADDRESS);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  pca9685.setupSingleDevice(Wire, PCA9685_ADDRESS);
  pca9685.setToFrequency(SERVO_FREQUENCY);

}

//Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void ServoHeadY(float angle) {
  angle = constrain(angle, 65, 130);
  angle = map(angle, 0, 180, 102, 512);
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_0, int(angle));
}

//Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void ServoDirection(float angle) {
  angle = constrain(angle, 45, 135);
  angle = map(angle, 0, 180, 102, 512);
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_1, int(angle));
}

//Set the rotation parameters of servo 3, and the parameters are 0-180 degrees
void ServoHeadX(float angle) {
  angle = constrain(angle, 0, 180);
  angle = map(angle, 0, 180, 102, 512);
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_2, int(angle));
}
// A function to control the car motor
void Motor(int Motor_ID, int dir, int Motor_speed)
{
  if(dir > 0){dir = 1;}
  else {dir = -1;} 

  if (Motor_ID % 2 == 0)
  {
    dir *= -1;
  }

  Motor_speed = constrain(Motor_speed, 0, 100);
  Motor_speed = map(Motor_speed, 0,100, 0,4095);

  if(Motor_ID == 1)
  {
    if(dir == 1){
      pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, Motor_speed);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
    }
    else{
      pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, Motor_speed);
      }
    }
  else if(Motor_ID == 2)
   {
    if(dir == 1){
      pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, Motor_speed);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
    }
    else{
      pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, Motor_speed);
      }
    }
  else if(Motor_ID == 3)
   {
    if(dir == 1){
      pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, Motor_speed);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
    }
    else{
      pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, Motor_speed);
      }
    }

   else if(Motor_ID == 4)
   {
    if(dir == 1){
      pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, Motor_speed);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
    }
    else{
      pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
      pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, Motor_speed);
      }
    }
}

//A function to control the car motor
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed) {
  m1_speed = MOTOR_1_DIRECTION * constrain(m1_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX); // Adjust the direction of the motor.
  m2_speed = MOTOR_2_DIRECTION * constrain(m2_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m3_speed = MOTOR_3_DIRECTION * constrain(m3_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m4_speed = MOTOR_4_DIRECTION * constrain(m4_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

  if (m1_speed >= 0) {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, m1_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
  } else {
    m1_speed = -m1_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, m1_speed);
  }
  if (m2_speed >= 0) {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, m2_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
  } else {
    m2_speed = -m2_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, m2_speed);
  }
  if (m3_speed >= 0) {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, m3_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
  } else {
    m3_speed = -m3_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, m3_speed);
  }
  if (m4_speed >= 0) {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, m4_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
  } else {
    m4_speed = -m4_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, m4_speed);
  }
}
