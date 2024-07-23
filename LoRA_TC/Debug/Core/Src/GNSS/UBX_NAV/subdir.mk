################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.c 

OBJS += \
./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.o 

C_DEPS += \
./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GNSS/UBX_NAV/%.o Core/Src/GNSS/UBX_NAV/%.su Core/Src/GNSS/UBX_NAV/%.cyclo: ../Core/Src/GNSS/UBX_NAV/%.c Core/Src/GNSS/UBX_NAV/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GNSS-2f-UBX_NAV

clean-Core-2f-Src-2f-GNSS-2f-UBX_NAV:
	-$(RM) ./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.cyclo ./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.d ./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.o ./Core/Src/GNSS/UBX_NAV/UBX_NAV_SIG.su

.PHONY: clean-Core-2f-Src-2f-GNSS-2f-UBX_NAV

