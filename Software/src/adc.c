#include "adc.h"
#include <avr/io.h>
#include <stdint.h>

// SRC: https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// Pages: (4,205,208,211,212,217-220)

// (1<<ADEN)Enables ADC, (1<<ADSC) starts conversion,
// If we want to prioritize speed, set prescaler to 64 by removing | (1 << ADPS0);

void ADC_init(void){

    ADMUX = (1<< REFS0); // Uses AVCC as the ADC reference (~5V) and selects ADC0 (A0)

    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1 << ADPS0); // Enables ADC with a prescaler of 128(125khz clock speed)- accuracy > speed approach

    DIDR0 |= (1 << ADC0D); //Disables the digital input buffer on ADC0

}

uint16_t ADC_read(void){

    ADCSRA |= (1<<ADSC); //Starts ADC conversion

    while(ADCSRA & (1<<ADSC)); //waits for conversion to finish.

    return ADC;
}

//Converts the raw ADC value into voltage
float ADC_toVoltage(uint16_t raw, float referenceVoltage) {
    return (raw * referenceVoltage) / 1024.0f;
}