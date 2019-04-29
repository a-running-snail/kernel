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
#ifndef FLASHLIGHT_DTNAME
#define FLASHLIGHT_DTNAME "mediatek,strobe_main"
#endif

/* TODO: define driver name */
#define FLASHLIGHT_NAME "flashlights-eup3611"

/* define registers */
/* TODO: define register */

/* define mutex and work queue */
static DEFINE_MUTEX(eup3611_mutex);
static struct work_struct eup3611_work;

/* define pinctrl */
/* TODO: define pinctrl */
#define FLASHLIGHT_PINCTRL_PIN_FLASH_MODE 0
#define FLASHLIGHT_PINCTRL_PIN_FLASH_ENABLE 1
#define FLASHLIGHT_PINCTRL_PINSTATE_LOW 0
#define FLASHLIGHT_PINCTRL_PINSTATE_HIGH 1
#define FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_HIGH "eup3611_high"
#define FLASHLIGHT_PINCTRL_STATE_SUB_FLASHLIGHT_LOW  "eup3611_low"
/*get GPIO*/
static struct pinctrl *strobectrl = NULL;
static struct pinctrl_state *strobe_flash_en_h = NULL;
static struct pinctrl_state *strobe_flash_en_l = NULL;
static struct pinctrl_state *strobe_flash_mode_h = NULL;
static struct pinctrl_state *strobe_flash_mode_l = NULL;


/* define usage count */
static int use_count;

static int g_flash_duty = -1;


/******************************************************************************
 * Pinctrl configuration
 *****************************************************************************/
static int strobe_main_gpio_init(struct platform_device *pdev)
{
	int ret = 0;
	//fl_pr_debug("strobe_main_gpio_init start !\r\n");
	strobectrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(strobectrl)) {
		fl_pr_debug("Cannot find strobe pinctrl!\r\n");
		ret = PTR_ERR(strobectrl);
	}
	
	strobe_flash_en_l = pinctrl_lookup_state(strobectrl, "en_pin_0");
	if (IS_ERR(strobe_flash_en_l)) {
		ret = PTR_ERR(strobe_flash_en_l);
		fl_pr_debug("pinctrl err, strobe_flash_en 0\r\n");
	}
	strobe_flash_mode_h = pinctrl_lookup_state(strobectrl, "mode_pin_1");
	if (IS_ERR(strobe_flash_mode_h)) {
		ret = PTR_ERR(strobe_flash_mode_h);
		fl_pr_debug("pinctrl err, strobe_flash_mode 1\r\n");
	}
	strobe_flash_mode_l = pinctrl_lookup_state(strobectrl, "mode_pin_0");
	if (IS_ERR(strobe_flash_mode_l)) {
		ret = PTR_ERR(strobe_flash_mode_l);
		fl_pr_debug("pinctrl err, strobe_flash_mode 0\r\n");
	}
	strobe_flash_en_h = pinctrl_lookup_state(strobectrl, "en_pin_1");
	if (IS_ERR(strobe_flash_en_h)) {
		ret = PTR_ERR(strobe_flash_en_h);
		fl_pr_debug("pinctrl err, strobe_flash_en 1\r\n");
	}
	
	fl_pr_debug("strobe_main_gpio_init end !\r\n");
	return ret;
}

