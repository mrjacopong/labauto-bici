################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/eMPL/inv_mpu.c 

COMPILER_OBJS += \
src/eMPL/inv_mpu.obj 

C_DEPS += \
src/eMPL/inv_mpu.d 

# Each subdirectory must supply rules for building sources it contributes
src/eMPL/inv_mpu.obj: ../src/eMPL/inv_mpu.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\eMPL\inv_mpucDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\eMPL\inv_mpucDepSubCommand.tmp"
	ccrx -subcommand="src\eMPL\inv_mpucDepSubCommand.tmp" "$<"
	@echo src\eMPL\inv_mpucSubCommand.tmp=
	@sed -e "s/^/ /" "src\eMPL\inv_mpucSubCommand.tmp"
	ccrx -subcommand="src\eMPL\inv_mpucSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

