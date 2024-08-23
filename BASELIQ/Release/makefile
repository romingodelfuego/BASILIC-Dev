################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/XMOS_xScope/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/UDP/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/TCPIP_Win32/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/TCPIP/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/Jlink_RTT/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/File/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/ARM_ITM/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/extras/RunnableExample/source/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/extras/ExtensionExample/source/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/RTOS_subfunctions/subdir.mk
-include Core/Src/LORA/subdir.mk
-include Core/Src/GNSS/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := BASELIQ
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BASELIQ.elf \

MAP_FILES += \
BASELIQ.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BASELIQ.list \


# All Target
all: main-build

# Main-build Target
main-build: BASELIQ.elf secondary-outputs

# Tool invocations
BASELIQ.elf BASELIQ.map: $(OBJS) $(USER_OBJS) C:\Users\romain.pace\Documents\GitHub\BASELIQ-Dev\BASELIQ\STM32L496VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "BASELIQ.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\romain.pace\Documents\GitHub\BASELIQ-Dev\BASELIQ\STM32L496VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BASELIQ.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BASELIQ.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BASELIQ.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) BASELIQ.elf BASELIQ.list BASELIQ.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
