################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../r_riic_rx600/src/r_riic_rx600.c \
../r_riic_rx600/src/r_riic_rx600_master.c 

COMPILER_OBJS += \
r_riic_rx600/src/r_riic_rx600.obj \
r_riic_rx600/src/r_riic_rx600_master.obj 

C_DEPS += \
r_riic_rx600/src/r_riic_rx600.d \
r_riic_rx600/src/r_riic_rx600_master.d 

# Each subdirectory must supply rules for building sources it contributes
r_riic_rx600/src/%.obj: ../r_riic_rx600/src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo r_riic_rx600\src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "r_riic_rx600\src\cDepSubCommand.tmp"
	ccrx -subcommand="r_riic_rx600\src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo r_riic_rx600\src\cSubCommand.tmp=
	@sed -e "s/^/ /" "r_riic_rx600\src\cSubCommand.tmp"
	ccrx -subcommand="r_riic_rx600\src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

