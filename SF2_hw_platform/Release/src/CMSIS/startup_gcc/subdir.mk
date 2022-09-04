################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CMSIS/startup_gcc/newlib_stubs.c 

S_UPPER_SRCS += \
../src/CMSIS/startup_gcc/startup_m2sxxx.S 

OBJS += \
./src/CMSIS/startup_gcc/newlib_stubs.o \
./src/CMSIS/startup_gcc/startup_m2sxxx.o 

S_UPPER_DEPS += \
./src/CMSIS/startup_gcc/startup_m2sxxx.d 

C_DEPS += \
./src/CMSIS/startup_gcc/newlib_stubs.d 


# Each subdirectory must supply rules for building sources it contributes
src/CMSIS/startup_gcc/%.o: ../src/CMSIS/startup_gcc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DNDEBUG -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\CMSIS" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\CMSIS\startup_gcc" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_nvm" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_sys_services" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_timer" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_uart" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_watchdog" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers_config" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers_config\sys_config" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal\CortexM3" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal\CortexM3\GNU" -I"C:\Microsemi\SoftConsole_v4.0\CMSIS\V4.3\Include" -std=gnu11 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/CMSIS/startup_gcc/%.o: ../src/CMSIS/startup_gcc/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -x assembler-with-cpp -DNDEBUG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


