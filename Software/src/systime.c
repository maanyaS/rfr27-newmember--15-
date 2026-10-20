
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "systime.h"

// Tracks milliseconds elapsed since initialization.
// Volatile is needed because this variable is constantly changing and should not be optimized
static volatile uint32_t ms_counter = 0;

void sysTime_init(void) {

    // Stop Timer0 during configuration
    TCCR0B = 0;

    // Disable Timer0 compare interrupt temporarily
    TIMSK0 &= ~(1 << OCIE0A);

   /* WAVEFORM GENERATION MODE
    SRC: https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    (Pages 79, 86)

    Normal mode: Timer0 counts from 0 to 255 (256 ticks).
    Each tick takes 4 microseconds with our prescaler.
    256 * 4 us = 1024 us = 1.024 ms

    We want exactly 1 ms, so we use CTC mode.

    CTC mode: Timer0 counts from 0 through 249.
    This gives 250 ticks before the counter repeats.

    250 * 4 us = 1000 us = 1 ms
    */

    // Set Timer0 to CTC mode
    // WGM02 = 0, WGM01 = 1, WGM00 = 0
    TCCR0A = (1 << WGM01);

    // Start counting from zero
    TCNT0 = 0;
    ms_counter = 0;

    // PRESCALER

    // Arduino Nano clock speed = 16 MHz
    // 16,000,000 clock cycles per second

    // Prescaler divides the timer clock by 64:

    // 16,000,000 / 64 = 250,000 timer ticks/second
    // 250,000 / 1000 = 250 timer ticks/millisecond

    // One timer tick takes:
    // 1 / 250,000 = 0.000004 seconds = 4 us

    // Set comparison value to 249 (250 counts)
    OCR0A = 249;

    // Clear any pending compare-match interrupt flag
    TIFR0 = (1 << OCF0A);

    // Enable Timer0 compare-match interrupt
    TIMSK0 |= (1 << OCIE0A);

    // Set prescaler to 64 and start Timer0
    TCCR0B |= (1 << CS01);
    TCCR0B |= (1 << CS00);
    TCCR0B &= ~(1 << CS01);

}

// INTERRUPT SERVICE ROUTINE

// Executes every 1 millisecond.
// Increments our system time counter.
ISR(TIMER0_COMPA_vect) {
    ms_counter++;
}

// Return elapsed milliseconds
uint32_t systime(void) {

    uint32_t time;

    // ATOMIC_BLOCK prevents interrupts from changing ms_counter while we read its 32-bit value.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        time = ms_counter;
    }

    return time;
}
