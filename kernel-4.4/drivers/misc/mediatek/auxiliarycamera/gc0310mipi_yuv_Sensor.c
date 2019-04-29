/*****************************************************************************
 *
 * Filename:
 * ---------
 *   gc0310yuv_Sensor.c
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Image sensor driver function
 *   V1.2.3
 *
 * Author:
 * -------
 *   Leo
 *
 *=============================================================
 *             HISTORY
 * Below this line, this part is controlled by GCoreinc. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 * 2012.02.29  kill bugs
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by GCoreinc. DO NOT MODIFY!!
 *=============================================================
 ******************************************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "gc0310mipi_yuv_Sensor.h"
#include "auxiliarycamera.h"

kal_uint16 Cam3_GC0310_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
    char puSendCmd[2] = {(char)(addr & 0xFF) , (char)(para & 0xFF)};
    Cam3WriteRegI2C(puSendCmd , 2, GC0310_WRITE_ID);
    return 0;
}

static kal_uint16 Cam3_GC0310_read_cmos_sensor(kal_uint32 addr)
{
    kal_uint16 get_byte=0;
    char pu_send_cmd[1] = {(char)(addr & 0xFF) };
    Cam3ReadRegI2C(pu_send_cmd, 1, (u8*)&get_byte, 1, GC0310_READ_ID);
    return get_byte;
}

int Cam3_GC0310CheckIsReady(void)
{
    int  retry = 1000;
    UINT32 sensorID;
    
    do {
        sensorID=((Cam3_GC0310_read_cmos_sensor(0xf0)<< 8)|Cam3_GC0310_read_cmos_sensor(0xf1));
        ALCAM_ERR("sensorID = 0x%04x\n", sensorID);        
        if (sensorID == GC0310_SENSOR_ID)
            return 1;
        retry--;
    } while (retry > 0);

    return 0;
}

/*************************************************************************
* FUNCTION
*   GC0310_Sensor_Init
*
* DESCRIPTION
*   This function apply all of the initial setting to sensor.
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
*************************************************************************/
void Cam3_GC0310_Sensor_preview(void)
{
    Cam3_GC0310_write_cmos_sensor(0xfe,0xf0);
    Cam3_GC0310_write_cmos_sensor(0xfe,0xf0);
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);
    Cam3_GC0310_write_cmos_sensor(0xfc,0x0e);
    Cam3_GC0310_write_cmos_sensor(0xfc,0x0e);
    Cam3_GC0310_write_cmos_sensor(0xf2,0x80);
    Cam3_GC0310_write_cmos_sensor(0xf3,0x00);
    Cam3_GC0310_write_cmos_sensor(0xf7,0x1b);
    Cam3_GC0310_write_cmos_sensor(0xf8,0x04);  // from 03 to 04
    Cam3_GC0310_write_cmos_sensor(0xf9,0x8e);
    Cam3_GC0310_write_cmos_sensor(0xfa,0x11);
    /////////////////////////////////////////////////      
    ///////////////////   MIPI   ////////////////////      
    /////////////////////////////////////////////////      
    Cam3_GC0310_write_cmos_sensor(0xfe,0x03);
    Cam3_GC0310_write_cmos_sensor(0x40,0x08);
    Cam3_GC0310_write_cmos_sensor(0x42,0x00);
    Cam3_GC0310_write_cmos_sensor(0x43,0x00);
    Cam3_GC0310_write_cmos_sensor(0x01,0x03);
    Cam3_GC0310_write_cmos_sensor(0x10,0x84);
                            
    Cam3_GC0310_write_cmos_sensor(0x01,0x03);             
    Cam3_GC0310_write_cmos_sensor(0x02,0x11); //0x00            
    Cam3_GC0310_write_cmos_sensor(0x03,0x94);             
    Cam3_GC0310_write_cmos_sensor(0x04,0x01);            
    Cam3_GC0310_write_cmos_sensor(0x05,0x40);  // 40      20     
    Cam3_GC0310_write_cmos_sensor(0x06,0x80);             
    Cam3_GC0310_write_cmos_sensor(0x11,0x1e);             
    Cam3_GC0310_write_cmos_sensor(0x12,0x00);      
    Cam3_GC0310_write_cmos_sensor(0x13,0x05);             
    Cam3_GC0310_write_cmos_sensor(0x15,0x10);                                                                    
    Cam3_GC0310_write_cmos_sensor(0x21,0x10);             
    Cam3_GC0310_write_cmos_sensor(0x22,0x01);             
    Cam3_GC0310_write_cmos_sensor(0x23,0x10);                                             
    Cam3_GC0310_write_cmos_sensor(0x24,0x02);                                             
    Cam3_GC0310_write_cmos_sensor(0x25,0x10);                                             
    Cam3_GC0310_write_cmos_sensor(0x26,0x03);                                             
    Cam3_GC0310_write_cmos_sensor(0x29,0x02); //02                                            
    Cam3_GC0310_write_cmos_sensor(0x2a,0x0a);   //0a                                          
    Cam3_GC0310_write_cmos_sensor(0x2b,0x04);                                             
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);
    /////////////////////////////////////////////////
    /////////////////   CISCTL reg  /////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x00,0x2f);
    Cam3_GC0310_write_cmos_sensor(0x01,0x0f);
    Cam3_GC0310_write_cmos_sensor(0x02,0x04);
    Cam3_GC0310_write_cmos_sensor(0x03,0x04);
    Cam3_GC0310_write_cmos_sensor(0x04,0xd0);
    Cam3_GC0310_write_cmos_sensor(0x09,0x00);
    Cam3_GC0310_write_cmos_sensor(0x0a,0x00);
    Cam3_GC0310_write_cmos_sensor(0x0b,0x00);
    Cam3_GC0310_write_cmos_sensor(0x0c,0x06);
    Cam3_GC0310_write_cmos_sensor(0x0d,0x01);
    Cam3_GC0310_write_cmos_sensor(0x0e,0xe8);
    Cam3_GC0310_write_cmos_sensor(0x0f,0x02);
    Cam3_GC0310_write_cmos_sensor(0x10,0x88);
    Cam3_GC0310_write_cmos_sensor(0x16,0x00);
    Cam3_GC0310_write_cmos_sensor(0x17,0x14);
    Cam3_GC0310_write_cmos_sensor(0x18,0x1a);
    Cam3_GC0310_write_cmos_sensor(0x19,0x14);
    Cam3_GC0310_write_cmos_sensor(0x1b,0x48);
    Cam3_GC0310_write_cmos_sensor(0x1e,0x6b);
    Cam3_GC0310_write_cmos_sensor(0x1f,0x28);
    Cam3_GC0310_write_cmos_sensor(0x20,0x8b);  // from 89 to 8b
    Cam3_GC0310_write_cmos_sensor(0x21,0x49);
    Cam3_GC0310_write_cmos_sensor(0x22,0xb0);
    Cam3_GC0310_write_cmos_sensor(0x23,0x04);
    Cam3_GC0310_write_cmos_sensor(0x24,0x16);
    Cam3_GC0310_write_cmos_sensor(0x34,0x20);

    /////////////////////////////////////////////////
    ////////////////////   BLK   ////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x26,0x23); 
    Cam3_GC0310_write_cmos_sensor(0x28,0xff); 
    Cam3_GC0310_write_cmos_sensor(0x29,0x00); 
    Cam3_GC0310_write_cmos_sensor(0x33,0x10); 
    Cam3_GC0310_write_cmos_sensor(0x37,0x20); 
    Cam3_GC0310_write_cmos_sensor(0x38,0x10); 
    Cam3_GC0310_write_cmos_sensor(0x47,0x80); 
    Cam3_GC0310_write_cmos_sensor(0x4e,0x66); 
    Cam3_GC0310_write_cmos_sensor(0xa8,0x02); 
    Cam3_GC0310_write_cmos_sensor(0xa9,0x80);

    /////////////////////////////////////////////////
    //////////////////   ISP reg  ///////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x40,0xff); 
    Cam3_GC0310_write_cmos_sensor(0x41,0x21); 
    Cam3_GC0310_write_cmos_sensor(0x42,0xcf); 
    Cam3_GC0310_write_cmos_sensor(0x44,0x01); // 02 yuv 
    Cam3_GC0310_write_cmos_sensor(0x45,0xa0); // from a8 - a4 a4-a0
    Cam3_GC0310_write_cmos_sensor(0x46,0x03); 
    Cam3_GC0310_write_cmos_sensor(0x4a,0x11);
    Cam3_GC0310_write_cmos_sensor(0x4b,0x01);
    Cam3_GC0310_write_cmos_sensor(0x4c,0x20); 
    Cam3_GC0310_write_cmos_sensor(0x4d,0x05); 
    Cam3_GC0310_write_cmos_sensor(0x4f,0x01);
    Cam3_GC0310_write_cmos_sensor(0x50,0x01); 
    Cam3_GC0310_write_cmos_sensor(0x55,0x01); 
    Cam3_GC0310_write_cmos_sensor(0x56,0xe0);
    Cam3_GC0310_write_cmos_sensor(0x57,0x02); 
    Cam3_GC0310_write_cmos_sensor(0x58,0x80);

    /////////////////////////////////////////////////  
    ///////////////////   GAIN   ////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x70,0x70); 
    Cam3_GC0310_write_cmos_sensor(0x5a,0x84); 
    Cam3_GC0310_write_cmos_sensor(0x5b,0xc9); 
    Cam3_GC0310_write_cmos_sensor(0x5c,0xed); 
    Cam3_GC0310_write_cmos_sensor(0x77,0x74); 
    Cam3_GC0310_write_cmos_sensor(0x78,0x40); 
    Cam3_GC0310_write_cmos_sensor(0x79,0x5f); 

    ///////////////////////////////////////////////// 
    ///////////////////   DNDD  /////////////////////
    ///////////////////////////////////////////////// 
    Cam3_GC0310_write_cmos_sensor(0x82,0x14); 
    Cam3_GC0310_write_cmos_sensor(0x83,0x0b);


    ///////////////////////////////////////////////// 
    //////////////////   EEINTP  ////////////////////
    ///////////////////////////////////////////////// 
    Cam3_GC0310_write_cmos_sensor(0x8f,0xaa); 
    Cam3_GC0310_write_cmos_sensor(0x90,0x8c); 
    Cam3_GC0310_write_cmos_sensor(0x91,0x90); 
    Cam3_GC0310_write_cmos_sensor(0x92,0x05); 
    Cam3_GC0310_write_cmos_sensor(0x93,0x05); 
    Cam3_GC0310_write_cmos_sensor(0x94,0x05);
    Cam3_GC0310_write_cmos_sensor(0x95,0x23); 
    Cam3_GC0310_write_cmos_sensor(0x96,0xf0); 

    ///////////////////////////////////////////////// 
    /////////////////////  ASDE  ////////////////////
    ///////////////////////////////////////////////// 
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);
    Cam3_GC0310_write_cmos_sensor(0x9a,0x20);
    Cam3_GC0310_write_cmos_sensor(0x9b,0x80);
    Cam3_GC0310_write_cmos_sensor(0x9c,0x40);
    Cam3_GC0310_write_cmos_sensor(0x9d,0x80);
    Cam3_GC0310_write_cmos_sensor(0xa1,0x30);
    Cam3_GC0310_write_cmos_sensor(0xa2,0x32);
    Cam3_GC0310_write_cmos_sensor(0xa4,0x30);
    Cam3_GC0310_write_cmos_sensor(0xa5,0x30);
    Cam3_GC0310_write_cmos_sensor(0xaa,0x10);  
    Cam3_GC0310_write_cmos_sensor(0xac,0x22);

    /////////////////////////////////////////////////
    ///////////////////   GAMMA   ///////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xbf,0x08); 
    Cam3_GC0310_write_cmos_sensor(0xc0,0x16); 
    Cam3_GC0310_write_cmos_sensor(0xc1,0x28); 
    Cam3_GC0310_write_cmos_sensor(0xc2,0x41); 
    Cam3_GC0310_write_cmos_sensor(0xc3,0x5a); 
    Cam3_GC0310_write_cmos_sensor(0xc4,0x6c); 
    Cam3_GC0310_write_cmos_sensor(0xc5,0x7a); 
    Cam3_GC0310_write_cmos_sensor(0xc6,0x96); 
    Cam3_GC0310_write_cmos_sensor(0xc7,0xac); 
    Cam3_GC0310_write_cmos_sensor(0xc8,0xbc); 
    Cam3_GC0310_write_cmos_sensor(0xc9,0xc9); 
    Cam3_GC0310_write_cmos_sensor(0xca,0xd3); 
    Cam3_GC0310_write_cmos_sensor(0xcb,0xdd); 
    Cam3_GC0310_write_cmos_sensor(0xcc,0xe5); 
    Cam3_GC0310_write_cmos_sensor(0xcd,0xf1); 
    Cam3_GC0310_write_cmos_sensor(0xce,0xfa); 
    Cam3_GC0310_write_cmos_sensor(0xcf,0xff);

    /////////////////////////////////////////////////
    ///////////////////   YCP  //////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xd0,0x40); 
    Cam3_GC0310_write_cmos_sensor(0xd1,0x2b); 
    Cam3_GC0310_write_cmos_sensor(0xd2,0x2b); 
    Cam3_GC0310_write_cmos_sensor(0xd3,0x40);  
    Cam3_GC0310_write_cmos_sensor(0xd5,0xfb);   
    Cam3_GC0310_write_cmos_sensor(0xd6,0xf2); 
    Cam3_GC0310_write_cmos_sensor(0xd7,0x1b); 
    Cam3_GC0310_write_cmos_sensor(0xd8,0x18); 
    Cam3_GC0310_write_cmos_sensor(0xdd,0x03); 
    /////////////////////////////////////////////////
    ////////////////////   AEC   ////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01);
    Cam3_GC0310_write_cmos_sensor(0x05,0x30); 
    Cam3_GC0310_write_cmos_sensor(0x06,0x75); 
    Cam3_GC0310_write_cmos_sensor(0x07,0x40); 
    Cam3_GC0310_write_cmos_sensor(0x08,0xb0); 
    Cam3_GC0310_write_cmos_sensor(0x0a,0xc5); 
    Cam3_GC0310_write_cmos_sensor(0x0b,0x11);
    Cam3_GC0310_write_cmos_sensor(0x0c,0x00); 
    Cam3_GC0310_write_cmos_sensor(0x12,0x52);
    Cam3_GC0310_write_cmos_sensor(0x13,0x40);                             
    Cam3_GC0310_write_cmos_sensor(0x18,0x95);
    Cam3_GC0310_write_cmos_sensor(0x19,0x96);
    Cam3_GC0310_write_cmos_sensor(0x1f,0x20);
    Cam3_GC0310_write_cmos_sensor(0x20,0xc0); 
    Cam3_GC0310_write_cmos_sensor(0x3e,0x40); 
    Cam3_GC0310_write_cmos_sensor(0x3f,0x57); 
    Cam3_GC0310_write_cmos_sensor(0x40,0x7d); 
    Cam3_GC0310_write_cmos_sensor(0x03,0x60); 
    Cam3_GC0310_write_cmos_sensor(0x44,0x02); 
    /////////////////////////////////////////////////      
    ////////////////////   AWB   ////////////////////      
    ///////////////////////////////////////////////// 
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01);     
    Cam3_GC0310_write_cmos_sensor(0x1c,0x91);                   
    Cam3_GC0310_write_cmos_sensor(0x21,0x15);                   
    Cam3_GC0310_write_cmos_sensor(0x50,0x80);                   
    Cam3_GC0310_write_cmos_sensor(0x56,0x04);                   
    Cam3_GC0310_write_cmos_sensor(0x59,0x08);                   
    Cam3_GC0310_write_cmos_sensor(0x5b,0x02);                   
    Cam3_GC0310_write_cmos_sensor(0x61,0x8d);                   
    Cam3_GC0310_write_cmos_sensor(0x62,0xa7);                   
    Cam3_GC0310_write_cmos_sensor(0x63,0xd0);                   
    Cam3_GC0310_write_cmos_sensor(0x65,0x06);                   
    Cam3_GC0310_write_cmos_sensor(0x66,0x06);                   
    Cam3_GC0310_write_cmos_sensor(0x67,0x84);                   
    Cam3_GC0310_write_cmos_sensor(0x69,0x08);                   
    Cam3_GC0310_write_cmos_sensor(0x6a,0x25);                   
    Cam3_GC0310_write_cmos_sensor(0x6b,0x01);                   
    Cam3_GC0310_write_cmos_sensor(0x6c,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0x6d,0x02);                   
    Cam3_GC0310_write_cmos_sensor(0x6e,0xf0);                   
    Cam3_GC0310_write_cmos_sensor(0x6f,0x80);                   
    Cam3_GC0310_write_cmos_sensor(0x76,0x80);                   
    Cam3_GC0310_write_cmos_sensor(0x78,0xaf);                   
    Cam3_GC0310_write_cmos_sensor(0x79,0x75);                   
    Cam3_GC0310_write_cmos_sensor(0x7a,0x40);                   
    Cam3_GC0310_write_cmos_sensor(0x7b,0x50);	                 
    Cam3_GC0310_write_cmos_sensor(0x7c,0x0c);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xa4,0xb9);                   
    Cam3_GC0310_write_cmos_sensor(0xa5,0xa0);                   
    Cam3_GC0310_write_cmos_sensor(0x90,0xc9);                   
    Cam3_GC0310_write_cmos_sensor(0x91,0xbe);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xa6,0xb8);                   
    Cam3_GC0310_write_cmos_sensor(0xa7,0x95);                   
    Cam3_GC0310_write_cmos_sensor(0x92,0xe6);                   
    Cam3_GC0310_write_cmos_sensor(0x93,0xca);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xa9,0xbc);                   
    Cam3_GC0310_write_cmos_sensor(0xaa,0x95);                   
    Cam3_GC0310_write_cmos_sensor(0x95,0x23);                   
    Cam3_GC0310_write_cmos_sensor(0x96,0xe7);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xab,0x9d);                   
    Cam3_GC0310_write_cmos_sensor(0xac,0x80);                   
    Cam3_GC0310_write_cmos_sensor(0x97,0x43);                   
    Cam3_GC0310_write_cmos_sensor(0x98,0x24);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xae,0xb7);                   
    Cam3_GC0310_write_cmos_sensor(0xaf,0x9e);                   
    Cam3_GC0310_write_cmos_sensor(0x9a,0x43);                   
    Cam3_GC0310_write_cmos_sensor(0x9b,0x24);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xb0,0xc8);                   
    Cam3_GC0310_write_cmos_sensor(0xb1,0x97);                   
    Cam3_GC0310_write_cmos_sensor(0x9c,0xc4);                   
    Cam3_GC0310_write_cmos_sensor(0x9d,0x44);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xb3,0xb7);                   
    Cam3_GC0310_write_cmos_sensor(0xb4,0x7f);                   
    Cam3_GC0310_write_cmos_sensor(0x9f,0xc7);                   
    Cam3_GC0310_write_cmos_sensor(0xa0,0xc8);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xb5,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0xb6,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0xa1,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0xa2,0x00);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0x86,0x60);                   
    Cam3_GC0310_write_cmos_sensor(0x87,0x08);                   
    Cam3_GC0310_write_cmos_sensor(0x88,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0x89,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0x8b,0xde);                   
    Cam3_GC0310_write_cmos_sensor(0x8c,0x80);                   
    Cam3_GC0310_write_cmos_sensor(0x8d,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0x8e,0x00);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0x94,0x55);                   
    Cam3_GC0310_write_cmos_sensor(0x99,0xa6);                   
    Cam3_GC0310_write_cmos_sensor(0x9e,0xaa);                   
    Cam3_GC0310_write_cmos_sensor(0xa3,0x0a);                   
    Cam3_GC0310_write_cmos_sensor(0x8a,0x0a);                   
    Cam3_GC0310_write_cmos_sensor(0xa8,0x55);                   
    Cam3_GC0310_write_cmos_sensor(0xad,0x55);                   
    Cam3_GC0310_write_cmos_sensor(0xb2,0x55);                   
    Cam3_GC0310_write_cmos_sensor(0xb7,0x05);                   
    Cam3_GC0310_write_cmos_sensor(0x8f,0x05);                   
                                               
    Cam3_GC0310_write_cmos_sensor(0xb8,0xcc);                   
    Cam3_GC0310_write_cmos_sensor(0xb9,0x9a);                   
    /////////////////////////////////////////////////      
    ////////////////////   CC    ////////////////////      
    /////////////////////////////////////////////////      
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01);                   
                                               
    /*Cam3_GC0310_write_cmos_sensor(0xd0,0x38);//skin red         
    Cam3_GC0310_write_cmos_sensor(0xd1,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0xd2,0x02);                   
    Cam3_GC0310_write_cmos_sensor(0xd3,0x04);                   
    Cam3_GC0310_write_cmos_sensor(0xd4,0x38);                   
    Cam3_GC0310_write_cmos_sensor(0xd5,0x12);  */                 
                                               
                                            
    Cam3_GC0310_write_cmos_sensor(0xd0,0x38);//skin white       
    Cam3_GC0310_write_cmos_sensor(0xd1,0xfd);                   
    Cam3_GC0310_write_cmos_sensor(0xd2,0x06);                   
    Cam3_GC0310_write_cmos_sensor(0xd3,0xf0);                   
    Cam3_GC0310_write_cmos_sensor(0xd4,0x40);                   
    Cam3_GC0310_write_cmos_sensor(0xd5,0x08);                   
                                          
                                                 
    /*                                                     
    Cam3_GC0310_write_cmos_sensor(0xd0,0x38);//guodengxiang     
    Cam3_GC0310_write_cmos_sensor(0xd1,0xf8);                   
    Cam3_GC0310_write_cmos_sensor(0xd2,0x06);                   
    Cam3_GC0310_write_cmos_sensor(0xd3,0xfd);                   
    Cam3_GC0310_write_cmos_sensor(0xd4,0x40);                   
    Cam3_GC0310_write_cmos_sensor(0xd5,0x00);                   
    */                                                     
    Cam3_GC0310_write_cmos_sensor(0xd6,0x30);                   
    Cam3_GC0310_write_cmos_sensor(0xd7,0x00);                   
    Cam3_GC0310_write_cmos_sensor(0xd8,0x0a);                   
    Cam3_GC0310_write_cmos_sensor(0xd9,0x16);                   
    Cam3_GC0310_write_cmos_sensor(0xda,0x39);                   
    Cam3_GC0310_write_cmos_sensor(0xdb,0xf8);                   


    /////////////////////////////////////////////////
    ////////////////////   LSC   ////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01); 
    Cam3_GC0310_write_cmos_sensor(0xc1,0x3c); 
    Cam3_GC0310_write_cmos_sensor(0xc2,0x50); 
    Cam3_GC0310_write_cmos_sensor(0xc3,0x00); 
    Cam3_GC0310_write_cmos_sensor(0xc4,0x40); 
    Cam3_GC0310_write_cmos_sensor(0xc5,0x30); 
    Cam3_GC0310_write_cmos_sensor(0xc6,0x30); 
    Cam3_GC0310_write_cmos_sensor(0xc7,0x10); 
    Cam3_GC0310_write_cmos_sensor(0xc8,0x00); 
    Cam3_GC0310_write_cmos_sensor(0xc9,0x00); 
    Cam3_GC0310_write_cmos_sensor(0xdc,0x20); 
    Cam3_GC0310_write_cmos_sensor(0xdd,0x10); 
    Cam3_GC0310_write_cmos_sensor(0xdf,0x00); 
    Cam3_GC0310_write_cmos_sensor(0xde,0x00); 

    /////////////////////////////////////////////////
    ///////////////////  Histogram  /////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x01,0x10); 
    Cam3_GC0310_write_cmos_sensor(0x0b,0x31); 
    Cam3_GC0310_write_cmos_sensor(0x0e,0x50); 
    Cam3_GC0310_write_cmos_sensor(0x0f,0x0f); 
    Cam3_GC0310_write_cmos_sensor(0x10,0x6e); 
    Cam3_GC0310_write_cmos_sensor(0x12,0xa0); 
    Cam3_GC0310_write_cmos_sensor(0x15,0x60); 
    Cam3_GC0310_write_cmos_sensor(0x16,0x60); 
    Cam3_GC0310_write_cmos_sensor(0x17,0xe0); 

    /////////////////////////////////////////////////
    //////////////   Measure Window   ///////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xcc,0x0c);  
    Cam3_GC0310_write_cmos_sensor(0xcd,0x10); 
    Cam3_GC0310_write_cmos_sensor(0xce,0xa0); 
    Cam3_GC0310_write_cmos_sensor(0xcf,0xe6); 

    /////////////////////////////////////////////////
    /////////////////   dark sun   //////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0x45,0xf7);
    Cam3_GC0310_write_cmos_sensor(0x46,0xff); 
    Cam3_GC0310_write_cmos_sensor(0x47,0x15);
    Cam3_GC0310_write_cmos_sensor(0x48,0x03); 
    Cam3_GC0310_write_cmos_sensor(0x4f,0x60); 

    /////////////////////////////////////////////////
    ///////////////////  banding  ///////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);             
    Cam3_GC0310_write_cmos_sensor(0x05,0x02);             
    Cam3_GC0310_write_cmos_sensor(0x06,0xd1); //HB        
    Cam3_GC0310_write_cmos_sensor(0x07,0x00);             
    Cam3_GC0310_write_cmos_sensor(0x08,0x22); //VB        
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01);             
    Cam3_GC0310_write_cmos_sensor(0x25,0x00); //step      
    Cam3_GC0310_write_cmos_sensor(0x26,0x6a);             
    Cam3_GC0310_write_cmos_sensor(0x27,0x03); //20fps     
    Cam3_GC0310_write_cmos_sensor(0x28,0x50);             
    Cam3_GC0310_write_cmos_sensor(0x29,0x04); //12.5fps   
    Cam3_GC0310_write_cmos_sensor(0x2a,0x24);             
    Cam3_GC0310_write_cmos_sensor(0x2b,0x05); //7.14fps   
    Cam3_GC0310_write_cmos_sensor(0x2c,0xcc);             
    Cam3_GC0310_write_cmos_sensor(0x2d,0x07); //5.55fps   
    Cam3_GC0310_write_cmos_sensor(0x2e,0x74);              
    Cam3_GC0310_write_cmos_sensor(0x3c,0x20);              
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);              

    /////////////////////////////////////////////////
    ///////////////////   MIPI   ////////////////////
    /////////////////////////////////////////////////
    Cam3_GC0310_write_cmos_sensor(0xfe,0x03);
    Cam3_GC0310_write_cmos_sensor(0x10,0x94);  
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);

    Cam3_GC0310_write_cmos_sensor(0xfe,0x03);
    Cam3_GC0310_write_cmos_sensor(0xf3,0x83);
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);
    mdelay(50);
}

int Cam3_GC0310GetBrightness(void)
{
    int brightness = 0;
    Cam3_GC0310_write_cmos_sensor(0xfe,0x01);
    brightness = Cam3_GC0310_read_cmos_sensor(0x14);
    Cam3_GC0310_write_cmos_sensor(0xfe,0x00);
    return brightness;
}

CAM3_FUNCTION_STRUCT gc0310_sensor_func = {
    Cam3_GC0310CheckIsReady,            //CheckIsReady
    Cam3_GC0310_Sensor_preview,         //StrartPreview
    Cam3_GC0310GetBrightness,           //GetBrightness
    "gc0310",                           //SensorName
    PWD_LEVEL_HIGH,                     //PowerDownEnable, Notice this value means sensor power off value, not work value
};
