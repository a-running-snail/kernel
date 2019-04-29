#include <linux/types.h>
#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#endif
#include <mt-plat/mtk_boot.h>
#include <mt-plat/upmu_common.h>
#include <mt-plat/charger_type.h>
#include "mtk_charger_intf.h"

#include "eta6937.h"

/**********************************************************
  *
  *   [I2C Slave Setting]
  *
  *********************************************************/
typedef enum {
	BATTERY_VOLT_00_000000_V = 0,
	BATTERY_VOLT_00_100000_V = 100000,
	BATTERY_VOLT_00_200000_V = 200000,
	BATTERY_VOLT_00_300000_V = 300000,
	BATTERY_VOLT_00_400000_V = 400000,
	BATTERY_VOLT_00_500000_V = 500000,
	BATTERY_VOLT_00_600000_V = 600000,
	BATTERY_VOLT_00_700000_V = 700000,
	BATTERY_VOLT_00_725000_V = 725000,
	BATTERY_VOLT_00_750000_V = 750000,
	BATTERY_VOLT_00_775000_V = 775000,
	BATTERY_VOLT_00_800000_V = 800000,
	BATTERY_VOLT_00_825000_V = 825000,
	BATTERY_VOLT_00_850000_V = 850000,
	BATTERY_VOLT_00_875000_V = 875000,
	BATTERY_VOLT_00_900000_V = 900000,
	BATTERY_VOLT_00_925000_V = 925000,
	BATTERY_VOLT_00_950000_V = 950000,
	BATTERY_VOLT_00_975000_V = 975000,
	BATTERY_VOLT_01_000000_V = 1000000,
	BATTERY_VOLT_01_025000_V = 1025000,
	BATTERY_VOLT_01_050000_V = 1050000,
	BATTERY_VOLT_01_075000_V = 1075000,
	BATTERY_VOLT_01_100000_V = 1100000,
	BATTERY_VOLT_01_125000_V = 1125000,
	BATTERY_VOLT_01_150000_V = 1150000,
	BATTERY_VOLT_01_175000_V = 1175000,
	BATTERY_VOLT_01_200000_V = 1200000,
	BATTERY_VOLT_01_225000_V = 1225000,
	BATTERY_VOLT_01_250000_V = 1250000,
	BATTERY_VOLT_01_275000_V = 1275000,
	BATTERY_VOLT_01_300000_V = 1300000,
	BATTERY_VOLT_01_325000_V = 1325000,
	BATTERY_VOLT_01_350000_V = 1350000,
	BATTERY_VOLT_01_375000_V = 1375000,
	BATTERY_VOLT_01_400000_V = 1400000,
	BATTERY_VOLT_01_425000_V = 1425000,
	BATTERY_VOLT_01_450000_V = 1450000,
	BATTERY_VOLT_01_475000_V = 1475000,
	BATTERY_VOLT_01_500000_V = 1500000,
	BATTERY_VOLT_01_520000_V = 1520000,
	BATTERY_VOLT_01_525000_V = 1525000,
	BATTERY_VOLT_01_540000_V = 1540000,
	BATTERY_VOLT_01_550000_V = 1550000,
	BATTERY_VOLT_01_560000_V = 1560000,
	BATTERY_VOLT_01_575000_V = 1575000,
	BATTERY_VOLT_01_580000_V = 1580000,
	BATTERY_VOLT_01_600000_V = 1600000,
	BATTERY_VOLT_01_620000_V = 1620000,
	BATTERY_VOLT_01_625000_V = 1625000,
	BATTERY_VOLT_01_640000_V = 1640000,
	BATTERY_VOLT_01_650000_V = 1650000,
	BATTERY_VOLT_01_660000_V = 1660000,
	BATTERY_VOLT_01_675000_V = 1675000,
	BATTERY_VOLT_01_680000_V = 1680000,
	BATTERY_VOLT_01_700000_V = 1700000,
	BATTERY_VOLT_01_720000_V = 1720000,
	BATTERY_VOLT_01_725000_V = 1725000,
	BATTERY_VOLT_01_740000_V = 1740000,
	BATTERY_VOLT_01_750000_V = 1750000,
	BATTERY_VOLT_01_760000_V = 1760000,
	BATTERY_VOLT_01_775000_V = 1775000,
	BATTERY_VOLT_01_780000_V = 1780000,
	BATTERY_VOLT_01_800000_V = 1800000,
	BATTERY_VOLT_01_820000_V = 1820000,
	BATTERY_VOLT_01_825000_V = 1825000,
	BATTERY_VOLT_01_840000_V = 1840000,
	BATTERY_VOLT_01_850000_V = 1850000,
	BATTERY_VOLT_01_860000_V = 1860000,
	BATTERY_VOLT_01_875000_V = 1875000,
	BATTERY_VOLT_01_880000_V = 1880000,
	BATTERY_VOLT_01_900000_V = 1900000,
	BATTERY_VOLT_01_920000_V = 1920000,
	BATTERY_VOLT_01_925000_V = 1925000,
	BATTERY_VOLT_01_940000_V = 1940000,
	BATTERY_VOLT_01_950000_V = 1950000,
	BATTERY_VOLT_01_960000_V = 1960000,
	BATTERY_VOLT_01_975000_V = 1975000,
	BATTERY_VOLT_01_980000_V = 1980000,
	BATTERY_VOLT_02_000000_V = 2000000,
	BATTERY_VOLT_02_020000_V = 2020000,
	BATTERY_VOLT_02_040000_V = 2040000,
	BATTERY_VOLT_02_050000_V = 2050000,
	BATTERY_VOLT_02_060000_V = 2060000,
	BATTERY_VOLT_02_080000_V = 2080000,
	BATTERY_VOLT_02_100000_V = 2100000,
	BATTERY_VOLT_02_120000_V = 2120000,
	BATTERY_VOLT_02_125000_V = 2125000,
	BATTERY_VOLT_02_140000_V = 2140000,
	BATTERY_VOLT_02_200000_V = 2200000,
	BATTERY_VOLT_02_275000_V = 2275000,
	BATTERY_VOLT_02_300000_V = 2300000,
	BATTERY_VOLT_02_350000_V = 2350000,
	BATTERY_VOLT_02_425000_V = 2425000,
	BATTERY_VOLT_02_400000_V = 2400000,
	BATTERY_VOLT_02_500000_V = 2500000,
	BATTERY_VOLT_02_575000_V = 2575000,
	BATTERY_VOLT_02_600000_V = 2600000,
	BATTERY_VOLT_02_650000_V = 2650000,
	BATTERY_VOLT_02_700000_V = 2700000,
	BATTERY_VOLT_02_725000_V = 2725000,
	BATTERY_VOLT_02_750000_V = 2750000,
	BATTERY_VOLT_02_800000_V = 2800000,
	BATTERY_VOLT_02_850000_V = 2850000,
	BATTERY_VOLT_02_875000_V = 2875000,
	BATTERY_VOLT_02_900000_V = 2900000,
	BATTERY_VOLT_02_950000_V = 2950000,
	BATTERY_VOLT_03_000000_V = 3000000,
	BATTERY_VOLT_03_025000_V = 3025000,
	BATTERY_VOLT_03_100000_V = 3100000,
	BATTERY_VOLT_03_175000_V = 3175000,
	BATTERY_VOLT_03_200000_V = 3200000,
	BATTERY_VOLT_03_250000_V = 3250000,
	BATTERY_VOLT_03_275000_V = 3275000,
	BATTERY_VOLT_03_300000_V = 3300000,
	BATTERY_VOLT_03_325000_V = 3325000,
	BATTERY_VOLT_03_350000_V = 3350000,
	BATTERY_VOLT_03_400000_V = 3400000,
	BATTERY_VOLT_03_450000_V = 3450000,
	BATTERY_VOLT_03_500000_V = 3500000,
	BATTERY_VOLT_03_520000_V = 3520000,
	BATTERY_VOLT_03_540000_V = 3540000,
	BATTERY_VOLT_03_550000_V = 3550000,
	BATTERY_VOLT_03_560000_V = 3560000,
	BATTERY_VOLT_03_580000_V = 3580000,
	BATTERY_VOLT_03_600000_V = 3600000,
	BATTERY_VOLT_03_620000_V = 3620000,
	BATTERY_VOLT_03_640000_V = 3640000,
	BATTERY_VOLT_03_650000_V = 3650000,
	BATTERY_VOLT_03_660000_V = 3660000,
	BATTERY_VOLT_03_680000_V = 3680000,
	BATTERY_VOLT_03_700000_V = 3700000,
	BATTERY_VOLT_03_720000_V = 3720000,
	BATTERY_VOLT_03_740000_V = 3740000,
	BATTERY_VOLT_03_750000_V = 3750000,
	BATTERY_VOLT_03_760000_V = 3760000,
	BATTERY_VOLT_03_762500_V = 3762500,
	BATTERY_VOLT_03_775000_V = 3775000,
	BATTERY_VOLT_03_780000_V = 3780000,
	BATTERY_VOLT_03_787500_V = 3787500,
	BATTERY_VOLT_03_800000_V = 3800000,
	BATTERY_VOLT_03_812500_V = 3812500,
	BATTERY_VOLT_03_820000_V = 3820000,
	BATTERY_VOLT_03_825000_V = 3825000,
	BATTERY_VOLT_03_837500_V = 3837500,
	BATTERY_VOLT_03_840000_V = 3840000,
	BATTERY_VOLT_03_850000_V = 3850000,
	BATTERY_VOLT_03_860000_V = 3860000,
	BATTERY_VOLT_03_862500_V = 3862500,
	BATTERY_VOLT_03_875000_V = 3875000,
	BATTERY_VOLT_03_880000_V = 3880000,
	BATTERY_VOLT_03_887500_V = 3887500,
	BATTERY_VOLT_03_900000_V = 3900000,
	BATTERY_VOLT_03_912500_V = 3912500,
	BATTERY_VOLT_03_920000_V = 3920000,
	BATTERY_VOLT_03_925000_V = 3925000,
	BATTERY_VOLT_03_937500_V = 3937500,
	BATTERY_VOLT_03_940000_V = 3940000,
	BATTERY_VOLT_03_950000_V = 3950000,
	BATTERY_VOLT_03_960000_V = 3960000,
	BATTERY_VOLT_03_975000_V = 3975000,
	BATTERY_VOLT_03_980000_V = 3980000,
	BATTERY_VOLT_03_987500_V = 3987500,
	BATTERY_VOLT_04_000000_V = 4000000,
	BATTERY_VOLT_04_012500_V = 4012500,
	BATTERY_VOLT_04_020000_V = 4020000,
	BATTERY_VOLT_04_025000_V = 4025000,
	BATTERY_VOLT_04_037500_V = 4037500,
	BATTERY_VOLT_04_040000_V = 4040000,
	BATTERY_VOLT_04_050000_V = 4050000,
	BATTERY_VOLT_04_060000_V = 4060000,
	BATTERY_VOLT_04_062500_V = 4062500,
	BATTERY_VOLT_04_067500_V = 4067500,
	BATTERY_VOLT_04_075000_V = 4075000,
	BATTERY_VOLT_04_080000_V = 4080000,
	BATTERY_VOLT_04_087500_V = 4087500,
	BATTERY_VOLT_04_100000_V = 4100000,
	BATTERY_VOLT_04_112500_V = 4112500,
	BATTERY_VOLT_04_115000_V = 4115000,
	BATTERY_VOLT_04_116000_V = 4116000,
	BATTERY_VOLT_04_120000_V = 4120000,
	BATTERY_VOLT_04_125000_V = 4125000,
	BATTERY_VOLT_04_137500_V = 4137500,
	BATTERY_VOLT_04_140000_V = 4140000,
	BATTERY_VOLT_04_150000_V = 4150000,
	BATTERY_VOLT_04_160000_V = 4160000,
	BATTERY_VOLT_04_162500_V = 4162500,
	BATTERY_VOLT_04_175000_V = 4175000,
	BATTERY_VOLT_04_180000_V = 4180000,
	BATTERY_VOLT_04_187500_V = 4187500,
	BATTERY_VOLT_04_200000_V = 4200000,
	BATTERY_VOLT_04_212500_V = 4212500,
	BATTERY_VOLT_04_220000_V = 4220000,
	BATTERY_VOLT_04_225000_V = 4225000,
	BATTERY_VOLT_04_237500_V = 4237500,
	BATTERY_VOLT_04_240000_V = 4240000,
	BATTERY_VOLT_04_250000_V = 4250000,
	BATTERY_VOLT_04_260000_V = 4260000,
	BATTERY_VOLT_04_262500_V = 4262500,
	BATTERY_VOLT_04_275000_V = 4275000,
	BATTERY_VOLT_04_280000_V = 4280000,
	BATTERY_VOLT_04_287500_V = 4287500,
	BATTERY_VOLT_04_300000_V = 4300000,
	BATTERY_VOLT_04_312500_V = 4312500,
	BATTERY_VOLT_04_320000_V = 4320000,
	BATTERY_VOLT_04_325000_V = 4325000,
	BATTERY_VOLT_04_337500_V = 4337500,
	BATTERY_VOLT_04_340000_V = 4340000,
	BATTERY_VOLT_04_350000_V = 4350000,
	BATTERY_VOLT_04_360000_V = 4360000,
	BATTERY_VOLT_04_362500_V = 4362500,
	BATTERY_VOLT_04_375000_V = 4375000,
	BATTERY_VOLT_04_380000_V = 4380000,
	BATTERY_VOLT_04_387500_V = 4387500,
	BATTERY_VOLT_04_400000_V = 4400000,
	BATTERY_VOLT_04_411500_V = 4411500,
	BATTERY_VOLT_04_412500_V = 4412500,
	BATTERY_VOLT_04_420000_V = 4420000,
	BATTERY_VOLT_04_425000_V = 4425000,
	BATTERY_VOLT_04_437500_V = 4437500,
	BATTERY_VOLT_04_440000_V = 4440000,
	BATTERY_VOLT_04_450000_V = 4450000,
	BATTERY_VOLT_04_460000_V = 4460000,
	BATTERY_VOLT_04_462500_V = 4462500,
	BATTERY_VOLT_04_475000_V = 4475000,
	BATTERY_VOLT_04_480000_V = 4480000,
	BATTERY_VOLT_04_487500_V = 4487500,
	BATTERY_VOLT_04_500000_V = 4500000,
	BATTERY_VOLT_04_512500_V = 4512500,
	BATTERY_VOLT_04_520000_V = 4520000,
	BATTERY_VOLT_04_525000_V = 4525000,
	BATTERY_VOLT_04_537500_V = 4537500,
	BATTERY_VOLT_04_540000_V = 4540000,
	BATTERY_VOLT_04_550000_V = 4550000,
	BATTERY_VOLT_04_560000_V = 4560000,
	BATTERY_VOLT_04_580000_V = 4580000,
	BATTERY_VOLT_04_600000_V = 4600000,
	BATTERY_VOLT_04_620000_V = 4620000,
	BATTERY_VOLT_04_640000_V = 4640000,
	BATTERY_VOLT_04_660000_V = 4660000,
	BATTERY_VOLT_04_680000_V = 4680000,
	BATTERY_VOLT_04_700000_V = 4700000,
	BATTERY_VOLT_04_720000_V = 4720000,
	BATTERY_VOLT_04_740000_V = 4740000,
	BATTERY_VOLT_04_760000_V = 4760000,
	BATTERY_VOLT_04_780000_V = 4780000,
	BATTERY_VOLT_04_800000_V = 4800000,
	BATTERY_VOLT_04_850000_V = 4850000,
	BATTERY_VOLT_04_950000_V = 4950000,
	BATTERY_VOLT_05_000000_V = 5000000,
	BATTERY_VOLT_05_150000_V = 5150000,
	BATTERY_VOLT_05_250000_V = 5250000,
	BATTERY_VOLT_05_300000_V = 5300000,
	BATTERY_VOLT_05_450000_V = 5450000,
	BATTERY_VOLT_06_000000_V = 6000000,
	BATTERY_VOLT_06_500000_V = 6500000,
	BATTERY_VOLT_06_750000_V = 6750000,
	BATTERY_VOLT_07_000000_V = 7000000,
	BATTERY_VOLT_07_250000_V = 7250000,
	BATTERY_VOLT_07_500000_V = 7500000,
	BATTERY_VOLT_08_000000_V = 8000000,
	BATTERY_VOLT_08_500000_V = 8500000,
	BATTERY_VOLT_09_000000_V = 9000000,
	BATTERY_VOLT_09_500000_V = 9500000,
	BATTERY_VOLT_10_000000_V = 10000000,
	BATTERY_VOLT_10_500000_V = 10500000,
	BATTERY_VOLT_MAX,
	BATTERY_VOLT_INVALID
} BATTERY_VOLTAGE_ENUM;


