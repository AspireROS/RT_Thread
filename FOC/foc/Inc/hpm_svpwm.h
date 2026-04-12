//
// Created by 21184 on 2026/3/24.
//

#ifndef RTTHREAD_HPM_SVPWM_H
#define RTTHREAD_HPM_SVPWM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {



#endif

/**
 * @brief SVPWM 计算结构体
 */
typedef struct {
    /* 输入量 */
    float Valpha; // alpha轴目标电压 (来自反Park变换)
    float Vbeta; // beta轴目标电压 (来自反Park变换)
    float Vdc; // 当前母线电压 (用于归一化占空比)
    float pwm_period_max; // 硬件 PWM 定时器的最大重载值 (如计数值)

    /* 输出量 (写入 PWM 硬件比较寄存器的值) */
    uint32_t cmp_a; // A相比较值
    uint32_t cmp_b; // B相比较值
    uint32_t cmp_c; // C相比较值
} svpwm_t;

/**
 * @brief SVPWM 计算，生成三相占空比比较值
 * @param pHandle SVPWM 结构体句柄
 */
void hpm_svpwm_calc(svpwm_t *pHandle);

#ifdef __cplusplus
}
#endif

#endif //RTTHREAD_HPM_SVPWM_H
