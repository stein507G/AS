################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../src/hal/CortexM3/GNU/hal.S \
../src/hal/CortexM3/GNU/hw_reg_access.S 

OBJS += \
./src/hal/CortexM3/GNU/hal.o \
./src/hal/CortexM3/GNU/hw_reg_access.o 

S_UPPER_DEPS += \
./src/hal/CortexM3/GNU/hal.d \
./src/hal/CortexM3/GNU/hw_reg_access.d 


# Each subdirectory must supply rules for building sources it contributes
src/hal/CortexM3/GNU/%.o: ../src/hal/CortexM3/GNU/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -x assembler-with-cpp -DNDEBUG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


