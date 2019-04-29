/*
 *
 * FocalTech TouchScreen driver.
 * 
 * Copyright (c) 2010-2015, Focaltech Ltd. All rights reserved.
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

 /*******************************************************************************
*
* File Name: Focaltech_Gestrue.c
*
* Author: Xu YongFeng
*
* Created: 2015-01-29
*   
* Modify by mshl on 2015-03-20
*
* Abstract:
*
* Reference:
*
*******************************************************************************/

/*******************************************************************************
* 1.Included header files
*******************************************************************************/
#include "focaltech_core.h"
#if FTS_GESTRUE
#include "ft_gesture_lib.h"
#include "teksun_gesture.h"
/*******************************************************************************
* Private constant and macro definitions using #define
*******************************************************************************/
#define GESTURE_LEFT								0x20
#define GESTURE_RIGHT								0x21
#define GESTURE_UP		    							0x22
#define GESTURE_DOWN								0x23
#define GESTURE_DOUBLECLICK						0x24
#define GESTURE_O		    							0x30
#define GESTURE_W		    							0x31
#define GESTURE_M		   	 						0x32
#define GESTURE_E		    							0x33
#define GESTURE_C		    0x34
#define GESTURE_S		    							0x46
#define GESTURE_V		    							0x54
#define GESTURE_Z		    							0x65
#define GESTURE_2		    0xfb
#define FTS_GESTRUE_POINTS 						255
#define FTS_GESTRUE_POINTS_ONETIME  				62
#define FTS_GESTRUE_POINTS_HEADER 				8
#define FTS_GESTURE_OUTPUT_ADRESS 				0xD3
#define FTS_GESTURE_OUTPUT_UNIT_LENGTH 			4

short pointnum 					= 0;
unsigned short coordinate_x[150] 	= {0};
unsigned short coordinate_y[150] 	= {0};

//static int gsl_lcd_flag = 0;




//#define KEY_HOMEPAGE  172
//#define KEY_BACK  158
//#define KEY_MENU  139




/*******************************************************************************
* Private enumerations, structures and unions using typedef
*******************************************************************************/


/*******************************************************************************
* Static variables
*******************************************************************************/


/*******************************************************************************
* Global variable or extern global variabls/functions
*******************************************************************************/

/*******************************************************************************
* Static function prototypes
*******************************************************************************/


/*******************************************************************************
*   Name: fts_Gesture_init
*  Brief:
*  Input:
* Output: None
* Return: None
*******************************************************************************/
int fts_Gesture_init(struct input_dev *input_dev)
{
#if 1	
    init_para(480,854,60,0,0);
#else   
	input_set_capability(input_dev, EV_KEY, KEY_POWER);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_U); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_UP); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_DOWN);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_LEFT); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_RIGHT); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_O);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_E); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_M); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_L);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_W);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_S); 
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_V);
	input_set_capability(input_dev, EV_KEY, KEY_GESTURE_Z);
		
	__set_bit(KEY_GESTURE_RIGHT, input_dev->keybit);
	__set_bit(KEY_GESTURE_LEFT, input_dev->keybit);
	__set_bit(KEY_GESTURE_UP, input_dev->keybit);
	__set_bit(KEY_GESTURE_DOWN, input_dev->keybit);
	__set_bit(KEY_GESTURE_U, input_dev->keybit);
	__set_bit(KEY_GESTURE_O, input_dev->keybit);
	__set_bit(KEY_GESTURE_E, input_dev->keybit);
	__set_bit(KEY_GESTURE_M, input_dev->keybit);
	__set_bit(KEY_GESTURE_W, input_dev->keybit);
	__set_bit(KEY_GESTURE_L, input_dev->keybit);
	__set_bit(KEY_GESTURE_S, input_dev->keybit);
	__set_bit(KEY_GESTURE_V, input_dev->keybit);
	__set_bit(KEY_GESTURE_Z, input_dev->keybit);
#endif
	return 0;
}


//    .owner = THIS_MODULE,

