################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CMSIS/system_m2sxxx.c 

OBJS += \
./src/CMSIS/system_m2sxxx.o 

C_DEPS += \
./src/CMSIS/system_m2sxxx.d 


# Each subdirectory must supply rules for building sources it contributes
src/CMSIS/%.o: ../src/CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__YI_CM_DEBUG__ -DMICROSEMI_STDIO_THRU_MMUART0 -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\CMSIS" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\CMSIS\startup_gcc" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers\mss_nvm" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers\mss_sys_services" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers\mss_timer" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers\mss_uart" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers\mss_watchdog" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers_config" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\drivers_config\sys_config" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\hal" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\hal\CortexM3" -I"D:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW-git\SoftConsole4-SF2\SF2_hw_platform\src\hal\CortexM3\GNU" -I"C:\Microsemi\SoftConsole_v4.0\CMSIS\V4.3\Include" -std=gnu11 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