static int flashlight_pinctrl_set(int pin, int state)
{
	int ret = 0;

	if (IS_ERR(strobectrl)) {
		//fl_pr_err("pinctrl is not available\n");
		return -1;
	}

	switch (pin) {
	case FLASHLIGHT_PINCTRL_PIN_FLASH_MODE:
		if (state == FLASHLIGHT_PINCTRL_PINSTATE_LOW && !IS_ERR(strobe_flash_mode_l))
			pinctrl_select_state(strobectrl, strobe_flash_mode_l);
		else if (state == FLASHLIGHT_PINCTRL_PINSTATE_HIGH && !IS_ERR(strobe_flash_mode_h))
			pinctrl_select_state(strobectrl, strobe_flash_mode_h);
		else
			fl_pr_err("set err, pin(%d) state(%d)\n", pin, state);
		break;
	case FLASHLIGHT_PINCTRL_PIN_FLASH_ENABLE:
		if (state == FLASHLIGHT_PINCTRL_PINSTATE_LOW && !IS_ERR(strobe_flash_en_l))
			pinctrl_select_state(strobectrl, strobe_flash_en_l);
		else if (state == FLASHLIGHT_PINCTRL_PINSTATE_HIGH && !IS_ERR(strobe_flash_en_h))
			pinctrl_select_state(strobectrl, strobe_flash_en_h);
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
 * eup3611 operations
 *****************************************************************************/
/* flashlight enable function */
static int eup3611_enable(void)
{
	if(g_flash_duty==1||g_flash_duty==0)
	{
		flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_ENABLE,FLASHLIGHT_PINCTRL_PINSTATE_HIGH);
		flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_MODE,FLASHLIGHT_PINCTRL_PINSTATE_LOW);
		fl_pr_debug(" FL_Enable line=%d\n",__LINE__);
	}
	else
	{
		flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_ENABLE,FLASHLIGHT_PINCTRL_PINSTATE_HIGH);
		flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_MODE,FLASHLIGHT_PINCTRL_PINSTATE_HIGH);
		fl_pr_debug(" FL_Enable line=%d\n",__LINE__);
	}
	return 0;
}

/* flashlight disable function */
static int eup3611_disable(void)
{
	flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_ENABLE,FLASHLIGHT_PINCTRL_PINSTATE_LOW);
	flashlight_pinctrl_set(FLASHLIGHT_PINCTRL_PIN_FLASH_MODE,FLASHLIGHT_PINCTRL_PINSTATE_LOW);

	fl_pr_debug(" FL_Disable line=%d\n", __LINE__);
	return 0;
}

static int eup3611_set_level(int level)
{
	g_flash_duty = level;
	return 0;
}

/* flashlight init */
static int eup3611_init(void)
{
	return 0;
}

/* flashlight uninit */
static int eup3611_uninit(void)
{
	eup3611_disable();
	return 0;
}

/******************************************************************************
 * Timer and work queue
 *****************************************************************************/
static struct hrtimer eup3611_timer;
static unsigned int eup3611_timeout_ms;

static void eup3611_work_disable(struct work_struct *data)
{
	fl_pr_debug("work queue callback\n");
	eup3611_disable();
}

static enum hrtimer_restart eup3611_timer_func(struct hrtimer *timer)
{
	schedule_work(&eup3611_work);
	return HRTIMER_NORESTART;
}


/******************************************************************************
 * Flashlight operations
 *****************************************************************************/
static int eup3611_ioctl(unsigned int cmd, unsigned long arg)
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
		eup3611_timeout_ms = fl_arg->arg;
		break;

	case FLASH_IOC_SET_DUTY:
		fl_pr_debug("FLASH_IOC_SET_DUTY(%d): %d\n",
				channel, (int)fl_arg->arg);
		eup3611_set_level(fl_arg->arg);
		break;

	case FLASH_IOC_SET_ONOFF:
		fl_pr_debug("FLASH_IOC_SET_ONOFF(%d): %d\n",
				channel, (int)fl_arg->arg);
		if (fl_arg->arg == 1) {
			if (eup3611_timeout_ms) {
				ktime = ktime_set(eup3611_timeout_ms / 1000,
						(eup3611_timeout_ms % 1000) * 1000000);
				hrtimer_start(&eup3611_timer, ktime, HRTIMER_MODE_REL);
			}
			eup3611_enable();
		} else {
			eup3611_disable();
			hrtimer_cancel(&eup3611_timer);
		}
		break;
	default:
		fl_pr_info("No such command and arg(%d): (%d, %d)\n",
				channel, _IOC_NR(cmd), (int)fl_arg->arg);
		return -ENOTTY;
	}

	return 0;
}

static int eup3611_open(void *pArg)
{
	/* Actual behavior move to set driver function since power saving issue */
	return 0;
}

