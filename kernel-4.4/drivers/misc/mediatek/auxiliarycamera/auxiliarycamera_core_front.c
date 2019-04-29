/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* drivers/hwmon/mt6516/amit/auxiliarycamerafront.c - auxiliarycamerafront ALS/PS driver
 *
 * Author: MingHsien Hsieh <minghsien.hsieh@mediatek.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/wakelock.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#include "auxiliarycamera.h"
#include "kd_camera_hw.h"
#include "kd_camera_typedef.h"
#include "kd_camera_hw.h"

/******************************************************************************
 * configuration
*******************************************************************************/

/*----------------------------------------------------------------------------*/
#define AUXILIARYCAMERAFRONT_DEV_NAME     "auxiliarycamerafront"
/*----------------------------------------------------------------------------*/

/******************************************************************************
 * extern functions
*******************************************************************************/

/*----------------------------------------------------------------------------*/
static struct i2c_client *auxiliarycamerafront_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id auxiliarycamerafront_i2c_id[] = {{AUXILIARYCAMERAFRONT_DEV_NAME,0},{}};
static struct platform_driver auxiliarycamerafront_cam3_driver;
static int auxiliarycamerafront_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int auxiliarycamerafront_i2c_remove(struct i2c_client *client);
static int auxiliarycamerafront_remove(struct platform_device *pdev);
static int auxiliarycamerafront_probe(struct platform_device *pdev);
static DEFINE_SPINLOCK(cam3front_drv_lock);
static struct pinctrl *cam3frontctrl = NULL;
static struct pinctrl_state *cam3front_pnd_h = NULL;
static struct pinctrl_state *cam3front_pnd_l = NULL;
static int cam3front_pnd = -1;
static int Cam3IsPowerOn = 0;
static int Cam3IsPowering = 0;
static int Cam3PreviewOn = 0;
static struct delayed_work thread_work;
volatile int thread_is_working_front = 0;
static void Cam3PowerOn(void);
static void Cam3PowerOff(void);
extern char* auxiliarycamera_getsubname(void);
extern int kstrtoint(const char *s, unsigned int base, int *res);
static SENSOR_BV_STRUCT main_bv[5];
static SENSOR_BV_STRUCT cam3front_bv[3];
static CAM3_PARAMETER_STRUCT parameter;
CAM3_FUNCTION_STRUCT* cam3front_current_ptr = NULL;

CAM3_FUNCTION_STRUCT* cam3front_sensor_list[] = 
{
    &gc0310front_sensor_func,
    NULL,//add imgsensor above this line
};


/*----------------------------------------------------------------------------*/
#ifdef CONFIG_OF
static const struct of_device_id cam3front_i2c_of_match[] = {
	{.compatible = "mediatek,auxiliarycamerafront"},
	{},
};
#endif

static struct i2c_driver cam3front_i2c_driver = {
	.probe      = auxiliarycamerafront_i2c_probe,
	.remove     = auxiliarycamerafront_i2c_remove,
	.id_table   = auxiliarycamerafront_i2c_id,
	.driver = {
		.owner  = THIS_MODULE,
		.name   = AUXILIARYCAMERAFRONT_DEV_NAME,
#ifdef CONFIG_OF
		.of_match_table = cam3front_i2c_of_match,
#endif
	},
};

static ssize_t cam3front_show_brightness(struct device_driver *ddri, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", Cam3PreviewOn ? cam3front_current_ptr->GetBrightness() : 0);
}

static ssize_t cam3front_store_brightness(struct device_driver *ddri, const char *buf, size_t count)
{
    int open;
	if(1 == sscanf(buf, "%d", &open))
	{
        ALCAM_ERR("cam3front_store_brightness open = %d\n", open);
        if (1 == open)
        {
			Cam3IsPowering = 1;
            schedule_delayed_work(&thread_work,0);
        }
        else
        {
			Cam3IsPowering = 0;
            Cam3PowerOff();
        }
	}
    else
    {
        ALCAM_ERR("invalid content: '%s', length = %d\n", buf, count);
    }
	return count;
}

static ssize_t cam3front_show_parameter(struct device_driver *ddri, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%d|%d|%d|%d|%d|%d\n",
         parameter.cumulative_cyles,parameter.diff_percent,parameter.cam3_max_bv,parameter.cam3_min_bv,parameter.main_max_bv,parameter.main_min_bv);
}

