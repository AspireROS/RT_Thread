//
// Created by 21184 on 2026/3/17.
//

#include "../Inc/hpm_adc.h"
#include "hpm_adc16_drv.h"
#include "hpm_soc.h"
#include "hpm_trgm_drv.h"
#include "hpm_pwmv2_drv.h"
#include "hpm_trgmmux_src.h"
#include "hpm_misc.h"
#include "hpm_plic_drv.h"
#include "hpm_l1c_drv.h"

/* ADC Preemption Mode Result Buffer
 * 必须按照 L1 Cache Line 对齐，防止 Cache 一致性问题 (如果开启了 Cache)
 */
adc16_pmt_dma_data_t pmt_result_buff[ADC_SOC_PMT_MAX_DMA_BUFF_LEN_IN_4BYTES] __attribute__((aligned(HPM_L1C_CACHELINE_SIZE)));

/**
 * @brief ADC0 中断处理函数
 */
void isr_adc0(void) {
    uint32_t status = adc16_get_status_flags(HPM_ADC0);
    if (status & adc16_event_trig_complete) {
        /* 清除中断标志 */
        adc16_clear_status_flags(HPM_ADC0, adc16_event_trig_complete);

        /* 强制 Cache 失效，确保从系统内存读取到 DMA 搬运的最新数据 */
        if (l1c_dc_is_enabled()) {
            l1c_dc_invalidate((uint32_t)pmt_result_buff, sizeof(pmt_result_buff));
        }

        /*
         * 检查 cycle_bit 状态（每次转换后翻转）来确保这是新鲜数据
         * 结果 1: pmt_result_buff[0].result (需检查 pmt_result_buff[0].cycle_bit)
         * 结果 2: pmt_result_buff[1].result (需检查 pmt_result_buff[1].cycle_bit)
         */
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_ADC0, isr_adc0)

void hpm_adc_gpio_init(void) {
    /* Configure the GPIO pins as analog function */
    HPM_IOC->PAD[IOC_PAD_PF23].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
    HPM_IOC->PAD[IOC_PAD_PF24].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
    HPM_IOC->PAD[IOC_PAD_PF16].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
    HPM_IOC->PAD[IOC_PAD_PF26].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
    HPM_IOC->PAD[IOC_PAD_PF19].FUNC_CTL = IOC_PAD_FUNC_CTL_ANALOG_MASK;
}

void hpm_adc_pmt_init(uint8_t ch1, uint8_t ch2) {
    /* 1. 确保引脚配置为模拟输入 */
    hpm_adc_gpio_init();

    /* 2. 配置 ADC 为抢占模式 (Preemption Mode) */
    adc16_config_t cfg;
    adc16_get_default_config(&cfg);
    cfg.res = adc16_res_16_bits;
    cfg.conv_mode = adc16_conv_mode_preemption;
    cfg.sel_sync_ahb = true;
    cfg.adc_clk_div = adc16_clock_divider_4;
    cfg.adc_ahb_en = true;
    adc16_init(HPM_ADC0, &cfg);

    /* 3. 配置 PMT DMA 地址 */
    /* 将本地地址转换为系统地址，供 DMA 使用 */
    uint32_t dma_addr = core_local_mem_to_sys_address(BOARD_RUNNING_CORE, (uint32_t)pmt_result_buff);
    adc16_init_pmt_dma(HPM_ADC0, dma_addr);

    /* 4. 配置抢占触发队列 */
    adc16_pmt_config_t pmt_cfg = {0};
    pmt_cfg.trig_ch = 0;    /* 对应 PTRGI0A */
    pmt_cfg.trig_len = 2;   /* 瞬间连采双通道 */
    pmt_cfg.adc_ch[0] = ch1;
    pmt_cfg.adc_ch[1] = ch2;
    pmt_cfg.inten[0] = false;
    pmt_cfg.inten[1] = true; /* 第二个通道采样完成后触发中断 */

    adc16_set_pmt_config(HPM_ADC0, &pmt_cfg);

    /* 5. 使能抢占队列 */
    adc16_enable_pmt_queue(HPM_ADC0, 0);

    /* 6. 配置 TRGM: 将 PWM0 的触发输出 TRGO_0 路由到 ADC 的抢占触发 PTRGI0A */
    trgm_output_update_source(HPM_TRGM0, HPM_TRGM0_OUTPUT_SRC_ADCX_PTRGI0A, HPM_TRGM0_INPUT_SRC_PWM0_TRGO_0);

    /* 7. 使能 ADC 模块外设级中断 */
    adc16_enable_interrupts(HPM_ADC0, adc16_event_trig_complete);

    /* 8. 使能 PLIC 级 ADC 中断 */
    intc_m_enable_irq_with_priority(IRQn_ADC0, 1);
}

void foc_pwm_trigger_adc_init(uint8_t adc_channel) {
    /* 旧函数保留兼容性，或根据需要重构 */
    hpm_adc_pmt_init(adc_channel, adc_channel);
}
