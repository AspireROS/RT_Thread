//
// Created by 21184 on 2026/3/17.
//

#ifndef RTTHREAD_HPM_PWM_H
#define RTTHREAD_HPM_PWM_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/**
 * @brief 初始化 3相 PWM 输出 (使用 PWM0 CH0-CH5), 设置40ns死区时间
 */
void hpm_pwm_foc_init(void);

/**
 * @brief 获取 PWM 的周期计数值 (PWM 重载值)
 */
uint32_t hpm_pwm_get_period(void);

/**
 * @brief 更新三相占空比寄存器
 * @param cmp_a 相 A 的比较值 (通常对应 CH0/1)
 * @param cmp_b 相 B 的比较值 (通常对应 CH2/3)
 * @param cmp_c 相 C 的比较值 (通常对应 CH4/5)
 */
void hpm_pwm_set_duty(uint32_t cmp_a, uint32_t cmp_b, uint32_t cmp_c);

/**
 * @brief Configure PWM valley trigger to TRGO_0 (ADC Trigger)
 */
void hpm_pwm_set_valley_trigger(void);

#endif //RTTHREAD_HPM_PWM_H
