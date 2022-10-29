#include "HardwareSerial.h"
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <data.h>

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

int32_t spo2; // SPO2 value
int8_t is_valid_spo2; // indicator to show if the SPO2 calculation is valid
int32_t heart_rate; // heart rate value
int8_t is_valid_heart_rate; // indicator to show if the heart rate calculation is valid

byte pulseLED = 11; // Must be on PWM pin
byte readLED = 13;

void setup()
{
	Serial.begin(
		9600); // initialize serial communication at 115200 bits per second:

	pinMode(pulseLED, OUTPUT);
	pinMode(readLED, OUTPUT);

	// Initialize sensor
	if (!particleSensor.begin(
		    Wire, I2C_SPEED_FAST)) // Use default I2C port, 400kHz speed
	{
		Serial.println(F(
			"MAX30105 was not found. Please check wiring/power."));
		while (1)
			;
	}

	Serial.println(F(
		"Attach sensor to finger with rubber band. Press any key to start conversion"));
	while (Serial.available() == 0)
		; // wait until user presses a key
	Serial.read();

	byte ledBrightness = 60; // Options: 0=Off to 255=50mA
	byte sampleAverage = 4; // Options: 1, 2, 4, 8, 16, 32
	byte ledMode =
		2; // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
	byte sampleRate =
		100; // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
	int pulseWidth = 411; // Options: 69, 118, 215, 411
	int adcRange = 4096; // Options: 2048, 4096, 8192, 16384

	particleSensor.setup(ledBrightness,
		sampleAverage,
		ledMode,
		sampleRate,
		pulseWidth,
		adcRange); // Configure sensor with these settings

	Data::init(&particleSensor,
		&heart_rate,
		&is_valid_spo2,
		&spo2,
		&is_valid_spo2);
}

void loop()
{
	Data::read(&particleSensor,
		&heart_rate,
		&is_valid_spo2,
		&spo2,
		&is_valid_spo2);

	Serial.print("Heart rate: ");
	Serial.print(heart_rate);
	Serial.print(", SP O2: ");
	Serial.println(spo2);
}
