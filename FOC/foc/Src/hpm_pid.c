//
// Created by 21184 on 2026/3/24.
//
#include "../Inc/hpm_pid.h"

/**
 * @brief PID 控制器初始化
 */
void hpm_pid_init(foc_pid_t *pHandle, float kp, float ki, float max_out) {
    pHandle->Kp = kp;
    pHandle->Ki = ki;
    pHandle->integral = 0.0f;
    pHandle->out_max = max_out;
    pHandle->out_min = -max_out;
}

/**
 * @brief PID 计算 (带积分限幅)
 * @param pHandle PID 结构体句柄
 * @param ref 目标值
 * @param fdb 反馈值
 * @return 控制器输出
 */
float hpm_pid_calc(foc_pid_t *pHandle, float ref, float fdb) {
    float error = ref - fdb;
    float output;

    // 比例项
    float up = pHandle->Kp * error;

    // 积分项
    pHandle->integral += pHandle->Ki * error;

    // 积分抗饱和 (Anti-windup)
    if (pHandle->integral > pHandle->out_max) {
        pHandle->integral = pHandle->out_max;
    } else if (pHandle->integral < pHandle->out_min) {
        pHandle->integral = pHandle->out_min;
    }

    // 计算总输出
    output = up + pHandle->integral;

    // 输出限幅
    if (output > pHandle->out_max) {
        output = pHandle->out_max;
    } else if (output < pHandle->out_min) {
        output = pHandle->out_min;
    }

    pHandle->out = output;
    return output;
}
