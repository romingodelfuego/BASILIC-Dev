################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.c 

C_DEPS += \
./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.d 

OBJS += \
./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/%.o Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/%.su Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/%.cyclo: ../Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/%.c Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/include" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-STM32_USB_CDC

clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-STM32_USB_CDC:
	-$(RM) ./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.cyclo ./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.d ./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.o ./Middlewares/Third_Party/TraceRecorder/streamports/STM32_USB_CDC/trcStreamPort.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-TraceRecorder-2f-streamports-2f-STM32_USB_CDC

