#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

#define CCW -1
#define CW 1
#define MAXDELAY 10
#define MINDELAY 2

class Motor {
	public:
		Motor(int pin1, int pin2, int pin3, int pin4);
		void begin();
		void cycle();
		
		// getter and setter methods
		void setDir(int newDir);
		int getDir();
		void setSpeed(int newSpeed);
		int getSpeed();
	private:
		// output pins for motors
		int _pin1;
		int _pin2;
		int _pin3;
		int _pin4;
		
		int dir;	// directions of spin: forward, reverse
		int step;	// step of cycle: 0-3
		int speedm; // speed of motor: determines delay
}

#endif