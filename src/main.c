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
#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrfx_twim.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const nrfx_twim_t m_twi_master = NRFX_TWIM_INSTANCE(TWIM_INST);

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

/**
 * @brief Initialize common modules and services.
 *
 */
void peripherals_init(void)
{
    ret_code_t err_code;

    /* Initializing TWI master interface. */
    err_code = twi_master_init();
    APP_ERROR_CHECK(err_code);

    // TODO.
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

    /* Main loop */
    while (1) {
        UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
    }
}