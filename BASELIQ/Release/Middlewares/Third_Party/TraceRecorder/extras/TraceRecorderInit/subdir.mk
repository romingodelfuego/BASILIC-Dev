################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.cpp 

OBJS += \
./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.o 

CPP_DEPS += \
./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/%.o Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/%.su Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/%.cyclo: ../Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/%.cpp Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit

clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit:
	-$(RM) ./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.cyclo ./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.d ./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.o ./Middlewares/Third_Party/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit

