################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/SVMFunctions/SVMFunctions.c \
../CMSIS/DSP/Source/SVMFunctions/SVMFunctionsF16.c 

C_DEPS += \
./CMSIS/DSP/Source/SVMFunctions/SVMFunctions.d \
./CMSIS/DSP/Source/SVMFunctions/SVMFunctionsF16.d 

OBJS += \
./CMSIS/DSP/Source/SVMFunctions/SVMFunctions.o \
./CMSIS/DSP/Source/SVMFunctions/SVMFunctionsF16.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/SVMFunctions/%.o: ../CMSIS/DSP/Source/SVMFunctions/%.c CMSIS/DSP/Source/SVMFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DMCUXPRESSO_SDK -DXIP_BOOT_HEADER_ENABLE=1 -DXIP_EXTERNAL_FLASH=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DOSA_USED -DDISABLEFLOAT16 -DSDK_I2C_BASED_COMPONENT_USED=1 -DCODEC_MULTI_ADAPTERS=1 -DCODEC_WM8960_ENABLE -DUSE_RTOS=0 -DSD_ENABLED=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Github\mic_embedded_machine_learning\source" -I"C:\Github\mic_embedded_machine_learning\source\lib" -I"C:\Github\mic_embedded_machine_learning\xip" -I"C:\Github\mic_embedded_machine_learning\drivers" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\Include" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\PrivateInclude" -I"C:\Github\mic_embedded_machine_learning\CMSIS\DSP\Source\DistanceFunctions" -I"C:\Github\mic_embedded_machine_learning\CMSIS" -I"C:\Github\mic_embedded_machine_learning\CMSIS\m-profile" -I"C:\Github\mic_embedded_machine_learning\device" -I"C:\Github\mic_embedded_machine_learning\device\periph" -I"C:\Github\mic_embedded_machine_learning\utilities" -I"C:\Github\mic_embedded_machine_learning\component\lists" -I"C:\Github\mic_embedded_machine_learning\utilities\str" -I"C:\Github\mic_embedded_machine_learning\utilities\debug_console_lite" -I"C:\Github\mic_embedded_machine_learning\codec" -I"C:\Github\mic_embedded_machine_learning\codec\port" -I"C:\Github\mic_embedded_machine_learning\codec\port\wm8960" -I"C:\Github\mic_embedded_machine_learning\component\gpio" -I"C:\Github\mic_embedded_machine_learning\component\i2c" -I"C:\Github\mic_embedded_machine_learning\component\uart" -I"C:\Github\mic_embedded_machine_learning\component\osa\config" -I"C:\Github\mic_embedded_machine_learning\component\osa" -I"C:\Github\mic_embedded_machine_learning\fatfs\source" -I"C:\Github\mic_embedded_machine_learning\fatfs\source\fsl_sd_disk" -I"C:\Github\mic_embedded_machine_learning\sdmmc\common" -I"C:\Github\mic_embedded_machine_learning\sdmmc\osa" -I"C:\Github\mic_embedded_machine_learning\sdmmc\sd" -I"C:\Github\mic_embedded_machine_learning\sdmmc\host\usdhc" -I"C:\Github\mic_embedded_machine_learning\board" -I"C:\Github\mic_embedded_machine_learning\source\template\sd" -I"C:\Github\mic_embedded_machine_learning\source\template" -I"C:\Github\mic_embedded_machine_learning\sdmmc\template\usdhc" -O0 -fno-common -g3 -gdwarf-4 -mthumb -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Github\mic_embedded_machine_learning\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS-2f-DSP-2f-Source-2f-SVMFunctions

clean-CMSIS-2f-DSP-2f-Source-2f-SVMFunctions:
	-$(RM) ./CMSIS/DSP/Source/SVMFunctions/SVMFunctions.d ./CMSIS/DSP/Source/SVMFunctions/SVMFunctions.o ./CMSIS/DSP/Source/SVMFunctions/SVMFunctionsF16.d ./CMSIS/DSP/Source/SVMFunctions/SVMFunctionsF16.o

.PHONY: clean-CMSIS-2f-DSP-2f-Source-2f-SVMFunctions

