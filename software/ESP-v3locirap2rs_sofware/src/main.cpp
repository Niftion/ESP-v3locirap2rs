/*
Junior Design I project

datasheet MPU6050 : https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
LiquidCrystal LCD help from : https://www.circuitschools.com/interfacing-16x2-lcd-module-with-esp32-with-and-without-i2c/
*/

// imports
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// constants
#define MPU6050_PWR_MGMT_1 0x6B // (MPU6050, p. 40)
#define MPU6050 0x68 // MPU-6050 reset register, bit 0-2 resets sensors (MPU6050, p. 8)
#define MPU6050_ACCEL_XOUT_H 0x3B // High byte of accelerometer X value (MPU6050, p. 29)
#define LSB_SENSITIVITY 8192.0 // For +-4g (MPU6050, p. 29)

// lcd with pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

void setup() {

	// setup serial
	Serial.begin(9600);

	// setup power management
	Wire.begin();
	Wire.beginTransmission(MPU6050);
	Wire.write(MPU6050_PWR_MGMT_1);
	Wire.write(0);
	Wire.endTransmission(true);

	// initialize the lcd
	lcd.begin(16, 2); // 16x2 screen
}

void loop() {

	// acceleration variables
	int16_t accel_xout, accel_yout, accel_zout;

	// Request accelerometer data
	Wire.beginTransmission(MPU6050);
	Wire.write(MPU6050_ACCEL_XOUT_H); // read starting from acceleration X high byte
	Wire.endTransmission(false);
	Wire.requestFrom(MPU6050, 6); // request 6 bytes for all X,Y,Z

	// read acceleration data, combining 2 bytes to each variable
	accel_xout = (Wire.read() << 8) | Wire.read();
	accel_yout = (Wire.read() << 8) | Wire.read();
	accel_zout = (Wire.read() << 8) | Wire.read();
	
	// assign acceleration values
	float accel_x = accel_xout / LSB_SENSITIVITY;
	float accel_y = accel_yout / LSB_SENSITIVITY;
	float accel_z = accel_zout / LSB_SENSITIVITY;
	float accel_m = sqrt(accel_x*accel_x + accel_y*accel_y + accel_z*accel_z); // magnitude

	// print acceleration data
	char disp_x[6], disp_y[6], disp_z[6], disp_m[6];
	sprintf(disp_x, "%5.2f", accel_x);
	sprintf(disp_y, "%5.2f", accel_y);
	sprintf(disp_z, "%5.2f", accel_z);
	sprintf(disp_m, "%5.2f", accel_m);

	// display acceleration to LCD screen
	lcd.setCursor(0, 0);
	lcd.print("Accel (g): " + String(disp_m)); // magnitude
	lcd.setCursor(0, 1);
	lcd.print(String(disp_x) + String(disp_y) + String(disp_z)); // directions

	// display acceleration to computer (if connected)
	Serial.println(
		"Magnitude: " + String(disp_m) + " | "
		"Accel X: " + String(disp_x) + " | "
		"Accel Y: " + String(disp_y) + " | "
		"Accel Z: " + String(disp_z) + " | "
	);

	// collect every 100ms (10 times a second)
	delay(100);
}