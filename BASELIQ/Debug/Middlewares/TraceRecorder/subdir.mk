################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/TraceRecorder/trcAssert.c \
../Middlewares/TraceRecorder/trcCounter.c \
../Middlewares/TraceRecorder/trcDependency.c \
../Middlewares/TraceRecorder/trcDiagnostics.c \
../Middlewares/TraceRecorder/trcEntryTable.c \
../Middlewares/TraceRecorder/trcError.c \
../Middlewares/TraceRecorder/trcEvent.c \
../Middlewares/TraceRecorder/trcEventBuffer.c \
../Middlewares/TraceRecorder/trcExtension.c \
../Middlewares/TraceRecorder/trcHardwarePort.c \
../Middlewares/TraceRecorder/trcHeap.c \
../Middlewares/TraceRecorder/trcISR.c \
../Middlewares/TraceRecorder/trcInternalEventBuffer.c \
../Middlewares/TraceRecorder/trcInterval.c \
../Middlewares/TraceRecorder/trcKernelPort.c \
../Middlewares/TraceRecorder/trcMultiCoreEventBuffer.c \
../Middlewares/TraceRecorder/trcObject.c \
../Middlewares/TraceRecorder/trcPrint.c \
../Middlewares/TraceRecorder/trcRunnable.c \
../Middlewares/TraceRecorder/trcSnapshotRecorder.c \
../Middlewares/TraceRecorder/trcStackMonitor.c \
../Middlewares/TraceRecorder/trcStateMachine.c \
../Middlewares/TraceRecorder/trcStaticBuffer.c \
../Middlewares/TraceRecorder/trcStreamingRecorder.c \
../Middlewares/TraceRecorder/trcString.c \
../Middlewares/TraceRecorder/trcTask.c \
../Middlewares/TraceRecorder/trcTimestamp.c 

C_DEPS += \
./Middlewares/TraceRecorder/trcAssert.d \
./Middlewares/TraceRecorder/trcCounter.d \
./Middlewares/TraceRecorder/trcDependency.d \
./Middlewares/TraceRecorder/trcDiagnostics.d \
./Middlewares/TraceRecorder/trcEntryTable.d \
./Middlewares/TraceRecorder/trcError.d \
./Middlewares/TraceRecorder/trcEvent.d \
./Middlewares/TraceRecorder/trcEventBuffer.d \
./Middlewares/TraceRecorder/trcExtension.d \
./Middlewares/TraceRecorder/trcHardwarePort.d \
./Middlewares/TraceRecorder/trcHeap.d \
./Middlewares/TraceRecorder/trcISR.d \
./Middlewares/TraceRecorder/trcInternalEventBuffer.d \
./Middlewares/TraceRecorder/trcInterval.d \
./Middlewares/TraceRecorder/trcKernelPort.d \
./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.d \
./Middlewares/TraceRecorder/trcObject.d \
./Middlewares/TraceRecorder/trcPrint.d \
./Middlewares/TraceRecorder/trcRunnable.d \
./Middlewares/TraceRecorder/trcSnapshotRecorder.d \
./Middlewares/TraceRecorder/trcStackMonitor.d \
./Middlewares/TraceRecorder/trcStateMachine.d \
./Middlewares/TraceRecorder/trcStaticBuffer.d \
./Middlewares/TraceRecorder/trcStreamingRecorder.d \
./Middlewares/TraceRecorder/trcString.d \
./Middlewares/TraceRecorder/trcTask.d \
./Middlewares/TraceRecorder/trcTimestamp.d 

