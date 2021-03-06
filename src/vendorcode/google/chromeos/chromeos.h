/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 The ChromiumOS Authors.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CHROMEOS_H__
#define __CHROMEOS_H__

#include <stddef.h>
#include <stdint.h>
#include <bootmode.h>
#include <device/device.h>
#include <rules.h>
#include <vboot/misc.h>
#include <vboot/vboot_common.h>

void save_chromeos_gpios(void);

#if CONFIG_CHROMEOS
/* functions implemented in watchdog.c */
void mark_watchdog_tombstone(void);
void reboot_from_watchdog(void);
#else
static inline void mark_watchdog_tombstone(void) { return; }
static inline void reboot_from_watchdog(void) { return; }
#endif /* CONFIG_CHROMEOS */

struct romstage_handoff;

#include "gnvs.h"
struct device;

#if CONFIG_CHROMEOS_RAMOOPS
void chromeos_ram_oops_init(chromeos_acpi_t *chromeos);
#if CONFIG_CHROMEOS_RAMOOPS_DYNAMIC
static inline void chromeos_reserve_ram_oops(struct device *dev, int idx) {}
#else /* CONFIG_CHROMEOS_RAMOOPS_DYNAMIC */
void chromeos_reserve_ram_oops(struct device *dev, int idx);
#endif /* CONFIG_CHROMEOS_RAMOOPS_DYNAMIC */
#else  /* !CONFIG_CHROMEOS_RAMOOPS */
static inline void chromeos_ram_oops_init(chromeos_acpi_t *chromeos) {}
static inline void chromeos_reserve_ram_oops(struct device *dev, int idx) {}
#endif /* CONFIG_CHROMEOS_RAMOOPS */

void cbmem_add_vpd_calibration_data(void);

/*
 * Create the OIPG package containing the Chrome OS gpios described by
 * the chromeos_gpio array.
 */
struct cros_gpio;
void chromeos_acpi_gpio_generate(const struct cros_gpio *gpios, size_t num);

/*
 * Common helper function and delcarations for mainboards to use to generate
 * ACPI-specific Chrome OS needs.
 */
void mainboard_chromeos_acpi_generate(void);
#if IS_ENABLED(CONFIG_CHROMEOS)
void chromeos_dsdt_generator(struct device *dev);
#else
#define chromeos_dsdt_generator DEVICE_NOOP
#endif

enum {
	CROS_GPIO_REC = 1, /* Recovery */
	CROS_GPIO_DEV = 2, /* Developer */
	CROS_GPIO_WP = 3, /* Write Protect */

	CROS_GPIO_ACTIVE_LOW = 0,
	CROS_GPIO_ACTIVE_HIGH = 1,

	CROS_GPIO_VIRTUAL = -1,
};

struct cros_gpio {
	int type;
	int polarity;
	int gpio_num;
	const char *device;
};

#define CROS_GPIO_INITIALIZER(typ, pol, num, dev) \
	{				\
		.type = (typ),		\
		.polarity = (pol),	\
		.gpio_num = (num),	\
		.device = (dev),	\
	}

#define CROS_GPIO_REC_INITIALIZER(pol, num, dev) \
	CROS_GPIO_INITIALIZER(CROS_GPIO_REC, pol, num, dev)

#define CROS_GPIO_REC_AL(num, dev) \
	CROS_GPIO_REC_INITIALIZER(CROS_GPIO_ACTIVE_LOW, num, dev)

#define CROS_GPIO_REC_AH(num, dev) \
	CROS_GPIO_REC_INITIALIZER(CROS_GPIO_ACTIVE_HIGH, num, dev)

#define CROS_GPIO_DEV_INITIALIZER(pol, num, dev) \
	CROS_GPIO_INITIALIZER(CROS_GPIO_DEV, pol, num, dev)

#define CROS_GPIO_DEV_AL(num, dev) \
	CROS_GPIO_DEV_INITIALIZER(CROS_GPIO_ACTIVE_LOW, num, dev)

#define CROS_GPIO_DEV_AH(num, dev) \
	CROS_GPIO_DEV_INITIALIZER(CROS_GPIO_ACTIVE_HIGH, num, dev)

#define CROS_GPIO_WP_INITIALIZER(pol, num, dev) \
	CROS_GPIO_INITIALIZER(CROS_GPIO_WP, pol, num, dev)

#define CROS_GPIO_WP_AL(num, dev) \
	CROS_GPIO_WP_INITIALIZER(CROS_GPIO_ACTIVE_LOW, num, dev)

#define CROS_GPIO_WP_AH(num, dev) \
	CROS_GPIO_WP_INITIALIZER(CROS_GPIO_ACTIVE_HIGH, num, dev)

#endif /* __CHROMEOS_H__ */
