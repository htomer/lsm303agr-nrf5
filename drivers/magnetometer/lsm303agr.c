/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#include "lsm303agr.h"

#include "nrf_delay.h"
#include "nrfx_twim.h"

#define NRF_LOG_MODULE_NAME LSM303AGR
#define NRF_LOG_LEVEL 3 // LOG_LEVEL
#include "nrfx_log.h"
NRF_LOG_MODULE_REGISTER();

bool lsm303agr_write_buffer(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t len)
{
    nrfx_twim_xfer_desc_t reg_xfer  = NRFX_TWIM_XFER_DESC_TX(addr, &reg, sizeof(reg));
    nrfx_twim_xfer_desc_t data_xfer = NRFX_TWIM_XFER_DESC_TX(addr, buffer, len);

    // Writing register to LSM303AGR using I2C.
    nrfx_err_t status = nrfx_twim_xfer(p_twi_master, &reg_xfer, NRFX_TWIM_FLAG_TX_NO_STOP);
    if (status != NRFX_SUCCESS) {
        NRFX_LOG_WARNING("%s Write transfer failed error: 0x%x.", __func__, status);
        return false;
    }

    // Writing data to LSM303AGR using I2C.
    status = nrfx_twim_xfer(p_twi_master, &data_xfer, 0);
    if (status != NRFX_SUCCESS) {
        NRFX_LOG_WARNING("%s Write transfer failed error: 0x%x.", __func__, status);
        return false;
    }

    NRFX_LOG_INFO("%s Write addr: 0x%x, register: 0x%x.", __func__, addr, reg);
    NRFX_LOG_HEXDUMP_INFO(buffer, len);

    return true;
}

bool lsm303agr_write_register(uint8_t addr, uint8_t reg, uint8_t value) { return lsm303agr_write_buffer(addr, reg, &value, sizeof(value)); }

bool lsm303agr_read_continuous(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t len)
{
    nrfx_twim_xfer_desc_t reg_xfer  = NRFX_TWIM_XFER_DESC_TX(addr, &reg, sizeof(reg));
    nrfx_twim_xfer_desc_t data_xfer = NRFX_TWIM_XFER_DESC_RX(addr, buffer, len);

    // Writing the address to I2C bus.
    nrfx_err_t status = nrfx_twim_xfer(p_twi_master, &reg_xfer, NRFX_TWIM_FLAG_TX_NO_STOP);
    if (status != NRFX_SUCCESS) {
        NRFX_LOG_WARNING("%s Write transfer failed error: 0x%x.", __func__, status);
        return false;
    }

    // Reading register from LSM303AGR using I2C.
    status = nrfx_twim_xfer(p_twi_master, &data_xfer, 0);
    if (status != NRFX_SUCCESS) {
        NRFX_LOG_WARNING("%s Read transfer failed error: 0x%x.", __func__, status);
        return false;
    }

    NRFX_LOG_INFO("%s Read address: 0x%x, register: 0x%x.", __func__, addr, reg);
    NRFX_LOG_HEXDUMP_INFO(buffer, len);

    return true;
}

/**@brief       Read value from register.
 *
 * @param[in]   reg       -   Register to read.
 *
 * @retval  Value from register.
 */
uint8_t lsm303agr_read_register(uint8_t addr, uint8_t reg)
{
    uint8_t value;

    // Read value from register.
    if (!lsm303agr_read_continuous(addr, reg, &value, sizeof(value))) {
        return 0;
    }

    return value;
}