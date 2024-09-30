#include "Arduino.h"
#include "Exceeders.h"

Exceeders::Exceeders() {	
	leftMotor = Motor(L1, L2, L3, L4);
	rightMotor = Motor(R1, R2, R3, R4);
	liftMotor = Motor(Li1, Li2, Li3, Li4);
	
	sonar(TRIG, ECHO, MAX_DISTANCE);
	irrecv (IR);

	liftHeight = 0;
	straightCount = 0;
	rotateCount = 0;
	driveCount = 0;
}

Exceeders::begin() {
	leftMotor.begin();
	rightMotor.begin();
	liftMotor.begin();
	
	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(BUTTON3, INPUT);
	pinMode(BUTTON4, INPUT);
	pinMode(BUTTON5, INPUT);
	
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
	
	irrecv.enableIRIn();
}

// straight(motorSpeed, rotations) allows robot to drive straight given distance
void Exceeders::straight(int motorSpeed, float rotations) {
	// sets states of speedm and dir per motor
	straight(motorSpeed);

	for (int i = 0; i < rotations * 2048; i++){
		cycle();
	}
}

// rotate(motorSpeed, rotations) allows robot to turn given distance
void Exceeders::rotate(int motorSpeed, float rotations) {
	// sets states of speedm and dir per motor
	rotate(motorSpeed);

	for (int i = 0; i < rotations * 2048; i++){
		cycle();
	}
}

// drive(leftSpeed, rightSpeed, rotations) allows robot to drive given distance
// requires: rotations >= 0
void Exceeders::drive(int leftSpeed, int rightSpeed, float rotations) {
	// sets states of speedm and dir per motor
	drive(leftSpeed, rightSpeed);
	
	for (int i = 0; i < rotations * 2048; i++){
		cycle();
	}
}

// straight(motorSpeed) sets motor speeds and directions
void Exceeders::straight(int motorSpeed) {
	leftMotor.setSpeed(abs(motorSpeed));
	rightMotor.setSpeed(abs(motorSpeed));
	
	if (motorSpeed > 0) {
		leftMotor.setDir(CCW);
		rightMotor.setDir(CW);
	} else {
		leftMotor.setDir(CW);
		rightMotor.setDir(CCW);
	}

	if (straightCount != 0) {
		cycle();
	}
	straightCount = 1;
	rotateCount = 0;
	driveCount = 0;
}

// rotate(motorSpeed) sets motor speeds and directions
void Exceeders::rotate(int motorSpeed) {
	leftMotor.setSpeed(abs(motorSpeed));
	rightMotor.setSpeed(abs(motorSpeed));
	
	if (motorSpeed > 0) {
		leftMotor.setDir(CCW);
		rightMotor.setDir(CCW);
	} else if (motorSpeed < 0) {
		leftMotor.setDir(CW);
		rightMotor.setDir(CW);
	}

	if (rotateCount != 0) {
		cycle();
	}
	rotateCount = 1;
	straightCount = 0;
	driveCount = 0;
}

// drive(leftSpeed, rightSpeed) sets motor speeds and directions
void Exceeders::drive(int leftSpeed, int rightSpeed) {
	leftMotor.setSpeed(abs(leftSpeed));
	rightMotor.setSpeed(abs(rightSpeed));
	if (leftSpeed > 0) {
		leftMotor.setDir(CCW);
	} else if (leftSpeed < 0) {
		leftMotor.setDir(CW);
	}
	if (rightSpeed > 0) {
		rightMotor.setDir(CW);
	} else if (rightSpeed < 0) {
		rightMotor.setDir(CCW);
	}

	if (driveCount != 0) {
		cycle();
	}
	driveCount = 1;
	straightCount = 0;
	rotateCount = 0;
}

// stop() stops robot
void Exceeders::stop() {
	leftMotor.setSpeed(0);
	rightMotor.setSpeed(0);
}

// sleep(seconds) runs cycle for seconds
// requires: seconds >= 0
void Exceeders::sleep(int seconds) {
	if (speed != 0) {
		unsigned double start = millis();
		while (millis() < seconds*1000 + start) { //find way to track seconds passed
			cycle();
		}
	} else {
		delay(seconds * 1000);
	}
}

