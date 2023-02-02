################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Tools/QAT_FIFO.cpp 

OBJS += \
./QA_Tools/QAT_FIFO.o 

CPP_DEPS += \
./QA_Tools/QAT_FIFO.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Tools/%.o QA_Tools/%.su: ../QA_Tools/%.cpp QA_Tools/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core -I../QA_Drivers -I../QA_Tools -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Tools

clean-QA_Tools:
	-$(RM) ./QA_Tools/QAT_FIFO.d ./QA_Tools/QAT_FIFO.o ./QA_Tools/QAT_FIFO.su

.PHONY: clean-QA_Tools

