CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR hpmicro)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "C:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/RISC-V/RISC-V-GCC-RV32/2022-04-12/bin/riscv32-unknown-elf-gcc.exe")
SET(CMAKE_ASM_COMPILER "C:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/RISC-V/RISC-V-GCC-RV32/2022-04-12/bin/riscv32-unknown-elf-gcc.exe")
SET(CMAKE_C_FLAGS " -mcmodel=medlow -march=rv32imac -mabi=ilp32  -DUSE_NONVECTOR_MODE=1  -DCONFIG_NDEBUG_CONSOLE=1  -ffunction-sections -fdata-sections -fno-common -fno-omit-frame-pointer -gdwarf-2 -Og")
SET(CMAKE_ASM_FLAGS " -mcmodel=medlow -march=rv32imac -mabi=ilp32  -DUSE_NONVECTOR_MODE=1  -DCONFIG_NDEBUG_CONSOLE=1  -ffunction-sections -fdata-sections -fno-common -fno-omit-frame-pointer -gdwarf-2 -I C:/RT-ThreadStudio/workspace/FOC/rt-thread/libcpu/risc-v/common")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "C:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/RISC-V/RISC-V-GCC-RV32/2022-04-12/bin/riscv32-unknown-elf-g++.exe")
SET(CMAKE_CXX_FLAGS " -mcmodel=medlow -march=rv32imac -mabi=ilp32  -DUSE_NONVECTOR_MODE=1  -DCONFIG_NDEBUG_CONSOLE=1  -ffunction-sections -fdata-sections -fno-common -fno-omit-frame-pointer -Wno-incompatible-pointer-types  -fomit-frame-pointer  -gdwarf-2 -Og -DFLASH_XIP=1 -Woverloaded-virtual -fno-exceptions -fno-rtti ")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "C:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/RISC-V/RISC-V-GCC-RV32/2022-04-12/bin/riscv32-unknown-elf-objcopy.exe")
SET(CMAKE_SIZE "C:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/RISC-V/RISC-V-GCC-RV32/2022-04-12/bin/riscv32-unknown-elf-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcmodel=medlow -march=rv32imac -mabi=ilp32   --specs=nano.specs --specs=nosys.specs  -u _printf_float -u _scanf_float -nostartfiles -Wl,-Map=rtthread.map,--gc-sections,-print-memory-usage,-z,max-page-size=0x8  -Og -T ${CMAKE_SOURCE_DIR}/board/linker_scripts/gcc/flash_rtt.ld -Wl,--gc-sections,--print-memory-usage")

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)

PROJECT(rtthread C CXX ASM)

INCLUDE_DIRECTORIES(
	.
	applications
	board
	libraries/drivers
	libraries/hpm_sdk/arch
	libraries/hpm_sdk/arch/riscv
	libraries/hpm_sdk/arch/riscv/l1c
	libraries/hpm_sdk/arch/riscv/intc
	libraries/hpm_sdk/components/debug_console
	libraries/hpm_sdk/components/touch
	libraries/hpm_sdk/components/usb
	libraries/hpm_sdk/components/dma_mgr
	libraries/hpm_sdk/drivers/inc
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/boot
	libraries/hpm_sdk/soc/HPM5E00/ip
	libraries/hpm_sdk/utils
	libraries/misc/rtt_hw_util
	libraries/misc/rtt_interrupt_util
	libraries/misc/rtt_os_tick
	libraries/misc/toolchain_stub
	libraries/third_party/RT-Thread-Insight/RT-Tunnel/CherryRB
	startup/HPM5E31
	rt-thread/include
	rt-thread/libcpu/risc-v/common
	rt-thread/components/drivers/include
	rt-thread/components/drivers/phy
	rt-thread/components/drivers/smp_call
	rt-thread/components/drivers/spi
	rt-thread/components/finsh
	rt-thread/components/libc/compilers/common/include
	rt-thread/components/libc/compilers/newlib
	rt-thread/components/libc/posix/io/epoll
	rt-thread/components/libc/posix/io/eventfd
	rt-thread/components/libc/posix/io/poll
	rt-thread/components/libc/posix/ipc
	rt-thread/components/net/utest
)

