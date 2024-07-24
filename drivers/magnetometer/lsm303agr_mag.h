/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#pragma once

#include "lsm303agr_types.h"

/** Initializing LSM303AGR magnetometer device.*/
bool lsm303agr_mag_init(void);

/** Read device who am i register.*/
uint8_t lsm303agr_mag_get_device_id(void);

/** Configuration registers and user registers reset. */
bool               lsm303agr_mag_set_soft_reset(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_soft_reset(void);

/** Reboots magnetometer memory content. */
bool               lsm303agr_mag_set_reboot(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_reboot(void);

/** Magnetometer temperature compensation. */
bool               lsm303agr_mag_set_comp_temp(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_comp_temp(void);

/** Enables low-power mode */
bool               lsm303agr_mag_set_low_power(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_low_power(void);

/** Mode select bits */
bool           lsm303agr_mag_set_md(lsm303agr_md_t value);
lsm303agr_md_t lsm303agr_mag_get_md(void);

/** Output data rate configuration */
bool            lsm303agr_mag_set_odr(lsm303agr_odr_t value);
lsm303agr_odr_t lsm303agr_mag_get_odr(void);

/** DRDY pin digital output configuration. */
bool               lsm303agr_mag_set_drdy_on_pin(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_drdy_on_pin(void);

/** Interrupt signal driven on INT_MAG_PIN configuration. */
bool               lsm303agr_mag_set_int_on_pin(lsm303agr_enable_t value);
lsm303agr_enable_t lsm303agr_mag_get_int_on_pin(void);

/** Interrupt signal driven on INT_MAG_PIN configuration. */
bool                 lsm303agr_mag_set_int_ctrl(lsm303agr_int_cntl_t value);
lsm303agr_int_cntl_t lsm303agr_mag_get_int_ctrl(void);

/** Interrupt source register */
bool                   lsm303agr_mag_set_int_source(lsm303agr_int_source_t value);
lsm303agr_int_source_t lsm303agr_mag_get_int_source(void);

/** Interrupt threshold registers */
bool    lsm303agr_mag_set_int_threshold(int16_t val);
int16_t lsm303agr_mag_get_int_threshold(void);