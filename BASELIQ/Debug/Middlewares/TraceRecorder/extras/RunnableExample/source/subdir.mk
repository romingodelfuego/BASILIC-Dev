################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/TraceRecorder/extras/RunnableExample/source/main.c 

C_DEPS += \
./Middlewares/TraceRecorder/extras/RunnableExample/source/main.d 

OBJS += \
./Middlewares/TraceRecorder/extras/RunnableExample/source/main.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/TraceRecorder/extras/RunnableExample/source/%.o Middlewares/TraceRecorder/extras/RunnableExample/source/%.su Middlewares/TraceRecorder/extras/RunnableExample/source/%.cyclo: ../Middlewares/TraceRecorder/extras/RunnableExample/source/%.c Middlewares/TraceRecorder/extras/RunnableExample/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-TraceRecorder-2f-extras-2f-RunnableExample-2f-source

clean-Middlewares-2f-TraceRecorder-2f-extras-2f-RunnableExample-2f-source:
	-$(RM) ./Middlewares/TraceRecorder/extras/RunnableExample/source/main.cyclo ./Middlewares/TraceRecorder/extras/RunnableExample/source/main.d ./Middlewares/TraceRecorder/extras/RunnableExample/source/main.o ./Middlewares/TraceRecorder/extras/RunnableExample/source/main.su

.PHONY: clean-Middlewares-2f-TraceRecorder-2f-extras-2f-RunnableExample-2f-source