/* Enum of charger current List */
typedef enum {
	CHARGE_CURRENT_0_00_MA = 0,
	CHARGE_CURRENT_50_00_MA = 50000,
	CHARGE_CURRENT_62_50_MA = 62500,
	CHARGE_CURRENT_70_00_MA = 70000,
	CHARGE_CURRENT_75_00_MA = 75000,
	CHARGE_CURRENT_87_50_MA = 87500,
	CHARGE_CURRENT_99_00_MA = 99000,
	CHARGE_CURRENT_100_00_MA = 100000,
	CHARGE_CURRENT_125_00_MA = 125000,
	CHARGE_CURRENT_150_00_MA = 150000,
	CHARGE_CURRENT_200_00_MA = 200000,
	CHARGE_CURRENT_225_00_MA = 225000,
	CHARGE_CURRENT_250_00_MA = 250000,
	CHARGE_CURRENT_300_00_MA = 300000,
	CHARGE_CURRENT_350_00_MA = 350000,
	CHARGE_CURRENT_375_00_MA = 375000,
	CHARGE_CURRENT_400_00_MA = 400000,
	CHARGE_CURRENT_425_00_MA = 425000,
	CHARGE_CURRENT_450_00_MA = 450000,
	CHARGE_CURRENT_500_00_MA = 500000,
	CHARGE_CURRENT_525_00_MA = 525000,
	CHARGE_CURRENT_550_00_MA = 550000,
	CHARGE_CURRENT_600_00_MA = 600000,
	CHARGE_CURRENT_625_00_MA = 625000,
	CHARGE_CURRENT_650_00_MA = 650000,
	CHARGE_CURRENT_675_00_MA = 675000,
	CHARGE_CURRENT_700_00_MA = 700000,
	CHARGE_CURRENT_750_00_MA = 750000,
	CHARGE_CURRENT_775_00_MA = 775000,
	CHARGE_CURRENT_800_00_MA = 800000,
	CHARGE_CURRENT_825_00_MA = 825000,
	CHARGE_CURRENT_850_00_MA = 850000,
	CHARGE_CURRENT_900_00_MA = 900000,
	CHARGE_CURRENT_925_00_MA = 925000,
	CHARGE_CURRENT_950_00_MA = 950000,
	CHARGE_CURRENT_975_00_MA = 975000,
	CHARGE_CURRENT_1000_00_MA = 1000000,
	CHARGE_CURRENT_1050_00_MA = 1050000,
	CHARGE_CURRENT_1075_00_MA = 1075000,
	CHARGE_CURRENT_1100_00_MA = 1100000,
	CHARGE_CURRENT_1125_00_MA = 1125000,
	CHARGE_CURRENT_1150_00_MA = 1150000,
	CHARGE_CURRENT_1200_00_MA = 1200000,
	CHARGE_CURRENT_1225_00_MA = 1225000,
	CHARGE_CURRENT_1250_00_MA = 1250000,
	CHARGE_CURRENT_1275_00_MA = 1275000,
	CHARGE_CURRENT_1300_00_MA = 1300000,
	CHARGE_CURRENT_1350_00_MA = 1350000,
	CHARGE_CURRENT_1375_00_MA = 1375000,
	CHARGE_CURRENT_1400_00_MA = 1400000,
	CHARGE_CURRENT_1425_00_MA = 1425000,
	CHARGE_CURRENT_1450_00_MA = 1450000,
	CHARGE_CURRENT_1500_00_MA = 1500000,
	CHARGE_CURRENT_1525_00_MA = 1525000,
	CHARGE_CURRENT_1550_00_MA = 1550000,
	CHARGE_CURRENT_1575_00_MA = 1575000,
	CHARGE_CURRENT_1600_00_MA = 1600000,
	CHARGE_CURRENT_1650_00_MA = 1650000,
	CHARGE_CURRENT_1675_00_MA = 1675000,
	CHARGE_CURRENT_1700_00_MA = 1700000,
	CHARGE_CURRENT_1725_00_MA = 1725000,
	CHARGE_CURRENT_1750_00_MA = 1750000,
	CHARGE_CURRENT_1800_00_MA = 1800000,
	CHARGE_CURRENT_1825_00_MA = 1825000,
	CHARGE_CURRENT_1850_00_MA = 1850000,
	CHARGE_CURRENT_1875_00_MA = 1875000,
	CHARGE_CURRENT_1900_00_MA = 1900000,
	CHARGE_CURRENT_1950_00_MA = 1950000,
	CHARGE_CURRENT_1975_00_MA = 1975000,
	CHARGE_CURRENT_2000_00_MA = 2000000,
	CHARGE_CURRENT_2025_00_MA = 2025000,
	CHARGE_CURRENT_2050_00_MA = 2050000,
	CHARGE_CURRENT_2100_00_MA = 2100000,
	CHARGE_CURRENT_2125_00_MA = 2125000,
	CHARGE_CURRENT_2150_00_MA = 2150000,
	CHARGE_CURRENT_2175_00_MA = 2175000,
	CHARGE_CURRENT_2200_00_MA = 2200000,
	CHARGE_CURRENT_2250_00_MA = 2250000,
	CHARGE_CURRENT_2275_00_MA = 2275000,
	CHARGE_CURRENT_2300_00_MA = 2300000,
	CHARGE_CURRENT_2325_00_MA = 2325000,
	CHARGE_CURRENT_2350_00_MA = 2350000,
	CHARGE_CURRENT_2400_00_MA = 2400000,
	CHARGE_CURRENT_2425_00_MA = 2425000,
	CHARGE_CURRENT_2450_00_MA = 2450000,
	CHARGE_CURRENT_2500_00_MA = 2500000,
	CHARGE_CURRENT_2550_00_MA = 2550000,
	CHARGE_CURRENT_2575_00_MA = 2575000,
	CHARGE_CURRENT_2600_00_MA = 2600000,
	CHARGE_CURRENT_2650_00_MA = 2650000,
	CHARGE_CURRENT_2700_00_MA = 2700000,
	CHARGE_CURRENT_2725_00_MA = 2725000,
	CHARGE_CURRENT_2750_00_MA = 2750000,
	CHARGE_CURRENT_2800_00_MA = 2800000,
	CHARGE_CURRENT_2850_00_MA = 2850000,
	CHARGE_CURRENT_2875_00_MA = 2875000,
	CHARGE_CURRENT_2900_00_MA = 2900000,
	CHARGE_CURRENT_2950_00_MA = 2950000,
	CHARGE_CURRENT_3000_00_MA = 3000000,
	CHARGE_CURRENT_3050_00_MA = 3050000,
	CHARGE_CURRENT_3100_00_MA = 3100000,
	CHARGE_CURRENT_3200_00_MA = 3200000,
	CHARGE_CURRENT_MAX
} CHR_CURRENT_ENUM;

