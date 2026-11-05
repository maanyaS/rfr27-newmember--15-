// Arduino clock speed 16mhz
#define CLOCK_SPEED 16000000UL
#define F_CPU CLOCK_SPEED

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "adc.h"
#include "mux.h"
#include "blink.h"
#include "systime.h"
#include "temperature.h"

// Constants
#define MUX_CHANNELS 16
#define REF_VOLTAGE 5.0f

int main(void){

    //Configure port A0 as an input
    DDRC &= ~(1 << PC0);
    PORTC &= ~(1 << PC0);

    // Initialize modules
    MUX_Init();
    ADC_init();
    sysTime_init();
    Blink_Init();

    sei();

    while (1) {

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
}