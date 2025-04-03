################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Middlewares/USB_Device_Library/subdir.mk
-include Middlewares/LwIP/subdir.mk
-include Middlewares/FreeRTOS/subdir.mk
-include Drivers/Vendor/ETHERNET_IF/subdir.mk
-include Drivers/Vendor/Components/subdir.mk
-include Drivers/Vendor/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/Components/subdir.mk
-include Application/User/gui/subdir.mk
-include Application/User/generated/subdir.mk
-include Application/User/USB_DEVICE/Target/subdir.mk
-include Application/User/USB_DEVICE/App/subdir.mk
-include Application/User/TouchGFX/target/generated/subdir.mk
-include Application/User/TouchGFX/target/subdir.mk
-include Application/User/TouchGFX/App/subdir.mk
-include Application/User/LWIP/Target/subdir.mk
-include Application/User/LWIP/App/subdir.mk
-include Application/User/Core/subdir.mk
-include Application/Startup/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := EVK070016
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EVK070016.elf \

MAP_FILES += \
EVK070016.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EVK070016.list \

OBJCOPY_HEX += \
EVK070016.hex \


# All Target
all: main-build

# Main-build Target
main-build: EVK070016.elf secondary-outputs

# Tool invocations
EVK070016.elf EVK070016.map: $(OBJS) $(USER_OBJS) C:\TouchGFXProjects\Mess_Display_BI_08-291024\STM32CubeIDE\STM32H750_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "EVK070016.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\TouchGFXProjects\Mess_Display_BI_08-291024\STM32CubeIDE\STM32H750_FLASH.ld" --specs=nosys.specs -Wl,-Map="EVK070016.map" -Wl,--gc-sections -static -L../../Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc --specs=standard_c_nano_cpp.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EVK070016.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EVK070016.list"
	@echo 'Finished building: $@'
	@echo ' '

EVK070016.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "EVK070016.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) EVK070016.elf EVK070016.hex EVK070016.list EVK070016.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
