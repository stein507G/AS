################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/procotol.c \
../src/run_program.c \
../src/storage.c \
../src/system.c \
../src/timer.c \
../src/util.c 

OBJS += \
./src/main.o \
./src/procotol.o \
./src/run_program.o \
./src/storage.o \
./src/system.o \
./src/timer.o \
./src/util.o 

C_DEPS += \
./src/main.d \
./src/procotol.d \
./src/run_program.d \
./src/storage.d \
./src/system.d \
./src/timer.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DNDEBUG -I"C:\Microsemi\SoftConsole_v4.0\CMSIS\V4.3\Include" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\CMSIS" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\CMSIS\startup_gcc" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_nvm" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_sys_services" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_timer" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_uart" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers\mss_watchdog" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers_config" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\drivers_config\sys_config" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal\CortexM3" -I"J:\Home2021\ChroZen-LC\LC_AS\SoC\Ch_AS_SF2_FW\ChAS-git\SF2_hw_platform\src\hal\CortexM3\GNU" -std=gnu11 -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


