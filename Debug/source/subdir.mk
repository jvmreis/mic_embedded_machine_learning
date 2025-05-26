################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/peripherals.c \
../source/playbackSineWave.c \
../source/playbackSong.c \
../source/recordPlayback.c \
../source/recordtoSdcard.c \
../source/sai_peripheral.c \
../source/sdmmc_config.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/peripherals.d \
./source/playbackSineWave.d \
./source/playbackSong.d \
./source/recordPlayback.d \
./source/recordtoSdcard.d \
./source/sai_peripheral.d \
./source/sdmmc_config.d \
./source/semihost_hardfault.d 

OBJS += \
./source/peripherals.o \
./source/playbackSineWave.o \
./source/playbackSong.o \
./source/recordPlayback.o \
./source/recordtoSdcard.o \
./source/sai_peripheral.o \
./source/sdmmc_config.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DMCUXPRESSO_SDK -DXIP_BOOT_HEADER_ENABLE=1 -DXIP_EXTERNAL_FLASH=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DOSA_USED -DDISABLEFLOAT16 -DSDK_I2C_BASED_COMPONENT_USED=1 -DCODEC_MULTI_ADAPTERS=1 -DCODEC_WM8960_ENABLE -DUSE_RTOS=0 -DSD_ENABLED=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Github\mic_embedded_machine_learning\source" -I"C:\Github\mic_embedded_machine_learning\xip" -I"C:\Github\mic_embedded_machine_learning\drivers" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\Include" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\PrivateInclude" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\Source\DistanceFunctions" -I"C:\Github\mic_embedded_machine_learning\CMSIS" -I"C:\Github\mic_embedded_machine_learning\CMSIS\m-profile" -I"C:\Github\mic_embedded_machine_learning\device" -I"C:\Github\mic_embedded_machine_learning\device\periph" -I"C:\Github\mic_embedded_machine_learning\utilities" -I"C:\Github\mic_embedded_machine_learning\component\lists" -I"C:\Github\mic_embedded_machine_learning\utilities\str" -I"C:\Github\mic_embedded_machine_learning\utilities\debug_console_lite" -I"C:\Github\mic_embedded_machine_learning\codec" -I"C:\Github\mic_embedded_machine_learning\codec\port" -I"C:\Github\mic_embedded_machine_learning\codec\port\wm8960" -I"C:\Github\mic_embedded_machine_learning\component\gpio" -I"C:\Github\mic_embedded_machine_learning\component\i2c" -I"C:\Github\mic_embedded_machine_learning\component\uart" -I"C:\Github\mic_embedded_machine_learning\component\osa\config" -I"C:\Github\mic_embedded_machine_learning\component\osa" -I"C:\Github\mic_embedded_machine_learning\fatfs\source" -I"C:\Github\mic_embedded_machine_learning\fatfs\source\fsl_sd_disk" -I"C:\Github\mic_embedded_machine_learning\sdmmc\common" -I"C:\Github\mic_embedded_machine_learning\sdmmc\osa" -I"C:\Github\mic_embedded_machine_learning\sdmmc\sd" -I"C:\Github\mic_embedded_machine_learning\sdmmc\host\usdhc" -I"C:\Github\mic_embedded_machine_learning\board" -I"C:\Github\mic_embedded_machine_learning\source\template\sd" -I"C:\Github\mic_embedded_machine_learning\source\template" -I"C:\Github\mic_embedded_machine_learning\sdmmc\template\usdhc" -O0 -fno-common -g3 -gdwarf-4 -mthumb -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Github\mic_embedded_machine_learning\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/peripherals.d ./source/peripherals.o ./source/playbackSineWave.d ./source/playbackSineWave.o ./source/playbackSong.d ./source/playbackSong.o ./source/recordPlayback.d ./source/recordPlayback.o ./source/recordtoSdcard.d ./source/recordtoSdcard.o ./source/sai_peripheral.d ./source/sai_peripheral.o ./source/sdmmc_config.d ./source/sdmmc_config.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

