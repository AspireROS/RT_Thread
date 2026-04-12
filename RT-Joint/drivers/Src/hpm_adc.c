/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */

#include <rtthread.h>
#include <rthw.h>  /* 引入 RT-Thread 硬件中断标准库 */
#include "./drivers/Inc/hpm_adc.h"

#include "hpm_adc16_drv.h"
#include "hpm_soc.h"
#include "hpm_trgm_drv.h"
#include "hpm_trgmmux_src.h"
#include "hpm_misc.h"
#include "hpm_plic_drv.h"
#include "hpm_l1c_drv.h"
#include "hpm_gpio_drv.h"

#include "./applications/Inc/hpm_text.h"

/* ADC Preemption Mode Result Buffer */
adc16_pmt_dma_data_t pmt_result_buff[ADC_SOC_PMT_MAX_DMA_BUFF_LEN_IN_4BYTES] __attribute__((aligned(
    HPM_L1C_CACHELINE_SIZE)));

static float offset_u = 0.0f;
static float offset_v = 0.0f;
static float offset_w = 0.0f;
static bool is_calibrated = false;

/* 提前声明动态中断服务函数 */
void isr_adc0(int irq, void *param);

static void hpm_adc_gpio_init(void) {

    HPM_IOC->PAD[IOC_PAD_PF23].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;

    HPM_IOC->PAD[IOC_PAD_PF24].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;

    HPM_IOC->PAD[IOC_PAD_PF27].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;

    HPM_IOC->PAD[IOC_PAD_PF26].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;

    HPM_IOC->PAD[IOC_PAD_PF18].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;

    HPM_IOC->PAD[IOC_PAD_PF22].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
}

void hpm_adc_init_for_foc(uint8_t ch_u, uint8_t ch_v, uint8_t ch_w) {
    hpm_adc_gpio_init();
    rt_kprintf("hpm_adc_gpio_init\n");
    adc16_config_t cfg;
    adc16_get_default_config(&cfg);
    rt_kprintf("adc16_get_default_config\n");
    cfg.res = adc16_res_16_bits;
    cfg.conv_mode = adc16_conv_mode_preemption;
    cfg.sel_sync_ahb = true;
    cfg.adc_clk_div = adc16_clock_divider_4;
    cfg.adc_ahb_en = true;
    rt_kprintf("adc16_init_begin\n");
    adc16_init(HPM_ADC0, &cfg);
    rt_kprintf("adc16_init\n");
    uint32_t dma_addr = core_local_mem_to_sys_address(BOARD_RUNNING_CORE, (uint32_t) pmt_result_buff);
    rt_kprintf("ADC DMA init\n");
    adc16_init_pmt_dma(HPM_ADC0, dma_addr);
    rt_kprintf("adc16_init_pmt_dma\n");
    adc16_pmt_config_t pmt_cfg = {0};
    pmt_cfg.trig_ch = 0;
    pmt_cfg.trig_len = 3;

    pmt_cfg.adc_ch[0] = ch_u;
    pmt_cfg.adc_ch[1] = ch_v;
    pmt_cfg.adc_ch[2] = ch_w;

    pmt_cfg.inten[0] = false;
    pmt_cfg.inten[1] = false;
    pmt_cfg.inten[2] = true;

    adc16_set_pmt_config(HPM_ADC0, &pmt_cfg);
    rt_kprintf("adc16_set_pmt_config\n");
    adc16_enable_pmt_queue(HPM_ADC0, 0);
    rt_kprintf("adc16_enable_pmt_queue\n");
    adc16_enable_interrupts(HPM_ADC0, adc16_event_trig_complete);
    rt_kprintf("adc16_enable_interrupts\n");
}

