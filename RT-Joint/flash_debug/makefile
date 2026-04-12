################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/HPM5E31/toolchains/gcc/subdir.mk
-include startup/HPM5E31/subdir.mk
-include rt-thread/src/klibc/subdir.mk
-include rt-thread/src/subdir.mk
-include rt-thread/libcpu/risc-v/common/subdir.mk
-include rt-thread/components/libc/compilers/newlib/subdir.mk
-include rt-thread/components/libc/compilers/common/subdir.mk
-include rt-thread/components/finsh/subdir.mk
-include rt-thread/components/drivers/serial/subdir.mk
-include rt-thread/components/drivers/pin/subdir.mk
-include rt-thread/components/drivers/misc/subdir.mk
-include rt-thread/components/drivers/ipc/subdir.mk
-include rt-thread/components/drivers/core/subdir.mk
-include libraries/third_party/RT-Thread-Insight/RT-Tunnel/CherryRB/subdir.mk
-include libraries/misc/toolchain_stub/subdir.mk
-include libraries/misc/rtt_os_tick/subdir.mk
-include libraries/misc/rtt_interrupt_util/subdir.mk
-include libraries/misc/rtt_hw_util/subdir.mk
-include libraries/hpm_sdk/utils/subdir.mk
-include libraries/hpm_sdk/soc/HPM5E00/HPM5E31/boot/subdir.mk
-include libraries/hpm_sdk/soc/HPM5E00/HPM5E31/subdir.mk
-include libraries/hpm_sdk/drivers/src/subdir.mk
-include libraries/hpm_sdk/components/dma_mgr/subdir.mk
-include libraries/hpm_sdk/components/debug_console/subdir.mk
-include libraries/hpm_sdk/arch/riscv/l1c/subdir.mk
-include libraries/drivers/subdir.mk
-include foc/Src/subdir.mk
-include drivers/Src/subdir.mk
-include build/libraries/third_party/RT-Thread-Insight/RT-Tunnel/CherryRB/subdir.mk
-include build/libraries/misc/toolchain_stub/subdir.mk
-include build/libraries/misc/rtt_os_tick/subdir.mk
-include build/libraries/misc/rtt_interrupt_util/subdir.mk
-include build/libraries/misc/rtt_hw_util/subdir.mk
-include build/libraries/hpm_sdk/utils/subdir.mk
-include build/libraries/hpm_sdk/soc/HPM5E00/HPM5E31/boot/subdir.mk
-include build/libraries/hpm_sdk/soc/HPM5E00/HPM5E31/subdir.mk
-include build/libraries/hpm_sdk/drivers/src/subdir.mk
-include build/libraries/hpm_sdk/arch/riscv/l1c/subdir.mk
-include build/libraries/drivers/subdir.mk
-include build/kernel/src/klibc/subdir.mk
-include build/kernel/src/subdir.mk
-include build/kernel/libcpu/risc-v/common/subdir.mk
-include build/kernel/components/libc/compilers/newlib/subdir.mk
-include build/kernel/components/libc/compilers/common/subdir.mk
-include build/kernel/components/finsh/subdir.mk
-include build/kernel/components/drivers/serial/subdir.mk
-include build/kernel/components/drivers/pin/subdir.mk
-include build/kernel/components/drivers/ipc/subdir.mk
-include build/kernel/components/drivers/core/subdir.mk
-include build/board/subdir.mk
-include build/applications/subdir.mk
-include board/subdir.mk
-include applications/Src/subdir.mk
-include applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rtthread.bin \

SECONDARY_LIST += \
rtthread.lst \

SECONDARY_SIZE += \
rtthread.siz \


# 所有目标
all: rtthread.elf secondary-outputs

# 工具调用
rtthread.elf: $(OBJS) $(USER_OBJS)
	riscv32-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-strict-align -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -gdwarf-2 -T "C:\RT-ThreadStudio\workspace\111\board\linker_scripts\gcc\flash_rtt.ld" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker -z max-page-size=0x8 -Wl,-Map,"rtthread.map" --specs=nano.specs -u _printf_float -u _scanf_float --specs=nosys.specs -nostartfiles -Wl,--gc-sections -Wl,-cref,-Map=rtthread.map -mcmodel=medlow -o "rtthread.elf" $(OBJS) $(USER_OBJS) $(LIBS)

rtthread.bin: rtthread.elf
	riscv32-unknown-elf-objcopy -O binary "rtthread.elf"  "rtthread.bin"

rtthread.lst: rtthread.elf
	riscv32-unknown-elf-objdump --source --all-headers --demangle --line-numbers --wide "rtthread.elf" > "rtthread.lst"

rtthread.siz: rtthread.elf
	riscv32-unknown-elf-size --format=berkeley "rtthread.elf"

# 其他目标
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) rtthread.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
