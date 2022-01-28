/*
 * Copyright (c)  2022 Immanuel Huebner
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 * @brief Silabs EFM32PG22 MCU pin definitions.
 *
 * This file contains pin configuration data required by different MCU
 * modules to correctly configure GPIO controller.
 */

#ifndef _SILABS_EFM32PG22_SOC_PINMAP_H_
#define _SILABS_EFM32PG22_SOC_PINMAP_H_

#include <devicetree.h>
#include <soc.h>
#include <em_gpio.h>

#define GPIO_NODE DT_INST(0, silabs_gecko_gpio)
#if DT_NODE_HAS_PROP(GPIO_NODE, location_swo)
#define SWO_LOCATION DT_PROP(GPIO_NODE, location_swo)
#endif

/* Serial Wire Output (SWO) */
#if (SWO_LOCATION == 0)
#define PIN_SWO {gpioPortA, 3, gpioModePushPull, 1}
#elif (SWO_LOCATION >= 1)
#error ("Invalid SWO pin location")
#endif

#endif /* _SILABS_EFM32PG22_SOC_PINMAP_H_ */
