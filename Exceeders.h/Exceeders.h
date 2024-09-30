#ifndef Exceeders_h //give diff name later
#define Exceeders_h

#include "Arduino.h"
#include "Motor.h"

#include "NewPing.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <IRremote.h>


#define OFF 0
#define FORWARD 1
#define REVERSE -1

#define MAX_DISTANCE 200
#define MAX_LIFT 256 // # of cycles
#define MIN_LIFT 0


// pin#s
// will need to change later

// motor pins
// left
#define L1 8
#define L2 9
#define L3 10
#define L4 11
// right
#define R1 2
#define R2 3
#define R3 5
#define R4 6
// lift
#define Li1 4
#define Li2 7
#define Li3 12
#define Li4 13

// this is for waitForButton()
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4
#define BUTTON5 5

// sensors
#define TRIG 8
#define ECHO 7
#define IR 4

class Exceeders {
	public:
		Exceeders();
		void begin();
		
		// driving using left and right motor
		void straight(int motorSpeed, float rotations);
		void straight(int motorSpeed);
		void rotate(int motorSpeed, float rotations);
		void rotate(int motorSpeed);
		void drive(int leftSpeed, int rightSpeed, float rotations);
		void drive(int leftSpeed, int rightSpeed);
		void stop();
		
		// arm motor
		void calibrateLift(); 
		void liftTo(int motorSpeed, int percent);
		
		// sensor functions
		float getUSDistance();
		int getGyroDegrees();	// x angle?
		*int getIRBeaconDirection();
		*int getIRBeaconStrength();
		int getIRRemoteChannelButtons();
		*int getColourReflected();
		
		// button functions
		int waitForButton();
		bool getButtonPress(int buttonNum);
		
		// other
		void sleep(int seconds);
		*void print(int line, char[] text);
		*void printVar(int line, int var);
		*void printVar(int line, char var);
		*void printVar(int line, char[] var);
		*void printVar(int line, float var);
		*void printVar(int line, bool var);
		
	private:
		Motor leftMotor;
		Motor rightMotor;
		Motor liftMotor;
		
		NewPing sonar;
		Adafruit_MPU6050 mpu;
		IRrecv irrecv;
		decode_results results;
		
		float liftHeight;
		int straightCount;
		int rotateCount;
		int driveCount;
};

#endif