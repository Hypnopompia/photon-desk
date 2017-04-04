/**
 ******************************************************************************
 * @file     photon-desk.cpp
 * @authors  TJ Hunter
 * @version  V1.0.0
 * @date     22-Oct-2015
 * @brief    Adjust the height of your geekdesk over WiFi using a Particle Photon
 ******************************************************************************
  Copyright (c) 2015 TJ Hunter.  All rights reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "application.h"

#define trigPin D6 // Trig pin on the HC-SR04
#define echoPin D5 // Echo pin on the HC-SR04

uint32_t readings[10];
uint32_t readingIndex = 0;

void setup() {
	// Initialize the HC-SR04
	pinMode(trigPin, OUTPUT);
	digitalWriteFast(trigPin, LOW);

	delay(50);
	Serial.begin(115200);
}

void loop() {
	delay(500);
	readPingSensor();
}

uint32_t avgReading() {
	uint32_t sum = 0, nonZero = 0, avg = 0;;
	for (uint32_t i = 0; i < 10; i++) {
		if (readings[i] > 0) nonZero++;
		sum += readings[i];
		Serial.print(readings[i]);
		Serial.print(" ");
	}

	if (sum > 0 && nonZero > 0) {
		avg = sum / nonZero;
	}

	Serial.print("= ");

	Serial.print(avg);

	Serial.println();
	return avg;
}

uint32_t readPingSensor() {
	uint32_t duration, trycount, cm, avg;
	uint32_t haveGoodReading = 0;
	uint32_t readingThreshold = 58 * 3; // about 3cm
	trycount = 0;

	do {
		// TODO: Maybe take a few readings and average them out? Throw out anomolies?
		duration = ping();
		cm = microsecondsToCentimeters(duration);

		avg = avgReading();

		if (cm >= 54 && cm <= 120 && (avg == 0 || abs(duration - avg) < readingThreshold ) ) { // less than 54cm or more than 120cm is likely an error. A reading more than 2cm from the average is probably also bad.
			readings[readingIndex++] = duration;
			if (readingIndex >= 10) readingIndex = 0;
			haveGoodReading = 1;
		} else {
			delay(50);
			Serial.print("Bad Reading: ");
			Serial.println(duration);
		}

	} while ( ( !haveGoodReading ) && trycount++ < 50);

	Serial.print(cm);
	Serial.print("cm, ");
	Serial.print(duration);
	Serial.println("us");

	return duration;
}

uint32_t ping() {
	uint32_t duration;
	pinMode(echoPin, INPUT);
	pinMode(trigPin, OUTPUT);

	// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
	digitalWriteFast(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWriteFast(trigPin, LOW);

	duration = pulseIn(echoPin, HIGH); // Time in microseconds to recieve a ping back on the echo pin

	return duration;
}

uint32_t microsecondsToInches(uint32_t microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

uint32_t microsecondsToCentimeters(uint32_t microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
