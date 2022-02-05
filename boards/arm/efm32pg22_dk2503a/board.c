/*
 * Copyright (c) 2022 Immanuel Huebner 
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include "board.h"
#include <drivers/gpio.h>
#include <sys/printk.h>


static int efm32pg22_dk2503a_init(const struct device *dev)
{

	ARG_UNUSED(dev);

	return 0;
}

/* needs to be done after GPIO driver init */
SYS_INIT(efm32pg22_dk2503a_init, POST_KERNEL,
	 CONFIG_KERNEL_INIT_PRIORITY_DEVICE);
