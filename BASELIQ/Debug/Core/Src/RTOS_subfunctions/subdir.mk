################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/RTOS_subfunctions/commandToGNSS.c \
../Core/Src/RTOS_subfunctions/debug.c \
../Core/Src/RTOS_subfunctions/fakeuseSD.c \
../Core/Src/RTOS_subfunctions/matcher.c \
../Core/Src/RTOS_subfunctions/receiverLoRA.c \
../Core/Src/RTOS_subfunctions/senderLoRA.c \
../Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.c 

C_DEPS += \
./Core/Src/RTOS_subfunctions/commandToGNSS.d \
./Core/Src/RTOS_subfunctions/debug.d \
./Core/Src/RTOS_subfunctions/fakeuseSD.d \
./Core/Src/RTOS_subfunctions/matcher.d \
./Core/Src/RTOS_subfunctions/receiverLoRA.d \
./Core/Src/RTOS_subfunctions/senderLoRA.d \
./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.d 

OBJS += \
./Core/Src/RTOS_subfunctions/commandToGNSS.o \
./Core/Src/RTOS_subfunctions/debug.o \
./Core/Src/RTOS_subfunctions/fakeuseSD.o \
./Core/Src/RTOS_subfunctions/matcher.o \
./Core/Src/RTOS_subfunctions/receiverLoRA.o \
./Core/Src/RTOS_subfunctions/senderLoRA.o \
./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/RTOS_subfunctions/%.o Core/Src/RTOS_subfunctions/%.su Core/Src/RTOS_subfunctions/%.cyclo: ../Core/Src/RTOS_subfunctions/%.c Core/Src/RTOS_subfunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/ARM_ITM/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/ARM_ITM/include" -I../Core/Inc -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/include" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-RTOS_subfunctions

clean-Core-2f-Src-2f-RTOS_subfunctions:
	-$(RM) ./Core/Src/RTOS_subfunctions/commandToGNSS.cyclo ./Core/Src/RTOS_subfunctions/commandToGNSS.d ./Core/Src/RTOS_subfunctions/commandToGNSS.o ./Core/Src/RTOS_subfunctions/commandToGNSS.su ./Core/Src/RTOS_subfunctions/debug.cyclo ./Core/Src/RTOS_subfunctions/debug.d ./Core/Src/RTOS_subfunctions/debug.o ./Core/Src/RTOS_subfunctions/debug.su ./Core/Src/RTOS_subfunctions/fakeuseSD.cyclo ./Core/Src/RTOS_subfunctions/fakeuseSD.d ./Core/Src/RTOS_subfunctions/fakeuseSD.o ./Core/Src/RTOS_subfunctions/fakeuseSD.su ./Core/Src/RTOS_subfunctions/matcher.cyclo ./Core/Src/RTOS_subfunctions/matcher.d ./Core/Src/RTOS_subfunctions/matcher.o ./Core/Src/RTOS_subfunctions/matcher.su ./Core/Src/RTOS_subfunctions/receiverLoRA.cyclo ./Core/Src/RTOS_subfunctions/receiverLoRA.d ./Core/Src/RTOS_subfunctions/receiverLoRA.o ./Core/Src/RTOS_subfunctions/receiverLoRA.su ./Core/Src/RTOS_subfunctions/senderLoRA.cyclo ./Core/Src/RTOS_subfunctions/senderLoRA.d ./Core/Src/RTOS_subfunctions/senderLoRA.o ./Core/Src/RTOS_subfunctions/senderLoRA.su ./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.cyclo ./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.d ./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.o ./Core/Src/RTOS_subfunctions/uartbyteToGnssMessage.su

.PHONY: clean-Core-2f-Src-2f-RTOS_subfunctions

