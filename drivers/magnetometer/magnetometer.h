/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#pragma once

#include "lsm303agr_types.h"
#include "nrfx_gpiote.h"

typedef enum {
    MAGNETOMETER_EVENT_NOTHING = 0,

    MAGNETOMETER_EVENT_START,
    MAGNETOMETER_EVENT_STOP,
    MAGNETOMETER_EVENT_MAGNET_DETECTED,
    MAGNETOMETER_EVENT_MAGNET_NOT_DETECTED,
} magnetometer_event_t;

// type of the event handler for the magnetometer events
typedef void (*magnetometer_handler_t)(magnetometer_event_t event);

/** Function for initializing LSM303AGR magnetometer device. */
bool magnetometer_init(magnetometer_handler_t handler);

/** Function for reset the magnetometer configuration. */
void magnetometer_reset(void);

/** Function for start the magnetometer measurement cycle. */
void magnetometer_start(void);

/** Function for stop the magnetometer for minimal power consumption. */
void magnetometer_stop(void);

/** GPIOTE Pin event handler */
void magnetometer_gpiote_event_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);