OBJS += \
./Middlewares/TraceRecorder/trcAssert.o \
./Middlewares/TraceRecorder/trcCounter.o \
./Middlewares/TraceRecorder/trcDependency.o \
./Middlewares/TraceRecorder/trcDiagnostics.o \
./Middlewares/TraceRecorder/trcEntryTable.o \
./Middlewares/TraceRecorder/trcError.o \
./Middlewares/TraceRecorder/trcEvent.o \
./Middlewares/TraceRecorder/trcEventBuffer.o \
./Middlewares/TraceRecorder/trcExtension.o \
./Middlewares/TraceRecorder/trcHardwarePort.o \
./Middlewares/TraceRecorder/trcHeap.o \
./Middlewares/TraceRecorder/trcISR.o \
./Middlewares/TraceRecorder/trcInternalEventBuffer.o \
./Middlewares/TraceRecorder/trcInterval.o \
./Middlewares/TraceRecorder/trcKernelPort.o \
./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.o \
./Middlewares/TraceRecorder/trcObject.o \
./Middlewares/TraceRecorder/trcPrint.o \
./Middlewares/TraceRecorder/trcRunnable.o \
./Middlewares/TraceRecorder/trcSnapshotRecorder.o \
./Middlewares/TraceRecorder/trcStackMonitor.o \
./Middlewares/TraceRecorder/trcStateMachine.o \
./Middlewares/TraceRecorder/trcStaticBuffer.o \
./Middlewares/TraceRecorder/trcStreamingRecorder.o \
./Middlewares/TraceRecorder/trcString.o \
./Middlewares/TraceRecorder/trcTask.o \
./Middlewares/TraceRecorder/trcTimestamp.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/TraceRecorder/%.o Middlewares/TraceRecorder/%.su Middlewares/TraceRecorder/%.cyclo: ../Middlewares/TraceRecorder/%.c Middlewares/TraceRecorder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/streamports/RingBuffer/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/streamports/RingBuffer/include" -I../Core/Inc -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/config" -I"C:/Users/romain.pace/Documents/GitHub/BASELIQ-Dev/BASELIQ/Middlewares/TraceRecorder/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-TraceRecorder

