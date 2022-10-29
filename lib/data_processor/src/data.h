#pragma once

#include "MAX30105.h"

#define SAMPLE_PER_CALCULATE 25
#define READ_LED 13

namespace Data
{

	void init(MAX30105 *ParticleSensor,
		int32_t *heart_rate,
		int8_t *is_valid_heart_rate,
		int32_t *spo2,
		int8_t *is_valid_spo2);

	void read(MAX30105 *ParticleSensor,
		int32_t *heart_rate,
		int8_t *is_valid_heart_rate,
		int32_t *spo2,
		int8_t *is_valid_spo2);

	void read_raw(MAX30105 *ParticleSensor, byte sample_index);
};
