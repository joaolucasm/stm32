################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Waveform/uartValueTransmist.c \
../App/Waveform/waveform.c 

OBJS += \
./App/Waveform/uartValueTransmist.o \
./App/Waveform/waveform.o 

C_DEPS += \
./App/Waveform/uartValueTransmist.d \
./App/Waveform/waveform.d 


# Each subdirectory must supply rules for building sources it contributes
App/Waveform/%.o App/Waveform/%.su App/Waveform/%.cyclo: ../App/Waveform/%.c App/Waveform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../App -I../App/ADE9000 -I../App/ADE9000/Inc -I../App/ADE9000/Src -I../App/Waveform -I../App/Support -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Waveform

clean-App-2f-Waveform:
	-$(RM) ./App/Waveform/uartValueTransmist.cyclo ./App/Waveform/uartValueTransmist.d ./App/Waveform/uartValueTransmist.o ./App/Waveform/uartValueTransmist.su ./App/Waveform/waveform.cyclo ./App/Waveform/waveform.d ./App/Waveform/waveform.o ./App/Waveform/waveform.su

.PHONY: clean-App-2f-Waveform