static ssize_t cam3front_store_parameter(struct device_driver *ddri, const char *buf, size_t count)
{
	if(sscanf(buf, "%d|%d|%d|%d|%d|%d", &parameter.cumulative_cyles, &parameter.diff_percent, &parameter.cam3_max_bv, &parameter.cam3_min_bv, &parameter.main_max_bv, &parameter.main_min_bv))
	{
        ALCAM_ERR("cumulative_cyles:%d diff_percent:%d cam3_max_bv:%d cam3_min_bv:%d main_max_bv:%d main_min_bv:%d\n",
            parameter.cumulative_cyles,parameter.diff_percent,parameter.cam3_max_bv,parameter.cam3_min_bv,parameter.main_max_bv,parameter.main_min_bv);
	}
    else
    {
        ALCAM_ERR("invalid content: '%s', length = %d\n", buf, count);
    }
	return count;
}

static DRIVER_ATTR(brightness,    0777, cam3front_show_brightness,   cam3front_store_brightness);
static DRIVER_ATTR(parameter,    0777, cam3front_show_parameter,   cam3front_store_parameter);

static struct driver_attribute *auxiliarycamerafront_attr_list[] = {
    &driver_attr_brightness,
    &driver_attr_parameter,        
};

static int auxiliarycamerafront_create_attr(struct device_driver *driver)
{
	int idx, err = 0;
	int num = (int)(sizeof(auxiliarycamerafront_attr_list)/sizeof(auxiliarycamerafront_attr_list[0]));
    ALCAM_FUN();
    
	if (driver == NULL)
	{
		return -EINVAL;
	}

	for(idx = 0; idx < num; idx++)
	{
		if((err = driver_create_file(driver, auxiliarycamerafront_attr_list[idx])))
		{
			ALCAM_ERR("driver_create_file (%s) = %d\n", auxiliarycamerafront_attr_list[idx]->attr.name, err);
			break;
		}
	}
	return err;
}

static int auxiliarycamerafront_delete_attr(struct device_driver *driver)
{
	int idx ,err = 0;
	int num = (int)(sizeof(auxiliarycamerafront_attr_list)/sizeof(auxiliarycamerafront_attr_list[0]));
    ALCAM_FUN();

	if (!driver)
	    return -EINVAL;

	for (idx = 0; idx < num; idx++)
	{
		driver_remove_file(driver, auxiliarycamerafront_attr_list[idx]);
	}

	return err;
}

static void Cam3Matching(void)
{
    static int matched = 0;
    int i=0;
    ALCAM_FUN();
    
    if (matched == 1)
    {
	    ALCAM_ERR("Already matched\n");
        return;
    }
    
	ISP_MCLK1_EN(1);
    for (i=0; cam3front_sensor_list[i] != NULL; i++)
    {
        if (gpio_is_valid(cam3front_pnd)) {
			gpio_request(cam3front_pnd,"cam3front power");
			mdelay(1);
			gpio_direction_output(cam3front_pnd,  cam3front_sensor_list[i]->PowerDownEnable ? 0 : 1);
		} else if (cam3front_pnd_l && cam3front_pnd_h) {
			pinctrl_select_state(cam3frontctrl, cam3front_sensor_list[i]->PowerDownEnable ? cam3front_pnd_l : cam3front_pnd_h);
		}
		
        if (cam3front_sensor_list[i]->CheckIsReady())
        {
            cam3front_current_ptr = cam3front_sensor_list[i];
            ALCAM_ERR("cam3front_current_ptr.sensorname = %s\n",cam3front_current_ptr->SensorName);
            break;
        }
    }
    ISP_MCLK1_EN(0);
    
    if (cam3front_current_ptr != NULL)
    {
        int i=0;
        for (i=0; i<sizeof(cam3front_bv)/sizeof(SENSOR_BV_STRUCT); i++)
        {
            if (strcmp(cam3front_bv[i].sensor_name, cam3front_current_ptr->SensorName) == 0)
            {
                parameter.cam3_max_bv = cam3front_bv[i].max_bv;
                parameter.cam3_min_bv = cam3front_bv[i].min_bv;
                break;
            }
        }
        for (i=0; i<sizeof(main_bv)/sizeof(SENSOR_BV_STRUCT); i++)
        {
            if (strcmp(main_bv[i].sensor_name, auxiliarycamera_getsubname()) == 0)
            {
                parameter.main_max_bv = main_bv[i].max_bv;
                parameter.main_min_bv = main_bv[i].min_bv;
                break;
            }
        }
        ALCAM_ERR("cumulative_cyles:%d diff_percent:%d cam3_max_bv:%d cam3_min_bv:%d main_max_bv:%d main_min_bv:%d\n",
            parameter.cumulative_cyles,parameter.diff_percent,parameter.cam3_max_bv,parameter.cam3_min_bv,parameter.main_max_bv,parameter.main_min_bv);
    }
    matched = 1;
}

