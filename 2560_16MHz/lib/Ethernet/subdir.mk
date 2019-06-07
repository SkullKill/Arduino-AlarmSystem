################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/Ethernet/Dhcp.cpp \
../lib/Ethernet/Dns.cpp \
../lib/Ethernet/Ethernet.cpp \
../lib/Ethernet/EthernetClient.cpp \
../lib/Ethernet/EthernetServer.cpp \
../lib/Ethernet/EthernetUdp.cpp 

OBJS += \
./lib/Ethernet/Dhcp.o \
./lib/Ethernet/Dns.o \
./lib/Ethernet/Ethernet.o \
./lib/Ethernet/EthernetClient.o \
./lib/Ethernet/EthernetServer.o \
./lib/Ethernet/EthernetUdp.o 

CPP_DEPS += \
./lib/Ethernet/Dhcp.d \
./lib/Ethernet/Dns.d \
./lib/Ethernet/Ethernet.d \
./lib/Ethernet/EthernetClient.d \
./lib/Ethernet/EthernetServer.d \
./lib/Ethernet/EthernetUdp.d 


# Each subdirectory must supply rules for building sources it contributes
lib/Ethernet/%.o: ../lib/Ethernet/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\src" -I"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\variants\mega" -I"C:\Users\SkullKill\git\skaccessLocal\AlarmSystem\lib" -Wall -g2 -gstabs -O0 -ffunction-sections -fdata-sections -fno-exceptions -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