ADD_DEFINITIONS(
	-D_REENT_SMALL
	-DD45
	-DHPM5E31
	-DHPM_SDK_L1C_NO_REENTRANT=1
	-DUSE_DMA_DECLARE_EXT_ISR_M=1
	-DHPMSOC_HAS_HPMSDK_DMAV2
	-DHPMSOC_HAS_HPMSDK_GPTMR
	-DHPMSOC_HAS_HPMSDK_I2C
	-DHPM_SOC_ISR_NUM=80
	-DRT_HW_ISR_NUM=HPM_SOC_ISR_NUM
	-D__RTTHREAD__
	-DRT_USING_NEWLIBC
	-DRT_USING_LIBC
	-D_POSIX_C_SOURCE=1
)

# Library source files
SET(RT_APPLICATIONS_SOURCES
	applications/main.c
		drivers/Inc/hpm_adc.h
		drivers/Src/hpm_adc.c
		drivers/Inc/hpm_can.h
		drivers/Src/hpm_can.c
		drivers/Inc/hpm_pwm.h
		drivers/Src/hpm_pwm.c
		foc/Inc/hpm_foc.h
		foc/Src/hpm_foc.c
		foc/Inc/hpm_clarke.h
		foc/Src/hpm_clarke.c
		foc/Inc/hpm_park.h
		foc/Src/hpm_park.c
		foc/Inc/hpm_svpwm.h
		foc/Src/hpm_svpwm.c
		foc/Inc/hpm_pid.h
		foc/Src/hpm_pid.c
		foc/Inc/hpm_motor.h
		foc/Src/hpm_motor.c
		applications/Inc/hpm_foc_thread.h
		applications/Src/hpm_foc_thread.c
		applications/Inc/hpm_command_thread.h
		applications/Src/hpm_command_thread.c
		drivers/Inc/hpm_encoder.h
		drivers/Src/hpm_encoder.c
		drivers/Inc/hpm_rs485.h
		drivers/Src/hpm_rs485.c
		drivers/Inc/hpm_esc.h
		drivers/Src/hpm_protect.c
		drivers/Inc/hpm_protect.h
		drivers/Src/hpm_esc.c
)

SET(RT_BOARD_SOURCES
	board/board.c
	board/fal_flash_port.c
	board/rtt_board.c
	board/pinmux.c
)

SET(RT_CHERRYRB_SOURCES
	libraries/third_party/RT-Thread-Insight/RT-Tunnel/CherryRB/chry_ringbuffer.c
)

SET(RT_COMPILER_SOURCES
	rt-thread/components/libc/compilers/common/cctype.c
	rt-thread/components/libc/compilers/common/cstdlib.c
	rt-thread/components/libc/compilers/common/cstring.c
	rt-thread/components/libc/compilers/common/ctime.c
	rt-thread/components/libc/compilers/common/cunistd.c
	rt-thread/components/libc/compilers/common/cwchar.c
	rt-thread/components/libc/compilers/newlib/syscalls.c
)

SET(RT_DEVICEDRIVERS_SOURCES
	rt-thread/components/drivers/core/device.c
	rt-thread/components/drivers/ipc/completion_comm.c
	rt-thread/components/drivers/ipc/completion_up.c
	rt-thread/components/drivers/ipc/condvar.c
	rt-thread/components/drivers/ipc/dataqueue.c
	rt-thread/components/drivers/ipc/pipe.c
	rt-thread/components/drivers/ipc/ringblk_buf.c
	rt-thread/components/drivers/ipc/ringbuffer.c
	rt-thread/components/drivers/ipc/waitqueue.c
	rt-thread/components/drivers/ipc/workqueue.c
	rt-thread/components/drivers/misc/adc.c
	rt-thread/components/drivers/pin/dev_pin.c
	rt-thread/components/drivers/rtc/dev_rtc.c
	rt-thread/components/drivers/serial/dev_serial_v2.c
	rt-thread/components/drivers/spi/dev_spi.c
	rt-thread/components/drivers/spi/dev_spi_core.c
)

