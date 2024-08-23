################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.cpp 

OBJS += \
./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.o 

CPP_DEPS += \
./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TraceRecorder/extras/TraceRecorderInit/%.o Core/Src/TraceRecorder/extras/TraceRecorderInit/%.su Core/Src/TraceRecorder/extras/TraceRecorderInit/%.cyclo: ../Core/Src/TraceRecorder/extras/TraceRecorderInit/%.cpp Core/Src/TraceRecorder/extras/TraceRecorderInit/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit

clean-Core-2f-Src-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit:
	-$(RM) ./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.cyclo ./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.d ./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.o ./Core/Src/TraceRecorder/extras/TraceRecorderInit/TraceRecorderInit.su

.PHONY: clean-Core-2f-Src-2f-TraceRecorder-2f-extras-2f-TraceRecorderInit

