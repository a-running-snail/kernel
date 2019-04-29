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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/leds.h>

#include "flashlight.h"
#include "flashlight-dt.h"

/* define device tree */
/* TODO: modify temp device tree name */
#ifndef SUB_FLASHLIGHT_DTNAME
#define SUB_FLASHLIGHT_DTNAME "mediatek,flashlights_subflashlight_gpio"
#endif

/* TODO: define driver name */
#define SUB_FLASHLIGHT_NAME "flashlights-subflashlight-gpio"

/* define registers */
/* TODO: define register */

/* define mutex and work queue */
static DEFINE_MUTEX(subflashlight_mutex);
static struct work_struct subflashlight_work;

/* define pinctrl */
/* TODO: define pinctrl */
#define SUB_FLASHLIGHT_PINCTRL_PIN_SUB_FLASHLIGHT 0
#define SUB_FLASHLIGHT_PINCTRL_PINSTATE_LOW 0
#define SUB_FLASHLIGHT_PINCTRL_PINSTATE_HIGH 1
#define SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_HIGH "subflashlight_high"
#define SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_LOW  "subflashlight_low"
static struct pinctrl *subflashlight_pinctrl;
static struct pinctrl_state *subflashlight_high;
static struct pinctrl_state *subflashlight_low;

/* define usage count */
static int use_count;


/******************************************************************************
 * Pinctrl configuration
 *****************************************************************************/
static int subflashlight_pinctrl_init(struct platform_device *pdev)
{
	int ret = 0;

	/* get pinctrl */
	subflashlight_pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(subflashlight_pinctrl)) {
		fl_pr_err("Failed to get flashlight pinctrl.\n");
		ret = PTR_ERR(subflashlight_pinctrl);
	}

	/* TODO: Flashlight SUB_FLASHLIGHT pin initialization */
	subflashlight_high = pinctrl_lookup_state(subflashlight_pinctrl, SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_HIGH);
	if (IS_ERR(subflashlight_high)) {
		fl_pr_err("Failed to init (%s)\n", SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_HIGH);
		ret = PTR_ERR(subflashlight_high);
	}
	subflashlight_low = pinctrl_lookup_state(subflashlight_pinctrl, SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_LOW);
	if (IS_ERR(subflashlight_low)) {
		fl_pr_err("Failed to init (%s)\n", SUB_FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_LOW);
		ret = PTR_ERR(subflashlight_low);
	}

	return ret;
}

static int subflashlight_pinctrl_set(int pin, int state)
{
	int ret = 0;

	if (IS_ERR(subflashlight_pinctrl)) {
		fl_pr_err("pinctrl is not available\n");
		return -1;
	}

	switch (pin) {
	case SUB_FLASHLIGHT_PINCTRL_PIN_SUB_FLASHLIGHT:
		if (state == SUB_FLASHLIGHT_PINCTRL_PINSTATE_LOW && !IS_ERR(subflashlight_low))
			pinctrl_select_state(subflashlight_pinctrl, subflashlight_low);
		else if (state == SUB_FLASHLIGHT_PINCTRL_PINSTATE_HIGH && !IS_ERR(subflashlight_high))
			pinctrl_select_state(subflashlight_pinctrl, subflashlight_high);
		else
			fl_pr_err("set err, pin(%d) state(%d)\n", pin, state);
		break;
	default:
		fl_pr_err("set err, pin(%d) state(%d)\n", pin, state);
		break;
	}
	fl_pr_debug("pin(%d) state(%d)\n", pin, state);

	return ret;
}


/******************************************************************************
 * subflashlight operations
 *****************************************************************************/
/* flashlight enable function */
static int subflashlight_enable(void)
{
	int pin = SUB_FLASHLIGHT_PINCTRL_PIN_SUB_FLASHLIGHT, state = SUB_FLASHLIGHT_PINCTRL_PINSTATE_HIGH;

	/* TODO: wrap enable function */

	return subflashlight_pinctrl_set(pin, state);
}

