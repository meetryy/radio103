################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../raydio103/src/agc.c \
../raydio103/src/audio.c \
../raydio103/src/dsp.c \
../raydio103/src/fft.c \
../raydio103/src/fir.c 

OBJS += \
./raydio103/src/agc.o \
./raydio103/src/audio.o \
./raydio103/src/dsp.o \
./raydio103/src/fft.o \
./raydio103/src/fir.o 

C_DEPS += \
./raydio103/src/agc.d \
./raydio103/src/audio.d \
./raydio103/src/dsp.d \
./raydio103/src/fft.d \
./raydio103/src/fir.d 


# Each subdirectory must supply rules for building sources it contributes
raydio103/src/%.o raydio103/src/%.su: ../raydio103/src/%.c raydio103/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM3 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/raydio103/raydio103/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/raydio103/raydio103/raydio103/drivers" -I"D:/raydio103/raydio103/raydio103/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-raydio103-2f-src

clean-raydio103-2f-src:
	-$(RM) ./raydio103/src/agc.d ./raydio103/src/agc.o ./raydio103/src/agc.su ./raydio103/src/audio.d ./raydio103/src/audio.o ./raydio103/src/audio.su ./raydio103/src/dsp.d ./raydio103/src/dsp.o ./raydio103/src/dsp.su ./raydio103/src/fft.d ./raydio103/src/fft.o ./raydio103/src/fft.su ./raydio103/src/fir.d ./raydio103/src/fir.o ./raydio103/src/fir.su

.PHONY: clean-raydio103-2f-src