/* Enum of charger current List */
typedef enum {
	CHR_VOLT_00_500000_V = 500,
	CHR_VOLT_01_000000_V = 1000,
	CHR_VOLT_04_000000_V = 4000,
	CHR_VOLT_04_500000_V = 4500,
	CHR_VOLT_05_000000_V = 5000,
	CHR_VOLT_05_500000_V = 5500,
	CHR_VOLT_06_000000_V = 6050,
	CHR_VOLT_06_500000_V = 6500,
	CHR_VOLT_07_000000_V = 7000,
	CHR_VOLT_07_500000_V = 7500,
	CHR_VOLT_08_000000_V = 8000,
	CHR_VOLT_08_500000_V = 8500,
	CHR_VOLT_09_000000_V = 9000,
	CHR_VOLT_09_500000_V = 9500,
	CHR_VOLT_10_000000_V = 10000,
	CHR_VOLT_10_500000_V = 10500,
	CHR_VOLT_11_000000_V = 11000,
	CHR_VOLT_11_500000_V = 11500,
	CHR_VOLT_12_000000_V = 12000,
	CHR_VOLT_12_500000_V = 12500,
	CHR_VOLT_13_000000_V = 13000,
	CHR_VOLT_13_500000_V = 13500,
	CHR_VOLT_14_000000_V = 14000,
	CHR_VOLT_14_500000_V = 14500,
	CHR_VOLT_15_000000_V = 15000,
	CHR_VOLT_15_500000_V = 15500,
	CHR_VOLT_16_000000_V = 16000,
	CHR_VOLT_16_500000_V = 16500,
	CHR_VOLT_17_000000_V = 17000,
	CHR_VOLT_17_500000_V = 17500,
	CHR_VOLT_18_000000_V = 18000,
	CHR_VOLT_18_500000_V = 18500,
	CHR_VOLT_19_000000_V = 19000,
	CHR_VOLT_19_500000_V = 19500,
	CHR_VOLT_20_000000_V = 20000,
	CHR_VOLT_MAX,

} CHR_VOLTAGE_ENUM;
const unsigned int VBAT_CV_VTH[]=
{
	BATTERY_VOLT_03_500000_V,   BATTERY_VOLT_03_520000_V,	BATTERY_VOLT_03_540000_V,   BATTERY_VOLT_03_560000_V,
	BATTERY_VOLT_03_580000_V,   BATTERY_VOLT_03_600000_V,	BATTERY_VOLT_03_620000_V,   BATTERY_VOLT_03_640000_V,
	BATTERY_VOLT_03_660000_V,	BATTERY_VOLT_03_680000_V,	BATTERY_VOLT_03_700000_V,	BATTERY_VOLT_03_720000_V,
	BATTERY_VOLT_03_740000_V,	BATTERY_VOLT_03_760000_V,	BATTERY_VOLT_03_780000_V,	BATTERY_VOLT_03_800000_V,
	BATTERY_VOLT_03_820000_V,	BATTERY_VOLT_03_840000_V,	BATTERY_VOLT_03_860000_V,	BATTERY_VOLT_03_880000_V,
	BATTERY_VOLT_03_900000_V,	BATTERY_VOLT_03_920000_V,	BATTERY_VOLT_03_940000_V,	BATTERY_VOLT_03_960000_V,
	BATTERY_VOLT_03_980000_V,	BATTERY_VOLT_04_000000_V,	BATTERY_VOLT_04_020000_V,	BATTERY_VOLT_04_040000_V,
	BATTERY_VOLT_04_060000_V,	BATTERY_VOLT_04_080000_V,	BATTERY_VOLT_04_100000_V,	BATTERY_VOLT_04_120000_V,
	BATTERY_VOLT_04_140000_V,   BATTERY_VOLT_04_160000_V,	BATTERY_VOLT_04_180000_V,   BATTERY_VOLT_04_200000_V,
	BATTERY_VOLT_04_220000_V,   BATTERY_VOLT_04_240000_V,	BATTERY_VOLT_04_260000_V,   BATTERY_VOLT_04_280000_V,
	BATTERY_VOLT_04_300000_V,   BATTERY_VOLT_04_320000_V,	BATTERY_VOLT_04_340000_V,   BATTERY_VOLT_04_360000_V,	
	BATTERY_VOLT_04_380000_V,   BATTERY_VOLT_04_400000_V,	BATTERY_VOLT_04_420000_V,   BATTERY_VOLT_04_440000_V	
	
};

