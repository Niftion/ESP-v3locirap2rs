#include <Arduino.h>
// #include <Wire.h>

#define LED_PIN 2

void setup() {
	// setup pins
	pinMode(LED_PIN, OUTPUT);
}

void loop() {
	// flash LED
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	delay(1000);
}