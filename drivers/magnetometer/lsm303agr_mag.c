/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#include "lsm303agr_mag.h"

#include "app_util.h"
#include "lsm303agr.h"
#include "nrf_delay.h"
#include "nrf_log_ctrl.h"

#define NRF_LOG_MODULE_NAME LSM303AGR_MAG
#define NRF_LOG_LEVEL 3 // LOG_LEVEL
#include "nrfx_log.h"
NRF_LOG_MODULE_REGISTER();

#define I2C_ADDR LSM303AGR_I2C_ADD_MG // Magnetometer I2C address.
#define BOOT_TIME 5

bool lsm303agr_mag_init(void)
{
    // Wait for device boot time.
    nrf_delay_ms(BOOT_TIME);

    // Read who am i register (in order to check device).
    uint8_t id = lsm303agr_mag_get_device_id();
    if (id != LSM303AGR_ID_MG) {
        NRFX_LOG_WARNING("%s LSM303AGR is not found expceted id: %x, received id: %d.", __func__, LSM303AGR_ID_MG, id);

        // LSM303AGR is not found.
        return false;
    }

    return true;
}

uint8_t lsm303agr_mag_get_device_id(void) { return lsm303agr_read_register(I2C_ADDR, LSM303AGR_WHO_AM_I_M); }

bool lsm303agr_mag_set_soft_reset(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte     = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.SOFT_RST = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

lsm303agr_enable_t lsm303agr_mag_get_soft_reset(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.SOFT_RST;
}

bool lsm303agr_mag_set_reboot(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte   = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.REBOOT = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

lsm303agr_enable_t lsm303agr_mag_get_reboot(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.REBOOT;
}

bool lsm303agr_mag_set_comp_temp(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte         = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.COMP_TEMP_EN = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

lsm303agr_enable_t lsm303agr_mag_get_comp_temp(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.COMP_TEMP_EN;
}

bool lsm303agr_mag_set_low_power(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.LP   = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

uint8_t lsm303agr_mag_get_low_power(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.LP;
}

bool lsm303agr_mag_set_md(lsm303agr_md_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.MD   = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

lsm303agr_md_t lsm303agr_mag_get_md(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.MD;
}

bool lsm303agr_mag_set_odr(lsm303agr_odr_t value)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);
    config.ODR  = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M, config.byte);
}

lsm303agr_odr_t lsm303ag_mag_get_odr(void)
{
    lsm303agr_config_reg_a_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_A_M);

    return config.ODR;
}

bool lsm303agr_mag_set_drdy_on_pin(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_c_t config;

    config.byte    = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M);
    config.INT_MAG = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M, config.byte);
}

lsm303agr_enable_t lsm303agr_mag_get_drdy_on_pin(void)
{
    lsm303agr_config_reg_c_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M);

    return config.INT_MAG;
}

bool lsm303agr_mag_set_int_on_pin(lsm303agr_enable_t value)
{
    lsm303agr_config_reg_c_t config;

    config.byte        = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M);
    config.INT_MAG_PIN = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M, config.byte);
}

lsm303agr_enable_t lsm303agr_mag_get_int_on_pin(void)
{
    lsm303agr_config_reg_c_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_C_M);

    return config.INT_MAG_PIN;
}

bool lsm303agr_mag_set_set_rst_mode(lsm303agr_set_rst_t value)
{
    lsm303agr_config_reg_b_t config;

    config.byte    = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_B_M);
    config.set_rst = value;

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_CFG_REG_B_M, config.byte);
}

lsm303agr_set_rst_t lsm303agr_mag_get_set_rst_mode(void)
{
    lsm303agr_config_reg_b_t config;

    config.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_CFG_REG_B_M);

    return config.set_rst;
}

bool lsm303agr_mag_set_int_ctrl(lsm303agr_int_cntl_t value)
{

    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_INT_CTRL_REG_M, value.byte);
}

lsm303agr_int_cntl_t lsm303agr_mag_get_int_ctrl(void)
{
    lsm303agr_int_cntl_t ret;

    ret.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_INT_CTRL_REG_M);

    return ret;
}

bool lsm303agr_mag_set_int_source(lsm303agr_int_source_t value)
{
    return lsm303agr_write_register(I2C_ADDR, LSM303AGR_INT_SOURCE_REG_M, value.byte);
}

lsm303agr_int_source_t lsm303agr_mag_get_int_source(void)
{
    lsm303agr_int_source_t ret;

    ret.byte = lsm303agr_read_register(I2C_ADDR, LSM303AGR_INT_SOURCE_REG_M);

    return ret;
}

bool lsm303agr_mag_set_int_threshold(int16_t val)
{
    uint8_t buff[2];

    buff[0] = (val >> 0) && 0xFF;
    buff[1] = (val >> 8) && 0xFF;

    // Write threshold registers.
    return lsm303agr_write_buffer(I2C_ADDR, LSM303AGR_INT_THS_L_REG_M, buff, sizeof(buff));
}

int16_t lsm303agr_mag_get_int_threshold(void)
{
    uint8_t buff[2] = {0};

    // Read ths registers.
    lsm303agr_read_continuous(I2C_ADDR, LSM303AGR_INT_THS_L_REG_M, buff, sizeof(buff));

    return (int16_t)((buff[1] << 8) + buff[0]);
}