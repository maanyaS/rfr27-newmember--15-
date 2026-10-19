// TODO:
// 1. Define the multiplexer selection pins.
// 2. Configure the pins as outputs
// 3. Create MUX_SelectChannel function();
// 4. Implement selection for channels 0-15
// 5. Add functionality to prevent invalid channel selection
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "mux.h"

#define NUM_MUX_CHANNELS 16U

#define MUX_S0_PIN PD2
#define MUX_S1_PIN PD3
#define MUX_S2_PIN PD4
#define MUX_S3_PIN PD5


//Checks to see if selected channel is within range of 0-15
bool MUX_IsValidChannel(uint8_t channel){
    return channel < NUM_MUX_CHANNELS;
}

/*Port Registers src: https://docs.arduino.cc/retired/hacking/software/PortManipulation/
Port registers allow for lower-level and faster manipulation of the i/o pins of the microcontroller on an Arduino board.
The chips used on the Arduino board (the ATmega8 and ATmega168) have three ports:
B (digital pin 8 to 13)
C (analog input pins)
D (digital pins 0 to 7)
Each port is controlled by three registers, which are also defined variables in the arduino language.
The DDR register, determines whether the pin is an INPUT or OUTPUT. The PORT register controls whether the pin is HIGH or LOW,
and the PIN register reads the state of INPUT pins set to input with pinMode().
The maps of the ATmega8 and ATmega168 chips show the ports. The newer Atmega328p chip follows the pinout of the Atmega168 exactly.

DDR and PORT registers may be both written to, and read. PIN registers correspond to the state of inputs and may only be read.

PORTD maps to Arduino digital pins 0 to 7

DDRD - The Port D Data Direction Register - read/write

PORTD - The Port D Data Register - read/write

PIND - The Port D Input Pins Register - read only


Each bit of these registers corresponds to a single pin; e.g. the low bit of DDRB, PORTB, and PINB refers to pin PB0 (digital pin 8).
For a complete mapping of Arduino pin numbers to ports and bits,
see the diagram for your chip: ATmega8, ATmega168.
(Note that some bits of a port may be used for things other than i/o;
be careful not to change the values of the register bits corresponding to them.)
*/

void MUX_Init(void){
    //Initializes multiplexer to channel 0: S3, S2, S1, S0 = 0000
    MUX_SelectChannel(0);

    // Sets Arduino pins D2-D5 as output pins.
    DDRD |= (1 << MUX_S0_PIN);
    DDRD |= (1 << MUX_S1_PIN);
    DDRD |= (1 << MUX_S2_PIN);
    DDRD |= (1 << MUX_S3_PIN);

}

//Selects the channel and verifies if it is within the range of 0-15.
// If the channel is valid, it determines which pins need to be high or low using bitwise operations
void MUX_SelectChannel(uint8_t channel){
    if (!MUX_IsValidChannel(channel)){
        return;
    }
    // Set S0 HIGH or LOW
    if (channel & 1) {
        //digitalWrite(PD2, HIGH);
        PORTD |= (1 << MUX_S0_PIN);
} else{
    //Set S0 to LOW
    PORTD &= ~(1 << MUX_S0_PIN);
}
    // Set S1 HIGH or LOW
    if (channel & 2) {
        PORTD |= (1 << MUX_S1_PIN);
    } else {
        PORTD &= ~(1 << MUX_S1_PIN);
    }

    // Set S2 HIGH or LOW
    if (channel & 4) {
        PORTD |= (1 << MUX_S2_PIN);
    } else {
        PORTD &= ~(1 << MUX_S2_PIN);
    }

    // Set S3 HIGH or LOW
    if (channel & 8) {
        PORTD |= (1 << MUX_S3_PIN);
    } else {
        PORTD &= ~(1 << MUX_S3_PIN);
    }
}