/*****************************************************************************
 *
 * Filename:
 * ---------
 *   gc0310_yuv_Sensor.h
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Image sensor driver declare and macro define in the header file.
 *
 * Author:
 * -------
 *   Mormo
 *
 *=============================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 * 2011/10/25 Firsty Released By Mormo;
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *=============================================================
 ******************************************************************************/
 
#ifndef __GC0310_SENSOR_H
#define __GC0310_SENSOR_H
#include "kd_camera_typedef.h"

#define VGA_PERIOD_PIXEL_NUMS						694
#define VGA_PERIOD_LINE_NUMS						488

#define IMAGE_SENSOR_VGA_GRAB_PIXELS			0
#define IMAGE_SENSOR_VGA_GRAB_LINES			0

#define IMAGE_SENSOR_VGA_WIDTH					(640)
#define IMAGE_SENSOR_VGA_HEIGHT					(480)

#define IMAGE_SENSOR_PV_WIDTH					(IMAGE_SENSOR_VGA_WIDTH)
#define IMAGE_SENSOR_PV_HEIGHT					(IMAGE_SENSOR_VGA_HEIGHT)

#define IMAGE_SENSOR_FULL_WIDTH					(IMAGE_SENSOR_VGA_WIDTH)
#define IMAGE_SENSOR_FULL_HEIGHT					(IMAGE_SENSOR_VGA_HEIGHT)
#define GC0310_SENSOR_ID                        0xa310

#define GC0310_WRITE_ID							        0x42
#define GC0310_READ_ID								0x43
#endif /* __SENSOR_H */