/* flashlight disable function */
static int subflashlight_disable(void)
{
	int pin =SUB_FLASHLIGHT_PINCTRL_PIN_SUB_FLASHLIGHT, state = SUB_FLASHLIGHT_PINCTRL_PINSTATE_LOW;

	/* TODO: wrap disable function */

	return subflashlight_pinctrl_set(pin, state);
}

/* set flashlight level */
static int subflashlight_set_level(int level)
{
	/* TODO: wrap set level function */

	return 0;
}

/* flashlight init */
static int subflashlight_init(void)
{
	/* TODO: wrap set level function */

	return 0;
}

/* flashlight uninit */
static int subflashlight_uninit(void)
{
	/* TODO: wrap set level function */

	return 0;
}

/******************************************************************************
 * Timer and work queue
 *****************************************************************************/
static struct hrtimer subflashlight_timer;
static unsigned int subflashlight_timeout_ms;

static void subflashlight_work_disable(struct work_struct *data)
{
	fl_pr_debug("work queue callback\n");
	subflashlight_disable();
}

static enum hrtimer_restart subflashlight_timer_func(struct hrtimer *timer)
{
	schedule_work(&subflashlight_work);
	return HRTIMER_NORESTART;
}


/******************************************************************************
 * Flashlight operations
 *****************************************************************************/
static int subflashlight_ioctl(unsigned int cmd, unsigned long arg)
{
	struct flashlight_dev_arg *fl_arg;
	int channel;
	ktime_t ktime;

	fl_arg = (struct flashlight_dev_arg *)arg;
	channel = fl_arg->channel;

	switch (cmd) {
	case FLASH_IOC_SET_TIME_OUT_TIME_MS:
		fl_pr_debug("FLASH_IOC_SET_TIME_OUT_TIME_MS(%d): %d\n",
				channel, (int)fl_arg->arg);
		subflashlight_timeout_ms = fl_arg->arg;
		break;

	case FLASH_IOC_SET_DUTY:
		fl_pr_debug("FLASH_IOC_SET_DUTY(%d): %d\n",
				channel, (int)fl_arg->arg);
		subflashlight_set_level(fl_arg->arg);
		break;

	case FLASH_IOC_SET_ONOFF:
		fl_pr_debug("FLASH_IOC_SET_ONOFF(%d): %d\n",
				channel, (int)fl_arg->arg);
		if (fl_arg->arg == 1) {
			if (subflashlight_timeout_ms) {
				ktime = ktime_set(subflashlight_timeout_ms / 1000,
						(subflashlight_timeout_ms % 1000) * 1000000);
				hrtimer_start(&subflashlight_timer, ktime, HRTIMER_MODE_REL);
			}
			subflashlight_enable();
		} else {
			subflashlight_disable();
			hrtimer_cancel(&subflashlight_timer);
		}
		break;
	default:
		fl_pr_info("No such command and arg(%d): (%d, %d)\n",
				channel, _IOC_NR(cmd), (int)fl_arg->arg);
		return -ENOTTY;
	}

	return 0;
}

static int subflashlight_open(void *pArg)
{
	/* Actual behavior move to set driver function since power saving issue */
	return 0;
}

static int subflashlight_release(void *pArg)
{
	/* uninit chip and clear usage count */
	mutex_lock(&subflashlight_mutex);
	use_count--;
	if (!use_count)
		subflashlight_uninit();
	if (use_count < 0)
		use_count = 0;
	mutex_unlock(&subflashlight_mutex);

	fl_pr_debug("Release: %d\n", use_count);

	return 0;
}

static int subflashlight_set_driver(void)
{
	/* init chip and set usage count */
	mutex_lock(&subflashlight_mutex);
	if (!use_count)
		subflashlight_init();
	use_count++;
	mutex_unlock(&subflashlight_mutex);

	fl_pr_debug("Set driver: %d\n", use_count);

	return 0;
}

