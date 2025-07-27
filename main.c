#include <stdio.h>
#include <stdint.h>
#include "spi_driver.h"

// Declare all simulated hardware registers
extern uint8_t SPCR_REG;
extern uint8_t SPSR_REG;
extern uint8_t SPDR_REG;
extern uint8_t MISO_LINE_DATA;

int main() {
    uint8_t received_data;

    printf("APP: Starting Full SPI Read/Write Test.\n");
    Spi_Init();
    printf("----------------------------------------\n");

    // --- Test Scenario ---
    // Read data from a sensor.
    // Send the command "READ_SENSOR" (0x80).
    // The sensor will respond with the sensor value (e.g., 0xC5).

    printf("APP: Sending command 0x80 to sensor...\n");

    // --- Simulate the sensor's response ---
    // We set our fake MISO line to what the sensor would send.
    printf("APP: Simulating sensor response of 0xC5 on MISO line.\n");
    MISO_LINE_DATA = 0xC5;

    // We still need to set the SPIF flag to allow the driver's loop to finish.
    SPSR_REG |= (1 << 7);

    // Call our driver function to send the command and get the response
    received_data = Spi_ReadWriteByte(0x80);

    printf("APP: Received data from sensor: 0x%02X\n", received_data);

    // Check if we received the correct data
    if (received_data == 0xC5) {
        printf("APP: SUCCESS! Correct data received from sensor.\n");
    } else {
        printf("APP: FAILED! Incorrect data received.\n");
    }

    return 0;
}