const unsigned int CS_VTH[]=
{
	CHARGE_CURRENT_550_00_MA,   CHARGE_CURRENT_650_00_MA,	CHARGE_CURRENT_750_00_MA, CHARGE_CURRENT_850_00_MA,
	CHARGE_CURRENT_950_00_MA,   CHARGE_CURRENT_1050_00_MA,	CHARGE_CURRENT_1150_00_MA, CHARGE_CURRENT_1250_00_MA,
	CHARGE_CURRENT_1350_00_MA,	CHARGE_CURRENT_1450_00_MA,	CHARGE_CURRENT_1550_00_MA,	CHARGE_CURRENT_1650_00_MA,
	CHARGE_CURRENT_1750_00_MA,	CHARGE_CURRENT_1850_00_MA,	CHARGE_CURRENT_1950_00_MA,	CHARGE_CURRENT_2050_00_MA,
	CHARGE_CURRENT_2150_00_MA,	CHARGE_CURRENT_2250_00_MA,	CHARGE_CURRENT_2350_00_MA,	CHARGE_CURRENT_2450_00_MA,
	CHARGE_CURRENT_2550_00_MA,	CHARGE_CURRENT_2650_00_MA,	CHARGE_CURRENT_2750_00_MA,	CHARGE_CURRENT_2850_00_MA,
	CHARGE_CURRENT_2950_00_MA,	CHARGE_CURRENT_3050_00_MA
}; 

 const unsigned int INPUT_CS_VTH[]=
 {
	 CHARGE_CURRENT_100_00_MA,	 CHARGE_CURRENT_500_00_MA,	 CHARGE_CURRENT_800_00_MA, CHARGE_CURRENT_MAX
 }; 

 const unsigned int VCDT_HV_VTH[]=
 {
	  BATTERY_VOLT_04_200000_V, BATTERY_VOLT_04_250000_V,	  BATTERY_VOLT_04_300000_V,   BATTERY_VOLT_04_350000_V,
	  BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_450000_V,	  BATTERY_VOLT_04_500000_V,   BATTERY_VOLT_04_550000_V,
	  BATTERY_VOLT_04_600000_V, BATTERY_VOLT_06_000000_V,	  BATTERY_VOLT_06_500000_V,   BATTERY_VOLT_07_000000_V,
	  BATTERY_VOLT_07_500000_V, BATTERY_VOLT_08_500000_V,	  BATTERY_VOLT_09_500000_V,   BATTERY_VOLT_10_500000_V		  
 };

#ifdef CONFIG_OF
#else

#define eta6937_SLAVE_ADDR_WRITE   0xD4
#define eta6937_SLAVE_ADDR_Read    0xD5

#ifdef I2C_SWITHING_CHARGER_CHANNEL
#define eta6937_BUSNUM I2C_SWITHING_CHARGER_CHANNEL
#else
#define eta6937_BUSNUM 0
#endif

#endif


static struct i2c_client *new_client=NULL;
static const struct i2c_device_id eta6937_i2c_id[] = {{"switching_charger",0},{}};   
bool chargin_hw_init_done = false; 
static int eta6937_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);

