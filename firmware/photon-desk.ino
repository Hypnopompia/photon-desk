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

#define trigPin D2 // Trig pin on the HC-SR04
#define echoPin D6 // Echo pin on the HC-SR04
#define upPin D4   // Pin connected to the Up button on the smartdesk via an opto-isolator
#define downPin D5 // Pin connected to the Down button on the smartdesk via an opto-isolator

uint32_t lastCm, targetCm;
bool movingUp, movingDown;

unsigned long movingStartTime;    // Keep track of when we started moving so we can timeout
unsigned long movingTimeOut = 30; // # of seconds; Stop trying to move after moving for this many seconds in case something bad happens

void setup() {
	Particle.function("getHeight", getHeight);
	Particle.function("setHeight", setHeight);

	// Initialize the HC-SR04
	pinMode(trigPin, OUTPUT);
	digitalWriteFast(trigPin, LOW);

	// Initialize the buttons
	pinMode(upPin, OUTPUT);
	pinMode(downPin, OUTPUT);
	stop();

	delay(50);
	Serial.begin(115200);
}

void loop() {
	// Timeout Check
	if ( (movingUp || movingDown) && ( (millis() - movingStartTime) / 1000 >= movingTimeOut ) ) {
		Serial.println("Timeout! Stop.");
		stop();
	}

	// Target height check
	if ( (movingUp   && microsecondsToCentimeters(readPingSensor()) >= targetCm) ||
	     (movingDown && microsecondsToCentimeters(readPingSensor()) <= targetCm)
	) { // We've reached our target height. Stop everything!
		Serial.println("Target height reached. Stop.");
		stop();
	}
}

void goUp() {
	Serial.println("Going Up");
	stop();
	digitalWrite(upPin, HIGH);
	movingUp = true;
	movingStartTime = millis();
}

void goDown() {
	Serial.println("Going Down");
	stop();
	digitalWrite(downPin, HIGH);
	movingDown = true;
	movingStartTime = millis();
}

void stop() {
	digitalWrite(upPin, LOW);
	digitalWrite(downPin, LOW);
	movingUp = false;
	movingDown = false;
}

int getHeight(String command) {
	lastCm = 0; // Force a debug print to Serial
	return microsecondsToCentimeters(readPingSensor());
}

int setHeight(String command) {
	uint32_t currentHeight = microsecondsToCentimeters(readPingSensor());
	targetCm = command.toInt();

	// TODO: We could calculate the distance we need to travel and using the known
	// travel speed of the desk, set a more appropriate timeout rather than hardcoding it.

	if (currentHeight > targetCm) {
		goDown();
	} else if (currentHeight < targetCm) {
		goUp();
	}

	return 1;
}

int readPingSensor() {
	uint32_t duration, trycount, cm;
	trycount = 0;

	do {
		// TODO: Maybe take a few readings and average them out? Throw out anomolies?
		duration = ping();
	} while (microsecondsToCentimeters(duration) > 400 && trycount++ < 50); // More than 400cm is likely an error. Try again

	/* Debugging output */
	cm = microsecondsToCentimeters(duration);

	if (cm != lastCm) {
		lastCm = cm;
		Serial.print(cm);
		Serial.print("cm, ");
		Serial.print(duration);
		Serial.println("us");
	}

	return duration;
}

int ping() {
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
