#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdbool.h>

// Converts voltage to temperature in Celsius.
// Returns true if the voltage is valid and stores value in temperatureC
bool SensorVoltageToTempC(float voltage, float *temperatureC);

#endif