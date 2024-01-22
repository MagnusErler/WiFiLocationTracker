################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/Leds/leds.c 

OBJS += \
./Drivers/BSP/Components/Leds/leds.o 

C_DEPS += \
./Drivers/BSP/Components/Leds/leds.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/Leds/%.o Drivers/BSP/Components/Leds/%.su Drivers/BSP/Components/Leds/%.cyclo: ../Drivers/BSP/Components/Leds/%.c Drivers/BSP/Components/Leds/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Core/Inc/boards -I../Core/Inc/smtc_hal -I../Core/Src -I../Core/Src/radio -I../Core/Src/radio/lr1110_modem/src -I../Drivers/BSP/Components/Leds -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-Leds

clean-Drivers-2f-BSP-2f-Components-2f-Leds:
	-$(RM) ./Drivers/BSP/Components/Leds/leds.cyclo ./Drivers/BSP/Components/Leds/leds.d ./Drivers/BSP/Components/Leds/leds.o ./Drivers/BSP/Components/Leds/leds.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-Leds

