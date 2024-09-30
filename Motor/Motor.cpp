#include "Arduino.h"
#include "Motor.h"

// Motor(pin1, pin2, pin3, pin4) sets pins of motor
Motor::Motor(int pin1, int pin2, int pin3, int pin4) {
	_pin1 = pin1;
	_pin2 = pin2;
	_pin3 = pin3;
	_pin4 = pin4;
	
	dir = 0;
	step = 0;
	speedm = 0;
}

// begin() initializes motor pins
void Motor::begin() {
	pinMode(_pin1, OUTPUT);
	pinMode(_pin2, OUTPUT);
	pinMode(_pin3, OUTPUT);
	pinMode(_pin4, OUTPUT);
}

// cycle() moves motor
void Motor::cycle() {
	if (speedm != 0) {
		// step of cycle
		if (step == 0) {
			digitalWrite(_pin1, HIGH);
			digitalWrite(_pin2, LOW);
			digitalWrite(_pin3, LOW);
			digitalWrite(_pin4, LOW);
		} else if (step == 1) {
			digitalWrite(_pin1, LOW);
			digitalWrite(_pin2, HIGH);
			digitalWrite(_pin3, LOW);
			digitalWrite(_pin4, LOW);
		} else if (step == 2) {
			digitalWrite(_pin1, LOW);
			digitalWrite(_pin2, LOW);
			digitalWrite(_pin3, HIGH);
			digitalWrite(_pin4, LOW);
		} else {
			digitalWrite(_pin1, LOW);
			digitalWrite(_pin2, LOW);
			digitalWrite(_pin3, LOW);
			digitalWrite(_pin4, HIGH);
		}
		// delay after each step, depends on speed
		delay(map(speedm, 0, 100, MINDELAY, MAXDELAY));

		// changes step based on direction of spin
		if (dir == CW) {	// clockwise
			if (step == 0) {
				step = 3;
			} else {
				--step;
			}
		} else {			// counterclockwise
			if (step == 3) {
				step = 0;
			} else {
				++step;
			}
		}
	}
}

void setDir(int newDir) {
	dir = newDir;
}

int getDir() {
	return dir;
}

void setSpeed(int newSpeed) {
	speedm = newSpeed;
}

int getSpeed() {
	return speedm;
}