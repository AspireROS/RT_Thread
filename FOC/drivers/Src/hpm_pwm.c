#include "../Inc/hpm_pwm.h"
#include <rtthread.h>
#include "hpm_gpio_drv.h"
#include "hpm_pwmv2_drv.h"
#include "hpm_clock_drv.h"
#include "hpm_ioc_regs.h"

#define PWM_FREQUENCY (20000) // 20kHz 
#define DEAD_TIME_NS  (40)    // 40ns 死区时间

/* 外部声明时钟初始化函数 */
extern uint32_t rtt_board_init_pwm_clock(PWMV2_Type *ptr);

static uint32_t pwm_period = 0;

static void pwm_pin_init(void) {
    /* PWM0 P6, P7 (PD06, PD07) - 用于 U 相 */
    HPM_IOC->PAD[IOC_PAD_PD06].FUNC_CTL = IOC_PD06_FUNC_CTL_PWM0_P_6;
    HPM_IOC->PAD[IOC_PAD_PD07].FUNC_CTL = IOC_PD07_FUNC_CTL_PWM0_P_7;

    /* PWM1 P0, P1 (PC08, PC09) - 用于 V 相 */
    HPM_IOC->PAD[IOC_PAD_PC08].FUNC_CTL = IOC_PC08_FUNC_CTL_PWM1_P_0;
    HPM_IOC->PAD[IOC_PAD_PC09].FUNC_CTL = IOC_PC09_FUNC_CTL_PWM1_P_1;

    /* PWM1 P6, P7 (PC14, PC15) - 用于 W 相 */
    HPM_IOC->PAD[IOC_PAD_PC14].FUNC_CTL = IOC_PC14_FUNC_CTL_PWM1_P_6;
    HPM_IOC->PAD[IOC_PAD_PC15].FUNC_CTL = IOC_PC15_FUNC_CTL_PWM1_P_7;
}

static void configure_complementary_pwm_foc(PWMV2_Type *pwm_base, uint8_t channel_index, uint8_t shadow_index,
                                            uint32_t reload, uint32_t dead_time_ticks) {
    pwmv2_cmp_config_t cmp_config = {0};

    /* 1. 解锁影子寄存器 */
    pwmv2_shadow_register_unlock(pwm_base);

    /* 2. 设置周期 (使用影子寄存器0) */
    pwmv2_set_shadow_val(pwm_base, 0, reload, 0, false);

    /* 3. 设定占空比比较配置 */
    cmp_config.update_trigger = pwm_shadow_register_update_on_reload;
    cmp_config.cmp_source = cmp_value_from_shadow_val;
    cmp_config.cmp_source_index = shadow_index;
    cmp_config.cmp = reload / 2; // 默认 50% 占空比

    /* 设置 P 和 N 通道的比较值 (共享同一个 shadow index) */
    pwmv2_config_cmp(pwm_base, channel_index, &cmp_config);
    pwmv2_config_cmp(pwm_base, channel_index + 1, &cmp_config);

    /* 互补极性: P通道不反相, N通道反相 */
    pwmv2_disable_output_invert(pwm_base, (pwm_channel_t) channel_index);
    pwmv2_enable_output_invert(pwm_base, (pwm_channel_t) (channel_index + 1));

    /* 开启 Pair 模式 (实现 P/N 联动) */
    pwmv2_enable_pair_mode(pwm_base, (pwm_channel_t) channel_index);

    /* 设置死区 */
    pwmv2_set_dead_area(pwm_base, (pwm_channel_t) channel_index, dead_time_ticks);
    pwmv2_set_dead_area(pwm_base, (pwm_channel_t) (channel_index + 1), dead_time_ticks);

    /* 使能通道输出 */
    pwmv2_channel_enable_output(pwm_base, (pwm_channel_t) channel_index);
    pwmv2_channel_enable_output(pwm_base, (pwm_channel_t) (channel_index + 1));

    /* 4. 同步影子寄存器，使配置生效 */
    pwmv2_issue_shadow_register_lock_event(pwm_base);
}

