/*
   -- car remote --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.6 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.8.01 or later version;
     - for iOS 1.5.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Servo.h> 

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "FireTruck"
#define REMOTEXY_WIFI_PASSWORD "87654321"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,3,0,11,0,51,0,13,1,0,
  5,3,61,26,30,30,16,26,31,4,
  0,13,14,8,43,16,26,129,0,4,
  2,17,8,17,83,97,109,0,129,0,
  21,3,12,7,8,73,79,84,0,67,
  1,47,7,24,12,2,31,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_1_x; // =-100..100 x-coordinate joystick position 
  int8_t joystick_1_y; // =-100..100 y-coordinate joystick position 
  int8_t slider_1; // =0..100 slider position 

    // output variables
  char timer[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

Servo Servo1; 

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

/* defined the right motor control pins */
#define PIN_MOTOR_RIGHT_UP D1
#define PIN_MOTOR_RIGHT_DN D2
#define PIN_MOTOR_RIGHT_SPEED D5

/* defined the left motor control pins */
#define PIN_MOTOR_LEFT_UP D3
#define PIN_MOTOR_LEFT_DN D4
#define PIN_MOTOR_LEFT_SPEED D6


/* defined two arrays with a list of pins for each motor */
unsigned char RightMotor[3] = 
  {PIN_MOTOR_RIGHT_UP, PIN_MOTOR_RIGHT_DN, PIN_MOTOR_RIGHT_SPEED};
unsigned char LeftMotor[3] = 
  {PIN_MOTOR_LEFT_UP, PIN_MOTOR_LEFT_DN, PIN_MOTOR_LEFT_SPEED};

/*
   speed control of the motor
   motor - pointer to an array of pins
   v - motor speed can be set from -100 to 100
*/
void Wheel (unsigned char * motor, int v)
{
  if (v>70) v=70;
  if (v<-70) v=-70;
  
  if (v>0) {
//    Serial.print("pos, ");
    digitalWrite(motor[0], HIGH);
    digitalWrite(motor[1], LOW);
    analogWrite(motor[2], v*2.55);
  }
  else if (v<0) {
//    Serial.print("neg, ");
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], HIGH);
    analogWrite(motor[2], (-v)*2.55);
  }
  else {
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], LOW);
    analogWrite(motor[2], 0);
  }
}

int prev_t;
int timer;
bool started;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Starting!");
  
  RemoteXY_Init(); 

  Serial.println("initialized!");
  delay(500);
  /* initialization pins */
  pinMode (PIN_MOTOR_RIGHT_UP, OUTPUT);
  pinMode (PIN_MOTOR_RIGHT_DN, OUTPUT);
  pinMode (PIN_MOTOR_LEFT_UP, OUTPUT);
  pinMode (PIN_MOTOR_LEFT_DN, OUTPUT);

  Servo1.attach(D7);
  timer = 90;
  prev_t = millis();
  started = false;
  
  Serial.println("Ready!");
}

void loop() 
{ 
  RemoteXY_Handler();
  if ((!started) && (RemoteXY.joystick_1_y != 0))
  {
    started = true;
  }
  
  Servo1.write(map(RemoteXY.slider_1, 0, 100, 0, 90));

  /* manage the right motor */
  Wheel(RightMotor, RemoteXY.joystick_1_y - RemoteXY.joystick_1_x);
  /* manage the left motor */
  Wheel(LeftMotor, RemoteXY.joystick_1_y + RemoteXY.joystick_1_x);


  if (started && (millis() - prev_t) > 1000)
  {
    dtostrf(timer, 0, 2, RemoteXY.timer);
//    RemoteXY.timer = itoa(timer, RemoteXY.timer, 10);
    timer -= 1;
    if (timer < 0)
    {
      timer = 0;
    }
    prev_t = millis();
    Serial.println(timer);
  }

  if (timer == -2)
  {
//    RemoteXY.joystick_1_y = 0;
//    RemoteXY.joystick_1_x = 0;
    Wheel(RightMotor, 0);
    Wheel(LeftMotor, 0);
    delay(10000);
    timer = 90;
    started = false;
  }
//  sprintf(RemoteXY.score, "Score: %d", 3);
//  Serial.println(RemoteXY.score);
}
