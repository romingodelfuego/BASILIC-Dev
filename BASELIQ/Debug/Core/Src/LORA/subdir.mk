################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LORA/LORACom.c \
../Core/Src/LORA/RFM9x.c 

C_DEPS += \
./Core/Src/LORA/LORACom.d \
./Core/Src/LORA/RFM9x.d 

OBJS += \
./Core/Src/LORA/LORACom.o \
./Core/Src/LORA/RFM9x.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LORA/%.o Core/Src/LORA/%.su Core/Src/LORA/%.cyclo: ../Core/Src/LORA/%.c Core/Src/LORA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LORA

clean-Core-2f-Src-2f-LORA:
	-$(RM) ./Core/Src/LORA/LORACom.cyclo ./Core/Src/LORA/LORACom.d ./Core/Src/LORA/LORACom.o ./Core/Src/LORA/LORACom.su ./Core/Src/LORA/RFM9x.cyclo ./Core/Src/LORA/RFM9x.d ./Core/Src/LORA/RFM9x.o ./Core/Src/LORA/RFM9x.su

.PHONY: clean-Core-2f-Src-2f-LORA