static void cam3front_thread_work(struct work_struct *work)
{
    ALCAM_ERR("Cam3PreviewOn = %d, thread_is_working_front = %d\n", Cam3PreviewOn, thread_is_working_front);
    
    if (thread_is_working_front == 1 || Cam3IsPowering == 0) {
		return;
	}
		
    thread_is_working_front = 1;
    Cam3Matching();
    
    if (Cam3IsPowerOn == 0) {
		Cam3PowerOn();
	}
	
	 if ((Cam3PreviewOn == 0) && (cam3front_current_ptr != NULL) && cam3front_current_ptr->CheckIsReady()) {
		 cam3front_current_ptr->StrartPreview();
         Cam3PreviewOn = 1;
	 }
    thread_is_working_front = 0;
    ALCAM_ERR("cam3front_thread_work exit\n");
}

static int auxiliarycamerafront_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int err = 0;
    ALCAM_FUN();

    auxiliarycamerafront_i2c_client = client;

	if ((err = auxiliarycamerafront_create_attr(&(auxiliarycamerafront_cam3_driver.driver))))
	{
		ALCAM_ERR("create attribute err = %d\n", err);
	}
    INIT_DELAYED_WORK(&thread_work, cam3front_thread_work);
	return 0;
}

static int auxiliarycamerafront_i2c_remove(struct i2c_client *client)
{
	int err;
    ALCAM_FUN();
    
	if((err = auxiliarycamerafront_delete_attr(&(auxiliarycamerafront_cam3_driver.driver))))
	{
		ALCAM_ERR("auxiliarycamerafront_delete_attr fail: %d\n", err);
	}
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id auxiliarycamerafront_of_match[] = {
	{.compatible = "mediatek,auxiliarycamerafront"},
	{},
};
#endif

static struct platform_driver auxiliarycamerafront_cam3_driver =
{
    .probe      = auxiliarycamerafront_probe,
    .remove     = auxiliarycamerafront_remove,
    .driver     = {
        .name  = "auxiliarycamerafront",
#ifdef CONFIG_OF
		.of_match_table = auxiliarycamerafront_of_match,
#endif
    }
};

static int  auxiliarycamerafront_remove(struct platform_device *pdev)
{
	ALCAM_FUN();
    Cam3PowerOff();
	i2c_del_driver(&cam3front_i2c_driver);
	auxiliarycamerafront_i2c_client = NULL;
	return 0;
}

int Cam3FrontWriteRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u16 i2cId)
{
	int i4RetValue = 0;
	int retry = 3;

	spin_lock(&cam3front_drv_lock);
	auxiliarycamerafront_i2c_client->addr = (i2cId >> 1);
	auxiliarycamerafront_i2c_client->ext_flag = (auxiliarycamerafront_i2c_client->ext_flag) & (~I2C_DMA_FLAG);
	spin_unlock(&cam3front_drv_lock);

	do {
		i4RetValue = i2c_master_send(auxiliarycamerafront_i2c_client, a_pSendData, a_sizeSendData);
		if (i4RetValue != a_sizeSendData)
        {
			ALCAM_ERR("[CAMERA SENSOR] I2C send failed!!, Addr = 0x%x, Data = 0x%x\n", a_pSendData[0], a_pSendData[1]);
		}
        else
		{
			break;
		}
		udelay(50);
	} while ((retry--) > 0);

	return 0;
}

