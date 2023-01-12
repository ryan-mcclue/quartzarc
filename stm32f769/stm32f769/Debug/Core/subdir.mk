################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/syscalls.c \
../Core/sysmem.c \
../Core/system_stm32f7xx.c 

CPP_SRCS += \
../Core/boot.cpp \
../Core/handlers.cpp \
../Core/main.cpp 

C_DEPS += \
./Core/syscalls.d \
./Core/sysmem.d \
./Core/system_stm32f7xx.d 

OBJS += \
./Core/boot.o \
./Core/handlers.o \
./Core/main.o \
./Core/syscalls.o \
./Core/sysmem.o \
./Core/system_stm32f7xx.o 

CPP_DEPS += \
./Core/boot.d \
./Core/handlers.d \
./Core/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o Core/%.su: ../Core/%.cpp Core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/%.o Core/%.su: ../Core/%.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core

clean-Core:
	-$(RM) ./Core/boot.d ./Core/boot.o ./Core/boot.su ./Core/handlers.d ./Core/handlers.o ./Core/handlers.su ./Core/main.d ./Core/main.o ./Core/main.su ./Core/syscalls.d ./Core/syscalls.o ./Core/syscalls.su ./Core/sysmem.d ./Core/sysmem.o ./Core/sysmem.su ./Core/system_stm32f7xx.d ./Core/system_stm32f7xx.o ./Core/system_stm32f7xx.su

.PHONY: clean-Core