static ssize_t subflashlight_strobe_store(struct flashlight_arg arg)
{
	subflashlight_set_driver();
	subflashlight_set_level(arg.level);
	subflashlight_enable();
	msleep(arg.dur);
	subflashlight_disable();
	subflashlight_release(NULL);

	return 0;
}

static struct flashlight_operations subflashlight_ops = {
	subflashlight_open,
	subflashlight_release,
	subflashlight_ioctl,
	subflashlight_strobe_store,
	subflashlight_set_driver
};


/******************************************************************************
 * Platform device and driver
 *****************************************************************************/
static int subflashlight_chip_init(void)
{
	/* NOTE: Chip initialication move to "set driver" operation for power saving issue.
	 * subflashlight_init();
	 */

	return 0;
}

static int subflashlight_probe(struct platform_device *dev)
{
	int err;

	fl_pr_debug("Probe start.\n");

	/* init pinctrl */
	if (subflashlight_pinctrl_init(dev)) {
		fl_pr_debug("Failed to init pinctrl.\n");
		err = -EFAULT;
		goto err;
	}

	/* init work queue */
	INIT_WORK(&subflashlight_work, subflashlight_work_disable);

	/* init timer */
	hrtimer_init(&subflashlight_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	subflashlight_timer.function = subflashlight_timer_func;
	subflashlight_timeout_ms = 100;

	/* init chip hw */
	subflashlight_chip_init();

	/* register flashlight operations */
	if (flashlight_dev_register(SUB_FLASHLIGHT_NAME, &subflashlight_ops)) {
		err = -EFAULT;
		goto err;
	}

	/* clear usage count */
	use_count = 0;

	fl_pr_debug("Probe done.\n");

	return 0;
err:
	return err;
}

static int subflashlight_remove(struct platform_device *dev)
{
	fl_pr_debug("Remove start.\n");

	/* flush work queue */
	flush_work(&subflashlight_work);

	/* unregister flashlight operations */
	flashlight_dev_unregister(SUB_FLASHLIGHT_NAME);

	fl_pr_debug("Remove done.\n");

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id subflashlight_gpio_of_match[] = {
	{.compatible = SUB_FLASHLIGHT_DTNAME},
	{},
};
MODULE_DEVICE_TABLE(of, subflashlight_gpio_of_match);
#else
static struct platform_device subflashlight_gpio_platform_device[] = {
	{
		.name = SUB_FLASHLIGHT_NAME,
		.id = 0,
		.dev = {}
	},
	{}
};
MODULE_DEVICE_TABLE(platform, subflashlight_gpio_platform_device);
#endif

static struct platform_driver subflashlight_platform_driver = {
	.probe = subflashlight_probe,
	.remove = subflashlight_remove,
	.driver = {
		.name = SUB_FLASHLIGHT_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = subflashlight_gpio_of_match,
#endif
	},
};

static int __init flashlight_subflashlight_init(void)
{
	int ret;

	fl_pr_debug("Init start.\n");

#ifndef CONFIG_OF
	ret = platform_device_register(&subflashlight_gpio_platform_device);
	if (ret) {
		fl_pr_err("Failed to register platform device\n");
		return ret;
	}
#endif

	ret = platform_driver_register(&subflashlight_platform_driver);
	if (ret) {
		fl_pr_err("Failed to register platform driver\n");
		return ret;
	}

	fl_pr_debug("Init done.\n");

	return 0;
}

static void __exit flashlight_subflashlight_exit(void)
{
	fl_pr_debug("Exit start.\n");

	platform_driver_unregister(&subflashlight_platform_driver);

	fl_pr_debug("Exit done.\n");
}

module_init(flashlight_subflashlight_init);
module_exit(flashlight_subflashlight_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simon Wang <Simon-TCH.Wang@mediatek.com>");
MODULE_DESCRIPTION("MTK Flashlight SUB_FLASHLIGHT GPIO Driver");