/*******************************************************************************
*   Name: fts_check_gesture
*  Brief:
*  Input:
* Output: None
* Return: None
*******************************************************************************/
static void fts_check_gesture(struct input_dev *input_dev,int gesture_id)
{
	unsigned int report_key = 0;
	
    printk("check_gesture ======================gesture_id==0x%x   ================\n ",gesture_id);
	switch(gesture_id)
	{

		case GESTURE_O:
			report_key = KEY_XUNHU_O;
			break;
		case GESTURE_W:
			report_key = KEY_XUNHU_W;    
			break;
		case GESTURE_M:
			report_key = KEY_XUNHU_M;
			break;
		case GESTURE_E:
			report_key = KEY_XUNHU_E;
			break;
		case GESTURE_C:
			report_key = KEY_XUNHU_C;
			break;
		case GESTURE_S:
			report_key = KEY_XUNHU_S;
			break ;
		case GESTURE_V:
			report_key = KEY_XUNHU_V;
			break;
		case GESTURE_Z:
			report_key = KEY_XUNHU_Z;
			break;
        case GESTURE_RIGHT:
            report_key = KEY_XUNHU_RIGHT;
            break ;
        case GESTURE_DOWN:
             report_key = KEY_XUNHU_DOWN;
            break ;
        case GESTURE_UP: 
             report_key = KEY_XUNHU_UP;
             break ;
        case GESTURE_LEFT:   
            report_key = KEY_XUNHU_LEFT;
            break ;
        case GESTURE_DOUBLECLICK:   
            report_key = KEY_XUNHU_DOUBLE;
            break ;
		case GESTURE_2:
			report_key = KEY_XUNHU_Z;
			break;	
		default:
			break ;
	}

   // gsl_gesture_c = (char)gesture_id;
   //input_report_key(tpd->dev, KEY_M, 1);
	teksun_gesture_report_key(report_key);
    //printk("lj xunhu tp report_key %d down\n", report_key);
}

 /************************************************************************
*   Name: fts_read_Gestruedata
* Brief: read data from TP register
* Input: no
* Output: no
* Return: fail <0
***********************************************************************/
int fts_read_Gestruedata(void)
{
    unsigned char buf[FTS_GESTRUE_POINTS * 4] = { 0 };
    int ret = -1;
    int i = 0;
    int gestrue_id = 0;
    short pointnum = 0;
    //u8 state;   
    buf[0] = 0xd3;

    //focaltech_get_upgrade_array();
    
    ret = fts_i2c_read(fts_i2c_client, buf, 1, buf, FTS_GESTRUE_POINTS_HEADER);
    if (ret < 0)
    {
        printk( "%s read touchdata failed.\n", __func__);
        return ret;
    }
	printk("qiao_fts_read_Gestruedata--------------------fts_updateinfo_curr.CHIP_ID=0x%x\n",fts_updateinfo_curr.CHIP_ID);
    /* FW */
     if (fts_updateinfo_curr.CHIP_ID==0x54 || fts_updateinfo_curr.CHIP_ID==0x58 || fts_updateinfo_curr.CHIP_ID==0x86 || fts_updateinfo_curr.CHIP_ID==0x87  || fts_updateinfo_curr.CHIP_ID == 0x64)
     {
		  printk( "%s read lishilin in ft6336GU \n", __func__);
		 gestrue_id = buf[0];
		 pointnum = (short)(buf[1]) & 0xff;
		 buf[0] = 0xd3;

		 if((pointnum * 4 + 2)<255)
		 {
				 ret = fts_i2c_read(fts_i2c_client, buf, 1, buf, (pointnum * 4 + 2));
		 }
		 else
		 {
				ret = fts_i2c_read(fts_i2c_client, buf, 1, buf, 255);
				ret = fts_i2c_read(fts_i2c_client, buf, 0, buf+255, (pointnum * 4 + 2) -255);
		 }
		 if (ret < 0)
		 {
			   printk( "%s read touchdata failed.\n", __func__);
			   return ret;
		 }

		 fts_check_gesture(fts_input_dev,gestrue_id);
		 for(i = 0;i < pointnum;i++)
		 {
				coordinate_x[i] =  (((s16) buf[0 + (4 * i+2)]) & 0x0F) <<
					8 | (((s16) buf[1 + (4 * i+2)])& 0xFF);
				coordinate_y[i] = (((s16) buf[2 + (4 * i+2)]) & 0x0F) <<
					8 | (((s16) buf[3 + (4 * i+2)]) & 0xFF);
		 }
		 return -1;
   }
	// other IC's gestrue in driver
   printk("fts_read_Gestruedata----qiao--------buf[0]=0x%x\n",buf[0]); 
    
    if (0x24 == buf[0])
    {
        gestrue_id = 0x24;

		 fts_check_gesture(fts_input_dev,gestrue_id);
		printk( "qiao----------check_gesture gestrue_id=0x%x\n", gestrue_id);
        return -1;
    }

    pointnum = (short)(buf[1]) & 0xff;
    buf[0] = 0xd3;
	if((pointnum * 4 + 8)<128)
	{
		ret = fts_i2c_read(fts_i2c_client, buf, 1, buf, (pointnum * 4 + 8));
	}
	else
	{
	     ret = fts_i2c_read(fts_i2c_client, buf, 1, buf, 128);
	     ret = fts_i2c_read(fts_i2c_client, buf, 0, buf+128, (pointnum * 4 + 8) -128);
	}
    if (ret < 0)
    {
        printk( "%s read touchdata failed.\n", __func__);
        return ret;
    }
		printk( "qiao----------pointnum=0x%x\n", pointnum);
	gestrue_id = fetch_object_sample(buf, pointnum);//need gesture lib.a
	//gestrue_id = 0x24;
	fts_check_gesture(fts_input_dev,gestrue_id);
	printk( "qiao------------------read gestrue_id.====0x%x\n", gestrue_id);

	for(i = 0;i < pointnum;i++)
	{
	    coordinate_x[i] =  (((s16) buf[0 + (4 * i+8)]) & 0x0F) <<
	        8 | (((s16) buf[1 + (4 * i+8)])& 0xFF);
	    coordinate_y[i] = (((s16) buf[2 + (4 * i+8)]) & 0x0F) <<
	        8 | (((s16) buf[3 + (4 * i+8)]) & 0xFF);
	}
	return -1;
}
//***************************************************************************************************

//wuhao start
	//char tmp_buf[16];
/*	
	if(copy_from_user(tmp_buf, buf, (count>16?16:count))){
		return -1;
	}
	*/
		//printk("[GSL_GESTURE] gsl_sysfs_tpgesturet_store off.\n");
		//printk("[GSL_GESTURE] gsl_sysfs_tpgesturet_store on.\n");


    

    
//*************************************************************************
	//int i = 0;
	//int in_use;
	//int is_counting;

	


	
	
//wuhao start


#endif