int Cam3FrontReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u8 *a_pRecvData, u16 a_sizeRecvData, u16 i2cId)
{
	int i4RetValue = 0;

	spin_lock(&cam3front_drv_lock);
	auxiliarycamerafront_i2c_client->addr = (i2cId >> 1);
	auxiliarycamerafront_i2c_client->ext_flag = (auxiliarycamerafront_i2c_client->ext_flag) & (~I2C_DMA_FLAG);
	auxiliarycamerafront_i2c_client->ext_flag = (auxiliarycamerafront_i2c_client->ext_flag) & (~I2C_A_FILTER_MSG);
	spin_unlock(&cam3front_drv_lock);

	i4RetValue = i2c_master_send(auxiliarycamerafront_i2c_client, a_pSendData, a_sizeSendData);
	if (i4RetValue != a_sizeSendData)
    {
		ALCAM_ERR("[CAMERA SENSOR] I2C send failed!!, Addr = 0x%x\n", a_pSendData[0]);
		return -1;
	}

	i4RetValue = i2c_master_recv(auxiliarycamerafront_i2c_client, (char *)a_pRecvData, a_sizeRecvData);
	if (i4RetValue != a_sizeRecvData)
    {
		ALCAM_ERR("[CAMERA SENSOR] I2C read failed!!\n");
		return -1;
	}
	return 0;
}

static SENSOR_BV_STRUCT* auxiliarycamerafront_construct_bv_data(SENSOR_BV_STRUCT* bv, const char* value)
{
    int i = 0, j = 0, k = 0;
    int res;
    char data[32];
    
    for(i=0; i<strlen(value); i++)
    {
        if (value[i] != ',')
        {           
            data[j++] = value[i];
        }
        else
        {
            data[j]='\0';
            j = 0;
            
            if (k == 0)
            {
                strcpy(bv->sensor_name, data);
            }
            else if(k == 1)
            {
                res = kstrtoint(data, 10, &bv->max_bv);
            }
            else
            {
                ALCAM_ERR("error can run this line!!\n");
            }
            k++;
        }
    }
    data[j]='\0';
    res = kstrtoint(data, 10, &bv->min_bv);
    
    return bv;
}

static int auxiliarycamerafront_dts_init(struct platform_device *pdev)
{
    int ret = 0;
	struct device_node *node = NULL;
    ALCAM_FUN();

    if (pdev->dev.of_node != NULL) {
		cam3front_pnd = of_get_named_gpio(pdev->dev.of_node, "cam3front_pnd", 0);
		if (!gpio_is_valid(cam3front_pnd)){
			ALCAM_ERR("%s :get cam3front_pnd gpio err!\n", __func__);
			cam3frontctrl = devm_pinctrl_get(&pdev->dev);
			if (IS_ERR(cam3frontctrl)) {
				ALCAM_ERR("Cannot find cam3frontctrl!");
				ret = PTR_ERR(cam3frontctrl);
			}
			cam3front_pnd_h = pinctrl_lookup_state(cam3frontctrl, "cam3front_pnd1");
			if (IS_ERR(cam3front_pnd_h)) {
				ret = PTR_ERR(cam3front_pnd_h);
				ALCAM_ERR("%s : pinctrl err, cam3front_pnd_h\n", __func__);
			}

			cam3front_pnd_l = pinctrl_lookup_state(cam3frontctrl, "cam3front_pnd0");
			if (IS_ERR(cam3front_pnd_l)) {
				ret = PTR_ERR(cam3front_pnd_l);
				ALCAM_ERR("%s : pinctrl err, cam3front_pnd_l\n", __func__);
			}
		}
	}
    memset((void*)&parameter,0,sizeof(CAM3_PARAMETER_STRUCT));
    memset((void*)&cam3front_bv,0,sizeof(cam3front_bv));
    memset((void*)&main_bv,0,sizeof(main_bv));
    
	node = of_find_compatible_node(NULL, NULL, "mediatek,auxiliarycamerafront_parameter");
	if (node) 
    {
        u32 data[] = {0};
        const char *string_data;
        int i=0;
		ret = of_property_read_u32_array(node , "cumulative_cyles", data, ARRAY_SIZE(data));
    	if (ret == 0)
    	{
    		parameter.cumulative_cyles = data[0];
            ALCAM_ERR("parameter.cumulative_cyles = %d\n", parameter.cumulative_cyles);
    	}
        
		ret = of_property_read_u32_array(node , "diff_percent", data, ARRAY_SIZE(data));
    	if (ret == 0)
    	{
    		parameter.diff_percent = data[0];            
            ALCAM_ERR("parameter.diff_percent = %d\n", parameter.diff_percent);
    	}

        for (i=0; i<sizeof(cam3front_bv)/sizeof(SENSOR_BV_STRUCT); i++)
        {
            char propname[20];
            sprintf(propname,"cam3_%d_bv_value", i+1);
    		ret = of_property_read_string(node , propname, &string_data);
            ALCAM_ERR("%s = %s\n", propname, string_data);
        	if (ret == 0)
        	{
                auxiliarycamerafront_construct_bv_data(&cam3front_bv[i],string_data);
        		ALCAM_ERR("cam3_bv[%d]=%s,%d,%d", i, cam3front_bv[i].sensor_name, cam3front_bv[i].max_bv, cam3front_bv[i].min_bv);
        	}
        }
        
        for (i=0; i<sizeof(main_bv)/sizeof(SENSOR_BV_STRUCT); i++)
        {
            char propname[20];
            sprintf(propname,"main%d_bv_value", i+1);
    		ret = of_property_read_string(node , propname, &string_data);
            ALCAM_ERR("%s = %s\n", propname, string_data);            
        	if (ret == 0)
        	{
                auxiliarycamerafront_construct_bv_data(&main_bv[i],string_data);
        		ALCAM_ERR("main_bv[%d]=%s,%d,%d", i, main_bv[i].sensor_name, main_bv[i].max_bv, main_bv[i].min_bv);
        	}
        }         
	}
	return ret;
}

