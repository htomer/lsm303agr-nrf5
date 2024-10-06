# Proximity Sensor


## Description 

This project is a driver for interfacing LSM303AGR magnetic sensor with NRF52 microcontroller. The driver is designed to detect the proximity of a magnet to the sensor and trigger an event on state change. When a magnet is detected, the system receives an event and turns on LED 2. If no magnet is detected, the system turns on LED 0.


## Getting Started

1. Install Nordic required tools and IDE [nrf_tools](https://infocenter.nordicsemi.com/topic/ug_gsg_ses/UG/gsg/nrf_tools.html).
1. Install nordic nrf5 sdk [install_nrf5_sdk](https://infocenter.nordicsemi.com/topic/ug_gsg_ses/UG/gsg/install_nrf5_sdk.html).
1. Add the path of your nRF5 SDK to the '''NRF5_SDK''' environment variable.


## Requirements

1. Nordic nRF52840DK or similar.
1. LSM303AGR acceleration and magnetic sensor.


## Wiring

Connect LSM303AGR to the nRF52840DK as follows:
* LSM303AGR `SCL` with nRF52 GPIO `P0.3`.
* LSM303AGR `SDA` with nRF52 GPIO `P0.4`.
* LSM303AGR `INT_MAG` with nRF52 GPIO `P0.5`.
* LSM303AGR `Vin` with nRF52 `VDD`.
* LSM303AGR `GND` with nRF52 `GND`.


## Reference

1. [LSM303AGR datasheet](https://www.st.com/resource/en/datasheet/lsm303agr.pdf)
1. [LSM303 Pinout](https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/pinouts)

## License

[MIT](https://github.com/htomer/lsm303agr-nrf5/blob/main/LICENSE)


The example project files are licensed under [Nordic License](https://github.com/htomer/lsm303agr-nrf5/blob/main/LICENSE-Nordic)