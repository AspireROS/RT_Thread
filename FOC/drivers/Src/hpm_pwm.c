#include "../Inc/hpm_pwm.h"
#include <rtthread.h>
#include "hpm_gpio_drv.h"
#include "hpm_pwmv2_drv.h"
#include "hpm_clock_drv.h"
#include "hpm_ioc_regs.h"

#define PWM_FREQUENCY (20000) // 20kHz

/* 外部声明时钟初始化函数 */
extern uint32_t rtt_board_init_pwm_clock(PWMV2_Type *ptr);

static void pwm_pin_init(void) {
    /* PWM0 P6, P7 (PD06, PD07) */
    HPM_IOC->PAD[IOC_PAD_PD06].FUNC_CTL = IOC_PD06_FUNC_CTL_PWM0_P_6;
    HPM_IOC->PAD[IOC_PAD_PD07].FUNC_CTL = IOC_PD07_FUNC_CTL_PWM0_P_7;

    /* PWM1 P0, P1 (PC08, PC09) */
    HPM_IOC->PAD[IOC_PAD_PC08].FUNC_CTL = IOC_PC08_FUNC_CTL_PWM1_P_0;
    HPM_IOC->PAD[IOC_PAD_PC09].FUNC_CTL = IOC_PC09_FUNC_CTL_PWM1_P_1;

    /* PWM1 P6, P7 (PC14, PC15) */
    HPM_IOC->PAD[IOC_PAD_PC14].FUNC_CTL = IOC_PC14_FUNC_CTL_PWM1_P_6;
    HPM_IOC->PAD[IOC_PAD_PC15].FUNC_CTL = IOC_PC15_FUNC_CTL_PWM1_P_7;
}

/**
 * 配置互补输出
 * @param pwm_base PWM基地址
 * @param channel_index P通道索引 (如6代表6和7组成互补对)
 * @param reload 重装载值 (周期)
 * @param duty_count 占空比计数值
 */
static void configure_complementary_pwm(PWMV2_Type *pwm_base, uint8_t channel_index, uint32_t reload,
                                        uint32_t duty_count) {
    pwmv2_cmp_config_t cmp_config = {0};

    /* 1. 解锁影子寄存器，允许更新配置 */
    pwmv2_shadow_register_unlock(pwm_base);

    /* 2. 设置周期 (影子寄存器0) */
    pwmv2_set_shadow_val(pwm_base, 0, reload, 0, false);

    /* 3. 设置 P 和 N 通道的比较值实现占空比 */
    cmp_config.cmp = duty_count;
    cmp_config.update_trigger = pwm_shadow_register_update_on_reload;
    pwmv2_config_cmp(pwm_base, channel_index, &cmp_config); /* P channel */
    pwmv2_config_cmp(pwm_base, channel_index + 1, &cmp_config); /* N channel */

    /* 4. 配置互补极性 */
    /* P通道 (偶数) 不反相 */
    pwmv2_disable_output_invert(pwm_base, (pwm_channel_t) channel_index);
    /* N通道 (奇数) 反相，实现互补 */
    pwmv2_enable_output_invert(pwm_base, (pwm_channel_t) (channel_index + 1));

    /* 5. 开启 Pair 模式 (实现 P/N 联动) */
    pwmv2_enable_pair_mode(pwm_base, (pwm_channel_t) channel_index);

    /* 6. 设置死区 (100个时钟周期，约 0.5us @ 200MHz) */
    /* 在 PWMV2 中，死区是基于通道设置的 */
    pwmv2_set_dead_area(pwm_base, (pwm_channel_t) channel_index, 100);
    pwmv2_set_dead_area(pwm_base, (pwm_channel_t) (channel_index + 1), 100);

    /* 7. 使能通道输出 */
    pwmv2_channel_enable_output(pwm_base, (pwm_channel_t) channel_index);
    pwmv2_channel_enable_output(pwm_base, (pwm_channel_t) (channel_index + 1));

    /* 8. 锁定/同步影子寄存器，使配置生效 */
    pwmv2_issue_shadow_register_lock_event(pwm_base);
}

void hpm_pwm_complementary_init(void) {
    uint32_t pwm_clk_freq;
    uint32_t reload;
    uint32_t duty;

    pwm_pin_init();

    /* --- PWM0 配置 --- */
    clock_add_to_group(clock_pwm0, 0);
    pwm_clk_freq = rtt_board_init_pwm_clock(HPM_PWM0);
    reload = pwm_clk_freq / PWM_FREQUENCY - 1;
    duty = reload / 2; // 50% 占空比

    pwmv2_disable_counter(HPM_PWM0, pwm_counter_0);
    pwmv2_reset_counter(HPM_PWM0, pwm_counter_0);

    /* 配置 PWM0 CH6 & CH7 */
    configure_complementary_pwm(HPM_PWM0, 6, reload, duty);

    /* 启动 PWM0 计数器 0 */
    pwmv2_start_pwm_output(HPM_PWM0, pwm_counter_0);
    pwmv2_enable_counter(HPM_PWM0, pwm_counter_0);

    /* --- PWM1 配置 --- */
    clock_add_to_group(clock_pwm1, 0);
    pwm_clk_freq = rtt_board_init_pwm_clock(HPM_PWM1);
    reload = pwm_clk_freq / PWM_FREQUENCY - 1;
    duty = reload / 2;

    pwmv2_disable_counter(HPM_PWM1, pwm_counter_0);
    pwmv2_reset_counter(HPM_PWM1, pwm_counter_0);

    /* 配置 PWM1 CH0 & CH1 */
    configure_complementary_pwm(HPM_PWM1, 0, reload, duty);
    /* 配置 PWM1 CH6 & CH7 */
    configure_complementary_pwm(HPM_PWM1, 6, reload, duty);

    /* 启动 PWM1 计数器 0 */
    pwmv2_start_pwm_output(HPM_PWM1, pwm_counter_0);
    pwmv2_enable_counter(HPM_PWM1, pwm_counter_0);
}

void hpm_pwm_set_valley_trigger(void) {
    /* 谷底触发 (Valley Trigger)
     * 在 PWMV2 中，选择 Counter 0 的 Reload 点 (索引 24) 作为触发输出
     * 将 Reload 事件映射到 TRGO_0 (Trigger Out 0)
     */
    pwmv2_set_trigout_cmp_index(HPM_PWM0, pwm_channel_0, 24);
}