static void Cam3PowerOn(void)
{
    ALCAM_FUN();
    ISP_MCLK1_EN(1);
    if (cam3front_current_ptr != NULL)
    {
		if (gpio_is_valid(cam3front_pnd)) {
			gpio_request(cam3front_pnd,"cam3front power");
			mdelay(1);
			gpio_direction_output(cam3front_pnd,  cam3front_current_ptr->PowerDownEnable ? 0: 1);
		} else if (cam3front_pnd_l && cam3front_pnd_h) {
			pinctrl_select_state(cam3frontctrl, cam3front_current_ptr->PowerDownEnable ? cam3front_pnd_l : cam3front_pnd_h);
		}
    }
    mdelay(50);
    Cam3IsPowerOn = 1;
}

static void Cam3PowerOff(void)
{
    ALCAM_FUN();
    while (thread_is_working_front)
    {
		ALCAM_ERR("thread_is_working_front wait for finish\n");
		mdelay(3);
	}
	
    if (Cam3IsPowerOn == 0)
    {
	    ALCAM_ERR("Already Power Off\n");
        return ;
    }
    
    Cam3IsPowerOn = 0;
    Cam3PreviewOn = 0;	
    ISP_MCLK1_EN(0);
    if (cam3front_current_ptr != NULL)
    {
		if (gpio_is_valid(cam3front_pnd)) {
			gpio_direction_output(cam3front_pnd,  cam3front_current_ptr->PowerDownEnable ? 1: 0);
			mdelay(1);
			gpio_free(cam3front_pnd);
		}else if (cam3front_pnd_l && cam3front_pnd_h) {
			pinctrl_select_state(cam3frontctrl, cam3front_current_ptr->PowerDownEnable ? cam3front_pnd_h : cam3front_pnd_l);
		}
    }
    mdelay(10);
}

static int auxiliarycamerafront_probe(struct platform_device *pdev)
{
	ALCAM_FUN();

    auxiliarycamerafront_dts_init(pdev);
	if(i2c_add_driver(&cam3front_i2c_driver))
	{
		ALCAM_ERR("add driver error\n");
		return -1;
	}

	return 0;
}

static int __init auxiliarycamerafront_init(void)
{
	ALCAM_FUN();

   	if(platform_driver_register(&auxiliarycamerafront_cam3_driver))
    {
        ALCAM_ERR("failed to register driver");
        return -ENODEV;
    }
	return 0;
}

static void __exit auxiliarycamerafront_exit(void)
{
	ALCAM_FUN();
}

late_initcall(auxiliarycamerafront_init);
module_exit(auxiliarycamerafront_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("Teksun Andy");
MODULE_DESCRIPTION("SensorTek auxiliarycamerafront imgsensor driver");
MODULE_LICENSE("GPL");
