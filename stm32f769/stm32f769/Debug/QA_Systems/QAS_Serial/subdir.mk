################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.cpp 

OBJS += \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.o 

CPP_DEPS += \
./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Systems/QAS_Serial/%.o QA_Systems/QAS_Serial/%.su: ../QA_Systems/QAS_Serial/%.cpp QA_Systems/QAS_Serial/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core -I../QA_Drivers -I../QA_Tools -I../QA_Systems -I../QA_Systems/QAS_Serial -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Systems-2f-QAS_Serial

clean-QA_Systems-2f-QAS_Serial:
	-$(RM) ./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.d ./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.o ./QA_Systems/QAS_Serial/QAS_Serial_Dev_Base.su

.PHONY: clean-QA_Systems-2f-QAS_Serial
