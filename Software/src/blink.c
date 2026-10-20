#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "blink.h"
#include "systime.h"

// Time between LED state changes, in milliseconds.
#define BLINK_INTERVAL_MS 500U

// Tracks the last time the LED changed state
static uint32_t lastTime = 0;

static bool isBlinking = false;

void Blink_Init(void) {

    // Configure D7 (PD7) as an output
    DDRD |= (1 << PD7);

    // Start with warning LED OFF
    PORTD &= ~(1 << PD7);

    // Reset blink state
    isBlinking = false;
    lastTime = 0;
}

// Receives temperature data and determines if overheating, if not keeps LED OFF, if overheating turns on led immediately,
// continues to blink every 500ms while overheating
void Blink_Update(bool overTemp) {

    // Get current time from systime.c
    uint32_t currentTime = systime();

    if (!overTemp) {
    // Temperature is safe; turn LED OFF
    PORTD &= ~(1 << PD7);
    isBlinking = false;
    lastTime = currentTime;

} else if (!isBlinking) {
    // Overheating detected, turn on LED
    PORTD |= (1 << PD7);
    isBlinking = true;
    lastTime = currentTime;

} else {
    // Still overheating, continue blinking every 500ms
    if (currentTime - lastTime >= BLINK_INTERVAL_MS) {
        PORTD ^= (1 << PD7);
        lastTime = currentTime;
    }
}
}
