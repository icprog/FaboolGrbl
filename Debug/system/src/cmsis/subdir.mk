################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_stm32f2xx.c 

S_UPPER_SRCS += \
../system/src/cmsis/startup_stm32f205xx.S 

OBJS += \
./system/src/cmsis/startup_stm32f205xx.o \
./system/src/cmsis/system_stm32f2xx.o 

S_UPPER_DEPS += \
./system/src/cmsis/startup_stm32f205xx.d 

C_DEPS += \
./system/src/cmsis/system_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DDEBUG -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DTRACE -DSTM32F205xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f2xx" -I"../system/include/cmsis/device" -I"../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DTRACE -DSTM32F205xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f2xx" -I"../system/include/cmsis/device" -I"../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


