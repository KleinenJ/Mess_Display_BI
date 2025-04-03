################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/Startup/startup_stm32h750xbhx.s 

C_SRCS += \
../Application/Startup/syscalls.c \
../Application/Startup/sysmem.c 

S_DEPS += \
./Application/Startup/startup_stm32h750xbhx.d 

C_DEPS += \
./Application/Startup/syscalls.d \
./Application/Startup/sysmem.d 

OBJS += \
./Application/Startup/startup_stm32h750xbhx.o \
./Application/Startup/syscalls.o \
./Application/Startup/sysmem.o 


# Each subdirectory must supply rules for building sources it contributes
Application/Startup/%.o: ../Application/Startup/%.s Application/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -DUSE_BPP=16 -c -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I../../FATFS -I../../Middlewares/Third_Party/FatFs -I../../EEPROM/include -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=standard_c_nano_cpp.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Application/Startup/%.o Application/Startup/%.su Application/Startup/%.cyclo: ../Application/Startup/%.c Application/Startup/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_USB_FS -DUSE_BPP=16 -c -I../../LWIP/App -I../../TouchGFX/target -I../../USB_DEVICE/Target -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../Core/Inc -I../../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../LWIP/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Drivers/BSP/Components/lan8742 -I../Utilities/JPEG -I../Drivers/CMSIS/Include -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../TouchGFX/App -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/netif -I../../Core/Inc -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/system -I../../USB_DEVICE/App -I../../TouchGFX/App -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../TouchGFX/target/generated -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../TouchGFX/target/generated -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../TouchGFX/target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/Vendor/Board/SE21159/Config -I../../Drivers/Vendor/Device -I../../Drivers/Vendor/Driver -I../../ETHERNET_IF/App/tcpecho -I../../ETHERNET_IF/Target -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I../../FATFS/include -I../../EEPROM/include -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=standard_c_nano_cpp.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Startup

clean-Application-2f-Startup:
	-$(RM) ./Application/Startup/startup_stm32h750xbhx.d ./Application/Startup/startup_stm32h750xbhx.o ./Application/Startup/syscalls.cyclo ./Application/Startup/syscalls.d ./Application/Startup/syscalls.o ./Application/Startup/syscalls.su ./Application/Startup/sysmem.cyclo ./Application/Startup/sysmem.d ./Application/Startup/sysmem.o ./Application/Startup/sysmem.su

.PHONY: clean-Application-2f-Startup

