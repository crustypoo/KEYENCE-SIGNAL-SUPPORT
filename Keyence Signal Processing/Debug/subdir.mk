################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fft_helper.c \
../filter_helper.c \
../sig_process.c \
../sig_support.c 

OBJS += \
./fft_helper.o \
./filter_helper.o \
./sig_process.o \
./sig_support.o 

C_DEPS += \
./fft_helper.d \
./filter_helper.d \
./sig_process.d \
./sig_support.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