struct eta6937_info {
	struct charger_device *chg_dev;
	struct power_supply *psy;
	struct charger_properties chg_props;
	struct device *dev;
	const char *chg_dev_name;
	const char *eint_name;
	CHARGER_TYPE chg_type;
	int irq;
};

/**********************************************************
  *
  *   [Global Variable] 
  *
  *********************************************************/
unsigned char eta6937_reg[eta6937_REG_NUM] = {0};

int g_eta6937_hw_exist;

static DEFINE_MUTEX(eta6937_i2c_access);
static DEFINE_MUTEX(eta6937_access_lock);

#define pr_info_eta6937(fmt,arg...) printk("<<proximity>> %s "fmt"\n",__func__,##arg)
#define GETARRAYNUM(array) (sizeof(array)/sizeof(array[0]))

unsigned int charging_value_to_parameter(const unsigned int *parameter, const unsigned int array_size, const unsigned int val)
{
	if (val < array_size)
	{
		return parameter[val];
	}
	else
	{
		pr_info_eta6937( "Can't find the parameter \r\n");	
		return parameter[0];
	}
}

 
 unsigned int charging_parameter_to_value(const unsigned int *parameter, const unsigned int array_size, const unsigned int val)
{
	unsigned int i;
	
	for(i=0;i<array_size;i++)
	{
		if (val == *(parameter + i))
		{
				return i;
		}
	}

    pr_info_eta6937( "NO register value match \r\n");
	//TODO: ASSERT(0);	// not find the value
	return 0;
}


 static unsigned int bmt_find_closest_level(const unsigned int *pList,unsigned int number,unsigned int level)
 {
	 unsigned int i;
	 unsigned int max_value_in_last_element;
 
	 if(pList[0] < pList[1])
		 max_value_in_last_element = true;
	 else
		 max_value_in_last_element = false;
 
	 if(max_value_in_last_element == true)
	 {
		 for(i = (number-1); i != 0; i--)	 //max value in the last element
		 {
			 if(pList[i] <= level)
			 {
				 return pList[i];
			 }	  
		 }

 		 pr_info_eta6937( "Can't find closest level, small value first \r\n");
		 return pList[0];
		 //return CHARGE_CURRENT_0_00_MA;
	 }
	 else
	 {
		 for(i = 0; i< number; i++)  // max value in the first element
		 {
			 if(pList[i] <= level)
			 {
				 return pList[i];
			 }	  
		 }

		 pr_info_eta6937( "Can't find closest level, large value first \r\n"); 	 
		 return pList[number -1];
  		 //return CHARGE_CURRENT_0_00_MA;
	 }
}
/**********************************************************
  *
  *   [I2C Function For Read/Write eta6937] 
  *
  *********************************************************/
#ifdef CONFIG_MTK_I2C_EXTENSION
int eta6937_read_byte(unsigned char cmd, unsigned char *returnData)
{
    char     cmd_buf[1]={0x00};
    char     readData = 0;
    int      ret=0;

    mutex_lock(&eta6937_i2c_access);
    cmd_buf[0] = cmd;
    ret = i2c_master_send(new_client, &cmd_buf[0], (1<<8 | 1));
    if (ret < 0) 
    {    
        new_client->ext_flag=0;

        mutex_unlock(&eta6937_i2c_access);
        return 0;
    }
    readData = cmd_buf[0];
    *returnData = readData;
 
    new_client->ext_flag=0;
    
    mutex_unlock(&eta6937_i2c_access);    
    return 1;
}

int eta6937_write_byte(unsigned char cmd, unsigned char writeData)
{
    char    write_data[2] = {0};
    int     ret=0;
    mutex_lock(&eta6937_i2c_access);
    
    write_data[0] = cmd;
    write_data[1] = writeData;
    new_client->ext_flag=((new_client->ext_flag ) & I2C_MASK_FLAG ) | I2C_DIRECTION_FLAG;
    
    ret = i2c_master_send(new_client, write_data, 2);
    if (ret < 0) 
    {
       
        new_client->ext_flag=0;
        mutex_unlock(&eta6937_i2c_access);
        return 0;
    }
    
	new_client->ext_flag=0;
	mutex_unlock(&eta6937_i2c_access);
    return 1;
}
#else
unsigned int eta6937_read_byte(unsigned char cmd, unsigned char *returnData)
{
	unsigned char xfers = 2;
	int ret, retries = 1;

	mutex_lock(&eta6937_i2c_access);

	do {
		struct i2c_msg msgs[2] = {
			{
				.addr = new_client->addr,
				.flags = 0,
				.len = 1,
				.buf = &cmd,
			},
			{

				.addr = new_client->addr,
				.flags = I2C_M_RD,
				.len = 1,
				.buf = returnData,
			}
		};

		/*
		 * Avoid sending the segment addr to not upset non-compliant
		 * DDC monitors.
		 */
		ret = i2c_transfer(new_client->adapter, msgs, xfers);

		if (ret == -ENXIO) {
			pr_info("skipping non-existent adapter %s\n", new_client->adapter->name);
			break;
		}
	} while (ret != xfers && --retries);

	mutex_unlock(&eta6937_i2c_access);

	return ret == xfers ? 1 : -1;
}

unsigned int eta6937_write_byte(unsigned char cmd, unsigned char writeData)
{
	unsigned char xfers = 1;
	int ret, retries = 1;
	unsigned char buf[8];

	mutex_lock(&eta6937_i2c_access);

	buf[0] = cmd;
	memcpy(&buf[1], &writeData, 1);

	do {
		struct i2c_msg msgs[1] = {
			{
				.addr = new_client->addr,
				.flags = 0,
				.len = 1 + 1,
				.buf = buf,
			},
		};

		/*
		 * Avoid sending the segment addr to not upset non-compliant
		 * DDC monitors.
		 */
		ret = i2c_transfer(new_client->adapter, msgs, xfers);

		if (ret == -ENXIO) {
			pr_info("skipping non-existent adapter %s\n", new_client->adapter->name);
			break;
		}
	} while (ret != xfers && --retries);

	mutex_unlock(&eta6937_i2c_access);

	return ret == xfers ? 1 : -1;
}
#endif
/**********************************************************
  *
  *   [Read / Write Function] 
  *
  *********************************************************/
unsigned int eta6937_read_interface (unsigned char RegNum, unsigned char *val, unsigned char MASK, unsigned char SHIFT)
{
    unsigned char eta6937_reg = 0;
    int ret = 0;

    ret = eta6937_read_byte(RegNum, &eta6937_reg);
	
    eta6937_reg &= (MASK << SHIFT);
    *val = (eta6937_reg >> SHIFT);
	
	return ret;
}

unsigned int eta6937_config_interface (unsigned char RegNum, unsigned char val, unsigned char MASK, unsigned char SHIFT)
{
    unsigned char eta6937_reg = 0;
    int ret = 0;

    ret = eta6937_read_byte(RegNum, &eta6937_reg);
    
    eta6937_reg &= ~(MASK << SHIFT);
    eta6937_reg |= (val << SHIFT);

	if(RegNum == eta6937_CON4 && val == 1 && MASK ==CON4_RESET_MASK && SHIFT == CON4_RESET_SHIFT)
	{
		// RESET bit
	}
	else if(RegNum == eta6937_CON4)
	{
		eta6937_reg &= ~0x80;	//RESET bit read returs 1, so clear it
	}
	 

    ret = eta6937_write_byte(RegNum, eta6937_reg);

    return ret;
}

//write one register directly
unsigned int eta6937_config_interface_liao (unsigned char RegNum, unsigned char val)
{   
    int ret = 0;
    
    ret = eta6937_write_byte(RegNum, val);

    return ret;
}

/**********************************************************
  *
  *   [Internal Function] 
  *
  *********************************************************/
//CON0----------------------------------------------------

