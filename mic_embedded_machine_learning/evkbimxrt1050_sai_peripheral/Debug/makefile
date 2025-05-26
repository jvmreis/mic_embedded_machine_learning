################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include xip/subdir.mk
-include utilities/str/subdir.mk
-include utilities/debug_console_lite/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include sdmmc/sd/subdir.mk
-include sdmmc/osa/subdir.mk
-include sdmmc/host/usdhc/non_blocking/subdir.mk
-include sdmmc/common/subdir.mk
-include fatfs/source/fsl_sd_disk/subdir.mk
-include fatfs/source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/osa/subdir.mk
-include component/lists/subdir.mk
-include component/i2c/subdir.mk
-include component/gpio/subdir.mk
-include codec/port/wm8960/subdir.mk
-include codec/port/subdir.mk
-include codec/subdir.mk
-include board/subdir.mk
-include CMSIS/DSP/Source/WindowFunctions/subdir.mk
-include CMSIS/DSP/Source/TransformFunctions/subdir.mk
-include CMSIS/DSP/Source/SupportFunctions/subdir.mk
-include CMSIS/DSP/Source/StatisticsFunctions/subdir.mk
-include CMSIS/DSP/Source/SVMFunctions/subdir.mk
-include CMSIS/DSP/Source/QuaternionMathFunctions/subdir.mk
-include CMSIS/DSP/Source/MatrixFunctions/subdir.mk
-include CMSIS/DSP/Source/InterpolationFunctions/subdir.mk
-include CMSIS/DSP/Source/FilteringFunctions/subdir.mk
-include CMSIS/DSP/Source/FastMathFunctions/subdir.mk
-include CMSIS/DSP/Source/DistanceFunctions/subdir.mk
-include CMSIS/DSP/Source/ControllerFunctions/subdir.mk
-include CMSIS/DSP/Source/ComplexMathFunctions/subdir.mk
-include CMSIS/DSP/Source/CommonTables/subdir.mk
-include CMSIS/DSP/Source/BayesFunctions/subdir.mk
-include CMSIS/DSP/Source/BasicMathFunctions/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := evkbimxrt1050_sai_peripheral
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: evkbimxrt1050_sai_peripheral.axf

# Tool invocations
evkbimxrt1050_sai_peripheral.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker -Map="evkbimxrt1050_sai_peripheral.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -T evkbimxrt1050_sai_peripheral_Debug.ld -o "evkbimxrt1050_sai_peripheral.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) evkbimxrt1050_sai_peripheral.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "evkbimxrt1050_sai_peripheral.axf"; # arm-none-eabi-objcopy -v -O binary "evkbimxrt1050_sai_peripheral.axf" "evkbimxrt1050_sai_peripheral.bin" ; # checksum -p MIMXRT1052xxxxB -d "evkbimxrt1050_sai_peripheral.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
