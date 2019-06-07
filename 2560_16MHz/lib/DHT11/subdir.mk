################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/DHT11/dht11.cpp 

OBJS += \
./lib/DHT11/dht11.o 

CPP_DEPS += \
./lib/DHT11/dht11.d 


# Each subdirectory must supply rules for building sources it contributes
lib/DHT11/%.o: ../lib/DHT11/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\src" -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\variants\mega" -I"C:\Users\SkullKill\git\skaccessLocal\AlarmSystem\lib" -Wall -g2 -gstabs -O0 -ffunction-sections -fdata-sections -fno-exceptions -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


