################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delay.c \
../src/gcode.c \
../src/gpio.c \
../src/i2c.c \
../src/main.c \
../src/planner.c \
../src/stepper.c \
../src/stm32f2xx_hal_msp.c \
../src/stm32f2xx_it.c \
../src/timers.c \
../src/usb.c \
../src/usbd_cdc_if.c \
../src/usbd_conf.c \
../src/usbd_desc.c 

OBJS += \
./src/delay.o \
./src/gcode.o \
./src/gpio.o \
./src/i2c.o \
./src/main.o \
./src/planner.o \
./src/stepper.o \
./src/stm32f2xx_hal_msp.o \
./src/stm32f2xx_it.o \
./src/timers.o \
./src/usb.o \
./src/usbd_cdc_if.o \
./src/usbd_conf.o \
./src/usbd_desc.o 

C_DEPS += \
./src/delay.d \
./src/gcode.d \
./src/gpio.d \
./src/i2c.d \
./src/main.d \
./src/planner.d \
./src/stepper.d \
./src/stm32f2xx_hal_msp.d \
./src/stm32f2xx_it.d \
./src/timers.d \
./src/usb.d \
./src/usbd_cdc_if.d \
./src/usbd_conf.d \
./src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DTRACE -DSTM32F205xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f2xx" -I"../system/include/cmsis/device" -I"../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


