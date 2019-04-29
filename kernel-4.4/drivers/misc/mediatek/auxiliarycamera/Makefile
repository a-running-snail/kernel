# In case the platform does NOT support this type of sensors
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
ARCH_MTK_PROJECT := $(subst ",,$(CONFIG_ARCH_MTK_PROJECT))

MTK_PLATFORM_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)
MTK_PROJECT_PATH_ROOT := $(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)

CONFIG_MTK_PLATFORM_SENSOR := $(CONFIG_MTK_PLATFORM)

subdir-ccflags-y += -I$(MTK_PROJECT_PATH_ROOT)/camera/camera \

MTK_CUSTOM_PATH  := $(srctree)/drivers/misc/mediatek

subdir-ccflags-y += -I$(MTK_CUSTOM_PATH)/camera/$(MTK_PLATFORM) \
             -I$(MTK_CUSTOM_PATH)/imgsensor/inc \
             -I$(MTK_CUSTOM_PATH)/imgsensor/src/mt6739/camera_hw \
             -I$(MTK_CUSTOM_PATH)/cam_cal/inc \
             -I$(MTK_CUSTOM_PATH)/smi \


obj-y   +=  auxiliarycamera_core.o gc0310mipi_yuv_Sensor.o
obj-y   +=  gc0310mipi_yuv_Sensor.o

ifeq ($(strip $(XUNHU_LPS_AUXILIARYCAMERAFRONT_SUPPORT)),yes)
obj-y   +=  auxiliarycamera_core_front.o gc0310mipi_yuv_Sensor_front.o
endif
#TEKSUN BEGIN
#add By caicai for kernel macro control
#$(warning -caicai-debug-CUSTOM_KERNEL_ALSPS=$(CUSTOM_KERNEL_ALSPS)==)
ifneq ($(strip $(CUSTOM_KERNEL_ALSPS)),)
ALSPS_LISTS := $(subst ",,$(CUSTOM_KERNEL_ALSPS))
$(info Teksun-kernel-build-alsps:$(ALSPS_LISTS))
#obj-y += $(foreach ALSPS,$(ALSPS_LISTS),$(ALSPS)/)
endif
#TEKSUN END
