/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#pragma once

#include "nrfx_twim.h"

#include <stdbool.h>
#include <stdint.h>

extern const nrfx_twim_t *p_hal_twim;

/** I2C Device Address 7 bit format**/
#define LSM303AGR_I2C_ADD_XL 0x19
#define LSM303AGR_I2C_ADD_MG 0x1E

/** Device Identification (Who am I) **/
#define LSM303AGR_ID_XL 0x33U
#define LSM303AGR_ID_MG 0x40U

typedef enum {
    // Device identification registers.
    LSM303AGR_WHO_AM_I_A = 0x0F,
    LSM303AGR_WHO_AM_I_M = 0x4F,

    // Magnetometer hard-iron registers
    LSM303AGR_OFFSET_X_REG_L_M = 0x45,
    LSM303AGR_OFFSET_X_REG_H_M = 0x46,
    LSM303AGR_OFFSET_Y_REG_L_M = 0x47,
    LSM303AGR_OFFSET_Y_REG_H_M = 0x48,
    LSM303AGR_OFFSET_Z_REG_L_M = 0x49,
    LSM303AGR_OFFSET_Z_REG_H_M = 0x4A,

    // Magnetometer configuration registers
    LSM303AGR_CFG_REG_A_M = 0x60,
    LSM303AGR_CFG_REG_B_M = 0x61,
    LSM303AGR_CFG_REG_C_M = 0x62,

    // Magnetometer interrupt configuration registers
    LSM303AGR_INT_CTRL_REG_M   = 0x63,
    LSM303AGR_INT_SOURCE_REG_M = 0x64,
    LSM303AGR_INT_THS_L_REG_M  = 0x65,
    LSM303AGR_INT_THS_H_REG_M  = 0x66,

    // Magnetometer Status registers
    LSM303AGR_STATUS_REG_M = 0x67,

    // Magnetometer output registers
    LSM303AGR_OUTX_L_REG_M = 0x68,
    LSM303AGR_OUTX_H_REG_M = 0x69,
    LSM303AGR_OUTY_L_REG_M = 0x6A,
    LSM303AGR_OUTY_H_REG_M = 0x6B,
    LSM303AGR_OUTZ_L_REG_M = 0x6C,
    LSM303AGR_OUTZ_H_REG_M = 0x6D,
} lsm303agr_registers_t;

#pragma pack(1)

// Low power mode
typedef enum {
    LSM303AGR_LP_HIGH = 0b0, // high-resolution mode
    LSM303AGR_LP_LOW  = 0b1, // low-power mode enabled
} lsm303agr_lp_t;

// System mode
typedef enum {
    LSM303AGR_MODE_CONTINUOUS = 0b00, // Continuous mode.
    LSM303AGR_MODE_SINGLE     = 0b01, // Single mode.
    LSM303AGR_MODE_IDLE       = 0b10, // Idle mode.
} lsm303agr_md_t;

// Output data rate configuration
typedef enum {
    LSM303AGR_ODR_10  = 0b00, // 10 Hz
    LSM303AGR_ODR_20  = 0b01, // 20 Hz
    LSM303AGR_ODR_50  = 0b10, // 50 Hz
    LSM303AGR_ODR_100 = 0b11, // 100 Hz
} lsm303agr_odr_t;

// Propertry enable constants.
typedef enum {
    LSM303AGR_DISABLE = 0b0, // 0: property disabled.
    LSM303AGR_ENABLE  = 0b1, // 1: property enabled.
} lsm303agr_enable_t;

// Controls the polarity of the INT bit when an interrupt occurs
typedef enum {
    LSM303AGR_INT_LOW  = 0b0, // If IEA = 0, then INT = 0 signals an interrupt. Default.
    LSM303AGR_INT_HIGH = 0b1, // If IEA = 1, then INT = 1 signals an interrupt.
} lsm303agr_int_polarity_t;

// Controls whether the INT bit is latched or pulsed.
typedef enum {
    LSM303AGR_INT_PULSE = 0b0, // If IEL = 0, then INT is pulsed.
    LSM303AGR_INT_LATCH =
        0b1, // If IEL = 1, then INT is latched. Once latched, INT remains in the same state until INT_SOURCE_REG_M(64h) is read.
} lsm303agr_int_signal_t;

