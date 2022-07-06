################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../raydio103/drivers/mcp23s17.c \
../raydio103/drivers/si5351.c \
../raydio103/drivers/st7920.c 

OBJS += \
./raydio103/drivers/mcp23s17.o \
./raydio103/drivers/si5351.o \
./raydio103/drivers/st7920.o 

C_DEPS += \
./raydio103/drivers/mcp23s17.d \
./raydio103/drivers/si5351.d \
./raydio103/drivers/st7920.d 


# Each subdirectory must supply rules for building sources it contributes
raydio103/drivers/%.o raydio103/drivers/%.su: ../raydio103/drivers/%.c raydio103/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM3 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/raydio103/raydio103/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/raydio103/raydio103/raydio103/drivers" -I"D:/raydio103/raydio103/raydio103/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-raydio103-2f-drivers

clean-raydio103-2f-drivers:
	-$(RM) ./raydio103/drivers/mcp23s17.d ./raydio103/drivers/mcp23s17.o ./raydio103/drivers/mcp23s17.su ./raydio103/drivers/si5351.d ./raydio103/drivers/si5351.o ./raydio103/drivers/si5351.su ./raydio103/drivers/st7920.d ./raydio103/drivers/st7920.o ./raydio103/drivers/st7920.su

.PHONY: clean-raydio103-2f-drivers

