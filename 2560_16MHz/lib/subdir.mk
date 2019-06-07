################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/AlarmSub.cpp \
../lib/AlarmSystem.cpp \
../lib/HomeAuto.cpp \
../lib/Keypad.cpp \
../lib/Network.cpp \
../lib/Password.cpp \
../lib/Remote.cpp \
../lib/TimedSwitch.cpp 

OBJS += \
./lib/AlarmSub.o \
./lib/AlarmSystem.o \
./lib/HomeAuto.o \
./lib/Keypad.o \
./lib/Network.o \
./lib/Password.o \
./lib/Remote.o \
./lib/TimedSwitch.o 

CPP_DEPS += \
./lib/AlarmSub.d \
./lib/AlarmSystem.d \
./lib/HomeAuto.d \
./lib/Keypad.d \
./lib/Network.d \
./lib/Password.d \
./lib/Remote.d \
./lib/TimedSwitch.d 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o: ../lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\src" -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\variants\mega" -I"C:\Users\SkullKill\git\skaccessLocal\AlarmSystem\lib" -Wall -g2 -gstabs -O0 -ffunction-sections -fdata-sections -fno-exceptions -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