typedef enum {
    LSM303AGR_SET_SENS_ODR_DIV_63       = 0,
    LSM303AGR_SENS_OFF_CANC_EVERY_ODR   = 1,
    LSM303AGR_SET_SENS_ONLY_AT_POWER_ON = 2,
} lsm303agr_set_rst_t;

typedef union {
    uint8_t byte;

    struct {
        lsm303agr_md_t     MD : 2;           // Mode select bits.
        lsm303agr_odr_t    ODR : 2;          // Output data rate configuration
        lsm303agr_lp_t     LP : 1;           // Enables low-power mode.
        lsm303agr_enable_t SOFT_RST : 1;     // configuration registers and user registers reset.
        lsm303agr_enable_t REBOOT : 1;       // Reboots magnetometer memory content.
        lsm303agr_enable_t COMP_TEMP_EN : 1; // Enables the magnetometer temperature compensation.
    };
} lsm303agr_config_reg_a_t;

typedef union {
    uint8_t byte;

    struct {
        lsm303agr_enable_t  lpf : 1;     // Enables low-pass filter.
        lsm303agr_set_rst_t set_rst : 2; // OFF_CANC + Set_FREQ
        uint8_t             int_on_dataoff : 1;
        lsm303agr_enable_t  off_canc_one_shot : 1; // Enables offset cancellation in single measurement mode.
        uint8_t : 3;
    };
} lsm303agr_config_reg_b_t;

typedef union {
    uint8_t byte;

    struct {
        lsm303agr_enable_t INT_MAG_PIN : 1; // If 1, the INTERRUPT signal (INT bit inside INT_SOURCE_REG_M (64h)) is driven on INT_MAG_PIN
        lsm303agr_enable_t I2C_DIS : 1;     // If 1, the I2C interface is inhibited. Only the SPI interface can be used.
        lsm303agr_enable_t BDU : 1;         // If enabled, reading of incorrect data is avoided when the user reads asynchronously.
        lsm303agr_enable_t BLE : 1;         // If 1, an inversion of the low and high parts of the data occurs.
        uint8_t : 1;                        // This bit must be set to 0 for the correct operation of the device.
        lsm303agr_enable_t Self_test : 1;   // If 1, the self-test is enabled.
        lsm303agr_enable_t INT_MAG : 1;     // If 1, the DRDY pin is configured as a digital output.
    };
} lsm303agr_config_reg_c_t;

typedef union {
    uint8_t byte;

    struct {
        lsm303agr_enable_t     IEN : 1; // Enables the interrupt. When set, enables the interrupt generation.
        lsm303agr_int_signal_t IEL : 1; // Default: 0, If IEL = 0, then INT is pulsed. If IEL = 1, then INT is latched.
        lsm303agr_int_polarity_t
            IEA : 1; //  Default: 0 If IEA = 0, then INT = 0 signals an interrupt. If IEA = 1, then INT = 1 signals an interrupt.
        uint8_t : 2;
        lsm303agr_enable_t ZIEN : 1; // Enables the interrupt recognition for the Z-axis.
        lsm303agr_enable_t YIEN : 1; // Enables the interrupt recognition for the Y-axis.
        lsm303agr_enable_t XIEN : 1; // Enables the interrupt recognition for the X-axis.
    };
} lsm303agr_int_cntl_t;

typedef union {
    uint8_t byte;

    struct {
        uint8_t INT : 1;      // This bit signals when the interrupt event occurs.
        uint8_t MROI : 1;     // MROI flag generation is alway enabled.This flag is reset by reading INT_SOURCE_REG_M (64h).
        uint8_t N_TH_S_Z : 1; // Z-axis value exceeds the threshold negative side
        uint8_t N_TH_S_Y : 1; // Y-axis value exceeds the threshold negative side
        uint8_t N_TH_S_X : 1; // X-axis value exceeds the threshold negative side
        uint8_t P_TH_S_Z : 1; // Z-axis value exceeds the threshold positive side
        uint8_t P_TH_S_Y : 1; // Y-axis value exceeds the threshold positive side
        uint8_t P_TH_S_X : 1; // X-axis value exceeds the threshold positive side
    };
} lsm303agr_int_source_t;

#pragma pack()