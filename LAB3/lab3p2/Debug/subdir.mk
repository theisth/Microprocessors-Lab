################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/90533/Desktop/stm32g0/include/startup_stm32g031k8tx.s 

C_SRCS += \
../main.c \
C:/Users/90533/Desktop/stm32g0/include/system_stm32g0xx.c 

OBJS += \
./main.o \
./startup_stm32g031k8tx.o \
./system_stm32g0xx.o 

S_DEPS += \
./startup_stm32g031k8tx.d 

C_DEPS += \
./main.d \
./system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
%.o %.su %.cyclo: ../%.c subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g -DSTM32 -DSTM32G031xx -DSTM32G0 -DNUCLEO_G031K8 -DSTM32G031K8Tx -DDEBUG -c -I../../include -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
startup_stm32g031k8tx.o: C:/Users/90533/Desktop/stm32g0/include/startup_stm32g031k8tx.s subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"
system_stm32g0xx.o: C:/Users/90533/Desktop/stm32g0/include/system_stm32g0xx.c subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g -DSTM32 -DSTM32G031xx -DSTM32G0 -DNUCLEO_G031K8 -DSTM32G031K8Tx -DDEBUG -c -I../../include -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean--2e-

clean--2e-:
	-$(RM) ./main.cyclo ./main.d ./main.o ./main.su ./startup_stm32g031k8tx.d ./startup_stm32g031k8tx.o ./system_stm32g0xx.cyclo ./system_stm32g0xx.d ./system_stm32g0xx.o ./system_stm32g0xx.su

.PHONY: clean--2e-

