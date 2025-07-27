# MCAL Driver Simulation: SPI Peripheral

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Layer](https://img.shields.io/badge/Layer-MCAL-red.svg)
![Version](https://img.shields.io/badge/Version-1.0-brightgreen.svg)

## Project Goal

This project demonstrates the ability to write a low-level hardware driver for a microcontroller peripheral from scratch. The goal was to simulate the development process for a driver within the **MCAL (Microcontroller Abstraction Layer)** of the AUTOSAR architecture.

The project involves:
1.  Reading a "datasheet" to understand hardware register functionality.
2.  Writing clean, portable C code to configure and control these registers using bitwise operations.
3.  Creating a clear and reusable API (`Spi_Init`, `Spi_ReadWriteByte`).
4.  Building a test application to validate the driver's functionality in a simulated environment.

## The Simulated Hardware

To create a realistic development environment without physical hardware, the project simulates a simple SPI (Serial Peripheral Interface) peripheral with three 8-bit memory-mapped registers.

#### `SPCR` - SPI Control Register
* **Purpose:** Used to enable the SPI module and configure its operational mode (e.g., Master/Slave).
* **Key Bits:** `SPE` (SPI Enable), `MSTR` (Master Select).

#### `SPSR` - SPI Status Register
* **Purpose:** A read-only register used to check the status of the peripheral.
* **Key Bits:** `SPIF` (SPI Transfer Complete Flag), which is set by hardware when a data transfer is finished.

#### `SPDR` - SPI Data Register
* **Purpose:** The main I/O register. Writing a byte to `SPDR` initiates a transfer. After a transfer, it holds the received byte.

## Core SPI Concepts Explained

To understand the driver's logic, it's helpful to know a few key concepts about how SPI communication works.

#### Full-Duplex Communication
SPI is a **full-duplex** protocol. This means that for every single clock cycle, data is being sent from the master to the slave device AND from the slave to the master device **at the same time**. There are two dedicated data lines for this:
* **MOSI:** Master-Out-Slave-In. The line the master uses to send data *to* the slave.
* **MISO:** Master-In-Slave-Out. The line the slave uses to send data *to* the master.

#### The `Spi_ReadWriteByte` Logic
Our `Spi_ReadWriteByte` function simulates this perfectly. When it's called:
1. The `tx_data` (`0x80`) is written to `SPDR_REG`. This simulates the **master** placing its data on the **MOSI** line.
2. The function then waits for the transfer to complete.
3. Once complete, it reads from `SPDR_REG`. In our simulation, we manually copy the `MISO_LINE_DATA` (`0xC5`) into `SPDR_REG` to simulate the **master** latching the data that the **slave** placed on the **MISO** line during the transfer.

This is why the function both sends and receives a value in a single call, accurately modeling the behavior of real SPI hardware.

## Driver API

The driver exposes a simple and clean API for the application layer to use.

* **`void Spi_Init(void);`**
    * Initializes the SPI peripheral by configuring the `SPCR` register, enabling the module, and setting it as the master device.

* **`uint8_t Spi_ReadWriteByte(uint8_t tx_data);`**
    * Performs a full-duplex 8-bit data transfer.
    * It writes `tx_data` to the data register, polls the `SPIF` flag until the transfer is complete, and then returns the byte received from the slave device.

## How to Build and Run

#### Dependencies
* A C compiler, such as **GCC**.

#### Build & Execution Steps
1.  Clone the repository.
2.  Open a terminal in the project's root directory.
3.  Compile the driver and the test application together:
    ```bash
    gcc main.c spi_driver.c -o spi_test.exe
    ```
4.  Run the final test executable:
    ```bash
    ./spi_test.exe
    ```

#### Final Test Output
The test application simulates sending a command (`0x80`) to a sensor and receiving a response (`0xC5`). The output confirms the driver works correctly.
```
APP: Starting Full SPI Read/Write Test. DRIVER: Spi_Init() called. DRIVER: SPCR register configured to: 0x50
APP: Sending command 0x80 to sensor...
APP: Simulating sensor response of 0xC5 on MISO line.
DRIVER: Spi_ReadWriteByte() called with tx_data: 0x80
DRIVER: Hardware busy, starting transfer...
DRIVER: Transfer complete!
DRIVER: Returning received rx_data: 0xC5
APP: Received data from sensor: 0xC5
APP: SUCCESS! Correct data received from sensor.
```