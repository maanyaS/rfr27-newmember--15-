# Temperature Sensor Code

The code intends to:

1. Read all 16 temperature sensors using a multiplexer.
2. Convert the temperature sensor voltages into temperatures.
3. Store temperature data and report highest temperature readings.
4. Blink the red LED when a temperature exceeds 60℃.
5. Send temperature data using I²C.

# Project Structure

- src/main.c - Main program to import functions and for code to be executed.
- src/mux.c - Contains functions relating to our multiplexer.
- src/adc.c - Contains functions relating to ADC voltage and conversions.
- src/temperature.c - Contains functions to calculate temperature data.
- src/warnings.c - Contains functions to determine overheating and blink red LED.
- src/iic.c - Contains functions to handle I²C communication and send temperature data.