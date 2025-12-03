################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/ADE9000/Src/ADE9000.c \
../App/ADE9000/Src/ADE9000API.c \
../App/ADE9000/Src/ADE9000SPI.c 

OBJS += \
./App/ADE9000/Src/ADE9000.o \
./App/ADE9000/Src/ADE9000API.o \
./App/ADE9000/Src/ADE9000SPI.o 

C_DEPS += \
./App/ADE9000/Src/ADE9000.d \
./App/ADE9000/Src/ADE9000API.d \
./App/ADE9000/Src/ADE9000SPI.d 


# Each subdirectory must supply rules for building sources it contributes
App/ADE9000/Src/%.o App/ADE9000/Src/%.su App/ADE9000/Src/%.cyclo: ../App/ADE9000/Src/%.c App/ADE9000/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../App -I../App/ADE9000 -I../App/ADE9000/Inc -I../App/ADE9000/Src -I../App/Waveform -I../App/Support -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-ADE9000-2f-Src

clean-App-2f-ADE9000-2f-Src:
	-$(RM) ./App/ADE9000/Src/ADE9000.cyclo ./App/ADE9000/Src/ADE9000.d ./App/ADE9000/Src/ADE9000.o ./App/ADE9000/Src/ADE9000.su ./App/ADE9000/Src/ADE9000API.cyclo ./App/ADE9000/Src/ADE9000API.d ./App/ADE9000/Src/ADE9000API.o ./App/ADE9000/Src/ADE9000API.su ./App/ADE9000/Src/ADE9000SPI.cyclo ./App/ADE9000/Src/ADE9000SPI.d ./App/ADE9000/Src/ADE9000SPI.o ./App/ADE9000/Src/ADE9000SPI.su

.PHONY: clean-App-2f-ADE9000-2f-Src