void eta6937_set_tmr_rst(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON0), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON0_TMR_RST_MASK),
                                    (unsigned char)(CON0_TMR_RST_SHIFT)
                                    );
}

unsigned int eta6937_get_otg_status(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON0), 
                                    (&val),
                                    (unsigned char)(CON0_OTG_MASK),
                                    (unsigned char)(CON0_OTG_SHIFT)
                                    );
    return val;
}

void eta6937_set_en_stat(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON0), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON0_EN_STAT_MASK),
                                    (unsigned char)(CON0_EN_STAT_SHIFT)
                                    );
}

unsigned int eta6937_get_chip_status(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON0), 
                                    (&val),
                                    (unsigned char)(CON0_STAT_MASK),
                                    (unsigned char)(CON0_STAT_SHIFT)
                                    );
    return val;
}

unsigned int eta6937_get_boost_status(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON0), 
                                    (&val),
                                    (unsigned char)(CON0_BOOST_MASK),
                                    (unsigned char)(CON0_BOOST_SHIFT)
                                    );
    return val;
}

unsigned int eta6937_get_fault_status(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON0), 
                                    (&val),
                                    (unsigned char)(CON0_FAULT_MASK),
                                    (unsigned char)(CON0_FAULT_SHIFT)
                                    );
    return val;
}

//CON1----------------------------------------------------

void eta6937_set_input_charging_current(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_LIN_LIMIT_MASK),
                                    (unsigned char)(CON1_LIN_LIMIT_SHIFT)
                                    );
}

void eta6937_set_v_low(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_LOW_V_MASK),
                                    (unsigned char)(CON1_LOW_V_SHIFT)
                                    );
}

void eta6937_set_te(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_TE_MASK),
                                    (unsigned char)(CON1_TE_SHIFT)
                                    );
}

void eta6937_set_ce(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_CE_MASK),
                                    (unsigned char)(CON1_CE_SHIFT)
                                    );
}

void eta6937_set_hz_mode(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_HZ_MODE_MASK),
                                    (unsigned char)(CON1_HZ_MODE_SHIFT)
                                    );
}

void eta6937_set_opa_mode(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON1), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON1_OPA_MODE_MASK),
                                    (unsigned char)(CON1_OPA_MODE_SHIFT)
                                    );
}

//CON2----------------------------------------------------

void eta6937_set_oreg(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON2), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON2_OREG_MASK),
                                    (unsigned char)(CON2_OREG_SHIFT)
                                    );
}

void eta6937_set_otg_pl(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON2), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON2_OTG_PL_MASK),
                                    (unsigned char)(CON2_OTG_PL_SHIFT)
                                    );
}

void eta6937_set_otg_en(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON2), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON2_OTG_EN_MASK),
                                    (unsigned char)(CON2_OTG_EN_SHIFT)
                                    );
}

//CON3----------------------------------------------------

unsigned int eta6937_get_vender_code(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON3), 
                                    (&val),
                                    (unsigned char)(CON3_VENDER_CODE_MASK),
                                    (unsigned char)(CON3_VENDER_CODE_SHIFT)
                                    );
    return val;
}

unsigned int eta6937_get_pn(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON3), 
                                    (&val),
                                    (unsigned char)(CON3_PIN_MASK),
                                    (unsigned char)(CON3_PIN_SHIFT)
                                    );
    return val;
}

unsigned int eta6937_get_revision(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON3), 
                                    (&val),
                                    (unsigned char)(CON3_REVISION_MASK),
                                    (unsigned char)(CON3_REVISION_SHIFT)
                                    );
    return val;
}

//CON4----------------------------------------------------

void eta6937_set_reset(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON4), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON4_RESET_MASK),
                                    (unsigned char)(CON4_RESET_SHIFT)
                                    );
}

void eta6937_set_iocharge(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON4), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON4_I_CHR_MASK),
                                    (unsigned char)(CON4_I_CHR_SHIFT)
                                    );
}

void eta6937_set_iterm(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON4), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON4_I_TERM_MASK),
                                    (unsigned char)(CON4_I_TERM_SHIFT)
                                    );
}

//CON5----------------------------------------------------

void eta6937_set_high_i_chr(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON5), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON5_I_CHR_MASK),
                                    (unsigned char)(CON5_I_CHR_SHIFT)
                                    );
}

void eta6937_set_io_level(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON5), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON5_IO_LEVEL_MASK),
                                    (unsigned char)(CON5_IO_LEVEL_SHIFT)
                                    );
}

unsigned int eta6937_get_sp_status(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON5), 
                                    (&val),
                                    (unsigned char)(CON5_SP_STATUS_MASK),
                                    (unsigned char)(CON5_SP_STATUS_SHIFT)
                                    );
    return val;
}

unsigned int eta6937_get_en_level(void)
{
    unsigned int ret=0;
    unsigned char val=0;

    ret=eta6937_read_interface(     (unsigned char)(eta6937_CON5), 
                                    (&val),
                                    (unsigned char)(CON5_EN_LEVEL_MASK),
                                    (unsigned char)(CON5_EN_LEVEL_SHIFT)
                                    );
    return val;
}

void eta6937_set_vsp(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON5), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON5_VSP_MASK),
                                    (unsigned char)(CON5_VSP_SHIFT)
                                    );
}

//CON6----------------------------------------------------

void eta6937_set_i_safe(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON6), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON6_ISAFE_MASK),
                                    (unsigned char)(CON6_ISAFE_SHIFT)
                                    );
}

void eta6937_set_v_safe(unsigned int val)
{
    unsigned int ret=0;    

    ret=eta6937_config_interface(   (unsigned char)(eta6937_CON6), 
                                    (unsigned char)(val),
                                    (unsigned char)(CON6_VSAFE_MASK),
                                    (unsigned char)(CON6_VSAFE_SHIFT)
                                    );
}

/**********************************************************
  *
  *   [Internal Function] 
  *
  *********************************************************/
static int eta6937_dump_register(void)
{
    int i=0;
    //pr_info_eta6937("[eta6937] ");
    for (i=0;i<eta6937_REG_NUM;i++)
    {
        eta6937_read_byte(i, &eta6937_reg[i]);
        //pr_info_eta6937("[0x%x]=0x%x ", i, eta6937_reg[i]);        
    }
    //pr_info_eta6937("\n");
	return 0;
}

static void eta6937_hw_component_detect(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = eta6937_read_interface(0x03, &val, 0xFF, 0x0);

	if (val == 0)
		g_eta6937_hw_exist = 0;
	else
		g_eta6937_hw_exist = 1;

	pr_debug("[eta6937_hw_component_detect] exist=%d, Reg[0x03]=0x%x\n",
		 g_eta6937_hw_exist, val);
}
static unsigned int eta6937_charging_hw_init(void)
{
    unsigned int status = 0;
  //  static bool charging_init_flag = false;

    #if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
        eta6937_config_interface_liao(0x06,0xD7); // 0xD7 ISAFE = 3050mA, VSAFE = 4.34V
        eta6937_config_interface_liao(0x02,0xa8);//a8=voltage=4.34v
    #else
        eta6937_config_interface_liao(0x06,0xD0); //0xD0
        eta6937_config_interface_liao(0x02,0x8C); //0x8c// 4.2
    #endif
        
     eta6937_config_interface_liao(0x07,0x00); 
     
    eta6937_set_tmr_rst(1);	//kick chip watch dog
    //TE=1, CE=0, HZ_MODE=0, OPA_MODE=0
    eta6937_set_te(1);
    eta6937_set_ce(0);
    eta6937_set_hz_mode(0);
    eta6937_set_opa_mode(0);

	eta6937_set_iterm(0x02); //150ma
    if ( !chargin_hw_init_done ) {   
        chargin_hw_init_done = true;
    }        
    return status;
}
static int charging_dump_register(struct charger_device *chg_dev)
 {
 	unsigned int status = 0;

	eta6937_dump_register();
   	
	return status;
 }	

