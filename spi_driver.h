#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h> // Use standard integer types for portability

/**
 * @brief Initializes the SPI peripheral.
 *
 * This function configures the SPI module to be enabled and set as the master device.
 * It must be called before any other SPI functions.
 */
void Spi_Init(void);

/**
 * @brief Sends a single byte over the SPI bus.
 *
 * This function writes a byte to the data register and then waits (polls)
 * until the hardware confirms the transfer is complete.
 * @param data The 8-bit data byte to send.
 */
void Spi_WriteByte(uint8_t data);

/**
 * @brief Sends one byte and receives one byte simultaneously.
 *
 * This is the primary function for SPI communication. It writes the transmit
 * data to the data register, waits for the transfer to complete, and then
 * returns the data that was received in the data register.
 * @param tx_data The 8-bit data byte to send.
 * @return The 8-bit data byte received from the other device.
 */
uint8_t Spi_ReadWriteByte(uint8_t tx_data);

#endif // SPI_DRIVER_H