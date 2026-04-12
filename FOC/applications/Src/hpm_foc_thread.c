//
// Created by 21184 on 2026/3/24.
//

#include "../Inc/hpm_foc_thread.h"
#include <rtthread.h>
#include <stdlib.h>
#include "../../drivers/Inc/hpm_pwm.h"
#include "../../drivers/Inc/hpm_encoder.h"
#include "../../foc/Inc/hpm_foc.h"

#define FOC_THREAD_PRIORITY         8
#define FOC_THREAD_STACK_SIZE       2048
#define FOC_THREAD_TIMESLICE        5

static rt_thread_t foc_thread = RT_NULL;

/* 模拟 FOC 运行线程 */
static void foc_thread_entry(void *parameter) {
    /* 1. 初始化外设 */
    hpm_vce2755_init();
    hpm_pwm_foc_init();

    /* 2. 初始化 FOC 算法 (极对数假设为 7, 母线电压 24.0V, 请根据实际电机修改) */
    hpm_foc_init(7, 24.0f);

    /* 3. 设置初始目标电流为 0 */
    hpm_foc_set_target(0.0f, 0.0f);

    rt_kprintf("FOC Thread and Peripherals Initialized.\n");

    while (1) {
        /* 
         * 在实际的电机控制中，这段代码应该放在硬件 PWM 触发的 ADC 采样的完成中断中执行，
         * 频率通常为 10kHz ~ 20kHz。
         * 这里我们暂且在 RTOS 线程中以 1ms (1kHz) 的周期运行，用于验证软件流程和 PWM 波形。
         * 相电流这里暂且模拟传入 0.0f。
         */
        float ia = 0.0f;
        float ib = 0.0f;
        float ic = 0.0f;

        /* 运行 FOC 单步计算并更新 PWM */
        hpm_foc_step(ia, ib, ic);

        /* 延时 1 毫秒 */
        rt_thread_mdelay(1);
    }
}

int foc_thread_init(void) {
    foc_thread = rt_thread_create("foc_task",
                                  foc_thread_entry, RT_NULL,
                                  FOC_THREAD_STACK_SIZE,
                                  FOC_THREAD_PRIORITY,
                                  FOC_THREAD_TIMESLICE);

    if (foc_thread != RT_NULL) {
        rt_thread_startup(foc_thread);
        return 0;
    }

    return -1;
}

/* 将该函数加入到自动初始化段，开机自动执行 */
INIT_APP_EXPORT(foc_thread_init);

/* 导出 MSH 命令，用于在终端测试设置目标 Iq (转矩电流) */
static void foc_set_iq(int argc, char **argv) {
    if (argc < 2) {
        rt_kprintf("Usage: foc_set_iq <target_iq>\n");
        return;
    }
    float target_iq = atof(argv[1]);
    hpm_foc_set_target(0.0f, target_iq);
    rt_kprintf("FOC Target Iq set to: %d.%03d A\n", (int) target_iq, (int) (target_iq * 1000) % 1000);
}

MSH_CMD_EXPORT(foc_set_iq, Set FOC Target Iq current);