// waitForButton() waits until button pressed and returns int corresponding 
//		with button pressed
int Exceeders::waitForButton() {
	while (true) {
		int buttonpressed1 = digitalRead(BUTTON1);
		if (buttonpressed1) {
			return 1;
		}
		int buttonpressed2 = digitalRead(BUTTON2);
		if (buttonpressed2) {
			return 2;
		}
		int buttonpressed3 = digitalRead(BUTTON3);
		if (buttonpressed3) {
			return 3;
		}
		int buttonpressed4 = digitalRead(BUTTON4);
		if (buttonpressed4) {
			return 4;
		}
		int buttonpressed5 = digitalRead(BUTTON5);
		if (buttonpressed5) {
			return 5;
		}
		cycle();
	}
}

// getButtonPress(buttonNum) returns true if button pressed and false otherwise
bool Exceeders::getButtonPress(int buttonNum) {
	int buttonPin;		// stores pin of button
	if (buttonNum == 1) {
		buttonPin = BUTTON1;
	} else if (buttonNum == 2) {
		buttonPin = BUTTON2;
	} else if (buttonNum == 3) {
		buttonPin = BUTTON3;
	} else if (buttonNum == 4) {
		buttonPin = BUTTON4;
	} else {
		buttonPin = BUTTON5;
	}
	return digitalRead(buttonPin);
}

// calibrateLift() shows maximum and minimum lift height
void Exceeders::calibrateLift() {
	liftMotor.setSpeed(100);

	// arm goes down for 2 seconds
	liftMotor.setDir(CCW);
	sleep(2);

	// arm goes up for 2 seconds
	liftMotor.setDir(CW);
	sleep(2);

	// arm goes down for 2 seconds
	liftMotor.setDir(CCW);
	sleep(2);
}

// liftTo(motorSpeed, percent) lifts arm motor to percent
void Exceeders::liftTo(int motorSpeed, int percent) {
	// throw error if calibrateLift() not called

	liftMotor.setSpeed(motorSpeed);
	if (liftHeight > percent) {
		liftMotor.setDir(CCW);
	} else {
		liftMotor.setDir(CW);
	}

	for (int i = 0; i < map(abs(liftHeight - percent), 0, 100, MIN_LIFT, MAX_LIFT); ++i) {
		cycle();
	}
}

//getUSDistance() returns distance read by ultrasonic sensor in mm
float Exceeders::getUSDistance() {
	return sonar.ping_cm() * 10;
}

// getGyroDegrees() returns angle moved on x axis
int Exceeders::getGyroDegrees() { 			// need to test
	sensors_event_t accel, gyroscope;
	mpu.getEvent(&accel, &gyroscope, NULL);
	return gyroscope.gyro.x;	// in rad/s
}


// getIRRemoteChannelButtons() returns the buttons pressed on IR remote
int Exceeders::getIRRemoteChannelButtons() {
	while (true) {
		if (irrecv.decode(&results)){
			Serial.println(results.value, HEX);
			irrecv.resume();
			
			if (results.value == 0xFF629D && results.value == 0xFF22DD) { 	     // up and left
				return 5;
			} else if (results.value == 0xFF629D && results.value == 0xFFC23D) { // up and right
				return 8;
			} else if (results.value == 0xFFA857 && results.value == 0xFF22DD) { // up and left
				return 5;
			} else if (results.value == 0xFFA857 && results.value == 0xFFC23D) { // up and right
				return 5;
			} else if (results.value == 0xFF629D) {	 // up arrow
				return 1;
			} else if (results.value == 0xFFA857) {	// down arrow
				return 2;
			} else if (results.value == 0xFF22DD) {	// left arrow
				return 3;
			} else if (results.value == 0xFFC23D) {	// right arrow
				return 4;
			} 
		}
		cycle();
	}
}

/****************************************************************************************************************/

// printVar(line, var) prints int variable
void Exceeders::printVar(int line, int var) {
	// print out var
}

// printVar(line, var) prints char variable
void Exceeders::printVar(int line, char var) {
	// print out var
}

// printVar(line, var) prints float variable
void Exceeders::printVar(int line, float var) {
	// print out var
}

// printVar(line, var) prints boolean variable
void Exceeders::printVar(int line, bool var) {
	// print out var
}

// print(line, text) prints user input
void print(int line, char[] text) {
	
}

/****************************************************************************************************************/