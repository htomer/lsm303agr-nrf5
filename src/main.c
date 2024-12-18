/**
 * Copyright (c) 2015 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "app_error.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "config.h"
#include "magnetometer/magnetometer.h"
#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrfx_gpiote.h"
#include "nrfx_twim.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const nrfx_twim_t m_twi_master = NRFX_TWIM_INSTANCE(TWIM_INST);

const nrfx_twim_t *p_twi_master = &m_twi_master;

static void magnetometer_evt_handler(const magnetometer_event_t event);

/**
 * @brief Initialize common modules and services.
 *
 */
static void modules_init(void)
{
    ret_code_t err_code;

    // Initialize log.
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Initialize clock.
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);

    // Initialize app timer.
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Initialize bsp.
    bsp_board_init(BSP_INIT_LEDS);
}

/**
 * @brief Initialize the master TWI.
 *
 * Function used to initialize the master TWI interface that would communicate with simulated EEPROM.
 *
 * @return NRF_SUCCESS or the reason of failure.
 */
static ret_code_t twi_master_init(void)
{
    ret_code_t               ret;
    const nrfx_twim_config_t config = {.scl                = TWIM_SCL_PIN,
                                       .sda                = TWIM_SDA_PIN,
                                       .frequency          = NRF_TWIM_FREQ_400K,
                                       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
                                       .hold_bus_uninit    = false};

    ret = nrfx_twim_init(&m_twi_master, &config, NULL, NULL);
    APP_ERROR_CHECK(ret);

    nrfx_twim_enable(&m_twi_master);

    return ret;
}

void dummy_event_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {}

static void gpio_init(void)
{
    nrfx_err_t              err_code;
    nrfx_gpiote_in_config_t in_config;

    err_code = nrfx_gpiote_init();
    APP_ERROR_CHECK(err_code);

    in_config      = (nrfx_gpiote_in_config_t)NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    err_code       = nrfx_gpiote_in_init(TWIM_SCL_PIN, &in_config, dummy_event_handler);
    // nrfx_gpiote_in_event_enable(TWIM_SCL_PIN, false);
    APP_ERROR_CHECK(err_code);

    in_config      = (nrfx_gpiote_in_config_t)NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    err_code       = nrfx_gpiote_in_init(TWIM_SDA_PIN, &in_config, dummy_event_handler);
    // nrfx_gpiote_in_event_enable(TWIM_SDA_PIN, false);
    APP_ERROR_CHECK(err_code);

    in_config      = (nrfx_gpiote_in_config_t)NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_NOPULL;
    err_code       = nrfx_gpiote_in_init(MAG_INT_PIN, &in_config, magnetometer_gpiote_event_handler);
    // nrfx_gpiote_in_event_enable(MAG_INT_PIN, true);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Initialize common modules and services.
 *
 */
void peripherals_init(void)
{
    ret_code_t err_code;

    /* Initializing TWI master interface. */
    twi_master_init();

    /* Initializing GPIOs. */
    gpio_init();

    // Initialize lsm303agr.
    magnetometer_init(magnetometer_evt_handler);
}

/**
 *  The beginning of the journey
 */
int main(void)
{
    // Initialize modules.
    modules_init();

    // Initialize peripherals.
    peripherals_init();

    NRF_LOG_INFO("Starting..");

    // Start magnetometer measurements.
    magnetometer_start();

    /* Main loop */
    while (1) {
        UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
    }
}

static void magnetometer_evt_handler(const magnetometer_event_t event)
{
    switch (event) {
    case MAGNETOMETER_EVENT_MAGNET_DETECTED:
        NRF_LOG_INFO("%s MAGNETOMETER_EVENT_MAGNET_DETECTED", __func__);

        // Magnet detected, turn the LED on.
        bsp_board_led_on(0);
        bsp_board_led_off(2);
        break;
    case MAGNETOMETER_EVENT_MAGNET_NOT_DETECTED:
        NRF_LOG_INFO("%s MAGNETOMETER_EVENT_MAGNET_NOT_DETECTED", __func__);

        // Magnet not detected, turn the LED off.
        bsp_board_led_off(0);
        bsp_board_led_on(2);
        break;
    default:
        NRF_LOG_WARNING("%s Unknown magnetometer event %d", __func__, event);
    }
}