static int eup3611_release(void *pArg)
{
	/* uninit chip and clear usage count */
	mutex_lock(&eup3611_mutex);
	use_count--;
	if (!use_count)
		eup3611_uninit();
	if (use_count < 0)
		use_count = 0;
	mutex_unlock(&eup3611_mutex);

	fl_pr_debug("Release: %d\n", use_count);

	return 0;
}

static int eup3611_set_driver(void)
{
	/* init chip and set usage count */
	mutex_lock(&eup3611_mutex);
	if (!use_count)
		eup3611_init();
	use_count++;
	mutex_unlock(&eup3611_mutex);

	fl_pr_debug("Set driver: %d\n", use_count);

	return 0;
}

static ssize_t eup3611_strobe_store(struct flashlight_arg arg)
{
	eup3611_set_driver();
	eup3611_set_level(arg.level);
	eup3611_enable();
	msleep(arg.dur);
	eup3611_disable();
	eup3611_release(NULL);

	return 0;
}

static struct flashlight_operations eup3611_ops = {
	eup3611_open,
	eup3611_release,
	eup3611_ioctl,
	eup3611_strobe_store,
	eup3611_set_driver
};


/******************************************************************************
 * Platform device and driver
 *****************************************************************************/
static int eup3611_chip_init(void)
{
	return 0;
}

static int eup3611_probe(struct platform_device *dev)
{
	int err;

	fl_pr_debug("Probe start.\n");

	/* init pinctrl */
	if (strobe_main_gpio_init(dev)) {
		fl_pr_debug("Failed to init pinctrl.\n");
		err = -EFAULT;
		goto err;
	}

	/* init work queue */
	INIT_WORK(&eup3611_work, eup3611_work_disable);

	/* init timer */
	hrtimer_init(&eup3611_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	eup3611_timer.function = eup3611_timer_func;
	eup3611_timeout_ms = 100;

	/* init chip hw */
	eup3611_chip_init();

	/* register flashlight operations */
	if (flashlight_dev_register(FLASHLIGHT_NAME, &eup3611_ops)) {
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

static int eup3611_remove(struct platform_device *dev)
{
	fl_pr_debug("Remove start.\n");

	/* flush work queue */
	flush_work(&eup3611_work);

	/* unregister flashlight operations */
	flashlight_dev_unregister(FLASHLIGHT_NAME);

	fl_pr_debug("Remove done.\n");

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id eup3611_gpio_of_match[] = {
	{.compatible = FLASHLIGHT_DTNAME},
	{},
};
MODULE_DEVICE_TABLE(of, eup3611_gpio_of_match);
#else
static struct platform_device eup3611_gpio_platform_device[] = {
	{
		.name = FLASHLIGHT_NAME,
		.id = 0,
		.dev = {}
	},
	{}
};
MODULE_DEVICE_TABLE(platform, eup3611_gpio_platform_device);
#endif

static struct platform_driver eup3611_platform_driver = {
	.probe = eup3611_probe,
	.remove = eup3611_remove,
	.driver = {
		.name = FLASHLIGHT_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = eup3611_gpio_of_match,
#endif
	},
};

static int __init flashlight_eup3611_init(void)
{
	int ret;

	fl_pr_debug("Init start.\n");

#ifndef CONFIG_OF
	ret = platform_device_register(&eup3611_gpio_platform_device);
	if (ret) {
		fl_pr_err("Failed to register platform device\n");
		return ret;
	}
#endif

	ret = platform_driver_register(&eup3611_platform_driver);
	if (ret) {
		fl_pr_err("Failed to register platform driver\n");
		return ret;
	}

	fl_pr_debug("Init done.\n");

	return 0;
}

static void __exit flashlight_eup3611_exit(void)
{
	fl_pr_debug("Exit start.\n");

	platform_driver_unregister(&eup3611_platform_driver);

	fl_pr_debug("Exit done.\n");
}

module_init(flashlight_eup3611_init);
module_exit(flashlight_eup3611_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simon Wang <Simon-TCH.Wang@mediatek.com>");
MODULE_DESCRIPTION("MTK Flashlight SUB_FLASHLIGHT GPIO Driver");

