################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/src/clock.c \
../rt-thread/src/components.c \
../rt-thread/src/cpu_up.c \
../rt-thread/src/defunct.c \
../rt-thread/src/idle.c \
../rt-thread/src/ipc.c \
../rt-thread/src/irq.c \
../rt-thread/src/kservice.c \
../rt-thread/src/mem.c \
../rt-thread/src/mempool.c \
../rt-thread/src/object.c \
../rt-thread/src/scheduler_comm.c \
../rt-thread/src/scheduler_up.c \
../rt-thread/src/thread.c \
../rt-thread/src/timer.c 

OBJS += \
./rt-thread/src/clock.o \
./rt-thread/src/components.o \
./rt-thread/src/cpu_up.o \
./rt-thread/src/defunct.o \
./rt-thread/src/idle.o \
./rt-thread/src/ipc.o \
./rt-thread/src/irq.o \
./rt-thread/src/kservice.o \
./rt-thread/src/mem.o \
./rt-thread/src/mempool.o \
./rt-thread/src/object.o \
./rt-thread/src/scheduler_comm.o \
./rt-thread/src/scheduler_up.o \
./rt-thread/src/thread.o \
./rt-thread/src/timer.o 

C_DEPS += \
./rt-thread/src/clock.d \
./rt-thread/src/components.d \
./rt-thread/src/cpu_up.d \
./rt-thread/src/defunct.d \
./rt-thread/src/idle.d \
./rt-thread/src/ipc.d \
./rt-thread/src/irq.d \
./rt-thread/src/kservice.d \
./rt-thread/src/mem.d \
./rt-thread/src/mempool.d \
./rt-thread/src/object.d \
./rt-thread/src/scheduler_comm.d \
./rt-thread/src/scheduler_up.d \
./rt-thread/src/thread.d \
./rt-thread/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/src/%.o: ../rt-thread/src/%.c
	riscv32-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-strict-align -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -gdwarf-2 -DDEBUG -DUSE_NONVECTOR_MODE=1 -DFLASH_XIP=1 -DCONFIG_NDEBUG_CONSOLE=1 -D__RT_KERNEL_SOURCE__ -I"C:\RT-ThreadStudio\workspace\111\applications\Src" -I"C:\RT-ThreadStudio\workspace\111\drivers\Inc" -I"C:\RT-ThreadStudio\workspace\111\drivers\Src" -I"C:\RT-ThreadStudio\workspace\111\applications\Inc" -I"C:\RT-ThreadStudio\workspace\111" -I"C:\RT-ThreadStudio\workspace\111\applications" -I"C:\RT-ThreadStudio\workspace\111\board" -I"C:\RT-ThreadStudio\workspace\111\libraries\drivers" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\arch\riscv\intc" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\arch\riscv\l1c" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\arch\riscv" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\arch" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\components\debug_console" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\components\dma_mgr" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\components\touch" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\components\usb" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\drivers\inc" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\soc\HPM5E00\HPM5E31\boot" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\soc\HPM5E00\HPM5E31" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\soc\HPM5E00\ip" -I"C:\RT-ThreadStudio\workspace\111\libraries\hpm_sdk\utils" -I"C:\RT-ThreadStudio\workspace\111\libraries\misc\rtt_hw_util" -I"C:\RT-ThreadStudio\workspace\111\libraries\misc\rtt_interrupt_util" -I"C:\RT-ThreadStudio\workspace\111\libraries\misc\rtt_os_tick" -I"C:\RT-ThreadStudio\workspace\111\libraries\misc\toolchain_stub" -I"C:\RT-ThreadStudio\workspace\111\libraries\third_party\RT-Thread-Insight\RT-Tunnel\CherryRB" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\drivers\phy" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\drivers\smp_call" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\compilers\common\include" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\posix\io\epoll" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\posix\io\eventfd" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\posix\io\poll" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\libc\posix\ipc" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\components\net\utest" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\111\rt-thread\libcpu\risc-v\common" -I"C:\RT-ThreadStudio\workspace\111\startup\HPM5E31" -include"C:\RT-ThreadStudio\workspace\111\rtconfig_preinc.h" -std=gnu11 -ffunction-sections -fdata-sections -fno-common   -mcmodel=medlow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