void hpm_adc_calibrate_offset(void) {
    uint32_t sum_u = 0, sum_v = 0, sum_w = 0;
    const uint16_t CALIBRATION_COUNT = 1000;
    uint32_t timeout;

    rt_kprintf("Starting ADC Current Offset Calibration...\n");

    for (int i = 0; i < CALIBRATION_COUNT; i++) {
        rt_kprintf("CALIBRATION_COUNT\n");
        adc16_trigger_pmt_by_sw(HPM_ADC0, 0); //卡这了
        rt_kprintf("adc16_trigger_pmt_by_sw\n");
        timeout = 100;
        while (!(adc16_get_status_flags(HPM_ADC0) & adc16_event_trig_complete) && timeout > 0) {
            timeout--;
            rt_kprintf("timeout: %d\n",timeout);
        }

        if (timeout == 0) {
            rt_kprintf("[ERROR] ADC Calibration Timeout!\n");
            break;
        }

        adc16_clear_status_flags(HPM_ADC0, adc16_event_trig_complete);
        rt_kprintf("adc16_clear_status_flags\n");
        if (l1c_dc_is_enabled()) {
            l1c_dc_invalidate((uint32_t) pmt_result_buff, sizeof(pmt_result_buff));
        }

        sum_u += pmt_result_buff[0].result;
        sum_v += pmt_result_buff[1].result;
        sum_w += pmt_result_buff[2].result;

        rt_thread_mdelay(20);
    }

    offset_u = (float) sum_u / CALIBRATION_COUNT;
    offset_v = (float) sum_v / CALIBRATION_COUNT;
    offset_w = (float) sum_w / CALIBRATION_COUNT;
    is_calibrated = true;

    rt_kprintf("ADC Calibration Done! Offset U:%d, V:%d, W:%d\n",
               (int) offset_u, (int) offset_v, (int) offset_w);

    /* ==========================================================
     * 硬件接通与 RT-Thread 标准动态中断注册
     * ========================================================== */
    trgm_output_update_source(HPM_TRGM0, HPM_TRGM0_OUTPUT_SRC_ADCX_PTRGI0A, HPM_TRGM0_INPUT_SRC_PWM0_TRGO_0);
    adc16_clear_status_flags(HPM_ADC0, 0xFFFFFFFF);

    /* 1. 使用操作系统标准的动态注册接口挂载中断 */
    rt_hw_interrupt_install(IRQn_ADC0, isr_adc0, RT_NULL, "adc0_isr");

    /* 2. 直接使用底层 PLIC 函数开启中断并赋予最高优先级 1 (已删除报错的 umask) */
    intc_m_enable_irq_with_priority(IRQn_ADC0, 1);
}

void hpm_adc_get_phase_currents(foc_phase_current_t *currents) {
    if (!is_calibrated) {
        currents->Iu = 0.0f;
        currents->Iv = 0.0f;
        currents->Iw = 0.0f;
        return;
    }
    currents->Iu = ((float) pmt_result_buff[0].result - offset_u) * ADC_TO_CURRENT_FACTOR;
    currents->Iv = ((float) pmt_result_buff[1].result - offset_v) * ADC_TO_CURRENT_FACTOR;
    currents->Iw = ((float) pmt_result_buff[2].result - offset_w) * ADC_TO_CURRENT_FACTOR;
}

/* 动态注册要求的中断函数标准签名 (带 irq 和 param) */
void isr_adc0(int irq, void *param) {
    rt_kprintf("ADC Calibration Done!\n");
    uint32_t status = adc16_get_status_flags(HPM_ADC0);

    if (status & adc16_event_trig_complete) {
        adc16_clear_status_flags(HPM_ADC0, status);

        /* 采用你自定义的引脚进行翻转 */
        gpio_toggle_pin(HPM_GPIO0, GPIO_DI_GPIOD, 6);

        if (l1c_dc_is_enabled()) {
            l1c_dc_invalidate((uint32_t) pmt_result_buff, sizeof(pmt_result_buff));
        }

        if (is_calibrated) {
            foc_phase_current_t phase_i;
            hpm_adc_get_phase_currents(&phase_i);
        }
    }
}

#include "../../libraries/hpm_sdk/drivers/src/hpm_adc16_drv.c"
