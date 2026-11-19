#include <Arduino.h>
#include <Wire.h>

extern "C" {
    #include "inc/adc.h"
    #include "inc/mux.h"
    #include "inc/temperature.h"
    #include "inc/blink.h"
    #include "inc/systime.h"
}

#define MUX_CHANNELS 8
#define REF_VOLTAGE 5.0f
#define TEST_VALUE 30


void onReq() {
    Wire.write((uint8_t) TEST_VALUE);
}

void setup() {
     // Configure A0 as an input
    DDRC &= ~(1 << PC0);
    PORTC &= ~(1 << PC0);

    // Initialize existing modules
    MUX_Init();
    ADC_init();
    sysTime_init();
    Blink_Init();

    Wire.begin(0x08);
    Wire.onRequest(onReq);
}

void loop() {

    // Reset value to false after each scan
        bool isOverHeating = false;

        for (uint8_t channel = 0; channel < MUX_CHANNELS; channel ++){

            MUX_SelectChannel(channel);

            // Allow multiplexer to settle
            _delay_us(100);

            // Read value
            uint16_t A0_val = ADC_read();

            // Convert ADC value into a voltage.
            float voltage = ADC_toVoltage(A0_val, REF_VOLTAGE);

            // Convert voltage into temperature
            float temperatureC = 0.0f;
            bool isValidTemp = SensorVoltageToTempC(voltage, &temperatureC);

            // Checks for overheating and updates LED
            if(isValidTemp && temperatureC > 60.0f){
                isOverHeating = true;

                Blink_Update(true);
            }

        }

        // Turn of if no temp sensors are overheating
            // Continues blinking if still overheating
            Blink_Update(isOverHeating);


}
