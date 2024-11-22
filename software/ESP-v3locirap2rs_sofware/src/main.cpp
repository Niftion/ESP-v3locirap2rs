// imports
#include <Arduino.h>
#include <Wire.h>

// constants
#define MPU6050_ADDR 0x68 // MPU-6050 I2C address
#define PWR_MGMT_1 0x6B // Power management register
#define AX_REG 0x3B // Accelerometer X-axis high byte

void setup() {

	// setup serial
	Serial.begin(115200);

	// setup power management
	Wire.begin();
	Wire.beginTransmission(MPU6050_ADDR);
	Wire.write(PWR_MGMT_1);
	Wire.write(0);
	Wire.endTransmission();
}

void loop() {

	// acceleration variables
	int16_t aX, aY, aZ;

	// Request accelerometer data
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(AX_REG); // read starting from acceleration X high byte
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6); // request 6 bytes for all X,Y,Z

    // read acceleration data, combining 2 bytes to each variable
    if (Wire.available() == 6) {
        aX = (Wire.read() << 8) | Wire.read();
        aY = (Wire.read() << 8) | Wire.read();
        aZ = (Wire.read() << 8) | Wire.read();
    }
	float aXg = aX/8192.0;
	float aYg = aY/8192.0;
	float aZg = aZ/8192.0;
	float magnitude = sqrt(aXg*aXg + aYg*aYg + aZg*aZg);

    // print acceleration data
    Serial.println(
		"Magnitude: " + String(magnitude) + " | "
		"Accel X: " + String(aXg) + " | "
		"Accel Y: " + String(aYg) + " | "
		"Accel Z: " + String(aZg) + " | "
		
	);

	// collect every half second
    delay(100);
}