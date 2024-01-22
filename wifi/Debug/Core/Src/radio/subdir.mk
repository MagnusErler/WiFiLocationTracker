################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/radio/lr1110.c 

OBJS += \
./Core/Src/radio/lr1110.o 

C_DEPS += \
./Core/Src/radio/lr1110.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/radio/%.o Core/Src/radio/%.su Core/Src/radio/%.cyclo: ../Core/Src/radio/%.c Core/Src/radio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Core/Inc/boards -I../Core/Inc/smtc_hal -I../Core/Src -I../Core/Src/radio -I../Core/Src/radio/lr1110_modem/src -I../Drivers/BSP/Components/Leds -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-radio

clean-Core-2f-Src-2f-radio:
	-$(RM) ./Core/Src/radio/lr1110.cyclo ./Core/Src/radio/lr1110.d ./Core/Src/radio/lr1110.o ./Core/Src/radio/lr1110.su

.PHONY: clean-Core-2f-Src-2f-radio

