################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GNSS/GNSSCom.c \
../Core/Src/GNSS/UBXParser.c \
../Core/Src/GNSS/traductor.c 

OBJS += \
./Core/Src/GNSS/GNSSCom.o \
./Core/Src/GNSS/UBXParser.o \
./Core/Src/GNSS/traductor.o 

C_DEPS += \
./Core/Src/GNSS/GNSSCom.d \
./Core/Src/GNSS/UBXParser.d \
./Core/Src/GNSS/traductor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GNSS/%.o Core/Src/GNSS/%.su Core/Src/GNSS/%.cyclo: ../Core/Src/GNSS/%.c Core/Src/GNSS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GNSS

clean-Core-2f-Src-2f-GNSS:
	-$(RM) ./Core/Src/GNSS/GNSSCom.cyclo ./Core/Src/GNSS/GNSSCom.d ./Core/Src/GNSS/GNSSCom.o ./Core/Src/GNSS/GNSSCom.su ./Core/Src/GNSS/UBXParser.cyclo ./Core/Src/GNSS/UBXParser.d ./Core/Src/GNSS/UBXParser.o ./Core/Src/GNSS/UBXParser.su ./Core/Src/GNSS/traductor.cyclo ./Core/Src/GNSS/traductor.d ./Core/Src/GNSS/traductor.o ./Core/Src/GNSS/traductor.su

.PHONY: clean-Core-2f-Src-2f-GNSS

