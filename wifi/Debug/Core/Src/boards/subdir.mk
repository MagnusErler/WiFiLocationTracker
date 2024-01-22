################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/boards/lr1110_modem_board.c 

OBJS += \
./Core/Src/boards/lr1110_modem_board.o 

C_DEPS += \
./Core/Src/boards/lr1110_modem_board.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/boards/%.o Core/Src/boards/%.su Core/Src/boards/%.cyclo: ../Core/Src/boards/%.c Core/Src/boards/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Core/Inc/boards -I../Core/Inc/smtc_hal -I../Core/Src -I../Core/Src/radio -I../Core/Src/radio/lr1110_modem/src -I../Drivers/BSP/Components/Leds -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-boards

clean-Core-2f-Src-2f-boards:
	-$(RM) ./Core/Src/boards/lr1110_modem_board.cyclo ./Core/Src/boards/lr1110_modem_board.d ./Core/Src/boards/lr1110_modem_board.o ./Core/Src/boards/lr1110_modem_board.su

.PHONY: clean-Core-2f-Src-2f-boards

