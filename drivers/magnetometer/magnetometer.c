/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#include "magnetometer.h"

#include "app_timer.h"
#include "app_util.h"
#include "config.h"
#include "lsm303agr_mag.h"
#include "nrf_delay.h"

#define NRF_LOG_MODULE_NAME MAGNETOMETER
#define NRF_LOG_LEVEL 3 // LOG_LEVEL
#include "nrfx_log.h"
NRF_LOG_MODULE_REGISTER();

APP_TIMER_DEF(m_magnetometer_timer);

#define MAGNETOMETER_DEBOUNCE_MS APP_TIMER_TICKS(200)
#define MAGNETOMETER_THRESHOLD_VALUE 0x300 // ~ 1.5*768 = 1152 [mgauss]
#define MAGNETOMETER_INT_IEA LSM303AGR_INT_HIGH

static magnetometer_handler_t m_event_handler = NULL; // Callback function to notify on events.
static magnetometer_event_t   m_last_event    = MAGNETOMETER_EVENT_NOTHING;

static bool m_lsm303_flag = false; // Flag to indicate lsm303 sensor is detected.

static void magnetometer_timer_handler(void *p_context);

static magnetometer_event_t get_current_event(uint32_t pin)
{
    bool state = (nrf_gpio_pin_read(pin) == MAGNETOMETER_INT_IEA);

    // Default: 0 If IEA = 0, then INT = 0 signals an interrupt. If IEA = 1, then INT = 1 signals an interrupt.
    return state ? MAGNETOMETER_EVENT_MAGNET_DETECTED : MAGNETOMETER_EVENT_MAGNET_NOT_DETECTED;
}

bool magnetometer_init(magnetometer_handler_t handler)
{
    nrfx_err_t err_code;

    ASSERT(handler);
    m_event_handler = handler;

    // Initialize lsm303.
    m_lsm303_flag = lsm303agr_mag_init();
    if (!m_lsm303_flag) {
        return false;
    }

    err_code = app_timer_create(&m_magnetometer_timer, APP_TIMER_MODE_SINGLE_SHOT, magnetometer_timer_handler);
    APP_ERROR_CHECK(err_code);

    // Restart LSM303.
    magnetometer_reset();

    return true;
}

void magnetometer_reset(void)
{
    // Restore default configuration for magnetometer.
    lsm303agr_enable_t rst = LSM303AGR_ENABLE;
    lsm303agr_mag_set_soft_reset(rst);

    while (rst == LSM303AGR_ENABLE) {
        // Delay
        nrf_delay_us(100);

        rst = lsm303agr_mag_get_soft_reset();
    }

    // Set device to idle mode.
    lsm303agr_mag_set_md(LSM303AGR_MODE_IDLE);

    // Set data rate.
    lsm303agr_mag_set_odr(LSM303AGR_ODR_10);

    // For proper operation, this bit must be set to 1.
    lsm303agr_mag_set_comp_temp(LSM303AGR_ENABLE);
}

void magnetometer_start(void)
{
    lsm303agr_int_cntl_t int_ctrl = {.IEN  = LSM303AGR_ENABLE,    // Enables interrupt.
                                     .ZIEN = LSM303AGR_ENABLE,    // Enables interrupt for z-axis.
                                     .YIEN = LSM303AGR_ENABLE,    // Enables interrupt for y-axis.
                                     .XIEN = LSM303AGR_ENABLE,    // Enables interrupt for x-axis.
                                     .IEL  = LSM303AGR_INT_PULSE, // Pulsed interrupt.
                                     .IEA  = LSM303AGR_INT_HIGH};

    APP_ERROR_CHECK_BOOL(m_lsm303_flag);

    // Set interrupt threshold.
    lsm303agr_mag_set_int_threshold(MAGNETOMETER_THRESHOLD_VALUE);

    // Enable interrupt on MAG_PIN.
    lsm303agr_mag_set_int_on_pin(LSM303AGR_ENABLE);

    // Set MAG_PIN as an output.
    lsm303agr_mag_set_drdy_on_pin(LSM303AGR_ENABLE);

    // Enable interrupt recognition for the Z-axis.
    lsm303agr_mag_set_int_ctrl(int_ctrl);

    // Set device to continuous mode.
    lsm303agr_mag_set_md(LSM303AGR_MODE_CONTINUOUS);

    // Set intial state for the magnet.
    app_timer_start(m_magnetometer_timer, MAGNETOMETER_DEBOUNCE_MS, (uint32_t *)MAG_INT_PIN);
}

void magnetometer_stop(void)
{
    // Disable interrupt.
    nrfx_gpiote_in_event_disable(MAG_INT_PIN);

    // Restart magnetometer.
    // magnetometer_reset();

    // Set device to idle mode.
    lsm303agr_mag_set_md(LSM303AGR_MODE_IDLE);
}

void magnetometer_gpiote_event_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    // Disable sensing event from MAG_INT_PIN input pint.
    nrfx_gpiote_in_event_disable(pin);

    // Start debounce timer.
    app_timer_start(m_magnetometer_timer, MAGNETOMETER_DEBOUNCE_MS, (uint32_t *)pin);
}

static void magnetometer_timer_handler(void *p_context)
{
    uint32_t pin = (uint32_t)p_context;

    // Enable sensing event from MAG_INT_PIN input pin.
    nrfx_gpiote_in_event_enable(pin, true);

    // Get magnet current state.
    magnetometer_event_t event = get_current_event(pin);
    if (m_last_event == event) {
        return;
    }

    // Set last event to current event.
    m_last_event = event;

    // Report new magnet state.
    m_event_handler(event);
}