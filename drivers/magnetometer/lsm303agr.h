/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#pragma once

#include "lsm303agr_types.h"

extern const nrfx_twim_t *p_twi_master;

/**@brief       Function for initializing LSM303AGR device.
 *
 */
// bool lsm303agr_init(void);

/**@brief       Write buffer to LSM303AGR register.
 *
 * @param[in]   addr      -   TWI slave address.
 * @param[in]   reg       -   Register to write to.
 * @param[in]   buffer    -   Pointer to transferred data.
 * @param[in]   len       -   Length of buffer in bytes.
 *
 * @retval  True if write completed successfully.
 */
bool lsm303agr_write_buffer(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t len);

/**@brief       Write value to LSM303AGR register.
 *
 * @param[in]   addr      -   TWI slave address.
 * @param[in]   reg       -   Register to write to.
 * @param[in]   value     -   Value to write.
 *
 * @retval  True if write completed successfully.
 */
bool lsm303agr_write_register(uint8_t addr, uint8_t reg, uint8_t value);

/**@brief       Read value from LSM303AGR register.
 *
 * @param[in]   addr      -   TWI slave address.
 * @param[in]   reg       -   Register to read from.
 * @param[in]   buffer    -   Pointer to transferred data.
 * @param[in]   len       -   Length of buffer in bytes.
 *
 * @retval  True if read completed successfully.
 */
bool lsm303agr_read_continuous(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t len);

/**@brief       Read value from LSM303AGR register.
 *
 * @param[in]   addr      -   TWI slave address.
 * @param[in]   reg       -   Register to read from.
 *
 * @retval  Value from register.
 */
uint8_t lsm303agr_read_register(uint8_t addr, uint8_t reg);