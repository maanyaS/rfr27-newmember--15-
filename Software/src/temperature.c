#include "temperature.h"
#include <stdint.h>
#include <stdbool.h>

// Source: https://enepaq.com/wp-content/uploads/2025/02/Sony-Murata-VTC5A-Li-ion-Battery-Module-With-Temperature-Sensor-Datasheet-.pdf
// rfr/firmware


bool SensorVoltageToTempC(float voltage, float *temperatureC) {

    // Stores temp values in Celsius from datasheet
    static const float temp_table[] = {
        -40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10,
        15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65,
        70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120
    };
    // Stores sensor voltages for each temperature from datasheet
    static const float volt_table[] = {
        2.44f, 2.42f, 2.40f, 2.38f, 2.35f, 2.32f,
        2.27f, 2.23f, 2.17f, 2.11f, 2.05f, 1.99f,
        1.92f, 1.86f, 1.80f, 1.74f, 1.68f, 1.63f,
        1.59f, 1.55f, 1.51f, 1.48f, 1.45f, 1.43f,
        1.40f, 1.38f, 1.37f, 1.35f, 1.34f, 1.33f,
        1.32f, 1.31f, 1.30f
    };

    // Counts number of values in temp table
    const uint8_t n = sizeof(temp_table) / sizeof(temp_table[0]);

    // Make sure the pointer output is not null
    if (temperatureC == 0) {
        return false;
    }

    // Reject voltages outside the datasheet's range as well as NaN values
    if (!(voltage >= volt_table[n - 1] &&
          voltage <= volt_table[0])) {
        return false;
    }

    // Find the two voltages surrounding the measured voltage
    for (uint8_t i = 0; i < n - 1; i++) {

        if ((voltage <= volt_table[i]) &&
            (voltage >= volt_table[i + 1])) {

            float v1 = volt_table[i];
            float v2 = volt_table[i + 1];
            float t1 = temp_table[i];
            float t2 = temp_table[i + 1];

            // Calculate temperature using linear interpolation
            *temperatureC = t1 + (voltage - v1) *
                            (t2 - t1) / (v2 - v1);

            return true;
        }
    }

    return false;
}
