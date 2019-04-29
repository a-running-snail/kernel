/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "flashlight.h"

const struct flashlight_device_id flashlight_id[] = {
	/* {TYPE, CT, PART, "NAME", CHANNEL, DECOUPLE} */
#if defined(CONFIG_MTK_FLASHLIGHT_EUP3611)
	{0, 0, 0, "flashlights-eup3611", 0, 0},
#else
	{0, 0, 0, "flashlights-none", 0, 0},
#endif
	{0, 1, 0, "flashlights-none", -1, 0},
#if  defined(CONFIG_MTK_FLASHLIGHT_SUB_GPIO)
	{1, 0, 0, "flashlights-subflashlight-gpio", -1, 0},
#else
	{1, 0, 0, "flashlights-none", -1, 0},
#endif
	{1, 1, 0, "flashlights-none", -1, 0},
	{0, 0, 1, "flashlights-none", -1, 0},
	{0, 1, 1, "flashlights-none", -1, 0},
	{1, 0, 1, "flashlights-none", -1, 0},
	{1, 1, 1, "flashlights-none", -1, 0},
};

