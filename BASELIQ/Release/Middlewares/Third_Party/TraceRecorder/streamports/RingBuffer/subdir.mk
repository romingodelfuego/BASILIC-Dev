################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.c 

C_DEPS += \
./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.d 

OBJS += \
./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/%.o Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/%.su Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/%.cyclo: ../Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/%.c Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-RingBuffer

clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-RingBuffer:
	-$(RM) ./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.cyclo ./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.d ./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.o ./Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/trcStreamPort.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-RingBuffer

