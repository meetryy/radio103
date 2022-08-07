################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../raydio103/src/adc.c \
../raydio103/src/agc.c \
../raydio103/src/audio.c \
../raydio103/src/dsp.c \
../raydio103/src/eeprom.c \
../raydio103/src/encoder.c \
../raydio103/src/fft.c \
../raydio103/src/fir.c \
../raydio103/src/gfx.c \
../raydio103/src/menu.c \
../raydio103/src/metrics.c \
../raydio103/src/radio.c \
../raydio103/src/rtc.c \
../raydio103/src/schedule.c \
../raydio103/src/tools.c 

OBJS += \
./raydio103/src/adc.o \
./raydio103/src/agc.o \
./raydio103/src/audio.o \
./raydio103/src/dsp.o \
./raydio103/src/eeprom.o \
./raydio103/src/encoder.o \
./raydio103/src/fft.o \
./raydio103/src/fir.o \
./raydio103/src/gfx.o \
./raydio103/src/menu.o \
./raydio103/src/metrics.o \
./raydio103/src/radio.o \
./raydio103/src/rtc.o \
./raydio103/src/schedule.o \
./raydio103/src/tools.o 

C_DEPS += \
./raydio103/src/adc.d \
./raydio103/src/agc.d \
./raydio103/src/audio.d \
./raydio103/src/dsp.d \
./raydio103/src/eeprom.d \
./raydio103/src/encoder.d \
./raydio103/src/fft.d \
./raydio103/src/fir.d \
./raydio103/src/gfx.d \
./raydio103/src/menu.d \
./raydio103/src/metrics.d \
./raydio103/src/radio.d \
./raydio103/src/rtc.d \
./raydio103/src/schedule.d \
./raydio103/src/tools.d 


# Each subdirectory must supply rules for building sources it contributes
raydio103/src/%.o raydio103/src/%.su: ../raydio103/src/%.c raydio103/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM3 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/raydio103/raydio103/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/raydio103/raydio103/raydio103/drivers" -I"D:/raydio103/raydio103/raydio103/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-raydio103-2f-src

clean-raydio103-2f-src:
	-$(RM) ./raydio103/src/adc.d ./raydio103/src/adc.o ./raydio103/src/adc.su ./raydio103/src/agc.d ./raydio103/src/agc.o ./raydio103/src/agc.su ./raydio103/src/audio.d ./raydio103/src/audio.o ./raydio103/src/audio.su ./raydio103/src/dsp.d ./raydio103/src/dsp.o ./raydio103/src/dsp.su ./raydio103/src/eeprom.d ./raydio103/src/eeprom.o ./raydio103/src/eeprom.su ./raydio103/src/encoder.d ./raydio103/src/encoder.o ./raydio103/src/encoder.su ./raydio103/src/fft.d ./raydio103/src/fft.o ./raydio103/src/fft.su ./raydio103/src/fir.d ./raydio103/src/fir.o ./raydio103/src/fir.su ./raydio103/src/gfx.d ./raydio103/src/gfx.o ./raydio103/src/gfx.su ./raydio103/src/menu.d ./raydio103/src/menu.o ./raydio103/src/menu.su ./raydio103/src/metrics.d ./raydio103/src/metrics.o ./raydio103/src/metrics.su ./raydio103/src/radio.d ./raydio103/src/radio.o ./raydio103/src/radio.su ./raydio103/src/rtc.d ./raydio103/src/rtc.o ./raydio103/src/rtc.su ./raydio103/src/schedule.d ./raydio103/src/schedule.o ./raydio103/src/schedule.su ./raydio103/src/tools.d ./raydio103/src/tools.o ./raydio103/src/tools.su

.PHONY: clean-raydio103-2f-src

