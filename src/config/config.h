/**
 * Copyright (c) 2024, Tomer Hanochi.
 *
 * All rights reserved.
 */

#pragma once

#include "boards.h"

/* TWIM Configuration */
#define TWIM_INST 0    //!< TWIM interface used as a master
#define TWIM_SCL_PIN 3 //!< Master SCL pin.
#define TWIM_SDA_PIN 4 //!< Master SDA pin.
#define MAG_INT_PIN 5  //!< Magnetometer interrupt pin.