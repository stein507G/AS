################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/mss_nvm/mss_nvm.c 

OBJS += \
./src/drivers/mss_nvm/mss_nvm.o 

C_DEPS += \
./src/drivers/mss_nvm/mss_nvm.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/mss_nvm/%.o: ../src/drivers/mss_nvm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DNDEBUG -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\CMSIS" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\CMSIS\startup_gcc" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers\mss_nvm" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers\mss_sys_services" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers\mss_timer" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers\mss_uart" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers\mss_watchdog" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers_config" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\drivers_config\sys_config" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\hal" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\hal\CortexM3" -I"J:\Ryu2022\ChrozenLC\AS\SoC\Ch_AS_SF2_FW\ChAS-git-S\SF2_hw_platform\src\hal\CortexM3\GNU" -I"C:\Microsemi\SoftConsole_v4.0\CMSIS\V4.3\Include" -std=gnu11 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


