#ifndef CAR_H
#define CAR_H

#include <PCA9685.h>

#define MOTOR_1_DIRECTION     1 //If the direction is reversed, change 1 to -1
#define MOTOR_2_DIRECTION     1 //If the direction is reversed, change 1 to -1
#define MOTOR_3_DIRECTION     1 //If the direction is reversed, change 1 to -1
#define MOTOR_4_DIRECTION     1 //If the direction is reversed, change 1 to -1

void PCA9685_Close_Com_Address(void);//Close the PCA9685 public address

/////////////////////PCA9685 drive area//////////////////////////////////////
void PCA9685_Setup(void);              //servo initialization
void ServoHeadY(float angle);       //Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void ServoDirection(float angle);       //Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void ServoHeadX(float angle);       //Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed);//A function to control the car motor
void Motor(int Motor_ID, int dir, int Motor_speed);//A function to control the car motor


#endif
