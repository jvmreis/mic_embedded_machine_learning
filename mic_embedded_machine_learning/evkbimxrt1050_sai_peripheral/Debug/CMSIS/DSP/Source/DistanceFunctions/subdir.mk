################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.c \
../CMSIS/DSP/Source/DistanceFunctions/DistanceFunctionsF16.c 

C_DEPS += \
./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.d \
./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctionsF16.d 

OBJS += \
./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.o \
./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctionsF16.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/DistanceFunctions/%.o: ../CMSIS/DSP/Source/DistanceFunctions/%.c CMSIS/DSP/Source/DistanceFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DMCUXPRESSO_SDK -DXIP_BOOT_HEADER_ENABLE=1 -DXIP_EXTERNAL_FLASH=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DOSA_USED -DDISABLEFLOAT16 -DSDK_I2C_BASED_COMPONENT_USED=1 -DCODEC_MULTI_ADAPTERS=1 -DCODEC_WM8960_ENABLE -DUSE_RTOS=0 -DSD_ENABLED=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\source" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\xip" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\drivers" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\CMSIS\DSP\Include" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\CMSIS\DSP\PrivateInclude" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\CMSIS\DSP\Source\DistanceFunctions" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\CMSIS" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\CMSIS\m-profile" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\device" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\device\periph" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\utilities" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\lists" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\utilities\str" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\utilities\debug_console_lite" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\codec" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\codec\port" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\codec\port\wm8960" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\gpio" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\i2c" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\uart" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\osa\config" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\component\osa" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\fatfs\source" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\fatfs\source\fsl_sd_disk" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\sdmmc\common" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\sdmmc\osa" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\sdmmc\sd" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\sdmmc\host\usdhc" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\board" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\source\template\sd" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\source\template" -I"C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\sdmmc\template\usdhc" -O0 -fno-common -g3 -gdwarf-4 -mthumb -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\xmartij01\Documents\MCUXpressoIDE_24.12.148\workspace\evkbimxrt1050_sai_peripheral\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS-2f-DSP-2f-Source-2f-DistanceFunctions

clean-CMSIS-2f-DSP-2f-Source-2f-DistanceFunctions:
	-$(RM) ./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.d ./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.o ./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctionsF16.d ./CMSIS/DSP/Source/DistanceFunctions/DistanceFunctionsF16.o

.PHONY: clean-CMSIS-2f-DSP-2f-Source-2f-DistanceFunctions