clean-Middlewares-2f-TraceRecorder:
	-$(RM) ./Middlewares/TraceRecorder/trcAssert.cyclo ./Middlewares/TraceRecorder/trcAssert.d ./Middlewares/TraceRecorder/trcAssert.o ./Middlewares/TraceRecorder/trcAssert.su ./Middlewares/TraceRecorder/trcCounter.cyclo ./Middlewares/TraceRecorder/trcCounter.d ./Middlewares/TraceRecorder/trcCounter.o ./Middlewares/TraceRecorder/trcCounter.su ./Middlewares/TraceRecorder/trcDependency.cyclo ./Middlewares/TraceRecorder/trcDependency.d ./Middlewares/TraceRecorder/trcDependency.o ./Middlewares/TraceRecorder/trcDependency.su ./Middlewares/TraceRecorder/trcDiagnostics.cyclo ./Middlewares/TraceRecorder/trcDiagnostics.d ./Middlewares/TraceRecorder/trcDiagnostics.o ./Middlewares/TraceRecorder/trcDiagnostics.su ./Middlewares/TraceRecorder/trcEntryTable.cyclo ./Middlewares/TraceRecorder/trcEntryTable.d ./Middlewares/TraceRecorder/trcEntryTable.o ./Middlewares/TraceRecorder/trcEntryTable.su ./Middlewares/TraceRecorder/trcError.cyclo ./Middlewares/TraceRecorder/trcError.d ./Middlewares/TraceRecorder/trcError.o ./Middlewares/TraceRecorder/trcError.su ./Middlewares/TraceRecorder/trcEvent.cyclo ./Middlewares/TraceRecorder/trcEvent.d ./Middlewares/TraceRecorder/trcEvent.o ./Middlewares/TraceRecorder/trcEvent.su ./Middlewares/TraceRecorder/trcEventBuffer.cyclo ./Middlewares/TraceRecorder/trcEventBuffer.d ./Middlewares/TraceRecorder/trcEventBuffer.o ./Middlewares/TraceRecorder/trcEventBuffer.su ./Middlewares/TraceRecorder/trcExtension.cyclo ./Middlewares/TraceRecorder/trcExtension.d ./Middlewares/TraceRecorder/trcExtension.o ./Middlewares/TraceRecorder/trcExtension.su ./Middlewares/TraceRecorder/trcHardwarePort.cyclo ./Middlewares/TraceRecorder/trcHardwarePort.d ./Middlewares/TraceRecorder/trcHardwarePort.o ./Middlewares/TraceRecorder/trcHardwarePort.su ./Middlewares/TraceRecorder/trcHeap.cyclo ./Middlewares/TraceRecorder/trcHeap.d ./Middlewares/TraceRecorder/trcHeap.o ./Middlewares/TraceRecorder/trcHeap.su ./Middlewares/TraceRecorder/trcISR.cyclo ./Middlewares/TraceRecorder/trcISR.d ./Middlewares/TraceRecorder/trcISR.o ./Middlewares/TraceRecorder/trcISR.su ./Middlewares/TraceRecorder/trcInternalEventBuffer.cyclo ./Middlewares/TraceRecorder/trcInternalEventBuffer.d ./Middlewares/TraceRecorder/trcInternalEventBuffer.o ./Middlewares/TraceRecorder/trcInternalEventBuffer.su ./Middlewares/TraceRecorder/trcInterval.cyclo ./Middlewares/TraceRecorder/trcInterval.d ./Middlewares/TraceRecorder/trcInterval.o ./Middlewares/TraceRecorder/trcInterval.su ./Middlewares/TraceRecorder/trcKernelPort.cyclo ./Middlewares/TraceRecorder/trcKernelPort.d ./Middlewares/TraceRecorder/trcKernelPort.o ./Middlewares/TraceRecorder/trcKernelPort.su ./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.cyclo ./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.d ./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.o ./Middlewares/TraceRecorder/trcMultiCoreEventBuffer.su ./Middlewares/TraceRecorder/trcObject.cyclo ./Middlewares/TraceRecorder/trcObject.d ./Middlewares/TraceRecorder/trcObject.o ./Middlewares/TraceRecorder/trcObject.su ./Middlewares/TraceRecorder/trcPrint.cyclo ./Middlewares/TraceRecorder/trcPrint.d ./Middlewares/TraceRecorder/trcPrint.o ./Middlewares/TraceRecorder/trcPrint.su ./Middlewares/TraceRecorder/trcRunnable.cyclo ./Middlewares/TraceRecorder/trcRunnable.d ./Middlewares/TraceRecorder/trcRunnable.o ./Middlewares/TraceRecorder/trcRunnable.su ./Middlewares/TraceRecorder/trcSnapshotRecorder.cyclo ./Middlewares/TraceRecorder/trcSnapshotRecorder.d ./Middlewares/TraceRecorder/trcSnapshotRecorder.o ./Middlewares/TraceRecorder/trcSnapshotRecorder.su ./Middlewares/TraceRecorder/trcStackMonitor.cyclo ./Middlewares/TraceRecorder/trcStackMonitor.d ./Middlewares/TraceRecorder/trcStackMonitor.o ./Middlewares/TraceRecorder/trcStackMonitor.su ./Middlewares/TraceRecorder/trcStateMachine.cyclo ./Middlewares/TraceRecorder/trcStateMachine.d ./Middlewares/TraceRecorder/trcStateMachine.o ./Middlewares/TraceRecorder/trcStateMachine.su ./Middlewares/TraceRecorder/trcStaticBuffer.cyclo ./Middlewares/TraceRecorder/trcStaticBuffer.d ./Middlewares/TraceRecorder/trcStaticBuffer.o ./Middlewares/TraceRecorder/trcStaticBuffer.su ./Middlewares/TraceRecorder/trcStreamingRecorder.cyclo ./Middlewares/TraceRecorder/trcStreamingRecorder.d ./Middlewares/TraceRecorder/trcStreamingRecorder.o ./Middlewares/TraceRecorder/trcStreamingRecorder.su ./Middlewares/TraceRecorder/trcString.cyclo ./Middlewares/TraceRecorder/trcString.d ./Middlewares/TraceRecorder/trcString.o ./Middlewares/TraceRecorder/trcString.su ./Middlewares/TraceRecorder/trcTask.cyclo ./Middlewares/TraceRecorder/trcTask.d ./Middlewares/TraceRecorder/trcTask.o ./Middlewares/TraceRecorder/trcTask.su ./Middlewares/TraceRecorder/trcTimestamp.cyclo ./Middlewares/TraceRecorder/trcTimestamp.d ./Middlewares/TraceRecorder/trcTimestamp.o ./Middlewares/TraceRecorder/trcTimestamp.su

.PHONY: clean-Middlewares-2f-TraceRecorder

