################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Drivers/QAD_GPIO.cpp 

OBJS += \
./QA_Drivers/QAD_GPIO.o 

CPP_DEPS += \
./QA_Drivers/QAD_GPIO.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Drivers/%.o QA_Drivers/%.su: ../QA_Drivers/%.cpp QA_Drivers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core -I../QA_Drivers -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Drivers

clean-QA_Drivers:
	-$(RM) ./QA_Drivers/QAD_GPIO.d ./QA_Drivers/QAD_GPIO.o ./QA_Drivers/QAD_GPIO.su

.PHONY: clean-QA_Drivers

