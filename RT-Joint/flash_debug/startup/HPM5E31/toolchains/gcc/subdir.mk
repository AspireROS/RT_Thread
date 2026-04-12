################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/HPM5E31/toolchains/gcc/port_gcc.S \
../startup/HPM5E31/toolchains/gcc/start.S 

O_SRCS += \
../startup/HPM5E31/toolchains/gcc/port_gcc.o \
../startup/HPM5E31/toolchains/gcc/start.o 

OBJS += \
./startup/HPM5E31/toolchains/gcc/port_gcc.o \
./startup/HPM5E31/toolchains/gcc/start.o 

S_UPPER_DEPS += \
./startup/HPM5E31/toolchains/gcc/port_gcc.d \
./startup/HPM5E31/toolchains/gcc/start.d 


# Each subdirectory must supply rules for building sources it contributes
startup/HPM5E31/toolchains/gcc/%.o: ../startup/HPM5E31/toolchains/gcc/%.S
	riscv32-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-strict-align -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -gdwarf-2 -x assembler-with-cpp -I"C:\RT-ThreadStudio\workspace\111" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\soc\HPM5E00\ip" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\soc\HPM5E00\HPM5E31" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\libcpu\risc-v\common" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\arch" -I"C:\RT-ThreadStudio\workspace\111\startup\HPM5E31" -include"C:\RT-ThreadStudio\workspace\111\rtconfig_preinc.h" -ffunction-sections -fdata-sections -fno-common -mcmodel=medlow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

