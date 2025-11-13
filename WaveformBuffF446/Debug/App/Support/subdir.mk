################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Support/calibration.c \
../App/Support/supportFunctions.c 

OBJS += \
./App/Support/calibration.o \
./App/Support/supportFunctions.o 

C_DEPS += \
./App/Support/calibration.d \
./App/Support/supportFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
App/Support/%.o App/Support/%.su App/Support/%.cyclo: ../App/Support/%.c App/Support/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../App -I../App/ADE9000 -I../App/ADE9000/Inc -I../App/ADE9000/Src -I../App/Waveform -I../App/Support -I../App/PrintWaveform -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Support

clean-App-2f-Support:
	-$(RM) ./App/Support/calibration.cyclo ./App/Support/calibration.d ./App/Support/calibration.o ./App/Support/calibration.su ./App/Support/supportFunctions.cyclo ./App/Support/supportFunctions.d ./App/Support/supportFunctions.o ./App/Support/supportFunctions.su

.PHONY: clean-App-2f-Support

