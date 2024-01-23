################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/smtc_hal/smtc_hal_flash.c \
../Core/Src/smtc_hal/smtc_hal_gpio.c \
../Core/Src/smtc_hal/smtc_hal_mcu.c \
../Core/Src/smtc_hal/smtc_hal_uart.c \
../Core/Src/smtc_hal/smtc_utilities.c 

OBJS += \
./Core/Src/smtc_hal/smtc_hal_flash.o \
./Core/Src/smtc_hal/smtc_hal_gpio.o \
./Core/Src/smtc_hal/smtc_hal_mcu.o \
./Core/Src/smtc_hal/smtc_hal_uart.o \
./Core/Src/smtc_hal/smtc_utilities.o 

C_DEPS += \
./Core/Src/smtc_hal/smtc_hal_flash.d \
./Core/Src/smtc_hal/smtc_hal_gpio.d \
./Core/Src/smtc_hal/smtc_hal_mcu.d \
./Core/Src/smtc_hal/smtc_hal_uart.d \
./Core/Src/smtc_hal/smtc_utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/smtc_hal/%.o Core/Src/smtc_hal/%.su Core/Src/smtc_hal/%.cyclo: ../Core/Src/smtc_hal/%.c Core/Src/smtc_hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Core/Inc/boards -I../Core/Inc/smtc_hal -I../Core/Src -I../Core/Src/radio -I../Core/Src/radio/lr1110_modem/src -I../Drivers/BSP/Components/Leds -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-smtc_hal

clean-Core-2f-Src-2f-smtc_hal:
	-$(RM) ./Core/Src/smtc_hal/smtc_hal_flash.cyclo ./Core/Src/smtc_hal/smtc_hal_flash.d ./Core/Src/smtc_hal/smtc_hal_flash.o ./Core/Src/smtc_hal/smtc_hal_flash.su ./Core/Src/smtc_hal/smtc_hal_gpio.cyclo ./Core/Src/smtc_hal/smtc_hal_gpio.d ./Core/Src/smtc_hal/smtc_hal_gpio.o ./Core/Src/smtc_hal/smtc_hal_gpio.su ./Core/Src/smtc_hal/smtc_hal_mcu.cyclo ./Core/Src/smtc_hal/smtc_hal_mcu.d ./Core/Src/smtc_hal/smtc_hal_mcu.o ./Core/Src/smtc_hal/smtc_hal_mcu.su ./Core/Src/smtc_hal/smtc_hal_uart.cyclo ./Core/Src/smtc_hal/smtc_hal_uart.d ./Core/Src/smtc_hal/smtc_hal_uart.o ./Core/Src/smtc_hal/smtc_hal_uart.su ./Core/Src/smtc_hal/smtc_utilities.cyclo ./Core/Src/smtc_hal/smtc_utilities.d ./Core/Src/smtc_hal/smtc_utilities.o ./Core/Src/smtc_hal/smtc_utilities.su

.PHONY: clean-Core-2f-Src-2f-smtc_hal

