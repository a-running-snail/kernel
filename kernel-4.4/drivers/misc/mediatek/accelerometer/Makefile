# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
obj-y   +=  accel.o accel_factory.o

ifeq ($(CONFIG_MTK_SCP_SENSORHUB_V1),y)
obj-$(CONFIG_MTK_ACCELHUB)		+=	accelhub/
else
obj-$(CONFIG_MTK_BMA2XX)   +=  bma2xx/
obj-$(CONFIG_MTK_MC3433)   +=  mc3433/
obj-$(CONFIG_MTK_MC3410_NEW)   +=  mc3410-new/
obj-$(CONFIG_MTK_LSM6DS3HG)   +=  lsm6ds3hg/
obj-$(CONFIG_MTK_BMI160_SPI)   +=  bmi160-spi/
obj-$(CONFIG_MTK_BMI160_I2C)   +=  bmi160-i2c/
obj-$(CONFIG_MTK_MPU6050G)   +=  mpu6050g/
obj-$(CONFIG_MTK_MIR3DA)   +=  mir3da/
obj-$(CONFIG_MTK_SC7A20_NEW)   +=  sc7a20/
endif
