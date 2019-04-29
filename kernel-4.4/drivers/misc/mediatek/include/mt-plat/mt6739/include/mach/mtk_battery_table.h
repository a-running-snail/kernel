/*
* Copyright (C) 2016 MediaTek Inc.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See http://www.gnu.org/licenses/gpl-2.0.html for more details.
*/

#ifndef _MTK_BATTERY_TABLE_H
#define _MTK_BATTERY_TABLE_H

#include "mtk_battery.h"

/* ============================================================
 * define
 * ============================================================
 */

/* Qmax for battery  */
#define Q_MAX_L_CURRENT 0
#define Q_MAX_H_CURRENT 10000

/* multiple battery profile compile options */
/*#define MTK_GET_BATTERY_ID_BY_AUXADC*/
#define BATTERY_ID_CHANNEL_NUM 1
#define BATTERY_PROFILE_ID 0
#define TOTAL_BATTERY_NUMBER 4
/*#define MTK_GET_BATTERY_ID_BY_GPIO*/

/* Qmax for battery  */
int g_Q_MAX_T0[TOTAL_BATTERY_NUMBER] = { 2800, 2800, 2800, 2800};
int g_Q_MAX_T1[TOTAL_BATTERY_NUMBER] = { 3100, 3100, 3100, 3100};
int g_Q_MAX_T2[TOTAL_BATTERY_NUMBER] = { 2600, 2600, 2600, 2600};
int g_Q_MAX_T3[TOTAL_BATTERY_NUMBER] = { 2400, 2400, 2400, 2400};
int g_Q_MAX_T4[TOTAL_BATTERY_NUMBER] = { 2400, 2400, 2400, 2400};

int g_Q_MAX_T0_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2750, 2750, 2750, 2750};
int g_Q_MAX_T1_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 3100, 3100, 3100, 3100};
int g_Q_MAX_T2_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2100, 2100, 2100, 2100};
int g_Q_MAX_T3_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 700, 700, 700, 700};
int g_Q_MAX_T4_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 700, 700, 700, 700};

int g_Q_MAX_SYS_VOLTAGE[TOTAL_BATTERY_NUMBER] = { 3400, 3400, 3400, 3400};

/* 0~0.5V for battery 0, 0.5~1V for battery 1*/
/* 1~1.5V for battery 2, -1 for the last one (battery 3) */
int g_battery_id_voltage[TOTAL_BATTERY_NUMBER] = {
	500000, 1000000, 1500000, -1};

int g_FG_PSEUDO1_T0[TOTAL_BATTERY_NUMBER] = { 2, 2, 2, 2};
int g_FG_PSEUDO1_T1[TOTAL_BATTERY_NUMBER] = { 2, 2, 2, 2};
int g_FG_PSEUDO1_T2[TOTAL_BATTERY_NUMBER] = { 2, 2, 2, 2};
int g_FG_PSEUDO1_T3[TOTAL_BATTERY_NUMBER] = { 2, 2, 2, 2};
int g_FG_PSEUDO1_T4[TOTAL_BATTERY_NUMBER] = { 2, 2, 2, 2};

int g_FG_PSEUDO100_T0[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 98};
int g_FG_PSEUDO100_T1[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 98};
int g_FG_PSEUDO100_T2[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 98};
int g_FG_PSEUDO100_T3[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 98};
int g_FG_PSEUDO100_T4[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 98};

/* shutdown_hl_zcv */
int g_SHUTDOWN_HL_ZCV_T0[TOTAL_BATTERY_NUMBER] = {3400, 3400, 3400, 3400};
int g_SHUTDOWN_HL_ZCV_T1[TOTAL_BATTERY_NUMBER] = {3450, 3400, 3400, 3400};
int g_SHUTDOWN_HL_ZCV_T2[TOTAL_BATTERY_NUMBER] = {3400, 3400, 3520, 3400};
int g_SHUTDOWN_HL_ZCV_T3[TOTAL_BATTERY_NUMBER] = {3400, 3400, 3400, 3400};
int g_SHUTDOWN_HL_ZCV_T4[TOTAL_BATTERY_NUMBER] = {3400, 3400, 3400, 3400};

/* g_FG_PSEUDO1_OFFSET */
int g_FG_PSEUDO1_OFFSET[TOTAL_BATTERY_NUMBER] = {0, 0, 0, 0};