void hpm_pwm_foc_init(void) {
    uint32_t pwm0_clk_freq;
    uint32_t pwm1_clk_freq;
    uint32_t dead_time_ticks;

    pwm_pin_init();

    /* --- 时钟和基础参数配置 --- */
    clock_add_to_group(clock_pwm0, 0);
    clock_add_to_group(clock_pwm1, 0);

    pwm0_clk_freq = rtt_board_init_pwm_clock(HPM_PWM0);
    pwm1_clk_freq = rtt_board_init_pwm_clock(HPM_PWM1);

    pwm_period = pwm0_clk_freq / PWM_FREQUENCY - 1;
    dead_time_ticks = (uint32_t) (((uint64_t) pwm0_clk_freq * DEAD_TIME_NS) / 1000000000ULL);

    /* --- PWM0 配置 (U 相) --- */
    pwmv2_disable_counter(HPM_PWM0, pwm_counter_0);
    pwmv2_reset_counter(HPM_PWM0, pwm_counter_0);
    // U 相: PWM0 CH6 & CH7 -> 映射到 PWM0 影子寄存器 1
    configure_complementary_pwm_foc(HPM_PWM0, 6, 1, pwm_period, dead_time_ticks);

    /* --- PWM1 配置 (V, W 相) --- */
    pwmv2_disable_counter(HPM_PWM1, pwm_counter_0);
    pwmv2_reset_counter(HPM_PWM1, pwm_counter_0);
    // V 相: PWM1 CH0 & CH1 -> 映射到 PWM1 影子寄存器 1
    configure_complementary_pwm_foc(HPM_PWM1, 0, 1, pwm_period, dead_time_ticks);
    // W 相: PWM1 CH6 & CH7 -> 映射到 PWM1 影子寄存器 2
    configure_complementary_pwm_foc(HPM_PWM1, 6, 2, pwm_period, dead_time_ticks);

    /* --- 启动计数器 (尽量同时启动以保证相位对齐) --- */
    pwmv2_start_pwm_output(HPM_PWM0, pwm_counter_0);
    pwmv2_enable_counter(HPM_PWM0, pwm_counter_0);

    pwmv2_start_pwm_output(HPM_PWM1, pwm_counter_0);
    pwmv2_enable_counter(HPM_PWM1, pwm_counter_0);
}

uint32_t hpm_pwm_get_period(void) {
    return pwm_period;
}

void hpm_pwm_set_duty(uint32_t cmp_a, uint32_t cmp_b, uint32_t cmp_c) {
    pwmv2_shadow_register_unlock(HPM_PWM0);
    pwmv2_shadow_register_unlock(HPM_PWM1);

    /* U相 (cmp_a): PWM0 影子寄存器 1 */
    pwmv2_set_shadow_val(HPM_PWM0, 1, cmp_a, 0, false);

    /* V相 (cmp_b): PWM1 影子寄存器 1 */
    pwmv2_set_shadow_val(HPM_PWM1, 1, cmp_b, 0, false);

    /* W相 (cmp_c): PWM1 影子寄存器 2 */
    pwmv2_set_shadow_val(HPM_PWM1, 2, cmp_c, 0, false);

    pwmv2_issue_shadow_register_lock_event(HPM_PWM0);
    pwmv2_issue_shadow_register_lock_event(HPM_PWM1);
}

void hpm_pwm_set_valley_trigger(void) {
    /* 谷底触发 (Valley Trigger)
     * 在 PWMV2 中，选择 Counter 0 的 Reload 点 (索引 24) 作为触发输出
     * 将 Reload 事件映射到 TRGO_0 (Trigger Out 0)
     */
    pwmv2_set_trigout_cmp_index(HPM_PWM0, pwm_channel_0, 24);
}