static int charging_enable(struct charger_device *chg_dev, bool en)
 {
 	int status = 0;
//	unsigned int enable = en;

	if (en)
	{
		eta6937_set_ce(0);
		eta6937_set_hz_mode(0);
		eta6937_set_opa_mode(0);
	}
	else
	{

#if defined(CONFIG_USB_MTK_HDRC_HCD)
   		if(mt_usb_is_device())
#endif 			
    	{
	        eta6937_set_ce(1);
    	}
	}
	eta6937_config_interface(   (unsigned char)(eta6937_CON5), (unsigned char)(4),(unsigned char)(CON5_VSP_MASK),(unsigned char)(CON5_VSP_SHIFT));  //7
		
	return status;
 }

static int charging_set_cv_voltage(struct charger_device *chg_dev, u32 cv)
 {
 	unsigned int status = 0;
 	unsigned short int array_size;
 	unsigned int set_cv_voltage;
	unsigned short register_value;
	array_size = ARRAY_SIZE(VBAT_CV_VTH);
	set_cv_voltage = bmt_find_closest_level(VBAT_CV_VTH, array_size, cv);
	register_value = charging_parameter_to_value(VBAT_CV_VTH, GETARRAYNUM(VBAT_CV_VTH) ,set_cv_voltage);
	
	eta6937_set_oreg(register_value); 

	return status;
 } 	

static int charging_get_current(struct charger_device *chg_dev, u32 *ichg)
 {
    unsigned int status = 0;
    unsigned int array_size;
    unsigned char reg_value;
	
    //Get current level
    array_size = GETARRAYNUM(CS_VTH);
    eta6937_read_interface(eta6937_CON4, &reg_value, 0x7, 0x4);	//IINLIM
    *(unsigned int *)ichg = charging_value_to_parameter(CS_VTH,array_size,reg_value);
	
    return status;
 }  
  
static int charging_set_current(struct charger_device *chg_dev, u32 current_value)
 {
 	unsigned int status = 0;
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;
	if(current_value <= CHARGE_CURRENT_350_00_MA)
	{
		eta6937_set_io_level(1);
	}
	else
	{
		eta6937_set_io_level(0);
		array_size = GETARRAYNUM(CS_VTH);
		set_chr_current = bmt_find_closest_level(CS_VTH, array_size, current_value);
		register_value = charging_parameter_to_value(CS_VTH, array_size ,set_chr_current);
		eta6937_set_iocharge(register_value&0x07);
		eta6937_set_high_i_chr((register_value>>3)&0x3);
	}
	
	return status;
 } 	

static int charging_set_input_current(struct charger_device *chg_dev, u32 current_value)
 {
    int status = 0;
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;

    if(current_value > CHARGE_CURRENT_500_00_MA)
    {
        register_value = 0x3;
    }
    else
    {
    	array_size = GETARRAYNUM(INPUT_CS_VTH);
    	set_chr_current = bmt_find_closest_level(INPUT_CS_VTH, array_size, current_value);
    	register_value = charging_parameter_to_value(INPUT_CS_VTH, array_size ,set_chr_current);
    	
    }
    
    eta6937_set_input_charging_current(register_value);

	return status;
 } 	
static int charging_get_input_current(struct charger_device *chg_dev, u32 *aicr)
{
    unsigned int status = 0;
    unsigned int array_size;
    unsigned char reg_value;

    //Get current level
    array_size = GETARRAYNUM(CS_VTH);
    eta6937_read_interface(0x1, &reg_value, 0x3, 0x6);	//IINLIM
    *(unsigned int *)aicr = charging_value_to_parameter(CS_VTH,array_size,reg_value);

    return status;
}  
static int charging_get_charging_status(struct charger_device *chg_dev, bool *is_done)
 {
 	unsigned int status = 0;
	unsigned int ret_val;

	ret_val = eta6937_get_chip_status();
	if(ret_val == 0x2)
		*(unsigned int *)is_done = true;
	else
		*(unsigned int *)is_done = false;
		
	return status;
 } 	


static int charging_reset_watch_dog_timer(struct charger_device *chg_dev)
 {
	 unsigned int status = 0;
	 eta6937_set_tmr_rst(1);
	 
	 return status;
 }
 #if 0
  static unsigned int charging_set_hv_threshold(void *data)
  {
	 unsigned int status = 0;
 
	 unsigned int set_hv_voltage;
	 unsigned int array_size;
	 unsigned short register_value;
	 unsigned int voltage = *(unsigned int*)(data);
	 
	 array_size = GETARRAYNUM(VCDT_HV_VTH);
	 set_hv_voltage = bmt_find_closest_level(VCDT_HV_VTH, array_size, voltage);
	 register_value = charging_parameter_to_value(VCDT_HV_VTH, array_size ,set_hv_voltage);
	 //upmu_set_rg_vcdt_hv_vth(register_value);
	 pmic_set_register_value(PMIC_RG_VCDT_HV_VTH, register_value);
	 
 
	 return status;
  }
 
 
  static unsigned int charging_get_hv_status(void *data)
  {
	   unsigned int status = 0;
 
	   //*(kal_bool*)(data) = upmu_get_rgs_vcdt_hv_det();
	   *(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_VCDT_HV_DET);
	   return status;
  }
#endif

static int charging_enable_otg(struct charger_device *chg_dev, bool en)
{
	int ret = 0;
	if (en) {
		eta6937_set_opa_mode(en);
		eta6937_set_otg_pl(1);
		eta6937_set_otg_en(en);
	} else {
		eta6937_set_opa_mode(en);
		eta6937_set_otg_en(en);
	}

	return ret;
}

#if 0
static unsigned int charging_get_charger_type(void *data)
{
    unsigned int status = 0;
     
#if defined(CONFIG_POWER_EXT) || defined(CONFIG_MTK_FPGA)
    *(CHARGER_TYPE*)(data) = STANDARD_HOST;
#else
    *(CHARGER_TYPE*)(data) = hw_charging_get_charger_type();
#endif

     return status;
}


static unsigned int charging_set_platform_reset(void *data)
{
    unsigned int status = 0;

    pr_info_eta6937( "charging_set_platform_reset\n");
 
//    arch_reset(0,NULL);
        
    return status;
}
					
static unsigned int charging_get_platform_boot_mode(void *data)
{
	unsigned int status = 0;

#if defined(CONFIG_POWER_EXT) || defined(CONFIG_MTK_FPGA)
#else
	*(unsigned int *) (data) = get_boot_mode();

	battery_log(BAT_LOG_CRTI, "get_boot_mode=%d\n", get_boot_mode());
#endif

	return status;
}


static unsigned int charging_set_power_off(void *data)
{
    unsigned int status = 0;
  
    battery_log(BAT_LOG_CRTI, "charging_set_power_off\n");
    mt_power_off();
         
    return status;
}

static unsigned int charging_get_power_source(void *data)
{
	unsigned int status = STATUS_UNSUPPORTED;

	return status;
}

static unsigned int charging_get_csdac_full_flag(void *data)
{
	return STATUS_UNSUPPORTED;	
}

static unsigned int charging_set_ta_current_pattern(void *data)
{
	return STATUS_UNSUPPORTED;	
}

