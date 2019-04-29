ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/ft6x36/include/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/ft6x36/include/$(CONFIG_FT6X36_FIRMWARE)/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/
# path := $(if $(filter yes,$(KBUILD_OUTPUT_SUPPORT)),$(srctree)/$(obj),$(obj))
# obj-y := $(patsubst %.c,%.o,$(subst $(path)/,,$(call wildcard2,$(path)/*.c)))
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include/
ccflags-y += -I$(srctree)/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/hwmsen/

ccflags-y += -I$(srctree)/drivers/misc/mediatek/alsps/inc

obj-y	+=  focaltech_core.o
obj-y	+=  focaltech_ctl.o
obj-y	+=  focaltech_ex_fun.o
obj-y	+=  focaltech_flash.o
obj-y	+=  focaltech_Gesture.o

obj-y += ft_gesture_lib.a
$(obj)/ft_gesture_lib.a:$(srctree)/$(obj)/ft_gesture_lib 
	cp $(srctree)/$(obj)/ft_gesture_lib $(obj)/ft_gesture_lib.a
