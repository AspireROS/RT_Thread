//
// Created by 21184 on 2026/3/24.
//

#ifndef RTTHREAD_HPM_FOC_H
#define RTTHREAD_HPM_FOC_H

#include <stdint.h>
#include "hpm_clarke.h"
#include "hpm_park.h"
#include "hpm_pid.h"
#include "hpm_svpwm.h"

#ifdef __cplusplus
extern "C" {


#endif

/**
 * @brief FOC 控制器状态与参数结构体
 */
typedef struct {
    clarke_t clarke;
    park_t park;
    park_t inv_park;
    foc_pid_t pid_d;
    foc_pid_t pid_q;
    svpwm_t svpwm;

    float theta_mech; // 机械角度 (rad)
    float theta_elec; // 电角度 (rad)
    uint8_t pole_pairs; // 电机极对数

    float target_Id; // 目标 Id (A)
    float target_Iq; // 目标 Iq (A)

    float phase_a_current; // 反馈相电流 A
    float phase_b_current; // 反馈相电流 B
    float phase_c_current; // 反馈相电流 C
} foc_controller_t;

extern foc_controller_t foc_ctrl;

/**
 * @brief 初始化 FOC 控制器
 * @param pole_pairs 电机极对数
 * @param vdc 母线电压 (V)
 */
void hpm_foc_init(uint8_t pole_pairs, float vdc);

/**
 * @brief 设置目标电流 (D/Q 轴)
 */
void hpm_foc_set_target(float id, float iq);

/**
 * @brief FOC 主循环单步执行 (需在定时器或 ADC 中断中定期调用)
 * @param ia 相电流 A
 * @param ib 相电流 B
 * @param ic 相电流 C
 */
void hpm_foc_step(float ia, float ib, float ic);

#ifdef __cplusplus
}
#endif

#endif //RTTHREAD_HPM_FOC_H