SET(RT_DRIVERS_SOURCES
	libraries/drivers/drv_adc.c
	libraries/drivers/drv_gpio.c
	libraries/drivers/drv_uart_v2.c
)

SET(RT_FINSH_SOURCES
	rt-thread/components/finsh/msh.c
	rt-thread/components/finsh/cmd.c
	rt-thread/components/finsh/shell.c
	rt-thread/components/finsh/msh_parse.c
)

SET(RT_KERNEL_SOURCES
	rt-thread/src/clock.c
	rt-thread/src/components.c
	rt-thread/src/cpu_up.c
	rt-thread/src/defunct.c
	rt-thread/src/idle.c
	rt-thread/src/ipc.c
	rt-thread/src/irq.c
	rt-thread/src/kservice.c
	rt-thread/src/mem.c
	rt-thread/src/mempool.c
	rt-thread/src/object.c
	rt-thread/src/scheduler_comm.c
	rt-thread/src/scheduler_up.c
	rt-thread/src/thread.c
	rt-thread/src/timer.c
)

SET(RT_KLIBC_SOURCES
	rt-thread/src/klibc/kstdio.c
	rt-thread/src/klibc/rt_vsscanf.c
	rt-thread/src/klibc/rt_vsnprintf_tiny.c
	rt-thread/src/klibc/kstring.c
	rt-thread/src/klibc/kerrno.c
)

SET(RT_LIBCPU_SOURCES
	rt-thread/libcpu/risc-v/common/cpuport.c
	rt-thread/libcpu/risc-v/common/trap_common.c
	rt-thread/libcpu/risc-v/common/interrupt_gcc.S
	rt-thread/libcpu/risc-v/common/context_gcc.S
)

SET(RT_LIBRARIES_SOURCES
	libraries/hpm_sdk/arch/riscv/l1c/hpm_l1c_drv.c
	libraries/hpm_sdk/components/debug_console/hpm_debug_console.c
	libraries/hpm_sdk/components/dma_mgr/hpm_dma_mgr.c
	libraries/hpm_sdk/drivers/src/hpm_adc16_drv.c
	libraries/hpm_sdk/drivers/src/hpm_dmav2_drv.c
	libraries/hpm_sdk/drivers/src/hpm_gpio_drv.c
	libraries/hpm_sdk/drivers/src/hpm_gptmr_drv.c
	libraries/hpm_sdk/drivers/src/hpm_i2c_drv.c
	libraries/hpm_sdk/drivers/src/hpm_pcfg_drv.c
	libraries/hpm_sdk/drivers/src/hpm_pllctlv2_drv.c
	libraries/hpm_sdk/drivers/src/hpm_pmp_drv.c
	libraries/hpm_sdk/drivers/src/hpm_uart_drv.c
	libraries/hpm_sdk/drivers/src/hpm_spi_drv.c
	libraries/hpm_sdk/drivers/src/hpm_qeiv2_drv.c
	libraries/hpm_sdk/drivers/src/hpm_pwmv2_drv.c
	libraries/hpm_sdk/utils/hpm_sbrk.c
)

SET(RT_POSIX_SOURCES
)

SET(RT_RTT_HW_UTIL_SOURCES
	libraries/misc/rtt_hw_util/hpm_rtt_hw_util.c
)

SET(RT_RTT_INTERRUPT_UTIL_SOURCES
	libraries/misc/rtt_interrupt_util/hpm_rtt_interrupt_util.c
)

SET(RT_RTT_OS_TICK_SOURCES
	libraries/misc/rtt_os_tick/hpm_rtt_os_tick.c
)

SET(RT_SOC_SOURCES
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/hpm_otp_drv.c
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/boot/hpm_bootheader.c
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/hpm_sysctl_drv.c
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/system.c
	libraries/hpm_sdk/soc/HPM5E00/HPM5E31/hpm_clock_drv.c
)

