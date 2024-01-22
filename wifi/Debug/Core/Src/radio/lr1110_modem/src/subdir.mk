################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/radio/lr1110_modem/src/lr1110_bootloader.c \
../Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.c \
../Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.c \
../Core/Src/radio/lr1110_modem/src/lr1110_modem_system.c 

OBJS += \
./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.o \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.o \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.o \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.o 

C_DEPS += \
./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.d \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.d \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.d \
./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/radio/lr1110_modem/src/%.o Core/Src/radio/lr1110_modem/src/%.su Core/Src/radio/lr1110_modem/src/%.cyclo: ../Core/Src/radio/lr1110_modem/src/%.c Core/Src/radio/lr1110_modem/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Core/Inc/boards -I../Core/Inc/smtc_hal -I../Core/Src -I../Core/Src/radio -I../Core/Src/radio/lr1110_modem/src -I../Drivers/BSP/Components/Leds -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-radio-2f-lr1110_modem-2f-src

clean-Core-2f-Src-2f-radio-2f-lr1110_modem-2f-src:
	-$(RM) ./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.cyclo ./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.d ./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.o ./Core/Src/radio/lr1110_modem/src/lr1110_bootloader.su ./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.cyclo ./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.d ./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.o ./Core/Src/radio/lr1110_modem/src/lr1110_modem_helper.su ./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.cyclo ./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.d ./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.o ./Core/Src/radio/lr1110_modem/src/lr1110_modem_lorawan.su ./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.cyclo ./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.d ./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.o ./Core/Src/radio/lr1110_modem/src/lr1110_modem_system.su

.PHONY: clean-Core-2f-Src-2f-radio-2f-lr1110_modem-2f-src

