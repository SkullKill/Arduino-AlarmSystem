################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lib/Syslog/subdir.mk
-include lib/SPI/subdir.mk
-include lib/Ethernet/utility/subdir.mk
-include lib/Ethernet/subdir.mk
-include lib/DHT11/subdir.mk
-include lib/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AlarmSystem.lss \

FLASH_IMAGE += \
AlarmSystem.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: AlarmSystem.elf secondary-outputs

# Tool invocations
AlarmSystem.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"AlarmSystem.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,AlarmSystem.map,--cref -Wl,--gc-sections -L"C:\Users\SkullKill\git\skaccessLocal\ArduinoLibrary\2560_16MHz" -mmcu=atmega2560
	@echo 'Finished building target: $@'
	@echo ' '

AlarmSystem.lss: AlarmSystem.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AlarmSystem.elf  >"AlarmSystem.lss"
	@echo 'Finished building: $@'
	@echo ' '

AlarmSystem.hex: AlarmSystem.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex AlarmSystem.elf  "AlarmSystem.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AlarmSystem.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 AlarmSystem.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) AlarmSystem.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