SET(RT_STARTUP_SOURCES
	startup/HPM5E31/toolchains/gcc/start.S
	startup/HPM5E31/toolchains/gcc/port_gcc.S
	startup/HPM5E31/trap.c
	startup/HPM5E31/startup.c
)

SET(RT_TOOLCHAIN_STUB_SOURCES
	libraries/misc/toolchain_stub/toolchain_lowlevel_impl.c
)

SET(RT_UTC_UTEST_SOURCES
)

SET(RT_UTESTCASES_SOURCES
)

# Library search paths
# Library local macro definitions
SET(RT_DEVICEDRIVERS_DEFINES
	__RT_IPC_SOURCE__
)

SET(RT_KERNEL_DEFINES
	__RT_KERNEL_SOURCE__
)

# Library dependencies
SET(RT_COMPILER_LIBS
	c
	m
)

# Libraries
ADD_LIBRARY(rtt_Board OBJECT ${RT_BOARD_SOURCES})
ADD_LIBRARY(rtt_CherryRB OBJECT ${RT_CHERRYRB_SOURCES})
ADD_LIBRARY(rtt_Compiler OBJECT ${RT_COMPILER_SOURCES})
ADD_LIBRARY(rtt_DeviceDrivers OBJECT ${RT_DEVICEDRIVERS_SOURCES})
ADD_LIBRARY(rtt_Drivers OBJECT ${RT_DRIVERS_SOURCES})
ADD_LIBRARY(rtt_Finsh OBJECT ${RT_FINSH_SOURCES})
ADD_LIBRARY(rtt_Kernel OBJECT ${RT_KERNEL_SOURCES})
ADD_LIBRARY(rtt_klibc OBJECT ${RT_KLIBC_SOURCES})
ADD_LIBRARY(rtt_libcpu OBJECT ${RT_LIBCPU_SOURCES})
ADD_LIBRARY(rtt_Libraries OBJECT ${RT_LIBRARIES_SOURCES})
ADD_LIBRARY(rtt_rtt_hw_util OBJECT ${RT_RTT_HW_UTIL_SOURCES})
ADD_LIBRARY(rtt_rtt_interrupt_util OBJECT ${RT_RTT_INTERRUPT_UTIL_SOURCES})
ADD_LIBRARY(rtt_rtt_os_tick OBJECT ${RT_RTT_OS_TICK_SOURCES})
ADD_LIBRARY(rtt_SoC OBJECT ${RT_SOC_SOURCES})
ADD_LIBRARY(rtt_Startup OBJECT ${RT_STARTUP_SOURCES})
ADD_LIBRARY(rtt_toolchain_stub OBJECT ${RT_TOOLCHAIN_STUB_SOURCES})

# Interface libraries
ADD_LIBRARY(rtt_POSIX INTERFACE)
ADD_LIBRARY(rtt_utc_UTest INTERFACE)
ADD_LIBRARY(rtt_utestcases INTERFACE)

# Private macros
TARGET_COMPILE_DEFINITIONS(rtt_DeviceDrivers PRIVATE ${RT_DEVICEDRIVERS_DEFINES})
TARGET_COMPILE_DEFINITIONS(rtt_Kernel PRIVATE ${RT_KERNEL_DEFINES})

# Interface library search paths
TARGET_LINK_LIBRARIES(rtt_Compiler INTERFACE ${RT_COMPILER_LIBS})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${RT_APPLICATIONS_SOURCES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf
	rtt_Board
	rtt_CherryRB
	rtt_Compiler
	rtt_DeviceDrivers
	rtt_Drivers
	rtt_Finsh
	rtt_Kernel
	rtt_klibc
	rtt_libcpu
	rtt_Libraries
	rtt_rtt_hw_util
	rtt_rtt_interrupt_util
	rtt_rtt_os_tick
	rtt_SoC
	rtt_Startup
	rtt_toolchain_stub
	rtt_POSIX
	rtt_utc_UTest
	rtt_utestcases
)

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD 
	COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf rtthread.bin
	COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf
)

# if custom.cmake is exist, add it
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake)
endif()
