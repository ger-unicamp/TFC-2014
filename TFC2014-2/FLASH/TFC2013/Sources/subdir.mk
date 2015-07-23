################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../TFC2013/Sources/camera.c" \

C_SRCS += \
../TFC2013/Sources/camera.c \

OBJS += \
./TFC2013/Sources/camera.o \

C_DEPS += \
./TFC2013/Sources/camera.d \

OBJS_QUOTED += \
"./TFC2013/Sources/camera.o" \

C_DEPS_QUOTED += \
"./TFC2013/Sources/camera.d" \

OBJS_OS_FORMAT += \
./TFC2013/Sources/camera.o \


# Each subdirectory must supply rules for building sources it contributes
TFC2013/Sources/camera.o: ../TFC2013/Sources/camera.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"TFC2013/Sources/camera.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"TFC2013/Sources/camera.o"
	@echo 'Finished building: $<'
	@echo ' '


