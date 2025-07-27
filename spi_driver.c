#include "spi_driver.h"
#include <stdio.h> // Include for printf in test functions

// =============================================================================
// 1. Hardware Register Simulation
// In a real embedded system, these would be pointers to memory addresses,
// e.g., #define SPCR (*(volatile uint8_t*)0x40001000)
// For this simulation, we use simple global variables.
// =============================================================================
uint8_t SPCR_REG = 0x00; // SPI Control Register
uint8_t SPSR_REG = 0x00; // SPI Status Register
uint8_t SPDR_REG = 0x00; // SPI Data Register
// This represents the data line coming FROM the slave device.
uint8_t MISO_LINE_DATA = 0x00;

// =============================================================================
// 2. Bit Definitions from Datasheet
// Create human-readable names for each bit position in the SPCR register.
// =============================================================================
#define SPE_BIT  6  // SPI Enable Bit
#define MSTR_BIT 4  // Master/Slave Select Bit
#define SPIF_BIT 7  // SPI Transfer Complete Flag Bit

// =============================================================================
// 3. Driver Function Implementation
// =============================================================================

void Spi_Init(void) {
    printf("DRIVER: Spi_Init() called.\n");

    // Configure the SPCR register according to the design.
    // Set the SPE bit (to enable SPI) and the MSTR bit (to set as master).
    SPCR_REG = (1 << SPE_BIT) | (1 << MSTR_BIT);

    printf("DRIVER: SPCR register configured to: 0x%02X\n", SPCR_REG);
}

void Spi_WriteByte(uint8_t data) {
    printf("DRIVER: Spi_WriteByte() called with data: 0x%02X\n", data);

    // 1. Write the data to the SPI Data Register to start the transfer.
    SPDR_REG = data;

    // --- Simulate the hardware setting the busy flag ---
    // In real hardware, this would be automatic. We do it manually.
    SPSR_REG |= (1 << 0); // Set BUSY bit
    printf("DRIVER: Hardware busy, starting transfer...\n");


    // 2. Poll the SPIF bit in the SPSR register.
    // We wait here until the hardware tells us the transfer is finished.
    // The 'while' loop condition checks if the SPIF bit is NOT set.
    while ( (SPSR_REG & (1 << SPIF_BIT)) == 0 ) {
        // This loop does nothing. It just waits.
    }

    // --- Simulate the hardware clearing the busy flag ---
    SPSR_REG &= ~(1 << 0); // Clear BUSY bit
    printf("DRIVER: Transfer complete!\n");
}

// Add the implementation for the new function after Spi_WriteByte()
uint8_t Spi_ReadWriteByte(uint8_t tx_data) {
    printf("DRIVER: Spi_ReadWriteByte() called with tx_data: 0x%02X\n", tx_data);

    // 1. Master places its data on the MOSI line (Master Out, Slave In)
    // In our simulation, this is like writing to the data register.
    SPDR_REG = tx_data;

    // --- Simulate hardware being busy ---
    SPSR_REG |= (1 << 0);
    printf("DRIVER: Hardware busy, starting transfer...\n");

    // 2. Poll the SPIF bit until the transfer is complete.
    while ( (SPSR_REG & (1 << SPIF_BIT)) == 0 ) {
        // Wait...
    }

    // --- Simulate hardware being idle again ---
    SPSR_REG &= ~(1 << 0);
    printf("DRIVER: Transfer complete!\n");

    // 3. Master latches the data from the MISO line (Master In, Slave Out)
    // In real hardware, the SPDR register would now hold this value.
    // Simulating this by copying the value from our fake MISO line.
    SPDR_REG = MISO_LINE_DATA;

    // 4. Return the byte that was received.
    printf("DRIVER: Returning received rx_data: 0x%02X\n", SPDR_REG);
    return SPDR_REG;
}