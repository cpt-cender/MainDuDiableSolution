/*
 Name:		MainDuDiable.ino
 Created:	18/01/2019 11:58:46
 Author:	Axel
*/

#include <Servo.h>

Servo thumb1, thumb2, index1, index2, middle1, middle2, ring1, ring2, pinky1, pinky2;

int fingerPins[10] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9 };
int fingerVals[10];
const char *names[10] = { "Thumb1: ", "Thumb2: ", "Index1: ", "Index2: ", "Middle1: ", "Middle2: ", "Ring1: ", "Ring2: ", "Pinky1: ", "Pinky2: " };
int CalibrationVals[2][10];

int angles[10];
int val;
int flexPins[] = { A0,A1,A2,A3,A4,A5,A6,A7,A8,A9 };
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void setup() {
	Serial.begin(9600);
	thumb1.attach(5);
	thumb2.attach(6);
	index1.attach(7);
	index2.attach(8);
	middle1.attach(9);
	middle2.attach(10);
	ring1.attach(11);
	ring2.attach(12);
	pinky1.attach(13);
	pinky2.attach(14);
	CalibrateFlexSensors();
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void loop() {
	for (int i = 0; i < 10; i += 1) {
		val = analogRead(flexPins[i]);
		angles[i] = map(val, CalibrationVals[0][i], CalibrationVals[1][i], 0, 180);
		angles[i] = constrain(angles[i], 0, 180);
		Serial.print(angles[i]);
		Serial.print('\t');
		if (i == 0) { thumb1.write(angles[i]); }
		if (i == 1) { thumb2.write(angles[i]); }
		if (i == 2) { index1.write(angles[i]); }
		if (i == 3) { index2.write(angles[i]); }
		if (i == 4) { middle1.write(angles[i]); }
		if (i == 5) { middle2.write(angles[i]); }
		if (i == 6) { ring1.write(angles[i]); }
		if (i == 7) { ring2.write(angles[i]); }
		if (i == 8) { pinky1.write(angles[i]); }
		if (i == 9) { pinky2.write(angles[i]); }
	}
	Serial.println();
	delay(100);
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void CalibrateFlexSensors() {
	int calibrationCount = 0;
	while (calibrationCount <= 1) {
		if (calibrationCount == 0) {
			Serial.println("Please lay your hand flat");
			delay(3000);
			Serial.println("Collecting minimum flex readings");
		}
		else if (calibrationCount == 1) {
			Serial.println("Please close you hand into a fist");
			delay(3000);
			Serial.println("Collecting maximum flex readings");
		}
		delay(500);
		for (int i = 0; i < 10; i++) {
			int readingCount = 0;
			int readingSum = 0;
			while (readingCount < 10) {
				readingSum += analogRead(fingerPins[i]);
				readingCount += 1;
				delay(200);
			}
			CalibrationVals[calibrationCount][i] = readingSum / 10;
		}
		calibrationCount += 1;
	}
	Serial.println("Calibration Complete");
	delay(1000);
	Serial.println("In the order from thumb to pinky, your offsets are:");
	for (int i = 0; i < 2; i++) {
		if (i == 0) { Serial.print("Minimum Offsets: "); }
		else { Serial.print("Maximum Offsets: "); }
		for (int j = 0; j < 10; j++) {
			Serial.print(CalibrationVals[i][j]);
			Serial.print(", ");
		}
		Serial.println();
	}
}