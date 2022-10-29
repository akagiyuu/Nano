#include "data.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
static uint16_t IrBuffer[BUFFER_SIZE]; // infrared LED sensor data
static uint16_t RedBuffer[BUFFER_SIZE]; // red LED sensor data
#else
static uint32_t IrBuffer[BUFFER_SIZE]; // infrared LED sensor data
static uint32_t RedBuffer[BUFFER_SIZE]; // red LED sensor data
#endif

template <typename T, unsigned int size>
void shift_left(T (&array)[size], const unsigned int offset)
{
	for (unsigned int i = offset; i < size; ++i) {
		array[i - offset] = array[i];
	}
}

namespace Data
{

	void init(MAX30105 *ParticleSensor,
		int32_t *heart_rate,
		int8_t *is_valid_heart_rate,
		int32_t *spo2,
		int8_t *is_valid_spo2)
	{
		for (byte i = 0; i < BUFFER_SIZE; i++) {
			while ((*ParticleSensor).available() == false)
				(*ParticleSensor).check();

			RedBuffer[i] = (*ParticleSensor).getRed();
			IrBuffer[i] = (*ParticleSensor).getIR();
			(*ParticleSensor).nextSample();
		}

		maxim_heart_rate_and_oxygen_saturation(IrBuffer,
			BUFFER_SIZE,
			RedBuffer,
			spo2,
			is_valid_spo2,
			heart_rate,
			is_valid_heart_rate);
	}

	void read(MAX30105 *ParticleSensor,
		int32_t *heart_rate,
		int8_t *is_valid_heart_rate,
		int32_t *spo2,
		int8_t *is_valid_spo2)
	{
		shift_left(IrBuffer, SAMPLE_PER_CALCULATE);
		shift_left(RedBuffer, SAMPLE_PER_CALCULATE);

		for (byte i = 100 - SAMPLE_PER_CALCULATE; i < 100; i++) {
			read_raw(ParticleSensor, i);
		}

		// After gathering 25 new samples recalculate HR and SP02
		maxim_heart_rate_and_oxygen_saturation(IrBuffer,
			BUFFER_SIZE,
			RedBuffer,
			spo2,
			is_valid_spo2,
			heart_rate,
			is_valid_heart_rate);
	}

	void read_raw(MAX30105 *ParticleSensor, byte sample_index)
	{
		while ((*ParticleSensor).available() == false)
			(*ParticleSensor).check();

		digitalWrite(READ_LED, !digitalRead(READ_LED));

		RedBuffer[sample_index] = (*ParticleSensor).getRed();
		IrBuffer[sample_index] = (*ParticleSensor).getIR();
		(*ParticleSensor).nextSample();
		//
		// Serial.print("Ir: ");
		// Serial.print(IrBuffer[sample_index]);
		// Serial.print(", Red: ");
		// Serial.println(RedBuffer[sample_index]);
	}
}