static unsigned int charging_set_error_state(void *data)
{
	return STATUS_UNSUPPORTED;	
}
#endif
static int eta6937_parse_dt(struct eta6937_info *info, struct device *dev)
{
	struct device_node *np = dev->of_node;

	pr_info_eta6937("%s\n", __func__);
	if (!np) {
		pr_err("%s: no of node\n", __func__);
		return -ENODEV;
	}
	if (of_property_read_string(np, "charger_name", &info->chg_dev_name) < 0) {
		info->chg_dev_name = "primary_chg";
		pr_err("%s: no charger name\n", __func__);
	}

	if (of_property_read_string(np, "alias_name", &(info->chg_props.alias_name)) < 0) {
		info->chg_props.alias_name = "eta6937";
		pr_err("%s: no alias name\n", __func__);
	}

	if (of_property_read_string(np, "eint_name", &info->eint_name) < 0) {
		info->eint_name = "chr_stat";
		pr_err("%s: no eint name\n", __func__);
	}
	
	return 0;
}
//static struct i2c_board_info __initdata i2c_eta6937 = { I2C_BOARD_INFO("eta6937", (eta6937_SLAVE_ADDR_WRITE>>1))};
static int eta6937_do_event(struct charger_device *chg_dev, u32 event, u32 args)
{
	if (chg_dev == NULL)
		return -EINVAL;

	pr_info("%s: event = %d\n", __func__, event);
	switch (event) {
	case EVENT_EOC:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_EOC);
		break;
	case EVENT_RECHARGE:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_RECHG);
		break;
	default:
		break;
	}

	return 0;
}

static struct charger_ops eta6937_chg_ops = {
#if 0
	.enable_hz = eta6937_enable_hz,
#endif

	/* Normal charging */
	.dump_registers = charging_dump_register,
	.enable = charging_enable,
	.get_charging_current = charging_get_current,
	.set_charging_current = charging_set_current,
	.get_input_current = charging_get_input_current,
	.set_input_current = charging_set_input_current,
	/*.get_constant_voltage = eta6937_get_battery_voreg,*/
	.set_constant_voltage = charging_set_cv_voltage,
	.kick_wdt = charging_reset_watch_dog_timer,
	//.set_mivr = eta6937_set_vindpm_voltage,
	.is_charging_done = charging_get_charging_status,

	/* Safety timer */
	//.enable_safety_timer = eta6937_enable_safetytimer,
	//.is_safety_timer_enabled = eta6937_get_is_safetytimer_enable,

	/* Power path */
	//.enable_powerpath = eta6937_enable_power_path,
	//.is_powerpath_enabled = eta6937_get_is_power_path_enable,

#if 0
	/* Charger type detection */
	.enable_chg_type_det = eta6937_enable_chg_type_det,
#endif

	/* OTG */
	.enable_otg = charging_enable_otg,
//	.set_boost_current_limit = eta6937_set_boost_current_limit,

#if 0
	/* IR compensation */
	.set_ircmp_resistor = eta6937_set_ircmp_resistor,
	.set_ircmp_vclamp = eta6937_set_ircmp_vclamp,
#endif

	/* PE+/PE+20 */
//	.send_ta_current_pattern = eta6937_set_ta_current_pattern,
#if 0
	.set_pe20_efficiency_table = eta6937_set_pep20_efficiency_table,
#endif
	//.send_ta20_current_pattern = eta6937_set_ta20_current_pattern,
	//.set_ta20_reset = eta6937_set_ta20_reset,
	.event = eta6937_do_event,
};
/**********************************************************
  *
  *   [platform_driver API] 
  *
  *********************************************************/
unsigned char g_reg_value_eta6937=0;
static ssize_t show_eta6937_access(struct device *dev,struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%u\n", g_reg_value_eta6937);
}
static ssize_t store_eta6937_access(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
    int ret=0;
    char *pvalue = NULL;
    unsigned int reg_value = 0;
    unsigned int reg_address = 0;
    
    
    if(buf != NULL && size != 0)
    {
        reg_address = simple_strtoul(buf,&pvalue,16);
        
        if(size > 3)
        {        
            reg_value = simple_strtoul((pvalue+1),NULL,16);        
            ret=eta6937_config_interface(reg_address, reg_value, 0xFF, 0x0);
        }
        else
        {    
            ret=eta6937_read_interface(reg_address, &g_reg_value_eta6937, 0xFF, 0x0);
        }        
    }    
    return size;
}
static DEVICE_ATTR(eta6937_access, 0664, show_eta6937_access, store_eta6937_access); //664

static int eta6937_user_space_probe(struct platform_device *dev)    
{    
    int ret_device_file = 0;
    
    ret_device_file = device_create_file(&(dev->dev), &dev_attr_eta6937_access);
    
    return 0;
}

struct platform_device eta6937_user_space_device = {
    .name   = "eta6937-user",
    .id     = -1,
};

static struct platform_driver eta6937_user_space_driver = {
    .probe      = eta6937_user_space_probe,
    .driver     = {
        .name = "eta6937-user",
    },
};


static int eta6937_driver_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;
	struct eta6937_info *info = NULL;

	pr_info_eta6937("[eta6937_driver_probe]\n");

	info = devm_kzalloc(&client->dev, sizeof(struct eta6937_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	new_client = client;
	info->dev = &client->dev;

	ret = eta6937_parse_dt(info, &client->dev);
	if (ret < 0)
		return ret;

	/* Register charger device */
	info->chg_dev = charger_device_register(info->chg_dev_name,
		&client->dev, info, &eta6937_chg_ops, &info->chg_props);
	if (IS_ERR_OR_NULL(info->chg_dev)) {
		pr_err("%s: register charger device  failed\n", __func__);
		ret = PTR_ERR(info->chg_dev);
		return ret;
	}

	/* --------------------- */
	eta6937_hw_component_detect();
    eta6937_charging_hw_init();
    chargin_hw_init_done = true;

	info->psy = power_supply_get_by_name("charger");
	if (!info->psy) {
		pr_err("%s: get power supply failed\n", __func__);
		return -EINVAL;
	}

	/* Force charger type detection */

#ifdef CONFIG_PROJECT_PHY
	Charger_Detect_Init();
#endif
	msleep(50);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id eta6937_of_match[] = {
	{.compatible = "mediatek,swithing_charger"},
	{},
};
MODULE_DEVICE_TABLE(of, eta6937_of_match);
#else
static struct i2c_board_info i2c_eta6937 __initdata = {
	I2C_BOARD_INFO("eta6937", (eta6937_SLAVE_ADDR_WRITE >> 1))
};
#endif

static struct i2c_driver eta6937_driver = {
	.driver = {
		   .name = "eta6937",
#ifdef CONFIG_OF
		   .of_match_table = eta6937_of_match,
#endif
		   },
	.probe = eta6937_driver_probe,
	.id_table = eta6937_i2c_id,
};

static int __init eta6937_init(void)
{
	int ret = 0;

	/* i2c registeration using DTS instead of boardinfo*/
#ifdef CONFIG_OF
	pr_info("[eta6937_init] init start with i2c DTS");
#else
	pr_info("[eta6937_init] init start. ch=%d\n", eta6937_BUSNUM);
	i2c_register_board_info(eta6937_BUSNUM, &i2c_eta6937, 1);
#endif
	if (i2c_add_driver(&eta6937_driver) != 0) {
		pr_info(
			    "[eta6937_init] failed to register eta6937 i2c driver.\n");
	} else {
		pr_info(
			    "[eta6937_init] Success to register eta6937 i2c driver.\n");
	}

	/* eta6937 user space access interface */
	ret = platform_device_register(&eta6937_user_space_device);
	if (ret) {
		pr_info("****[eta6937_init] Unable to device register(%d)\n",
			    ret);
		return ret;
	}
	ret = platform_driver_register(&eta6937_user_space_driver);
	if (ret) {
		pr_info("****[eta6937_init] Unable to register driver (%d)\n",
			    ret);
		return ret;
	}

	return 0;
}

static void __exit eta6937_exit(void)
{
	i2c_del_driver(&eta6937_driver);
}
module_init(eta6937_init);
module_exit(eta6937_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C eta6937 Driver");
MODULE_AUTHOR("will cai <will.cai@mediatek.com>");
