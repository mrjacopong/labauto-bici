################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/back/main.c 

COMPILER_OBJS += \
src/back/main.obj 

C_DEPS += \
src/back/main.d 

# Each subdirectory must supply rules for building sources it contributes
src/back/%.obj: ../src/back/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\back\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\back\cDepSubCommand.tmp"
	ccrx -subcommand="src\back\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\back\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\back\cSubCommand.tmp"
	ccrx -subcommand="src\back\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