/* pmic_min_vol by temp ,control by MULTI_TEMP_GAUGE0=1, 34000=3.4v */
int g_PMIC_MIN_VOL_T0[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_PMIC_MIN_VOL_T1[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_PMIC_MIN_VOL_T2[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_PMIC_MIN_VOL_T3[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_PMIC_MIN_VOL_T4[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};

/* POWERON_SYSTEM_IBOOT by temp ,control by MULTI_TEMP_GAUGE0=1 ,5000 = 500ma*/
int g_PON_SYS_IBOOT_T0[TOTAL_BATTERY_NUMBER] = {4000, 4100, 4200, 4300};
int g_PON_SYS_IBOOT_T1[TOTAL_BATTERY_NUMBER] = {5000, 5100, 5200, 5300};
int g_PON_SYS_IBOOT_T2[TOTAL_BATTERY_NUMBER] = {6000, 6100, 6200, 6300};
int g_PON_SYS_IBOOT_T3[TOTAL_BATTERY_NUMBER] = {7000, 7100, 7200, 7300};
int g_PON_SYS_IBOOT_T4[TOTAL_BATTERY_NUMBER] = {8000, 8100, 8200, 8300};

/* Q_MAX_SYS_VOLTAGE by temp ,control by MULTI_TEMP_GAUGE0=1, */
int g_QMAX_SYS_VOL_T0[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_QMAX_SYS_VOL_T1[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_QMAX_SYS_VOL_T2[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_QMAX_SYS_VOL_T3[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};
int g_QMAX_SYS_VOL_T4[TOTAL_BATTERY_NUMBER] = {34000, 34000, 34000, 34000};

#define BAT_NTC_10 1
#define BAT_NTC_47 0

#if (BAT_NTC_10 == 1)
#define RBAT_PULL_UP_R             16900
#endif

#if (BAT_NTC_47 == 1)
#define RBAT_PULL_UP_R             61900
#endif

#define RBAT_PULL_UP_VOLT          1800

#define BIF_NTC_R 16000

#if (BAT_NTC_10 == 1)
struct FUELGAUGE_TEMPERATURE Fg_Temperature_Table[21] = {
		{-40, 195652},
		{-35, 148171},
		{-30, 113347},
		{-25, 87559},
		{-20, 68237},
		{-15, 53650},
		{-10, 42506},
		{-5, 33892},
		{0, 27219},
		{5, 22021},
		{10, 17926},
		{15, 14674},
		{20, 12081},
		{25, 10000},
		{30, 8315},
		{35, 6948},
		{40, 5834},
		{45, 4917},
		{50, 4161},
		{55, 3535},
		{60, 3014}
};
#endif

#if (BAT_NTC_47 == 1)
struct FUELGAUGE_TEMPERATURE Fg_Temperature_Table[21] = {
		{-40, 1747920},
		{-35, 1245428},
		{-30, 898485},
		{-25, 655802},
		{-20, 483954},
		{-15, 360850},
		{-10, 271697},
		{-5, 206463},
		{0, 158214},
		{5, 122259},
		{10, 95227},
		{15, 74730},
		{20, 59065},
		{25, 47000},
		{30, 37643},
		{35, 30334},
		{40, 24591},
		{45, 20048},
		{50, 16433},
		{55, 13539},
		{60, 11210}
};
#endif



/* ============================================================
 * <DOD, Battery_Voltage> Table
 * ============================================================
 */

#define TEMPERATURE_T0	50
#define TEMPERATURE_T1	25
#define TEMPERATURE_T2	0
#define TEMPERATURE_T3	-10
#define TEMPERATURE_T4	-20
#define TEMPERATURE_TB0	255
#define TEMPERATURE_TB1	254



/* T0 */
struct FUELGAUGE_PROFILE_STRUCT fg_profile_t0[TOTAL_BATTERY_NUMBER][59] = {
	{
		{0,43880,1100},
		{500,43620,800},
		{1000,43400,750},
		{1500,43190,750},
		{2000,42980,770},
		{2500,42780,790},
		{3000,42570,760},
		{3500,42360,770},
		{4000,42160,790},
		{4500,41950,760},
		{5000,41740,770},
		{5500,41550,770},
		{6000,41360,810},
		{6500,41170,780},
		{7000,40980,800},
		{7500,40800,810},
		{8000,40610,800},
		{8500,40450,880},
		{9000,40280,880},
		{9500,40130,910},
		{10000,39960,890},
		{10500,39820,970},
		{11000,39670,990},
		{11500,39520,1010},
		{12000,39360,1060},
		{12500,39180,1020},
		{13000,38970,850},
		{13500,38800,760},
		{14000,38670,720},
		{14500,38560,710},
		{15000,38470,740},
		{15500,38370,750},
		{16000,38280,710},
		{16500,38220,780},
		{17000,38150,790},
		{17500,38080,780},
		{18000,38020,830},
		{18500,37960,840},
		{19000,37910,880},
		{19500,37850,910},
		{20000,37800,910},
		{20500,37740,910},
		{21000,37620,810},
		{21500,37540,790},
		{22000,37470,800},
		{22500,37380,760},
		{23000,37320,800},
		{23500,37250,830},
		{24000,37170,780},
		{24500,37050,780},
		{25000,36950,810},
		{25500,36810,760},
		{26000,36780,800},
		{26500,36760,840},
		{27000,36730,950},
		{27500,36500,920},
		{28000,35720,960},
		{28500,34430,1210},
		{29000,34430,1210},
	},
	{
		{0,43880,1100},
		{500,43620,800},
		{1000,43400,750},
		{1500,43190,750},
		{2000,42980,770},
		{2500,42780,790},
		{3000,42570,760},
		{3500,42360,770},
		{4000,42160,790},
		{4500,41950,760},
		{5000,41740,770},
		{5500,41550,770},
		{6000,41360,810},
		{6500,41170,780},
		{7000,40980,800},
		{7500,40800,810},
		{8000,40610,800},
		{8500,40450,880},
		{9000,40280,880},
		{9500,40130,910},
		{10000,39960,890},
		{10500,39820,970},
		{11000,39670,990},
		{11500,39520,1010},
		{12000,39360,1060},
		{12500,39180,1020},
		{13000,38970,850},
		{13500,38800,760},
		{14000,38670,720},
		{14500,38560,710},
		{15000,38470,740},
		{15500,38370,750},
		{16000,38280,710},
		{16500,38220,780},
		{17000,38150,790},
		{17500,38080,780},
		{18000,38020,830},
		{18500,37960,840},
		{19000,37910,880},
		{19500,37850,910},
		{20000,37800,910},
		{20500,37740,910},
		{21000,37620,810},
		{21500,37540,790},
		{22000,37470,800},
		{22500,37380,760},
		{23000,37320,800},
		{23500,37250,830},
		{24000,37170,780},
		{24500,37050,780},
		{25000,36950,810},
		{25500,36810,760},
		{26000,36780,800},
		{26500,36760,840},
		{27000,36730,950},
		{27500,36500,920},
		{28000,35720,960},
		{28500,34430,1210},
		{29000,34430,1210},
	},
	{
		{0,43880,1100},
		{500,43620,800},
		{1000,43400,750},
		{1500,43190,750},
		{2000,42980,770},
		{2500,42780,790},
		{3000,42570,760},
		{3500,42360,770},
		{4000,42160,790},
		{4500,41950,760},
		{5000,41740,770},
		{5500,41550,770},
		{6000,41360,810},
		{6500,41170,780},
		{7000,40980,800},
		{7500,40800,810},
		{8000,40610,800},
		{8500,40450,880},
		{9000,40280,880},
		{9500,40130,910},
		{10000,39960,890},
		{10500,39820,970},
		{11000,39670,990},
		{11500,39520,1010},
		{12000,39360,1060},
		{12500,39180,1020},
		{13000,38970,850},
		{13500,38800,760},
		{14000,38670,720},
		{14500,38560,710},
		{15000,38470,740},
		{15500,38370,750},
		{16000,38280,710},
		{16500,38220,780},
		{17000,38150,790},
		{17500,38080,780},
		{18000,38020,830},
		{18500,37960,840},
		{19000,37910,880},
		{19500,37850,910},
		{20000,37800,910},
		{20500,37740,910},
		{21000,37620,810},
		{21500,37540,790},
		{22000,37470,800},
		{22500,37380,760},
		{23000,37320,800},
		{23500,37250,830},
		{24000,37170,780},
		{24500,37050,780},
		{25000,36950,810},
		{25500,36810,760},
		{26000,36780,800},
		{26500,36760,840},
		{27000,36730,950},
		{27500,36500,920},
		{28000,35720,960},
		{28500,34430,1210},
		{29000,34430,1210},
	},
	{
		{0,43880,1100},
		{500,43620,800},
		{1000,43400,750},
		{1500,43190,750},
		{2000,42980,770},
		{2500,42780,790},
		{3000,42570,760},
		{3500,42360,770},
		{4000,42160,790},
		{4500,41950,760},
		{5000,41740,770},
		{5500,41550,770},
		{6000,41360,810},
		{6500,41170,780},
		{7000,40980,800},
		{7500,40800,810},
		{8000,40610,800},
		{8500,40450,880},
		{9000,40280,880},
		{9500,40130,910},
		{10000,39960,890},
		{10500,39820,970},
		{11000,39670,990},
		{11500,39520,1010},
		{12000,39360,1060},
		{12500,39180,1020},
		{13000,38970,850},
		{13500,38800,760},
		{14000,38670,720},
		{14500,38560,710},
		{15000,38470,740},
		{15500,38370,750},
		{16000,38280,710},
		{16500,38220,780},
		{17000,38150,790},
		{17500,38080,780},
		{18000,38020,830},
		{18500,37960,840},
		{19000,37910,880},
		{19500,37850,910},
		{20000,37800,910},
		{20500,37740,910},
		{21000,37620,810},
		{21500,37540,790},
		{22000,37470,800},
		{22500,37380,760},
		{23000,37320,800},
		{23500,37250,830},
		{24000,37170,780},
		{24500,37050,780},
		{25000,36950,810},
		{25500,36810,760},
		{26000,36780,800},
		{26500,36760,840},
		{27000,36730,950},
		{27500,36500,920},
		{28000,35720,960},
		{28500,34430,1210},
		{29000,34430,1210},
	}
};

/* T1 */
struct FUELGAUGE_PROFILE_STRUCT fg_profile_t1[TOTAL_BATTERY_NUMBER][59] = {
	{
		{0,43800,1380},
		{500,43520,1470},
		{1000,43290,1470},
		{1500,43060,1460},
		{2000,42850,1400},
		{2500,42650,1430},
		{3000,42450,1420},
		{3500,42240,1420},
		{4000,42050,1450},
		{4500,41830,1380},
		{5000,41650,1390},
		{5500,41460,1430},
		{6000,41270,1450},
		{6500,41090,1440},
		{7000,40900,1450},
		{7500,40720,1420},
		{8000,40560,1480},
		{8500,40380,1500},
		{9000,40200,1520},
		{9500,40020,1510},
		{10000,39890,1560},
		{10500,39750,1620},
		{11000,39610,1610},
		{11500,39460,1640},
		{12000,39310,1600},
		{12500,39140,1550},
		{13000,38950,1380},
		{13500,38780,1290},
		{14000,38630,1230},
		{14500,38530,1210},
		{15000,38430,1220},
		{15500,38340,1230},
		{16000,38260,1200},
		{16500,38190,1250},
		{17000,38130,1290},
		{17500,38060,1260},
		{18000,38010,1320},
		{18500,37940,1300},
		{19000,37890,1340},
		{19500,37840,1360},
		{20000,37810,1390},
		{20500,37760,1390},
		{21000,37710,1390},
		{21500,37640,1300},
		{22000,37570,1270},
		{22500,37510,1290},
		{23000,37450,1280},
		{23500,37390,1370},
		{24000,37300,1360},
		{24500,37170,1360},
		{25000,37070,1410},
		{25500,36930,1450},
		{26000,36870,1470},
		{26500,36860,1590},
		{27000,36820,1780},
		{27500,36670,1990},
		{28000,35930,2120},
		{28500,34650,2590},
		{29000,34650,2590},
	},
	{
		{0,43800,1380},
		{500,43520,1470},
		{1000,43290,1470},
		{1500,43060,1460},
		{2000,42850,1400},
		{2500,42650,1430},
		{3000,42450,1420},
		{3500,42240,1420},
		{4000,42050,1450},
		{4500,41830,1380},
		{5000,41650,1390},
		{5500,41460,1430},
		{6000,41270,1450},
		{6500,41090,1440},
		{7000,40900,1450},
		{7500,40720,1420},
		{8000,40560,1480},
		{8500,40380,1500},
		{9000,40200,1520},
		{9500,40020,1510},
		{10000,39890,1560},
		{10500,39750,1620},
		{11000,39610,1610},
		{11500,39460,1640},
		{12000,39310,1600},
		{12500,39140,1550},
		{13000,38950,1380},
		{13500,38780,1290},
		{14000,38630,1230},
		{14500,38530,1210},
		{15000,38430,1220},
		{15500,38340,1230},
		{16000,38260,1200},
		{16500,38190,1250},
		{17000,38130,1290},
		{17500,38060,1260},
		{18000,38010,1320},
		{18500,37940,1300},
		{19000,37890,1340},
		{19500,37840,1360},
		{20000,37810,1390},
		{20500,37760,1390},
		{21000,37710,1390},
		{21500,37640,1300},
		{22000,37570,1270},
		{22500,37510,1290},
		{23000,37450,1280},
		{23500,37390,1370},
		{24000,37300,1360},
		{24500,37170,1360},
		{25000,37070,1410},
		{25500,36930,1450},
		{26000,36870,1470},
		{26500,36860,1590},
		{27000,36820,1780},
		{27500,36670,1990},
		{28000,35930,2120},
		{28500,34650,2590},
		{29000,34650,2590},
	},
	{
		{0,43800,1380},
		{500,43520,1470},
		{1000,43290,1470},
		{1500,43060,1460},
		{2000,42850,1400},
		{2500,42650,1430},
		{3000,42450,1420},
		{3500,42240,1420},
		{4000,42050,1450},
		{4500,41830,1380},
		{5000,41650,1390},
		{5500,41460,1430},
		{6000,41270,1450},
		{6500,41090,1440},
		{7000,40900,1450},
		{7500,40720,1420},
		{8000,40560,1480},
		{8500,40380,1500},
		{9000,40200,1520},
		{9500,40020,1510},
		{10000,39890,1560},
		{10500,39750,1620},
		{11000,39610,1610},
		{11500,39460,1640},
		{12000,39310,1600},
		{12500,39140,1550},
		{13000,38950,1380},
		{13500,38780,1290},
		{14000,38630,1230},
		{14500,38530,1210},
		{15000,38430,1220},
		{15500,38340,1230},
		{16000,38260,1200},
		{16500,38190,1250},
		{17000,38130,1290},
		{17500,38060,1260},
		{18000,38010,1320},
		{18500,37940,1300},
		{19000,37890,1340},
		{19500,37840,1360},
		{20000,37810,1390},
		{20500,37760,1390},
		{21000,37710,1390},
		{21500,37640,1300},
		{22000,37570,1270},
		{22500,37510,1290},
		{23000,37450,1280},
		{23500,37390,1370},
		{24000,37300,1360},
		{24500,37170,1360},
		{25000,37070,1410},
		{25500,36930,1450},
		{26000,36870,1470},
		{26500,36860,1590},
		{27000,36820,1780},
		{27500,36670,1990},
		{28000,35930,2120},
		{28500,34650,2590},
		{29000,34650,2590},
	},
	{
		{0,43800,1380},
		{500,43520,1470},
		{1000,43290,1470},
		{1500,43060,1460},
		{2000,42850,1400},
		{2500,42650,1430},
		{3000,42450,1420},
		{3500,42240,1420},
		{4000,42050,1450},
		{4500,41830,1380},
		{5000,41650,1390},
		{5500,41460,1430},
		{6000,41270,1450},
		{6500,41090,1440},
		{7000,40900,1450},
		{7500,40720,1420},
		{8000,40560,1480},
		{8500,40380,1500},
		{9000,40200,1520},
		{9500,40020,1510},
		{10000,39890,1560},
		{10500,39750,1620},
		{11000,39610,1610},
		{11500,39460,1640},
		{12000,39310,1600},
		{12500,39140,1550},
		{13000,38950,1380},
		{13500,38780,1290},
		{14000,38630,1230},
		{14500,38530,1210},
		{15000,38430,1220},
		{15500,38340,1230},
		{16000,38260,1200},
		{16500,38190,1250},
		{17000,38130,1290},
		{17500,38060,1260},
		{18000,38010,1320},
		{18500,37940,1300},
		{19000,37890,1340},
		{19500,37840,1360},
		{20000,37810,1390},
		{20500,37760,1390},
		{21000,37710,1390},
		{21500,37640,1300},
		{22000,37570,1270},
		{22500,37510,1290},
		{23000,37450,1280},
		{23500,37390,1370},
		{24000,37300,1360},
		{24500,37170,1360},
		{25000,37070,1410},
		{25500,36930,1450},
		{26000,36870,1470},
		{26500,36860,1590},
		{27000,36820,1780},
		{27500,36670,1990},
		{28000,35930,2120},
		{28500,34650,2590},
		{29000,34650,2590},
	}

};

/* T2 */
struct FUELGAUGE_PROFILE_STRUCT fg_profile_t2[TOTAL_BATTERY_NUMBER][59] = {
	{
		{0,43650,2250},
		{500,43320,5340},
		{1000,42950,5640},
		{1500,42540,5970},
		{2000,42180,6430},
		{2500,41870,6860},
		{3000,41650,7130},
		{3500,41440,7120},
		{4000,41240,7070},
		{4500,41060,7100},
		{5000,40910,7110},
		{5500,40750,7190},
		{6000,40560,7150},
		{6500,40320,7030},
		{7000,40100,6920},
		{7500,39900,6880},
		{8000,39760,6890},
		{8500,39620,6900},
		{9000,39470,6880},
		{9500,39330,6880},
		{10000,39160,6760},
		{10500,39000,6710},
		{11000,38830,6620},
		{11500,38680,6550},
		{12000,38560,6530},
		{12500,38450,6510},
		{13000,38370,6560},
		{13500,38280,6580},
		{14000,38190,6630},
		{14500,38130,6680},
		{15000,38060,6720},
		{15500,38000,6760},
		{16000,37940,6800},
		{16500,37900,6880},
		{17000,37860,7010},
		{17500,37820,7100},
		{18000,37800,7290},
		{18500,37760,7420},
		{19000,37720,7630},
		{19500,37660,7790},
		{20000,37610,7990},
		{20500,37560,8290},
		{21000,37480,8550},
		{21500,37400,8880},
		{22000,37300,9190},
		{22500,37180,9530},
		{23000,37080,9940},
		{23500,36990,10390},
		{24000,36940,11080},
		{24500,36870,11880},
		{25000,36760,12880},
		{25500,36420,13810},
		{26000,35620,14770},
		{26500,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
	},
	{
		{0,43650,2250},
		{500,43320,5340},
		{1000,42950,5640},
		{1500,42540,5970},
		{2000,42180,6430},
		{2500,41870,6860},
		{3000,41650,7130},
		{3500,41440,7120},
		{4000,41240,7070},
		{4500,41060,7100},
		{5000,40910,7110},
		{5500,40750,7190},
		{6000,40560,7150},
		{6500,40320,7030},
		{7000,40100,6920},
		{7500,39900,6880},
		{8000,39760,6890},
		{8500,39620,6900},
		{9000,39470,6880},
		{9500,39330,6880},
		{10000,39160,6760},
		{10500,39000,6710},
		{11000,38830,6620},
		{11500,38680,6550},
		{12000,38560,6530},
		{12500,38450,6510},
		{13000,38370,6560},
		{13500,38280,6580},
		{14000,38190,6630},
		{14500,38130,6680},
		{15000,38060,6720},
		{15500,38000,6760},
		{16000,37940,6800},
		{16500,37900,6880},
		{17000,37860,7010},
		{17500,37820,7100},
		{18000,37800,7290},
		{18500,37760,7420},
		{19000,37720,7630},
		{19500,37660,7790},
		{20000,37610,7990},
		{20500,37560,8290},
		{21000,37480,8550},
		{21500,37400,8880},
		{22000,37300,9190},
		{22500,37180,9530},
		{23000,37080,9940},
		{23500,36990,10390},
		{24000,36940,11080},
		{24500,36870,11880},
		{25000,36760,12880},
		{25500,36420,13810},
		{26000,35620,14770},
		{26500,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
	},
	{
		{0,43650,2250},
		{500,43320,5340},
		{1000,42950,5640},
		{1500,42540,5970},
		{2000,42180,6430},
		{2500,41870,6860},
		{3000,41650,7130},
		{3500,41440,7120},
		{4000,41240,7070},
		{4500,41060,7100},
		{5000,40910,7110},
		{5500,40750,7190},
		{6000,40560,7150},
		{6500,40320,7030},
		{7000,40100,6920},
		{7500,39900,6880},
		{8000,39760,6890},
		{8500,39620,6900},
		{9000,39470,6880},
		{9500,39330,6880},
		{10000,39160,6760},
		{10500,39000,6710},
		{11000,38830,6620},
		{11500,38680,6550},
		{12000,38560,6530},
		{12500,38450,6510},
		{13000,38370,6560},
		{13500,38280,6580},
		{14000,38190,6630},
		{14500,38130,6680},
		{15000,38060,6720},
		{15500,38000,6760},
		{16000,37940,6800},
		{16500,37900,6880},
		{17000,37860,7010},
		{17500,37820,7100},
		{18000,37800,7290},
		{18500,37760,7420},
		{19000,37720,7630},
		{19500,37660,7790},
		{20000,37610,7990},
		{20500,37560,8290},
		{21000,37480,8550},
		{21500,37400,8880},
		{22000,37300,9190},
		{22500,37180,9530},
		{23000,37080,9940},
		{23500,36990,10390},
		{24000,36940,11080},
		{24500,36870,11880},
		{25000,36760,12880},
		{25500,36420,13810},
		{26000,35620,14770},
		{26500,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
	},
	{
		{0,43650,2250},
		{500,43320,5340},
		{1000,42950,5640},
		{1500,42540,5970},
		{2000,42180,6430},
		{2500,41870,6860},
		{3000,41650,7130},
		{3500,41440,7120},
		{4000,41240,7070},
		{4500,41060,7100},
		{5000,40910,7110},
		{5500,40750,7190},
		{6000,40560,7150},
		{6500,40320,7030},
		{7000,40100,6920},
		{7500,39900,6880},
		{8000,39760,6890},
		{8500,39620,6900},
		{9000,39470,6880},
		{9500,39330,6880},
		{10000,39160,6760},
		{10500,39000,6710},
		{11000,38830,6620},
		{11500,38680,6550},
		{12000,38560,6530},
		{12500,38450,6510},
		{13000,38370,6560},
		{13500,38280,6580},
		{14000,38190,6630},
		{14500,38130,6680},
		{15000,38060,6720},
		{15500,38000,6760},
		{16000,37940,6800},
		{16500,37900,6880},
		{17000,37860,7010},
		{17500,37820,7100},
		{18000,37800,7290},
		{18500,37760,7420},
		{19000,37720,7630},
		{19500,37660,7790},
		{20000,37610,7990},
		{20500,37560,8290},
		{21000,37480,8550},
		{21500,37400,8880},
		{22000,37300,9190},
		{22500,37180,9530},
		{23000,37080,9940},
		{23500,36990,10390},
		{24000,36940,11080},
		{24500,36870,11880},
		{25000,36760,12880},
		{25500,36420,13810},
		{26000,35620,14770},
		{26500,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
		{27000,34170,16480},
	}

};

/* T3 */
struct FUELGAUGE_PROFILE_STRUCT fg_profile_t3[TOTAL_BATTERY_NUMBER][59] = {
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	}

};

/* T4 */
struct FUELGAUGE_PROFILE_STRUCT fg_profile_t4[TOTAL_BATTERY_NUMBER][59] = {
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},

	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	},
	{
		{0,43110,2680},
		{500,42730,12130},
		{1000,42300,12110},
		{1500,41820,12820},
		{2000,41450,13640},
		{2500,41160,14040},
		{3000,40970,14050},
		{3500,40720,13840},
		{4000,40450,13520},
		{4500,40190,13290},
		{5000,39990,13150},
		{5500,39820,13080},
		{6000,39680,13050},
		{6500,39520,13020},
		{7000,39350,12910},
		{7500,39180,12860},
		{8000,39010,12770},
		{8500,38840,12670},
		{9000,38660,12500},
		{9500,38530,12490},
		{10000,38410,12390},
		{10500,38320,12430},
		{11000,38230,12430},
		{11500,38160,12490},
		{12000,38100,12500},
		{12500,38050,12630},
		{13000,38000,12730},
		{13500,37970,12890},
		{14000,37920,13090},
		{14500,37880,13280},
		{15000,37840,13520},
		{15500,37800,13780},
		{16000,37770,14070},
		{16500,37720,14390},
		{17000,37660,14740},
		{17500,37600,15040},
		{18000,37550,15380},
		{18500,37470,15730},
		{19000,37400,16080},
		{19500,37310,16460},
		{20000,37230,16770},
		{20500,37160,17200},
		{21000,37080,17600},
		{21500,37000,18140},
		{22000,37170,19290},
		{22500,36930,15340},
		{23000,36710,19430},
		{23500,36400,20660},
		{24000,35730,21420},
		{24500,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
		{25000,34370,23050},
	}

};

/* ============================================================
 * function prototype
 * ============================================================
 */

#endif
